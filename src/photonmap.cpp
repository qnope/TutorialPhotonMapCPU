#include "include/photonmap.hpp"

using namespace glm;

#include <iostream>

void SimplePhotonMap::addPhoton(const Photon &photon) {
    mPhotons.emplace_back(photon);
}

void SimplePhotonMap::clear() {
    mPhotons.clear();
}

vec3 SimplePhotonMap::gatherIrradiance(glm::vec3 position, glm::vec3 normal, float radius) {
    float radiusSquare = radius * radius;
    vec3 irradiance;
    for(auto &photon : mPhotons) {
        if(dot(photon.position - position, photon.position - position) < radiusSquare)
            if(dot(photon.direction, normal) < 0.0)
                irradiance += photon.flux;
    }

    return irradiance / ((float)M_PI * radiusSquare);
}
