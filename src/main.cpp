#include <iostream>

#include "mediaLibrary.h"
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
        game.Draw(600, 600);
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

    //nd::Sprite gameCharacter;
    //int32_t x = 0, y = 0;

    //MediaLibrary<>().Draw(x, y, gameCharacter.GetDataPtr());


    return 0;
}