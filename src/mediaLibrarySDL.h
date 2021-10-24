////////////////////////////////////////////////////////////////////////
// SDL Media Library Wrapper
//
// This is wrapper over SDL Media Library.
// This is default library to implement Startegy pattern
// in mediaLibrary.h 
// 
////////////////////////////////////////////////////////////////////////

#ifndef MEDIA_LIBRARY_SDL_H_
#define MEDIA_LIBRARY_SDL_H_

#include <cstdint>
#include <SDL.h>
#undef main     // unresolved external symbol _SDL_main

class MediaLibrarySDL
{
public:

    MediaLibrarySDL();

    void Init(bool controllerSupport);

    void CreateWindow(bool fullScreen);

    void Draw(int32_t x, int32_t y, void* pixels);

private:
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_GameController* controller = NULL;
    uint32_t initFlags;
    bool fullScreen = false;

};

#endif /* MEDIA_LIBRARY_SDL_H_ */