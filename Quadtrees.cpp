#include <raylib.h>
#include <raymath.h>

#include <vector>
#include <iostream>
#include <string>

// g++ quadtree.cpp -o out -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm

// ----- CONSTANTS -----
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const float TARGET_FPS = 60;
const float TIMESTEP = 1 / TARGET_FPS;
const float ELASTICITY = 1.0f;
const int PARTICLES_SPAWNED_PER_INPUT = 25;

const int CELL_SIZE = 60;

const int DEPTH = 5;

// Gets the number of cells in both axes of the Uniform Grid based on the Window Size and Cell Size
const int NUM_CELLS_X = WINDOW_WIDTH / CELL_SIZE + 1;
const int NUM_CELLS_Y = WINDOW_HEIGHT / CELL_SIZE + 1;

// ----- STRUCTS -----
struct GridCell;

struct Particle {
    Vector2 position;
    float radius;

    Vector2 velocity;

    float mass;
    float inverse_mass;

    Color color;

    Vector2 min = {position.x - radius, position.y - radius};
    Vector2 max = {position.x + radius, position.y + radius};

    // A list of all cells this particle belongs to
    std::vector<GridCell*> cells_occupied;
    std::vector<GridCell*> cells_overlapped;
};

struct GridCell {
    Vector2 position;
    float size;
    std::vector<Particle*> particles;
    GridCell* parent;
    GridCell* children[4];
    bool active = false;

    // Removes the specific particle from the grid cell
    void RemoveParticle(Particle* particle) {
        for (size_t i = 0; i < particles.size(); i++) {
            if (particles[i] == particle) {
                particles.erase(particles.begin() + i);
                break;
            }
        }
    }
};

// ----- FUNCTIONS -----
// Creates Quadtree
void MakeQuadtree(Vector2 pos, float s, GridCell* par, int currentDepth, GridCell* root)
{
    root -> position = pos;
    root -> size = s;

    if(currentDepth <= 0)
    {
        root -> children[0] = nullptr;
        root -> children[1] = nullptr;
        root -> children[2] = nullptr;
        root -> children[3] = nullptr;
        return;
    }
    else
    {
        GridCell newChild0;
        GridCell newChild1;
        GridCell newChild2;
        GridCell newChild3;

        MakeQuadtree({root -> position.x, root -> position.y}, s/2, root, currentDepth - 1, &newChild0);
        MakeQuadtree({root -> position.x + s/2, root -> position.y}, s/2, root, currentDepth - 1,  &newChild1);
        MakeQuadtree({root -> position.x, root -> position.y + s/2}, s/2, root, currentDepth - 1, &newChild2);
        MakeQuadtree({root -> position.x + s/2, root -> position.y + s/2}, s/2, root, currentDepth - 1, &newChild3);

        root -> children[0] = new GridCell(newChild0);
        root -> children[1] = new GridCell(newChild1);
        root -> children[2] = new GridCell(newChild2);
        root -> children[3] = new GridCell(newChild3);
    }
}

void DrawQuadtree(GridCell* node)
{
    if(node -> children[0] == nullptr)
    {
        if(node -> active)
        {
            DrawRectangleLines(node -> position.x, node -> position.y, node -> size, node -> size, RED);
            DrawText(std::to_string(node -> particles.size()).c_str(), node -> position.x + ((node -> size / 2) - 4), node -> position.y + ((node -> size / 2) - 4), 16, GREEN);
        }
        return;
    }
    else
    {
        if(node -> active)
        {
            DrawRectangleLines(node -> position.x, node -> position.y, node -> size, node -> size, RED);
            DrawText(std::to_string(node -> particles.size()).c_str(), node -> position.x + ((node -> size / 2) - 4), node -> position.y + ((node -> size / 2) - 4), 16, GREEN);
        }
        DrawQuadtree(node -> children[0]);
        DrawQuadtree(node -> children[1]);
        DrawQuadtree(node -> children[2]);
        DrawQuadtree(node -> children[3]);
    }
}

// Computes for the impulse
float GetImpulse(float elasticity, Vector2 relative_velocity, Vector2 collision_normal, float inverse_mass_a, float inverse_mass_b) {
    float numerator = -(1 + elasticity) * Vector2DotProduct(relative_velocity, collision_normal);
    float denominator = Vector2DotProduct(collision_normal, collision_normal) * (inverse_mass_a + inverse_mass_b);
    return numerator / denominator;
}

// Handles collision between two particles
void HandleCollision(Particle &particle_a, Particle &particle_b) {
    if(&particle_a != &particle_b) {
        Vector2 collision_normal = Vector2Subtract(particle_a.position, particle_b.position);
        float distance = Vector2Length(collision_normal);
        float radius_sum = particle_a.radius + particle_b.radius;

        Vector2 relative_velocity = Vector2Subtract(particle_a.velocity, particle_b.velocity);
        float normal_rel_vel_dot = Vector2DotProduct(collision_normal, relative_velocity);

        if (distance <= radius_sum && normal_rel_vel_dot < 0.0f) {
            float impulse = GetImpulse(ELASTICITY, relative_velocity, collision_normal, particle_a.inverse_mass, particle_b.inverse_mass);
            particle_a.velocity = Vector2Add(particle_a.velocity, Vector2Scale(collision_normal, impulse * particle_a.inverse_mass));
            particle_b.velocity = Vector2Subtract(particle_b.velocity, Vector2Scale(collision_normal, impulse * particle_b.inverse_mass));
        }
    } 
}

bool CheckEncompass(Particle* particle, GridCell* node)
{
    bool greater_min = ((particle -> min.x > node -> position.x) && (particle -> min.y > node -> position.y));
    // std::cout << "Min check: Particlex-" << particle -> min.x << " Nodex-" << node -> min.x << " || Particley-" << particle -> min.y << " Nodey-" << node -> min.y << std::endl;
    bool less_max = ((particle -> max.x < ((node -> position.x) + node -> size)) && (particle -> max.y < ((node -> position.y) + node -> size)));
    // std::cout << "Max check: Particlex-" << particle -> max.x << " Nodex-" << node -> max.x << " || Particley-" << particle -> max.y << " Nodey-" << node -> max.y << std::endl;

    if(greater_min && less_max)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CheckIntersection(Particle* particle1, Particle* particle2)
{
  return (particle1->min.x <= particle2->max.x && particle1->max.x >= particle2->min.x) &&
         (particle1->min.y <= particle2->max.y && particle1->max.y >= particle2->min.y);
}

bool CheckIntersectionNode(Particle* particle1, GridCell* node)
{
  return (particle1->min.x <= node->position.x + node->size && particle1->max.x >= node->position.x) &&
         (particle1->min.y <= node->position.y + node->size && particle1->max.y >= node->position.y);
}

void TraverseQuadtreeNodeOverlap(Particle* particle, GridCell* node)
{
    if (node->children[0] == nullptr)
    {
        // std::cout << "Leaf" << std::endl;

        particle -> cells_overlapped.push_back(node);
        
        return;
    }
    else
    {

        for (int i = 0; i < 4; i++)
        {
            if (CheckIntersectionNode(particle, node->children[i]))
            {
                particle -> cells_overlapped.push_back(node->children[i]);
                TraverseQuadtreeNodeOverlap(particle, node->children[i]);
            }
        }
    }
}

void TraverseQuadtree(Particle* particle, GridCell* node)
{
    GridCell* child = nullptr;

    if(node -> children[0] == nullptr)
    {
        // std::cout << "Leaf" << std::endl;

        node -> particles.push_back(particle);
        node -> active = true;
        particle -> cells_occupied.push_back(node);

        // Performs the collision check for all objects along the particular cells
        for(size_t j = 0; j < node -> particles.size(); j++) {
            HandleCollision(*particle, *(node -> particles[j])); 
        }

        return;
    }

    for(int i = 0; i < 4; i++)
    {
        if(CheckEncompass(particle, node -> children[i]))
        {
            child = node -> children[i];
            break;
        }
    }

    if(child == nullptr)
    {
        node -> particles.push_back(particle);
        node -> active = true;
        particle -> cells_occupied.push_back(node);

        // Performs the collision check for all objects along the particular cells
        for(size_t j = 0; j < node -> particles.size(); j++) {
            HandleCollision(*particle, *(node -> particles[j])); 
        }

        return;
    }
    else
    {
        TraverseQuadtree(particle, child);
    }
}

void CheckParticles(Particle* particle, GridCell* node)
{
    std::vector<Particle*> intersecting_objects;
    for (size_t i = 0; i < node->particles.size(); i++)
    {
        if (CheckIntersection(particle, node->particles[i]))
        {
            intersecting_objects.push_back(node->particles[i]);
        }
    }

    for (size_t j = 0; j < intersecting_objects.size(); j++)
    {
        HandleCollision(*particle, *(intersecting_objects[j]));
    }
}

// void TraverseQuadtree(Particle* particle, GridCell* node)
// {
//     GridCell* child = nullptr;

//     if (node->children[0] == nullptr)
//     {
//         // std::cout << "Leaf" << std::endl;

//         node->particles.push_back(particle);
//         node->active = true;
//         particle->cells_occupied.push_back(node);

//         std::vector<Particle*> intersecting_objects;
//         for (size_t i = 0; i < node->particles.size(); i++)
//         {
//             if (CheckIntersection(particle, node->particles[i]))
//             {
//                 intersecting_objects.push_back(node->particles[i]);
//             }
//         }

//         for (size_t j = 0; j < intersecting_objects.size(); j++)
//         {
//             HandleCollision(*particle, *(intersecting_objects[j]));
//         }
//     }
//     else
//     {

//         for (int i = 0; i < 4; i++)
//         {
//             if (CheckEncompass(particle, node->children[i]))
//             {
//                 child = node->children[i];
//                 break;
//             }
//         }

//         if (child == nullptr)
//         {

//             node->particles.push_back(particle);
//             node->active = true;
//             particle->cells_occupied.push_back(node);

//             std::vector<Particle*> intersecting_objects;
//             for (size_t i = 0; i < node->particles.size(); i++)
//             {
//                 if (CheckIntersection(particle, node->particles[i]))
//                 {
//                     intersecting_objects.push_back(node->particles[i]);
//                 }
//             }


//             for (size_t j = 0; j < intersecting_objects.size(); j++)
//             {
//                 HandleCollision(*particle, *(intersecting_objects[j]));
//             }
//         }
//         else
//         {

//             TraverseQuadtree(particle, child);
//         }
//     }
// }

Vector2 GetRandomVelocity(float min_speed, float max_speed) {
    Vector2 ret = Vector2Zero();

    // Gets a random direction for the velocity vector
    ret.x = GetRandomValue(-100.0f, 100.0f) / 100.0f;
    ret.y = GetRandomValue(-100.0f, 100.0f) / 100.0f;

    ret = Vector2Normalize(ret);

    // Gets a random speed based on the range for the velocity vector
    float speed = min_speed + GetRandomValue(0.0f, 100.0f) / 100.0f * (max_speed - min_speed);
    
    ret = Vector2Scale(ret, speed);

    return ret;
}

Color GetRandomColor() {
    Color ret;
    ret.r = GetRandomValue(0, 255);
    ret.g = GetRandomValue(0, 255);
    ret.b = GetRandomValue(0, 255);
    ret.a = 255;

    return ret;
}

float GetRandomFloat(float min, float max)
{
    int rand = GetRandomValue(0, 100);
    return min + rand / 100.0f * (max - min);
}

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "UNIFORM GRID SAMPLE");

    GridCell* currentNode;

    // List of all particles
    std::vector<Particle> particles;
    particles.reserve(10000);


    // initializes the Quadtree
    GridCell quadTree;
    MakeQuadtree({0, 0}, WINDOW_WIDTH, NULL, DEPTH, &quadTree);

    currentNode = &quadTree;
    std::cout << "Current Node: " << currentNode -> position.x << ", " << currentNode -> position.y << "|| size: "<< currentNode -> size << std::endl;


    SetTargetFPS(TARGET_FPS);

    float accumulator = 0.0f;

    int spawn_counter = 0;

    bool grid_visible = false;

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        // Spawns 25 Particles per press, but on the 11th, spawns a big Particle
        if (IsKeyPressed(KEY_SPACE)) {
            if (spawn_counter == 10) {
                particles.emplace_back();
                particles.back().position = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
                particles.back().radius = 25.0f;
                particles.back().velocity = GetRandomVelocity(100.0f, 200.0f);
                particles.back().mass = 20.0f;
                particles.back().inverse_mass = 1.0f / particles.back().mass;
                particles.back().color = GetRandomColor();

                spawn_counter = 0;
            }
            else {
                for (int i = 0; i < PARTICLES_SPAWNED_PER_INPUT; i++) {
                    particles.emplace_back();
                    particles.back().position = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
                    particles.back().radius = GetRandomFloat(5.0f, 10.0f);
                    particles.back().velocity = GetRandomVelocity(200.0f, 400.0f);
                    particles.back().mass = 1.0f;
                    particles.back().inverse_mass = 1.0f / particles.back().mass;
                    particles.back().color = GetRandomColor();
                }

                spawn_counter++;
            }
        }

        // Toggle For Grid Visibility 
        if(IsKeyPressed(KEY_Q)) {
            grid_visible = !grid_visible;
        }

        // Physics Loop
        accumulator += delta_time;
        while (accumulator >= TIMESTEP) {
            for(size_t i = 0; i < particles.size(); i++) {
                Particle& particle_a = particles[i];
                // No need to update velocity based on acceleration as there is no acceleration
                particle_a.position = Vector2Add(particle_a.position, Vector2Scale(particle_a.velocity, TIMESTEP));
                particle_a.min = {particle_a.position.x - particle_a.radius, particle_a.position.y - particle_a.radius};
                particle_a.max = {particle_a.position.x + particle_a.radius, particle_a.position.y + particle_a.radius};

                // Bounce off Screen Edges with prevention for screen hugging
                if(particle_a.position.x - particle_a.radius <= 0.0f) {
                    particle_a.position.x = particle_a.radius;
                    particle_a.velocity.x *= -1.0f;
                }
                if(particle_a.position.x + particle_a.radius >= WINDOW_WIDTH) {
                    particle_a.position.x = WINDOW_WIDTH - particle_a.radius;
                    particle_a.velocity.x *= -1.0f;
                }
                if(particle_a.position.y - particle_a.radius <= 0.0f) {
                    particle_a.position.y = particle_a.radius;
                    particle_a.velocity.y *= -1.0f;
                }
                if(particle_a.position.y + particle_a.radius >= WINDOW_HEIGHT) {
                    particle_a.position.y = WINDOW_HEIGHT - particle_a.radius;
                    particle_a.velocity.y *= -1.0f;
                }

                // After Moving the Particle
                // Remove the particle from its current cells
                for (size_t j = 0; j < particle_a.cells_occupied.size(); j++) {
                    particle_a.cells_occupied[j] -> RemoveParticle(&particle_a);
                    particle_a.cells_occupied[j] -> active = false;
                }
                particle_a.cells_occupied.clear();
                particle_a.cells_overlapped.clear();

                // Adding Particle to Cell
                //std::cout << "BOOM" << std::endl;
                TraverseQuadtree(&particle_a, currentNode);
                TraverseQuadtreeNodeOverlap(&particle_a, particle_a.cells_occupied[0]);

                for(int i = 0; i < particle_a.cells_overlapped.size(); i++)
                {
                    CheckParticles(&particle_a, particle_a.cells_overlapped[i]);
                }
            }

            accumulator -= TIMESTEP;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Draws the grid if true
        if(grid_visible) {
            // DrawRectangle(100, 100, 100, 100, RED);
            DrawQuadtree(&quadTree);

            // for(size_t x = 0; x < NUM_CELLS_X; x++) {
            //     for(size_t y = 0; y < NUM_CELLS_Y; y++) {
            //         DrawRectangleLines(grid[x][y].position.x, grid[x][y].position.y, CELL_SIZE, CELL_SIZE, RED);
            //         std::string grid_index = std::to_string(x) + "," + std::to_string(y);
            //         DrawText(grid_index.c_str(), grid[x][y].position.x + 1, grid[x][y].position.y + 1, 10, GREEN);
            //         DrawText(std::to_string(grid[x][y].particles.size()).c_str(), grid[x][y].position.x + ((CELL_SIZE / 2) - 4), grid[x][y].position.y + ((CELL_SIZE / 2) - 4), 16, GREEN);
            //     }
            // }
        }

        for (size_t i = 0; i < particles.size(); i++) {
            DrawCircleV(particles[i].position, particles[i].radius, particles[i].color);
        }

        std::string number_of_particles = "Particles: " + std::to_string(particles.size());
        DrawText(number_of_particles.c_str(), 10, 10, 24, GREEN);
        std::string q_to_toggle = "Press Q to Toggle Uniform Grid Visibility";
        DrawText(q_to_toggle.c_str(), 10, 40, 24, GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}