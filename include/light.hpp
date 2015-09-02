#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <tuple>

/**
 * @brief      Interface for a light
 */
class AbstractLight {
public:
    AbstractLight(glm::vec3 const &flux);

    /**
     * @brief      Compute irradiance receive by the projected area in position
     *
     * @param      position  surface's position
     * @param      normal    surface's normal
     *
     * @return     irradiance
     */
    virtual glm::vec3 getIrradiance(glm::vec3 const &position, glm::vec3 const &normal) = 0;

    virtual ~AbstractLight() = default;

protected:
    glm::vec3 mTotalFlux;
};

class PointLight final : public AbstractLight {
public:
    PointLight(glm::vec3 const &position, glm::vec3 const &flux);

    glm::vec3 getIrradiance(glm::vec3 const &position, glm::vec3 const &normal);

private:
    glm::vec3 mPosition;
};

class SpotLight final : public AbstractLight {
public:
    SpotLight(glm::vec3 const &position, glm::vec3 const &direction, float cosCutoff, glm::vec3 const &flux);

    glm::vec3 getIrradiance(const glm::vec3 &position, const glm::vec3 &normal);

private:
    glm::vec3 mPosition;
    glm::vec3 mDirection;
    float mCosCutoff;
};


#endif // LIGHT_H
