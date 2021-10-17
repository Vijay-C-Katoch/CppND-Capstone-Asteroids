#include <iostream>

#include "mediaLibrary.h"
#include "ndGameEngine.h"

int main() {

    std::cout << "Hello World!" << "\n";

    nd::Sprite gameCharacter;
    int32_t x = 0, y = 0;

    MediaLibrary<>().Draw(x, y, gameCharacter.GetDataPtr());


    return 0;
}