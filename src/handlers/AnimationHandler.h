#ifndef ANIMATIONHANDLER_H
#define ANIMATIONHANDLER_H
#include <vector>

#include "FrameHandler.h"
#include "../objects/Transformation.h"


class AnimationHandler final : public FrameHandler {
public:
    void handleFrame(DrawingWindow& window, float deltaTime) override;

    Transformation& transformation;

    // Keyframes of <frame, transformation>
    std::vector<std::pair<int, Transformation>> animation;

    explicit AnimationHandler(Transformation& transformation) :
        transformation(transformation)
    {};

private:
    int currentFrame = 0;
    int currentKeyframe = 0;
    int totalCurrentFrame = 0;
};


#endif //ANIMATIONHANDLER_H
