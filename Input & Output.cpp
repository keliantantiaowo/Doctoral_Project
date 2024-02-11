#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Common.h"
#include "Input & Output.h"

void ReadDataFile(void)
{
  std::ifstream ifsFile;
}

void WriteDataFile(void)
{
  std::ofstream ofsFile;

  ofsFile.open(GlobalVariable::strFileName);

  if (!ofsFile.is_open()) {
    std::cerr << "ファイル「" << GlobalVariable::strFileName << "」を開けませんでした。" << std::endl;

    std::exit(EXIT_FAILURE);
  }

  ofsFile << "頭部 左肩 左肘 右肩 右肘 腰部 左股関節 左膝 左踝 右股関節 右膝 右踝 負荷合計値" << std::endl;

  for (int i = 0; i < Head::iJointCount; i++)
    ofsFile << dJointGetHingeAngle(GlobalVariable::HumanoidData.GetHead().GetJoint(i)) << ' ' << std::flush;

  for (int i = 0; i < Humanoid::iArmCount; i++)
    for (int j = 0; j < Arm::iJointCount - 1; j++)
      ofsFile << dJointGetHingeAngle(GlobalVariable::HumanoidData.GetArm(i).GetJoint(j)) << ' ' << std::flush;

  for (int i = 0; i < Torso::iJointCount - 1; i++)
    ofsFile << dJointGetHingeAngle(GlobalVariable::HumanoidData.GetTorso().GetJoint(i)) << ' ' << std::flush;

  for (int i = 0; i < Humanoid::iArmCount; i++)
    for (int j = 0; j < Leg::iJointCount; j++)
      ofsFile << dJointGetHingeAngle(GlobalVariable::HumanoidData.GetArm(i).GetJoint(j)) << ' ' << std::flush;

  ofsFile << GlobalVariable::dTotalMagnitude << std::endl;

  ofsFile.close();
}