#include <Simulation.hpp>
#include <iostream>

Simulation::Simulation()
    : window(sf::VideoMode(1900,1000), "Ant Colony Simulation") {
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
    int numAnts = 25000; // escolhe o número que quiseres

    for (int i = 0; i < numAnts; ++i) {
        float x = static_cast<float>(rand() % 10000);
        float y = static_cast<float>(rand() % 10000);
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
        if (event.type == sf::Event::MouseWheelScrolled) {
            view.zoom(event.mouseWheelScroll.delta > 0 ? 0.9f : 1.1f);
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            isDragging = true;
            dragStart = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            isDragging = false;
            sf::Vector2f current = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Vector2f offset = dragStart - current;
            view.move(offset);
            dragStart = current;
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
    window.setView(view);
    for (auto& ant : ants) {
        ant.draw(window);
    }
    window.setView(window.getDefaultView());
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