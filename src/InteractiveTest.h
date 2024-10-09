//
// Created by Thomas Parr on 09/10/2024.
//

#ifndef INTERACTIVETEST_H
#define INTERACTIVETEST_H
#include <CanvasTriangle.h>
#include <Colour.h>
#include <DrawingWindow.h>


class InteractiveTest {
public:
    explicit InteractiveTest(DrawingWindow &window);

    void run();

private:
    void handleEvent(const SDL_Event& event, const DrawingWindow &window);

    DrawingWindow &window;

    // Between 0 and 3 based on which vertex is currently selected. 3 means no vertex selected
    int selectedVertex;

    CanvasTriangle triangle;
    Colour triangleColour;
    Colour vertexColour;
};



#endif //INTERACTIVETEST_H
