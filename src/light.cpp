#include "include/light.hpp"
#include "photonmap.hpp"
#include "world.hpp"

using namespace glm;

float lambertCosineLaw(vec3 const &posToLight, vec3 const &normal) {
    return max(0.0f, dot(posToLight, normal));
}

AbstractLight::AbstractLight(const vec3 &flux) :
    mTotalFlux(flux) {}

PointLight::PointLight(const vec3 &position, const vec3 &flux) :
    AbstractLight(flux), mPosition(position) {}

vec3 PointLight::getIrradiance(const vec3 &position, vec3 const &normal) {
    vec3 posToLight = mPosition - position;
    vec3 posToLightNormalized = normalize(posToLight);
    return lambertCosineLaw(posToLightNormalized, normal) * mTotalFlux /
            (4.f * (float)M_PI * dot(posToLight, posToLight));
}

void PointLight::emitPhotons(std::size_t number) {
    Photon photon;

    photon.flux = mTotalFlux / (float)number;
    photon.position = mPosition;

    for(auto i(0u); i < number; ++i) {
        photon.direction = Random::random.getSphereDirection();
        tracePhoton(photon);
    }
}

SpotLight::SpotLight(const vec3 &position, const vec3 &direction, float cosCutoff, const vec3 &flux) :
    AbstractLight(flux), mPosition(position), mDirection(normalize(direction)), mCosCutoff(cosCutoff) {}

vec3 SpotLight::getIrradiance(const vec3 &position, const vec3 &normal) {
    vec3 posToLight = mPosition - position;
    vec3 posToLightNormalized = normalize(posToLight);

    if(dot(-posToLightNormalized, mDirection) > mCosCutoff) {
        float solidAngle = - 2.f * M_PI * (mCosCutoff - 1);

        return lambertCosineLaw(posToLightNormalized, normal) * mTotalFlux /
            (solidAngle * dot(posToLight, posToLight));
    }

    return vec3();
}

void SpotLight::emitPhotons(std::size_t number) {
    Photon photon;

    photon.flux = mTotalFlux / (float)number;
    photon.position = mPosition;

    for(auto i(0u); i < number; ++i) {
        vec3 directionPhoton;
        do
            directionPhoton = Random::random.getSphereDirection();
        while(dot(directionPhoton, mDirection) < mCosCutoff);


        photon.direction = directionPhoton;
        tracePhoton(photon);
    }
}
