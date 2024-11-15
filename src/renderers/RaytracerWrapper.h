#ifndef RAYTRACERWRAPPER_H
#define RAYTRACERWRAPPER_H
#include "Raytracer.h"
#include "RendererWrapper.h"


class RaytracerWrapper final : public RendererWrapper {
public:
    Transformation& getCamera() override;
    Model& getModel() override;
    Transformation& getLight() override;

    void renderFrame(DrawingWindow& window) override;

    explicit RaytracerWrapper(Raytracer& raytracer);

private:
    Raytracer& raytracer;
};



#endif //RAYTRACERWRAPPER_H
