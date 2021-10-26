#include <iostream>

#include "ndGameEngine.h"

using std::vector;

class AsteroidGame : public nd::NdGameEngine
{
public:
    AsteroidGame() = default;

protected:

    void onClientCreate() override
    {
       _vecModelShip = { { 0.0f, -50.0f}, {-25.0f, +25.0f}, {+25.0f, +25.0f} };

    }

    void onClientUpdate(std::int32_t) override
    {
        ClearScreen(nd::BLANK);

        // Just draw some lines to test
        DrawLine(10, 20, 50, 80, nd::RED);
        DrawLine(40, 20, 40, 100, nd::GREEN);
        DrawLine(20, 40, 80, 40, nd::YELLOW);

        nd::ndVector<float> translate{ 100.0f, 100.0f };
        DrawWireFrame(_vecModelShip, translate, 1.0, 0.0);    

    }

private:

    vector<nd::ndVector<float>> _vecModelShip;

};

int main() {

    std::cout << "Hello World!" << "\n";    

    try
    {
        AsteroidGame game;
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