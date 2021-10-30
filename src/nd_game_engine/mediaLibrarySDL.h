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
#include <vector>
#include <SDL.h>
#undef main     // unresolved external symbol _SDL_main

#include "ndException.h"
#include "hw/keyboardController.h"

class MediaLibrarySDL
{
public:

    MediaLibrarySDL();

    void Init(bool controllerSupport);

    void CreateWindow(
        std::uint32_t	screenWidth = 256,
        std::uint32_t	screenHeight = 240,
        std::uint32_t pixelWidth = 4,
        std::uint32_t pixelHeight = 4,
        bool fullScreen = false);

    void Draw(int32_t width, int32_t height, void* pixels);

    void ConnectKeyPressCb(const nd::Key& key, const nd::KeyControllerContext::KeyFunc_t& func);

    void ConnectKeyReleaseCb(const nd::Key& key, const nd::KeyControllerContext::KeyFunc_t& func);

    void PollEvent();

    // Media Client callbacks
    using MediaClientFunc_t = std::function<void(void)>;
    void ConnectQuitCb(MediaClientFunc_t clientCbk);

private:
    SDL_Window* _window = NULL;
    SDL_Surface* _screenSurface = NULL;
    SDL_Renderer* _renderer = NULL;
    SDL_GameController* _controller = NULL;
    std::uint32_t _windowX;
    std::uint32_t _windowY;
    std::uint32_t _windowFlags;
    std::uint32_t _rendererFlags;
    std::uint32_t _initFlags;
    bool _fullScreen = false;

    nd::KeyController _keycontroller;

    // SDL KeyMap
    std::vector<SDL_Scancode> _sdlKeys;

    // handle input and events
    SDL_Event _sdlEvent;

    // client callbacks
    MediaClientFunc_t _onQuitCb;

    void MapSDLKeys();

};

// Media Library Exceptions type
using MediaLibSDLException = nd::ClassException<MediaLibrarySDL, nd::RuntimeException>;

#endif /* MEDIA_LIBRARY_SDL_H_ */