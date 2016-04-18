#include "GameContext.h"
#include "GameWorld.h"
#include <exception>
#include <iostream>
#include <string>
#include <vector>


int main(int argc, char ** argv)
{
    // Try to initialize our context with files/input
    GameContext context{};
    try { /* ... */ } catch (std::exception& ex) {
        std::cerr << "Could not initialize context: \n" << ex.what() << '\n';
        return 1;
    } 

    // Try to generate the context through our initialization
    GameSettings settings{};
    try { settings = context.generateSettings(); } catch (std::domain_error& ex) {
        std::cerr << "Could not generate context: \n" << ex.what() << '\n';
        return 1;
    }

    // Now allow our GameWorld to run until it either ends naturally or crashes
    // through some exception
    try {
        GameWorld world{context, settings}; world.run(); 
    } catch (std::exception& ex) {
        std::cerr << "Unhandled exception!!\n" << ex.what() << '\n';
    }

    return 0;
}

