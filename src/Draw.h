//
// Created by Thomas Parr on 01/10/2024.
//

#ifndef LINE_H
#define LINE_H
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>
#include <DrawingWindow.h>
#include <TextureMap.h>


class Draw {
public:
    static void drawLine(DrawingWindow& window, const CanvasPoint& from, const CanvasPoint& to, const Colour& colour);
    static void drawStrokedTriangle(DrawingWindow& window, const CanvasTriangle& triangle, const Colour& colour);
    static void drawFilledTriangle(DrawingWindow& window, const CanvasTriangle& triangle, const Colour& colour);
    static void drawTexturedTriangle(DrawingWindow& window, const CanvasTriangle& triangle, const TextureMap& texture);
};



#endif //LINE_H
