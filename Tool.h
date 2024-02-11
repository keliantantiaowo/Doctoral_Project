#ifndef TOOL
#define TOOL

#include <cstdlib>
#include "Definition.h"

template <typename Type, std::size_t szSize> constexpr inline std::size_t GetArraySize(const Type (&rmvarArray)[szSize])
{
  return szSize;
}

void Normalize(dReal* const& rpdVector, const dReal& rdNorm);

bool CheckLimit(dReal& rdValue, const dReal& rdNorm);

bool CheckLimit(dReal* const& rpdVector, const dReal& rdNorm);

dReal DotProduct(const dReal* const& rpdVectorA, const dReal* const& rpdVectorB);

void CrossProduct(const dReal* const& rpdVectorA, const dReal* const& rpdVectorB, dReal* const& rpdResult);

void ComposeQuaternion(dReal* const& rpdOriginal, const dReal* const& rpdApplied);

dReal HandleSingularPoint(const dReal& rdTarget, const dReal& rdAngle);

dReal Randomize(void);

#endif