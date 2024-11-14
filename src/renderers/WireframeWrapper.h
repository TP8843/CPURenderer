#ifndef WIREFRAMEWRAPPER_H
#define WIREFRAMEWRAPPER_H
#include "RasterRenderer.h"
#include "RendererWrapper.h"


class WireframeWrapper final : public RendererWrapper {
public:
    Camera& getCamera() override;
    Model& getModel() override;
    Light& getLight() override;

    void renderFrame(DrawingWindow& window) override;

    explicit WireframeWrapper(RasterRenderer& renderer);

private:
    RasterRenderer& renderer;
};



#endif //WIREFRAMEWRAPPER_H
