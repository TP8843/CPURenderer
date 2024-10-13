//
// Created by Thomas Parr on 13/10/2024.
//

#ifndef OPTIONS_H
#define OPTIONS_H
#include <SDL_keycode.h>

#endif //OPTIONS_H

namespace constants
{
    namespace window
    {
        constexpr int WIDTH = 640;
        constexpr int HEIGHT = 480;
    }

    namespace speed
    {
        constexpr float ROTATION_SPEED = 0.03f;
        constexpr float TRANSLATION_SPEED = 0.04f;
    }

    namespace keyboard
    {
        // Translation
        constexpr int PEDESTAL_UP = SDLK_r;
        constexpr int PEDESTAL_DOWN = SDLK_f;
        constexpr int DOLLY_FORWARD = SDLK_s;
        constexpr int DOLLY_BACK = SDLK_w;
        constexpr int TRUCK_LEFT = SDLK_a;
        constexpr int TRUCK_RIGHT = SDLK_d;

        // Rotation
        constexpr int PAN_LEFT = SDLK_LEFT;
        constexpr int PAN_RIGHT = SDLK_RIGHT;
        constexpr int TILT_UP = SDLK_UP;
        constexpr int TILT_DOWN = SDLK_DOWN;

        // Test Switching
        constexpr int MAIN_TEST = SDLK_m;
        constexpr int SUB_TEST = SDLK_n;
        constexpr int RAND_TRIANGLE = SDLK_u;
    };
}