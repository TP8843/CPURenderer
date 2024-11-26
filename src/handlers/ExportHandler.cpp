#include "ExportHandler.h"

void ExportHandler::handleFrame(DrawingWindow &window, float deltaTime)
{
    if (exporting)
    {
        window.savePNG("../output/" + std::to_string(frame) + ".png");
        frame += 1;
    }
}

void ExportHandler::startExporting()
{
    exporting = true;
    frame = 0;
}

void ExportHandler::stopExporting()
{
    exporting = false;
    std::cout << "Export complete :D" << std::endl;
}
