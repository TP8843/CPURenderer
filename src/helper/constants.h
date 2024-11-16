//
// Created by Thomas Parr on 13/10/2024.
//

#ifndef OPTIONS_H
#define OPTIONS_H
#include <SDL_keycode.h>

namespace constants
{
    namespace window
    {
        constexpr int WIDTH = 640;
        constexpr int HEIGHT = 480;
    }

    namespace speed
    {
        constexpr float ROTATION_SPEED = 1.2f;
        constexpr float ORBIT_SPEED = 1.2f;
        constexpr float TRANSLATION_SPEED = 6.0f;
        constexpr float LIGHT_TRANSLATION_SPEED = 3.0f;
    }

    namespace keyboard
    {
        // Translation
        constexpr int PEDESTAL_UP = SDL_SCANCODE_R;
        constexpr int PEDESTAL_DOWN = SDL_SCANCODE_F;
        constexpr int DOLLY_FORWARD = SDL_SCANCODE_S;
        constexpr int DOLLY_BACK = SDL_SCANCODE_W;
        constexpr int TRUCK_LEFT = SDL_SCANCODE_A;
        constexpr int TRUCK_RIGHT = SDL_SCANCODE_D;

        // Rotation
        constexpr int PAN_LEFT = SDL_SCANCODE_LEFT;
        constexpr int PAN_RIGHT = SDL_SCANCODE_RIGHT;
        constexpr int TILT_UP = SDL_SCANCODE_UP;
        constexpr int TILT_DOWN = SDL_SCANCODE_DOWN;

        // Light Translation
        constexpr int LIGHT_PEDESTAL_UP = SDL_SCANCODE_Y;
        constexpr int LIGHT_PEDESTAL_DOWN = SDL_SCANCODE_H;
        constexpr int LIGHT_DOLLY_FORWARD = SDL_SCANCODE_K;
        constexpr int LIGHT_DOLLY_BACK = SDL_SCANCODE_I;
        constexpr int LIGHT_TRUCK_LEFT = SDL_SCANCODE_J;
        constexpr int LIGHT_TRUCK_RIGHT = SDL_SCANCODE_L;

        constexpr int PLACE_LIGHT_AT_CAMERA = SDLK_u;

        constexpr int ORBIT = SDLK_o;
        constexpr int RESET_RENDER = SDLK_z;
        constexpr int LOOK_AT = SDLK_p;

        // Test Switching
        constexpr int MAIN_TEST = SDLK_m;
        constexpr int SUB_TEST = SDLK_n;
        constexpr int RAND_TRIANGLE = SDLK_u;
        constexpr int DEBUG_SWITCH = SDLK_b;
    }
}

#endif //OPTIONS_H