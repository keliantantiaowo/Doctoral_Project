#include <stdexcept>
#include "Common.h"
#include "Pose Control.h"
#include "Tool.h"

void InitializeAngle(void)
{
  for (int i = 0; i < Head::iJointCount; i++)
    GlobalVariable::mdHeadAngle[i] = MotionRange(Head, Lower, i) + std::fabs(MotionRange(Head, Upper, i) - MotionRange(Head, Lower, i)) * Randomize();

  for (int i = 0; i < Torso::iJointCount - 1; i++)
    GlobalVariable::mdTorsoAngle[i] = MotionRange(Torso, Lower, i) + std::fabs(MotionRange(Torso, Upper, i) - MotionRange(Torso, Lower, i)) * Randomize();

  for (int i = 0; i < Humanoid::iArmCount; i++)
    for (int j = 0; j < Arm::iJointCount - 1; j++)
      GlobalVariable::mdArmAngle[i][j] = MotionRange(Arm, Lower, !i ? Left : Right, j) + std::fabs(MotionRange(Arm, Upper, !i ? Left : Right, j) - MotionRange(Arm, Lower, !i ? Left : Right, j)) * Randomize();

  for (int i = 0; i < Humanoid::iLegCount; i++)
    for (int j = 0; j < Leg::iJointCount; j++)
      GlobalVariable::mdLegAngle[i][j] = MotionRange(Leg, Lower, !i ? Left : Right, j) + std::fabs(MotionRange(Leg, Upper, !i ? Left : Right, j) - MotionRange(Leg, Lower, !i ? Left : Right, j)) * Randomize();

  GlobalVariable::mdRootAngle[SEGMENT_A] = std::acos(1.0 - Randomize() * 2.0);
  GlobalVariable::mdRootAngle[SEGMENT_B] = Randomize() * M_PI * 2.0;
}

void CalculateAngle(void)
{
  InverseKinematics();

  for (int i = 0; i < Humanoid::iArmCount; i++)
    InverseKinematics(GlobalVariable::mdArmPosition[i], GlobalVariable::mdArmAngle[i], !i ? Left : Right);

  for (int i = 0; i < Humanoid::iLegCount; i++)
    InverseKinematics(GlobalVariable::mdLegPosition[i], GlobalVariable::mdLegPosture[i], GlobalVariable::mdLegAngle[i], !i ? Left : Right);
}

void ConstrainAngle(void)
{
  for (int i = 0; i < Head::iPartCount + Head::iDummyCount; i++)
    SustainState(GlobalVariable::HumanoidData.GetHead().GetObject(i).GetBody());

  for (int i = 0; i < Torso::iPartCount + Torso::iDummyCount; i++)
    SustainState(GlobalVariable::HumanoidData.GetTorso().GetObject(i).GetBody());

  for (int i = 0; i < Humanoid::iArmCount; i++)
    for (int j = 0; j < Arm::iPartCount + Arm::iDummyCount; j++)
      SustainState(GlobalVariable::HumanoidData.GetArm(i).GetObject(j).GetBody());

  for (int i = 0; i < Humanoid::iLegCount; i++)
    for (int j = 0; j < Leg::iPartCount + Leg::iDummyCount; j++)
      SustainState(GlobalVariable::HumanoidData.GetLeg(i).GetObject(j).GetBody());

  RestrictPosture();
/*
  for (int i = 0; i < Head::iJointCount; i++)
    SetJointParameter(GlobalVariable::HumanoidData.GetHead().GetJoint(i), GlobalVariable::mdHeadAngle[i]);

  for (int i = 0; i < Torso::iJointCount - 1; i++)
    SetJointParameter(GlobalVariable::HumanoidData.GetTorso().GetJoint(i), GlobalVariable::mdTorsoAngle[i]);

  for (int i = 0; i < Humanoid::iArmCount; i++)
    for (int j = 0; j < Arm::iJointCount - 1; j++)
      SetJointParameter(GlobalVariable::HumanoidData.GetArm(i).GetJoint(j), GlobalVariable::mdArmAngle[i][j]);

  for (int i = 0; i < Humanoid::iLegCount; i++)
    for (int j = 0; j < Leg::iJointCount; j++)
      SetJointParameter(GlobalVariable::HumanoidData.GetLeg(i).GetJoint(j), GlobalVariable::mdLegAngle[i][j]);
*/
}

void InverseKinematics(void)
{
  dVector3 vecPosition;

  dBodyVectorToWorld(GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_B).GetBody(), 0.0, 0.0, -1.0, vecPosition);
  dBodyVectorFromWorld(GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_A).GetBody(), vecPosition[OFFSET_X], vecPosition[OFFSET_Y], vecPosition[OFFSET_Z], vecPosition);

  GlobalVariable::mdTorsoAngle[OFFSET_X] = HandleSingularPoint(dJointGetHingeAngle(GlobalVariable::HumanoidData.GetTorso().GetJoint(SEGMENT_A)), std::atan2(-vecPosition[OFFSET_Y], -vecPosition[OFFSET_Z]));
  GlobalVariable::mdTorsoAngle[OFFSET_Y] = HandleSingularPoint(dJointGetHingeAngle(GlobalVariable::HumanoidData.GetTorso().GetJoint(SEGMENT_B)), std::atan2(vecPosition[OFFSET_X], std::sqrt(std::pow(vecPosition[OFFSET_Y], 2.0) + std::pow(vecPosition[OFFSET_Z], 2.0))));

  dBodyVectorToWorld(GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_A).GetBody(), std::cos(GlobalVariable::mdTorsoAngle[OFFSET_Y]), std::sin(GlobalVariable::mdTorsoAngle[OFFSET_X]) * std::sin(GlobalVariable::mdTorsoAngle[OFFSET_Y]), std::cos(GlobalVariable::mdTorsoAngle[OFFSET_X]) * std::sin(GlobalVariable::mdTorsoAngle[OFFSET_Y]), vecPosition);
  dBodyVectorFromWorld(GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_B).GetBody(), vecPosition[OFFSET_X], vecPosition[OFFSET_Y], vecPosition[OFFSET_Z], vecPosition);

  GlobalVariable::mdTorsoAngle[OFFSET_Z] = HandleSingularPoint(dJointGetHingeAngle(GlobalVariable::HumanoidData.GetTorso().GetJoint(SEGMENT_C)), std::atan2(-vecPosition[OFFSET_Y], vecPosition[OFFSET_X]));
}

void InverseKinematics(const dReal* const& rpdTipPosition, dReal* const& rpdAngle, const eSide& reSideSetting)
{
  static const dReal dArmLength = dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_A)).GetLength() + dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_B)).GetLength() + dynamic_cast<const Sphere&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_C)).GetRadius();

  dReal mdNormalizedTipPosition[COMPONENT_COUNT_3D] = {rpdTipPosition[OFFSET_X], rpdTipPosition[OFFSET_Y], rpdTipPosition[OFFSET_Z]}, dSquaredDistance = 0.0, dDistance = 0.0;

  if (CheckLimit(mdNormalizedTipPosition, dArmLength)) {
    dSquaredDistance = std::pow(dArmLength, 2.0);
    dDistance = dArmLength;
  } else {
    dSquaredDistance = std::pow(mdNormalizedTipPosition[OFFSET_X], 2.0) + std::pow(mdNormalizedTipPosition[OFFSET_Y], 2.0) + std::pow(mdNormalizedTipPosition[OFFSET_Z], 2.0);
    dDistance = std::sqrt(dSquaredDistance);
  }

  dVector3 vecPosition;

  dBodyVectorFromWorld(GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_A).GetBody(), mdNormalizedTipPosition[OFFSET_X], mdNormalizedTipPosition[OFFSET_Y], mdNormalizedTipPosition[OFFSET_Z], vecPosition);

  dReal dCosineA = (std::pow(dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_A)).GetLength(), 2.0) - std::pow(dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_B)).GetLength() + dynamic_cast<const Sphere&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_C)).GetRadius(), 2.0) + dSquaredDistance) / (dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_A)).GetLength() * dDistance * 2.0), dCosineB = (std::pow(dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_A)).GetLength(), 2.0) + std::pow(dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_B)).GetLength() + dynamic_cast<const Sphere&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_C)).GetRadius(), 2.0) - dSquaredDistance) / (dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_A)).GetLength() * (dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_B)).GetLength() + dynamic_cast<const Sphere&>(GlobalVariable::HumanoidData.GetArm(int()).GetObject(SEGMENT_C)).GetRadius()) * 2.0);

  switch (reSideSetting) {
  case Left :
    rpdAngle[SEGMENT_A] = HandleSingularPoint(dJointGetHingeAngle(GlobalVariable::HumanoidData.GetArm(SEGMENT_A).GetJoint(SEGMENT_A)), std::atan2(-vecPosition[OFFSET_Z], vecPosition[OFFSET_Y]));
    rpdAngle[SEGMENT_B] = HandleSingularPoint(dJointGetHingeAngle(GlobalVariable::HumanoidData.GetArm(SEGMENT_A).GetJoint(SEGMENT_B)), std::atan2(vecPosition[OFFSET_X], std::sqrt(std::pow(vecPosition[OFFSET_Y], 2.0) + std::pow(vecPosition[OFFSET_Z], 2.0))) - std::atan2(std::sqrt(1.0 - std::pow(dCosineA, 2.0)), dCosineA));

    rpdAngle[SEGMENT_C] = 0.0;
    rpdAngle[SEGMENT_D] = -(std::atan2(std::sqrt(1.0 - std::pow(dCosineB, 2.0)), dCosineB) - M_PI);

    break;
  case Right :
    rpdAngle[SEGMENT_A] = HandleSingularPoint(dJointGetHingeAngle(GlobalVariable::HumanoidData.GetArm(SEGMENT_B).GetJoint(SEGMENT_A)), std::atan2(vecPosition[OFFSET_Z], -vecPosition[OFFSET_Y]));
    rpdAngle[SEGMENT_B] = HandleSingularPoint(dJointGetHingeAngle(GlobalVariable::HumanoidData.GetArm(SEGMENT_B).GetJoint(SEGMENT_B)), -(std::atan2(vecPosition[OFFSET_X], std::sqrt(std::pow(vecPosition[OFFSET_Y], 2.0) + std::pow(vecPosition[OFFSET_Z], 2.0))) - std::atan2(std::sqrt(1.0 - std::pow(dCosineA, 2.0)), dCosineA)));
    rpdAngle[SEGMENT_C] = 0.0;
    rpdAngle[SEGMENT_D] = std::atan2(std::sqrt(1.0 - std::pow(dCosineB, 2.0)), dCosineB) - M_PI;

    break;
  default : throw(std::invalid_argument("不正のサイド情報が入力されました。"));
  }
}

void InverseKinematics(const dReal* const& rpdTipPosition, const dReal* const& rpdTipPosture, dReal* const& rpdAngle, const eSide& reSideSetting)
{
  static const dReal dLegLength = dynamic_cast<const Cylinder&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_A)).GetLength() + dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_B)).GetLength();

  dVector3 vecPosition, vecDirection;

  dReal mdNormalizedTipPosition[COMPONENT_COUNT_3D] = {rpdTipPosition[OFFSET_X] - dynamic_cast<const Box&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_Z) * rpdTipPosture[OFFSET_X], rpdTipPosition[OFFSET_Y] - dynamic_cast<const Box&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_Z) * rpdTipPosture[OFFSET_Y], rpdTipPosition[OFFSET_Z] - dynamic_cast<const Box&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_Z) * rpdTipPosture[OFFSET_Z]}, dSquaredDistance = 0.0, dDistance = 0.0;

  if (CheckLimit(mdNormalizedTipPosition, dLegLength)) {
    dSquaredDistance = std::pow(dLegLength, 2.0);
    dDistance = dLegLength;
  } else {
    dSquaredDistance = std::pow(mdNormalizedTipPosition[OFFSET_X], 2.0) + std::pow(mdNormalizedTipPosition[OFFSET_Y], 2.0) + std::pow(mdNormalizedTipPosition[OFFSET_Z], 2.0);
    dDistance = std::sqrt(dSquaredDistance);
  }

  dBodyVectorFromWorld(GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_B).GetBody(), mdNormalizedTipPosition[OFFSET_X], mdNormalizedTipPosition[OFFSET_Y], mdNormalizedTipPosition[OFFSET_Z], vecPosition);

  dReal dCosineA = (std::pow(dynamic_cast<const Cylinder&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_A)).GetLength(), 2.0) - std::pow(dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_B)).GetLength(), 2.0) + dSquaredDistance) / (dynamic_cast<const Cylinder&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_A)).GetLength() * dDistance * 2.0), dCosineB = (std::pow(dynamic_cast<const Cylinder&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_A)).GetLength(), 2.0) + std::pow(dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_B)).GetLength(), 2.0) - dSquaredDistance) / (dynamic_cast<const Cylinder&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_A)).GetLength() * dynamic_cast<const Capsule&>(GlobalVariable::HumanoidData.GetLeg(int()).GetObject(SEGMENT_B)).GetLength() * 2.0);

  switch (reSideSetting) {
  case Left :
    rpdAngle[SEGMENT_A] = HandleSingularPoint(dJointGetHingeAngle(GlobalVariable::HumanoidData.GetLeg(SEGMENT_A).GetJoint(SEGMENT_A)), std::atan2(-vecPosition[OFFSET_Y], -vecPosition[OFFSET_Z]));

    break;
  case Right :
    rpdAngle[SEGMENT_A] = HandleSingularPoint(dJointGetHingeAngle(GlobalVariable::HumanoidData.GetLeg(SEGMENT_B).GetJoint(SEGMENT_A)), std::atan2(-vecPosition[OFFSET_Y], -vecPosition[OFFSET_Z]));

    break;
  default : throw(std::invalid_argument("不正のサイド情報が入力されました。"));
  }

  rpdAngle[SEGMENT_B] = std::atan2(vecPosition[OFFSET_X], std::sqrt(std::pow(vecPosition[OFFSET_Y], 2.0) + std::pow(vecPosition[OFFSET_Z], 2.0)));

  do {
    dReal mdTemp[COMPONENT_COUNT_3D] = {0.0};

    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      if (i == OFFSET_X)
        vecDirection[i] = 0.0;
      else if (i == OFFSET_Y)
        vecDirection[i] = std::cos(rpdAngle[SEGMENT_A]);
      else if (i == OFFSET_Z)
        vecDirection[i] = -std::sin(rpdAngle[SEGMENT_A]);

    if (CrossProduct(vecPosition, (const dReal[COMPONENT_COUNT_3D]){1.0, 0.0, 0.0}, mdTemp), !std::signbit(DotProduct(vecDirection, mdTemp)))
      rpdAngle[SEGMENT_B] = (!std::signbit(rpdAngle[SEGMENT_B]) ? -rpdAngle[SEGMENT_B] + M_PI : -(rpdAngle[SEGMENT_B] + M_PI));
  } while (false);

  rpdAngle[SEGMENT_B] += std::atan2(std::sqrt(1.0 - std::pow(dCosineA, 2.0)), dCosineA);

  switch (reSideSetting) {
  case Left :
    rpdAngle[SEGMENT_B] = HandleSingularPoint(dJointGetHingeAngle(GlobalVariable::HumanoidData.GetLeg(SEGMENT_A).GetJoint(SEGMENT_B)), rpdAngle[SEGMENT_B]);

    break;
  case Right :
    rpdAngle[SEGMENT_B] = HandleSingularPoint(dJointGetHingeAngle(GlobalVariable::HumanoidData.GetLeg(SEGMENT_B).GetJoint(SEGMENT_B)), rpdAngle[SEGMENT_B]);

    break;
  default : throw(std::invalid_argument("不正のサイド情報が入力されました。"));
  }

  rpdAngle[SEGMENT_C] = 0.0;
  rpdAngle[SEGMENT_D] = std::atan2(std::sqrt(1.0 - std::pow(dCosineB, 2.0)), dCosineB) - M_PI;

  dReal mdAxis[COMPONENT_COUNT_3D] = {std::cos(rpdAngle[SEGMENT_B] + rpdAngle[SEGMENT_D]), std::sin(rpdAngle[SEGMENT_A]) * std::sin(rpdAngle[SEGMENT_B] + rpdAngle[SEGMENT_D]), std::cos(rpdAngle[SEGMENT_A]) * std::sin(rpdAngle[SEGMENT_B] + rpdAngle[SEGMENT_D])}, mdDirection[COMPONENT_COUNT_3D] = {std::sin(rpdAngle[SEGMENT_B] + rpdAngle[SEGMENT_D]), -std::sin(rpdAngle[SEGMENT_A]) * std::cos(rpdAngle[SEGMENT_B] + rpdAngle[SEGMENT_D]), -std::cos(rpdAngle[SEGMENT_A]) * std::cos(rpdAngle[SEGMENT_B] + rpdAngle[SEGMENT_D])}, mdIntermediate[COMPONENT_COUNT_3D] = {0.0};

  dBodyVectorFromWorld(GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_B).GetBody(), rpdTipPosture[OFFSET_X], rpdTipPosture[OFFSET_Y], rpdTipPosture[OFFSET_Z], vecDirection);

  do {
    dReal dTemp = DotProduct(vecDirection, mdAxis);

    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      mdIntermediate[i] = vecDirection[i] - mdAxis[i] * dTemp;

    Normalize(mdIntermediate, 1.0);
  } while (false);

  CheckLimit(dCosineA = DotProduct(mdDirection, mdIntermediate), 1.0);

  rpdAngle[SEGMENT_E] = std::atan2(std::sqrt(1.0 - std::pow(dCosineA, 2.0)), dCosineA);

  do {
    dReal mdTemp[COMPONENT_COUNT_3D] = {0.0};

    if (CrossProduct(mdDirection, mdIntermediate, mdTemp), !std::signbit(DotProduct(mdAxis, mdTemp)))
      rpdAngle[SEGMENT_E] = -rpdAngle[SEGMENT_E];
  } while (false);

  CheckLimit(dCosineB = DotProduct(vecDirection, mdIntermediate), 1.0);

  rpdAngle[SEGMENT_F] = std::atan2(std::sqrt(1.0 - std::pow(dCosineB, 2.0)), dCosineB);

  do {
    dReal mdTemp[COMPONENT_COUNT_3D] = {0.0};

    if (CrossProduct(vecDirection, mdAxis, mdTemp), CrossProduct(vecDirection, mdIntermediate, mdAxis), std::signbit(DotProduct(mdAxis, mdTemp)))
      rpdAngle[SEGMENT_F] = -rpdAngle[SEGMENT_F];
  } while (false);
}

void ProportionalDifferentialControl(void)
{
  for (int i = 0; i < Head::iJointCount; i++)
    SetJointParameter(GlobalVariable::HumanoidData.GetHead().GetJoint(i), GlobalVariable::mdHeadAngle[i]);

  for (int i = 0; i < Torso::iJointCount - 1; i++)
    SetJointParameter(GlobalVariable::HumanoidData.GetTorso().GetJoint(i), GlobalVariable::mdTorsoAngle[i]);

  for (int i = 0; i < Humanoid::iArmCount; i++)
    for (int j = 0; j < Arm::iJointCount - 1; j++)
      SetJointParameter(GlobalVariable::HumanoidData.GetArm(i).GetJoint(j), GlobalVariable::mdArmAngle[i][j]);

  for (int i = 0; i < Humanoid::iLegCount; i++)
    for (int j = 0; j < Leg::iJointCount; j++)
      SetJointParameter(GlobalVariable::HumanoidData.GetLeg(i).GetJoint(j), GlobalVariable::mdLegAngle[i][j]);
}

void SetCoordinate(void)
{
  dReal dBase = GetBoundingBox();

  for (int i = 0; i < Head::iPartCount + Head::iDummyCount; i++)
    dBodySetPosition(GlobalVariable::HumanoidData.GetHead().GetObject(i).GetBody(), dBodyGetPosition(GlobalVariable::HumanoidData.GetHead().GetObject(i).GetBody())[OFFSET_X], dBodyGetPosition(GlobalVariable::HumanoidData.GetHead().GetObject(i).GetBody())[OFFSET_Y], dBodyGetPosition(GlobalVariable::HumanoidData.GetHead().GetObject(i).GetBody())[OFFSET_Z] - dBase);

  for (int i = 0; i < Torso::iPartCount + Torso::iDummyCount; i++)
    dBodySetPosition(GlobalVariable::HumanoidData.GetTorso().GetObject(i).GetBody(), dBodyGetPosition(GlobalVariable::HumanoidData.GetTorso().GetObject(i).GetBody())[OFFSET_X], dBodyGetPosition(GlobalVariable::HumanoidData.GetTorso().GetObject(i).GetBody())[OFFSET_Y], dBodyGetPosition(GlobalVariable::HumanoidData.GetTorso().GetObject(i).GetBody())[OFFSET_Z] - dBase);

  for (int i = 0; i < Humanoid::iArmCount; i++)
    for (int j = 0; j < Arm::iPartCount + Arm::iDummyCount; j++)
      dBodySetPosition(GlobalVariable::HumanoidData.GetArm(i).GetObject(j).GetBody(), dBodyGetPosition(GlobalVariable::HumanoidData.GetArm(i).GetObject(j).GetBody())[OFFSET_X], dBodyGetPosition(GlobalVariable::HumanoidData.GetArm(i).GetObject(j).GetBody())[OFFSET_Y], dBodyGetPosition(GlobalVariable::HumanoidData.GetArm(i).GetObject(j).GetBody())[OFFSET_Z] - dBase);

  for (int i = 0; i < Humanoid::iLegCount; i++)
    for (int j = 0; j < Leg::iPartCount + Leg::iDummyCount; j++)
      dBodySetPosition(GlobalVariable::HumanoidData.GetLeg(i).GetObject(j).GetBody(), dBodyGetPosition(GlobalVariable::HumanoidData.GetLeg(i).GetObject(j).GetBody())[OFFSET_X], dBodyGetPosition(GlobalVariable::HumanoidData.GetLeg(i).GetObject(j).GetBody())[OFFSET_Y], dBodyGetPosition(GlobalVariable::HumanoidData.GetLeg(i).GetObject(j).GetBody())[OFFSET_Z] - dBase);
}

void SetJointParameter(dJointID jtJoint, const dReal& rdTarget)
{
  static const dReal dFiction = 0.2, dTorque = 1.0E4, dThreshold = M_PI / 1.8E4;

  dReal dDifference = rdTarget - dJointGetHingeAngle(jtJoint);

  if (dDifference > M_PI)
    if (dDifference = std::fmod(dDifference, M_PI * 2.0), dDifference > M_PI)
      dDifference -= M_PI * 2.0;
    else
      ;
  else if (dDifference < -M_PI)
    if (dDifference = std::fmod(dDifference, M_PI * 2.0), dDifference < -M_PI)
      dDifference += M_PI * 2.0;
    else
      ;

  dJointSetHingeParam(jtJoint, dParamVel, dDifference - dJointGetHingeAngleRate(jtJoint) * dFiction);
  dJointSetHingeParam(jtJoint, dParamFMax, dTorque);
}

void SustainState(dBodyID rbBody)
{
  static dQuaternion quatIdentity = {1.0};

  dBodySetPosition(rbBody, dBodyGetPosition(rbBody)[OFFSET_X], dBodyGetPosition(rbBody)[OFFSET_Y], dBodyGetPosition(rbBody)[OFFSET_Z]);
  dBodySetQuaternion(rbBody, quatIdentity);
  dBodySetLinearVel(rbBody, 0.0, 0.0, dBodyGetLinearVel(rbBody)[OFFSET_Z]);
  dBodySetAngularVel(rbBody, 0.0, 0.0, 0.0);
}

dReal AggregateMagnitude(void)
{
  return GlobalVariable::dTotalMagnitude = GlobalVariable::HumanoidData.GetForceMagnitude();
}

dReal MotionRange(const eBodyPart& reBodyPartSetting, const eRangeMode& reRangeModeSetting, const int& riIndex)
{
  static const dReal dNeckFlexion = M_PI * 60.0 / 1.8E2, dNeckExtension = M_PI * 50.0 / 1.8E2, dNeckLateralBending = M_PI * 50.0 / 1.8E2, dNeckRotation = M_PI * 60.0 / 1.8E2, dWaistFlexion = M_PI * 60.0 / 1.8E2, dWaistExtension = M_PI * 35.0 / 1.8E2, dWaistLateralBending = M_PI * 40.0 / 1.8E2, dWaistRotation = M_PI * 40.0 / 1.8E2;
  static const dReal mdHeadMotionRange[Head::iJointCount][GlobalVariable::iRangeCount] = {
    {-dNeckLateralBending, dNeckLateralBending},
    {-dNeckExtension, dNeckFlexion},
    {-dNeckRotation, dNeckRotation}
  }, mdTorsoMotionRange[Torso::iJointCount - 1][GlobalVariable::iRangeCount] = {
    {-dWaistLateralBending, dWaistLateralBending},
    {-dWaistExtension, dWaistFlexion},
    {-dWaistRotation, dWaistRotation}
  };

  int iTarget = static_cast<int>(reRangeModeSetting);

  switch (reBodyPartSetting) {
  case Head : return mdHeadMotionRange[riIndex][iTarget];
  case Torso : return mdTorsoMotionRange[riIndex][iTarget];
  default : throw(std::invalid_argument("与えられたパーツ情報が不正です。"));
  }
}

dReal MotionRange(const eBodyPart& reBodyPartSetting, const eRangeMode& reRangeModeSetting, const eSide& reSideSetting, const int& riIndex)
{
  static const dReal dShoulderFlexion = M_PI * 170.0 / 1.8E2, dShoulderExtension = M_PI * 55.0 / 1.8E2, dShoulderAbduction = M_PI * 85.0 / 1.8E2, dShoulderAdduction = M_PI * 140.0 / 1.8E2, dShoulderExternalRotation = M_PI * 70.0 / 1.8E2, dShoulderInternalRotation = M_PI * 90.0 / 1.8E2, dElbowFlexion = M_PI * 140.0 / 1.8E2, dElbowExtension = 0.0, dHipFlexion = M_PI * 105.0 / 1.8E2, dHipExtension = M_PI * 35.0 / 1.8E2, dHipAbduction = M_PI * 65.0 / 1.8E2, dHipAdduction = M_PI * 35.0 / 1.8E2, dHipExternalRotation = M_PI * 30.0 / 1.8E2, dHipInternalRotation = M_PI * 30.0 / 1.8E2, dKneeFlexion = M_PI * 100.0 / 1.8E2, dKneeExtension = 0.0, dAnkleFlexion = M_PI * 40.0 / 1.8E2, dAnkleExtension = M_PI * 30.0 / 1.8E2, dAnkleEversion = M_PI * 20.0 / 1.8E2, dAnkleInversion = M_PI * 30.0 / 1.8E2;
  static const dReal mdArmMotionRange[Humanoid::iArmCount][Arm::iJointCount - 1][GlobalVariable::iRangeCount] = {
    {
      {-dShoulderAbduction, dShoulderAdduction},
      {-dShoulderExtension, dShoulderFlexion},
      {-dShoulderExternalRotation, dShoulderInternalRotation},
      {-dElbowExtension, dElbowFlexion}
    },
    {
      {-dShoulderAdduction, dShoulderAbduction},
      {-dShoulderFlexion, dShoulderExtension},
      {-dShoulderExternalRotation, dShoulderInternalRotation},
      {-dElbowFlexion, dElbowExtension}
    }
  }, mdLegMotionRange[Humanoid::iLegCount][Leg::iJointCount][GlobalVariable::iRangeCount] = {
    {
      {-dHipAbduction, dHipAdduction},
      {-dHipExtension, dHipFlexion},
      {-dHipInternalRotation, dHipExternalRotation},
      {-dKneeFlexion, dKneeExtension},
      {-dAnkleEversion, dAnkleInversion},
      {-dAnkleExtension, dAnkleFlexion}
    },
    {
      {-dHipAdduction, dHipAbduction},
      {-dHipExtension, dHipFlexion},
      {-dHipExternalRotation, dHipInternalRotation},
      {-dKneeFlexion, dKneeExtension},
      {-dAnkleInversion, dAnkleEversion},
      {-dAnkleExtension, dAnkleFlexion}
    }
  };

  int iTarget = static_cast<int>(reRangeModeSetting);

  switch (reBodyPartSetting) {
  case Arm :
    switch (reSideSetting) {
    case Left : return mdArmMotionRange[SEGMENT_A][riIndex][iTarget];
    case Right : return mdArmMotionRange[SEGMENT_B][riIndex][iTarget];
    default : throw(std::invalid_argument("不正のサイド情報が入力されました。"));
    }
  case Leg :
    switch (reSideSetting) {
    case Left : return mdLegMotionRange[SEGMENT_A][riIndex][iTarget];
    case Right : return mdLegMotionRange[SEGMENT_B][riIndex][iTarget];
    default : throw(std::invalid_argument("不正のサイド情報が入力されました。"));
    }
  default : throw(std::invalid_argument("与えられたパーツ情報が不正です。"));
  }
}