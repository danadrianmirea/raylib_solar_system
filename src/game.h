#pragma once

#include <string>
#include <vector>
#include "raylib.h"
#include "globals.h"

class Game
{
public:
    Game(int width, int height);
    ~Game();
    void Update(float dt);
    void Draw();

private:
    int width;
    int height;

    struct CelestialBody
    {
        float x = 0.0f;
        float y = 0.0f;
        float radius = 1.0f;
        Color color = WHITE;
        double mass = 0.0;
        float orbitalRadius = 0.0f;
        float orbitalSpeed = 0.0f;
        float angle = 0.0f;
        std::string name;

        void Update(float dt);
        void Draw(int screenWidth, int screenHeight, float viewZoom, float viewOffsetX, float viewOffsetY) const;
    };

    static Vector2 ScreenToWorld(float screenX, float screenY, int screenWidth, int screenHeight, float viewZoom, float viewOffsetX, float viewOffsetY);

    void HandleInput();
    void DrawSolarSystem() const;

    // Simulation settings
    const float simulationSpeed = 1.5f;

    // View settings
    const float zoomFactor = 1.2f;
    const float minZoom = 0.1f;
    const float maxZoom = 10.0f;
    float viewZoom = 0.83f;
    float viewOffsetX = 227.0f;
    float viewOffsetY = 65.0f;
    bool isPanning = false;
    Vector2 lastMousePos = {0.0f, 0.0f};

    // Bodies
    CelestialBody sun;
    std::vector<CelestialBody> planets;
};