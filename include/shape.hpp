#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "ray.hpp"
#include "material.hpp"

/**
 * @brief      This class provides an interface to manage differents shape
 */
class AbstractShape {
public:
    AbstractShape() = default;
    AbstractShape(std::unique_ptr<AbstractMaterial> &&material);

    /**
     * @brief      Return the distance between shape and ray emetter   
     *
     * @param      ray     
     *
     * @return     Negative value if not found. Distance between object and ray emetter
     */
    virtual float intersect(Ray const &ray) const = 0;
    virtual glm::vec3 getNormal(glm::vec3 const &position) const = 0;

    /**
     * @brief      Return the radiance returned by the material owned 
     *
     * @param      ray  
     *
     * @return     radiance
     */
    glm::vec3 getReflectedRadiance(Ray const &ray);

    void bouncePhoton(Photon const &photon);

    virtual ~AbstractShape() = default;

private:
    std::unique_ptr<AbstractMaterial> mMaterial = std::make_unique<UniformLambertianMaterial>(glm::vec3(1.0, 1.0, 1.0), 1.0);
};

class Sphere final : public AbstractShape {
public:
    Sphere(glm::vec3 const &position, float radius);
    Sphere(glm::vec3 const &position, float radius, std::unique_ptr<AbstractMaterial> &&material);

    float intersect(Ray const &ray) const override;
    glm::vec3 getNormal(glm::vec3 const &position) const override;

private:
    glm::vec3 mPosition;
    float mRadius;
};

class Plane final : public AbstractShape {
public:
    Plane(glm::vec4 const &equation);
    Plane(glm::vec4 const &equation, std::unique_ptr<AbstractMaterial> &&material);

    float intersect(Ray const &ray) const override;
    glm::vec3 getNormal(glm::vec3 const &position) const override;

private:
    glm::vec4 mEquation;
};

#endif // SHAPE_H
