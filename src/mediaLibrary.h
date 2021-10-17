////////////////////////////////////////////////////////////////////////
// Media Library Wrapper
//
// This is wrapper over any type of Media Library.
// Programming to interface provided by mediaLibrary.h make it possible
// for Game Engine to work with different media libraries such as SDL,
// OpenGL etc.
// 
// It is an implementatio of Startegy Pattern using variadic class
// templates.
// 
////////////////////////////////////////////////////////////////////////

#ifndef MEDIA_LIBRARY_H_
#define MEDIA_LIBRARY_H_

#include "mediaLibrarySDL.h"  // default media library


template<typename InLibrary= MediaLibrarySDL>
class MediaLibrary
{
public:

    template<typename... Args>
    void Draw(Args &&..args);
};



#endif /* MEDIA_LIBRARY_H_ */