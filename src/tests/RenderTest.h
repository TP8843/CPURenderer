//
// Created by Thomas Parr on 09/10/2024.
//

#ifndef RENDERTESTS_H
#define RENDERTESTS_H
#include <DrawingWindow.h>
#include <SDL_events.h>

// Class for allowing multiple render tests to be loaded and iterated between
class RenderTest
{
public:
    virtual ~RenderTest() {}

    virtual void renderFrame(DrawingWindow &window) = 0;
    virtual void handleEvent(SDL_Event &event, DrawingWindow &window) = 0;

};

#endif //RENDERTESTS_H
