#include <Simulation.hpp>
#include <iostream>

Simulation::Simulation()
    : window(sf::VideoMode(1000, 800), "Ant Colony Simulation", sf::Style::Fullscreen) {
        window.setFramerateLimit(144);

        if (!antTexture.loadFromFile("res/ant.png")) {
            std::cerr << "Failed to load ant texture!\n";
        }
    int numAnts = 20; // escolhe o número que quiseres

    for (int i = 0; i < numAnts; ++i) {
        float x = static_cast<float>(rand() % 1920);
        float y = static_cast<float>(rand() % 1080);
        ants.emplace_back(antTexture, sf::Vector2f(x, y));
}
}

Simulation::~Simulation() {
    // Do something here
}

void Simulation::run() {
    while (window.isOpen()) {
        processEvents();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}

void Simulation::processEvents() {
    sf::Event event;
    while(window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Simulation::update(float dt) {
    for (auto& ant : ants) {
        ant.update(dt, window);
    }
}

void Simulation::render() {
    window.clear();
    for (auto& ant : ants) {
        ant.draw(window);
    }
    window.display();
}