#include <raylib.h>
#include <raymath.h>

#include <vector>
#include <iostream>
#include <string>

// ----- CONSTANTS -----
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const float TARGET_FPS = 60;
const float TIMESTEP = 1 / TARGET_FPS;
const float ELASTICITY = 1.0f;
const int PARTICLES_SPAWNED_PER_INPUT = 25;

const int CELL_SIZE = 60;

// Gets the number of cells in both axes of the Uniform Grid based on the Window Size and Cell Size
const int NUM_CELLS_X = WINDOW_WIDTH / CELL_SIZE + 1;
const int NUM_CELLS_Y = WINDOW_HEIGHT / CELL_SIZE + 1;

// ----- STRUCTS -----
struct Particle {
    Vector2 position;
    float radius;

    Vector2 velocity;

    float mass;
    float inverse_mass;

    Color color;

    // A list of all cell indices this particle belongs to
    std::vector<Vector2> cell_indices;
};

struct GridCell {
    Vector2 position;
    float size;
    std::vector<Particle*> particles;

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

    // List of all particles
    std::vector<Particle> particles;
    particles.reserve(10000);
    // initializes the Uniform Grid
    GridCell grid[NUM_CELLS_X][NUM_CELLS_Y];

    for (size_t x = 0; x < NUM_CELLS_X; x++) {
        for (size_t y = 0; y < NUM_CELLS_Y; y++) {
            grid[x][y].position.x = x * CELL_SIZE;
            grid[x][y].position.y = y * CELL_SIZE;
            grid[x][y].size = CELL_SIZE;
        }
    }

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
                for (size_t j = 0; j < particle_a.cell_indices.size(); j++) {
                    Vector2 index = particle_a.cell_indices[j];
                    grid[int(index.x)][int(index.y)].RemoveParticle(&particle_a);
                }
                particle_a.cell_indices.clear();

                // Gets the minimum and maximum x and y cell indices of the particle. (AABB surrounding the circle divided by the cell size)
                // int automatically floors the value so it returns the actual index instead of the position.
                int upper_left_cell_index_x = (particle_a.position.x - particle_a.radius) / CELL_SIZE;
                int upper_left_cell_index_y = (particle_a.position.y - particle_a.radius) / CELL_SIZE;
                int lower_right_cell_index_x = (particle_a.position.x + particle_a.radius) / CELL_SIZE;
                int lower_right_cell_index_y = (particle_a.position.y + particle_a.radius) / CELL_SIZE;

                // This adds particle A to all cells it intersects with.
                // Since you already have to add particle A to the cells it intersects with,
                // might as well check for collision in just those particular cells.
                // This also makes sure you only check cells that definitely has objects in it.
                for (int x = upper_left_cell_index_x; x <= lower_right_cell_index_x; x++) { // Goes through all X indices the object intersects with
                    for (int y = upper_left_cell_index_y; y <= lower_right_cell_index_y; y++) { // Goes through all Y indices the object intersects with
                        // Adds particle A to the specific grid cells and adds the cell indices to particle a
                        grid[x][y].particles.push_back(&particle_a);
                        particle_a.cell_indices.push_back({float(x), float(y)});
                        
                        // Performs the collision check for all objects along the particular cells
                        for(size_t j = 0; j < grid[x][y].particles.size(); j++) {
                            HandleCollision(particle_a, *grid[x][y].particles[j]); 
                        }
                    }
                }
            }

            accumulator -= TIMESTEP;
        }
        BeginDrawing();
        ClearBackground(WHITE);

        // Draws the grid if true
        if(grid_visible) {
            for(size_t x = 0; x < NUM_CELLS_X; x++) {
                for(size_t y = 0; y < NUM_CELLS_Y; y++) {
                    DrawRectangleLines(grid[x][y].position.x, grid[x][y].position.y, CELL_SIZE, CELL_SIZE, RED);
                    std::string grid_index = std::to_string(x) + "," + std::to_string(y);
                    DrawText(grid_index.c_str(), grid[x][y].position.x + 1, grid[x][y].position.y + 1, 10, BLACK);
                    DrawText(std::to_string(grid[x][y].particles.size()).c_str(), grid[x][y].position.x + ((CELL_SIZE / 2) - 4), grid[x][y].position.y + ((CELL_SIZE / 2) - 4), 16, BLACK);
                }
            }
        }

        for (size_t i = 0; i < particles.size(); i++) {
            DrawCircleV(particles[i].position, particles[i].radius, particles[i].color);
        }

        std::string number_of_particles = "Particles: " + std::to_string(particles.size());
        DrawText(number_of_particles.c_str(), 10, 10, 24, BLACK);
        std::string q_to_toggle = "Press Q to Toggle Uniform Grid Visibility";
        DrawText(q_to_toggle.c_str(), 10, 40, 24, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}