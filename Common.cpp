#include "Common.h"

namespace GlobalVariable {
  dWorldID wdWorld;
  dSpaceID spcSpace;
  dGeomID geomGround;
  dJointGroupID jgContactGroup;
  Humanoid HumanoidData;
  dReal mdRootAngle[COMPONENT_COUNT_3D - 1] = {0.0}, mdHeadPosition[COMPONENT_COUNT_3D] = {0.0}, mdHeadAngle[Head::iJointCount] = {0.0}, mdTorsoPosition[COMPONENT_COUNT_3D] = {0.0}, mdTorsoAngle[COMPONENT_COUNT_3D];
  dReal mdArmPosition[Humanoid::iArmCount][COMPONENT_COUNT_3D] = {
    {0.0}
  }, mdArmAngle[Humanoid::iArmCount][Arm::iJointCount - 1] = {
    {0.0}
  }, mdLegPosition[Humanoid::iLegCount][COMPONENT_COUNT_3D] = {
    {0.0}
  }, mdLegPosture[Humanoid::iLegCount][COMPONENT_COUNT_3D] = {
    {0.0}
  }, mdLegAngle[Humanoid::iLegCount][Leg::iJointCount] = {
    {0.0}
  };
  dReal dTotalMagnitude;
  eDisplayMode eDisplayModeSetting = Polyhedron;
  dsFunctions fnFunction;
}