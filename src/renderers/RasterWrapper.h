//
// Created by Thomas Parr on 29/10/2024.
//

#ifndef RASTERWRAPPER_H
#define RASTERWRAPPER_H
#include "RasterRenderer2.h"
#include "RendererWrapper.h"


class RasterWrapper final : public RendererWrapper {
public:
    Camera& getCamera() override;
    Model& getModel() override;

    void renderFrame(DrawingWindow& window) override;

    explicit RasterWrapper(RasterRenderer2& renderer);

private:
    RasterRenderer2& renderer;
};



#endif //RASTERWRAPPER_H
