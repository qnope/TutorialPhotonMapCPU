#include "include/world.hpp"

World World::world;
Random Random::random(1'000'000);

using namespace std;
using namespace glm;

vec3 getRadianceFromNearest(Ray const &_ray) {
   Ray ray = _ray;
   auto nearest = World::world.findNearest(ray);

   if(get<0>(nearest) != nullptr) {
       ray.distMax = get<1>(nearest);
       ray.origin += ray.direction * ray.distMax;
       return get<0>(nearest)->getReflectedRadiance(ray);
   }

   return vec3();
}

void tracePhoton(Photon const &_photon) {
    Photon photon = _photon;
    Ray ray(photon.position + photon.direction * RAY_EPSILON, photon.direction);

    auto nearest = World::world.findNearest(ray);

    if(get<0>(nearest) != nullptr) {
        photon.position += photon.direction * get<1>(nearest);
        get<0>(nearest)->bouncePhoton(photon);
    }
}

void traceShadowPhoton(const Photon &_photon) {
    Photon photon = _photon;
    Ray ray(photon.position + photon.direction * RAY_EPSILON, photon.direction);

    photon.flux = -photon.flux;

    auto nearest = World::world.findNearest(ray);

    while(get<0>(nearest) != nullptr) {
        ray.origin += ray.direction * get<1>(nearest);
        photon.position = ray.origin;

        if(dot(ray.direction, get<0>(nearest)->getNormal(ray.origin)) < 0.f)
            World::world.addPhoton(photon);

        ray.origin += RAY_EPSILON * ray.direction;

        nearest = World::world.findNearest(ray);
    }
}
