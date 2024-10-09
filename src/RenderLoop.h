//
// Created by Thomas Parr on 09/10/2024.
//

#ifndef RENDERLOOP_H
#define RENDERLOOP_H
#include <DrawingWindow.h>
#include <SDL_events.h>

#include "tests/RenderTest.h"


class RenderLoop {
public:
    RenderLoop();
    void run();
    void addTest(RenderTest *test);

private:
    void handleEvent(SDL_Event &event);
    RenderTest* getCurrentRenderTest();

    DrawingWindow window;
    int currentTest;
    std::vector<RenderTest*> tests;
};



#endif //RENDERLOOP_H
