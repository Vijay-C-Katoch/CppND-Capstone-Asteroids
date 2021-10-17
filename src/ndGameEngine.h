#ifndef ND_GAME_ENGINE_H_
#define ND_GAME_ENGINE_H_

#include <cstdint>
#include <vector>

#include "ndException.h"

namespace nd
{
    namespace defaults 
    {
        constexpr uint8_t   alpha = 0xFF; //opacity. 255 = opaque
        constexpr uint32_t  pixelValue = (alpha <<24);
    }

    // utility functions
    template<typename T>
    bool IsInBounds(const T& value, const T& low, const T& high) {
        return !(value < low) && !(high < value);
    }

    // Represents a 32 bit RGBA color
    struct Pixel
    {
        union
        {
            uint32_t data = defaults::pixelValue;
            struct { uint8_t r; uint8_t g; uint8_t b; uint8_t a; };
        };

        // Transparency mode
        enum class Tmode { NORMAL, ALPHA, MASK, CUSTOM };

        Pixel() = default;
        Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = defaults::alpha);
        Pixel(uint32_t p);
    };

    // useful Pixel constants
    static const Pixel
        WHITE(255, 255, 255),
        BLACK(0, 0, 0), BLANK(0, 0, 0, 0),
        RED(255, 0, 0), DARK_RED(128, 0, 0), VERY_DARK_RED(64, 0, 0),
        GREEN(0, 255, 0), DARK_GREEN(0, 128, 0), VERY_DARK_GREEN(0, 64, 0),
        BLUE(0, 0, 255), DARK_BLUE(0, 0, 128), VERY_DARK_BLUE(0, 0, 64),
        GREY(192, 192, 192), DARK_GREY(128, 128, 128), VERY_DARK_GREY(64, 64, 64),
        YELLOW(255, 255, 0), DARK_YELLOW(128, 128, 0), VERY_DARK_YELLOW(64, 64, 0),
        CYAN(0, 255, 255), DARK_CYAN(0, 128, 128), VERY_DARK_CYAN(0, 64, 64),
        MAGENTA(255, 0, 255), DARK_MAGENTA(128, 0, 128), VERY_DARK_MAGENTA(64, 0, 64);
        

    // 2D Sprite game character representation
    class Sprite
    {
    public:
        Sprite() = default;
        Sprite(int32_t w, int32_t h);
        ~Sprite();

        Pixel GetPixel(int32_t x, int32_t y)const;
        void SetPixel(int32_t x, int32_t y, Pixel p);
        Pixel* GetDataPtr();

    private:
        int32_t _width = 0;
        int32_t _height = 0;
        std::vector<Pixel> _pixels;

    };

    // Game Engine Exceptions type
    using PixelException = ClassException<Pixel, RuntimeException>;
    using SpriteException = ClassException<Sprite, RuntimeException>;


    // Keys
    enum class Key
    {
        NONE,
        UP, DOWN, LEFT, RIGHT,
        SPACE, TAB, SHIFT, CTRL,
        ESCAPE, RETURN, BACK
    };

    class NdGameEngine
    {
    public:
        NdGameEngine();
    private:
    };

}

#endif /* ND_GAME_ENGINE_H_ */