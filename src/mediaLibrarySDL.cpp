
#include "mediaLibrarySDL.h"
#include "SDL.h"


void MediaLibrarySDL::Draw(int32_t x, int32_t y, void* pixels)
{
    const SDL_Surface* surf = 
        SDL_CreateRGBSurfaceFrom(pixels, x, y, 32, x * sizeof(Uint32), 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    (void)surf;
}