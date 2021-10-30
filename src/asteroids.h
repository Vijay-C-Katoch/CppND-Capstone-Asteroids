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
    // Model space-ship as Triangle made up of 2D vectors
    _vecModelShip = { { 0.0f, -50.0f}, {-25.0f, +25.0f}, {+25.0f, +25.0f} };
    
    // player control of spaceShip. Initialize for spaceship to be at centre
    _playerCtrl.x = ScreenWidth() / 2.0f;
    _playerCtrl.y = ScreenHeight() / 2.0f;
    _playerCtrl.dx = 0.0f;
    _playerCtrl.dy = 0.0f;
    _playerCtrl.angle = 0.0f;     
    _translateVecSpShip = { {1.0f, 1.0f} };


    // Model Asteroids as Polygon made up of 2D vectors
    int verts = 20;
    for (int i = 0; i < verts; i++)
    {
      float radius = 50.0f;
      float a = ((float)i / (float)verts) * 6.28318f;
      _vecModelAsteroids.push_back({ { radius * sinf(a), radius * cosf(a)} });
    }

    // asteroids control as space objects is  random
    _asteroidsCtrl.push_back({ 200.0f, 200.0f, 80.0f, -60.0f, (int)160, 0.0f });

    // Connect keyboard keys to functions
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
    for (auto& a : _asteroidsCtrl)
    {
      // Formula: newPosition = velosity*time + old position;
      a.x += a.dx * elapsedTicks;  
      a.y += a.dy * elapsedTicks;  

      _translateVecAsteroid = { {WrapX(a.x) , WrapY(a.y)} };

      DrawWireFrame(_vecModelAsteroids, _translateVecAsteroid, a.angle);                   
    }

    // Player control change ship position wrt velocity
    // Formula: newPosition = velosity*time + old position;
    _playerCtrl.x += _playerCtrl.dx * elapsedTicks * 50.0f;
    _playerCtrl.y += _playerCtrl.dy * elapsedTicks * 50.0f;

    _translateVecSpShip = { {WrapX(_playerCtrl.x) , WrapY(_playerCtrl.y)} };

    DrawWireFrame(_vecModelShip, _translateVecSpShip, _playerCtrl.angle);    // rotate 45 deg or radian 0.7854
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

  // Model space-ship as Triangle made up of 2D vectors
  std::vector<nd::ndVector<float>> _vecModelShip;
  // Model Asteroids as Polygon made up of 2D vectors
  std::vector<nd::ndVector<float>> _vecModelAsteroids;

  // player control of space-ship as space object
  spaceObject _playerCtrl;
  // asteroids control as space objects
  std::vector<spaceObject> _asteroidsCtrl;
  
  // Translation vectors 
  nd::ndVector<float> _translateVecSpShip;
  nd::ndVector<float> _translateVecAsteroid;

};


#endif // !ASTEROIDS_H_
