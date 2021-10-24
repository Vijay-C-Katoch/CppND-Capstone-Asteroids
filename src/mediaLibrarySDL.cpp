
#include "mediaLibrarySDL.h"
#include "SDL.h"


MediaLibrarySDL::MediaLibrarySDL()
{
    initFlags = SDL_INIT_VIDEO;
}

void MediaLibrarySDL::Init(bool controllerSupport)
{

}

void MediaLibrarySDL::CreateWindow(bool fullScreen )
{

}


void MediaLibrarySDL::Draw(int32_t x, int32_t y, void* pixels)
{
    SDL_Surface* surf = 
        SDL_CreateRGBSurfaceFrom(pixels, x, y, 32, x * sizeof(Uint32), 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);


    window = SDL_CreateWindow("Asteroid Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 300, SDL_WINDOW_SHOWN);

    screenSurface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_SetWindowTitle(window, "Hello World");
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    //update screen
    const uint32_t max = UINT32_MAX/20;
    for(uint32_t i = 0; i < max; i++) {
        if(i%10000 == 0)
            SDL_RenderPresent(renderer);
    }
}