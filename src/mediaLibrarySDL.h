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

class MediaLibrarySDL
{
public:

    MediaLibrarySDL() = default;

    void Draw(int32_t x, int32_t y, void* pixels);

};

#endif /* MEDIA_LIBRARY_SDL_H_ */