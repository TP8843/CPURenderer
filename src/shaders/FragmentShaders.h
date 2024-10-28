//
// Created by Thomas Parr on 28/10/2024.
//

#ifndef FRAGMENTSHADERS_H
#define FRAGMENTSHADERS_H
#include <CanvasTriangle.h>
#include <DrawingWindow.h>

#include "FragmentData.h"


struct FragmentShaders {
    static void filled(DrawingWindow &window,
        CanvasTriangle triangle,
        int x,
        int y,
        float **depthBuffer,
        const FragmentData::FilledDataUniform& uniform,
        const FragmentData::FilledData& data);

    static void rainbow(DrawingWindow &window,
    CanvasTriangle triangle,
    int x,
    int y,
    float **depthBuffer,
    const FragmentData::FilledDataUniform& uniform,
    const FragmentData::FilledData& data);

    static void outline(DrawingWindow &window,
    CanvasTriangle triangle,
    int x,
    int y,
    float **depthBuffer,
    const FragmentData::FilledDataUniform& uniform,
    const FragmentData::FilledData& data);

    static void depth(DrawingWindow &window,
    CanvasTriangle triangle,
    int x,
    int y,
    float **depthBuffer,
    const FragmentData::FilledDataUniform& uniform,
    const FragmentData::FilledData& data);

    static void material(DrawingWindow &window,
    CanvasTriangle triangle,
    int x,
    int y,
    float **depthBuffer,
    const FragmentData::TextureDataUniform& uniform,
    const FragmentData::TextureData& data);
};



#endif //FRAGMENTSHADERS_H
