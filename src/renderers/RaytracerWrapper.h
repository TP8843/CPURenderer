#ifndef RAYTRACERWRAPPER_H
#define RAYTRACERWRAPPER_H
#include "Raytracer.h"
#include "RendererWrapper.h"


class RaytracerWrapper : public RendererWrapper {
public:
    Camera& getCamera() override;
    Model& getModel() override;

    void renderFrame(DrawingWindow& window) override;

    explicit RaytracerWrapper(Raytracer& raytracer);

private:
    Raytracer& raytracer;
};



#endif //RAYTRACERWRAPPER_H
