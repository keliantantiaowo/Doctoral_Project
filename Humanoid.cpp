#include <utility>
#include "Common.h"
#include "Humanoid.h"
#include "Pose Control.h"

Humanoid::Humanoid(void) : HeadData(typename Head::Head()), TorsoData(typename Torso::Torso()), mArmArray{typename Arm::Arm(), typename Arm::Arm()}, mLegArray{typename Leg::Leg(), typename Leg::Leg()}
{
}

Humanoid::Humanoid(const dReal& rdHeadRadius, const dReal& rdHeadMass, const dReal& rdTorsoRadiusA, const dReal& rdTorsoRadiusB, const dReal& rdTorsoLengthA, const dReal& rdTorsoLengthB, const dReal& rdTorsoLengthC, const dReal& rdTorsoMassA, const dReal& rdTorsoMassB, const dReal& rdTorsoMassC, const dReal& rdArmRadiusA, const dReal& rdArmRadiusB, const dReal& rdArmRadiusC, const dReal& rdArmLengthA, const dReal& rdArmLengthB, const dReal& rdArmMassA, const dReal& rdArmMassB, const dReal& rdArmMassC, const dReal& rdLegRadiusA, const dReal& rdLegRadiusB, const dReal& rdLegLengthA, const dReal& rdLegLengthB, const dReal* const& rpdLegSizeInfo, const dReal& rdLegMassA, const dReal& rdLegMassB, const dReal& rdLegMassC) : HeadData(rdHeadRadius, rdHeadMass), TorsoData(rdTorsoRadiusA, rdTorsoRadiusB, rdTorsoLengthA, rdTorsoLengthB, rdTorsoLengthC, rdTorsoMassA, rdTorsoMassB, rdTorsoMassC), mArmArray{typename Arm::Arm(rdArmRadiusA, rdArmRadiusB, rdArmRadiusC, rdArmLengthA, rdArmLengthB, rdArmMassA, rdArmMassB, rdArmMassC), typename Arm::Arm(rdArmRadiusA, rdArmRadiusB, rdArmRadiusC, rdArmLengthA, rdArmLengthB, rdArmMassA, rdArmMassB, rdArmMassC)}, mLegArray{typename Leg::Leg(rdLegRadiusA, rdLegRadiusB, rdLegLengthA, rdLegLengthB, rpdLegSizeInfo, rdLegMassA, rdLegMassB, rdLegMassC), typename Leg::Leg(rdLegRadiusA, rdLegRadiusB, rdLegLengthA, rdLegLengthB, rpdLegSizeInfo, rdLegMassA, rdLegMassB, rdLegMassC)}
{
  Initialize();
}

Humanoid::Humanoid(const Humanoid& rHumanoidData) : HeadData(rHumanoidData.GetHead()), TorsoData(rHumanoidData.GetTorso()), mArmArray{rHumanoidData.GetArm(SEGMENT_A), rHumanoidData.GetArm(SEGMENT_B)}, mLegArray{rHumanoidData.GetLeg(SEGMENT_A), rHumanoidData.GetLeg(SEGMENT_B)}
{
  Initialize();
}

Humanoid::Humanoid(Humanoid&& rHumanoidData) : HeadData(std::move(rHumanoidData.HeadData)), TorsoData(std::move(rHumanoidData.TorsoData)), mArmArray{std::move(rHumanoidData.mArmArray[SEGMENT_A]), std::move(rHumanoidData.mArmArray[SEGMENT_B])}, mLegArray{std::move(rHumanoidData.mLegArray[SEGMENT_A]), std::move(rHumanoidData.mLegArray[SEGMENT_B])}
{
}

Humanoid::~Humanoid(void)
{
}

Humanoid& Humanoid::operator=(const Humanoid& rHumanoidData)
{
  if (&rHumanoidData == this)
    return *this;

  HeadData = rHumanoidData.GetHead();
  TorsoData = rHumanoidData.GetTorso();

  for (int i = 0; i < Humanoid::iArmCount; i++)
    mArmArray[i] = rHumanoidData.GetArm(i);

  for (int i = 0; i < Humanoid::iLegCount; i++)
    mLegArray[i] = rHumanoidData.GetLeg(i);

  Initialize();

  return *this;
}

Humanoid& Humanoid::operator=(Humanoid&& rHumanoidData)
{
  if (&rHumanoidData == this)
    return *this;

  HeadData = std::move(rHumanoidData.HeadData);
  TorsoData = std::move(rHumanoidData.TorsoData);

  for (int i = 0; i < Humanoid::iArmCount; i++)
    mArmArray[i] = std::move(rHumanoidData.mArmArray[i]);

  for (int i = 0; i < Humanoid::iLegCount; i++)
    mLegArray[i] = std::move(rHumanoidData.mLegArray[i]);

  return *this;
}

const class Head& Humanoid::GetHead(void) const
{
  return HeadData;
}

const class Torso& Humanoid::GetTorso(void) const
{
  return TorsoData;
}

const class Arm& Humanoid::GetArm(const int& riIndex) const
{
  return mArmArray[riIndex];
}

const class Leg& Humanoid::GetLeg(const int& riIndex) const
{
  return mLegArray[riIndex];
}

dReal Humanoid::GetForceMagnitude(void) const
{
  dReal dResult = 0.0;

  dResult += HeadData.GetForceMagnitude(SEGMENT_A);
  dResult += TorsoData.GetForceMagnitude(SEGMENT_A) + TorsoData.GetForceMagnitude(SEGMENT_D);

  for (int i = 0; i < Humanoid::iArmCount; i++)
    dResult += mArmArray[i].GetForceMagnitude(SEGMENT_A) + mArmArray[i].GetForceMagnitude(SEGMENT_D) + mArmArray[i].GetForceMagnitude(SEGMENT_E);

  for (int i = 0; i < Humanoid::iLegCount; i++)
    dResult += mLegArray[i].GetForceMagnitude(SEGMENT_A) + mLegArray[i].GetForceMagnitude(SEGMENT_D) + mLegArray[i].GetForceMagnitude(SEGMENT_E);

  return dResult;
}

void Humanoid::DrawObject(void) const
{
  HeadData.DrawObject();
  TorsoData.DrawObject();

  for (int i = 0; i < Humanoid::iArmCount; i++)
    mArmArray[i].DrawObject();

  for (int i = 0; i < Humanoid::iLegCount; i++)
    mLegArray[i].DrawObject();
}

void Humanoid::Initialize(void) const
{
  BindSkeletonStructure(this);

  ObtainPosture();

  Initialize(SkeletonTree().GetRoot(), nullptr);
}

void Humanoid::Initialize(Tree<std::string, PostureManager>::Node* const& rpNodePointer, dReal* const& rpdPosition) const
{
  static const dReal dAjustment = 1.0E-5;

  dVector3 vecDirection;

  dBodySetQuaternion(rpNodePointer->GetValue().GetBody(), rpNodePointer->GetValue().GetPosture());

  if (rpNodePointer->GetKey() == "頭部") {
    dReal dFactor = dynamic_cast<const Sphere&>(HeadData.GetObject(SEGMENT_A)).GetRadius();

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);

    for (int i = 0; i < Head::iPartCount + Head::iDummyCount; i++)
      if (i == SEGMENT_A)
        dBodySetPosition(HeadData.GetObject(i).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
      else if (i == SEGMENT_B)
        dBodySetPosition(HeadData.GetObject(i).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);
        dBodySetPosition(HeadData.GetObject(i).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      }

    for (int i = 0; i < Head::iJointCount; i++) {
      if (i == SEGMENT_A) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 1.0, 0.0, 0.0, vecDirection);

        dJointAttach(HeadData.GetJoint(i), HeadData.GetObject(SEGMENT_A).GetBody(), HeadData.GetObject(SEGMENT_B).GetBody());
        dJointSetHingeAnchor(HeadData.GetJoint(i), dBodyGetPosition(HeadData.GetObject(SEGMENT_B).GetBody())[OFFSET_X], dBodyGetPosition(HeadData.GetObject(SEGMENT_B).GetBody())[OFFSET_Y], dBodyGetPosition(HeadData.GetObject(SEGMENT_B).GetBody())[OFFSET_Z]);
      } else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), std::sin(GlobalVariable::mdHeadAngle[SEGMENT_C]), std::cos(GlobalVariable::mdHeadAngle[SEGMENT_C]), 0.0, vecDirection);

        dJointAttach(HeadData.GetJoint(i), HeadData.GetObject(SEGMENT_B).GetBody(), HeadData.GetObject(SEGMENT_C).GetBody());
        dJointSetHingeAnchor(HeadData.GetJoint(i), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);
      } else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);

        dJointAttach(HeadData.GetJoint(i), TorsoData.GetObject(SEGMENT_C).GetBody(), HeadData.GetObject(SEGMENT_C).GetBody());
        dJointSetHingeAnchor(HeadData.GetJoint(i), dBodyGetPosition(HeadData.GetObject(SEGMENT_C).GetBody())[OFFSET_X], dBodyGetPosition(HeadData.GetObject(SEGMENT_C).GetBody())[OFFSET_Y], dBodyGetPosition(HeadData.GetObject(SEGMENT_C).GetBody())[OFFSET_Z]);
      }

      dJointSetHingeAxis(HeadData.GetJoint(i), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
      dJointSetHingeParam(HeadData.GetJoint(i), dParamLoStop, MotionRange(Head, Lower, i));
      dJointSetHingeParam(HeadData.GetJoint(i), dParamHiStop, MotionRange(Head, Upper, i));
    }
  } else if (rpNodePointer->GetKey() == "頸部") {
    dReal dFactor = dynamic_cast<const Cylinder&>(TorsoData.GetObject(SEGMENT_C)).GetLength() * 0.125;

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);
    dBodySetPosition(TorsoData.GetObject(SEGMENT_C).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);

    dJointAttach(TorsoData.GetJoint(SEGMENT_D), TorsoData.GetObject(SEGMENT_A).GetBody(), TorsoData.GetObject(SEGMENT_C).GetBody());
    dJointSetFixed(TorsoData.GetJoint(SEGMENT_D));

    for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++) {
      for (int i = 0; i < COMPONENT_COUNT_3D; i++)
        rpdPosition[i] += vecDirection[i] * dFactor * 2.0;

      Initialize(*i, rpdPosition);
    }
  } else if (rpNodePointer->GetKey() == "胸部") {
    dReal dFactor = dynamic_cast<const Capsule&>(TorsoData.GetObject(SEGMENT_A)).GetLength() / 2.0;

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);

    for (int i = 0; i < Torso::iDummyCount + 1; i++)
      if (i == SEGMENT_A)
        dBodySetPosition(TorsoData.GetObject(SEGMENT_A).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
      else if (i == SEGMENT_B)
        dBodySetPosition(TorsoData.GetObject(SEGMENT_D).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);
        dBodySetPosition(TorsoData.GetObject(SEGMENT_E).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      }

    for (int i = 0; i < Torso::iJointCount - 1; i++) {
      if (i == SEGMENT_A) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 1.0, 0.0, 0.0, vecDirection);

        dJointAttach(TorsoData.GetJoint(i), TorsoData.GetObject(SEGMENT_A).GetBody(), TorsoData.GetObject(SEGMENT_D).GetBody());
        dJointSetHingeAnchor(TorsoData.GetJoint(i), dBodyGetPosition(TorsoData.GetObject(SEGMENT_D).GetBody())[OFFSET_X], dBodyGetPosition(TorsoData.GetObject(SEGMENT_D).GetBody())[OFFSET_Y], dBodyGetPosition(TorsoData.GetObject(SEGMENT_D).GetBody())[OFFSET_Z]);
      } else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), std::sin(GlobalVariable::mdTorsoAngle[SEGMENT_B]), std::cos(GlobalVariable::mdTorsoAngle[SEGMENT_B]), 0.0, vecDirection);

        dJointAttach(TorsoData.GetJoint(i), TorsoData.GetObject(SEGMENT_D).GetBody(), TorsoData.GetObject(SEGMENT_E).GetBody());
        dJointSetHingeAnchor(TorsoData.GetJoint(i), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);
      } else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);

        dJointAttach(TorsoData.GetJoint(i), TorsoData.GetObject(SEGMENT_B).GetBody(), TorsoData.GetObject(SEGMENT_E).GetBody());
        dJointSetHingeAnchor(TorsoData.GetJoint(i), dBodyGetPosition(TorsoData.GetObject(SEGMENT_E).GetBody())[OFFSET_X], dBodyGetPosition(TorsoData.GetObject(SEGMENT_E).GetBody())[OFFSET_Y], dBodyGetPosition(TorsoData.GetObject(SEGMENT_E).GetBody())[OFFSET_Z]);
      }

      dJointSetHingeAxis(TorsoData.GetJoint(i), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
      dJointSetHingeParam(TorsoData.GetJoint(i), dParamLoStop, MotionRange(Torso, Lower, i));
      dJointSetHingeParam(TorsoData.GetJoint(i), dParamHiStop, MotionRange(Torso, Upper, i));
    }

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);

    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      rpdPosition[i] += vecDirection[i] * dFactor * 2.0;

    dFactor = dynamic_cast<const Capsule&>(TorsoData.GetObject(SEGMENT_A)).GetRadius();

    for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++) {
      dReal mdPosition[COMPONENT_COUNT_3D] = {0.0};

      if ((*i)->GetKey() == "頸部")
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);
      else if ((*i)->GetKey() == "左上腕")
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 1.0, 0.0, vecDirection);
      else if ((*i)->GetKey() == "右上腕")
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, -1.0, 0.0, vecDirection);

      for (int j = 0; j < COMPONENT_COUNT_3D; j++)
        mdPosition[j] = rpdPosition[j] + vecDirection[j] * dFactor;

      Initialize(*i, mdPosition);
    }
  } else if (rpNodePointer->GetKey() == "腹部") {
    dReal dFactor = dynamic_cast<const Capsule&>(TorsoData.GetObject(SEGMENT_B)).GetLength() / 2.0;

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);
    dBodySetPosition(TorsoData.GetObject(SEGMENT_B).GetBody(), 0.0, 0.0, 0.0);

    dReal mdPosition[COMPONENT_COUNT_3D] = {dBodyGetPosition(TorsoData.GetObject(SEGMENT_B).GetBody())[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, dBodyGetPosition(TorsoData.GetObject(SEGMENT_B).GetBody())[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, dBodyGetPosition(TorsoData.GetObject(SEGMENT_B).GetBody())[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor};

    dFactor += dynamic_cast<const Capsule&>(TorsoData.GetObject(SEGMENT_B)).GetRadius() / 2.0;

    for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++) {
      dReal mdNewPosition[COMPONENT_COUNT_3D] = {0.0};

      if ((*i)->GetKey() == "胸部") {
        Initialize(*i, mdPosition);

        for (int i = 0; i < COMPONENT_COUNT_3D; i++)
          mdPosition[i] = dBodyGetPosition(TorsoData.GetObject(SEGMENT_B).GetBody())[i] - vecDirection[i] * dFactor;

        dFactor = dynamic_cast<const Box&>(mLegArray->GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_Y);

        continue;
      } else if ((*i)->GetKey() == "左大腿")
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 1.0, 0.0, vecDirection);
      else if ((*i)->GetKey() == "右大腿")
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, -1.0, 0.0, vecDirection);

      for (int i = 0; i < COMPONENT_COUNT_3D; i++)
        mdNewPosition[i] = mdPosition[i] + vecDirection[i] * dFactor;

      Initialize(*i, mdNewPosition);
    }
  } else if (rpNodePointer->GetKey() == "左上腕") {
    dReal dFactor = dynamic_cast<const Capsule&>(mArmArray[SEGMENT_A].GetObject(SEGMENT_A)).GetLength() / 2.0;

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);

    for (int i = 0; i < Arm::iDummyCount + 1; i++)
      if (i == SEGMENT_A)
        dBodySetPosition(mArmArray[SEGMENT_A].GetObject(SEGMENT_A).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
      else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, -1.0, 0.0, vecDirection);
        dBodySetPosition(mArmArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      } else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);
        dBodySetPosition(mArmArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      }

    for (int i = 0; i < Arm::iJointCount / 2 + 1; i++) {
      if (i == SEGMENT_A) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 1.0, 0.0, 0.0, vecDirection);

        dJointAttach(mArmArray[SEGMENT_A].GetJoint(i), TorsoData.GetObject(SEGMENT_A).GetBody(), mArmArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody());
        dJointSetHingeAnchor(mArmArray[SEGMENT_A].GetJoint(i), dBodyGetPosition(mArmArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody())[OFFSET_X], dBodyGetPosition(mArmArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody())[OFFSET_Y], dBodyGetPosition(mArmArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody())[OFFSET_Z]);
      } else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, std::sin(GlobalVariable::mdArmAngle[SEGMENT_A][i]), std::cos(GlobalVariable::mdArmAngle[SEGMENT_A][i]), vecDirection);

        dJointAttach(mArmArray[SEGMENT_A].GetJoint(i), mArmArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody(), mArmArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody());
        dJointSetHingeAnchor(mArmArray[SEGMENT_A].GetJoint(i), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);
      } else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);

        dJointAttach(mArmArray[SEGMENT_A].GetJoint(i), mArmArray[SEGMENT_A].GetObject(SEGMENT_A).GetBody(), mArmArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody());
        dJointSetHingeAnchor(mArmArray[SEGMENT_A].GetJoint(i), dBodyGetPosition(mArmArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody())[OFFSET_X], dBodyGetPosition(mArmArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody())[OFFSET_Y], dBodyGetPosition(mArmArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody())[OFFSET_Z]);
      }

      dJointSetHingeAxis(mArmArray[SEGMENT_A].GetJoint(i), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
      dJointSetHingeParam(mArmArray[SEGMENT_A].GetJoint(i), dParamLoStop, MotionRange(Arm, Lower, Left, i));
      dJointSetHingeParam(mArmArray[SEGMENT_A].GetJoint(i), dParamHiStop, MotionRange(Arm, Upper, Left, i));
    }

    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      rpdPosition[i] += vecDirection[i] * dFactor * 2.0;

    for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
      Initialize(*i, rpdPosition);
  } else if (rpNodePointer->GetKey() == "左前腕") {
    dReal dFactor = dynamic_cast<const Capsule&>(mArmArray[SEGMENT_A].GetObject(SEGMENT_B)).GetLength() / 2.0;

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);
    dBodySetPosition(mArmArray[SEGMENT_A].GetObject(SEGMENT_B).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 1.0, 0.0, vecDirection);

    dJointAttach(mArmArray[SEGMENT_A].GetJoint(SEGMENT_D), mArmArray[SEGMENT_A].GetObject(SEGMENT_A).GetBody(), mArmArray[SEGMENT_A].GetObject(SEGMENT_B).GetBody());
    dJointSetHingeAnchor(mArmArray[SEGMENT_A].GetJoint(SEGMENT_D), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);
    dJointSetHingeAxis(mArmArray[SEGMENT_A].GetJoint(SEGMENT_D), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
    dJointSetHingeParam(mArmArray[SEGMENT_A].GetJoint(SEGMENT_D), dParamLoStop, MotionRange(Arm, Lower, Left, SEGMENT_D));
    dJointSetHingeParam(mArmArray[SEGMENT_A].GetJoint(SEGMENT_D), dParamHiStop, MotionRange(Arm, Upper, Left, SEGMENT_D));

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);

    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      rpdPosition[i] += vecDirection[i] * dFactor * 2.0;

    for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
      Initialize(*i, rpdPosition);
  } else if (rpNodePointer->GetKey() == "左手") {
    dReal dFactor = dynamic_cast<const Sphere&>(mArmArray[SEGMENT_A].GetObject(SEGMENT_C)).GetRadius();

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);
    dBodySetPosition(mArmArray[SEGMENT_A].GetObject(SEGMENT_C).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);

    dJointAttach(mArmArray[SEGMENT_A].GetJoint(SEGMENT_E), mArmArray[SEGMENT_A].GetObject(SEGMENT_B).GetBody(), mArmArray[SEGMENT_A].GetObject(SEGMENT_C).GetBody());
    dJointSetFixed(mArmArray[SEGMENT_A].GetJoint(SEGMENT_E));
  } else if (rpNodePointer->GetKey() == "右上腕") {
    dReal dFactor = dynamic_cast<const Capsule&>(mArmArray[SEGMENT_B].GetObject(SEGMENT_A)).GetLength() / 2.0;

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);

    for (int i = 0; i < Arm::iDummyCount + 1; i++)
      if (i == SEGMENT_A)
        dBodySetPosition(mArmArray[SEGMENT_B].GetObject(SEGMENT_A).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
      else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, 1.0, 0.0, vecDirection);
        dBodySetPosition(mArmArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      } else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);
        dBodySetPosition(mArmArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      }

    for (int i = 0; i < Arm::iJointCount / 2 + 1; i++) {
      if (i == SEGMENT_A) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 1.0, 0.0, 0.0, vecDirection);

        dJointAttach(mArmArray[SEGMENT_B].GetJoint(i), TorsoData.GetObject(SEGMENT_A).GetBody(), mArmArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody());
        dJointSetHingeAnchor(mArmArray[SEGMENT_B].GetJoint(i), dBodyGetPosition(mArmArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody())[OFFSET_X], dBodyGetPosition(mArmArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody())[OFFSET_Y], dBodyGetPosition(mArmArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody())[OFFSET_Z]);
      } else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, -std::sin(GlobalVariable::mdArmAngle[SEGMENT_B][i]), std::cos(GlobalVariable::mdArmAngle[SEGMENT_B][i]), vecDirection);

        dJointAttach(mArmArray[SEGMENT_B].GetJoint(i), mArmArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody(), mArmArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody());
        dJointSetHingeAnchor(mArmArray[SEGMENT_B].GetJoint(i), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);
      } else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);

        dJointAttach(mArmArray[SEGMENT_B].GetJoint(i), mArmArray[SEGMENT_B].GetObject(SEGMENT_A).GetBody(), mArmArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody());
        dJointSetHingeAnchor(mArmArray[SEGMENT_B].GetJoint(i), dBodyGetPosition(mArmArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody())[OFFSET_X], dBodyGetPosition(mArmArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody())[OFFSET_Y], dBodyGetPosition(mArmArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody())[OFFSET_Z]);
      }

      dJointSetHingeAxis(mArmArray[SEGMENT_B].GetJoint(i), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
      dJointSetHingeParam(mArmArray[SEGMENT_B].GetJoint(i), dParamLoStop, MotionRange(Arm, Lower, Right, i));
      dJointSetHingeParam(mArmArray[SEGMENT_B].GetJoint(i), dParamHiStop, MotionRange(Arm, Upper, Right, i));
    }

    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      rpdPosition[i] += vecDirection[i] * dFactor * 2.0;

    for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
      Initialize(*i, rpdPosition);
  } else if (rpNodePointer->GetKey() == "右前腕") {
    dReal dFactor = dynamic_cast<const Capsule&>(mArmArray[SEGMENT_B].GetObject(SEGMENT_B)).GetLength() / 2.0;

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);
    dBodySetPosition(mArmArray[SEGMENT_B].GetObject(SEGMENT_B).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 1.0, 0.0, vecDirection);

    dJointAttach(mArmArray[SEGMENT_B].GetJoint(SEGMENT_D), mArmArray[SEGMENT_B].GetObject(SEGMENT_A).GetBody(), mArmArray[SEGMENT_B].GetObject(SEGMENT_B).GetBody());
    dJointSetHingeAnchor(mArmArray[SEGMENT_B].GetJoint(SEGMENT_D), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);
    dJointSetHingeAxis(mArmArray[SEGMENT_B].GetJoint(SEGMENT_D), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
    dJointSetHingeParam(mArmArray[SEGMENT_B].GetJoint(SEGMENT_D), dParamLoStop, MotionRange(Arm, Lower, Right, SEGMENT_D));
    dJointSetHingeParam(mArmArray[SEGMENT_B].GetJoint(SEGMENT_D), dParamHiStop, MotionRange(Arm, Upper, Right, SEGMENT_D));

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);

    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      rpdPosition[i] += vecDirection[i] * dFactor * 2.0;

    for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
      Initialize(*i, rpdPosition);
  } else if (rpNodePointer->GetKey() == "右手") {
    dReal dFactor = dynamic_cast<const Sphere&>(mArmArray[SEGMENT_B].GetObject(SEGMENT_C)).GetRadius();

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);
    dBodySetPosition(mArmArray[SEGMENT_B].GetObject(SEGMENT_C).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);

    dJointAttach(mArmArray[SEGMENT_B].GetJoint(SEGMENT_E), mArmArray[SEGMENT_B].GetObject(SEGMENT_B).GetBody(), mArmArray[SEGMENT_B].GetObject(SEGMENT_C).GetBody());
    dJointSetFixed(mArmArray[SEGMENT_B].GetJoint(SEGMENT_E));
  } else if (rpNodePointer->GetKey() == "左大腿") {
    dReal dFactor = dynamic_cast<const Cylinder&>(mLegArray[SEGMENT_A].GetObject(SEGMENT_A)).GetLength() / 2.0;

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);

    for (int i = 0; i < Leg::iDummyCount; i++)
      if (i == SEGMENT_A)
        dBodySetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_A).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
      else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);
        dBodySetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      } else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);
        dBodySetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      }

    for (int i = 0; i < Leg::iJointCount / 2; i++) {
      if (i == SEGMENT_A) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 1.0, 0.0, 0.0, vecDirection);

        dJointAttach(mLegArray[SEGMENT_A].GetJoint(i), TorsoData.GetObject(SEGMENT_B).GetBody(), mLegArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody());
        dJointSetHingeAnchor(mLegArray[SEGMENT_A].GetJoint(i), dBodyGetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody())[OFFSET_X], dBodyGetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody())[OFFSET_Y], dBodyGetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody())[OFFSET_Z]);
      } else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, std::cos(GlobalVariable::mdLegAngle[SEGMENT_A][i]), -std::sin(GlobalVariable::mdLegAngle[SEGMENT_A][i]), vecDirection);

        dJointAttach(mLegArray[SEGMENT_A].GetJoint(i), mLegArray[SEGMENT_A].GetObject(SEGMENT_D).GetBody(), mLegArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody());
        dJointSetHingeAnchor(mLegArray[SEGMENT_A].GetJoint(i), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);
      } else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);

        dJointAttach(mLegArray[SEGMENT_A].GetJoint(i), mLegArray[SEGMENT_A].GetObject(SEGMENT_A).GetBody(), mLegArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody());
        dJointSetHingeAnchor(mLegArray[SEGMENT_A].GetJoint(i), dBodyGetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody())[OFFSET_X], dBodyGetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody())[OFFSET_Y], dBodyGetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_E).GetBody())[OFFSET_Z]);
      }

      dJointSetHingeAxis(mLegArray[SEGMENT_A].GetJoint(i), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
      dJointSetHingeParam(mLegArray[SEGMENT_A].GetJoint(i), dParamLoStop, MotionRange(Leg, Lower, Left, i));
      dJointSetHingeParam(mLegArray[SEGMENT_A].GetJoint(i), dParamHiStop, MotionRange(Leg, Upper, Left, i));
    }

    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      rpdPosition[i] += vecDirection[i] * dFactor * 2.0;

    for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
      Initialize(*i, rpdPosition);
  } else if (rpNodePointer->GetKey() == "左下腿") {
    dReal dFactor = dynamic_cast<const Capsule&>(mLegArray[SEGMENT_A].GetObject(SEGMENT_B)).GetLength() / 2.0;

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);
    dBodySetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_B).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 1.0, 0.0, vecDirection);

    dJointAttach(mLegArray[SEGMENT_A].GetJoint(SEGMENT_D), mLegArray[SEGMENT_A].GetObject(SEGMENT_A).GetBody(), mLegArray[SEGMENT_A].GetObject(SEGMENT_B).GetBody());
    dJointSetHingeAnchor(mLegArray[SEGMENT_A].GetJoint(SEGMENT_D), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);
    dJointSetHingeAxis(mLegArray[SEGMENT_A].GetJoint(SEGMENT_D), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
    dJointSetHingeParam(mLegArray[SEGMENT_A].GetJoint(SEGMENT_D), dParamLoStop, MotionRange(Leg, Lower, Left, SEGMENT_D));
    dJointSetHingeParam(mLegArray[SEGMENT_A].GetJoint(SEGMENT_D), dParamHiStop, MotionRange(Leg, Upper, Left, SEGMENT_D));

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);

    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      rpdPosition[i] += vecDirection[i] * dFactor * 2.0;

    for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
      Initialize(*i, rpdPosition);
  } else if (rpNodePointer->GetKey() == "左足") {
    dReal dFactor = std::sqrt(std::pow(dynamic_cast<const Box&>(mLegArray[SEGMENT_A].GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_X) * 0.25, 2.0) + std::pow(dynamic_cast<const Box&>(mLegArray[SEGMENT_A].GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_Z) / 2.0, 2.0));

    do {
      dReal dTheta = std::atan2(dynamic_cast<const Box&>(mLegArray[SEGMENT_A].GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_X) / 2.0, dynamic_cast<const Box&>(mLegArray[SEGMENT_A].GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_Z));

      dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), std::sin(dTheta), 0.0, -std::cos(dTheta), vecDirection);
    } while (false);

    for (int i = 0; i < Leg::iDummyCount - 1; i++)
      if (i == SEGMENT_A)
        dBodySetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_C).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
      else if (i == SEGMENT_B)
        dBodySetPosition(mLegArray[SEGMENT_A].GetObject(SEGMENT_F).GetBody(), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);

    for (int i = 0; i < Leg::iJointCount / 2 - 1; i++) {
      if (i == SEGMENT_A) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);

        dJointAttach(mLegArray[SEGMENT_A].GetJoint(Leg::iJointCount / 2 + i + 1), mLegArray[SEGMENT_A].GetObject(SEGMENT_B).GetBody(), mLegArray[SEGMENT_A].GetObject(SEGMENT_F).GetBody());
        dJointSetHingeAnchor(mLegArray[SEGMENT_A].GetJoint(Leg::iJointCount / 2 + i + 1), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);

        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 1.0, 0.0, 0.0, vecDirection);
      } else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);

        dJointAttach(mLegArray[SEGMENT_A].GetJoint(Leg::iJointCount / 2 + i + 1), mLegArray[SEGMENT_A].GetObject(SEGMENT_C).GetBody(), mLegArray[SEGMENT_A].GetObject(SEGMENT_F).GetBody());
        dJointSetHingeAnchor(mLegArray[SEGMENT_A].GetJoint(Leg::iJointCount / 2 + i + 1), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);

        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 1.0, 0.0, vecDirection);
      }

      dJointSetHingeAxis(mLegArray[SEGMENT_A].GetJoint(Leg::iJointCount / 2 + i + 1), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
      dJointSetHingeParam(mLegArray[SEGMENT_A].GetJoint(Leg::iJointCount / 2 + i + 1), dParamLoStop, MotionRange(Leg, Lower, Left, Leg::iJointCount / 2 + i + 1));
      dJointSetHingeParam(mLegArray[SEGMENT_A].GetJoint(Leg::iJointCount / 2 + i + 1), dParamHiStop, MotionRange(Leg, Upper, Left, Leg::iJointCount / 2 + i + 1));
    }
  } else if (rpNodePointer->GetKey() == "右大腿") {
    dReal dFactor = dynamic_cast<const Cylinder&>(mLegArray[SEGMENT_B].GetObject(SEGMENT_A)).GetLength() / 2.0;

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);

    for (int i = 0; i < Leg::iDummyCount; i++)
      if (i == SEGMENT_A)
        dBodySetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_A).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
      else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);
        dBodySetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      } else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);
        dBodySetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);
      }

    for (int i = 0; i < Leg::iJointCount / 2; i++) {
      if (i == SEGMENT_A) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 1.0, 0.0, 0.0, vecDirection);

        dJointAttach(mLegArray[SEGMENT_B].GetJoint(i), TorsoData.GetObject(SEGMENT_B).GetBody(), mLegArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody());
        dJointSetHingeAnchor(mLegArray[SEGMENT_B].GetJoint(i), dBodyGetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody())[OFFSET_X], dBodyGetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody())[OFFSET_Y], dBodyGetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody())[OFFSET_Z]);
      } else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, std::cos(GlobalVariable::mdLegAngle[SEGMENT_B][i]), -std::sin(GlobalVariable::mdLegAngle[SEGMENT_B][i]), vecDirection);

        dJointAttach(mLegArray[SEGMENT_B].GetJoint(i), mLegArray[SEGMENT_B].GetObject(SEGMENT_D).GetBody(), mLegArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody());
        dJointSetHingeAnchor(mLegArray[SEGMENT_B].GetJoint(i), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);
      } else if (i == SEGMENT_C) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);

        dJointAttach(mLegArray[SEGMENT_B].GetJoint(i), mLegArray[SEGMENT_B].GetObject(SEGMENT_A).GetBody(), mLegArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody());
        dJointSetHingeAnchor(mLegArray[SEGMENT_B].GetJoint(i), dBodyGetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody())[OFFSET_X], dBodyGetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody())[OFFSET_Y], dBodyGetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_E).GetBody())[OFFSET_Z]);
      }

      dJointSetHingeAxis(mLegArray[SEGMENT_B].GetJoint(i), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
      dJointSetHingeParam(mLegArray[SEGMENT_B].GetJoint(i), dParamLoStop, MotionRange(Leg, Lower, Left, i));
      dJointSetHingeParam(mLegArray[SEGMENT_B].GetJoint(i), dParamHiStop, MotionRange(Leg, Upper, Left, i));
    }

    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      rpdPosition[i] += vecDirection[i] * dFactor * 2.0;

    for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
      Initialize(*i, rpdPosition);
  } else if (rpNodePointer->GetKey() == "右下腿") {
    dReal dFactor = dynamic_cast<const Capsule&>(mLegArray[SEGMENT_B].GetObject(SEGMENT_B)).GetLength() / 2.0;

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);
    dBodySetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_B).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 1.0, 0.0, vecDirection);

    dJointAttach(mLegArray[SEGMENT_B].GetJoint(SEGMENT_D), mLegArray[SEGMENT_B].GetObject(SEGMENT_A).GetBody(), mLegArray[SEGMENT_B].GetObject(SEGMENT_B).GetBody());
    dJointSetHingeAnchor(mLegArray[SEGMENT_B].GetJoint(SEGMENT_D), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);
    dJointSetHingeAxis(mLegArray[SEGMENT_B].GetJoint(SEGMENT_D), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
    dJointSetHingeParam(mLegArray[SEGMENT_B].GetJoint(SEGMENT_D), dParamLoStop, MotionRange(Leg, Lower, Left, SEGMENT_D));
    dJointSetHingeParam(mLegArray[SEGMENT_B].GetJoint(SEGMENT_D), dParamHiStop, MotionRange(Leg, Upper, Left, SEGMENT_D));

    dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);

    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      rpdPosition[i] += vecDirection[i] * dFactor * 2.0;

    for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
      Initialize(*i, rpdPosition);
  } else if (rpNodePointer->GetKey() == "右足") {
    dReal dFactor = std::sqrt(std::pow(dynamic_cast<const Box&>(mLegArray[SEGMENT_B].GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_X) * 0.25, 2.0) + std::pow(dynamic_cast<const Box&>(mLegArray[SEGMENT_B].GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_Z) / 2.0, 2.0));

    do {
      dReal dTheta = std::atan2(dynamic_cast<const Box&>(mLegArray[SEGMENT_B].GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_X) / 2.0, dynamic_cast<const Box&>(mLegArray[SEGMENT_B].GetObject(SEGMENT_C)).GetSizeInfo(OFFSET_Z));

      dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), std::sin(dTheta), 0.0, -std::cos(dTheta), vecDirection);
    } while (false);

    for (int i = 0; i < Leg::iDummyCount - 1; i++)
      if (i == SEGMENT_A)
        dBodySetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_C).GetBody(), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dFactor, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dFactor, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dFactor);
      else if (i == SEGMENT_B)
        dBodySetPosition(mLegArray[SEGMENT_B].GetObject(SEGMENT_F).GetBody(), rpdPosition[OFFSET_X], rpdPosition[OFFSET_Y], rpdPosition[OFFSET_Z]);

    for (int i = 0; i < Leg::iJointCount / 2 - 1; i++) {
      if (i == SEGMENT_A) {
        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 0.0, 0.0, 1.0, vecDirection);

        dJointAttach(mLegArray[SEGMENT_B].GetJoint(Leg::iJointCount / 2 + i + 1), mLegArray[SEGMENT_B].GetObject(SEGMENT_B).GetBody(), mLegArray[SEGMENT_B].GetObject(SEGMENT_F).GetBody());
        dJointSetHingeAnchor(mLegArray[SEGMENT_B].GetJoint(Leg::iJointCount / 2 + i + 1), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);

        dBodyVectorToWorld(rpNodePointer->GetParent()->GetValue().GetBody(), 1.0, 0.0, 0.0, vecDirection);
      } else if (i == SEGMENT_B) {
        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 0.0, -1.0, vecDirection);

        dJointAttach(mLegArray[SEGMENT_B].GetJoint(Leg::iJointCount / 2 + i + 1), mLegArray[SEGMENT_B].GetObject(SEGMENT_C).GetBody(), mLegArray[SEGMENT_B].GetObject(SEGMENT_F).GetBody());
        dJointSetHingeAnchor(mLegArray[SEGMENT_B].GetJoint(Leg::iJointCount / 2 + i + 1), rpdPosition[OFFSET_X] + vecDirection[OFFSET_X] * dAjustment, rpdPosition[OFFSET_Y] + vecDirection[OFFSET_Y] * dAjustment, rpdPosition[OFFSET_Z] + vecDirection[OFFSET_Z] * dAjustment);

        dBodyVectorToWorld(rpNodePointer->GetValue().GetBody(), 0.0, 1.0, 0.0, vecDirection);
      }

      dJointSetHingeAxis(mLegArray[SEGMENT_B].GetJoint(Leg::iJointCount / 2 + i + 1), vecDirection[OFFSET_X], vecDirection[OFFSET_Y], vecDirection[OFFSET_Z]);
      dJointSetHingeParam(mLegArray[SEGMENT_B].GetJoint(Leg::iJointCount / 2 + i + 1), dParamLoStop, MotionRange(Leg, Lower, Left, Leg::iJointCount / 2 + i + 1));
      dJointSetHingeParam(mLegArray[SEGMENT_B].GetJoint(Leg::iJointCount / 2 + i + 1), dParamHiStop, MotionRange(Leg, Upper, Left, Leg::iJointCount / 2 + i + 1));
    }
  }
}

void Humanoid::DrawForce(void) const
{
  HeadData.DrawForce();
  TorsoData.DrawForce();

  for (int i = 0; i < Humanoid::iArmCount; i++)
    mArmArray[i].DrawForce();

  for (int i = 0; i < Humanoid::iLegCount; i++)
    mLegArray[i].DrawForce();
}

void Humanoid::UpdateForce(void)
{
  HeadData.UpdateForce();
  TorsoData.UpdateForce();

  for (int i = 0; i < Humanoid::iArmCount; i++)
    mArmArray[i].UpdateForce();

  for (int i = 0; i < Humanoid::iLegCount; i++)
    mLegArray[i].UpdateForce();
}