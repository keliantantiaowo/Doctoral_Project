#include <utility>
#include <stdexcept>
#include "Common.h"
#include "Joint.h"

Joint::Joint(void) : jtJoint(nullptr), jtType(dJointType()), fbSensor(dJointFeedback()), bSensorOn(false), qForceList(), dForceMagnitude(dReal())
{
  Initialize();
}

Joint::Joint(const dJointType& rjtType, const bool& rbSensorOn) : jtJoint(rjtType == dJointTypeBall ? dJointCreateBall(GlobalVariable::wdWorld, 0)
  : rjtType == dJointTypeHinge ? dJointCreateHinge(GlobalVariable::wdWorld, 0)
  : rjtType == dJointTypeSlider ? dJointCreateSlider(GlobalVariable::wdWorld, 0)
  : rjtType == dJointTypeFixed ? dJointCreateFixed(GlobalVariable::wdWorld, 0)
  : throw(std::invalid_argument("指定されたジョイントタイプが存在しません。"))), jtType(rjtType), fbSensor(dJointFeedback()), bSensorOn(rbSensorOn), qForceList(), dForceMagnitude(dReal())
{
  Initialize();
}

Joint::Joint(const Joint& rJointData) : jtJoint(rJointData.GetType() == dJointTypeBall ? dJointCreateBall(GlobalVariable::wdWorld, 0)
  : rJointData.GetType() == dJointTypeHinge ? dJointCreateHinge(GlobalVariable::wdWorld, 0)
  : rJointData.GetType() == dJointTypeSlider ? dJointCreateSlider(GlobalVariable::wdWorld, 0)
  : rJointData.GetType() == dJointTypeFixed ? dJointCreateFixed(GlobalVariable::wdWorld, 0)
  : throw(std::invalid_argument("指定されたジョイントタイプが存在しません。"))), jtType(rJointData.GetType()), fbSensor(rJointData.GetSensor()), bSensorOn(rJointData.bSensorOn), qForceList(rJointData.qForceList), dForceMagnitude(rJointData.dForceMagnitude)
{
  Initialize();
}

Joint::Joint(Joint&& rJointData) : jtJoint(std::move(rJointData.jtJoint)), jtType(rJointData.GetType()), fbSensor(std::move(rJointData.fbSensor)), bSensorOn(rJointData.bSensorOn), qForceList(std::move(rJointData.qForceList)), dForceMagnitude(rJointData.dForceMagnitude)
{
  Initialize();

  rJointData.jtJoint = nullptr;
}

Joint::~Joint(void)
{
  Clear();
}

Joint& Joint::operator=(const Joint& rJointData)
{
  if (&rJointData == this)
    return *this;

  Clear();

  switch (rJointData.GetType()) {
  case dJointTypeBall : jtJoint = dJointCreateBall(GlobalVariable::wdWorld, 0); break;
  case dJointTypeHinge : jtJoint = dJointCreateHinge(GlobalVariable::wdWorld, 0); break;
  case dJointTypeSlider : jtJoint = dJointCreateSlider(GlobalVariable::wdWorld, 0); break;
  case dJointTypeFixed : jtJoint = dJointCreateFixed(GlobalVariable::wdWorld, 0); break;
  default : throw(std::invalid_argument("指定されたジョイントタイプが存在しません。"));
  }

  jtType = rJointData.GetType();
  fbSensor = rJointData.GetSensor();
  bSensorOn = rJointData.bSensorOn;
  qForceList = rJointData.qForceList;
  dForceMagnitude = rJointData.dForceMagnitude;

  Initialize();

  return *this;
}

Joint& Joint::operator=(Joint&& rJointData)
{
  if (&rJointData == this)
    return *this;

  Clear();

  jtJoint = std::move(rJointData.jtJoint);
  jtType = rJointData.GetType();
  fbSensor = std::move(rJointData.fbSensor);
  bSensorOn = rJointData.bSensorOn;
  qForceList = std::move(rJointData.qForceList);
  dForceMagnitude = rJointData.dForceMagnitude;

  Initialize();

  rJointData.jtJoint = nullptr;

  return *this;
}

dJointID Joint::GetJoint(void) const
{
  return jtJoint;
}

dJointType Joint::GetType(void) const
{
  return jtType;
}

dJointFeedback Joint::GetSensor(void) const
{
  return fbSensor;
}

dReal Joint::GetForceMagnitude(void) const
{
  return dForceMagnitude;
}

void Joint::DrawForce(void) const
{
  if (jtJoint == nullptr)
    ;
  else if (bSensorOn) {
    dReal dColor = std::signbit(dForceMagnitude / GlobalVariable::dForceLimit - 1.0) ? (1.0F - static_cast<float>(dForceMagnitude / GlobalVariable::dForceLimit)) * 0.8F + 0.2F : 0.2F;

    dsSetColor(1.0F, dColor, dColor);
  }
}

void Joint::UpdateForce(void)
{
  if (jtJoint == nullptr)
    ;
  else if (bSensorOn) {
    dForceMagnitude *= static_cast<dReal>(qForceList.size());

    qForceList.push(std::sqrt(std::pow(fbSensor.f1[OFFSET_X], 2.0) + std::pow(fbSensor.f1[OFFSET_Y], 2.0) + std::pow(fbSensor.f1[OFFSET_Z], 2.0)));

    if (std::signbit(Joint::iDataCapacity - static_cast<int>(qForceList.size()))) {
      dForceMagnitude -= qForceList.front();

      qForceList.pop();
    }

    (dForceMagnitude += qForceList.back()) /= static_cast<dReal>(qForceList.size());
  }
}

void Joint::Initialize(void) const
{
  if (jtJoint == nullptr)
    ;
  else if (bSensorOn)
    dJointSetFeedback(jtJoint, const_cast<dJointFeedback*>(&fbSensor));
}

void Joint::Clear(void)
{
  if (jtJoint == nullptr)
    ;
  else
    dJointDestroy(jtJoint);
}