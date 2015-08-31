#ifndef WORLD_HPP
#define WORLD_HPP

#include "shapestorage.hpp"
#include "light.hpp"

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

    std::vector<std::shared_ptr<AbstractLight>> lights;

    static World world;

private:
    std::shared_ptr<AbstractCamera> mCamera;
    std::unique_ptr<AbstractShapeStorage> mShapeStorage = std::make_unique<SimpleShapeStorage>();
};

#endif // WORLD_H
