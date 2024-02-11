#include <utility>
#include "Common.h"
#include "Body Region.h"

Base::Base(void)
{
}

Base::~Base(void)
{
}

Head::Head(void) : pObjectList{new Sphere(), new Dummy(), new Dummy()}, mJointArray{Joint(), Joint(), Joint()}
{
}

Head::Head(const dReal& rdRadius, const dReal& rdPhysicalMass) : pObjectList{new Sphere(rdRadius, rdPhysicalMass), new Dummy(true), new Dummy(true)}, mJointArray{Joint(dJointTypeHinge, true), Joint(dJointTypeHinge, false), Joint(dJointTypeHinge, false)}
{
}

Head::Head(const Head& rHeadData) : pObjectList{new Sphere(*dynamic_cast<const Sphere*>(rHeadData.pObjectList[SEGMENT_A])), new Dummy(true), new Dummy(true)}, mJointArray{rHeadData.mJointArray[SEGMENT_A], rHeadData.mJointArray[SEGMENT_B], rHeadData.mJointArray[SEGMENT_C]}
{
}

Head::Head(Head&& rHeadData) : pObjectList{std::move(rHeadData.pObjectList[SEGMENT_A]), std::move(rHeadData.pObjectList[SEGMENT_B]), std::move(rHeadData.pObjectList[SEGMENT_C])}, mJointArray{std::move(rHeadData.mJointArray[SEGMENT_A]), std::move(rHeadData.mJointArray[SEGMENT_B]), std::move(rHeadData.mJointArray[SEGMENT_C])}
{
  for (int i = 0; i < Head::iPartCount + Head::iDummyCount; i++)
    rHeadData.pObjectList[i] = nullptr;
}

Head::~Head(void)
{
  Clear();
}

class Head& Head::operator=(const Head& rHeadData)
{
  if (&rHeadData == this)
    return *this;

  *dynamic_cast<Sphere*>(pObjectList[SEGMENT_A]) = *dynamic_cast<const Sphere*>(rHeadData.pObjectList[SEGMENT_A]);
  *dynamic_cast<Dummy*>(pObjectList[SEGMENT_B]) = *dynamic_cast<const Dummy*>(rHeadData.pObjectList[SEGMENT_B]);
  *dynamic_cast<Dummy*>(pObjectList[SEGMENT_C]) = *dynamic_cast<const Dummy*>(rHeadData.pObjectList[SEGMENT_C]);

  for (int i = 0; i < Head::iJointCount; i++)
    mJointArray[i] = rHeadData.mJointArray[i];

  return *this;
}

class Head& Head::operator=(Head&& rHeadData)
{
  if (&rHeadData == this)
    return *this;

  Clear();

  for (int i = 0; i < Head::iPartCount + Head::iDummyCount; i++) {
    pObjectList[i] = std::move(rHeadData.pObjectList[i]);
    rHeadData.pObjectList[i] = nullptr;
  }

  for (int i = 0; i < Head::iJointCount; i++)
    mJointArray[i] = std::move(rHeadData.mJointArray[i]);

  return *this;
}

const Object& Head::GetObject(const int& riIndex) const
{
  return *pObjectList[riIndex];
}

dJointID Head::GetJoint(const int& riIndex) const
{
  return mJointArray[riIndex].GetJoint();
}

dReal Head::GetForceMagnitude(const int& riIndex) const
{
  return mJointArray[riIndex].GetForceMagnitude();
}

void Head::DrawObject(void) const
{
  for (int i = 0; i < Head::iPartCount; i++)
    pObjectList[i]->DrawObject();
}

void Head::DrawForce(void) const
{
  mJointArray->DrawForce();

  dsDrawSphere(dBodyGetPosition(pObjectList[SEGMENT_B]->GetBody()), dBodyGetRotation(pObjectList[SEGMENT_B]->GetBody()), GlobalVariable::dJointRadius);
}

void Head::UpdateForce(void)
{
  mJointArray->UpdateForce();
}

void Head::Clear(void)
{
  for (int i = 0; i < Head::iPartCount + Head::iDummyCount; i++)
    delete pObjectList[i];
}

Torso::Torso(void) : pObjectList{new Capsule(), new Capsule(), new Cylinder(), new Dummy(), new Dummy()}, mJointArray{Joint(), Joint(), Joint(), Joint()}
{
}

Torso::Torso(const dReal& rdRadiusA, const dReal& rdRadiusB, const dReal& rdLengthA, const dReal& rdLengthB, const dReal& rdLengthC, const dReal& rdPhysicalMassA, const dReal& rdPhysicalMassB, const dReal& rdPhysicalMassC) : pObjectList{new Capsule(rdRadiusA, rdLengthA, rdPhysicalMassA), new Capsule(rdRadiusA, rdLengthB, rdPhysicalMassB), new Cylinder(rdRadiusB, rdLengthC, rdPhysicalMassC), new Dummy(true), new Dummy(true)}, mJointArray{Joint(dJointTypeHinge, true), Joint(dJointTypeHinge, false), Joint(dJointTypeHinge, false), Joint(dJointTypeFixed, true)}
{
}

Torso::Torso(const Torso& rTorsoData) : pObjectList{new Capsule(*dynamic_cast<const Capsule*>(rTorsoData.pObjectList[SEGMENT_A])), new Capsule(*dynamic_cast<const Capsule*>(rTorsoData.pObjectList[SEGMENT_B])), new Cylinder(*dynamic_cast<const Cylinder*>(rTorsoData.pObjectList[SEGMENT_C])), new Dummy(true), new Dummy(true)}, mJointArray{rTorsoData.mJointArray[SEGMENT_A], rTorsoData.mJointArray[SEGMENT_B], rTorsoData.mJointArray[SEGMENT_C], rTorsoData.mJointArray[SEGMENT_D]}
{
}

Torso::Torso(Torso&& rTorsoData) : pObjectList{std::move(rTorsoData.pObjectList[SEGMENT_A]), std::move(rTorsoData.pObjectList[SEGMENT_B]), std::move(rTorsoData.pObjectList[SEGMENT_C]), std::move(rTorsoData.pObjectList[SEGMENT_D]), std::move(rTorsoData.pObjectList[SEGMENT_E])}, mJointArray{std::move(rTorsoData.mJointArray[SEGMENT_A]), std::move(rTorsoData.mJointArray[SEGMENT_B]), std::move(rTorsoData.mJointArray[SEGMENT_C]), std::move(rTorsoData.mJointArray[SEGMENT_D])}
{
  for (int i = 0; i < Torso::iPartCount + Torso::iDummyCount; i++)
    rTorsoData.pObjectList[i] = nullptr;
}

Torso::~Torso(void)
{
  Clear();
}

class Torso& Torso::operator=(const Torso& rTorsoData)
{
  if (&rTorsoData == this)
    return *this;

  *dynamic_cast<Capsule*>(pObjectList[SEGMENT_A]) = *dynamic_cast<const Capsule*>(rTorsoData.pObjectList[SEGMENT_A]);
  *dynamic_cast<Capsule*>(pObjectList[SEGMENT_B]) = *dynamic_cast<const Capsule*>(rTorsoData.pObjectList[SEGMENT_B]);
  *dynamic_cast<Cylinder*>(pObjectList[SEGMENT_C]) = *dynamic_cast<const Cylinder*>(rTorsoData.pObjectList[SEGMENT_C]);
  *dynamic_cast<Dummy*>(pObjectList[SEGMENT_D]) = *dynamic_cast<const Dummy*>(rTorsoData.pObjectList[SEGMENT_D]);
  *dynamic_cast<Dummy*>(pObjectList[SEGMENT_E]) = *dynamic_cast<const Dummy*>(rTorsoData.pObjectList[SEGMENT_E]);

  for (int i = 0; i < Torso::iJointCount; i++)
    mJointArray[i] = rTorsoData.mJointArray[i];

  return *this;
}

class Torso& Torso::operator=(Torso&& rTorsoData)
{
  if (&rTorsoData == this)
    return *this;

  Clear();

  for (int i = 0; i < Torso::iPartCount + Torso::iDummyCount; i++) {
    pObjectList[i] = std::move(rTorsoData.pObjectList[i]);
    rTorsoData.pObjectList[i] = nullptr;
  }

  for (int i = 0; i < Torso::iJointCount; i++)
    mJointArray[i] = std::move(rTorsoData.mJointArray[i]);

  return *this;
}

const Object& Torso::GetObject(const int& riIndex) const
{
  return *pObjectList[riIndex];
}

dJointID Torso::GetJoint(const int& riIndex) const
{
  return mJointArray[riIndex].GetJoint();
}

dReal Torso::GetForceMagnitude(const int& riIndex) const
{
  return mJointArray[riIndex].GetForceMagnitude();
}

void Torso::DrawObject(void) const
{
  for (int i = 0; i < Torso::iPartCount; i++)
    pObjectList[i]->DrawObject();
}

void Torso::DrawForce(void) const
{
  mJointArray[SEGMENT_A].DrawForce();

  dsDrawSphere(dBodyGetPosition(pObjectList[SEGMENT_D]->GetBody()), dBodyGetRotation(pObjectList[SEGMENT_D]->GetBody()), GlobalVariable::dJointRadius);

  mJointArray[SEGMENT_D].DrawForce();

  dVector3 vecPosition;

  dBodyGetRelPointPos(pObjectList[SEGMENT_A]->GetBody(), 0.0, 0.0, dynamic_cast<Capsule*>(pObjectList[SEGMENT_A])->GetLength() / 2.0, vecPosition);

  dsDrawSphere(vecPosition, dBodyGetRotation(pObjectList[SEGMENT_A]->GetBody()), GlobalVariable::dJointRadius);
}

void Torso::UpdateForce(void)
{
  mJointArray[SEGMENT_A].UpdateForce();
  mJointArray[SEGMENT_D].UpdateForce();
}

void Torso::Clear(void)
{
  for (int i = 0; i < Torso::iPartCount + Torso::iDummyCount; i++)
    delete pObjectList[i];
}

Arm::Arm(void) : pObjectList{new Capsule(), new Capsule(), new Sphere(), new Dummy(), new Dummy()}, mJointArray{Joint(), Joint(), Joint(), Joint(), Joint()}
{
}

Arm::Arm(const dReal& rdRadiusA, const dReal& rdRadiusB, const dReal& rdRadiusC, const dReal& rdLengthA, const dReal& rdLengthB, const dReal& rdPhysicalMassA, const dReal& rdPhysicalMassB, const dReal& rdPhysicalMassC) : pObjectList{new Capsule(rdRadiusA, rdLengthA, rdPhysicalMassA), new Capsule(rdRadiusB, rdLengthB, rdPhysicalMassB), new Sphere(rdRadiusC, rdPhysicalMassC), new Dummy(true), new Dummy(true)}, mJointArray{Joint(dJointTypeHinge, true), Joint(dJointTypeHinge, false), Joint(dJointTypeHinge, false), Joint(dJointTypeHinge, true), Joint(dJointTypeFixed, true)}
{
}

Arm::Arm(const Arm& rArmData) : pObjectList{new Capsule(*dynamic_cast<const Capsule*>(rArmData.pObjectList[SEGMENT_A])), new Capsule(*dynamic_cast<const Capsule*>(rArmData.pObjectList[SEGMENT_B])), new Sphere(*dynamic_cast<const Sphere*>(rArmData.pObjectList[SEGMENT_C])), new Dummy(true), new Dummy(true)}, mJointArray{rArmData.mJointArray[SEGMENT_A], rArmData.mJointArray[SEGMENT_B], rArmData.mJointArray[SEGMENT_C], rArmData.mJointArray[SEGMENT_D], rArmData.mJointArray[SEGMENT_E]}
{
}

Arm::Arm(Arm&& rArmData) : pObjectList{std::move(rArmData.pObjectList[SEGMENT_A]), std::move(rArmData.pObjectList[SEGMENT_B]), std::move(rArmData.pObjectList[SEGMENT_C]), std::move(rArmData.pObjectList[SEGMENT_D]), std::move(rArmData.pObjectList[SEGMENT_E])}, mJointArray{std::move(rArmData.mJointArray[SEGMENT_A]), std::move(rArmData.mJointArray[SEGMENT_B]), std::move(rArmData.mJointArray[SEGMENT_C]), std::move(rArmData.mJointArray[SEGMENT_D]), std::move(rArmData.mJointArray[SEGMENT_E])}
{
  for (int i = 0; i < Arm::iPartCount + Arm::iDummyCount; i++)
    rArmData.pObjectList[i] = nullptr;
}

Arm::~Arm(void)
{
  Clear();
}

class Arm& Arm::operator=(const Arm& rArmData)
{
  if (&rArmData == this)
    return *this;

  *dynamic_cast<Capsule*>(pObjectList[SEGMENT_A]) = *dynamic_cast<const Capsule*>(rArmData.pObjectList[SEGMENT_A]);
  *dynamic_cast<Capsule*>(pObjectList[SEGMENT_B]) = *dynamic_cast<const Capsule*>(rArmData.pObjectList[SEGMENT_B]);
  *dynamic_cast<Sphere*>(pObjectList[SEGMENT_C]) = *dynamic_cast<const Sphere*>(rArmData.pObjectList[SEGMENT_C]);
  *dynamic_cast<Dummy*>(pObjectList[SEGMENT_D]) = *dynamic_cast<const Dummy*>(rArmData.pObjectList[SEGMENT_D]);
  *dynamic_cast<Dummy*>(pObjectList[SEGMENT_E]) = *dynamic_cast<const Dummy*>(rArmData.pObjectList[SEGMENT_E]);

  for (int i = 0; i < Arm::iJointCount; i++)
    mJointArray[i] = rArmData.mJointArray[i];

  return *this;
}

class Arm& Arm::operator=(Arm&& rArmData)
{
  if (&rArmData == this)
    return *this;

  Clear();

  for (int i = 0; i < Arm::iPartCount + Arm::iDummyCount; i++) {
    pObjectList[i] = std::move(rArmData.pObjectList[i]);
    rArmData.pObjectList[i] = nullptr;
  }

  for (int i = 0; i < Arm::iJointCount; i++)
    mJointArray[i] = std::move(rArmData.mJointArray[i]);

  return *this;
}

const Object& Arm::GetObject(const int& riIndex) const
{
  return *pObjectList[riIndex];
}

dJointID Arm::GetJoint(const int& riIndex) const
{
  return mJointArray[riIndex].GetJoint();
}

dReal Arm::GetForceMagnitude(const int& riIndex) const
{
  return mJointArray[riIndex].GetForceMagnitude();
}

void Arm::DrawObject(void) const
{
  for (int i = 0; i < Arm::iPartCount; i++)
    pObjectList[i]->DrawObject();
}

void Arm::DrawForce(void) const
{
  mJointArray[SEGMENT_A].DrawForce();

  dsDrawSphere(dBodyGetPosition(pObjectList[SEGMENT_D]->GetBody()), dBodyGetRotation(pObjectList[SEGMENT_D]->GetBody()), GlobalVariable::dJointRadius);

  mJointArray[SEGMENT_D].DrawForce();

  dVector3 vecPosition;

  dJointGetHingeAnchor(mJointArray[SEGMENT_D].GetJoint(), vecPosition);

  dsDrawSphere(vecPosition, dBodyGetRotation(pObjectList[SEGMENT_A]->GetBody()), GlobalVariable::dJointRadius);

  mJointArray[SEGMENT_E].DrawForce();

  dsDrawSphere(dBodyGetPosition(pObjectList[SEGMENT_C]->GetBody()), dBodyGetRotation(pObjectList[SEGMENT_C]->GetBody()), GlobalVariable::dJointRadius);
}

void Arm::UpdateForce(void)
{
  mJointArray[SEGMENT_A].UpdateForce();
  mJointArray[SEGMENT_D].UpdateForce();
  mJointArray[SEGMENT_E].UpdateForce();
}

void Arm::Clear(void)
{
  for (int i = 0; i < Arm::iPartCount + Arm::iDummyCount; i++)
    delete pObjectList[i];
}

Leg::Leg(void) : pObjectList{new Cylinder(), new Capsule(), new Box(), new Dummy(), new Dummy(), new Dummy()}, mJointArray{Joint(), Joint(), Joint(), Joint(), Joint(), Joint()}
{
}

Leg::Leg(const dReal& rdRadiusA, const dReal& rdRadiusB, const dReal& rdLengthA, const dReal& rdLengthB, const dReal* const& rpdSizeInfo, const dReal& rdPhysicalMassA, const dReal& rdPhysicalMassB, const dReal& rdPhysicalMassC) : pObjectList{new Cylinder(rdRadiusA, rdLengthA, rdPhysicalMassA), new Capsule(rdRadiusB, rdLengthB, rdPhysicalMassB), new Box(rpdSizeInfo, rdPhysicalMassC), new Dummy(true), new Dummy(true), new Dummy(true)}, mJointArray{Joint(dJointTypeHinge, true), Joint(dJointTypeHinge, false), Joint(dJointTypeHinge, false), Joint(dJointTypeHinge, true), Joint(dJointTypeHinge, true), Joint(dJointTypeHinge, false)}
{
}

Leg::Leg(const Leg& rLegData) : pObjectList{new Cylinder(*dynamic_cast<const Cylinder*>(rLegData.pObjectList[SEGMENT_A])), new Capsule(*dynamic_cast<const Capsule*>(rLegData.pObjectList[SEGMENT_B])), new Box(*dynamic_cast<const Box*>(rLegData.pObjectList[SEGMENT_C])), new Dummy(true), new Dummy(true), new Dummy(true)}, mJointArray{rLegData.mJointArray[SEGMENT_A], rLegData.mJointArray[SEGMENT_B], rLegData.mJointArray[SEGMENT_C], rLegData.mJointArray[SEGMENT_D], rLegData.mJointArray[SEGMENT_E], rLegData.mJointArray[SEGMENT_F]}
{
}

Leg::Leg(Leg&& rLegData) : pObjectList{std::move(rLegData.pObjectList[SEGMENT_A]), std::move(rLegData.pObjectList[SEGMENT_B]), std::move(rLegData.pObjectList[SEGMENT_C]), std::move(rLegData.pObjectList[SEGMENT_D]), std::move(rLegData.pObjectList[SEGMENT_E]), std::move(rLegData.pObjectList[SEGMENT_F])}, mJointArray{std::move(rLegData.mJointArray[SEGMENT_A]), std::move(rLegData.mJointArray[SEGMENT_B]), std::move(rLegData.mJointArray[SEGMENT_C]), std::move(rLegData.mJointArray[SEGMENT_D]), std::move(rLegData.mJointArray[SEGMENT_E]), std::move(rLegData.mJointArray[SEGMENT_F])}
{
  for (int i = 0; i < Leg::iPartCount + Leg::iDummyCount; i++)
    rLegData.pObjectList[i] = nullptr;
}

Leg::~Leg(void)
{
  Clear();
}

class Leg& Leg::operator=(const Leg& rLegData)
{
  if (&rLegData == this)
    return *this;

  *dynamic_cast<Cylinder*>(pObjectList[SEGMENT_A]) = *dynamic_cast<const Cylinder*>(rLegData.pObjectList[SEGMENT_A]);
  *dynamic_cast<Capsule*>(pObjectList[SEGMENT_B]) = *dynamic_cast<const Capsule*>(rLegData.pObjectList[SEGMENT_B]);
  *dynamic_cast<Box*>(pObjectList[SEGMENT_C]) = *dynamic_cast<const Box*>(rLegData.pObjectList[SEGMENT_C]);
  *dynamic_cast<Dummy*>(pObjectList[SEGMENT_D]) = *dynamic_cast<const Dummy*>(rLegData.pObjectList[SEGMENT_D]);
  *dynamic_cast<Dummy*>(pObjectList[SEGMENT_E]) = *dynamic_cast<const Dummy*>(rLegData.pObjectList[SEGMENT_E]);
  *dynamic_cast<Dummy*>(pObjectList[SEGMENT_F]) = *dynamic_cast<const Dummy*>(rLegData.pObjectList[SEGMENT_F]);

  for (int i = 0; i < Leg::iJointCount; i++)
    mJointArray[i] = rLegData.mJointArray[i];

  return *this;
}

class Leg& Leg::operator=(Leg&& rLegData)
{
  if (&rLegData == this)
    return *this;

  Clear();

  for (int i = 0; i < Leg::iPartCount + Leg::iDummyCount; i++) {
    pObjectList[i] = std::move(rLegData.pObjectList[i]);
    rLegData.pObjectList[i] = nullptr;
  }

  for (int i = 0; i < Leg::iJointCount; i++)
    mJointArray[i] = std::move(rLegData.mJointArray[i]);

  return *this;
}

const Object& Leg::GetObject(const int& riIndex) const
{
  return *pObjectList[riIndex];
}

dJointID Leg::GetJoint(const int& riIndex) const
{
  return mJointArray[riIndex].GetJoint();
}

void Leg::DrawObject(void) const
{
  pObjectList[SEGMENT_A]->Object::DrawObject();

  dsDrawCapsule(dBodyGetPosition(pObjectList[SEGMENT_A]->GetBody()), dBodyGetRotation(pObjectList[SEGMENT_A]->GetBody()), dynamic_cast<Cylinder*>(pObjectList[SEGMENT_A])->GetLength(), dynamic_cast<Cylinder*>(pObjectList[SEGMENT_A])->GetRadius());

  for (int i = 0; i < Leg::iPartCount - 1; i++)
    pObjectList[i + 1]->DrawObject();
}

dReal Leg::GetForceMagnitude(const int& riIndex) const
{
  return mJointArray[riIndex].GetForceMagnitude();
}

void Leg::DrawForce(void) const
{
  mJointArray[SEGMENT_A].DrawForce();

  dsDrawSphere(dBodyGetPosition(pObjectList[SEGMENT_D]->GetBody()), dBodyGetRotation(pObjectList[SEGMENT_D]->GetBody()), GlobalVariable::dJointRadius);

  mJointArray[SEGMENT_D].DrawForce();

  dVector3 vecPosition;

  dJointGetHingeAnchor(mJointArray[SEGMENT_D].GetJoint(), vecPosition);

  dsDrawSphere(vecPosition, dBodyGetRotation(pObjectList[SEGMENT_A]->GetBody()), GlobalVariable::dJointRadius);

  mJointArray[SEGMENT_E].DrawForce();

  dsDrawSphere(dBodyGetPosition(pObjectList[SEGMENT_F]->GetBody()), dBodyGetRotation(pObjectList[SEGMENT_F]->GetBody()), GlobalVariable::dJointRadius);
}

void Leg::UpdateForce(void)
{
  mJointArray[SEGMENT_A].UpdateForce();
  mJointArray[SEGMENT_D].UpdateForce();
  mJointArray[SEGMENT_E].UpdateForce();
}

void Leg::Clear(void)
{
  for (int i = 0; i < Leg::iPartCount + Leg::iDummyCount; i++)
    delete pObjectList[i];
}