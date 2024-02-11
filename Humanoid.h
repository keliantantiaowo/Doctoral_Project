#ifndef HUMANOID
#define HUMANOID

#include "Body Region.h"
#include "Tree.h"
#include "Skeleton Hierarchy.h"

class Humanoid;

class Humanoid {
public:
  static const int iHeadCount = 1;
  static const int iTorsoCount = 1;
  static const int iArmCount = 2;
  static const int iLegCount = 2;

protected:
  class Head HeadData;
  class Torso TorsoData;
  class Arm mArmArray[iArmCount];
  class Leg mLegArray[iLegCount];

public:
  explicit Humanoid(void);
  explicit Humanoid(const dReal& rdHeadRadius, const dReal& rdHeadMass, const dReal& rdTorsoRadiusA, const dReal& rdTorsoRadiusB, const dReal& rdTorsoLengthA, const dReal& rdTorsoLengthB, const dReal& rdTorsoLengthC, const dReal& rdTorsoMassA, const dReal& rdTorsoMassB, const dReal& rdTorsoMassC, const dReal& rdArmRadiusA, const dReal& rdArmRadiusB, const dReal& rdArmRadiusC, const dReal& rdArmLengthA, const dReal& rdArmLengthB, const dReal& rdArmMassA, const dReal& rdArmMassB, const dReal& rdArmMassC, const dReal& rdLegRadiusA, const dReal& rdLegRadiusB, const dReal& rdLegLengthA, const dReal& rdLegLengthB, const dReal* const& rpdLegSizeInfo, const dReal& rdLegMassA, const dReal& rdLegMassB, const dReal& rdLegMassC);
  Humanoid(const Humanoid& rHumanoidData);
  Humanoid(Humanoid&& rHumanoidData);
  virtual ~Humanoid(void);

  Humanoid& operator=(const Humanoid& rHumanoidData);
  Humanoid& operator=(Humanoid&& rHumanoidData);

  virtual const class Head& GetHead(void) const;
  virtual const class Torso& GetTorso(void) const;
  virtual const class Arm& GetArm(const int& riIndex) const;
  virtual const class Leg& GetLeg(const int& riIndex) const;

  virtual dReal GetForceMagnitude(void) const;

  virtual void DrawObject(void) const;
  virtual void DrawForce(void) const;

  virtual void UpdateForce(void);

protected:
  virtual void Initialize(void) const;
  virtual void Initialize(Tree<std::string, PostureManager>::Node* const& rpNodePointer, dReal* const& rpdPosition) const;
};

#endif