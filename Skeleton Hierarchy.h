#ifndef SKELETON_HIERARCHY
#define SKELETON_HIERARCHY

#include <string>
#include "Definition.h"
#include "Tree.h"

class PostureManager {
protected:
  dBodyID rbBody;
  dQuaternion quatPosture;

public:
  explicit PostureManager(void);
  explicit PostureManager(const dBodyID& rrbInitializer, const dQuaternion& rquatInitializer);
  PostureManager(const PostureManager& rPostureManagerData);
  PostureManager(PostureManager&& rPostureManagerData);
  virtual ~PostureManager(void);

  PostureManager& operator=(const PostureManager& rPostureManagerData);
  PostureManager& operator=(PostureManager&& rPostureManagerData);

  dBodyID GetBody(void) const;
  const dReal* const GetPosture(void) const;

  void SetBody(const dBodyID& rrbNewBody);
  void SetPosture(const dReal* const& rpdNewPosture);
};

const std::string& PartName(const int& riIndex);

Tree<std::string, PostureManager>& SkeletonTree(void);

Tree<std::string, PostureManager>&& BuildSkeletonStructure(void);

void BindSkeletonStructure(const void* const& rpvPointer);

void ObtainPosture(void);

void ObtainPosture(Tree<std::string, PostureManager>::Node* const& rpNodePointer);

void RestrictPosture(void);

void RestrictPosture(const Tree<std::string, PostureManager>::Node* const& rpNodePointer);

dReal GetBoundingBox(void);

void GetBoundingBox(const Tree<std::string, PostureManager>::Node* const& rpNodePointer, dReal& rdBase);

#endif