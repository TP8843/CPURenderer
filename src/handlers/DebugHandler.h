#ifndef DEBUGHANDLER_H
#define DEBUGHANDLER_H
#include <DrawingWindow.h>

#include "EventHandler.h"
#include "FrameHandler.h"
#include "../objects/Model.h"


class DebugHandler final: public FrameHandler, public EventHandler {
public:
    void handleFrame(DrawingWindow& window, float deltaTime) override;
    void handleEvent(DrawingWindow &window, SDL_Event &event) override;

    explicit DebugHandler(Model& model, Transformation& camera);

private:
    void drawFaceNormals(DrawingWindow& window) const;
    void drawVertexNormals(DrawingWindow& window) const;

private:
    Model& model;
    Transformation& camera;
    int currentDebug;
};



#endif //DEBUGHANDLER_H
