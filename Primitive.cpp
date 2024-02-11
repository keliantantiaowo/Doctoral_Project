#include <utility>
#include <cmath>
#include "Common.h"
#include "Primitive.h"

Object::Object(void) : rbBody(nullptr), geomGeometry(nullptr), dPhysicalMass(int()), mfColor{float(), float(), float()}
{
}

Object::Object(const dBodyID& rrbBody, const dGeomID& rgeomGeometry, const dReal& rdPhysicalMass) : rbBody(rrbBody), geomGeometry(rgeomGeometry), dPhysicalMass(rdPhysicalMass), mfColor{1.0F, 1.0F, 1.0F}
{
}

Object::Object(const Object& rObjectData) : rbBody(rObjectData.GetBody()), geomGeometry(rObjectData.GetGeometry()), dPhysicalMass(rObjectData.GetMass()), mfColor{rObjectData.GetColor(OFFSET_X), rObjectData.GetColor(OFFSET_Y), rObjectData.GetColor(OFFSET_Z)}
{
}

Object::Object(Object&& rObjectData) : rbBody(std::move(rObjectData.rbBody)), geomGeometry(std::move(rObjectData.geomGeometry)), dPhysicalMass(rObjectData.GetMass()), mfColor{rObjectData.GetColor(OFFSET_X), rObjectData.GetColor(OFFSET_Y), rObjectData.GetColor(OFFSET_Z)}
{
  rObjectData.rbBody = nullptr;
  rObjectData.geomGeometry = nullptr;
}

Object::~Object(void)
{
  Clear();
}

dBodyID Object::GetBody(void) const
{
  return rbBody;
}

dGeomID Object::GetGeometry(void) const
{
  return geomGeometry;
}

dReal Object::GetMass(void) const
{
  return dPhysicalMass;
}

void Object::SetMass(const dReal& rdNewPhysicalMass)
{
  dPhysicalMass = rdNewPhysicalMass;
}

float Object::GetColor(const int& riOffSet) const
{
  return mfColor[riOffSet];
}

void Object::SetColor(const float* const& rpfNewColor)
{
  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    mfColor[i] = rpfNewColor[i];
}

void Object::DrawObject(void) const
{
  dsSetColor(mfColor[OFFSET_X], mfColor[OFFSET_Y], mfColor[OFFSET_Z]);
}

void Object::Clear(void)
{
  if (rbBody == nullptr)
    ;
  else
    dBodyDestroy(rbBody);

  if (geomGeometry == nullptr)
    ;
  else
    dGeomDestroy(geomGeometry);
}

Sphere::Sphere(void) : Object(), dRadius(dReal())
{
}

Sphere::Sphere(const dReal& rdRadius, const dReal& rdPhysicalMass) : Object(dBodyCreate(GlobalVariable::wdWorld), dCreateSphere(GlobalVariable::spcSpace, rdRadius), rdPhysicalMass), dRadius(rdRadius)
{
  Initialize();
}

Sphere::Sphere(const Sphere& rSphereData) : Object(dBodyCreate(GlobalVariable::wdWorld), dCreateSphere(GlobalVariable::spcSpace, rSphereData.GetRadius()), rSphereData.GetMass()), dRadius(rSphereData.GetRadius())
{
  Initialize();
}

Sphere::Sphere(Sphere&& rSphereData) : Object(std::move(rSphereData.rbBody), std::move(rSphereData.geomGeometry), rSphereData.GetMass()), dRadius(rSphereData.GetRadius())
{
  rSphereData.rbBody = nullptr;
  rSphereData.geomGeometry = nullptr;
}

Sphere::~Sphere(void)
{
}

Sphere& Sphere::operator=(const Sphere& rSphereData)
{
  if (&rSphereData == this)
    return *this;

  Clear();

  rbBody = dBodyCreate(GlobalVariable::wdWorld);
  geomGeometry = dCreateSphere(GlobalVariable::spcSpace, rSphereData.GetRadius());

  SetRadius(rSphereData.GetRadius());
  SetMass(rSphereData.GetMass());

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    mfColor[i] = rSphereData.mfColor[i];

  Initialize();

  return *this;
}

Sphere& Sphere::operator=(Sphere&& rSphereData)
{
  if (&rSphereData == this)
    return *this;

  Clear();

  rbBody = std::move(rSphereData.rbBody);
  geomGeometry = std::move(rSphereData.geomGeometry);

  SetRadius(rSphereData.GetRadius());
  SetMass(rSphereData.GetMass());

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    mfColor[i] = rSphereData.mfColor[i];

  rSphereData.rbBody = nullptr;
  rSphereData.geomGeometry = nullptr;

  return *this;
}

dReal Sphere::GetRadius(void) const
{
  return dRadius;
}

void Sphere::SetRadius(const dReal& rdNewRadius)
{
  dRadius = rdNewRadius;
}

void Sphere::DrawObject(void) const
{
  Object::DrawObject();

  dsDrawSphere(dBodyGetPosition(rbBody), dBodyGetRotation(rbBody), dRadius);
}

void Sphere::Initialize(void) const
{
  dMass msMass;

  dMassSetZero(&msMass);
  dMassSetSphereTotal(&msMass, dPhysicalMass, dRadius);
  dBodySetMass(rbBody, &msMass);
  dGeomSetBody(geomGeometry, rbBody);
}

Box::Box(void) : Object(), mdSizeInfo{dReal(), dReal(), dReal()}
{
}

Box::Box(const dReal* const& rpdSizeInfo, const dReal& rdPhysicalMass) : Object(dBodyCreate(GlobalVariable::wdWorld), dCreateBox(GlobalVariable::spcSpace, rpdSizeInfo[OFFSET_X], rpdSizeInfo[OFFSET_Y], rpdSizeInfo[OFFSET_Z]), rdPhysicalMass), mdSizeInfo{rpdSizeInfo[OFFSET_X], rpdSizeInfo[OFFSET_Y], rpdSizeInfo[OFFSET_Z]}
{
  Initialize();
}

Box::Box(const Box& rBoxData) : Object(dBodyCreate(GlobalVariable::wdWorld), dCreateBox(GlobalVariable::spcSpace, rBoxData.GetSizeInfo(OFFSET_X), rBoxData.GetSizeInfo(OFFSET_Y), rBoxData.GetSizeInfo(OFFSET_Z)), rBoxData.GetMass()), mdSizeInfo{rBoxData.GetSizeInfo(OFFSET_X), rBoxData.GetSizeInfo(OFFSET_Y), rBoxData.GetSizeInfo(OFFSET_Z)}
{
  Initialize();
}

Box::Box(Box&& rBoxData) : Object(std::move(rBoxData.rbBody), std::move(rBoxData.geomGeometry), rBoxData.GetMass()), mdSizeInfo{rBoxData.GetSizeInfo(OFFSET_X), GetSizeInfo(OFFSET_Y), rBoxData.GetSizeInfo(OFFSET_Z)}
{
  rBoxData.rbBody = nullptr;
  rBoxData.geomGeometry = nullptr;
}

Box::~Box(void)
{
}

Box& Box::operator=(const Box& rBoxData)
{
  if (&rBoxData == this)
    return *this;

  Clear();

  rbBody = dBodyCreate(GlobalVariable::wdWorld);
  geomGeometry = dCreateBox(GlobalVariable::spcSpace, rBoxData.GetSizeInfo(OFFSET_X), rBoxData.GetSizeInfo(OFFSET_Y), rBoxData.GetSizeInfo(OFFSET_Z));

  SetSizeInfo(rBoxData.mdSizeInfo);
  SetMass(rBoxData.GetMass());

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    mfColor[i] = rBoxData.mfColor[i];

  Initialize();

  return *this;
}

Box& Box::operator=(Box&& rBoxData)
{
  if (&rBoxData == this)
    return *this;

  Clear();

  rbBody = std::move(rBoxData.rbBody);
  geomGeometry = std::move(rBoxData.geomGeometry);

  SetSizeInfo(rBoxData.mdSizeInfo);
  SetMass(rBoxData.GetMass());

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    mfColor[i] = rBoxData.mfColor[i];

  rBoxData.rbBody = nullptr;
  rBoxData.geomGeometry = nullptr;

  return *this;
}

dReal Box::GetSizeInfo(const int& riOffSet) const
{
  return mdSizeInfo[riOffSet];
}

void Box::SetSizeInfo(const dReal* const& rpdNewSizeInfo)
{
  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    mdSizeInfo[i] = rpdNewSizeInfo[i];
}

void Box::DrawObject(void) const
{
  Object::DrawObject();

  dsDrawBox(dBodyGetPosition(rbBody), dBodyGetRotation(rbBody), mdSizeInfo);
}

void Box::Initialize(void) const
{
  dMass msMass;

  dMassSetZero(&msMass);
  dMassSetBoxTotal(&msMass, dPhysicalMass, mdSizeInfo[OFFSET_X], mdSizeInfo[OFFSET_Y], mdSizeInfo[OFFSET_Z]);
  dBodySetMass(rbBody, &msMass);
  dGeomSetBody(geomGeometry, rbBody);
}

Cylinder::Cylinder(void) : Object(), dRadius(dReal()), dLength(dReal())
{
}

Cylinder::Cylinder(const dReal& rdRadius, const dReal& rdLength, const dReal& rdPhysicalMass) : Object(dBodyCreate(GlobalVariable::wdWorld), dCreateCylinder(GlobalVariable::spcSpace, rdRadius, rdLength), rdPhysicalMass), dRadius(rdRadius), dLength(rdLength)
{
  Initialize();
}

Cylinder::Cylinder(const Cylinder& rCylinderData) : Object(dBodyCreate(GlobalVariable::wdWorld), dCreateCylinder(GlobalVariable::spcSpace, rCylinderData.dRadius, rCylinderData.GetLength()), rCylinderData.GetMass()), dRadius(rCylinderData.GetRadius()), dLength(rCylinderData.GetLength())
{
  Initialize();
}

Cylinder::Cylinder(Cylinder&& rCylinderData) : Object(std::move(rCylinderData.rbBody), std::move(rCylinderData.geomGeometry), rCylinderData.GetMass()), dRadius(rCylinderData.GetRadius()), dLength(rCylinderData.GetLength())
{
  rCylinderData.rbBody = nullptr;
  rCylinderData.geomGeometry = nullptr;
}

Cylinder::~Cylinder(void)
{
}

Cylinder& Cylinder::operator=(const Cylinder& rCylinderData)
{
  if (&rCylinderData == this)
    return *this;

  Clear();

  rbBody = dBodyCreate(GlobalVariable::wdWorld);
  geomGeometry = dCreateCylinder(GlobalVariable::spcSpace, rCylinderData.GetRadius(), rCylinderData.GetLength());

  SetRadius(rCylinderData.GetRadius());
  SetLength(dLength = rCylinderData.GetLength());
  SetMass(rCylinderData.GetMass());

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    mfColor[i] = rCylinderData.mfColor[i];

  Initialize();

  return *this;
}

Cylinder& Cylinder::operator=(Cylinder&& rCylinderData)
{
  if (&rCylinderData == this)
    return *this;

  Clear();

  rbBody = std::move(rCylinderData.rbBody);
  geomGeometry = std::move(rCylinderData.geomGeometry);

  SetRadius(rCylinderData.GetRadius());
  SetLength(dLength = rCylinderData.GetLength());
  SetMass(rCylinderData.GetMass());

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    mfColor[i] = rCylinderData.mfColor[i];

  rCylinderData.rbBody = nullptr;
  rCylinderData.geomGeometry = nullptr;

  return *this;
}

dReal Cylinder::GetRadius(void) const
{
  return dRadius;
}

void Cylinder::SetRadius(const dReal& rdNewRadius)
{
  dRadius = rdNewRadius;
}

dReal Cylinder::GetLength(void) const
{
  return dLength;
}

void Cylinder::SetLength(const dReal& rdNewLength)
{
  dLength = rdNewLength;
}

void Cylinder::DrawObject(void) const
{
  Object::DrawObject();

  dsDrawCylinder(dBodyGetPosition(rbBody), dBodyGetRotation(rbBody), dLength, dRadius);
}

void Cylinder::Initialize(void) const
{
  dMass msMass;

  dMassSetZero(&msMass);
  dMassSetCylinderTotal(&msMass, dPhysicalMass, ZAxis, dRadius, dLength);
  dBodySetMass(rbBody, &msMass);
  dGeomSetBody(geomGeometry, rbBody);
}

Capsule::Capsule(void) : Object(), dRadius(dReal()), dLength(dReal())
{
}

Capsule::Capsule(const dReal& rdRadius, const dReal& rdLength, const dReal& rdPhysicalMass) : Object(dBodyCreate(GlobalVariable::wdWorld), dCreateCapsule(GlobalVariable::spcSpace, rdRadius, rdLength), rdPhysicalMass), dRadius(rdRadius), dLength(rdLength)
{
  Initialize();
}

Capsule::Capsule(const Capsule& rCapsuleData) : Object(dBodyCreate(GlobalVariable::wdWorld), dCreateCapsule(GlobalVariable::spcSpace, rCapsuleData.GetRadius(), rCapsuleData.GetLength()), rCapsuleData.GetMass()), dRadius(rCapsuleData.GetRadius()), dLength(rCapsuleData.GetLength())
{
  Initialize();
}

Capsule::Capsule(Capsule&& rCapsuleData) : Object(std::move(rCapsuleData.rbBody), std::move(rCapsuleData.geomGeometry), rCapsuleData.GetMass()), dRadius(rCapsuleData.GetRadius()), dLength(rCapsuleData.GetLength())
{
  rCapsuleData.rbBody = nullptr;
  rCapsuleData.geomGeometry = nullptr;
}

Capsule::~Capsule(void)
{
}

Capsule& Capsule::operator=(const Capsule& rCapsuleData)
{
  if (&rCapsuleData == this)
    return *this;

  Clear();

  rbBody = dBodyCreate(GlobalVariable::wdWorld);
  geomGeometry = dCreateCapsule(GlobalVariable::spcSpace, rCapsuleData.GetRadius(), rCapsuleData.GetLength());

  SetRadius(rCapsuleData.GetRadius());
  SetLength(rCapsuleData.GetLength());
  SetMass(rCapsuleData.GetMass());

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    mfColor[i] = rCapsuleData.mfColor[i];

  Initialize();

  return *this;
}

Capsule& Capsule::operator=(Capsule&& rCapsuleData)
{
  if (&rCapsuleData == this)
    return *this;

  Clear();

  rbBody = std::move(rCapsuleData.rbBody);
  geomGeometry = std::move(rCapsuleData.geomGeometry);

  SetRadius(rCapsuleData.GetRadius());
  SetLength(rCapsuleData.GetLength());
  SetMass(rCapsuleData.GetMass());

  for (int i = 0; i < COMPONENT_COUNT_3D; i++)
    mfColor[i] = rCapsuleData.mfColor[i];

  rCapsuleData.rbBody = nullptr;
  rCapsuleData.geomGeometry = nullptr;

  return *this;
}

dReal Capsule::GetRadius(void) const
{
  return dRadius;
}

void Capsule::SetRadius(const dReal& rdNewRadius)
{
  dRadius = rdNewRadius;
}

dReal Capsule::GetLength(void) const
{
  return dLength;
}

void Capsule::SetLength(const dReal& rdNewLength)
{
  dLength = rdNewLength;
}

void Capsule::DrawObject(void) const
{
  Object::DrawObject();

  dsDrawCapsule(dBodyGetPosition(rbBody), dBodyGetRotation(rbBody), dLength, dRadius);
}

void Capsule::Initialize(void) const
{
  dMass msMass;

  dMassSetZero(&msMass);
  dMassSetCapsuleTotal(&msMass, dPhysicalMass, ZAxis, dRadius, dLength);
  dBodySetMass(rbBody, &msMass);
  dGeomSetBody(geomGeometry, rbBody);
}

Dummy::Dummy(void) : Object()
{
}

Dummy::Dummy(const bool& rbInstantiation) : Object(dBodyCreate(GlobalVariable::wdWorld), nullptr, Dummy::dEpsilon)
{
  Initialize();
}

Dummy::Dummy(const Dummy& rDummyData) : Object(dBodyCreate(GlobalVariable::wdWorld), nullptr, Dummy::dEpsilon)
{
  Initialize();
}

Dummy::Dummy(Dummy&& rDummyData) : Object(std::move(rDummyData.rbBody), std::move(rDummyData.geomGeometry), Dummy::dEpsilon)
{
  rDummyData.rbBody = nullptr;
}

Dummy::~Dummy(void)
{
}

Dummy& Dummy::operator=(const Dummy& rDummyData)
{
  if (&rDummyData == this)
    return *this;

  Clear();

  rbBody = dBodyCreate(GlobalVariable::wdWorld);

  Initialize();

  return *this;
}

Dummy& Dummy::operator=(Dummy&& rDummyData)
{
  if (&rDummyData == this)
    return *this;

  Clear();

  rbBody = std::move(rDummyData.rbBody);
  rDummyData.rbBody = nullptr;

  return *this;
}

void Dummy::DrawObject(void) const
{
}

void Dummy::Initialize(void) const
{
  dMass msMass;

  dMassSetZero(&msMass);
  dMassSetSphereTotal(&msMass, Dummy::dEpsilon, Dummy::dEpsilon);
  dBodySetMass(rbBody, &msMass);
}