#include "include/raytracer.hpp"
#include <thread>
#include "system/threadpool.hpp"

using namespace std;
using namespace glm;

#include <iostream>

RayTracer::RayTracer(Device *device, std::size_t numThreads) :
    mDevice(device), mThreadPool(numThreads) {}

void RayTracer::renderThread(int y) {
    for(auto x(0); x < mDevice->width(); ++x) {
        Ray rayFromCamera(*mCamera, x, mDevice->height() - y, mDevice->width(), mDevice->height());

        mDevice->writePixel(x, y, getRadianceFromNearest(rayFromCamera));
    }

    mPixelsWrite++;
}

void RayTracer::render() {
    mPixelsWrite = 0;

    for(auto y(0); y < mDevice->height(); ++y)
        mThreadPool.addTask(std::bind(&RayTracer::renderThread, this, y));

    while(mPixelsWrite != mDevice->height());
}
