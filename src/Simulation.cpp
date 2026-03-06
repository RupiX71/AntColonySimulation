#include <Simulation.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

Simulation::Simulation()
    : window(sf::VideoMode(1920,1080), "Ant Colony Simulation") 
    , view(window.getDefaultView()) 
    , world(1000, 1000)
{

    // Loading Files
    if (!antTexture.loadFromFile("res/ant.png")) { std::cerr << "Failed to load ant texture!\n"; }
    if (!font.loadFromFile("res/Cascadia.ttf")) { std::cerr << "Failed to load font file!\n"; }

    auto setupText = [&](sf::Text& text, float yPos) {
        text.setFont(font);
        text.setCharacterSize(30.f);
        text.setFillColor(sf::Color::White);
        text.setPosition(20.f, yPos);
    };

    setupText(timeText, 20.f);
    setupText(velText, 50.f);
    setupText(accelText, 80.f);

    // Initial Reset
    reset();
}

Simulation::~Simulation() {
}

void Simulation::run() {
    while (window.isOpen()) {
        processEvents();
        float dt = clock.restart().asSeconds();
        if (dt > 0.25f) { dt = 0.25f; }
        
        if (!paused || stepOnce) {
            accumulator += dt;
        }

        if (!paused || stepOnce) {
            while (accumulator >= TIME_STEP) {
                update(TIME_STEP);
                timeCounter(TIME_STEP);

                accumulator -= TIME_STEP;
            }
            stepOnce = false;
        }
        drag();
        render();
    }
}

void Simulation::processEvents() {
    sf::Event event;
    while(window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
            {
                window.close();
            }
            break;
            
            case sf::Event::Resized:   // guess what! im doing it right now
            {
                sf::Vector2f newSize(static_cast<float>(event.size.width), 
                                     static_cast<float>(event.size.height));
                newSize *= currentZoom;  // Adjust for current zoom level
                sf::Vector2f currentCenter = view.getCenter();
                view.setSize(newSize);
                view.setCenter(currentCenter); // Keep the center consistent
            }
            break;

            case sf::Event::MouseWheelScrolled:
            {
                float zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.95f : 1.05f;
                float newZoom = currentZoom * zoomFactor;

                if (newZoom >= 0.05f && newZoom <= 1.05f) {
                    currentZoom = newZoom;
                    view.zoom(zoomFactor);
                }
            }
            break;
            
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isDragging = true;

                    selectedAntIndex = -1; // Deselect any selected ant when starting to drag
                    dragStart = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
                }
            }
            break;
            
            case sf::Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isDragging = false;
                }
            }
            break;

            case sf::Event::KeyPressed:
            {
                handleInput(event.key.code);
            }
            break;

            default: break;
        }
    }
}

void Simulation::handleInput(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Space) {
        paused = !paused;

        if (paused) { timeText.setString(timeText.getString() + " [PAUSED] "); }
    }

    if (key == sf::Keyboard::Right) { stepOnce = true; }
    if (key == sf::Keyboard::R) { reset(); }

    if (!ants.empty()) {
        if (key == sf::Keyboard::Up) {
            selectedAntIndex = (selectedAntIndex + 1) % ants.size();
        }
        else if (key == sf::Keyboard::Down) {
            selectedAntIndex = (selectedAntIndex - 1 + ants.size()) % ants.size();
        }
    }
}

void Simulation::update(float dt) {
    world.update(dt);
    for (auto& ant : ants) {
        ant.update(dt);
    }

    if (selectedAntIndex != -1 && selectedAntIndex < ants.size()) {
        sf::Vector2f antPos = ants[selectedAntIndex].getPosition();
        view.setCenter(antPos);
    }
}

void Simulation::render() { // TO DO: Change the color of the ants from white to black 
    window.clear(sf::Color(121, 71, 41)); // Bright Brown Background
    window.setView(view);

    world.draw(window);
    for (auto& ant : ants) {
        ant.draw(window);
    }
    
    renderSelectedAntDebug();
    

    window.setView(window.getDefaultView());
    window.draw(timeText);

    if (selectedAntIndex != -1) {
        window.draw(velText);
        window.draw(accelText);
    }

    window.display();
}

void Simulation::renderSelectedAntDebug() {
    if (selectedAntIndex < 0 || selectedAntIndex >= ants.size()) return;

    const Ant& selected = ants[selectedAntIndex];
    sf::Vector2f pos = selected.getPosition();
    sf::Vector2f vel = selected.getVelocity();
    sf::Vector2f acc = selected.getAcceleration();

    std::ostringstream ssVel, ssAcc;
    ssVel << std::fixed << std::setprecision(2) << "Vel: (" << vel.x << ", " << vel.y << ")";
    ssAcc << std::fixed << std::setprecision(2) << "Acc: (" << acc.x << ", " << acc.y << ")";

    velText.setString(ssVel.str());
    accelText.setString(ssAcc.str());

    auto drawVectors = [&](sf::Vector2f vector, sf::Color color) {
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = pos;
        line[0].color = color;
        line[1].position = pos + (vector) * 0.5f;
        line[1].color = color;
        window.draw(line);
    };

    drawVectors(vel, sf::Color::Cyan);
    drawVectors(acc, sf::Color::Red);
}

void Simulation::timeCounter(float dt) {
    simulationTime += dt;

    int totalMs = static_cast<int>(simulationTime * 1000);
    int min = (totalMs / 60000);
    int sec = (totalMs / 1000) % 60;
    int ms = totalMs % 1000;

    char buffer[32];

    snprintf(buffer, sizeof(buffer), "Time: %02d:%02d:%03d", min, sec, ms);
    timeText.setString(buffer);
}

void Simulation::reset() {
    ants.clear();
    ants.reserve(500);

    simulationTime = 0.f;
    timeText.setString("Time: 00:00:000");
    selectedAntIndex = -1;

    int numAnts = 250;
    sf::Vector2u winSize = window.getSize();

    for (int i = 0; i < numAnts; ++i) {
        float x = static_cast<float>(rand() % winSize.x*10);
        float y = static_cast<float>(rand() % winSize.y*10);
        ants.emplace_back(antTexture, sf::Vector2f(x, y));
    }
}

void Simulation::drag() {
    if (isDragging) {
        sf::Vector2f current = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
        sf::Vector2f offset = (dragStart - current); // Multiply by zoom factor
        view.move(offset);

        // Update dragStart to the new position for smooth dragging
        // Note: AS the view just moved, now the mouse points to a new place
        // The ideal is to recalculate the dragStart after moving the view so the draggin is smooth
        dragStart = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
    }
}