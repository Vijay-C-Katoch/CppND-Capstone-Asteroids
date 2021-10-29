#ifndef ASTEROIDS_H_
#define ASTEROIDS_H_

#include "ndGameEngine.h"

class AsteroidGame : public nd::NdGameEngine
{
public:
  AsteroidGame() = default;

protected:

  void onClientCreate() override
  {
    // space-ship is a triangle shape made up of vectors
    _vecSpaceShip = { { 0.0f, -50.0f}, {-25.0f, +25.0f}, {+25.0f, +25.0f} };
    // asteroids
    _vecAsteroids.push_back({200.0f, 200.0f, 80.0f, -60.0f, (int)160 });



    /*ConnectKeyPressCb(nd::Key::UP, std::bind(KeyUPPress));
    ConnectKeyPressCb(nd::Key::DOWN, std::bind(KeyDownPress));
    ConnectKeyPressCb(nd::Key::RIGHT, std::bind(KeyRightPress));
    ConnectKeyPressCb(nd::Key::LEFT, std::bind(KeyLeftPress));*/
  }

  void onClientUpdate(float elapsedTicks) override
  {
    ClearScreen(nd::BLANK);

    // A translation vector
    nd::ndVector<float> translateVec{ 100.0f, 100.0f };

    //update and draw asteroids
    for (auto& a : _vecAsteroids)
    {
      // newPosition = velosity*time + old position;
      a.x += a.dx * elapsedTicks;  
      a.y += a.dy * elapsedTicks;  

      //a.x = WrapX(a.x);
      //a.y = WrapY(a.y);

      // Draw as rectangle
      for (int x = 0; x < a.size; x++)
        for (int y = 0; y < a.size; y++)
        {
          //Draw(a.x + x, a.y + y, nd::RED);
          Draw(WrapX(a.x + x), WrapY(a.y + y), nd::RED);
        }                    
    }

    DrawWireFrame(_vecSpaceShip, translateVec, 0.7854);    // rotate 45 deg or radian 0.7854
  }

private:

  struct spaceObject
  {
    float x;    // location x component
    float y;    // location y component
    float dx;   // veloctity x component
    float dy;   // veloctity y component
    int size;   // object size
  };

  // space-ship is made up of 2D vectors
  std::vector<nd::ndVector<float>> _vecSpaceShip;
  // asteroids are made up of space objects
  std::vector<spaceObject> _vecAsteroids;

};


#endif // !ASTEROIDS_H_
