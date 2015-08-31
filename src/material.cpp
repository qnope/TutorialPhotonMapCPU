#include "material.hpp"
#include "shape.hpp"
#include "world.hpp"

using namespace glm;
using namespace std;

vec3 getIrradianceFromDirectLighting(Ray const &ray, AbstractShape const &shape) {
    vec3 position = ray.origin + ray.direction * ray.distMax;
    vec3 normal = shape.getNormal(position);
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
    vec3 directLighting = getIrradianceFromDirectLighting(ray, shape);
    float f = brdf(vec3(), vec3(), vec3());

    return color * f * (directLighting);
}

MirrorMaterial::MirrorMaterial(float albedo) :
    AbstractMaterial(albedo) {}

float MirrorMaterial::brdf(const vec3 &, const vec3 &, const vec3 &) {
    return albedo;
}

vec3 MirrorMaterial::getReflectedRadiance(const Ray &ray, const AbstractShape &shape) {
    if(ray.recursionDeep >= MAX_BOUNCES)
        return vec3();

    Ray reflectedRay = getReflectedRay(ray, shape.getNormal(ray.origin + ray.direction * ray.distMax));

    auto nearest = World::world.findNearest(reflectedRay);

    if(get<0>(nearest) != nullptr) {
        reflectedRay.distMax = get<1>(nearest);
        return brdf(vec3(), vec3(), vec3()) * get<0>(nearest)->getReflectedRadiance(reflectedRay);
    }

    return vec3();
}

TransmitterMaterial::TransmitterMaterial(float albedo, float index) :
    AbstractMaterial(albedo), index(index) {}

float TransmitterMaterial::brdf(const vec3 &, const vec3 &, const vec3 &) {
    return albedo;
}

vec3 TransmitterMaterial::getReflectedRadiance(const Ray &ray, const AbstractShape &shape) {
    if(ray.recursionDeep >= MAX_BOUNCES)
        return vec3();

    Ray refractedRay = getRefractedRay(ray, shape.getNormal(ray.origin + ray.direction * ray.distMax), index);

    auto nearest = World::world.findNearest(refractedRay);

    if(get<0>(nearest) != nullptr) {
        refractedRay.distMax = get<1>(nearest);
        return brdf(vec3(), vec3(), vec3()) * get<0>(nearest)->getReflectedRadiance(refractedRay);
    }

    return vec3();
}
