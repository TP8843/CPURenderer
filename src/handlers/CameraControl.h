#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H
#include "EventHandler.h"
#include "FrameHandler.h"
#include "../objects/Transformation.h"


class CameraControl final : public FrameHandler, public EventHandler {
public:
    void handleFrame(DrawingWindow& window, float deltaTime) override;
    void handleEvent(DrawingWindow& window, SDL_Event& event) override;
    CameraControl(Transformation& camera, Transformation& light);

private:
    Transformation& camera;
    Transformation& light;
};



#endif //CAMERACONTROL_H
