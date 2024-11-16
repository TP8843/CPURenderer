#include "CameraControl.h"

#include <glm/glm.hpp>

#include "../helper/constants.h"

void CameraControl::handleFrame(DrawingWindow& window, const float deltaTime)
{
    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    // Pedestal camera up
    if (keys[constants::keyboard::PEDESTAL_UP])
    {
        camera.translateRelative(glm::vec3(0, constants::speed::TRANSLATION_SPEED * deltaTime, 0));
    }

    // Pedestal camera down
    if (keys[constants::keyboard::PEDESTAL_DOWN])
    {
        camera.translateRelative(glm::vec3(0, -constants::speed::TRANSLATION_SPEED * deltaTime, 0));
    }

    // Dolly camera "forwards" in camera space
    if (keys[constants::keyboard::DOLLY_FORWARD])
    {
        camera.translateRelative(glm::vec3(0, 0, constants::speed::TRANSLATION_SPEED * deltaTime));
    }
    // Dolly camera "backwards"
    if (keys[constants::keyboard::DOLLY_BACK])
    {
        camera.translateRelative(glm::vec3(0, 0, -constants::speed::TRANSLATION_SPEED * deltaTime));
    }

    // Truck camera left
    if (keys[constants::keyboard::TRUCK_LEFT])
    {
        camera.translateRelative(glm::vec3(-constants::speed::TRANSLATION_SPEED * deltaTime, 0, 0));
    }

    // Truck camera right
    if (keys[constants::keyboard::TRUCK_RIGHT])
    {
        camera.translateRelative(glm::vec3(constants::speed::TRANSLATION_SPEED * deltaTime, 0, 0));
    }

    // Pan left
    if (keys[constants::keyboard::PAN_LEFT])
    {
        camera.rotateY(constants::speed::ROTATION_SPEED * deltaTime);
    }

    // Pan right
    if (keys[constants::keyboard::PAN_RIGHT])
    {
        camera.rotateY(-constants::speed::ROTATION_SPEED * deltaTime);
    }

    // Tilt up
    if (keys[constants::keyboard::TILT_UP])
    {
        camera.rotateX(constants::speed::ROTATION_SPEED * deltaTime);
    }

    // Tilt down
    if (keys[constants::keyboard::TILT_DOWN])
    {
        camera.rotateX(-constants::speed::ROTATION_SPEED * deltaTime);
    }

    if (keys[constants::keyboard::LIGHT_PEDESTAL_UP])
    {
        light.position += glm::vec3(0, constants::speed::LIGHT_TRANSLATION_SPEED * deltaTime, 0);
    }

    if (keys[constants::keyboard::LIGHT_PEDESTAL_DOWN])
    {
        light.position -= glm::vec3(0, constants::speed::LIGHT_TRANSLATION_SPEED * deltaTime, 0);
    }

    if (keys[constants::keyboard::LIGHT_TRUCK_LEFT])
    {
        light.position -= glm::vec3(constants::speed::LIGHT_TRANSLATION_SPEED * deltaTime, 0, 0);
    }

    if (keys[constants::keyboard::LIGHT_TRUCK_RIGHT])
    {
        light.position += glm::vec3(constants::speed::LIGHT_TRANSLATION_SPEED * deltaTime, 0, 0);
    }

    if (keys[constants::keyboard::LIGHT_DOLLY_FORWARD])
    {
        light.position += glm::vec3(0, 0, constants::speed::LIGHT_TRANSLATION_SPEED * deltaTime);
    }

    if (keys[constants::keyboard::LIGHT_DOLLY_BACK])
    {
        light.position -= glm::vec3(0, 0, constants::speed::LIGHT_TRANSLATION_SPEED * deltaTime);
    }
}

void CameraControl::handleEvent(DrawingWindow& window, SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == constants::keyboard::RESET_RENDER)
        {
            camera.reset();
            light.reset();
        }

        if (event.key.keysym.sym == constants::keyboard::LOOK_AT)
        {
            camera.lookAt(glm::vec3(0, 0, 0));
        }

        if (event.key.keysym.sym == constants::keyboard::PLACE_LIGHT_AT_CAMERA)
        {
            light.position = camera.position;
        }
    }
}

CameraControl::CameraControl(Transformation& camera, Transformation& light) :
    camera(camera), light(light)
{
}
