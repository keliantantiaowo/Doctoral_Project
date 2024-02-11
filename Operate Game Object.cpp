#include <stdexcept>
#include "Common.h"
#include "Simulation Setting.h"
#include "Operate Game Object.h"
#include "Pose Control.h"

void GenerateScene(void)
{
  switch (GlobalVariable::eDisplayModeSetting) {
  case Blank : break;
  case Polyhedron : DrawHumanoid(); break;
  case Force : VisualizeJointForce(); break;
  default : throw(std::invalid_argument("描画モードに不正の数値が指定されました。"));
  }
}

void CreateHumanoid(void) {
  static const dReal dHeadRadius = 0.12, dHeadMass = 5.0, dTorsoRadiusA = 0.16, dTorsoRadiusB = 5.0E-2, dTorsoLengthA = 0.4 - dTorsoRadiusA, dTorsoLengthB = 0.275 - dTorsoRadiusA, dTorsoLengthC = 8.0E-2, dTorsoMassA = 16.0, dTorsoMassB = 12.0, dTorsoMassC = 2.75, dArmRadiusA = 3.2E-2, dArmRadiusB = 3.0E-2, dArmRadiusC = 7.5E-2, dArmLengthA = 0.3, dArmLengthB = 0.24, dArmMassA = 2.5, dArmMassB = 1.8, dArmMassC = 0.6, dLegRadiusA = 6.4E-2, dLegRadiusB = 5.0E-2, dLegLengthA = 0.4, dLegLengthB = 0.36, dLegMassA = 4.5, dLegMassB = 3.6, dLegMassC = 1.2;
  static const dReal mdLegSizeInfo[COMPONENT_COUNT_3D] = {0.25, 0.1, 6.0E-2};

  GlobalVariable::HumanoidData = Humanoid(dHeadRadius, dHeadMass, dTorsoRadiusA, dTorsoRadiusB, dTorsoLengthA, dTorsoLengthB, dTorsoLengthC, dTorsoMassA, dTorsoMassB, dTorsoMassC, dArmRadiusA, dArmRadiusB, dArmRadiusC, dArmLengthA, dArmLengthB, dArmMassA, dArmMassB, dArmMassC, dLegRadiusA, dLegRadiusB, dLegLengthA, dLegLengthB, mdLegSizeInfo, dLegMassA, dLegMassB, dLegMassC);

  SetCoordinate();
  SetConnectedList();
}

void DrawHumanoid(void)
{
  GlobalVariable::HumanoidData.DrawObject();
}

void DestroyHumanoid(void)
{
  GlobalVariable::HumanoidData = Humanoid();
}

void VisualizeJointForce(void)
{
  GlobalVariable::HumanoidData.DrawForce();
}