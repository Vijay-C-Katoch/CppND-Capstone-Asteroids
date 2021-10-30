#ifndef ASTEROIDS_H_
#define ASTEROIDS_H_

#include "ndGameEngine.h"

#include <iostream>

class AsteroidGame : public nd::NdGameEngine
{
public:
  AsteroidGame() = default;

  void Draw(std::int32_t x, std::int32_t y, nd::Pixel p) override
  {
    x = WrapX(x);
    y = WrapY(y);
    nd::NdGameEngine::Draw(x, y, p);
  }

protected:

  void onClientCreate() override
  {
    // space-ship is a triangle shape made up of vectors
    _vecSpaceShip = { { 0.0f, -50.0f}, {-25.0f, +25.0f}, {+25.0f, +25.0f} };
    // asteroids
    _vecAsteroids.push_back({200.0f, 200.0f, 80.0f, -60.0f, (int)160, 0.0f });
    // player control of spaceShip. Initialize for spaceship to be at centre
    _playerCtrl.x = ScreenWidth() / 2.0f;
    _playerCtrl.y = ScreenHeight() / 2.0f;
    _playerCtrl.dx = 0.0f;
    _playerCtrl.dy = 0.0f;
    _playerCtrl.angle = 0.0f;

    _translateVec = { {100.0f, 100.0f} };


    ConnectKeyPressCb(nd::Key::UP, std::bind(&AsteroidGame::OnUpKeyPress, this));
    ConnectKeyPressCb(nd::Key::DOWN, std::bind(&AsteroidGame::OnDownKeyPress, this));
    // steer the space ship
    ConnectKeyPressCb(nd::Key::LEFT, [&]() { _playerCtrl.angle -= 5.0f * ElapsedTicks();});
    ConnectKeyPressCb(nd::Key::RIGHT, [&](){ _playerCtrl.angle += 5.0f * ElapsedTicks();});
}

  void onClientUpdate(float elapsedTicks) override
  {
    ClearScreen(nd::BLACK);
    
    //update and draw asteroids
    for (auto& a : _vecAsteroids)
    {
      // Formula: newPosition = velosity*time + old position;
      a.x += a.dx * elapsedTicks;  
      a.y += a.dy * elapsedTicks;  

      // Draw as rectangle
      for (int x = 0; x < a.size; x++)
        for (int y = 0; y < a.size; y++)
        {
          Draw(a.x + x, a.y + y, nd::RED);
        }                    
    }

    // Player control change ship position wrt velocity
    // Formula: newPosition = velosity*time + old position;
    _playerCtrl.x += _playerCtrl.dx * elapsedTicks * 50.0f;
    _playerCtrl.y += _playerCtrl.dy * elapsedTicks * 50.0f;

    _translateVec = { {_playerCtrl.x , _playerCtrl.y} };
   

    DrawWireFrame(_vecSpaceShip, _translateVec, _playerCtrl.angle);    // rotate 45 deg or radian 0.7854
  }

public:   //Hardware connect
  void OnUpKeyPress()
  {
    // Thurst i.e Accelerate the ship. Acceleration is proportianal to angle
    // Formula: newVelocity = Acceleration*time + old velocity;
    _playerCtrl.dx += sin(_playerCtrl.angle) * ElapsedTicks() * 20.0f;
    _playerCtrl.dy += -cos(_playerCtrl.angle) * ElapsedTicks() * 20.0f;
  }

  void OnDownKeyPress()
  {
    // Reduce thurst. opposite of UP
    _playerCtrl.dx -= sin(_playerCtrl.angle) * ElapsedTicks() * 20.0f;
    _playerCtrl.dy -= -cos(_playerCtrl.angle) * ElapsedTicks() * 20.0f;
  }


private:

  struct spaceObject
  {
    float x;      // location x component
    float y;      // location y component
    float dx;     // veloctity x component
    float dy;     // veloctity y component
    int size;     // object size
    float angle;  // angle in radians
  };

  // space-ship is made up of 2D vectors
  std::vector<nd::ndVector<float>> _vecSpaceShip;
  // asteroids are made up of space objects
  std::vector<spaceObject> _vecAsteroids;
  // player control of spaceShip
  spaceObject _playerCtrl;
  // A translation vector
  nd::ndVector<float> _translateVec;

};


#endif // !ASTEROIDS_H_
