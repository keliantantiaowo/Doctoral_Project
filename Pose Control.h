#ifndef WALKING_CONTROL
#define WALKING_CONTROL

#include <cstdlib>
#include "Definition.h"

void InitializeAngle(void);

void CalculateAngle(void);

void ConstrainAngle(void);

void InverseKinematics(void);

void InverseKinematics(const dReal* const& rpdTipPosition, dReal* const& rpdAngle, const eSide& reSideSetting);

void InverseKinematics(const dReal* const& rpdTipPosition, const dReal* const& rpdTipPosture, dReal* const& rpdAngle, const eSide& reSideSetting);

void ProportionalDifferentialControl(void);

void SetCoordinate(void);

void SetJointParameter(dJointID jtJoint, const dReal& rdTarget);

void SustainState(dBodyID rbBody);

dReal AggregateMagnitude(void);

dReal MotionRange(const eBodyPart& reBodyPartSetting, const eRangeMode& reRangeModeSetting, const int& riIndex);

dReal MotionRange(const eBodyPart& reBodyPartSetting, const eRangeMode& reRangeModeSetting, const eSide& reSideSetting, const int& riIndex);

#endif