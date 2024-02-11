#include <iostream>
#include <stdexcept>
#include "Common.h"
#include "Simulation Setting.h"
#include "Operate Game Object.h"
#include "Pose Control.h"

void SetDrawStuff(void)
{
  GlobalVariable::fnFunction.version = DS_VERSION;
  GlobalVariable::fnFunction.start = Start;
  GlobalVariable::fnFunction.step = Loop;
  GlobalVariable::fnFunction.command = Command;
  GlobalVariable::fnFunction.path_to_textures = "../../drawstuff/textures";
}

void Start(void)
{
  static float mfViewpointPosition[COMPONENT_COUNT_3D] = {2.5F, 0.0F, 1.0F};
  static float mfViewDirection[COMPONENT_COUNT_3D] = {1.8E2F, 0.0F, 0.0F};

  dsSetViewpoint(mfViewpointPosition, mfViewDirection);
  dsSetSphereQuality(4);
  dsSetCapsuleQuality(5);
}

void Loop(int iPause)
{
  if (!iPause) {
    for (int i = 0; i < GlobalVariable::iSimulationRate; i++)
      SimulationProcessing();

    std::cout << AggregateMagnitude() << std::endl;
  }

  GenerateScene();
}

void Restart(void)
{
  for (int i = 0; i < COMPONENT_COUNT_3D - 1; i++)
    GlobalVariable::mdRootAngle[i] = 0.0;

  for (int i = 0; i < Head::iJointCount; i++)
    GlobalVariable::mdHeadAngle[i] = 0.0;

  for (int i = 0; i < Torso::iJointCount - 1; i++)
    GlobalVariable::mdTorsoAngle[i] = 0.0;

  for (int i = 0; i < Humanoid::iArmCount; i++)
    for (int j = 0; j < Arm::iJointCount - 1; j++)
    GlobalVariable::mdArmAngle[i][j] = 0.0;

  for (int i = 0; i < Humanoid::iLegCount; i++)
    for (int j = 0; j < Leg::iJointCount; j++)
      GlobalVariable::mdLegAngle[i][j] = 0.0;

  DestroyHumanoid();

  dJointGroupDestroy(GlobalVariable::jgContactGroup);

  GlobalVariable::jgContactGroup = dJointGroupCreate(0);

  InitializeAngle();

  CreateHumanoid();
}

void Command(int iKey)
{
  switch (iKey) {
  case 'E' :
  case 'e' :
    if (GlobalVariable::eDisplayModeSetting == Polyhedron)
      GlobalVariable::eDisplayModeSetting = Force;
    else if (GlobalVariable::eDisplayModeSetting == Force)
      GlobalVariable::eDisplayModeSetting = Polyhedron;

    break;
  case 'R' :
  case 'r' : Restart(); break;
  case ' ' :
  default : break;
  }
}

void SimulationProcessing(void)
{
  ConstrainAngle();

  dSpaceCollide(GlobalVariable::spcSpace, nullptr, CallBack);

  GlobalVariable::HumanoidData.UpdateForce();

  dWorldStep(GlobalVariable::wdWorld, GlobalVariable::dInterval / static_cast<dReal>(GlobalVariable::iSimulationRate));

  dJointGroupEmpty(GlobalVariable::jgContactGroup);
}

void CallBack(void* pvPointer, dGeomID geomColliderA, dGeomID geomColliderB)
{
  if (GlobalVariable::geomGround == geomColliderA || GlobalVariable::geomGround == geomColliderB) {
    dContact mconContactList[GlobalVariable::iContactLimit];
    int iContactCount = dCollide(geomColliderA, geomColliderB, GlobalVariable::iContactLimit, &mconContactList->geom, sizeof(dContact));

    for (int i = 0; i < iContactCount; i++) {
      mconContactList[i].surface.mu = dInfinity;
      mconContactList[i].surface.mode = dContactSoftERP | dContactSoftCFM | dContactApprox1;
      mconContactList[i].surface.soft_erp = 1.0;
      mconContactList[i].surface.soft_cfm = 0.0;
      mconContactList[i].surface.bounce = 0.0;
      mconContactList[i].surface.bounce_vel = 0.0;

      dJointID jtContact = dJointCreateContact(GlobalVariable::wdWorld, GlobalVariable::jgContactGroup, mconContactList + i);

      dJointAttach(jtContact, dGeomGetBody(mconContactList[i].geom.g1), dGeomGetBody(mconContactList[i].geom.g2));
    }
  }
}

bool CheckDetectionCondition(const dBodyID& rrbBodyA, const dBodyID& rrbBodyB)
{
  const std::vector<std::pair<dBodyID, dBodyID>>& rvecConnectedList = GetConnectedList();

  for (int i = 0; i < rvecConnectedList.size(); i++)
    if (rrbBodyA == rvecConnectedList[i].first && rrbBodyB == rvecConnectedList[i].second)
      return true;
    else if (rrbBodyA == rvecConnectedList[i].second && rrbBodyB == rvecConnectedList[i].first)
      return true;

  return false;
}

std::vector<std::pair<dBodyID, dBodyID>>& GetConnectedList(void)
{
  static std::vector<std::pair<dBodyID, dBodyID>> vecConnectedList;

  return vecConnectedList;
}

void SetConnectedList(void)
{
  GetConnectedList() = std::vector<std::pair<dBodyID, dBodyID>>{
    {GlobalVariable::HumanoidData.GetHead().GetObject(SEGMENT_A).GetBody(), GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_C).GetBody()},
    {GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_A).GetBody(), GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_C).GetBody()},
    {GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_A).GetBody(), GlobalVariable::HumanoidData.GetArm(SEGMENT_A).GetObject(SEGMENT_A).GetBody()},
    {GlobalVariable::HumanoidData.GetArm(SEGMENT_A).GetObject(SEGMENT_A).GetBody(), GlobalVariable::HumanoidData.GetArm(SEGMENT_A).GetObject(SEGMENT_B).GetBody()},
    {GlobalVariable::HumanoidData.GetArm(SEGMENT_A).GetObject(SEGMENT_B).GetBody(), GlobalVariable::HumanoidData.GetArm(SEGMENT_A).GetObject(SEGMENT_C).GetBody()},
    {GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_A).GetBody(), GlobalVariable::HumanoidData.GetArm(SEGMENT_B).GetObject(SEGMENT_A).GetBody()},
    {GlobalVariable::HumanoidData.GetArm(SEGMENT_B).GetObject(SEGMENT_A).GetBody(), GlobalVariable::HumanoidData.GetArm(SEGMENT_B).GetObject(SEGMENT_B).GetBody()},
    {GlobalVariable::HumanoidData.GetArm(SEGMENT_B).GetObject(SEGMENT_B).GetBody(), GlobalVariable::HumanoidData.GetArm(SEGMENT_B).GetObject(SEGMENT_C).GetBody()},
    {GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_A).GetBody(), GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_B).GetBody()},
    {GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_B).GetBody(), GlobalVariable::HumanoidData.GetLeg(SEGMENT_A).GetObject(SEGMENT_A).GetBody()},
    {GlobalVariable::HumanoidData.GetLeg(SEGMENT_A).GetObject(SEGMENT_A).GetBody(), GlobalVariable::HumanoidData.GetLeg(SEGMENT_A).GetObject(SEGMENT_B).GetBody()},
    {GlobalVariable::HumanoidData.GetLeg(SEGMENT_A).GetObject(SEGMENT_B).GetBody(), GlobalVariable::HumanoidData.GetLeg(SEGMENT_A).GetObject(SEGMENT_C).GetBody()},
    {GlobalVariable::HumanoidData.GetTorso().GetObject(SEGMENT_B).GetBody(), GlobalVariable::HumanoidData.GetLeg(SEGMENT_B).GetObject(SEGMENT_A).GetBody()},
    {GlobalVariable::HumanoidData.GetLeg(SEGMENT_B).GetObject(SEGMENT_A).GetBody(), GlobalVariable::HumanoidData.GetLeg(SEGMENT_B).GetObject(SEGMENT_B).GetBody()},
    {GlobalVariable::HumanoidData.GetLeg(SEGMENT_B).GetObject(SEGMENT_B).GetBody(), GlobalVariable::HumanoidData.GetLeg(SEGMENT_B).GetObject(SEGMENT_C).GetBody()}
  };
}