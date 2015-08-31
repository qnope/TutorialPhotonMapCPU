#include "include/camera.hpp"

using namespace glm;

AbstractCamera::AbstractCamera(const vec3 &_position, const vec3 &_center) :
    position(_position), center(_center) {}

mat4 AbstractCamera::getMatrix() const {
    return glm::lookAt(position, center, glm::vec3(0.0f, 1.0f, 0.0f));
}

StaticCamera::StaticCamera(const vec3 &position, const vec3 &center) :
    AbstractCamera(position, center) {}
