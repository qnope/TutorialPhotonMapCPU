#ifndef RAY_HPP
#define RAY_HPP

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include "camera.hpp"

#define RAY_EPSILON 0.001f

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

inline Ray getReflectedRay(Ray const &ray, glm::vec3 const &normal) {
    glm::vec3 position = ray.origin + ray.direction * ray.distMax;
    Ray reflectedRay(position, reflect(ray.direction, normal));
    reflectedRay.origin += RAY_EPSILON * reflectedRay.direction;
    reflectedRay.recursionDeep = ray.recursionDeep + 1;
    return reflectedRay;
}

inline Ray getRefractedRay(Ray const &ray, glm::vec3 const &normal, float index) {
    glm::vec3 position = ray.origin + ray.direction * ray.distMax;
    Ray refractedRay;

    if(glm::dot(ray.direction, normal) < 0.f)
        refractedRay = Ray(position, glm::refract(ray.direction, normal, 1.0f / index));

    else
        refractedRay = Ray(position, glm::refract(ray.direction, -normal, index));

    refractedRay.origin += RAY_EPSILON * refractedRay.direction;
    refractedRay.recursionDeep = ray.recursionDeep + 1;
    return refractedRay;
}

#endif // RAY_H
