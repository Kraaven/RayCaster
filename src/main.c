#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    float BaseDiection = 0;

    


    InitWindow(screenWidth, screenHeight, "My First Raylib Project");
    SetTargetFPS(60);

    Shader Renderer = LoadShader(0, "Resources/renderer.glsl");
    SetShaderValue(Renderer,GetShaderLocation(Renderer,"resolution"), &(float[2]){ screenWidth, screenHeight}, SHADER_UNIFORM_VEC2 );

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        
        BeginShaderMode(Renderer);
        DrawRectangle(0,0,screenWidth, screenHeight, WHITE);
        EndShaderMode();

        DrawFPS(10,10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
