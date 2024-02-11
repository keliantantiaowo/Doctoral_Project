#ifndef DEFINITION
#define DEFINITION

#define _USE_MATH_DEFINES

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#ifdef dDOUBLE
#define dsDrawSphere dsDrawSphereD
#define dsDrawBox dsDrawBoxD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#define dsDrawTriangle dsDrawTriangleD
#define dsDrawLine dsDrawLineD
#endif

#define GRAVITATIONAL_ACCELERATION 9.80665
#define FILE_EXTENSION ".txt"
#define COMPONENT_COUNT_2D 2
#define COMPONENT_COUNT_3D 3
#define COMPONENT_COUNT_HOMOGENEOUS 4
#define OFFSET_X 0
#define OFFSET_Y 1
#define OFFSET_Z 2
#define SEGMENT_A 0
#define SEGMENT_B 1
#define SEGMENT_C 2
#define SEGMENT_D 3
#define SEGMENT_E 4
#define SEGMENT_F 5
#define SEGMENT_G 6
#define SEGMENT_H 7

enum eDirection {None, XAxis, YAxis, ZAxis};
enum eSide {Up, Down, Left, Right};
enum eSimulationMode {Play, Pause, Stop};
enum eDisplayMode {Blank, Polyhedron, Force};
enum eRotationMode {Clockwise, Anticlockwise};
enum eRangeMode {Lower, Upper};
enum eBodyPart {Head, Torso, Arm, Leg, Other};
enum eMovability {Movable, Fixed};

#endif