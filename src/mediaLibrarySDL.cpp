
#include "mediaLibrarySDL.h"
#include "SDL.h"

using std::uint32_t;
using std::int32_t;

MediaLibrarySDL::MediaLibrarySDL()
{

    _windowX = SDL_WINDOWPOS_UNDEFINED;
    _windowY = SDL_WINDOWPOS_UNDEFINED;
    _windowFlags = SDL_WINDOW_SHOWN;
    _rendererFlags = SDL_RENDERER_ACCELERATED;
    _initFlags = SDL_INIT_VIDEO;
}

void MediaLibrarySDL::Init(bool controllerSupport)
{
    if (controllerSupport)
    {
        _initFlags |= SDL_INIT_GAMECONTROLLER;
    }
}

void MediaLibrarySDL::CreateWindow(
    uint32_t screenWidth,
    uint32_t screenHeight,
    uint32_t pixelWidth, 
    uint32_t pixelHeight, 
    bool fullScreen )
{
    if (fullScreen)
    {
        _windowFlags |= SDL_WINDOW_FULLSCREEN;
    }


    int32_t windowWidth = screenWidth * pixelWidth;
    int32_t windowHeight = screenHeight * pixelHeight;

    if (SDL_Init(_initFlags) < 0)
    {
        throw MediaLibSDLException(__FILE__, __LINE__, __func__, "[SDL2] : SDL Initialization error: ", SDL_GetError());
    }

    _window = SDL_CreateWindow("", _windowX, _windowY, windowWidth, windowHeight, _windowFlags);
    if (_window == NULL)
    {
        throw MediaLibSDLException(__FILE__, __LINE__, __func__, "[SDL2] : Window creation error: ", SDL_GetError());
    }

    _screenSurface = SDL_GetWindowSurface(_window);
    _renderer = SDL_CreateRenderer(_window, -1, _rendererFlags);
    if (_renderer == NULL)
    {
        throw MediaLibSDLException(__FILE__, __LINE__, __func__, "[SDL2] : Renderer creation error: ", SDL_GetError());
    }

}


void MediaLibrarySDL::Draw(int32_t width, int32_t height, void* pixels)
{

    SDL_Surface* surf = SDL_CreateRGBSurfaceFrom(pixels, width, height, 32, width * sizeof(Uint32), 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surf);
    SDL_RenderCopy(_renderer, texture, NULL, NULL);
    SDL_RenderPresent(_renderer);
    SDL_DestroyTexture(texture);

}
