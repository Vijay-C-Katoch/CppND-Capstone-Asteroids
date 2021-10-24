#include <iostream>

#include "ndGameEngine.h"

class AsteroidGame : public nd::NdGameEngine
{
public:
    AsteroidGame() = default;
};

int main() {

    std::cout << "Hello World!" << "\n";    

    try
    {
        AsteroidGame game;
        game.ConstructGame(300, 300);
        game.DrawLine(10, 20, 50, 80, nd::RED);
        game.DrawLine(40, 20, 40, 100, nd::GREEN);
        game.DrawLine(20, 40, 80, 40, nd::YELLOW);

        std::vector<nd::ndVector<float>> vecModelShip = {{ 0.0f, -50.0f}, {-25.0f, +25.0f}, {+25.0f, +25.0f} };
        nd::ndVector<float> translate{100.0f, 100.0f};

       //game.DrawWireFrame<float>(vecModelShip, translate);
        game.DrawWireFrame(vecModelShip, translate, 1.0f, 0.0f);

        game.TestDrawFinal();
    }
    catch (const nd::GameEngineException& gameEngineException)
    {
        std::cout << "Caught Exception: {" << gameEngineException.what() << "}\n";
    }
    catch (const nd::SpriteException& spriteException)
    {
        std::cout << "Caught Exception: {" << spriteException.what() << "}\n";
    }
    catch (const nd::PixelException& pixelException)
    {
        std::cout << "Caught Exception: {" << pixelException.what() << "}\n";
    }


    return 0;
}