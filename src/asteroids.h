#ifndef ASTEROIDS_H_
#define ASTEROIDS_H_

#include <cstdlib>
#include "ndGameEngine.h"

class AsteroidGame : public nd::NdGameEngine
{
public:
  AsteroidGame() = default;

  void Draw(std::int32_t x, std::int32_t y, nd::Pixel p = nd::WHITE) override
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
    //_vecModelShip = { { 0.0f, -5.0f}, {-2.5f, +2.5f}, {+2.5f, +2.5f} };
    
    // player control of spaceShip. Initialize for spaceship to be at centre
    _playerCtrl.x = ScreenWidth() / 2.0f;
    _playerCtrl.y = ScreenHeight() / 2.0f;
    _playerCtrl.dx = 0.0f;
    _playerCtrl.dy = 0.0f;
    _playerCtrl.angle = 0.0f;     
    _translateVecSpShip = { {1.0f, 1.0f} };


    /* Model Asteroids as Polygon made up of 2D vectors.
    *  Number of (verts) small line segments at angle make closed polygon
    * */
    int verts = 50;
    for (int i = 0; i < verts; i++)
    {
      float radius = 1.0f;
      float a = ((float)i / (float)verts) * nd::defaults::radian2Pi;
      _vecModelAsteroids.push_back({ { radius * sinf(a), radius * cosf(a)} });
    }

    // asteroids control as space objects is  random
    _asteroidsCtrl.push_back({ 200.0f, 200.0f, 80.0f, -60.0f, (int)80, 0.0f });
    //_asteroidsCtrl.push_back({ 200.0f, 200.0f, 80.0f, -60.0f, (int)16, 0.0f });

    // Connect keyboard keys to functions
    ConnectKeyPressCb(nd::Key::UP, std::bind(&AsteroidGame::OnUpKeyPress, this));
    ConnectKeyPressCb(nd::Key::DOWN, std::bind(&AsteroidGame::OnDownKeyPress, this));
    // steer the space-ship
    ConnectKeyPressCb(nd::Key::LEFT, [&]() { _playerCtrl.angle -= 15.0f * ElapsedTicks();});
    ConnectKeyPressCb(nd::Key::RIGHT, [&](){ _playerCtrl.angle += 15.0f * ElapsedTicks();});
    // Fire bullet on space key release
    ConnectKeyReleaseCb(nd::Key::SPACE, [&](){
      _bulletsCtrl.push_back({ WrapX(_playerCtrl.x), WrapY(_playerCtrl.y), sinf(_playerCtrl.angle) * 50.0f, -cosf(_playerCtrl.angle) * 50.0f , 0, 0.0f });
    });
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
      a.x = WrapX(a.x);
      a.y = WrapY(a.y);

      _translateVecAsteroid = { {a.x , a.y} };

      DrawWireFrame(_vecModelAsteroids, _translateVecAsteroid, a.angle, a.size);
    }

    // Player control change ship position wrt velocity
    // Formula: newPosition = velosity*time + old position;
    _playerCtrl.x += _playerCtrl.dx * elapsedTicks * 50.0f;
    _playerCtrl.y += _playerCtrl.dy * elapsedTicks * 50.0f;
    _playerCtrl.x = WrapX(_playerCtrl.x);
    _playerCtrl.y = WrapY(_playerCtrl.y);

    _translateVecSpShip = { {_playerCtrl.x , _playerCtrl.y} };    

    // new disintegrated asteroids control as space objects
    std::vector<SpaceObject> _newAsteroidsCtrl;


    //update Bullets
    for (auto& b : _bulletsCtrl)
    {
      b.x += b.dx * elapsedTicks * 3.0f;
      b.y += b.dy * elapsedTicks * 3.0f;
      b.x = WrapX(b.x);
      b.y = WrapY(b.y);      

      // check bullet collision/hit with Asteroid
      for (auto& a : _asteroidsCtrl)
      {
        if (isInsideCircle(a.x, a.y, a.size, b.x, b.y))
        {
          // Asteroid is hit. Disintegrate if size is greater than minimum
          if (a.size > 10)
          {
            // Disintegrate to two, half size each, at same position, with random velocity
            // calculate angle, which corresponds to acceleration in velocity calculation
            float angle1 = ((float)std::rand() / (float)RAND_MAX )* nd::defaults::radianPi;
            float angle2 = ((float)std::rand() / (float)RAND_MAX) * nd::defaults::radianPi;
            
             // append to asteroid control, half size, at current position, with random acceleartion angle
            _newAsteroidsCtrl.push_back({a.x, a.y, 10.0f * sinf(angle1), 10.0f * cosf(angle1), a.size >>1, 0.0f});
            _newAsteroidsCtrl.push_back({ a.x, a.y, 10.0f * sinf(angle2), 10.0f * cosf(angle1), a.size >> 1, 0.0f });
          }

          // remove astroids bullets on hit . Forcefully making them  offscreen.
          a.x = -1000;
          b.x = -1000;
          
        }
      }
    }

    // control new disintegrated asteroids by pushing to main astroid control
    for (auto a : _newAsteroidsCtrl)
      _asteroidsCtrl.push_back(a);

    // Remove off screen asteroids hit by bullets and the bullets too
    removeOffscreenObjects(_asteroidsCtrl);
    removeOffscreenObjects(_bulletsCtrl);

    if (_asteroidsCtrl.empty()) // If no asteroids on screen, level completed
    {
      _asteroidsCtrl.clear();
      _bulletsCtrl.clear();

      // Add two new asteroids on screen, add them at 90 degree left and
      // right to space-ship current position      
      addNewAsteroidOnScreen(- nd::defaults::radianPiby2);
      addNewAsteroidOnScreen(+ nd::defaults::radianPiby2);

      // update score on level Clear
      //score += 1000; 
    }

    // Draw Bullets
    for (auto b : _bulletsCtrl)
      Draw(b.x, b.y);

    // Draw space ship
    DrawWireFrame(_vecModelShip, _translateVecSpShip, _playerCtrl.angle);
    

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
    // Thurst. opposite of UP
    _playerCtrl.dx -= sin(_playerCtrl.angle) * ElapsedTicks() * 20.0f;
    _playerCtrl.dy -= -cos(_playerCtrl.angle) * ElapsedTicks() * 20.0f;
  }

private:

  struct SpaceObject
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
  SpaceObject _playerCtrl;
  // asteroids control as space objects
  std::vector<SpaceObject> _asteroidsCtrl;
  // controlling bullets as space objects
  std::vector<SpaceObject> _bulletsCtrl;
  
  // Translation vectors 
  nd::ndVector<float> _translateVecSpShip;
  nd::ndVector<float> _translateVecAsteroid;



 private:

  // utility function
  void removeOffscreenObjects(std::vector<SpaceObject>& objectCtrl)
  {
    if (!objectCtrl.empty())
    {
      auto i = remove_if(objectCtrl.begin(), objectCtrl.end(), [&](SpaceObject so) {
        return (so.x < 1 || so.y < 1 || so.x >= ScreenWidth() - 1 || so.y >= ScreenHeight() - 1);
      });
      if (i != objectCtrl.end())
        objectCtrl.erase(i);
    }
  }

  void addNewAsteroidOnScreen(float userAngle)
  {
    // Add new asteroids on screen at an angle to the existing player space ship location
    // so as newly created astroid do not hit space ship. Create them at position offseted
    // scaling factor times the user angle
    constexpr float posScaling = 140.0f;

    _asteroidsCtrl.push_back({ posScaling * sinf(_playerCtrl.angle + userAngle) + _playerCtrl.x,
                        posScaling* cosf(_playerCtrl.angle + userAngle) + _playerCtrl.y,
                        80.0f * sinf(_playerCtrl.angle), -60.0f * cosf(_playerCtrl.angle), (int)80, 0.0f });
  }

};


#endif // !ASTEROIDS_H_
