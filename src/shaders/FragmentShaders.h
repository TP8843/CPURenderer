#ifndef FRAGMENTSHADERS_H
#define FRAGMENTSHADERS_H

#include <CanvasTriangle.h>

#include "FragmentData.h"


struct FragmentShaders
{
    static void prePass(CanvasTriangle triangle,
                        int x,
                        int y,
                        const FragmentData::PrePassUniform& uniform,
                        const FragmentData::PrePassData& data);

    static void filled(CanvasTriangle triangle,
                       int x,
                       int y,
                       const FragmentData::DataUniform& uniform,
                       const FragmentData::FilledData& data);

    static void filledPhong(CanvasTriangle triangle,
                     int x,
                     int y,
                     const FragmentData::DataUniform& uniform,
                     const FragmentData::FilledData& data);

    static void rainbow(CanvasTriangle triangle,
                        int x,
                        int y,
                        const FragmentData::DataUniform& uniform,
                        const FragmentData::FilledData& data);

    static void outline(CanvasTriangle triangle,
                        int x,
                        int y,
                        const FragmentData::DataUniform& uniform,
                        const FragmentData::FilledData& data);

    static void depth(CanvasTriangle triangle,
                      int x,
                      int y,
                      const FragmentData::DataUniform& uniform,
                      const FragmentData::FilledData& data);

    static void material(CanvasTriangle triangle,
                         int x,
                         int y,
                         const FragmentData::DataUniform& uniform,
                         const FragmentData::TextureData& data);

    static void materialPhong(CanvasTriangle triangle,
                     int x,
                     int y,
                     const FragmentData::DataUniform& uniform,
                     const FragmentData::TextureData& data);
};


#endif //FRAGMENTSHADERS_H
