#ifndef CAMERA_H
#define CAMERA_H

#include "system/device.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class AbstractCamera {
public:
    AbstractCamera(glm::vec3 const &position, glm::vec3 const &center);

    virtual void update(Device &device) = 0;

    glm::mat4 getMatrix() const;

    virtual ~AbstractCamera() = default;

public:
    glm::vec3 position;
    glm::vec3 center;
};

class StaticCamera final : public AbstractCamera {
public:
    StaticCamera(glm::vec3 const &position, glm::vec3 const &center);

    void update(Device &) {}
};

#endif // CAMERA_H
