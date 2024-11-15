//
// Created by Thomas Parr on 29/10/2024.
//

#ifndef RASTERWRAPPER_H
#define RASTERWRAPPER_H
#include "../RasterRenderer.h"
#include "RendererWrapper.h"


class RasterWrapper final : public RendererWrapper {
public:
    Transformation& getCamera() override;
    Model& getModel() override;
    Transformation& getLight() override;

    void renderFrame(DrawingWindow& window) override;

    explicit RasterWrapper(RasterRenderer& renderer);

private:
    RasterRenderer& renderer;
};



#endif //RASTERWRAPPER_H
