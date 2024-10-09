//
// Created by Thomas Parr on 09/10/2024.
//

#ifndef DRAWTESTS_H
#define DRAWTESTS_H
#include <DrawingWindow.h>


class DrawTests {
public:
    static void drawRedNoise(DrawingWindow& window);
    static void drawGreyscaleInterpolation(DrawingWindow& window);
    static void drawTwoDimensionalColorInterpolation(DrawingWindow& window);
    static void drawLineTest(DrawingWindow& window);
    static void drawTexturedTriangleTest(DrawingWindow& window);
    static void drawEdgeCaseTriangles(DrawingWindow& window);

    explicit DrawTests(DrawingWindow &window);

    void run();

private:
    void handleEvent(const SDL_Event& event, DrawingWindow& window);

    DrawingWindow &window;
    int currentTest = 0;
    std::vector<void (*)(DrawingWindow&)> testFunctions = std::vector<void (*)(DrawingWindow&)>
    {
        drawRedNoise,
        drawGreyscaleInterpolation,
        drawTwoDimensionalColorInterpolation,
        drawLineTest,
        drawTexturedTriangleTest,
        drawEdgeCaseTriangles,
    };
};



#endif //DRAWTESTS_H
