
#include "ndGameEngine.h"
#include "mediaLibrary.h"

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
        throw SpriteException(__FILE__, __LINE__, __func__, "[Sprite] : Pixel out of xy bound");
    }
}

Pixel* Sprite::GetDataPtr()
{
    return _pixels.data();
}


// Game Engine

void NdGameEngine::ConstructGame(
    int32_t screen_w, int32_t screen_h, int32_t pixel_w,
    int32_t pixel_h, bool full_screen)
{
    if (0 >= screen_w || 0 >= screen_h || 0 >= pixel_w || 0 >= pixel_h)
    {
        throw GameEngineException(__FILE__, __LINE__, __func__, "[Game Engine] : Invalid Game construct parameters");
    }

    _drawTarget = std::make_unique<Sprite>(screen_w, screen_h);
}

void NdGameEngine::SetDrawTarget(std::unique_ptr<Sprite> target) // caller loose ownership
{
    if (nullptr != target)
    {
        _drawTarget = std::move(target);
    }
    else
    {
        throw GameEngineException(__FILE__, __LINE__, __func__, "[Game Engine] : Draw target is null");
    }
}

void NdGameEngine::Draw(int32_t x, int32_t y, Pixel p)
{
    //MediaLibrary<>().Draw(x, y, _drawTarget->GetDataPtr());

    if (!_drawTarget)
    {
        throw GameEngineException(__FILE__, __LINE__, __func__, "[Game Engine] : Draw target is null");
    }

    switch (_pixelMode) {
        case Pixel::Tmode::NORMAL:
            _drawTarget->SetPixel(x, y, p);
            break;

        case Pixel::Tmode::MASK:
            if (255 == p.a)
                _drawTarget->SetPixel(x, y, p);
            break;

        case Pixel::Tmode::ALPHA:
        {
            Pixel dest = _drawTarget->GetPixel(x, y);
            float srcTransp = (p.a / 255.0f) * _pixelBlendFactor;
            float destTransp = 1.0f - srcTransp;
            uint8_t r = (uint8_t)(srcTransp * (float)p.r + destTransp * (float)dest.r);
            uint8_t g = (uint8_t)(srcTransp * (float)p.g + destTransp * (float)dest.g);
            uint8_t b = (uint8_t)(srcTransp * (float)p.b + destTransp * (float)dest.b);
            _drawTarget->SetPixel(x, y, Pixel(r, g, b));
            break;
        }

        case Pixel::Tmode::CUSTOM:
            throw GameEngineException(__FILE__, __LINE__, __func__, "[Game Engine] : TMode not implemented");
            break;

        default:
            throw GameEngineException(__FILE__, __LINE__, __func__, "[Game Engine] : TMode undefined");

    }

}

void NdGameEngine::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p = WHITE, uint32_t pattern = 0xFFFFFFFF)
{

}