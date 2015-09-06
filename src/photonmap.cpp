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
    for(auto &photon : mPhotons)
        if(dot(photon.position - position, photon.position - position) < radiusSquare)
            if(dot(photon.direction, normal) < 0.0)
                irradiance += photon.flux;

    return irradiance / ((float)M_PI * radiusSquare);
}

RegularPhotonMap::RegularPhotonMap(std::size_t subdivision, AABB aabb) :
    mAABB(aabb), mSubdivision(subdivision) {

    mPhotons.resize(subdivision);

    for(auto z(0u); z < subdivision; ++z) {
        mPhotons[z].resize(subdivision);
        for(auto y(0u); y < subdivision; ++y)
            mPhotons[z][y].resize(subdivision);
    }
}

void RegularPhotonMap::addPhoton(const Photon &photon) {
    if(!(photon.position.x > mAABB.x1 && photon.position.x < mAABB.x2))
        return;

    if(!(photon.position.y > mAABB.y1 && photon.position.y < mAABB.y2))
        return;

    if(!(photon.position.z > mAABB.z1 && photon.position.z < mAABB.z2))
        return;

    int i, j, k;

    i = (int)(((photon.position.x - mAABB.x1) / (mAABB.x2 - mAABB.x1)) * mSubdivision);
    j = (int)(((photon.position.y - mAABB.y1) / (mAABB.y2 - mAABB.y1)) * mSubdivision);
    k = (int)(((photon.position.z - mAABB.z1) / (mAABB.z2 - mAABB.z1)) * mSubdivision);

    mPhotons[k][j][i].emplace_back(photon);
}

void RegularPhotonMap::clear() {
    for(auto &z : mPhotons)
        for(auto &y : z)
            for(auto &x : y)
                x.clear();
}

vec3 RegularPhotonMap::gatherIrradiance(vec3 position, vec3 normal, float radius) {
    float radiusSquare = radius * radius;
    int i, j, k;
    int offsetI, offsetJ, offsetK;

    i = (int)(((position.x - mAABB.x1) / (mAABB.x2 - mAABB.x1)) * mSubdivision);
    j = (int)(((position.y - mAABB.y1) / (mAABB.y2 - mAABB.y1)) * mSubdivision);
    k = (int)(((position.z - mAABB.z1) / (mAABB.z2 - mAABB.z1)) * mSubdivision);

    offsetI = radius / ((mAABB.x2 - mAABB.x1) / mSubdivision) + 1;
    offsetJ = radius / ((mAABB.x2 - mAABB.x1) / mSubdivision) + 1;
    offsetK = radius / ((mAABB.x2 - mAABB.x1) / mSubdivision) + 1;

    vec3 irradiance;

    for(auto z(k - offsetK); z < k + offsetK + 1; ++z)
        if(z >= 0 && z < mSubdivision)
            for(auto y(j - offsetJ); y < j + offsetJ + 1; ++y)
                if(y >= 0 && y < mSubdivision)
                    for(auto x(i - offsetI); x < i + offsetI + 1; ++x)
                        if(x >= 0 && x < mSubdivision)
                            for(auto &photon : mPhotons[z][y][x])
                                if(dot(photon.position - position, photon.position - position) < radiusSquare)
                                    if(dot(photon.direction, normal) < 0.0)
                                        irradiance += photon.flux;

    return irradiance / ((float)M_PI * radiusSquare);
}
