#ifndef RAY_HPP
#define RAY_HPP

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include "camera.hpp"

#define RAY_EPSILON 0.01f

#define MAX_BOUNCES 3

class Ray {
public:
    Ray() = default;
    Ray(glm::vec3 const &origin, glm::vec3 const &direction);

    /**
     * @brief Create Ray from camera
     * @param camera
     * @param x
     * @param y
     * @param width
     * @param height
     */
     Ray(AbstractCamera const &camera,
        unsigned x, unsigned y, unsigned width, unsigned height);

public:
    glm::vec3 origin;
    glm::vec3 direction;
    float distMax = FLT_MAX;
    unsigned recursionDeep = 0;
};

inline glm::vec3 getReflectedDir(glm::vec3 const &incoming, glm::vec3 const &normal) {
    return glm::reflect(incoming, normal);
}

inline glm::vec3 getRefractedDir(glm::vec3 const &incoming, glm::vec3 const &normal, float index) {
    if(glm::dot(incoming, normal) < 0.f)
        return glm::refract(incoming, normal, 1.f / index);

    else
        return glm::refract(incoming, -normal, index);
}

inline Ray getReflectedRay(Ray const &ray, glm::vec3 const &normal) {
    Ray reflectedRay(ray.origin, getReflectedDir(ray.direction, normal));
    reflectedRay.recursionDeep = ray.recursionDeep + 1;
    reflectedRay.origin += RAY_EPSILON * reflectedRay.direction;
    return reflectedRay;
}

inline Ray getRefractedRay(Ray const &ray, glm::vec3 const &normal, float index) {
    Ray refractedRay(ray.origin, getRefractedDir(ray.direction, normal, index));
    refractedRay.recursionDeep = ray.recursionDeep + 1;
    refractedRay.origin += RAY_EPSILON * refractedRay.direction;
    return refractedRay;
}

#endif // RAY_H
