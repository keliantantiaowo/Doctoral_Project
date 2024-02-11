#include "Common.h"
#include "Simulation Setting.h"
#include "Operate Game Object.h"
#include "Pose Control.h"

int main(int iArgumentCount, char** pchArgumentVector)
{
  SetDrawStuff();

  dInitODE2(0U);

  GlobalVariable::wdWorld = dWorldCreate();

  dWorldSetGravity(GlobalVariable::wdWorld, 0.0, 0.0, -GRAVITATIONAL_ACCELERATION);
  dWorldSetERP(GlobalVariable::wdWorld, 1.0);
  dWorldSetCFM(GlobalVariable::wdWorld, 1.0E-12);

  GlobalVariable::spcSpace = dHashSpaceCreate(0);
  GlobalVariable::geomGround = dCreatePlane(GlobalVariable::spcSpace, 0.0, 0.0, 1.0, 0.0);
  GlobalVariable::jgContactGroup = dJointGroupCreate(0);

  InitializeAngle();

  CreateHumanoid();

  dsSimulationLoop(iArgumentCount, pchArgumentVector, 1280, 960, &GlobalVariable::fnFunction);

  DestroyHumanoid();

  dJointGroupDestroy(GlobalVariable::jgContactGroup);
  dSpaceDestroy(GlobalVariable::spcSpace);
  dWorldDestroy(GlobalVariable::wdWorld);

  dCloseODE();
}