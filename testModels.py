import models

outPinA = 2
outPinB = 3
outPinC = 4
outPinD = 7

skeleton = models.Skeleton(models.sample_names,
                           models.sample_links,
                           models.sample_servos)

skeleton.actuateMotion(models.sample_motions[1])
print(skeleton.joints[0])
