#include <Simulation.hpp>
#include <cstdlib>
#include <ctime>

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Simulation simulation;
    simulation.run();
    return 0;
}