#include <Simulation.hpp>
#include <iostream>

Simulation::Simulation()
    : window(sf::VideoMode(1870,970), "Ant Colony Simulation") {
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
    velText.setFont(font);
    velText.setCharacterSize(24);
    velText.setFillColor(sf::Color::White);
    accelText.setFont(font);
    accelText.setCharacterSize(24);
    accelText.setFillColor(sf::Color::White);
    timeText.setPosition(20.f,20.f);
    int numAnts = 5;

    for (int i = 0; i < numAnts; ++i) {
        float x = static_cast<float>(rand() % numAnts*10);
        float y = static_cast<float>(rand() % numAnts*10);
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

        if (!paused || stepOnce) {
            timeCounter(dt);
            update(dt);
            stepOnce = false;
        }
        drag();
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
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                if (!paused) {
                    paused = true;
                }
                else {
                    paused = false;
                }
            }
            if (event.key.code == sf::Keyboard::Right) {
                stepOnce = true;
            }
            if (event.key.code == sf::Keyboard::R) {
                reset();
            }

            if (event.key.code == sf::Keyboard::Up) {
                if (!ants.empty()) {
                    selectedAntIndex = (selectedAntIndex + 1) % ants.size();
                }
            }
            if (event.key.code == sf::Keyboard::Down) {
                if (!ants.empty()) {
                    selectedAntIndex = (selectedAntIndex - 1 + ants.size()) % ants.size();
                }
            }
        }
    }
}

void Simulation::update(float dt) {
    for (auto& ant : ants) {
        ant.update(dt, window);
    }
}

void Simulation::render() {
    window.setView(view);
    window.clear();
    for (auto& ant : ants) {
        ant.draw(window);
    }

    if (selectedAntIndex >= 0 && selectedAntIndex < ants.size()) { // What the fuck is this ??? change this lmao?
        const Ant& selected = ants[selectedAntIndex];
        sf::Vector2f pos = selected.getPosition();
        sf::Vector2f vel = selected.getVelocity();
        velText.setPosition(20.f, 40.f);
        std::string velstring = (std::to_string(vel.x) + "," + std::to_string(vel.y));
        velText.setString("Velocity: " + velstring);
        sf::VertexArray velocityLine(sf::Lines, 2);
        velocityLine[0].position = pos;
        velocityLine[0].color = sf::Color::Cyan;
        velocityLine[1].position = pos + vel;
        velocityLine[1].color = sf::Color::Cyan;

        sf::Vector2f acc = selected.getAcceleration();
        accelText.setPosition(20.f, 60.f);
        std::string accelstring = (std::to_string(acc.x) + "," + std::to_string(acc.y));
        accelText.setString("Acceleration: " + accelstring);
        sf::VertexArray accelLine(sf::Lines, 2);
        accelLine[0].position = pos;
        accelLine[0].color = sf::Color::Red;
        accelLine[1].position = pos + acc;
        accelLine[1].color = sf::Color::Red;

        window.draw(velocityLine);
        window.draw(accelLine);
    }
    
    window.setView(window.getDefaultView());
    window.draw(timeText);
    window.draw(velText);
    window.draw(accelText);
    window.display();
}

void Simulation::timeCounter(float dt) {
    simulationTime += dt;
    int minutes = static_cast<int>(simulationTime) / 60;
    int seconds = static_cast<int>(simulationTime) % 60;
    int miliseconds = static_cast<int>((simulationTime - static_cast<int>(simulationTime)) * 1000);

    std::string timeString = 
    (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
    (seconds < 10 ? "0" : "") + std::to_string(seconds) + ":" +
    (miliseconds < 100 ? (miliseconds < 10 ? "00" : "0") : "") + std::to_string(miliseconds);

    timeText.setString("Time: " + timeString);
}

void Simulation::reset() {
    ants.clear();
    simulationTime = 0.f;
    int numAnts = 5;
    for (int i = 0 ; i < numAnts ; i++) {
        float x = static_cast<float>(rand() % numAnts*10);
        float y = static_cast<float>(rand() % numAnts*10);
        ants.emplace_back(antTexture, sf::Vector2f(x,y));
    }
}

void Simulation::drag() {
    if (isDragging) {
        sf::Vector2f current = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2f offset = dragStart - current;
        view.move(offset);
        dragStart = current;
    }
}