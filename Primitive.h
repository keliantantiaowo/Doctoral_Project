#ifndef PRIMITIVE
#define PRIMITIVE

#include "Definition.h"

class Object;
class Sphere;
class Box;
class Cylinder;
class Capsule;
class Dummy;

class Object {
protected:
  dBodyID rbBody;
  dGeomID geomGeometry;
  dReal dPhysicalMass;
  float mfColor[COMPONENT_COUNT_3D];

public:
  explicit Object(void);
  explicit Object(const dBodyID& rrbBody, const dGeomID& rgeomGeometry, const dReal& rdPhysicalMass);
  Object(const Object& rObjectData);
  Object(Object&& rObjectData);
  virtual ~Object(void);

  virtual dBodyID GetBody(void) const;
  virtual dGeomID GetGeometry(void) const;

  virtual dReal GetMass(void) const;
  virtual void SetMass(const dReal& rdNewPhysicalMass);

  virtual float GetColor(const int& riOffSet) const;
  virtual void SetColor(const float* const& rpfNewColor);

  virtual void DrawObject(void) const;

protected:
  virtual void Clear(void);
};

class Sphere : public Object {
protected:
  dReal dRadius;

public:
  explicit Sphere(void);
  explicit Sphere(const dReal& rdRadius, const dReal& rdPhysaicalMass);
  Sphere(const Sphere& rSphereData);
  Sphere(Sphere&& rSphereData);
  virtual ~Sphere(void);

  Sphere& operator=(const Sphere& rSphereData);
  Sphere& operator=(Sphere&& rSphereData);

  virtual dReal GetRadius(void) const;
  virtual void SetRadius(const dReal& rdNewRadius);

  virtual void DrawObject(void) const;

protected:
  virtual void Initialize(void) const;
};

class Box : public Object {
protected:
  dReal mdSizeInfo[COMPONENT_COUNT_3D];

public:
  explicit Box(void);
  explicit Box(const dReal* const& rpdSizeInfo, const dReal& rdPhysaicalMass);
  Box(const Box& rBoxData);
  Box(Box&& rBoxData);
  virtual ~Box(void);

  Box& operator=(const Box& rBoxData);
  Box& operator=(Box&& rBoxData);

  virtual dReal GetSizeInfo(const int& riOffSet) const;
  virtual void SetSizeInfo(const dReal* const& rpdNewSizeInfo);

  virtual void DrawObject(void) const;

protected:
  virtual void Initialize(void) const;
};

class Cylinder : public Object {
protected:
  dReal dRadius;
  dReal dLength;

public:
  explicit Cylinder(void);
  explicit Cylinder(const dReal& rdRadius, const dReal& rdLength, const dReal& rdPhysaicalMass);
  Cylinder(const Cylinder& rCylinderData);
  Cylinder(Cylinder&& rCylinderData);
  virtual ~Cylinder(void);

  Cylinder& operator=(const Cylinder& rCylinderData);
  Cylinder& operator=(Cylinder&& rCylinderData);

  virtual dReal GetRadius(void) const;
  virtual void SetRadius(const dReal& rdNewRadius);
  virtual dReal GetLength(void) const;
  virtual void SetLength(const dReal& rdNewLength);

  virtual void DrawObject(void) const;

protected:
  virtual void Initialize(void) const;
};

class Capsule : public Object {
protected:
  dReal dRadius;
  dReal dLength;

public:
  explicit Capsule(void);
  explicit Capsule(const dReal& rdRadius, const dReal& rdLength, const dReal& rdPhysaicalMass);
  Capsule(const Capsule& rCapsuleData);
  Capsule(Capsule&& rCapsuleData);
  virtual ~Capsule(void);

  Capsule& operator=(const Capsule& rCapsuleData);
  Capsule& operator=(Capsule&& rCapsuleData);

  virtual dReal GetRadius(void) const;
  virtual void SetRadius(const dReal& rdNewRadius);
  virtual dReal GetLength(void) const;
  virtual void SetLength(const dReal& rdNewLength);

  virtual void DrawObject(void) const;

protected:
  virtual void Initialize(void) const;
};

class Dummy : public Object {
public:
  static constexpr dReal dEpsilon = 1.0E-5;

  explicit Dummy(void);
  explicit Dummy(const bool& rbInstantiation);
  Dummy(const Dummy& rDummyData);
  Dummy(Dummy&& rDummyData);
  virtual ~Dummy(void);

  Dummy& operator=(const Dummy& rDummyData);
  Dummy& operator=(Dummy&& rDummyData);

private:
  virtual void DrawObject(void) const;

protected:
  virtual void Initialize(void) const;
};

#endif