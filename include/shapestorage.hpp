#ifndef SHAPESTORAGE_H
#define SHAPESTORAGE_H

#include "shape.hpp"
#include <memory>
#include <tuple>

class AbstractShapeStorage {
public:    
    virtual void addShape(std::shared_ptr<AbstractShape> const &shape) = 0;
    virtual std::tuple<std::shared_ptr<AbstractShape>, float> findNearest(Ray const &ray) = 0;

    ~AbstractShapeStorage() = default;
};

class SimpleShapeStorage final : public AbstractShapeStorage{
public:
    void addShape(const std::shared_ptr<AbstractShape> &shape);
    std::tuple<std::shared_ptr<AbstractShape>, float> findNearest(Ray const &ray);

private:
    std::vector<std::shared_ptr<AbstractShape>> mShapes;
};

#endif // SHAPESTORAGE_H
