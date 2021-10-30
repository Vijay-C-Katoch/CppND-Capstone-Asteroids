
#include "mediaLibrarySDL.h"
#include "SDL.h"

using std::uint32_t;
using std::int32_t;

using namespace nd;

MediaLibrarySDL::MediaLibrarySDL()
{

  _windowX = SDL_WINDOWPOS_UNDEFINED;
  _windowY = SDL_WINDOWPOS_UNDEFINED;
  _windowFlags = SDL_WINDOW_SHOWN;
  _rendererFlags = SDL_RENDERER_ACCELERATED;
  _initFlags = SDL_INIT_VIDEO;

  _sdlKeys.resize(Key::ORDINAL);

  MapSDLKeys();
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

void MediaLibrarySDL::ConnectKeyPressCb(const Key& key, const KeyControllerContext::KeyFunc_t& func)
{
  _keycontroller.ConnectKeyPress(
    static_cast<ScanCode_t>(_sdlKeys[key]), 
    func
  );
}

void MediaLibrarySDL::ConnectKeyReleaseCb(const Key& key, const KeyControllerContext::KeyFunc_t& func)
{
  _keycontroller.ConnectKeyRelease(
    static_cast<ScanCode_t>(_sdlKeys[key]),
    func
  );
}

void MediaLibrarySDL::PollEvent()
{
#if 0
bool running = true;
while (running)
{
  while (SDL_PollEvent(&_sdlEvent) > 0)
  {
#endif
    if(SDL_PollEvent(&_sdlEvent) > 0)
    {
      switch (_sdlEvent.type)
      {
      case SDL_KEYDOWN:
      {
        _keycontroller.OnKeyPress(_sdlEvent.key.keysym.scancode);
        break;
      }

      case SDL_KEYUP:
      {
        _keycontroller.OnKeyRelease(_sdlEvent.key.keysym.scancode);
        break;
      }

      case SDL_QUIT:
      {
        if (_onQuitCb)
          _onQuitCb();

        SDL_Quit();
        break;
      }
      }
    }

#if 0
  }
}
#endif

}

void MediaLibrarySDL::ConnectQuitCb(MediaClientFunc_t clientCbk)
{
  _onQuitCb = clientCbk;
}


void MediaLibrarySDL::MapSDLKeys()
{
  _sdlKeys[Key::UP] = SDL_SCANCODE_UP;
  _sdlKeys[Key::DOWN] = SDL_SCANCODE_DOWN;
  _sdlKeys[Key::LEFT] = SDL_SCANCODE_LEFT;
  _sdlKeys[Key::RIGHT] = SDL_SCANCODE_RIGHT;
  _sdlKeys[Key::SPACE] = SDL_SCANCODE_SPACE;
  _sdlKeys[Key::TAB] = SDL_SCANCODE_TAB;
  _sdlKeys[Key::LSHIFT] = SDL_SCANCODE_LSHIFT;
  _sdlKeys[Key::RSHIFT] = SDL_SCANCODE_RSHIFT;
  _sdlKeys[Key::LCTRL] = SDL_SCANCODE_LCTRL;
  _sdlKeys[Key::RCTRL] = SDL_SCANCODE_RCTRL;
  _sdlKeys[Key::ESCAPE] = SDL_SCANCODE_ESCAPE;
  _sdlKeys[Key::RETURN] = SDL_SCANCODE_RETURN;
  _sdlKeys[Key::BACKSPACE] = SDL_SCANCODE_BACKSPACE;

}


