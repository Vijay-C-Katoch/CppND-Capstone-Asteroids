
#include <cstddef>
#include <thread>
#include<cmath>

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

bool NdGameEngine::isInsideCircle(float cx, float cy, float radius, float x, float y) const
{
  return std::sqrtf( std::powf((x - cx),2.0f) + std::powf((y - cy),2.0f) ) < radius;
}

void NdGameEngine::OnQuitCb() const
{
  g_isEngineRunning = false;
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


void NdGameEngine::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p)
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

void NdGameEngine::DrawRectangle(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p)
{
  DrawLine(x, y, x + w, y, p);
  DrawLine(x + w, y, x + w, y + h, p);
  DrawLine(x + w, y + h, x, y + h, p);
  DrawLine(x, y + h, x, y, p);
}

void NdGameEngine::DrawWireFrame(const std::vector<ndVector<float>>& model, const ndVector<float>& trlVec, float r, float s, Pixel p)
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
    vectorTransformed[j % verts].GetAtIndex(0), vectorTransformed[j % verts].GetAtIndex(1), p);
  }

}

void NdGameEngine::DrawString(int32_t x, int32_t y, const std::string& text, Pixel p, uint32_t scale)
{
  int32_t sx = 0;
  int32_t sy = 0;

  for (auto c : text)
  {
    if (c == '\n')
    {
      sx = 0; sy += 8 * scale;
    }
    else
    {
      // Font sheet start at 32 (space). So ASCII - 32
      // All characters follows ASCII sequence
      // sprite sheet has 16 chars in a row
      int32_t ox = (c - 32) % 16;   
      int32_t oy = (c - 32) / 16;   

      if (scale > 1)
      {
        for (uint32_t i = 0; i < 8; i++)
          for (uint32_t j = 0; j < 8; j++)
            if (_fontTarget->GetPixel(i + ox * 8, j + oy * 8).r > 0)
              for (uint32_t is = 0; is < scale; is++)
                for (uint32_t js = 0; js < scale; js++)
                  Draw(x + sx + (i * scale) + is, y + sy + (j * scale) + js, p);
      }
      else
      {
        for (uint32_t i = 0; i < 8; i++)    // each char is 8 x 8 pixel. 
          for (uint32_t j = 0; j < 8; j++)  // ox, oy at offset of 16 chars in row
            if (_fontTarget->GetPixel(i + ox * 8, j + oy * 8).r > 0)
              Draw(x + sx + i, y + sy + j, p);
      }
      sx += 8 * scale;
    }
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

void NdGameEngine::FontInit()
{
  _fontTarget = std::make_unique<Sprite>(128, 48);

  // Below is NES font where each character is 8 x 8 pixels in size and bitmap is
  // of total 128 x 48 pisels in size. The sheet has 16 characters in a row and
  // total of 6 rows, hence 16 x 6 = 96 characters. 16 x 6 x 8 x 8 = 128 x 48.
  // Below font/sprite sheet data is encoded in readable and printable characters
  //  and stored in string data. The pixels are represented by 6 bits and packed
  // into char (a byte) to restrict to a 64 character range to remain within the 
  // printable part of the ascii character set. An offset of 48 with 6 bits of 
  // possible values gives a range from ‘0’ (ascii 48) upto and including 
  // ‘o’ (ascii 111).
  // OLC-Font: Utility to encode 128 x 48 font png to printable string 
  // https://github.com/gorbit99/OLC-Font

  std::string data;
  data += "?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000";
  data += "O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400";
  data += "Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000";
  data += "720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000";
  data += "OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000";
  data += "ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000";
  data += "Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000";
  data += "70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000";
  data += "OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000";
  data += "00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000";
  data += "<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000";
  data += "O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000";
  data += "00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000";
  data += "Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0";
  data += "O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000";
  data += "?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";

  int px = 0, py = 0;
  for (size_t b = 0; b < 1024; b += 4)
  {
    uint32_t sym1 = (uint32_t)data[b + 0] - 48;
    uint32_t sym2 = (uint32_t)data[b + 1] - 48;
    uint32_t sym3 = (uint32_t)data[b + 2] - 48;
    uint32_t sym4 = (uint32_t)data[b + 3] - 48;
    uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

    for (int i = 0; i < 24; i++)
    {
      int k = r & (1 << i) ? 255 : 0;
      _fontTarget->SetPixel(px, py, Pixel(k, k, k, k));
      if (++py == 48) { px++; py = 0; }
    }
  }

}

void NdGameEngine::EngineInit()
{

  _mediaLib.CreateWindow(_screenWidth, _screenHeight, _pixelWidth, _pixelHeight, _fullScreen);

  _mediaLib.ConnectQuitCb(std::bind(&NdGameEngine::OnQuitCb, this));

  FontInit();

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

