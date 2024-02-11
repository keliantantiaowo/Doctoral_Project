#ifndef BODY_REGION
#define BODY_REGION

#include "Primitive.h"
#include "Joint.h"

class Base;
class Head;
class Torso;
class Arm;
class Leg;

class Base {
public:
  explicit Base(void);
  Base(const Base& rBaseData) = delete;
  Base(Base&& rBaseData) = delete;
  virtual ~Base(void);

  Base& operator=(const Base& rBaseData) = delete;
  Base& operator=(Base&& rBaseData) = delete;

  virtual const Object& GetObject(const int& riIndex) const = 0;
  virtual dJointID GetJoint(const int& riIndex) const = 0;
  virtual dReal GetForceMagnitude(const int& riIndex) const = 0;

  virtual void DrawObject(void) const = 0;
  virtual void DrawForce(void) const = 0;

  virtual void UpdateForce(void) = 0;

protected:
  virtual void Clear(void) = 0;
};

class Head : public Base {
public:
  static const int iPartCount = 1;
  static const int iDummyCount = 2;
  static const int iJointCount = 3;

protected:
  Object* pObjectList[iPartCount + iDummyCount];
  Joint mJointArray[iJointCount];

public:
  explicit Head(void);
  explicit Head(const dReal& rdRadius, const dReal& rdPhysicalMass);
  Head(const Head& rHeadData);
  Head(Head&& rHeadData);
  virtual ~Head(void);

  Head& operator=(const Head& rHeadData);
  Head& operator=(Head&& rHeadData);

  virtual const Object& GetObject(const int& riIndex) const;
  virtual dJointID GetJoint(const int& riIndex) const;
  virtual dReal GetForceMagnitude(const int& riIndex) const;

  virtual void DrawObject(void) const;
  virtual void DrawForce(void) const;

  virtual void UpdateForce(void);

protected:
  virtual void Clear(void);
};

class Torso : public Base {
public:
  static const int iPartCount = 3;
  static const int iDummyCount = 2;
  static const int iJointCount = 4;

protected:
  Object* pObjectList[iPartCount + iDummyCount];
  Joint mJointArray[iJointCount];

public:
  explicit Torso(void);
  explicit Torso(const dReal& rdRadiusA, const dReal& rdRadiusB, const dReal& rdLengthA, const dReal& rdLengthB, const dReal& rdLengthC, const dReal& rdPhysicalMassA, const dReal& rdPhysicalMassB, const dReal& rdPhysicalMassC);
  Torso(const Torso& rTorsoData);
  Torso(Torso&& rTorsoData);
  virtual ~Torso(void);

  Torso& operator=(const Torso& rTorsoData);
  Torso& operator=(Torso&& rTorsoData);

  virtual const Object& GetObject(const int& riIndex) const;
  virtual dJointID GetJoint(const int& riIndex) const;
  virtual dReal GetForceMagnitude(const int& riIndex) const;

  virtual void DrawObject(void) const;
  virtual void DrawForce(void) const;

  virtual void UpdateForce(void);

protected:
  virtual void Clear(void);
};

class Arm : public Base {
public:
  static const int iPartCount = 3;
  static const int iDummyCount = 2;
  static const int iJointCount = 5;

protected:
  Object* pObjectList[iPartCount + iDummyCount];
  Joint mJointArray[iJointCount];

public:
  explicit Arm(void);
  explicit Arm(const dReal& rdRadiusA, const dReal& rdRadiusB, const dReal& rdRadiusC, const dReal& rdLengthA, const dReal& rdLengthB, const dReal& rdPhysicalMassA, const dReal& rdPhysicalMassB, const dReal& rdPhysicalMassC);
  Arm(const Arm& rArmData);
  Arm(Arm&& rArmData);
  virtual ~Arm(void);

  Arm& operator=(const Arm& rArmData);
  Arm& operator=(Arm&& rArmData);

  virtual const Object& GetObject(const int& riIndex) const;
  virtual dJointID GetJoint(const int& riIndex) const;
  virtual dReal GetForceMagnitude(const int& riIndex) const;

  virtual void DrawObject(void) const;
  virtual void DrawForce(void) const;

  virtual void UpdateForce(void);

protected:
  virtual void Clear(void);
};

class Leg {
public:
  static const int iPartCount = 3;
  static const int iDummyCount = 3;
  static const int iJointCount = 6;

protected:
  Object* pObjectList[iPartCount + iDummyCount];
  Joint mJointArray[iJointCount];

public:
  explicit Leg(void);
  explicit Leg(const dReal& rdRadiusA, const dReal& rdRadiusB, const dReal& rdLengthA, const dReal& rdLengthB, const dReal* const& rpdSizeInfo, const dReal& rdPhysicalMassA, const dReal& rdPhysicalMassB, const dReal& rdPhysicalMassC);
  Leg(const Leg& rLegData);
  Leg(Leg&& rLegData);
  virtual ~Leg(void);

  Leg& operator=(const Leg& rLegData);
  Leg& operator=(Leg&& rLegData);

  virtual const Object& GetObject(const int& riIndex) const;
  virtual dJointID GetJoint(const int& riIndex) const;
  virtual dReal GetForceMagnitude(const int& riIndex) const;

  virtual void DrawObject(void) const;
  virtual void DrawForce(void) const;

  virtual void UpdateForce(void);

protected:
  virtual void Clear(void);
};

#endif