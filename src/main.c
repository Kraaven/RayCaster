#define GRAPHICS_API_OPENGL_43
#include "raylib.h"
#include "rlgl.h"
#include "math.h"
#include "stdlib.h"

#include "stdio.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "CPU-Based Raycasting");

    // Load the map texture (black & white)
    Image mapImage = LoadImage("Resources/EX_MAP.png");
    Texture2D mapTexture = LoadTextureFromImage(mapImage);

    // Create a buffer to store ray distances
    float *rayDistances = (float *)malloc(screenWidth * sizeof(float));

    Image outputImage = GenImageColor(screenWidth, 1, BLACK);
    ImageFormat(&outputImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    Texture2D outputTexture = LoadTextureFromImage(outputImage);
    Color *pixels = (Color *)outputImage.data;

    Shader fragmentShader = LoadShader(0, TextFormat("Resources/renderer.glsl", 330));
    int distancesLoc = GetShaderLocation(fragmentShader, "hitTexture");
    SetShaderValueTexture(fragmentShader, distancesLoc, outputTexture);

    int screenWidthLoc = GetShaderLocation(fragmentShader, "screenWidth");
    SetShaderValue(fragmentShader, screenWidthLoc, &screenWidth, SHADER_UNIFORM_FLOAT);

    Vector2 playerPosition = {400, 225};
    float playerRotation = 0.0f;
    const float FOV = PI / 3.0f; // 60 degrees in radians

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Rotate player with keys
        if (IsKeyDown(KEY_LEFT))
            playerRotation -= 0.05f;
        if (IsKeyDown(KEY_RIGHT))
            playerRotation += 0.05f;

        // Move player with keys
        float moveSpeed = 2.0f;
        if (IsKeyDown(KEY_W))
        {
            playerPosition.x += cosf(playerRotation) * moveSpeed;
            playerPosition.y += sinf(playerRotation) * moveSpeed;
        }
        if (IsKeyDown(KEY_S))
        {
            playerPosition.x -= cosf(playerRotation) * moveSpeed;
            playerPosition.y -= sinf(playerRotation) * moveSpeed;
        }
        if (IsKeyDown(KEY_A))
        {
            playerPosition.x += cosf(playerRotation - PI / 2) * moveSpeed;
            playerPosition.y += sinf(playerRotation - PI / 2) * moveSpeed;
        }
        if (IsKeyDown(KEY_D))
        {
            playerPosition.x += cosf(playerRotation + PI / 2) * moveSpeed;
            playerPosition.y += sinf(playerRotation + PI / 2) * moveSpeed;
        }

        // Cast rays for each column
        for (int x = 0; x < screenWidth; x++)
        {
            // Calculate ray angle for this column
            float rayAngle = playerRotation + ((float)x / screenWidth - 0.5f) * FOV;
            Vector2 rayDir = {cosf(rayAngle), sinf(rayAngle)};

            Vector2 currentPos = playerPosition;
            float distance = 0.0f;
            const float stepSize = 1.0f; // You can adjust for precision/performance
            const int maxSteps = 1000;   // Prevent infinite loops

            // Ray marching loop
            bool hitWall = false;
            for (int i = 0; i < maxSteps; i++)
            {
                int mapX = (int)currentPos.x;
                int mapY = (int)currentPos.y;

                // Check for out-of-bounds
                if (mapX < 0 || mapX >= mapImage.width || mapY < 0 || mapY >= mapImage.height)
                {
                    break;
                }

                // Get pixel color from the map image
                Color pixel = GetImageColor(mapImage, mapX, mapY);

                // White pixel (wall) check - assuming white is RGB > 200
                if (pixel.r < 200 && pixel.g < 200 && pixel.b < 200)
                {
                    rayDistances[x] = distance;
                    hitWall = true;

                    Color Distance = {0, 0, 0, 255}; // Initialize color

                    // Process red channel
                    if (distance > 255)
                    {
                        Distance.r = 255;
                        distance -= 255;
                    }
                    else
                    {
                        Distance.r = (short)distance;
                        distance = 0; // No need to continue once the distance is fully allocated
                    }

                    // Process green channel if distance remains
                    if (distance > 0)
                    {
                        if (distance > 255)
                        {
                            Distance.g = 255;
                            distance -= 255;
                        }
                        else
                        {
                            Distance.g = (short)distance;
                            distance = 0; // No need to continue once the distance is fully allocated
                        }
                    }

                    // // Process blue channel if distance remains
                    // if (distance > 0)
                    // {
                    //     if (distance > 255)
                    //     {
                    //         Distance.b = 255;
                    //         distance -= 255;
                    //     }
                    //     else
                    //     {
                    //         Distance.b = (short)distance;
                    //         distance = 0; // No need to continue once the distance is fully allocated
                    //     }
                    // }

                    // Process alpha channel if distance remains
                    if (distance > 0)
                    {
                        Distance.b = (short)distance; // Final remaining value
                    }


                    pixels[x] = Distance; // Store the result in the pixels array

                    break;
                }

                // Step forward along the ray
                currentPos.x += rayDir.x * stepSize;
                currentPos.y += rayDir.y * stepSize;
                distance += stepSize;
            }

            // If no wall was hit, set distance to maximum
            if (!hitWall)
            {
                rayDistances[x] = 1000.0f;
                pixels[x] = (Color){255, 255, 255, 255};
            }
        }

        // Update the texture with the new distance data
        UpdateTexture(outputTexture, pixels);

        if (IsKeyPressed(KEY_SPACE))
        {
            for (int i = 0; i < screenWidth; i++)
            {
                Color pixel = pixels[i];
                printf("Pixel [%d]: Color(%d, %d, %d, %d) => %d\n", i, pixel.r, pixel.g, pixel.b, pixel.a, (int)rayDistances[i]);
            }
        }

        // Render the output texture
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw ray distances using shader
        BeginShaderMode(fragmentShader);
        SetShaderValueTexture(fragmentShader, distancesLoc, outputTexture);
        DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
        EndShaderMode();

        // Debug: Draw the map texture in a corner
        DrawTextureEx(mapTexture, (Vector2){10, 10}, 0.0f, 0.25f, WHITE);

        // Draw player position on map
        DrawCircle(10 + playerPosition.x * 0.25f, 10 + playerPosition.y * 0.25f, 3, RED);

        // Draw direction line
        DrawLine(
            10 + playerPosition.x * 0.25f,
            10 + playerPosition.y * 0.25f,
            10 + (playerPosition.x + cosf(playerRotation) * 20.0f) * 0.25f,
            10 + (playerPosition.y + sinf(playerRotation) * 20.0f) * 0.25f,
            RED);

        EndDrawing();
    }

    // Cleanup
    free(rayDistances);
    UnloadImage(mapImage);
    UnloadImage(outputImage);
    UnloadTexture(mapTexture);
    UnloadTexture(outputTexture);
    UnloadShader(fragmentShader);

    CloseWindow();
    return 0;
}