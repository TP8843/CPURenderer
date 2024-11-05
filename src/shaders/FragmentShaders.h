//
// Created by Thomas Parr on 28/10/2024.
//

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
                       const FragmentData::Data& data);

    static void filledPhong(CanvasTriangle triangle,
                            int x,
                            int y,
                            const FragmentData::DataUniform& uniform,
                            const FragmentData::Data& data);

    static void rainbow(CanvasTriangle triangle,
                        int x,
                        int y,
                        const FragmentData::DataUniform& uniform,
                        const FragmentData::Data& data);

    static void outline(CanvasTriangle triangle,
                        int x,
                        int y,
                        const FragmentData::DataUniform& uniform,
                        const FragmentData::Data& data);

    static void depth(CanvasTriangle triangle,
                      int x,
                      int y,
                      const FragmentData::DataUniform& uniform,
                      const FragmentData::Data& data);

    static void material(CanvasTriangle triangle,
                         int x,
                         int y,
                         const FragmentData::DataUniform& uniform,
                         const FragmentData::Data& data);

    static void materialPhong(CanvasTriangle triangle,
                     int x,
                     int y,
                     const FragmentData::DataUniform& uniform,
                     const FragmentData::Data& data);
};


#endif //FRAGMENTSHADERS_H
