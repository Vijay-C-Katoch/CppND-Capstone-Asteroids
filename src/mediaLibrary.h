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

#include <utility>

template<typename InLibrary>
class MediaLibrary
{
public:

    MediaLibrary() = default;

    template<typename... Args>
    void Init(Args &&...args)
    {
        _media.Init(std::forward<Args>(args) ...);
    }

    template<typename... Args>
    void CreateWindow(Args &&...args)
    {
        _media.CreateWindow(std::forward<Args>(args) ...);
    }

    template<typename... Args>
    void Draw(Args &&...args)
    {
        _media.Draw(std::forward<Args>(args) ...);
    }

private:
    InLibrary _media;
};



#endif /* MEDIA_LIBRARY_H_ */