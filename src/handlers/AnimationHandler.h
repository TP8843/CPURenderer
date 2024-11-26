#ifndef ANIMATIONHANDLER_H
#define ANIMATIONHANDLER_H
#include <vector>

#include "EventHandler.h"
#include "ExportHandler.h"
#include "FrameHandler.h"
#include "../objects/Transformation.h"


class AnimationHandler final : public FrameHandler, public EventHandler {
public:
    void handleFrame(DrawingWindow& window, float deltaTime) override;

    Transformation& transformation;

    // Keyframes of <frame, transformation>
    std::vector<std::pair<int, Transformation>> animation;

    explicit AnimationHandler(Transformation& transformation, ExportHandler& exportHandler) :
        transformation(transformation), exportHandler(exportHandler)
    {}

    void handleEvent(DrawingWindow &window, SDL_Event &event) override;

private:
    int currentFrame = 0;
    int currentKeyframe = 0;
    int totalCurrentFrame = 0;

    bool animating = false;

    ExportHandler& exportHandler;
};


#endif //ANIMATIONHANDLER_H
