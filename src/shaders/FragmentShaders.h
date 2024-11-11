#ifndef FRAGMENTSHADERS_H
#define FRAGMENTSHADERS_H

#include <CanvasTriangle.h>

#include "FragmentData.h"


struct FragmentShaders
{
    static void prePass(const CanvasTriangle& triangle,
                        int x,
                        int y,
                        const FragmentData::PrePassUniform& uniform,
                        const FragmentData::PrePassData& data);

    static void filled(const CanvasTriangle& triangle,
                       int x,
                       int y,
                       const FragmentData::DataUniform& uniform,
                       const FragmentData::FilledData& data);

    static void filledPhong(const CanvasTriangle& triangle,
                     int x,
                     int y,
                     const FragmentData::DataUniform& uniform,
                     const FragmentData::FilledData& data);

    static void rainbow(const CanvasTriangle& triangle,
                        int x,
                        int y,
                        const FragmentData::DataUniform& uniform,
                        const FragmentData::FilledData& data);

    static void outline(const CanvasTriangle& triangle,
                        int x,
                        int y,
                        const FragmentData::DataUniform& uniform,
                        const FragmentData::FilledData& data);

    static void depth(const CanvasTriangle& triangle,
                      int x,
                      int y,
                      const FragmentData::DataUniform& uniform,
                      const FragmentData::FilledData& data);

    static void material(const CanvasTriangle& triangle,
                         int x,
                         int y,
                         const FragmentData::DataUniform& uniform,
                         const FragmentData::TextureData& data);

    static void materialPhong(const CanvasTriangle& triangle,
                     int x,
                     int y,
                     const FragmentData::DataUniform& uniform,
                     const FragmentData::TextureData& data);
};


#endif //FRAGMENTSHADERS_H
