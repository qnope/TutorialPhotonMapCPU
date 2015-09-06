#include "system/device.hpp"

using namespace std;

Keyboard::Keyboard() {
    std::fill(mKeys.begin(), mKeys.end(), false);
}

Mouse::Mouse() {
    std::fill(mButtons.begin(), mButtons.end(), false);
}

Device::Device(int width, int height) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        throw runtime_error("Unable to initialize SDL : "s + SDL_GetError());

    atexit(SDL_Quit);

    if((mScreen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE)) == NULL)
        throw runtime_error("Unable to create VideoSurface : "s + SDL_GetError());

    for(auto y = 0; y < height; ++y)
        mPixels.emplace_back((unsigned*)mScreen->pixels + y * width);

    mKb = make_unique<Keyboard>();
    mMouse = make_unique<Mouse>();
}

void Device::update() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT: mQuit = true; break;

        default: break;
        }
    }
}

void Device::writePixel(unsigned x, unsigned y, unsigned color) {
    mPixels[y][x] = color;
}

void Device::writePixel(unsigned x, unsigned y, glm::vec3 const &color) {
    unsigned r = max(0.0f, min(1.0f, color.r)) * 255;
    unsigned g = max(0.0f, min(1.0f, color.g)) * 255;
    unsigned b = max(0.0f, min(1.0f, color.b)) * 255;

    mPixels[y][x] = (r << 16) + (g << 8) + b;
}
