#ifndef ORBITHANDLER_H
#define ORBITHANDLER_H
#include <chrono>

#include "EventHandler.h"
#include "FrameHandler.h"
#include "../objects/Transformation.h"


class OrbitHandler final : public EventHandler, public FrameHandler {
public:
    // Toggles orbit mode
    bool toggleOrbit();

    // Move camera to next stage of orbit. Only works in orbit mode
    void iterateOrbit();

    void handleFrame(DrawingWindow& window, float deltaTime) override;
    void handleEvent(DrawingWindow& window, SDL_Event& event) override;

    // Return state of orbit
    bool getOrbit() const;

    OrbitHandler(Transformation& transformation, const glm::vec3& centre, bool orbit = false);

private:
    // Update previous time and return time difference in seconds.
    float updateDTime();

    // Object to complete orbit on
    Transformation& transformation;

    bool isOrbiting = false;

    glm::vec3 centre;
    std::chrono::milliseconds prevTime{};
};

#endif //ORBITHANDLER_H
