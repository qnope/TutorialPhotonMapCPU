#include "material.hpp"
#include "shape.hpp"
#include "world.hpp"

using namespace glm;
using namespace std;

vec3 getIrradianceFromDirectLighting(vec3 position, vec3 normal) {
    vec3 irradiance;

    for(auto &light : World::world.lights)
        irradiance += light->getIrradiance(position, normal);

    return irradiance;
}

AbstractMaterial::AbstractMaterial(float albedo) :
    albedo(albedo){}

LambertianMaterial::LambertianMaterial(float albedo) :
    AbstractMaterial(albedo) {}

float LambertianMaterial::brdf(const glm::vec3 &, const glm::vec3 &, glm::vec3 const &) {
    return albedo / M_PI;
}

UniformLambertianMaterial::UniformLambertianMaterial(const vec3 &color, float albedo) :
    LambertianMaterial(albedo), color(color) {}

vec3 UniformLambertianMaterial::getReflectedRadiance(Ray const &ray, AbstractShape const &shape) {
    vec3 directLighting = getIrradianceFromDirectLighting(ray.origin, shape.getNormal(ray.origin));
    float f = brdf(vec3(), vec3(), vec3());

    return color * f * (directLighting + World::world.gatherIrradiance(ray.origin, shape.getNormal(ray.origin), 0.5f));
}

void UniformLambertianMaterial::bouncePhoton(const Photon &_photon, const AbstractShape &shape) {
    Photon photon = _photon;

    if(photon.recursionDeep > 0)
        World::world.addPhoton(_photon);

    if(++photon.recursionDeep > MAX_BOUNCES)
        return;

    photon.flux *= color * brdf(vec3(), vec3(), vec3());
    photon.direction = Random::random.getHemisphereDirection(shape.getNormal(photon.position));
    tracePhoton(photon);
}

MirrorMaterial::MirrorMaterial(float albedo) :
    AbstractMaterial(albedo) {}

float MirrorMaterial::brdf(const vec3 &, const vec3 &, const vec3 &) {
    return albedo;
}

vec3 MirrorMaterial::getReflectedRadiance(const Ray &ray, const AbstractShape &shape) {
    if(ray.recursionDeep >= MAX_BOUNCES)
        return vec3();

    Ray reflectedRay = getReflectedRay(ray, shape.getNormal(ray.origin));

    return brdf(vec3(), vec3(), vec3()) * getRadianceFromNearest(reflectedRay);
}

void MirrorMaterial::bouncePhoton(const Photon &_photon, const AbstractShape &shape) {
    Photon photon = _photon;

    if(++photon.recursionDeep > MAX_BOUNCES)
        return;

    if(photon.recursionDeep == 1)
        traceShadowPhoton(photon);

    photon.flux *= brdf(vec3(), vec3(), vec3());
    photon.direction = getReflectedDir(photon.direction, shape.getNormal(photon.position));
    tracePhoton(photon);
}

TransmitterMaterial::TransmitterMaterial(float albedo, float index) :
    AbstractMaterial(albedo), index(index) {}

float TransmitterMaterial::brdf(const vec3 &, const vec3 &, const vec3 &) {
    return albedo;
}

vec3 TransmitterMaterial::getReflectedRadiance(const Ray &ray, const AbstractShape &shape) {
    if(ray.recursionDeep >= MAX_BOUNCES)
        return vec3();

    Ray refractedRay = getRefractedRay(ray, shape.getNormal(ray.origin), index);

    return brdf(vec3(), vec3(), vec3()) * getRadianceFromNearest(refractedRay);
}

void TransmitterMaterial::bouncePhoton(const Photon &_photon, const AbstractShape &shape) {
    Photon photon = _photon;

    if(++photon.recursionDeep > MAX_BOUNCES)
        return;

    if(photon.recursionDeep == 1)
        traceShadowPhoton(photon);

    photon.flux *= brdf(vec3(), vec3(), vec3());
    photon.direction = getRefractedDir(photon.direction, shape.getNormal(photon.position), index);
    tracePhoton(photon);
}
