/**
 * @file Simulation.hpp
 * @brief Defines the Simulation class, which manages the main loop and rendering
 *        of the Ant Colony Simulation.
 * 
 * This class encapsulates all high-level behaviour of the simulation, including:
 * - Event handling
 * - Time tracking and pause control
 * - Camera Movement
 * - Drawing ants and debug overlays
 * - Simulation reset and step-by-step execution
 * 
 * @date 2025-10-29
 * @author rupixe
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <Ant.hpp>
#include <vector>

/**
 * @class Simulation
 * @brief Manages the entire simulation lifecycle and rendering pipeline.
 */
class Simulation {
    
public:
    /**
     * @brief Constructs and initializes the simulation window, font, and entities.
     */
    Simulation();

    /**
     * @brief Cleans up simulation resources.
     */
    ~Simulation();

    /**
     * @brief Main loop that updates, renders, and processes input events.
     */
    void run();

private:
    /** @brief Handles all SFML window and keyboard/mouse input events. */
    void processEvents();

    /** @brief Updates all simulation entities and logic for the current frame. */
    void update(float dt);

    /** @brief Renders ants, UI, and other visual elements to the window. */
    void render();

    /** @brief Tracks and updates the elapsed simulation time. */
    void timeCounter(float dt);

    /** @brief Resets the simulation state, clearing all ants and resetting time. */
    void reset();

    /** @brief Handles camera panning via mouse drag. */
    void drag();

private:
    // ─────────────────────────────
    // Window and View Management
    // ─────────────────────────────
    sf::RenderWindow window;                    ///< Main application window.
    sf::View view = window.getDefaultView();    ///< Camera view for zoom/pan control.

    // ─────────────────────────────
    // Timing and Display
    // ─────────────────────────────
    sf::Clock clock;                            ///< Frame timing clock.
    sf::Font font;                              ///< Font used for text rendering.
    sf::Text timeText;                          ///< Displays simulation time.
    sf::Text velText;                           ///< Displays selected ant velocity (debug).
    sf::Text accelText;                         ///< Displays selected ant acceleration (debug).

    // ─────────────────────────────
    // Camera Interaction
    // ─────────────────────────────
    sf::Vector2f dragStart;                     ///< Starting position for mouse drag.
    bool isDragging = false;                    ///< Whether the camera is currently being dragged.

    // ─────────────────────────────
    // Simulation Control
    // ─────────────────────────────
    bool paused = false;                        ///< Indicates whether the simulation is paused.
    bool stepOnce = false;                      ///< Allows advancing a single frame when paused.
    float simulationTime = 0.f;                 ///< Elapsed time since simulation start (in seconds).
    int selectedAntIndex = -1;                  ///< Index of the currently selected ant (-1 = none).

    // ─────────────────────────────
    // Entities
    // ─────────────────────────────
    sf::Texture antTexture;                     ///< Texture used by all ants.
    std::vector<Ant> ants;                      ///< List of all active ants.
};