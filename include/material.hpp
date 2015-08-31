#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"

class AbstractShape;

class AbstractMaterial {
public:
    AbstractMaterial(float albedo);
    virtual glm::vec3 getReflectedRadiance(Ray const &ray, AbstractShape const &shape) = 0;

    virtual ~AbstractMaterial() = default;

    float albedo;
protected:
    virtual float brdf(glm::vec3 const &ingoing, glm::vec3 const &outgoing, glm::vec3 const &normal) = 0;
};

class LambertianMaterial : public AbstractMaterial {
public :
    LambertianMaterial(float albedo);

    virtual glm::vec3 getReflectedRadiance(Ray const &ray, AbstractShape const &shape) = 0;

    virtual ~LambertianMaterial() = default;

protected:
    virtual float brdf(const glm::vec3 &ingoing, const glm::vec3 &, glm::vec3 const &);

};

class UniformLambertianMaterial final : public LambertianMaterial {
public :
    UniformLambertianMaterial(glm::vec3 const &colour, float albedo = 0.8);

    glm::vec3 getReflectedRadiance(Ray const &ray, AbstractShape const &shape);

public:
    glm::vec3 color;
};

class MirrorMaterial final : public AbstractMaterial {
public:
    MirrorMaterial(float albedo);

    glm::vec3 getReflectedRadiance(const Ray &ray, const AbstractShape &shape);

protected:
    float brdf(const glm::vec3 &, const glm::vec3 &, const glm::vec3 &);
};

class TransmitterMaterial final : public AbstractMaterial {
public:
    TransmitterMaterial(float albedo, float index);

    glm::vec3 getReflectedRadiance(const Ray &ray, const AbstractShape &shape);

public:
    float index;

protected:
    float brdf(const glm::vec3 &, const glm::vec3 &, const glm::vec3 &);
};

#endif // MATERIAL_H

