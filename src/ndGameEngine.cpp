
#include "ndGameEngine.h"

using namespace nd;

Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) 
{
    data = red | (green << 8) | (blue << 16) | (alpha << 24);
}

Pixel::Pixel(uint32_t p)
{
    data = p;
}

Sprite::Sprite(int32_t w, int32_t h) : _width{w}, _height{h}
{
    _pixels.resize(_width * _height);
}

Sprite::~Sprite()
{
    _pixels.clear();
}

Pixel Sprite::GetPixel(int32_t x, int32_t y) const
{
    if (IsInBounds(x, 0, _width) && IsInBounds(y, 0, _height))
        return _pixels[y * _width + x];
    else
        return Pixel(0, 0, 0, 0);   // Blank pixel
}

void Sprite::SetPixel(int32_t x, int32_t y, Pixel p)
{
    if (IsInBounds(x, 0, _width) && IsInBounds(y, 0, _height))
    {
        _pixels[y * _width + x] = p;
    }
    else
    {
        // throw exception
    }
}

Pixel* Sprite::GetDataPtr()
{
    return _pixels.data();
}