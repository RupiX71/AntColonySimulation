#include <Simulation.hpp>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

Simulation::Simulation()
    : window(sf::VideoMode(1920,1080), "Ant Colony Simulation") 
    , view(window.getDefaultView()) 
{

    // Loading Files
    if (!antTexture.loadFromFile("res/ant.png")) { std::cerr << "Failed to load ant texture!\n"; }
    if (!font.loadFromFile("res/Cascadia.ttf")) { std::cerr << "Failed to load font file!\n"; }

    auto setupText = [&](sf::Text& text, float yPos) {
        text.setFont(font);
        text.setCharacterSize(20.f);
        text.setFillColor(sf::Color::White);
        text.setPosition(20.f, yPos);
    };

    setupText(timeText, 20.f);
    setupText(posText, 40.f);
    setupText(velText, 60.f);
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
        
        if (!paused) {
            accumulator += dt;
        }

        if (!paused) {
            while (accumulator >= TIME_STEP) {
                update(TIME_STEP);
                timeCounter(TIME_STEP);

                accumulator -= TIME_STEP;
            }
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

                if (newZoom >= 0.05f && newZoom <= 5.0f) {
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

    if (key == sf::Keyboard::R) { reset(); }

    if (!ants.empty()) {
        if (key == sf::Keyboard::Up) {
            selectedAntIndex = (selectedAntIndex + 1) % ants.size();
        }
        else if (key == sf::Keyboard::Down) {
            selectedAntIndex = (selectedAntIndex - 1 + ants.size()) % ants.size();
        }
    }

    if (key == sf::Keyboard::F) {
        followMouse = !followMouse;

        std::cout << "Follow Mouse: " << (followMouse ? "On" : "Off") << std::endl;
    }
}

void Simulation::update(float dt) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
    # pragma omp parallel for            // this is a cool bit of parallelization
    for (auto& ant : ants) {
        ant.update(dt, mousePos, followMouse);
    }
    
    if (selectedAntIndex != -1 && selectedAntIndex < ants.size()) {
        sf::Vector2f antPos = ants[selectedAntIndex].getPosition();
        view.setCenter(antPos);
    }
}

void Simulation::render() { // TO DO: Change the color of the ants from white to black (this was not that great idk)
    window.clear(sf::Color(121, 71, 41)); // Bright Brown Background
    window.setView(view);

    for(size_t i = 0 ; i < ants.size() ; ++i) {
        ants[i].updateVertices(&antVerticies[i * 6]);
    }
    
    sf::RenderStates states;
    states.texture = &antTexture;

    window.draw(antVerticies, states);

    renderSelectedAntDebug();
    

    window.setView(window.getDefaultView());
    window.draw(timeText);

    if (selectedAntIndex != -1) {
        window.draw(posText);
        window.draw(velText);
        window.draw(accelText);
    }

    window.display();
}

void Simulation::renderSelectedAntDebug()
{
    if (selectedAntIndex < 0 || selectedAntIndex >= ants.size()) return;

    const Ant& selected = ants[selectedAntIndex];

    sf::Vector2f pos = selected.getPosition();
    sf::Vector2f vel = selected.getVelocity();
    sf::Vector2f acc = selected.getAcceleration();

    std::ostringstream ssPos, ssVel, ssAcc;
    ssPos << std::fixed << std::setprecision(2) << "Pos: (" << pos.x << ", " << pos.y << ")"; 
    ssVel << std::fixed << std::setprecision(2) << "Vel: (" << vel.x << ", " << vel.y << ")";
    ssAcc << std::fixed << std::setprecision(2) << "Acc: (" << acc.x << ", " << acc.y << ")";

    posText.setString(ssPos.str());
    velText.setString(ssVel.str());
    accelText.setString(ssAcc.str());

    // --- Vector drawing ---
    auto drawVector = [&](sf::Vector2f vector, sf::Color color)
    {
        sf::VertexArray line(sf::Lines, 2);

        line[0].position = pos;
        line[0].color = color;

        line[1].position = pos + vector;
        line[1].color = color;

        window.draw(line);
    };

    drawVector(vel, sf::Color::Cyan);
    drawVector(acc, sf::Color::Red);

    if (!followMouse) { 
        // 1. O Círculo à frente da formiga
        sf::CircleShape wanderCircle(selected.getWanderRadius());
        wanderCircle.setOrigin(selected.getWanderRadius(), selected.getWanderRadius());
        wanderCircle.setPosition(selected.getWanderCenter());
        wanderCircle.setFillColor(sf::Color::Transparent);
        wanderCircle.setOutlineColor(sf::Color(255, 255, 255, 150)); // Branco meio transparente
        wanderCircle.setOutlineThickness(1.f);
        window.draw(wanderCircle);

        // 2. O Ponto Alvo (a "mosca")
        sf::CircleShape targetDot(3.f); // Um pontinho de 3 pixeis
        targetDot.setOrigin(3.f, 3.f);
        targetDot.setPosition(selected.getWanderTarget());
        targetDot.setFillColor(sf::Color::Yellow);
        window.draw(targetDot);

        // 3. Linha de Visão (Formiga -> Alvo)
        sf::VertexArray wanderLine(sf::Lines, 2);
        wanderLine[0].position = pos; // Posição da formiga (já tens esta variável definida em cima)
        wanderLine[0].color = sf::Color(255, 255, 0, 100); // Amarelo transparente
        wanderLine[1].position = selected.getWanderTarget();
        wanderLine[1].color = sf::Color(255, 255, 0, 100);
        window.draw(wanderLine);
    }
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
    ants.reserve(100000);

    simulationTime = 0.f;
    timeText.setString("Time: 00:00:000");
    selectedAntIndex = -1;

    int numAnts = 10000;

    for (int i = 0; i < numAnts; ++i) {
        // Cool little code i made just to instead of spawning in a square
        // The ants will spawn in a circle lmao just fun

        float radius_x = static_cast<float>(rand() % 1000);
        float radius_y = static_cast<float>(rand() % 500);
        float random_angle = (static_cast<float>(rand() / static_cast<float> (RAND_MAX)) * (M_PI*4));

        ants.emplace_back(antTexture, sf::Vector2f(std::cos(random_angle) * radius_x, std::sin(random_angle) * radius_y));
    }

    antVerticies.setPrimitiveType(sf::Triangles);
    antVerticies.resize(ants.size() * 6);
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