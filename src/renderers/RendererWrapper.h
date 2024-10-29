#ifndef RENDERERWRAPPER_H
#define RENDERERWRAPPER_H
#include <DrawingWindow.h>

class RendererWrapper
{
public:
    virtual ~RendererWrapper() {}

    virtual void renderFrame(DrawingWindow &window) = 0;
    virtual Camera& getCamera() = 0;
    virtual Model& getModel() = 0;
};

#endif //RENDERERWRAPPER_H
