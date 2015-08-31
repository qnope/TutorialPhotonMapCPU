#include "include/ray.hpp"

using namespace glm;

Ray::Ray(const glm::vec3 &_origin, const glm::vec3 &_direction) :
    origin(_origin), direction(normalize(_direction)) {}

Ray::Ray(AbstractCamera const &camera, unsigned x, unsigned y, unsigned width, unsigned height) :
    origin(camera.position),
    direction(normalize(vec3(inverse(infinitePerspective(radians(45.0f), (float)width / height, 0.1f) * camera.getMatrix()) *
                        vec4(vec2(x, y) / vec2(width, height) * 2.f - 1.f, 1.f, 1.f)))) {}
