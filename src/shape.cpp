#include "shape.hpp"

using namespace glm;
using namespace std;

AbstractShape::AbstractShape(unique_ptr<AbstractMaterial> &&material) :
    mMaterial(move(material)) {}

vec3 AbstractShape::getReflectedRadiance(const Ray &ray) {
    return mMaterial->getReflectedRadiance(ray, *this);
}

Sphere::Sphere(const vec3 &position, float radius) :
    AbstractShape(), mPosition(position), mRadius(radius) {}

Sphere::Sphere(const vec3 &position, float radius, unique_ptr<AbstractMaterial> &&material) :
    AbstractShape(move(material)), mPosition(position), mRadius(radius) {}

float Sphere::intersect(const Ray &ray) const {
    float radiusSquare = mRadius * mRadius;

    vec3 posToRo = ray.origin - mPosition;

    float b = dot(ray.direction, posToRo); // b / 2.0
    float c = dot(posToRo, posToRo) - radiusSquare;

    float delta = b * b - c; // (b² - 4ac) / 4.0 : does not need divide by 4

    if(delta < 0.0f)
        return -1.f;

    float disc = sqrtf(delta);

    float near = (-b - disc);
    float far = (-b + disc);

    if(near < 0.0f)
        near = far;

    return near < ray.distMax ? near : -1.0f;
}

vec3 Sphere::getNormal(const vec3 &position) const {
    return (position - mPosition) / mRadius;
}

Plane::Plane(const vec4 &equation) :
    AbstractShape(), mEquation(equation) {}

Plane::Plane(const vec4 &equation, unique_ptr<AbstractMaterial> &&material) :
    AbstractShape(move(material)), mEquation(equation) {}

float Plane::intersect(const Ray &ray) const {
   vec3 equation3(mEquation);

   float dist = -(mEquation.w + dot(ray.origin, equation3)) / dot(ray.direction, equation3);

   return dist < ray.distMax ? dist : -1.0f;
}

vec3 Plane::getNormal(const vec3 &) const {
    return normalize(mEquation.xyz());
}
