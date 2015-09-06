#ifndef PHOTONMAP_HPP
#define PHOTONMAP_HPP

#include <glm/glm.hpp>
#include <vector>

struct Photon {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 flux;
    unsigned recursionDeep = 0;
};

struct AABB {
    float x1, y1, z1;
    float x2, y2, z2;
};

class AbstractPhotonMap {
public:
    AbstractPhotonMap() = default;

    virtual glm::vec3 gatherIrradiance(glm::vec3 position, glm::vec3 normal, float radius) = 0;
    virtual void addPhoton(Photon const &photon) = 0;
    virtual void clear() = 0;

    virtual ~AbstractPhotonMap() = default;
private:

};

class SimplePhotonMap final : public AbstractPhotonMap {
public:
    SimplePhotonMap() = default;

    glm::vec3 gatherIrradiance(glm::vec3 position, glm::vec3 normal, float radius);
    void addPhoton(Photon const &photon);
    void clear();

    ~SimplePhotonMap() = default;

private:
    std::vector<Photon> mPhotons;
};

class RegularPhotonMap final : public AbstractPhotonMap {
public:
    RegularPhotonMap(std::size_t subdivision, AABB aabb);

    glm::vec3 gatherIrradiance(glm::vec3 position, glm::vec3 normal, float radius);
    void addPhoton(Photon const &photon);
    void clear();

    ~RegularPhotonMap() = default;

private:
    std::vector<std::vector<std::vector<std::vector<Photon>>>> mPhotons;
    AABB mAABB;
    std::size_t mSubdivision;
};

#endif // PHOTONMAP_H
