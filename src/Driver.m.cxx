#include "GameContext.h"
#include "GameWorld.h"
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>


int main(int argc, char ** argv)
{
    GameContext context{};
    try {
        // TODO

    } catch (std::exception& ex) {
        std::cerr << "Could not initialize context: \n" << ex.what() << '\n';
        return 1;
    } 

    GameSettings settings{};
    try {
        settings = context.generateSettings();

    } catch (std::exception& ex) {
        std::cerr << "Could not generate settings: \n" << ex.what() << '\n';
        return 1;
    }

    sf::Clock timer{};
    try {
        GameWorld world{context, settings};
        world.run(); 

    } catch (std::exception& ex) {
        std::cerr << "Unhandled exception!!\n" << ex.what() << '\n';
        return 1;
    }

    std::cout << timer.getElapsedTime().asMilliseconds() << "ms\n";
    return 0;
}

