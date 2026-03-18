#include "game.h"
#include "globals.h"
#include "raylib.h"

#include <algorithm>
#include <cmath>

void Game::CelestialBody::Update(float dt)
{
    if (orbitalRadius != 0.0f && orbitalSpeed != 0.0f)
    {
        angle += orbitalSpeed * dt;
        x = cosf(angle) * orbitalRadius;
        y = sinf(angle) * orbitalRadius;
    }
}

void Game::CelestialBody::Draw(int screenWidth, int screenHeight, float viewZoom, float viewOffsetX, float viewOffsetY) const
{
    float screenX = x * viewZoom + screenWidth / 2.0f - viewOffsetX * viewZoom;
    float screenY = y * viewZoom + screenHeight / 2.0f - viewOffsetY * viewZoom;
    int scaledRadius = std::max(1, (int)(radius * viewZoom));

    DrawCircle((int)screenX, (int)screenY, (float)scaledRadius, color);

    if (!name.empty())
    {
        int fontSize = std::max(12, (int)(16.0f * viewZoom));
        int textWidth = MeasureText(name.c_str(), fontSize);
        int textX = (int)screenX - textWidth / 2;
        int textY = (int)(screenY - scaledRadius - 10 - fontSize / 2);
        DrawText(name.c_str(), textX, textY, fontSize, WHITE);
    }
}

Vector2 Game::ScreenToWorld(float screenX, float screenY, int screenWidth, int screenHeight, float viewZoom, float viewOffsetX, float viewOffsetY)
{
    float worldX = (screenX - screenWidth / 2.0f + viewOffsetX * viewZoom) / viewZoom;
    float worldY = (screenY - screenHeight / 2.0f + viewOffsetY * viewZoom) / viewZoom;
    return {worldX, worldY};
}

Game::Game(int width, int height) : width(width), height(height)
{
    // Create celestial bodies (values match src/solar_system.py)
    sun = CelestialBody{0.0f, 0.0f, 30.0f, YELLOW, 1.989e30, 0.0f, 0.0f, 0.0f, "Sun"};

    // Spread bodies farther apart (initial orbital radius + starting x/y).
    // This does not change planet size or orbital speed; it only scales distance.
    constexpr float spacingFactor = 1.5f;

    const float speed = simulationSpeed;
    planets = {
        CelestialBody{60.0f * spacingFactor, 0.0f, 4.0f, GRAY, 3.285e23, 60.0f * spacingFactor, 4.17f * speed, 0.0f, "Mercury"},
        CelestialBody{90.0f * spacingFactor, 0.0f, 6.0f, ORANGE, 4.867e24, 90.0f * spacingFactor, 1.61f * speed, 0.0f, "Venus"},
        CelestialBody{150.0f * spacingFactor, 0.0f, 10.0f, GREEN, 5.972e24, 150.0f * spacingFactor, 1.0f * speed, 0.0f, "Earth"},
        CelestialBody{200.0f * spacingFactor, 0.0f, 8.0f, RED, 6.39e23, 200.0f * spacingFactor, 0.53f * speed, 0.0f, "Mars"},
        CelestialBody{300.0f * spacingFactor, 0.0f, 20.0f, ORANGE, 1.898e27, 300.0f * spacingFactor, 0.084f * speed, 0.0f, "Jupiter"},
        CelestialBody{400.0f * spacingFactor, 0.0f, 18.0f, YELLOW, 5.683e26, 400.0f * spacingFactor, 0.034f * speed, 0.0f, "Saturn"},
        CelestialBody{500.0f * spacingFactor, 0.0f, 15.0f, SKYBLUE, 8.681e25, 500.0f * spacingFactor, 0.012f * speed, 0.0f, "Uranus"},
        CelestialBody{600.0f * spacingFactor, 0.0f, 15.0f, DARKBLUE, 1.024e26, 600.0f * spacingFactor, 0.006f * speed, 0.0f, "Neptune"},
    };
}

Game::~Game() = default;

void Game::HandleInput()
{
    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        isPanning = true;
        lastMousePos = mousePos;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        isPanning = false;
    }

    if (isPanning)
    {
        float dx = (mousePos.x - lastMousePos.x) / viewZoom;
        float dy = (mousePos.y - lastMousePos.y) / viewZoom;
        viewOffsetX -= dx;
        viewOffsetY -= dy;
        lastMousePos = mousePos;
    }

    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f)
    {
        // Match src/solar_system.py:
        // - zoom in/out clamped
        // - "zoom at mouse position" math preserved as-is (even though it cancels out)
        float oldZoom = viewZoom;
        Vector2 world = ScreenToWorld(mousePos.x, mousePos.y, width, height, viewZoom, viewOffsetX, viewOffsetY);

        if (wheel > 0.0f)
        {
            viewZoom = std::min(viewZoom * zoomFactor, maxZoom);
            viewOffsetX += (world.x - (mousePos.x - width / 2.0f + viewOffsetX * oldZoom) / oldZoom);
            viewOffsetY += (world.y - (mousePos.y - height / 2.0f + viewOffsetY * oldZoom) / oldZoom);
        }
        else
        {
            viewZoom = std::max(viewZoom / zoomFactor, minZoom);
            viewOffsetX += (world.x - (mousePos.x - width / 2.0f + viewOffsetX * oldZoom) / oldZoom);
            viewOffsetY += (world.y - (mousePos.y - height / 2.0f + viewOffsetY * oldZoom) / oldZoom);
        }
    }
}

void Game::Update(float dt)
{
    if (dt == 0.0f)
    {
        return;
    }

    if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))
    {
        exitWindow = true;
        return;
    }

    HandleInput();

    for (auto& p : planets)
    {
        p.Update(dt);
    }
}

void Game::DrawSolarSystem() const
{
    ClearBackground(BLACK);

    sun.Draw(width, height, viewZoom, viewOffsetX, viewOffsetY);
    for (const auto& p : planets)
    {
        p.Draw(width, height, viewZoom, viewOffsetX, viewOffsetY);
    }

    // UI text (matches src/solar_system.py)
    int fontSize = 16;
    int y = 10;
    DrawText(TextFormat("Zoom: %.2fx", viewZoom), 10, y, fontSize, WHITE);
    y += 20;
    DrawText(TextFormat("View Offset X: %.1f", viewOffsetX), 10, y, fontSize, WHITE);
    y += 20;
    DrawText(TextFormat("View Offset Y: %.1f", viewOffsetY), 10, y, fontSize, WHITE);

    const char* controls[] = {
        "Controls:",
        "Left Click + Drag: Pan View",
        "Mouse Wheel: Zoom In/Out",
        "ESC: Quit",
    };

    int xRight = width - 250;
    y = 10;
    for (const char* line : controls)
    {
        DrawText(line, xRight, y, fontSize, WHITE);
        y += 20;
    }
}

void Game::Draw()
{
    BeginDrawing();
    DrawSolarSystem();
    EndDrawing();
}

