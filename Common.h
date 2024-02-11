#ifndef COMMON
#define COMMON

#include <string>
#include "Definition.h"
#include "Humanoid.h"

namespace GlobalVariable {
  const std::string strFileName("Joint Load" + std::string(FILE_EXTENSION));
  const dReal dInterval = 1.0E-2, dForceLimit = 10.0E2, dJointRadius = 2.5E-2;
  const int iContactLimit = static_cast<int>(1.0E4), iRangeCount = 2, iSimulationRate = 80;

  extern dWorldID wdWorld;
  extern dSpaceID spcSpace;
  extern dGeomID geomGround;
  extern dJointGroupID jgContactGroup;
  extern Humanoid HumanoidData;
  extern dReal mdRootAngle[COMPONENT_COUNT_3D - 1], mdHeadPosition[COMPONENT_COUNT_3D], mdHeadAngle[Head::iJointCount], mdTorsoPosition[COMPONENT_COUNT_3D], mdTorsoAngle[Torso::iJointCount - 1];
  extern dReal mdArmPosition[Humanoid::iArmCount][COMPONENT_COUNT_3D], mdArmAngle[Humanoid::iArmCount][Arm::iJointCount - 1], mdLegPosition[Humanoid::iLegCount][COMPONENT_COUNT_3D], mdLegPosture[Humanoid::iLegCount][COMPONENT_COUNT_3D], mdLegAngle[Humanoid::iLegCount][Leg::iJointCount];
  extern dReal dTotalMagnitude;
  extern eDisplayMode eDisplayModeSetting;
  extern dsFunctions fnFunction;
}

#endif