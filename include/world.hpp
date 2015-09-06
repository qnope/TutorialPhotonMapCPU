#ifndef WORLD_HPP
#define WORLD_HPP

#include "photonmap.hpp"
#include "shapestorage.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "random.hpp"

class World {
public:    
    template<typename T, typename ...Args>
    std::shared_ptr<T> make_shape(Args &&...args) {
        std::shared_ptr<T> ptr(std::make_shared<T>(std::forward<Args>(args)...));
        mShapeStorage->addShape(ptr);
        return ptr;
    }

    template<typename T, typename ...Args>
    std::shared_ptr<T> addCamera(Args &&...args) {
        std::shared_ptr<T> ptr(std::make_shared<T>(std::forward<Args>(args)...));
        mCamera = ptr;
        return ptr;
    }

    template<typename T, typename ...Args>
    std::shared_ptr<T> make_light(Args &&...args) {
        std::shared_ptr<T> ptr(std::make_shared<T>(std::forward<Args>(args)...));
        lights.emplace_back(ptr);
        return ptr;
    }

    std::tuple<std::shared_ptr<AbstractShape>, float> findNearest(Ray const &ray) {
        return mShapeStorage->findNearest(ray);
    }

    void addPhoton(Photon const &photon) {
        mPhotonMap->addPhoton(photon);
    }

    glm::vec3 gatherIrradiance(glm::vec3 position, glm::vec3 normal, float radius) {
        return mPhotonMap->gatherIrradiance(position, normal, radius);
    }

    std::vector<std::shared_ptr<AbstractLight>> lights;

    void setPhotonmap(std::unique_ptr<AbstractPhotonMap> &&photonMap) {
        mPhotonMap = std::move(photonMap);
    }

    static World world;

private:
    std::shared_ptr<AbstractCamera> mCamera;
    std::unique_ptr<AbstractPhotonMap> mPhotonMap = std::make_unique<SimplePhotonMap>();
    std::unique_ptr<AbstractShapeStorage> mShapeStorage = std::make_unique<SimpleShapeStorage>();
};

glm::vec3 getRadianceFromNearest(Ray const &_ray);

void tracePhoton(Photon const &photon);
void traceShadowPhoton(Photon const &photon);

#endif // WORLD_H
