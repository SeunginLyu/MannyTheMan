using Microsoft.Kinect;
using Microsoft.Kinect.Toolkit;
using System;
using System.Threading;
using System.Collections.Generic;
using System.Data;
using System.IO.Ports;
using System.IO;
using System.Globalization;
using System.Linq;
using System.Windows;
using System.Management;
using System.Windows.Media;
using System.IO.Ports;
using System.Diagnostics;


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

        SerialPort port = new SerialPort(DetectArduino());
       
        bool responseReceived = true;
        int lenSent = 0;
        public MainWindow()
        {
         
            InitializeComponent();
            PortLabel.Text += " PORT : " + port.PortName;
            if(port.PortName != "NOPORT")
            {
                port.BaudRate = 9600;
                port.DataBits = 8;
                port.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
                port.Open();
                port.DiscardOutBuffer();
                port.DiscardInBuffer();
            }
            else
            {
                MessageBoxResult result = MessageBox.Show("NO ARDUINO PORT FOUND");
            }
            
            Loaded += MainWindowLoaded;
        }
        public static string DetectArduino()
        {
            ManagementScope connectionScope = new ManagementScope();
            SelectQuery serialQuery = new SelectQuery("SELECT * FROM Win32_SerialPort");
            ManagementObjectSearcher searcher = new ManagementObjectSearcher(connectionScope, serialQuery);
            try
            {
                foreach (ManagementObject item in searcher.Get())
                {
                    string desc = item["Description"].ToString();
                    string deviceId = item["DeviceID"].ToString();
                    if (desc.Contains("Arduino"))
                    {
                        return deviceId;
                    }
                }
            }
            catch (ManagementException e)
            {
                /* Do Nothing */
            }
            return "NOPORT";
        }
        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            string message = sp.ReadLine();
            System.Threading.Thread.Sleep(500); // waits for the full serial input
            System.Diagnostics.Debug.WriteLine("received:" + message);
            // SerialMonitor.Text += "received:" + message + Environment.NewLine;
            responseReceived = true; 
            port.DiscardOutBuffer();
            port.DiscardInBuffer();
                                                       
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
            if(responseReceived == true){
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

                var allJoints = new[] {leftHand, rightHand, leftElbow, rightElbow, leftAnkle, leftKnee, rightAnkle, rightKnee};

                //XValueRight.Text = rightHand.Position.X.ToString(CultureInfo.InvariantCulture);
                // YValueRight.Text = rightHand.Position.Y.ToString(CultureInfo.InvariantCulture);
                // ZValueRight.Text = rightHand.Position.Z.ToString(CultureInfo.InvariantCulture);

                // XValueLeft.Text = leftHand.Position.X.ToString(CultureInfo.InvariantCulture);
                // YValueLeft.Text = leftHand.Position.Y.ToString(CultureInfo.InvariantCulture);
                // ZValueLeft.Text = leftHand.Position.Z.ToString(CultureInfo.InvariantCulture);

                int jointcnt = 0;
                string res ="";
                foreach (var joint in allJoints)
                {
                    jointcnt++; 
                    int angle = 0;
                    if(jointcnt == 1 ){ // lefthand, relative to leftElbow
                        double radians = Math.Atan((joint.Position.Y - leftElbow.Position.Y)/(joint.Position.X - leftElbow.Position.X));
                        angle = (int)(radians * (180/Math.PI) + 90); // 90 is the default angle(horizontal)
                    }
                    if (jointcnt == 2) // righthand
                    { // righthand
                        double radians = Math.Atan((joint.Position.Y - rightElbow.Position.Y) / (joint.Position.X - rightElbow.Position.X));
                        angle = (int)(radians * (180 / Math.PI) + 90); // 90 is the default angle(horizontal)
                    }
                    if (jointcnt == 3){ // leftelbow, relative to centreShoulder
                        double radians = Math.Atan((joint.Position.Y - centreShoulder.Position.Y)/(joint.Position.X - centreShoulder.Position.X));
                        angle = (int)(radians * (180/Math.PI) + 90); // 90 is the default angle(horizontal)
                    }
                    if(jointcnt == 4){ // right elbow
                        double radians = Math.Atan((joint.Position.Y - centreShoulder.Position.Y)/(joint.Position.X - centreShoulder.Position.X));
                        angle = (int)(radians * (180/Math.PI) + 90); // 90 is the default angle(horizontal)
                    }

                    if(jointcnt == 5){
                        double radians = Math.Atan((joint.Position.Y - leftKnee.Position.Y)/(joint.Position.X - leftKnee.Position.X));
                        angle = (int)(radians * (180/Math.PI) + 90); // 90 is the default angle(horizontal)
                    }
                    if(jointcnt == 6){
                        double radians = Math.Atan((joint.Position.Y - centreHip.Position.Y)/(joint.Position.X - centreHip.Position.X));
                        angle = (int)(radians * (180/Math.PI) + 90); // 90 is the default angle(horizontal)
                    }
                    if(jointcnt == 7){
                        double radians = Math.Atan((joint.Position.Y - rightKnee.Position.Y)/(joint.Position.X - rightKnee.Position.X));
                        angle = (int)(radians * (180/Math.PI) + 90); // 90 is the default angle(horizontal)
                    }
                    if(jointcnt == 8){
                        double radians = Math.Atan((joint.Position.Y - centreHip.Position.Y)/(joint.Position.X - centreHip.Position.X));
                        angle = (int)(radians * (180/Math.PI) + 90); // 90 is the default angle(horizontal)
                    }
                    res = res + jointcnt.ToString() + ',' + angle.ToString() + ','; 
                }
                lenSent = res.Length;
                port.WriteLine(res);  // 1,90       
                System.Diagnostics.Debug.WriteLine("sending.." + res);

                SerialMonitor.Text += "   sent:" + res + Environment.NewLine;
                responseReceived = false;
            }                                                
        }
    }
}