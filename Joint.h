#ifndef JOINT
#define JOINT

#include <queue>
#include "Definition.h"

class Joint;

class Joint {
public:
  static const int iDataCapacity = static_cast<int>(5.0E2);

protected:
  dJointID jtJoint;
  dJointType jtType;
  dJointFeedback fbSensor;
  bool bSensorOn;
  std::queue<dReal> qForceList;
  dReal dForceMagnitude;

public:
  explicit Joint(void);
  explicit Joint(const dJointType& rjtType, const bool& rbSensorOn);
  Joint(const Joint& rJointData);
  Joint(Joint&& rJointData);
  virtual ~Joint(void);

  Joint& operator=(const Joint& rJointData);
  Joint& operator=(Joint&& rJointData);

  virtual dJointID GetJoint(void) const;
  virtual dJointType GetType(void) const;
  virtual dJointFeedback GetSensor(void) const;
  virtual dReal GetForceMagnitude(void) const;

  virtual void DrawForce(void) const;

  virtual void UpdateForce(void);

protected:
  virtual void Initialize(void) const;

  virtual void Clear(void);
};

#endif