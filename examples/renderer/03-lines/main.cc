/*
 * This example creates an SDL window and renderer, and then draws some lines
 * to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Lines", "1.0", "com.example.renderer-lines");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/lines", 640, 480, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }


    IMGUI_CHECKVERSION ( );
    ImGui::CreateContext ( );
    ImGui::StyleColorsDark ( );
    ImGui_ImplSDL3_InitForSDLRenderer ( window, renderer );
    ImGui_ImplSDLRenderer3_Init ( renderer );

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    ImGui_ImplSDL3_ProcessEvent ( event );
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{

    ImGui_ImplSDLRenderer3_NewFrame ( );
    ImGui_ImplSDL3_NewFrame ( );
    ImGui::NewFrame ( );

    ImGui::Begin ( "Hello, world!" );  
    ImGui::Text ( "Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO ( ).Framerate,
                    ImGui::GetIO ( ).Framerate );
    ImGui::End ( );
     
    int i;

    /* Lines (line segments, really) are drawn in terms of points: a set of
       X and Y coordinates, one set for each end of the line.
       (0, 0) is the top left of the window, and larger numbers go down
       and to the right. This isn't how geometry works, but this is pretty
       standard in 2D graphics. */
    static const SDL_FPoint line_points[] = {
        { 100, 354 }, { 220, 230 }, { 140, 230 }, { 320, 100 }, { 500, 230 },
        { 420, 230 }, { 540, 354 }, { 400, 354 }, { 100, 354 }
    };

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);  /* grey, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    /* You can draw lines, one at a time, like these brown ones... */
    SDL_SetRenderDrawColor(renderer, 127, 49, 32, 255);
    SDL_RenderLine(renderer, 240, 450, 400, 450);
    SDL_RenderLine(renderer, 240, 356, 400, 356);
    SDL_RenderLine(renderer, 240, 356, 240, 450);
    SDL_RenderLine(renderer, 400, 356, 400, 450);

    /* You can also draw a series of connected lines in a single batch... */
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderLines(renderer, line_points, SDL_arraysize(line_points));

    /* here's a bunch of lines drawn out from a center point in a circle. */
    /* we randomize the color of each line, so it functions as animation. */
    for (i = 0; i < 360; i++) {
        const float size = 30.0f;
        const float x = 320.0f;
        const float y = 95.0f - (size / 2.0f);
        SDL_SetRenderDrawColor(renderer, SDL_rand(256), SDL_rand(256), SDL_rand(256), 255);
        SDL_RenderLine(renderer, x, y, x + SDL_sinf((float) i) * size, y + SDL_cosf((float) i) * size);
    }

    
    ImGui::Render ( );
    ImGui_ImplSDLRenderer3_RenderDrawData ( ImGui::GetDrawData ( ), renderer );
    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    ImGui_ImplSDLRenderer3_Shutdown ( );
    ImGui_ImplSDL3_Shutdown ( );
    ImGui::DestroyContext ( );
}

