//
// Created by Thomas Parr on 28/10/2024.
//

#ifndef FRAGMENTSHADERS_H
#define FRAGMENTSHADERS_H
#include <CanvasTriangle.h>
#include <DrawingWindow.h>

#include "PointData.h"


struct FragmentShaders {
    static void filled(DrawingWindow &window,
        CanvasTriangle triangle,
        int x,
        int y,
        float **depthBuffer,
        const PointData::FilledData& data);

    static void rainbow(DrawingWindow &window,
    CanvasTriangle triangle,
    int x,
    int y,
    float **depthBuffer,
    const PointData::FilledData& data);

    static void outline(DrawingWindow &window,
    CanvasTriangle triangle,
    int x,
    int y,
    float **depthBuffer,
    const PointData::FilledData& data);

    static void depth(DrawingWindow &window,
    CanvasTriangle triangle,
    int x,
    int y,
    float **depthBuffer,
    const PointData::FilledData& data);
};



#endif //FRAGMENTSHADERS_H
