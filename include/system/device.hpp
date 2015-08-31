#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <SDL/SDL.h>
#include <vector>
#include <memory>
#include <array>
#include <stdexcept>
#include <utility>
#include <glm/glm.hpp>

class Mouse {
public:
    Mouse();

    void update(SDL_Event &event);

private:
    std::array<bool, 8> mButtons;
};

class Keyboard {
public:
    Keyboard();

    void update(SDL_Event &event);

private:
    std::array<bool, SDLK_LAST> mKeys;
};

class Device {
public:
    Device(int width, int height);

    void begin() {
        mLastTick = SDL_GetTicks();
        SDL_FillRect(mScreen, NULL, 0);
    }

    void end() {
        SDL_Flip(mScreen);
        mRenderingTime = SDL_GetTicks() - mLastTick;
    }

    void update();

    void writePixel(unsigned x, unsigned y, unsigned colour);
    void writePixel(unsigned x, unsigned y, glm::vec3 const &colour);

    bool run() {
        return !mQuit;
    }

    int width() const {
        return mScreen->w;
    }

    int height() const {
        return mScreen->h;
    }

    unsigned renderingTime() const {
        return mRenderingTime;
    }

private:
     SDL_Surface *mScreen;
     std::unique_ptr<Keyboard> mKb;
     std::unique_ptr<Mouse> mMouse;
     std::vector<unsigned*> mPixels;
     bool mQuit = false;

     unsigned mLastTick;
     unsigned mRenderingTime;
};

#endif // DEVICE_H
