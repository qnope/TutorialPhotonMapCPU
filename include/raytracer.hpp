#ifndef RAYTRACER_HPP
#define RAYTRACER_HPP

#include "light.hpp"
#include "world.hpp"

#define NUM_THREADS 8 

class RayTracer {
public:
    RayTracer(Device *device);

    template<typename T, typename ...Args>
    std::shared_ptr<T> make_shape(Args &&...args) {
        return World::world.make_shape<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename ...Args>
    std::shared_ptr<T> addCamera(Args &&...args) {
        std::shared_ptr<T> ptr(std::make_shared<T>(std::forward<Args>(args)...));
        mCamera = ptr;
        return ptr;
    }

    template<typename T, typename ...Args>
    std::shared_ptr<T> make_light(Args &&...args) {
        return World::world.make_light<T>(std::forward<Args>(args)...);
    }

    void render();

private:
    Device *mDevice;
    std::shared_ptr<AbstractCamera> mCamera;
    void renderThread(int yStart, int yEnd);
};

#endif // RAYTRACER_H
