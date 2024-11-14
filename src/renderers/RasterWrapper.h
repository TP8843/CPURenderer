//
// Created by Thomas Parr on 29/10/2024.
//

#ifndef RASTERWRAPPER_H
#define RASTERWRAPPER_H
#include "RasterRenderer.h"
#include "RendererWrapper.h"


class RasterWrapper final : public RendererWrapper {
public:
    Camera& getCamera() override;
    Model& getModel() override;
    Light& getLight() override;

    void renderFrame(DrawingWindow& window) override;

    explicit RasterWrapper(RasterRenderer& renderer);

private:
    RasterRenderer& renderer;
};



#endif //RASTERWRAPPER_H
