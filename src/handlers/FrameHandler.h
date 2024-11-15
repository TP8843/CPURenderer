#ifndef FRAMEHANDLER_H
#define FRAMEHANDLER_H
#include <DrawingWindow.h>

class FrameHandler {
public:
    virtual ~FrameHandler() {};

    virtual void handleFrame(DrawingWindow& window, float deltaTime) = 0;
};

#endif //FRAMEHANDLER_H
