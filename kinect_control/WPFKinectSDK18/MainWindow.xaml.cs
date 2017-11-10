using Microsoft.Kinect;
using Microsoft.Kinect.Toolkit;
using System;
using System.Threading;
using System.IO.Ports;
using System.IO;
using System.Globalization;
using System.Linq;
using System.Windows;
using System.Windows.Media;
using System.IO.Ports;

namespace WPFKinectSDK18
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
   
    public partial class MainWindow : Window
    {
        /// <summary>
        /// Active Kinect sensor
        /// </summary>
        private KinectSensor sensor;
        SerialPort port = new SerialPort("COM4", 9600);
        public MainWindow()
        {
            InitializeComponent();
            port.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
            port.Open();
            Loaded += MainWindowLoaded;
        }
        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            string message = sp.ReadLine();
            //System.Diagnostics.Debug.Write("received:" + message);
        }
        private void MainWindowLoaded(object sender, RoutedEventArgs e)
        {
            var sensorStatus = new KinectSensorChooser();

            sensorStatus.KinectChanged += KinectSensorChooserKinectChanged;

            kinectChooser.KinectSensorChooser = sensorStatus;
            sensorStatus.Start();

        }

        private void KinectSensorChooserKinectChanged(object sender, KinectChangedEventArgs e)
        {
            
            if (sensor != null)
                sensor.SkeletonFrameReady -= KinectSkeletonFrameReady;

            sensor = e.NewSensor;

            if (sensor == null)
                return;

            switch (Convert.ToString(e.NewSensor.Status))
            {
                case "Connected": KinectStatus.Content = "Connected";
                    break;
                case "Disconnected": KinectStatus.Content = "Disconnected";
                    break;
                case "Error": KinectStatus.Content = "Error";
                    break;
                case "NotReady": KinectStatus.Content = "Not Ready";
                    break;
                case "NotPowered": KinectStatus.Content = "Not Powered";
                    break;
                case "Initializing": KinectStatus.Content = "Initialising";
                    break;
                default: KinectStatus.Content = "Undefined";
                    break;
            }

            sensor.SkeletonStream.Enable(); 
            sensor.SkeletonFrameReady += KinectSkeletonFrameReady;

        }


        private void SensorDepthFrameReady(object sender, DepthImageFrameReadyEventArgs e)
        {
            throw new NotImplementedException();
        }

        private void KinectSkeletonFrameReady(object sender, SkeletonFrameReadyEventArgs e)
        {
            var skeletons = new Skeleton[0];

            using (var skeletonFrame = e.OpenSkeletonFrame())
            {
                if (skeletonFrame != null)
                {
                    skeletons = new Skeleton[skeletonFrame.SkeletonArrayLength];
                    skeletonFrame.CopySkeletonDataTo(skeletons);
                }
            }

            if (skeletons.Length == 0)
            {
                return;
            }

            var skel = skeletons.FirstOrDefault(x => x.TrackingState == SkeletonTrackingState.Tracked);
            if (skel == null)
            {
                return;
            }

            var leftHand = skel.Joints[JointType.WristLeft]; // 1
            var rightHand = skel.Joints[JointType.WristRight]; // 2
            var leftElbow = skel.Joints[JointType.ElbowLeft]; // 3
            var rightElbow = skel.Joints[JointType.ElbowRight]; // 4
            var leftKnee= skel.Joints[JointType.KneeLeft]; // 5
            var rightKnee = skel.Joints[JointType.KneeRight]; // 6
            var leftAnkle = skel.Joints[JointType.AnkleLeft]; // 7
            var rightAnkle = skel.Joints[JointType.AnkleRight]; // 8

            var centreHip = skel.Joints[JointType.HipCenter];
            var centreShoulder = skel.Joints[JointType.ShoulderCenter];

            // var upperJoints = new[] {leftHand, rightHand, leftElbow, rightElbow};
            var upperJoints = new[] {leftHand, leftElbow};
            // var lowerJoints = new[] {leftKnee, rightKnee, leftAnkle, rightAnkle};

            XValueRight.Text = rightHand.Position.X.ToString(CultureInfo.InvariantCulture);
            YValueRight.Text = rightHand.Position.Y.ToString(CultureInfo.InvariantCulture);
            ZValueRight.Text = rightHand.Position.Z.ToString(CultureInfo.InvariantCulture);

            XValueLeft.Text = leftHand.Position.X.ToString(CultureInfo.InvariantCulture);
            YValueLeft.Text = leftHand.Position.Y.ToString(CultureInfo.InvariantCulture);
            ZValueLeft.Text = leftHand.Position.Z.ToString(CultureInfo.InvariantCulture);

            int jointcnt = 0;
            foreach (var joint in upperJoints)
            {
                jointcnt++; 
                int angle = 0;
                if(jointcnt == 1){ // lefthand, relative to leftElbow
                    double radians = Math.Atan((joint.Position.Y - leftElbow.Position.Y)/(joint.Position.X - leftElbow.Position.X));
                    angle = (int)(radians * (180/Math.PI) + 90);
                }
                if(jointcnt == 2){ // leftelbow, relative to centreShoulder
                    double radians = Math.Atan((joint.Position.Y - centreShoulder.Position.Y)/(joint.Position.X - centreShoulder.Position.X));
                    angle = (int)(radians * (180/Math.PI) + 90);
                }                                  
                port.WriteLine(jointcnt.ToString() + ',' + angle.ToString());  // 1,90        
                System.Diagnostics.Debug.WriteLine(jointcnt.ToString() + ',' + angle.ToString());
            }
        }
    }
}