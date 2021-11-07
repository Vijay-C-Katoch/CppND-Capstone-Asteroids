#ifndef SNAKE_H_
#define SNAKE_H_

#include <cstdlib>
#include <random>
#include "ndGameEngine.h"
#include "ndUtils.h"

class SnakeGame : public nd::NdGameEngine
{
public:
  SnakeGame(int32_t grid_width, int32_t grid_height) :
    _grid_width(grid_width),
    _grid_height(grid_height),
    _randomEngine(_dev()),
    _random_w(0, static_cast<int>(grid_width - 1)),
    _random_h(0, static_cast<int>(grid_height - 1))
  {
    PlaceFood();
  }

  void Draw(std::int32_t x, std::int32_t y, nd::Pixel p = nd::WHITE) override
  {
    x = WrapX(x);
    y = WrapY(y);
    nd::NdGameEngine::Draw(x, y, p);
  }

  ~SnakeGame() = default;


protected:

  void onClientCreate() override
  {
    /*************************************
    * 1. Create Game
    *************************************/
    
    /*************************************
    * 3. Hardware connection
    *************************************/
    // Connect keyboard keys to functions
    ConnectKeyPressCb(nd::Key::UP, [&]() { ChangeDirection(SnakeDirection::kUp, SnakeDirection::kDown); });
    ConnectKeyPressCb(nd::Key::DOWN, [&]() { ChangeDirection(SnakeDirection::kDown, SnakeDirection::kUp); });
    ConnectKeyPressCb(nd::Key::LEFT, [&]() { ChangeDirection(SnakeDirection::kLeft, SnakeDirection::kRight); });
    ConnectKeyPressCb(nd::Key::RIGHT, [&]() { ChangeDirection(SnakeDirection::kRight, SnakeDirection::kLeft); });

  }

  void onClientUpdate(float elapsedTicks) override
  {

    ClearScreen(nd::BLACK);

    if (!_alive) return;

    _prev_cell.x = _head_x; // We first capture the head's cell before updating.
    _prev_cell.y = _head_y;

    UpdateHead();

    _current_cell.x = _head_x; // Capture the head's cell after updating.
    _current_cell.y = _head_y;

    // Update all of the body vector items if the snake head has moved to a new
    // cell.
    if (_current_cell.x != _prev_cell.x || _current_cell.y != _prev_cell.y) {
      UpdateBody();
    }

    // Check for food in front of Snake's head
    if (static_cast<int>(_food.x) == static_cast<int>(_head_x) && 
      static_cast<int>(_food.y) == static_cast<int>(_head_y)
       ) {
      _score++;
      PlaceFood();
      // Grow snake and increase speed.
      GrowBody();
      _speed += 0.0005;
    }

    RenderGame();

    // Update score
    DrawString(2, 2, _scoreText + std::to_string(_score), nd::YELLOW, 2);

  }


private:

  struct Block
  {
    int32_t x, y;
    int32_t w, h;
  };

  struct GameCell
  {
    float x;      // location x component
    float y;      // location y component
  };

  enum class SnakeDirection { kUp, kDown, kLeft, kRight };

  void ChangeDirection(SnakeDirection input, SnakeDirection opposite)
  {
    if (_direction != opposite || size == 1)
      _direction = input;

    return;
  }

  void UpdateHead() {
    switch (_direction) {
    case SnakeDirection::kUp:
      _head_y -= _speed;
      break;

    case SnakeDirection::kDown:
      _head_y += _speed;
      break;

    case SnakeDirection::kLeft:
      _head_x -= _speed;
      break;

    case SnakeDirection::kRight:
      _head_x += _speed;
      break;
    }

    // Wrap the Snake around to the beginning if going off of the screen.
    _head_x = WrapX(_head_x, _grid_width);
    _head_y = WrapY(_head_y, _grid_height);
  }

  void UpdateBody()
  {
    // Add previous checked and verified cell location to vector as body
    body.push_back(_prev_cell);

    if (!_growing) {
      // Remove the tail from the vector.
      body.erase(body.begin());
    }
    else {
      _growing = false;
      size++;
    }

    // Check if the snake has died.
    for (auto const& item : body) {
      if (_current_cell.x == item.x && _current_cell.y == item.y) {
        _alive = false;
      }
    }
  }

  void GrowBody() { _growing = true; }

  // method to check if cell is occupied by snake.
  bool SnakeCell(int x, int y)
  {
    if (x == static_cast<int>(_head_x) && y == static_cast<int>(_head_y)) {
      return true;
    }
    for (auto const& item : body) {
      if (x == static_cast<int>(item.x) && y == static_cast<int>(item.y)) {
        return true;
      }
    }
    return false;
  }

  void PlaceFood()
  {
    int x, y;
    while (true) {
      x = _random_w(_randomEngine);
      y = _random_h(_randomEngine);
      // Check that the location is not occupied by a snake item before placing
      // food.
      if (!SnakeCell(x, y)) {
        _food.x = x;
        _food.y = y;
        return;
      }
    }
  }

  void RenderGame()
  {
    ///// Render on screen
    Block block;
    block.w = ScreenWidth() / _grid_width;
    block.h = ScreenHeight() / _grid_height;

    // Render food
    block.x = _food.x * block.w;
    block.y = _food.y * block.h;
    DrawRectangle(block.x, block.y, block.w, block.h, nd::RED);

    // Render Snake body
    for (GameCell const& point : body) {
      block.x = point.x * block.w;
      block.y = point.y * block.h;
      DrawRectangle(block.x, block.y, block.w, block.h);
    }

    // Render snake's head
    block.x = static_cast<int32_t>(_head_x) * block.w;
    block.y = static_cast<int32_t>(_head_y) * block.h;
    DrawRectangle(block.x, block.y, block.w, block.h);
  }

  int32_t _grid_width;
  int32_t _grid_height;
  float _speed{ 0.02f };
  int size{ 1 };
  bool _alive{ true };
  float _head_x;
  float _head_y;
  bool _growing{ false };
  int _score{ 0 };
  std::string _scoreText = "SCORE : ";

  SnakeDirection _direction = SnakeDirection::kUp;

  std::vector<GameCell> body;
  GameCell _prev_cell;
  GameCell _current_cell;
  GameCell _food;

  std::random_device _dev;
  std::mt19937 _randomEngine;
  std::uniform_int_distribution<int> _random_w;
  std::uniform_int_distribution<int> _random_h;  

};

#endif // !SNAKE_H_
