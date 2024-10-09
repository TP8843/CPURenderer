#include <CanvasTriangle.h>
#include <Colour.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>

#include "Draw.h"
#include "DrawTests.h"
#include "InteractiveTest.h"
#include "Interpolation.h"
#include "Model.h"

#define WIDTH 640
#define HEIGHT 480

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);

	// auto interactiveTest = InteractiveTest(window);
	// interactiveTest.run();

	auto drawTests = DrawTests(window);
	drawTests.run();
}
