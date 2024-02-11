#ifndef SIMULATION_SETTING
#define SIMULATION_SETTING

#include <vector>
#include <utility>
#include "Definition.h"

void SetDrawStuff(void);

void Start(void);

void Loop(int iPause);

void Restart(void);

void Command(int iKey);

void SimulationProcessing(void);

void CallBack(void* pvPointer, dGeomID geomColliderA, dGeomID geomColliderB);

bool CheckDetectionCondition(const dBodyID& rrbBodyA, const dBodyID& rrbBodyB);

std::vector<std::pair<dBodyID, dBodyID>>& GetConnectedList(void);

void SetConnectedList(void);

#endif