#include <Simulation.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

int main(int argc, char* argv[])
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    int numAnts = 25000;

    if (argc > 1) {
        numAnts = std::atoi(argv[1]);

        if (numAnts < 0) {
            std::cout << "Invalid number of ants! Inicializing with default number 25k ants." << std::endl;
            numAnts = 25000;
        }
    }
    std::cout << "Iniciating the simulation with " << numAnts << " Ants!" << std::endl;

    Simulation simulation(numAnts);
    simulation.run();
    return 0;
}