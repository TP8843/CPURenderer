#include "OrbitHandler.h"

#include "../helper/constants.h"

bool OrbitHandler::toggleOrbit()
{
    isOrbiting = !isOrbiting;

    updateDTime();

    return isOrbiting;
}

void OrbitHandler::iterateOrbit()
{
    const float timeElapsed = updateDTime();

    const auto rotationMatrix = glm::mat3(
        glm::vec3(glm::cos(constants::speed::ORBIT_SPEED * timeElapsed), 0, -glm::sin(constants::speed::ORBIT_SPEED * timeElapsed)),
        glm::vec3(0, 1, 0),
        glm::vec3(glm::sin(constants::speed::ORBIT_SPEED * timeElapsed), 0, glm::cos(constants::speed::ORBIT_SPEED * timeElapsed)));

    transformation.position = rotationMatrix * (transformation.position - centre) + centre;
    transformation.lookAt(glm::vec3(0,0,0));
}

void OrbitHandler::handleFrame(DrawingWindow& window, float deltaTime)
{
    if (isOrbiting) iterateOrbit();
}

void OrbitHandler::handleEvent(DrawingWindow& window, SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == constants::keyboard::ORBIT)
        {
            toggleOrbit();
        }
    }
}

OrbitHandler::OrbitHandler(Transformation& transformation, const glm::vec3& centre, const bool orbit) :
    transformation(transformation), isOrbiting(orbit), centre(centre)
{
}

float OrbitHandler::updateDTime()
{
    const std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch());

    const float timeElapsed = static_cast<float>((time - prevTime).count()) / 1000.0f;

    prevTime = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch());

    return timeElapsed;
}

