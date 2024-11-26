#ifndef EXPORTHANDLER_H
#define EXPORTHANDLER_H
#include "FrameHandler.h"


class ExportHandler final : public FrameHandler {
public:
    void handleFrame(DrawingWindow &window, float deltaTime) override;

    void startExporting();
    void stopExporting();

    ExportHandler() = default;

private:
    bool exporting = false;
    int frame = 0;
};



#endif //EXPORTHANDLER_H
