#include <random>
#include <limits>
#include "Tool.h"

void Normalize(dReal* const& rpdVector, const dReal& rdNorm)
{
  dReal dDistance = 0.0;

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    dDistance += std::pow(rpdVector[i], 2.0);

  dDistance = std::sqrt(dDistance);

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    rpdVector[i] *= rdNorm / dDistance;
}

bool CheckLimit(dReal& rdValue, const dReal& rdNorm)
{
  if (std::fabs(rdValue) > rdNorm) {
    rdValue *= rdNorm / std::fabs(rdValue);

    return true;
  } else
    return false;
}

bool CheckLimit(dReal* const& rpdVector, const dReal& rdNorm)
{
  dReal dDistance = 0.0;

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    dDistance += std::pow(rpdVector[i], 2.0);

  if (dDistance = std::sqrt(dDistance), rdNorm < dDistance) {
    for (int i = 0; i < COMPONENT_COUNT_3D; i++)
      rpdVector[i] *= rdNorm / dDistance;

    return true;
  } else
    return false;
}

dReal DotProduct(const dReal* const& rpdVectorA, const dReal* const& rpdVectorB)
{
  dReal dResult = 0.0;

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    dResult += rpdVectorA[i] * rpdVectorB[i];

  return dResult;
}

void CrossProduct(const dReal* const& rpdVectorA, const dReal* const& rpdVectorB, dReal* const& rpdResult)
{
  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    rpdResult[i] = rpdVectorA[(i + 1) % COMPONENT_COUNT_3D] * rpdVectorB[(i + COMPONENT_COUNT_3D - 1) % COMPONENT_COUNT_3D] - rpdVectorA[(i + COMPONENT_COUNT_3D - 1) % COMPONENT_COUNT_3D] * rpdVectorB[(i + 1) % COMPONENT_COUNT_3D];
}

void ComposeQuaternion(dReal* const& rpdOriginal, const dReal* const& rpdApplied)
{
  dQuaternion quatTemp;

  dQMultiply0(quatTemp, rpdOriginal, rpdApplied);

  for (int i = 0; i < COMPONENT_COUNT_HOMOGENEOUS; i++)
    rpdOriginal[i] = quatTemp[i];
}

dReal HandleSingularPoint(const dReal& rdTarget, const dReal& rdAngle)
{
  dReal dDifference = std::fabs(rdTarget - rdAngle);

  if (!std::signbit(rdTarget - rdAngle))
    if (std::fabs(rdTarget - rdAngle - M_PI) > dDifference)
      if (std::fabs(rdTarget - rdAngle - M_PI * 2.0) > dDifference)
        return rdAngle;
      else
        return rdAngle + M_PI * 2.0;
    else if (std::fabs(rdTarget - rdAngle - M_PI) > std::fabs(rdTarget - rdAngle - M_PI * 2.0))
      return rdAngle + M_PI * 2.0;
    else
      return rdAngle + M_PI;
  else
    if (std::fabs(rdTarget - rdAngle + M_PI) > dDifference)
      if (std::fabs(rdTarget - rdAngle + M_PI * 2.0) > dDifference)
        return rdAngle;
      else
        return rdAngle - M_PI * 2.0;
    else if (std::fabs(rdTarget - rdAngle + M_PI) > std::fabs(rdTarget - rdAngle + M_PI * 2.0))
      return rdAngle - M_PI * 2.0;
    else
      return rdAngle - M_PI;
}

dReal Randomize(void)
{
  static std::mt19937 othGenerator((std::random_device())());
  static std::uniform_real_distribution<dReal> othDistribution(0.0, std::nextafter(1.0, std::numeric_limits<dReal>::infinity()));

  return othDistribution(othGenerator);
}