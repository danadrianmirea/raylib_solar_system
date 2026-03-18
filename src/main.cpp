#include "raylib.h"
#include "globals.h"
#include "game.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Game* game = nullptr;

void mainLoop()
{
    float dt = GetFrameTime();
    game->Update(dt);
    game->Draw();
}

int main()
{
    InitWindow(gameScreenWidth, gameScreenHeight, "Solar System");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    
    game = new Game(gameScreenWidth, gameScreenHeight);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    if(fullscreen) { 
        ToggleBorderlessWindowed();
    }
    while (!exitWindow)
    {
        mainLoop();
    }
    delete game;
    CloseWindow();
#endif

    return 0;
}
