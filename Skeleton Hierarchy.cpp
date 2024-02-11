#include <vector>
#include <limits>
#include "Common.h"
#include "Skeleton Hierarchy.h"
#include "Tool.h"

PostureManager::PostureManager(void) : rbBody(nullptr), quatPosture{dReal(), dReal(), dReal(), dReal()}
{
}

PostureManager::PostureManager(const dBodyID& rrbInitializer, const dQuaternion& rquatInitializer) : rbBody(rrbInitializer), quatPosture{rquatInitializer[SEGMENT_A], rquatInitializer[SEGMENT_B], rquatInitializer[SEGMENT_C], rquatInitializer[SEGMENT_D]}
{
}

PostureManager::PostureManager(const PostureManager& rPostureManagerData) : rbBody(rPostureManagerData.rbBody), quatPosture{rPostureManagerData.quatPosture[SEGMENT_A], rPostureManagerData.quatPosture[SEGMENT_B], rPostureManagerData.quatPosture[SEGMENT_C], rPostureManagerData.quatPosture[SEGMENT_D]}
{
}

PostureManager::PostureManager(PostureManager&& rPostureManagerData) : rbBody(std::move(rPostureManagerData.rbBody)), quatPosture{rPostureManagerData.quatPosture[SEGMENT_A], rPostureManagerData.quatPosture[SEGMENT_B], rPostureManagerData.quatPosture[SEGMENT_C], rPostureManagerData.quatPosture[SEGMENT_D]}
{
}

PostureManager::~PostureManager(void)
{
}

PostureManager& PostureManager::operator=(const PostureManager& rPostureManagerData)
{
  if (&rPostureManagerData == this)
    return *this;

  rbBody = rPostureManagerData.rbBody;

  for (int i = 0; i < COMPONENT_COUNT_HOMOGENEOUS; i++)
    quatPosture[i] = rPostureManagerData.quatPosture[i];

  return *this;
}

PostureManager& PostureManager::operator=(PostureManager&& rPostureManagerData)
{
  if (&rPostureManagerData == this)
    return *this;

  rbBody = std::move(rPostureManagerData.rbBody);

  for (int i = 0; i < COMPONENT_COUNT_HOMOGENEOUS; i++)
    quatPosture[i] = rPostureManagerData.quatPosture[i];

  return *this;
}

dBodyID PostureManager::GetBody(void) const
{
  return rbBody;
}

const dReal* const PostureManager::GetPosture(void) const
{
  return quatPosture;
}

void PostureManager::SetBody(const dBodyID& rrbNewBody)
{
  rbBody = rrbNewBody;
}

void PostureManager::SetPosture(const dReal* const& rpdNewPosture)
{
  for (int i = 0; i < COMPONENT_COUNT_HOMOGENEOUS; i++)
    quatPosture[i] = rpdNewPosture[i];
}

const std::string& PartName(const int& riIndex)
{
  static const std::vector<std::string> vecNameList{"頭部", "頸部", "胸部", "腹部", "左上腕", "左前腕", "左手", "右上腕", "右手", "左大腿", "左下腿", "左足", "右大腿", "右下腿", "右足"};

  return vecNameList[riIndex];
}

Tree<std::string, PostureManager>& SkeletonTree(void)
{
  static Tree<std::string, PostureManager> TreeData(BuildSkeletonStructure());

  return TreeData;
}

Tree<std::string, PostureManager>&& BuildSkeletonStructure(void)
{
  static Tree<std::string, PostureManager> TreeData("腹部", PostureManager());

  TreeData.Insert("腹部", "胸部", PostureManager());
  TreeData.Insert("腹部", "左大腿", PostureManager());
  TreeData.Insert("腹部", "右大腿", PostureManager());
  TreeData.Insert("胸部", "頸部", PostureManager());
  TreeData.Insert("胸部", "左上腕", PostureManager());
  TreeData.Insert("胸部", "右上腕", PostureManager());
  TreeData.Insert("頸部", "頭部", PostureManager());
  TreeData.Insert("左上腕", "左前腕", PostureManager());
  TreeData.Insert("左前腕", "左手", PostureManager());
  TreeData.Insert("右上腕", "右前腕", PostureManager());
  TreeData.Insert("右前腕", "右手", PostureManager());
  TreeData.Insert("左大腿", "左下腿", PostureManager());
  TreeData.Insert("左下腿", "左足", PostureManager());
  TreeData.Insert("右大腿", "右下腿", PostureManager());
  TreeData.Insert("右下腿", "右足", PostureManager());

  return std::move(TreeData);
}

void BindSkeletonStructure(const void* const& rpvPointer)
{
  const Humanoid* const& rpHumanoidPointer = reinterpret_cast<const Humanoid* const&>(rpvPointer);

  SkeletonTree()["頭部"].SetBody(rpHumanoidPointer->GetHead().GetObject(SEGMENT_A).GetBody());
  SkeletonTree()["頸部"].SetBody(rpHumanoidPointer->GetTorso().GetObject(SEGMENT_C).GetBody());
  SkeletonTree()["胸部"].SetBody(rpHumanoidPointer->GetTorso().GetObject(SEGMENT_A).GetBody());
  SkeletonTree()["腹部"].SetBody(rpHumanoidPointer->GetTorso().GetObject(SEGMENT_B).GetBody());
  SkeletonTree()["左上腕"].SetBody(rpHumanoidPointer->GetArm(SEGMENT_A).GetObject(SEGMENT_A).GetBody());
  SkeletonTree()["左前腕"].SetBody(rpHumanoidPointer->GetArm(SEGMENT_A).GetObject(SEGMENT_B).GetBody());
  SkeletonTree()["左手"].SetBody(rpHumanoidPointer->GetArm(SEGMENT_A).GetObject(SEGMENT_C).GetBody());
  SkeletonTree()["右上腕"].SetBody(rpHumanoidPointer->GetArm(SEGMENT_B).GetObject(SEGMENT_A).GetBody());
  SkeletonTree()["右前腕"].SetBody(rpHumanoidPointer->GetArm(SEGMENT_B).GetObject(SEGMENT_B).GetBody());
  SkeletonTree()["右手"].SetBody(rpHumanoidPointer->GetArm(SEGMENT_B).GetObject(SEGMENT_C).GetBody());
  SkeletonTree()["左大腿"].SetBody(rpHumanoidPointer->GetLeg(SEGMENT_A).GetObject(SEGMENT_A).GetBody());
  SkeletonTree()["左下腿"].SetBody(rpHumanoidPointer->GetLeg(SEGMENT_A).GetObject(SEGMENT_B).GetBody());
  SkeletonTree()["左足"].SetBody(rpHumanoidPointer->GetLeg(SEGMENT_A).GetObject(SEGMENT_C).GetBody());
  SkeletonTree()["右大腿"].SetBody(rpHumanoidPointer->GetLeg(SEGMENT_B).GetObject(SEGMENT_A).GetBody());
  SkeletonTree()["右下腿"].SetBody(rpHumanoidPointer->GetLeg(SEGMENT_B).GetObject(SEGMENT_B).GetBody());
  SkeletonTree()["右足"].SetBody(rpHumanoidPointer->GetLeg(SEGMENT_B).GetObject(SEGMENT_C).GetBody());
}

void ObtainPosture(void)
{
  ObtainPosture(SkeletonTree().GetRoot());
}

void ObtainPosture(Tree<std::string, PostureManager>::Node* const& rpNodePointer)
{
  dQuaternion quatResult, quatAddition;
  bool bMultiplication = false;

  if (rpNodePointer->GetKey() == "頭部") {
    dQFromAxisAndAngle(quatResult, 1.0, 0.0, 0.0, GlobalVariable::mdHeadAngle[SEGMENT_A]);
    dQFromAxisAndAngle(quatAddition, 0.0, 1.0, 0.0, GlobalVariable::mdHeadAngle[SEGMENT_B]);

    ComposeQuaternion(quatResult, quatAddition);

    dQFromAxisAndAngle(quatAddition, 0.0, 0.0, 1.0, GlobalVariable::mdHeadAngle[SEGMENT_C]);

    ComposeQuaternion(quatResult, quatAddition);

    bMultiplication = true;
  } else if (rpNodePointer->GetKey() == "頸部")
    rpNodePointer->GetValue().SetPosture(rpNodePointer->GetParent()->GetValue().GetPosture());
  else if (rpNodePointer->GetKey() == "胸部") {
    dQFromAxisAndAngle(quatResult, 1.0, 0.0, 0.0, GlobalVariable::mdTorsoAngle[SEGMENT_A]);
    dQFromAxisAndAngle(quatAddition, 0.0, 1.0, 0.0, GlobalVariable::mdTorsoAngle[SEGMENT_B]);

    ComposeQuaternion(quatResult, quatAddition);

    dQFromAxisAndAngle(quatAddition, 0.0, 0.0, 1.0, GlobalVariable::mdTorsoAngle[SEGMENT_C]);

    ComposeQuaternion(quatResult, quatAddition);

    bMultiplication = true;
  } else if (rpNodePointer->GetKey() == "腹部") {
    dQFromAxisAndAngle(quatResult, 0.0, 1.0, 0.0, GlobalVariable::mdRootAngle[SEGMENT_A]);
    dQFromAxisAndAngle(quatAddition, 0.0, 0.0, 1.0, GlobalVariable::mdRootAngle[SEGMENT_B]);

    ComposeQuaternion(quatResult, quatAddition);

    rpNodePointer->GetValue().SetPosture(quatResult);
  } else if (rpNodePointer->GetKey() == "左上腕") {
    dQFromAxisAndAngle(quatResult, 0.0, 0.0, 1.0, GlobalVariable::mdArmAngle[SEGMENT_A][SEGMENT_C]);
    dQFromAxisAndAngle(quatAddition, 0.0, 1.0, 0.0, GlobalVariable::mdArmAngle[SEGMENT_A][SEGMENT_B]);

    ComposeQuaternion(quatResult, quatAddition);

    dQFromAxisAndAngle(quatAddition, 1.0, 0.0, 0.0, GlobalVariable::mdArmAngle[SEGMENT_A][SEGMENT_A] + M_PI_2);

    ComposeQuaternion(quatResult, quatAddition);

    bMultiplication = true;
  } else if (rpNodePointer->GetKey() == "左前腕") {
    dQFromAxisAndAngle(quatResult, 0.0, 1.0, 0.0, GlobalVariable::mdArmAngle[SEGMENT_A][SEGMENT_D]);

    bMultiplication = true;
  } else if (rpNodePointer->GetKey() == "左手")
    rpNodePointer->GetValue().SetPosture(rpNodePointer->GetParent()->GetValue().GetPosture());
  else if (rpNodePointer->GetKey() == "右上腕") {
    dQFromAxisAndAngle(quatResult, 0.0, 0.0, 1.0, GlobalVariable::mdArmAngle[SEGMENT_B][SEGMENT_C]);
    dQFromAxisAndAngle(quatAddition, 0.0, 1.0, 0.0, GlobalVariable::mdArmAngle[SEGMENT_B][SEGMENT_B]);

    ComposeQuaternion(quatResult, quatAddition);

    dQFromAxisAndAngle(quatAddition, 1.0, 0.0, 0.0, GlobalVariable::mdArmAngle[SEGMENT_B][SEGMENT_A] + M_PI_2);

    ComposeQuaternion(quatResult, quatAddition);

    bMultiplication = true;
  } else if (rpNodePointer->GetKey() == "右前腕") {
    dQFromAxisAndAngle(quatResult, 0.0, 0.0, 1.0, GlobalVariable::mdArmAngle[SEGMENT_B][SEGMENT_D]);

    bMultiplication = true;
  } else if (rpNodePointer->GetKey() == "右手")
    rpNodePointer->GetValue().SetPosture(rpNodePointer->GetParent()->GetValue().GetPosture());
  else if (rpNodePointer->GetKey() == "左大腿") {
    dQFromAxisAndAngle(quatResult, 0.0, 0.0, 1.0, GlobalVariable::mdLegAngle[SEGMENT_A][SEGMENT_C]);
    dQFromAxisAndAngle(quatAddition, 0.0, 1.0, 0.0, GlobalVariable::mdLegAngle[SEGMENT_A][SEGMENT_B]);

    ComposeQuaternion(quatResult, quatAddition);

    dQFromAxisAndAngle(quatAddition, 1.0, 0.0, 0.0, GlobalVariable::mdLegAngle[SEGMENT_A][SEGMENT_A]);

    ComposeQuaternion(quatResult, quatAddition);

    bMultiplication = true;
  } else if (rpNodePointer->GetKey() == "左下腿") {
    dQFromAxisAndAngle(quatResult, 0.0, 1.0, 0.0, GlobalVariable::mdLegAngle[SEGMENT_A][SEGMENT_D]);

    bMultiplication = true;
  } else if (rpNodePointer->GetKey() == "左足") {
    dQFromAxisAndAngle(quatResult, 0.0, 1.0, 0.0, GlobalVariable::mdLegAngle[SEGMENT_A][SEGMENT_F]);
    dQFromAxisAndAngle(quatAddition, 1.0, 0.0, 0.0, GlobalVariable::mdLegAngle[SEGMENT_A][SEGMENT_E]);

    ComposeQuaternion(quatResult, quatAddition);

    bMultiplication = true;
  } else if (rpNodePointer->GetKey() == "右大腿") {
    dQFromAxisAndAngle(quatResult, 0.0, 0.0, 1.0, GlobalVariable::mdLegAngle[SEGMENT_B][SEGMENT_C]);
    dQFromAxisAndAngle(quatAddition, 0.0, 1.0, 0.0, GlobalVariable::mdLegAngle[SEGMENT_B][SEGMENT_B]);

    ComposeQuaternion(quatResult, quatAddition);

    dQFromAxisAndAngle(quatAddition, 1.0, 0.0, 0.0, GlobalVariable::mdLegAngle[SEGMENT_B][SEGMENT_A]);

    ComposeQuaternion(quatResult, quatAddition);

    bMultiplication = true;
  } else if (rpNodePointer->GetKey() == "右下腿") {
    dQFromAxisAndAngle(quatResult, 0.0, 1.0, 0.0, GlobalVariable::mdLegAngle[SEGMENT_B][SEGMENT_D]);

    bMultiplication = true;
  } else if (rpNodePointer->GetKey() == "右足") {
    dQFromAxisAndAngle(quatResult, 0.0, 1.0, 0.0, GlobalVariable::mdLegAngle[SEGMENT_B][SEGMENT_F]);
    dQFromAxisAndAngle(quatAddition, 1.0, 0.0, 0.0, GlobalVariable::mdLegAngle[SEGMENT_B][SEGMENT_E]);

    ComposeQuaternion(quatResult, quatAddition);

    bMultiplication = true;
  }

  if (bMultiplication) {
    ComposeQuaternion(quatResult, rpNodePointer->GetParent()->GetValue().GetPosture());

    rpNodePointer->GetValue().SetPosture(quatResult);
  }

  for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
    ObtainPosture(*i);
}

void RestrictPosture(void)
{
  RestrictPosture(SkeletonTree().GetRoot());
}

void RestrictPosture(const Tree<std::string, PostureManager>::Node* const& rpNodePointer)
{
  dBodySetQuaternion(rpNodePointer->GetValue().GetBody(), rpNodePointer->GetValue().GetPosture());

  for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
    RestrictPosture(*i);
}

dReal GetBoundingBox(void)
{
  dReal dBase = std::numeric_limits<dReal>::max();

  GetBoundingBox(SkeletonTree().GetRoot(), dBase);

  return dBase;
}

void GetBoundingBox(const Tree<std::string, PostureManager>::Node* const& rpNodePointer, dReal& rdBase)
{
  dReal mdAxisAlignedBoundingBox[GlobalVariable::iRangeCount * COMPONENT_COUNT_3D] = {0.0};
  dReal mdTemp = 0.0;

  dGeomGetAABB(dBodyGetFirstGeom(rpNodePointer->GetValue().GetBody()), mdAxisAlignedBoundingBox);

  if (mdTemp = mdAxisAlignedBoundingBox[GlobalVariable::iRangeCount * OFFSET_Z + static_cast<int>(Lower)], mdTemp < rdBase)
    rdBase = mdTemp;

  for (std::list<Tree<std::string, PostureManager>::Node*>::const_iterator i = rpNodePointer->GetChildren().begin(); i != rpNodePointer->GetChildren().end(); i++)
    GetBoundingBox(*i, rdBase);
}