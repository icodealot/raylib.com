/*******************************************************************************************
*
*   raylib [core] example - Load textures from raw data (adapted for HTML5 platform)
*
*   NOTE: Images are loaded in CPU memory (RAM); textures are loaded in GPU memory (VRAM)
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <stdlib.h>     // Required for malloc() and free()

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#if defined(PLATFORM_ANDROID)
    #include "android_native_app_glue.h"
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = 800;
int screenHeight = 450;

Texture2D fudesumi;
Texture2D checked;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void);     // Update and Draw one frame

//----------------------------------------------------------------------------------
// Main Enry Point
//----------------------------------------------------------------------------------
#if defined(PLATFORM_ANDROID)
void android_main(struct android_app *app) 
#else
int main(void)
#endif
{
    // Initialization
    //--------------------------------------------------------------------------------------
#if defined(PLATFORM_ANDROID)
    InitWindow(screenWidth, screenHeight, app);
#else
    InitWindow(screenWidth, screenHeight, "raylib [textures] example - texture from raw data");
#endif

    // Load RAW image data (512x512, 32bit RGBA, no file header)
    Image fudesumiRaw = LoadImageRaw("resources/fudesumi.raw", 384, 512, UNCOMPRESSED_R8G8B8A8, 0);
    fudesumi = LoadTextureFromImage(fudesumiRaw);       // Upload CPU (RAM) image to GPU (VRAM)
    UnloadImage(fudesumiRaw);                           // Unload CPU (RAM) image data
    
    // Generate a checked texture by code (1024x1024 pixels)
    int width = 1024;
    int height = 1024;
    
    // Dynamic memory allocation to store pixels data (Color type)
    Color *pixels = (Color *)malloc(width*height*sizeof(Color));
    
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (((x/32+y/32)/1)%2 == 0) pixels[y*height + x] = ORANGE;
            else pixels[y*height + x] = GOLD;
        }
    }
    
    // Load pixels data into an image structure and create texture
    Image checkedIm = LoadImageEx(pixels, width, height);
    checked = LoadTextureFromImage(checkedIm);
    UnloadImage(checkedIm);     // Unload CPU (RAM) image data
    
    // Dynamic memory must be freed after using it
    free(pixels);               // Unload CPU (RAM) pixels data
    
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(fudesumi);    // Texture unloading
    UnloadTexture(checked);     // Texture unloading
    
    CloseWindow();				// Close window and OpenGL context
    //--------------------------------------------------------------------------------------
#if !defined(PLATFORM_ANDROID)
    return 0;
#endif
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(checked, screenWidth/2 - checked.width/2, screenHeight/2 - checked.height/2, Fade(WHITE, 0.5f));
        DrawTexture(fudesumi, 430, -30, WHITE);

        DrawText("CHECKED TEXTURE ", 84, 100, 30, BROWN);
        DrawText("GENERATED by CODE", 72, 164, 30, BROWN);
        DrawText("and RAW IMAGE LOADING", 46, 226, 30, BROWN);
        
        DrawText("(c) Fudesumi sprite by Eiden Marsal", 310, screenHeight - 20, 10, BROWN);

    EndDrawing();
    //----------------------------------------------------------------------------------
}