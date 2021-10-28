#include <iostream>

#include "nd/ndGameEngine.h"

using std::vector;


void KeyUPPress()
{
  std::cout << "Key Up Pressed\n";
}

void KeyDownPress()
{
  std::cout << "Key Down Pressed\n";
}

class AsteroidGame : public nd::NdGameEngine
{
public:
    AsteroidGame() = default;

protected:

    void onClientCreate() override
    {
       //_vecModelShip = { { 0.0f, -5.0f}, {-2.5f, +2.5f}, {+2.5f, +2.5f} };
        _vecModelShip = { { 0.0f, -50.0f}, {-25.0f, +25.0f}, {+25.0f, +25.0f} };

        ConnectKeyPressCb(nd::Key::UP, std::bind(KeyUPPress));
        ConnectKeyPressCb(nd::Key::DOWN, std::bind(KeyDownPress));
    }

    void onClientUpdate(std::int32_t) override
    {
        ClearScreen(nd::BLANK);

        // Just draw some lines to test
        /*DrawLine(1, 2, 5, 8, nd::RED);
        DrawLine(4, 2, 4, 10, nd::GREEN);
        DrawLine(2, 4, 8, 4, nd::YELLOW);*/

        DrawLine(10, 20, 50, 80, nd::RED);
        DrawLine(40, 20, 40, 100, nd::GREEN);
        DrawLine(20, 40, 80, 40, nd::YELLOW);

        //nd::ndVector<float> translate{ 10.0f, 10.0f };
        nd::ndVector<float> translate{ 100.0f, 100.0f };


        DrawWireFrame(_vecModelShip, translate, 0.7854);    // rotate 45 deg or radian 0.7854

    }

private:

    vector<nd::ndVector<float>> _vecModelShip;

};

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