#ifndef SHAPE_H
#define SHAPE_H

#include "ray.hpp"
#include "material.hpp"
class AbstractShape {
public:
    AbstractShape() = default;
    AbstractShape(std::unique_ptr<AbstractMaterial> &&material);

    virtual float intersect(Ray const &ray) const = 0;
    virtual glm::vec3 getNormal(glm::vec3 const &position) const = 0;

    glm::vec3 getReflectedRadiance(Ray const &ray);

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
