//
// Created by tparr on 16/11/24.
//

#include "DebugHandler.h"

#include "../helper/constants.h"
#include "../helper/Draw.h"
#include "../renderers/RasterRenderer.h"

void DebugHandler::handleFrame(DrawingWindow &window, float deltaTime)
{
    switch (currentDebug)
    {
        case 1: drawFaceNormals(window); break;
        case 2: drawVertexNormals(window); break;
        default: break;
    }
}

void DebugHandler::handleEvent(DrawingWindow &window, SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == constants::keyboard::DEBUG_SWITCH)
        {
            currentDebug = (currentDebug + 1) % 3;
        }
    }
}

DebugHandler::DebugHandler(Model &model, Transformation& camera) :
    model(model), camera(camera), currentDebug(0)
{
}

void DebugHandler::drawFaceNormals(DrawingWindow &window) const
{
    for (ModelTriangle& triangle : model.getRasterPreparedTriangles(camera))
    {
        const glm::vec3 centre = (triangle.vertices.at(0) + triangle.vertices.at(1) + triangle.vertices.at(2)) / 3.0f;
        Draw::drawLine(
            window,
            RasterRenderer::projectVertexOntoCanvasPoint(centre, window.width, window.height, camera.scale),
            RasterRenderer::projectVertexOntoCanvasPoint(centre + triangle.normal, window.width, window.height, camera.scale),
            Colour(255, 255, 255));
    }
}

void DebugHandler::drawVertexNormals(DrawingWindow &window) const
{
    for (ModelTriangle& triangle : model.getRasterPreparedTriangles(camera))
    {
        for (int i = 0; i < 3; i++)
        {
            Draw::drawLine(
            window,
            RasterRenderer::projectVertexOntoCanvasPoint(triangle.vertices[i], window.width, window.height, camera.scale),
            RasterRenderer::projectVertexOntoCanvasPoint(triangle.vertices[i] + triangle.vertexNormals[i], window.width, window.height, camera.scale),
            Colour(255, 255, 255));
        }
    }
}
