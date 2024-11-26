#include "AnimationHandler.h"

#include "../helper/constants.h"
#include "../helper/Interpolation.h"

void AnimationHandler::handleFrame(DrawingWindow& window, float deltaTime)
{
    if (animating)
    {
        const auto current = animation.at(currentKeyframe);
        const auto next = animation.at((currentKeyframe + 1) % animation.size());

        const auto proportion = static_cast<float>(currentFrame) / static_cast<float>(next.first);

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

            if (currentKeyframe == 0)
            {
                animating = false;
                exportHandler.stopExporting();
            }
        }
    }
}

void AnimationHandler::handleEvent(DrawingWindow &window, SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == constants::keyboard::RUN_ANIMATION)
        {
            animating = true;
            currentFrame = 0;
            currentKeyframe = 0;
            exportHandler.startExporting();
        }
    }
}
