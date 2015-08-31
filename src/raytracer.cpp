#include "include/raytracer.hpp"
#include <thread>

using namespace std;
using namespace glm;

#include <iostream>

RayTracer::RayTracer(Device *device) :
    mDevice(device) {}

void RayTracer::renderThread(int yStart, int yEnd) {
    for(auto y = yStart; y < yEnd; ++y) {
        for(auto x = 0; x < mDevice->width(); ++x) {
            Ray rayFromCamera(*mCamera, x, mDevice->height() - y, mDevice->width(), mDevice->height());

            auto nearest = World::world.findNearest(rayFromCamera);

            if(get<0>(nearest) != nullptr) {
                rayFromCamera.distMax = get<1>(nearest);
                mDevice->writePixel(x, y, get<0>(nearest)->getReflectedRadiance(rayFromCamera));
            }
        }
    }
}

void RayTracer::render() {
    thread threads[NUM_THREADS - 1];

    for(auto i(0); i < NUM_THREADS - 1; ++i)
        threads[i] = thread(&RayTracer::renderThread, this, i * mDevice->height() / NUM_THREADS, (i + 1) * mDevice->height() / NUM_THREADS);

    renderThread((NUM_THREADS - 1) * mDevice->height() / NUM_THREADS, NUM_THREADS * mDevice->height() / NUM_THREADS);

    for(auto &t : threads)
        t.join();
}
