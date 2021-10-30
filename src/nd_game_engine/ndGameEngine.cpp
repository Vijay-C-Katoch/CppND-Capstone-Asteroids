
#include <cstddef>
#include <thread>

#include "ndGameEngine.h"
#include "ndUtils.h"

using namespace nd;
using namespace std::chrono;

using std::uint8_t;
using std::int32_t;
using std::uint32_t;
using std::size_t;

// global
std::atomic<bool> nd::g_isEngineRunning = false;


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

  _screenWidth = screen_w;
  _screenHeight = screen_h;
  _pixelWidth = pixel_w;
  _pixelHeight = pixel_h;
  _fullScreen = full_screen;

  _drawTarget = std::make_unique<Sprite>(_screenWidth, _screenHeight);
}

void NdGameEngine::Run()
{
  //enable engine thread run
  g_isEngineRunning = true;
  std::thread t{ &NdGameEngine::GameEngineThread, this };

  t.join();
}

void NdGameEngine::onClientCreate()
{
  return;
}

void NdGameEngine::onClientUpdate(float elapsedTicks)
{
  return;
}

void NdGameEngine::onClientDestroy()
{
  return;
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

int32_t NdGameEngine::GetDrawTargetWidth()
{
  if (_drawTarget)
    return _drawTarget->Width();
  else
    return 0;
}

int32_t NdGameEngine::GetDrawTargetHeight()
{
  if (_drawTarget)
    return _drawTarget->Height();
  else
    return 0;
}


void NdGameEngine::Draw(int32_t x, int32_t y, Pixel p)
{
  if (!_drawTarget) {
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


void NdGameEngine::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p, uint32_t pattern)
{
  //Bresenham's line draw algorithm
  const int32_t rise = y2 - y1;
  const int32_t run = x2 - x1;
        
  if (0 == run) {         // vertical line x1 = x2
    if (y2 < y1) {
      std::swap(y2, y1);
    }
    for (int32_t y = y1; y <= y2; y++) {
      Draw(x1, y, p);
    }
  }
  else {
    int32_t m = divCeil(rise,run);
    int32_t adjust = m >= 0 ? 1 : -1;
    int32_t offset = 0;
    const int32_t abs_rise = std::abs(rise);
    const int32_t abs_run = std::abs(run);

    if (m >= -1 && m <= 1) { // run > rise, more horizontal, increment y
      const int32_t delta = abs_rise * 2;
      int32_t threshold = abs_run;
      const int32_t thresholdInc = abs_run * 2;
      int32_t y = y1;
      if (x2 < x1) {
        std::swap(x2, x1);
        y = y2;
      }
      for (int32_t x = x1; x <= x2; x++) {
        Draw(x, y, p);
        offset += delta;
        if (offset >= threshold) {
          y += adjust;
          threshold += thresholdInc;
        }
      }            
    }
    else { // rise > run, more vertical, increment x
      const int32_t delta = abs_run * 2;
      int32_t threshold = abs_rise;
      const int32_t thresholdInc = abs_rise * 2;
      int32_t x = x1;
      if (y2 < y1) {
        std::swap(y2, y1);
        x = x2;
      }
      for (int32_t y = y1; y <= y2; y++) {
        Draw(x, y, p);
        offset += delta;
        if (offset >= threshold) {
          x += adjust;
          threshold += thresholdInc;
        }
      }

    }
  }
}

//template<class T>
void NdGameEngine::DrawWireFrame(const std::vector<ndVector<float>>& model, const ndVector<float>& trlVec, float r, float s)
{

  std::vector<ndVector<float>> vectorTransformed;
  size_t verts = model.size();
  vectorTransformed.resize(verts);

  // Rotate
  // ToDo: implement in Matrix
  for (size_t i = 0; i < verts; i++)
  {
    vectorTransformed[i] = rotate(model[i], r);
  }

  // Scale
  for (auto& ndVec : vectorTransformed)
  {
    ndVec * s;
  }

  // Translate
  for (auto& ndVec : vectorTransformed)
  {
    ndVec + trlVec;
  }

  // Draw Closed Polygon
  for (int i = 0; i < verts + 1; i++)
  {
  int j = (i + 1);
  DrawLine(vectorTransformed[i % verts].GetAtIndex(0), vectorTransformed[i % verts].GetAtIndex(1),
    vectorTransformed[j % verts].GetAtIndex(0), vectorTransformed[j % verts].GetAtIndex(1), nd::WHITE);
  }

}

void NdGameEngine::ClearScreen(Pixel p)
{
  const int32_t numPixels = _screenWidth * _screenHeight;
  Pixel* const pixels = _drawTarget->GetDataPtr();

  for (int32_t i = 0; i < numPixels; i++)
  {
    pixels[i] = p;
  }
}

void NdGameEngine::EngineInit()
{

  _mediaLib.CreateWindow(_screenWidth, _screenHeight, _pixelWidth, _pixelHeight, _fullScreen);

  _tp1 = system_clock::now();
  _tp2 = system_clock::now();
}

void NdGameEngine::EngineCoreUpdate()
{
  // Timing management
  _tp2 = system_clock::now();
  duration<float> elapsedTime = _tp2 - _tp1;
  _tp1 = _tp2;
  // update ticks
  _elapsedTicks = elapsedTime.count();

  // Poll for hardware events
  _mediaLib.PollEvent();

  // Client update
  onClientUpdate(_elapsedTicks);

}

void NdGameEngine::GameEngineThread()
{
  // Initialize Game Engine
  EngineInit();
    
  // Client Resource creation
  onClientCreate();


  while (g_isEngineRunning)
  {

    EngineCoreUpdate();

    _mediaLib.Draw(_screenWidth, _screenHeight, _drawTarget->GetDataPtr());

  }
}

