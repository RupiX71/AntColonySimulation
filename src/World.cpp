#include <World.hpp>
#include <cmath>
#include <algorithm>

World::World(int width, int height)
    : width(width), height(height)
{
    this->width = width/CELL_SIZE;
    this->height = height/CELL_SIZE;

    grid.resize(this->width * this->height);
    vertexArray.setPrimitiveType(sf::Quads);
    vertexArray.resize(this->width * this->height * 4);

    for (int x = 0 ; x < this->width ; ++x) {
        for (int y = 0 ; y < this->height ; ++y) {
            int i = x + y * this->width;    
            int v = i * CELL_SIZE;
            
            vertexArray[v+0].position = sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE);
            vertexArray[v+1].position = sf::Vector2f((x+1) * CELL_SIZE, y * CELL_SIZE);
            vertexArray[v+2].position = sf::Vector2f((x+1) * CELL_SIZE, (y+1) * CELL_SIZE);
            vertexArray[v+3].position = sf::Vector2f(x * CELL_SIZE, (y+1) * CELL_SIZE);

            for (int k = 0 ; k < CELL_SIZE ; k++) vertexArray[k+v].color = sf::Color(0,0,0,0);
        }
    }
}

void World::update(float dt) {
    float evaporationRate = 0.5f;

    for (int i = 0 ; i < grid.size() ; ++i) {
        grid[i].homePheromone = std::max(0.0f, grid[i].homePheromone - evaporationRate * dt);
        grid[i].foodPheromone = std::max(0.0f, grid[i].foodPheromone - evaporationRate * dt);

        if (grid[i].homePheromone > 0.01f || grid[i].foodPheromone > 0.01f) {
            int v = i * CELL_SIZE;

            sf::Uint8 red = static_cast<sf::Uint8>(std::min(255.0f, grid[i].foodPheromone * 255));
            sf::Uint8 blue = static_cast<sf::Uint8>(std::min(255.0f, grid[i].homePheromone * 255));

            sf::Uint8 alpha = std::max(red, blue); 

            sf::Color color(red, 0, blue, alpha);

            for(int k=0; k<4; k++) vertexArray[v+k].color = color;
        } else {
            int v = i * 4;
            if (vertexArray[v].color.a != 0) {
                 for(int k=0; k<4; k++) vertexArray[v+k].color = sf::Color::Transparent;
            }
        }
    }
}

void World::draw(sf::RenderWindow &window) {
    window.draw(vertexArray);
}

void World::addPheromone(sf::Vector2f position, float amount, int type) {
    int index = getIndexFromPos(position);
    if (index == -1) return;

    if (type == 0) {
        grid[index].homePheromone = std::min(1.0f, grid[index].homePheromone + amount);
    } else {
        grid[index].foodPheromone = std::min(1.0f, grid[index].foodPheromone + amount);
    }
}

bool World::isWall(sf::Vector2f position) const {
    int index = getIndexFromPos(position);
    if (index == -1) return true;
    return grid[index].isWall;
}

int World::getIndexFromPos(sf::Vector2f pos) const {
    int x = static_cast<int>(pos.x) / CELL_SIZE;
    int y = static_cast<int>(pos.y) / CELL_SIZE;

    if (x < 0 || x >= width || y < 0 || y >= height) return -1;
    
    return x + y * width;
}