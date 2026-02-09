#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

const int CELL_SIZE = 4;                    // Size of each cell in pixel (4x4)

struct Cell {
    float foodPheromone;                    // Amount of food pheromone in the cell
    float homePheromone;                    // Amount of home pheromone in the cell
    bool hasFood;                           // Whether the cell contains food
    bool isWall;                            // Whether the cell is a wall
};

class World {
    public:
        World(int width, int height);                                       

        void update(float dt);
        void draw(sf::RenderWindow &window);

        void addPheromone(sf::Vector2f position, float amount, int type);

        float getPheromoneLevel(sf::Vector2f position, int type) const;

        bool isWall(sf::Vector2f position) const;

        sf::Vector2u getSize() const {return sf::Vector2u(width, height);};

    private:
        int width, height;
        int pixelWidth, pixelHeight;

        std::vector<Cell> grid;

        sf::VertexArray vertexArray;

        int getIndexFromPos(sf::Vector2f pos) const;
};