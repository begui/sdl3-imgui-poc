/*
* Example taken from SDL3 repo, slightly modified to incoporate imgui
*/

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <iostream>
#define SDL_MAIN_USE_CALLBACKS 1  
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


void draw_renderer_presentation ( );

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Surface *surface = NULL;
    char *bmp_path = NULL;

    SDL_SetAppMetadata("Example Renderer Scaling Textures", "1.0", "com.example.renderer-scaling-textures");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    auto windowFlags = SDL_WINDOW_RESIZABLE;

    if (!SDL_CreateWindowAndRenderer("examples/renderer/scaling-textures", WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }


  if ( !::SDL_SetRenderLogicalPresentation ( renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_DISABLED) ) {
      std::cerr << SDL_GetError ( ) << std::endl;
    }

    
    SDL_asprintf(&bmp_path, "%ssample.bmp", SDL_GetBasePath());  
    surface = SDL_LoadBMP(bmp_path);
    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_free(bmp_path);  

    texture_width = surface->w;
    texture_height = surface->h;

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(surface);  

    IMGUI_CHECKVERSION ( );
    ImGui::CreateContext ( );
    ImGui::StyleColorsDark ( );
    ImGui_ImplSDL3_InitForSDLRenderer ( window, renderer );
    ImGui_ImplSDLRenderer3_Init ( renderer );

    return SDL_APP_CONTINUE;  
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{

  //NOTE: Adding this here, seems to work a bit better but still off 
    SDL_ConvertEventToRenderCoordinates(renderer, event);

    ImGui_ImplSDL3_ProcessEvent ( event );
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  
    }
    return SDL_APP_CONTINUE;  
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    ImGui_ImplSDLRenderer3_NewFrame ( );
    ImGui_ImplSDL3_NewFrame ( );
    ImGui::NewFrame ( );

    SDL_FRect dst_rect;
    const Uint64 now = SDL_GetTicks();

    const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  
    SDL_RenderClear(renderer);  

    
    dst_rect.w = (float) texture_width + (texture_width * scale);
    dst_rect.h = (float) texture_height + (texture_height * scale);
    dst_rect.x = ((float) (WINDOW_WIDTH - dst_rect.w)) / 2.0f;
    dst_rect.y = ((float) (WINDOW_HEIGHT - dst_rect.h)) / 2.0f;
    SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    draw_renderer_presentation();

    ImGui::Render ( );
    ImGui_ImplSDLRenderer3_RenderDrawData ( ImGui::GetDrawData ( ), renderer );


    SDL_RenderPresent(renderer);  

    return SDL_APP_CONTINUE;  
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyTexture(texture);

        ImGui_ImplSDLRenderer3_Shutdown ( );
        ImGui_ImplSDL3_Shutdown ( );
        ImGui::DestroyContext ( );
}
///
//
///
bool draw_logical_size_widget ( int32_t &width, int32_t &height, SDL_RendererLogicalPresentation & mode) {
  bool changed = false;

  ImGui::Text ( "Logical Resolution" );
  if ( ImGui::InputInt ( "Width##LogicalWidth", &width ) )
    changed = true;
  if ( ImGui::InputInt ( "Height##LogicalHeight", &height ) )
    changed = true;


int currentItem   = static_cast< int > ( mode );
 static constexpr const char* modes[] {
        "None", 
        "Stretch", 
        "Letterbox", 
        "Overscan", 
        "Integer Scale"
  };

  if ( ImGui::Combo ( "Presentation Type", &currentItem, modes, IM_ARRAYSIZE ( modes ) ) ) {
    mode = static_cast<SDL_RendererLogicalPresentation>(currentItem); 
    changed          = true;
  }

  return changed;
}
///
//
///
void draw_renderer_presentation (  ) {
int32_t logicalW{ 0 }, logicalH{ 0 };

  SDL_RendererLogicalPresentation logicalMode;
  if ( !::SDL_GetRenderLogicalPresentation ( renderer, &logicalW, &logicalH, &logicalMode) ) {
    std::cerr << SDL_GetError ( ) << std::endl;
    return;
  }
  //NOTE: You can comment this out to include the imgui within the logical presentation 
  if ( !::SDL_SetRenderLogicalPresentation ( renderer, logicalW, logicalH, SDL_LOGICAL_PRESENTATION_DISABLED) ) {
    std::cerr << SDL_GetError ( ) << std::endl;
    return;
  }

  ImGui::Begin ( "Renderer Debug Info" );
  ImGui::Text ( "Performance:" );
  ImGui::Text ( "  %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO ( ).Framerate, ImGui::GetIO ( ).Framerate );


  if( draw_logical_size_widget ( logicalW, logicalH, logicalMode) ){
    // do nothing
  }

int32_t outputW, outputH;
 if ( !SDL_GetCurrentRenderOutputSize (renderer, &outputW, &outputH ) ) {
    std::cerr << SDL_GetError ( ) << std::endl;
 }
ImGui::Text ( "Renderer Output Size: %dx%d", outputW, outputH);

 // logical size
  ImGui::Text ( "Logical Size: %dx%d", logicalW, logicalH);


  const char *rendererName = SDL_GetRendererName(renderer);
    ImGui::Text("Renderer Details:");
    ImGui::Text("  Name: %s", rendererName);

  // Scale
  float scaleX, scaleY;
  if ( !::SDL_GetRenderScale ( renderer, &scaleX, &scaleY ) ) {
    std::cerr << SDL_GetError ( ) << std::endl;
  }
  ImGui::Text ( "Scale: %.2fx%.2f", scaleX, scaleY );

  // Viewport
  SDL_Rect viewport;
  if ( !::SDL_GetRenderViewport( renderer, &viewport) ) {
    std::cerr << SDL_GetError ( ) << std::endl;
  }
  ImGui::Text ( "Viewport: x=%d y=%d w=%d h=%d", viewport.x, viewport.y, viewport.w, viewport.h );

  ImGui::End ( );

  if ( !::SDL_SetRenderLogicalPresentation ( renderer, logicalW, logicalH, logicalMode) ) {
    std::cerr << SDL_GetError ( ) << std::endl;
    return;
  }
}