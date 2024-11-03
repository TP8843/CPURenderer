//
// Created by Thomas Parr on 28/10/2024.
//

#ifndef FRAGMENTSHADERS_H
#define FRAGMENTSHADERS_H

#include <CanvasTriangle.h>

#include "FragmentData.h"


struct FragmentShaders
{
    static void filled(CanvasTriangle triangle,
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
};


#endif //FRAGMENTSHADERS_H
