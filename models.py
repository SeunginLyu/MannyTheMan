"""
This module contains all abstract models of the body.
Joint, Skeleton
"""

# For now let's limit the possible angles to 0, 45, 90, 135, 180
# [left_arm, left_shoulder, right_shoulder, left_arm]
# Basic skeleton with only four joints o-o-o-o

sample_names = {0: "left_arm",
                1: "left_shoulder",
                2: "right_shoulder",
                3: "right_arm"}

sample_links = {0: [1],
                1: [0, 2],
                2: [1, 3],
                3: [2]}

sample_servos = {0: "outPinA",
                 1: "outPinB",
                 2: "outPinC",
                 3: "outPinD"}

sample_motions = [[90, 90, 90, 90],
                  [45, 90, 90, 45],
                  [135, 90, 90, 135],
                  [135, 90, 90, 45],
                  [90, 0, 0, 90],
                  [90, 180, 180, 90]]


class Joint:
    """

    """
    def __init__(self, name, servo, angle=0):
        self.name = name
        self.servo = servo
        self.angle = angle

    def updateAngle(self, angle):
        self.angle = angle

    def updateName(self, name):
        self.name = name

    def __str__(self):
        return self.name + ", " + self.servo + ", " + str(self.angle)


class Skeleton:
    """
    """
    def __init__(self, jointNames, jointLinks, jointServos):
        """
        jointNum = number of joints
        jointLinks = dictionary that contains linkage between joints
        jointNames = dictionary that maps names to each joint
        """

        self.jointNames = jointNames
        self.jointLinks = jointLinks
        self.jointNum = len(jointLinks.keys())
        self.jointServos = jointServos

        self.joints = []
        for i in range(self.jointNum):
            newJoint = Joint(jointNames[i], jointServos[i])
            self.joints.append(newJoint)

    def actuateMotion(self, motion):
        for i, angle in enumerate(motion):
            self.joints[i].updateAngle(angle)
