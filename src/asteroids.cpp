#include <iostream>

#include "asteroids.h"



int main() {

    std::cout << "Hello World!" << "\n";    

    try
    {
        AsteroidGame game;
        
        //game.ConstructGame(160, 100, 8, 8);
        game.ConstructGame(640, 480, 1, 1);

        // main run thread
        game.Run();


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
    catch (const nd::VectorException<float>& vectorException)
    {
        std::cout << "Caught Exception: {" << vectorException.what() << "}\n";
    }
    catch (const MediaLibSDLException& mediaLibException)
    {
        std::cout << "Caught Exception: {" << mediaLibException.what() << "}\n";
    }

    return 0;
}