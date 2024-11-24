#include "AnimationHandler.h"

#include "../helper/Interpolation.h"

void AnimationHandler::handleFrame(DrawingWindow& window, float deltaTime)
{
    const auto current = animation.at(currentKeyframe);
    const auto next = animation.at((currentKeyframe + 1) % animation.size());

    auto proportion = static_cast<float>(currentFrame) / static_cast<float>(next.first);

    transformation = Interpolation::interpolate(
        current.second,
        next.second,
        proportion);

    currentFrame += 1;
    totalCurrentFrame += 1;

    if (currentFrame == next.first)
    {
        currentFrame = 0;
        currentKeyframe = (currentKeyframe + 1) % animation.size();
        transformation = next.second;

        if (currentKeyframe == 0) totalCurrentFrame = 0;
    }
}
