#include <Simulation.hpp>
#include <iostream>

Simulation::Simulation()
    : window(sf::VideoMode(1000, 800), "Ant Colony Simulation") {
        window.setFramerateLimit(144);

    if (!antTexture.loadFromFile("res/ant.png")) {
        std::cerr << "Failed to load ant texture!\n";
    }

    if (!font.loadFromFile("res/Cascadia.ttf")) {
        std::cerr << "Failed to load font file!\n";
    }

    timeText.setFont(font);
    timeText.setCharacterSize(24);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(20.f,20.f);
    int numAnts = 500; // escolhe o número que quiseres

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
        timeCounter(dt);
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
    window.draw(timeText);

    window.display();
}

void Simulation::timeCounter(float dt) {
    simulationTime += dt;
    int minutes = static_cast<int>(simulationTime) / 60;
    int seconds = static_cast<int>(simulationTime) % 60;

    std::string timeString = 
    (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
    (seconds < 10 ? "0" : "") + std::to_string(seconds);

    timeText.setString("Time: " + timeString);
}