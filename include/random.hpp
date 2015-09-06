#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <vector>
#include <glm/gtc/random.hpp>
#include <iostream>

class Random {
public:
    Random(std::size_t n) {
        for(auto i(0u); i < n; ++i)
            mSphereDirection.emplace_back(glm::ballRand(1.0f));
    }

    glm::vec3 getSphereDirection() {
        index = (index + 1) % mSphereDirection.size();
        return mSphereDirection[index];
    }

    glm::vec3 getHemisphereDirection(glm::vec3 const &normal) {
        glm::vec3 dir = getSphereDirection();

        if(glm::dot(dir, normal) < 0.0)
            return -dir;

        return dir;
    }

    static Random random;
private:
    std::vector<glm::vec3> mSphereDirection;
    unsigned index = 0;
};

#endif // RANDOM

