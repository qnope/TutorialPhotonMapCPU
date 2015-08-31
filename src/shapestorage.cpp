#include "include/shapestorage.hpp"

using namespace std;

void SimpleShapeStorage::addShape(std::shared_ptr<AbstractShape> const &shape) {
    mShapes.emplace_back(shape);
}

tuple<shared_ptr<AbstractShape>, float> SimpleShapeStorage::findNearest(const Ray &_ray) {
    tuple<shared_ptr<AbstractShape>, float> nearest(nullptr, 0.f);
    Ray ray = _ray;

    for(auto &shape : mShapes) {
        float distTmp;
        if((distTmp = shape->intersect(ray)) > 0.f) {
            ray.distMax = distTmp;
            get<0>(nearest) = shape;
            get<1>(nearest) = distTmp;
        }
    }

    return nearest;
}

