/******************************************************************************
 * Homework 3 - Pool Table
 * Antiporda, Banal, Labre, 2023
 *
 * Accepts input during runtime - based on config keys, if blank config then default keys are used based on specification 
 * Outputs white screen with particles that emit from either bottom center of the screen or on mouse cursor
 *****************************************************************************/

// g++ HW3_AntipordaBanalLabre_Main.cpp -o out -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm


#include <raylib.h>
#include <raymath.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// declare constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float FPS = 60;
const float TIMESTEP = 1/FPS;
const float FRICTION = 4.0;

// Circle struct
struct Ball {
    Vector2 position, velocity, acceleration;
    float radius, mass, inverse_mass;
    Color color;
    bool active_ball, pocket;
    std::string type;
};

// Rectangle struct
struct Border {
    Vector2 position, size, velocity;
    Color color;
    float mass, inverse_mass;
};

// Line struct
struct Spring {
    float spring_max, k, spring_damp, rest_length;
    Vector2 spring_start, spring_end;
    Color color;
    std::string name;
    bool active;
};

// calculate the collision normal vector
Vector2 calculateCollisionNormal(Ball* a, Ball* b) {
    Vector2 nv;
    nv = Vector2Normalize(Vector2Subtract(a->position, b->position));
    return nv;
}

// calculate the relative velocity
Vector2 calculateRelativeVelocity(Ball* a, Ball* b) {
    Vector2 rv;
    rv = Vector2Subtract(a->velocity, b->velocity);
    return rv;
}

// calculate the collision normal vector for rect
Vector2 calculateCollisionNormalRect(Border* border, Ball* ball) {
    Vector2 nv;
    nv = Vector2Normalize(Vector2Subtract(ball->position, border->position));
    return nv;
}

// calculate the relative velocity for rect
Vector2 calculateRelativeVelocityRect(Border* border, Ball* ball) {
    Vector2 rv;
    rv = Vector2Subtract(ball->velocity, border->velocity);
    return rv;
}

// calculate collision impulse for ball
float calculateBallImpulse(Ball* a, Ball* b, float e) {
    Vector2 nv = calculateCollisionNormal(a, b);
    Vector2 rv = calculateRelativeVelocity(a, b);

    float num = (1 + e) * Vector2DotProduct(rv, nv);
    float den = Vector2DotProduct(nv, nv) * ( (a->inverse_mass) + (b->inverse_mass) );
    float j = -(num / den);

    return j;
}

// calculate collision impulse for rectangle
float calculateRectImpulse(Border* border, Ball* ball, float e) {
    Vector2 nv = calculateCollisionNormalRect(border, ball);
    Vector2 rv = calculateRelativeVelocityRect(border, ball);

    float num = (1 + e) * Vector2DotProduct(rv, nv);
    float den = Vector2DotProduct(nv, nv) * ( (border->inverse_mass) + (ball->inverse_mass) );
    float j = -(num / den);

    return j;
}

// calculate angle and convert to degrees
float calculateAngle(Ball* a, Ball* b) {
    float angle = Vector2Angle(calculateCollisionNormal(a, b), calculateRelativeVelocity(a, b));
    float deg = (angle*180) / M_PI;
    return deg;
}

// calculate initial velocity and position
void calculatePhysics(Ball* a, float delta_time) {
    a->velocity = Vector2Add(a->velocity, Vector2Scale(a->acceleration, TIMESTEP));
    a->velocity = Vector2Subtract(a->velocity, Vector2Scale(a->velocity, FRICTION * a->inverse_mass * delta_time));
    a->position = Vector2Add(a->position, Vector2Scale(a->velocity, TIMESTEP));
}

// calculate spring force
Vector2 calculateSpringForce(Spring* spring, Ball* ball) {
    Vector2 spring_force;
    Vector2 D = Vector2Subtract(spring->spring_end, spring->spring_start);
    float d_mag = Vector2Length(D);
    Vector2 d_over_d_mag = Vector2Scale(D, 1/d_mag);
    spring_force = Vector2Scale(d_over_d_mag, -spring->k * (d_mag - spring->rest_length));
    spring_force = Vector2Subtract(spring_force, Vector2Scale(ball->velocity, spring->spring_damp));

    return spring_force;
}

Vector2 calculateQ(Border* border, Ball* ball) {
    Vector2 q;

    Vector2 min = Vector2Subtract(border->position, {0, border->size.y});
    Vector2 max = Vector2Add(border->position, {border->size.x, 0});
    // Check if the circle's center position is already inside the rectangle.
    if (ball->position.x >= min.x && ball->position.x <= max.x &&
        ball->position.y >= min.y && ball->position.y <= max.y) {
        q = ball->position;
    } else {
        q.x = Clamp(ball->position.x, min.x, min.x + max.x);
        q.y = Clamp(ball->position.y, min.y, min.y + max.y);
    }
    //std::cout << Vector2Length(min) << std::endl;

    return q;
}

// check border collision
void calculateWallCollision(Ball* ball) {
    if(ball->position.x + ball->radius >= WINDOW_WIDTH || ball->position.x - ball->radius <= 0) ball->velocity.x *= -1;
    if(ball->position.y + ball->radius >= WINDOW_HEIGHT || ball->position.y - ball->radius <= 0) ball->velocity.y *= -1;
}

// check ball-ball collision
void calculateBallCollision(Ball* a, Ball* b, float e) {
    float j = calculateBallImpulse(a, b, e);

    a->velocity = Vector2Add(a->velocity, Vector2Scale(calculateCollisionNormal(a, b), j/a->mass));
    b->velocity = Vector2Subtract(b->velocity, Vector2Scale(calculateCollisionNormal(a, b), j/b->mass));
}

// check ball-rectangle collision
void calculateRectCollision(Border* border, Ball* ball, float e) {
    float j = calculateRectImpulse(border, ball, e);

    ball->velocity = Vector2Add(ball->velocity, Vector2Scale(calculateCollisionNormalRect(border, ball), j/ball->mass));
    border->velocity = Vector2Subtract(border->velocity, Vector2Scale(calculateCollisionNormalRect(border, ball), j/border->mass));
}

// set Ball properties
void setBallProperties(Ball* ball, Vector2 pos, float r, Color col, float m, bool a, bool p, std::string t) {
    // Vector2* position = &pos;
    // float* radius = &r;
    // Color* color = &col;
    // float* mass = &m;
    // bool* active_ball = &a;
    // bool* pocket = &p;
    // std::string* type = &t;

    ball->position = pos;
    ball->radius = r;
    ball->color = col;
    ball->mass = m;
    ball->inverse_mass = 1 / ball->mass;
    ball->active_ball = a;
    ball->pocket = p;
    ball->type = t;
    ball->velocity = Vector2Zero();
    ball->acceleration = Vector2Zero();
}

// set Border properties
void setBorderProperties(Border* border, Vector2 pos, Vector2 s, Vector2 vel, Color col, float m, float im) {
    // Vector2* position = &pos;
    // Vector2* size = &s;
    // Color* color = &col;

    border->position = pos;
    border->size = s;
    border->velocity = vel;
    border->color = col;
    border->mass = m;
    border->inverse_mass = im;
}

// set Spring properties
void setSpringProperties(Spring* spring, Vector2 s_start, Vector2 s_end, float r_length, float s_max, float k_val, float s_damp, std::string n, bool act) {
    // Vector2* spring_start = &s_start;
    // Vector2 spring_end = s_end;
    // float* spring_max = &s_max;
    // float* spring_damp = &s_damp;
    // float* rest_length = &r_length;
    // float* k = &k_val;
    // std::string name = name;
    // bool* active = &act;

    spring->spring_start = s_start;
    spring->spring_end = s_end;
    spring->spring_max = s_max;
    spring->spring_damp = s_damp;
    spring->rest_length = r_length;
    spring->k = k_val;
    spring->name = n;
    spring->active = act;
}

int main() {
    // Initialize balls (0 - cue ball, 1-16 - colored balls, 17-20 pockets)
    Ball ball[9];
    Ball* pBall[9];
    pBall[0] = &ball[0];
    pBall[1] = &ball[1];
    pBall[2] = &ball[2];
    pBall[3] = &ball[3];
    pBall[4] = &ball[4];
    pBall[5] = &ball[5];
    pBall[6] = &ball[6];
    pBall[7] = &ball[7];
    pBall[8] = &ball[8];
    // pBall[9] = &ball[9];
    // pBall[10] = &ball[10];
    // pBall[11] = &ball[11];
    // pBall[12] = &ball[12];
    // pBall[13] = &ball[13];
    // pBall[14] = &ball[14];
    // pBall[15] = &ball[15];
    // pBall[16] = &ball[16];
    // pBall[17] = &ball[17];
    // pBall[18] = &ball[18];
    // pBall[19] = &ball[19];
    // pBall[20] = &ball[20];

    // Initialize borders
    Border border[4];
    Border* pBorder[4];
    pBorder[0] = &border[0];
    pBorder[1] = &border[1];
    pBorder[2] = &border[2];
    pBorder[3] = &border[3];

    // Initialize spring
    Spring spring[2];
    Spring* pSpring[2];
    pSpring[0] = &spring[0];
    pSpring[1] = &spring[1];

    // Set cue ball
    setBallProperties(pBall[0],
                        {WINDOW_WIDTH/4, WINDOW_HEIGHT/2},
                        25.0f,
                        WHITE,
                        5.0f,
                        true,
                        false,
                        "cue");

    std::cout << "WORKS" << std::endl;

    // Set other balls
    // for (int i=1; i<17; i++) {
    //     if (i<9) {
    //         setBallProperties(pBall[i],
    //                     {WINDOW_WIDTH/2, WINDOW_HEIGHT/2},
    //                     25.0f,
    //                     WHITE,
    //                     5.0f,
    //                     false,
    //                     false,
    //                     "solid");
    //     } else {
    //         setBallProperties(pBall[i],
    //                     {WINDOW_WIDTH/2, WINDOW_HEIGHT/2},
    //                     25.0f,
    //                     WHITE,
    //                     5.0f,
    //                     false,
    //                     false,
    //                     "stripe");
    //     }
    // }

    setBallProperties(pBall[1],
                        {WINDOW_WIDTH/2, WINDOW_HEIGHT/2},
                        25.0f,
                        BLUE,
                        5.0f,
                        false,
                        false,
                        "solid");
    setBallProperties(pBall[2],
                        {WINDOW_WIDTH/2 + 100, WINDOW_HEIGHT/2},
                        25.0f,
                        RED,
                        5.0f,
                        false,
                        false,
                        "solid");
    setBallProperties(pBall[3],
                        {WINDOW_WIDTH/2 + 50, WINDOW_HEIGHT/2 - 27},
                        25.0f,
                        PURPLE,
                        5.0f,
                        false,
                        false,
                        "stripe");
    setBallProperties(pBall[4],
                        {WINDOW_WIDTH/2 + 50, WINDOW_HEIGHT/2 + 27},
                        25.0f,
                        YELLOW,
                        5.0f,
                        false,
                        false,
                        "stripe");

    std::cout << "WORKS" << std::endl;



    // Set pockets
    // for (int i=17; i<21; i++) {
    //     setBallProperties(pBall[i],
    //                     {WINDOW_WIDTH/2, WINDOW_HEIGHT/2},
    //                     50.0f,
    //                     BLACK,
    //                     5.0f,
    //                     false,
    //                     true,
    //                     "pocket");
    // }

    setBallProperties(pBall[5],
                        {50, 50},
                        50.0f,
                        BLACK,
                        100000.0f,
                        false,
                        true,
                        "pocket");

    setBallProperties(pBall[6],
                        {WINDOW_WIDTH - 50, 50},
                        50.0f,
                        BLACK,
                        100000.0f,
                        false,
                        true,
                        "pocket");
    
    setBallProperties(pBall[7],
                        {50, WINDOW_HEIGHT - 50},
                        50.0f,
                        BLACK,
                        100000.0f,
                        false,
                        true,
                        "pocket");
    
    setBallProperties(pBall[8],
                        {WINDOW_WIDTH - 50, WINDOW_HEIGHT - 50},
                        50.0f,
                        BLACK,
                        100000.0f,
                        false,
                        true,
                        "pocket");

    std::cout << "WORKS" << std::endl;

    // Set borders
    // for (int i=0; i<4; i++) {
    //     setBorderProperties(pBorder[i],
    //                         {WINDOW_WIDTH/2, WINDOW_HEIGHT/2},
    //                         {100, 100},
    //                         BROWN);
    // }

    setBorderProperties(pBorder[0],
                            {100, 0},
                            {WINDOW_WIDTH - 200, 50},
                            Vector2Zero(),
                            BROWN,
                            1000000000.0f,
                            0.0f);

    setBorderProperties(pBorder[1],
                            {100, WINDOW_HEIGHT - 50},
                            {WINDOW_WIDTH - 200, 50},
                            Vector2Zero(),
                            BROWN,
                            1000000000.0f,
                            0.0f);

    setBorderProperties(pBorder[2],
                            {0, 100},
                            {50, WINDOW_HEIGHT - 200},
                            Vector2Zero(),
                            BROWN,
                            1000000000.0f,
                            0.0f);

    setBorderProperties(pBorder[3],
                            {WINDOW_WIDTH - 50, 100},
                            {50, WINDOW_HEIGHT - 200},
                            Vector2Zero(),
                            BROWN,
                            1000000000.0f,
                            0.0f);

    // Set cues
    // for (int i=0; i<2; i++) {
    //     setSpringProperties(pSpring[i],
    //                         {WINDOW_WIDTH/4, WINDOW_HEIGHT/2},
    //                         ball[0].position,
    //                         Vector2Distance(spring[i].spring_start, Vector2Scale(spring[i].spring_end, 1.1)),
    //                         spring[i].rest_length * 1.5,
    //                         100.0f,
    //                         1.0f,
    //                         "player i",
    //                         true);
    // }

    setSpringProperties(pSpring[0],
                        {0, WINDOW_HEIGHT/2},
                        GetMousePosition(),
                        Vector2Distance(spring[0].spring_start, Vector2Scale(spring[0].spring_end, 1.1)),
                        spring[0].rest_length * 1.5,
                        100.0f,
                        1.0f,
                        "player i",
                        true);

    std::cout << "WORKS" << std::endl;


    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Billiards");
    std::cout << "STILLWORKS" << std::endl;
    SetTargetFPS(FPS);

    float accumulator = 0.0f;
    float e = 1.0f;

    bool holding = false;
    

    while(!WindowShouldClose()) {

        float delta_time = GetFrameTime();

        Vector2 forces = Vector2Zero();

        // calculate spring force
        // Vector2 spring_force = calculateSpringForce(pSpring[0], pBall[0]);
        // forces = Vector2Add(forces, spring_force);

        // use mouse to hold cue
        if (CheckCollisionPointCircle(GetMousePosition(), ball[0].position, ball[0].radius) || holding) {
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                std::cout << "HOLDING" << std::endl;
                holding = true;
                // ball[0].position = GetMousePosition();
                // forces = Vector2Zero();
            }
        }

        if(IsMouseButtonUp(MOUSE_BUTTON_LEFT) && holding)
        {
            holding = false;
            std::cout << "HIT" << std::endl;
            forces = Vector2Scale(Vector2Add(forces, Vector2Subtract(ball[0].position, GetMousePosition())), 1000);
        }

        // calculate ball acceleration
        ball[0].acceleration = Vector2Scale(forces, ball[0].inverse_mass);

        accumulator += delta_time;
        while(accumulator >= TIMESTEP) {
            for(int i=0; i < 9; i++) {
                calculatePhysics(pBall[i], delta_time);
                    for (int j=0; j < 9; j++) {
                        
                        // check self-collision
                        if (i!=j) {

                            // check ball collision

                            // cue ball and colored ball
                            if (i==0 && (j>0 && j<5)) {
                                if (Vector2Distance(ball[i].position, ball[j].position) <= ball[i].radius + ball[j].radius && Vector2DotProduct(calculateRelativeVelocity(pBall[i], pBall[j]), calculateCollisionNormal(pBall[i], pBall[j])) < 0) {
                                    calculateBallCollision(pBall[i], pBall[j], e);
                                }
                            }
                            // cue ball and pocket
                            if (i==0 && j>4) {
                                if (Vector2Distance(ball[i].position, ball[j].position) <= ball[i].radius + ball[j].radius && Vector2DotProduct(calculateRelativeVelocity(pBall[i], pBall[j]), calculateCollisionNormal(pBall[i], pBall[j])) < 0) {
                                    // calculateObjectCollision(pBall[i], pBall[j], e);

                                    ball[i].position = {WINDOW_WIDTH/4, WINDOW_HEIGHT/2};
                                    ball[i].acceleration = Vector2Zero();
                                    ball[i].velocity = Vector2Zero();
                                }

                                // set new cue ball position manually by mouse
                            }
                            // colored ball and pocket
                            if ((i>0 && i<5) && j>4) {
                                if (Vector2Distance(ball[i].position, ball[j].position) <= ball[i].radius + ball[j].radius && Vector2DotProduct(calculateRelativeVelocity(pBall[i], pBall[j]), calculateCollisionNormal(pBall[i], pBall[j])) < 0) {
                                    ball[i].position = {10000, 10000};
                                }

                                // disappear colored ball and add point for active player
                            }
                            // colored ball and colored ball
                            if ((i>0 && i<5) && (j>0 && j<5)) {
                                if (Vector2Distance(ball[i].position, ball[j].position) <= ball[i].radius + ball[j].radius && Vector2DotProduct(calculateRelativeVelocity(pBall[i], pBall[j]), calculateCollisionNormal(pBall[i], pBall[j])) < 0) {
                                    calculateBallCollision(pBall[i], pBall[j], e);
                                }
                            }
                        }
                        calculateWallCollision(pBall[i]);
                        spring[0].spring_end = ball[0].position;
                    }
                // stop ball
                // std::cout << Vector2Length(ball[i].velocity) << std::endl;

                if (Vector2Length(ball[i].velocity) <= 4.0f) ball[i].velocity = Vector2Zero();
                if (i<5) {
                    for(int k=0; k<4; k++) {
                            
                            if (Vector2Distance(ball[i].position, calculateQ(pBorder[k], pBall[i])) + ball[i].radius <= ball[i].radius && Vector2DotProduct(calculateRelativeVelocityRect(pBorder[k], pBall[i]), calculateCollisionNormalRect(pBorder[k], pBall[i])) < 0 && Vector2Length(calculateRelativeVelocityRect(pBorder[k], pBall[i])) > 0.0f) {
                                // && Vector2DotProduct(calculateRelativeVelocityRect(pBorder[k], pBall[i]), calculateCollisionNormalRect(pBorder[k], pBall[i])) < 0)
                                std::cout << "BALL: " << i <<" COLLIDES WALL: " << k << std::endl;
                                std::cout << "BALL CENTER: " << Vector2Length(ball[i].position) << std::endl;
                                std::cout << "Q VALUE: " << Vector2Length(calculateQ(pBorder[k], pBall[i])) << std::endl;
                                std::cout << "BORDER: " << k << " POSITION: " << Vector2Length(border[k].position) << std::endl;

                                calculateRectCollision(pBorder[k], pBall[i], e);
                            }
                        border[k].velocity = Vector2Zero();
                    }
                }
            }
            accumulator -= TIMESTEP;
        }

        float spring_thick = 5;

        // DRAW
        BeginDrawing();
        ClearBackground(GREEN);
        // std::cout << "WORKS" << std::endl;

        // draw balls
        for (int i=0; i<9; i++) {
            DrawCircleV(ball[i].position, ball[i].radius, ball[i].color);
        }
        // draw borders
        for (int i=0; i<4; i++) {
            DrawRectangleV(border[i].position, border[i].size, border[i].color);
        }
        // draw cue
        // for (int i=0; i<1; i++) {
        //     // check if active player
        //     if (spring[i].active) {
        //         DrawLineEx(spring[i].spring_start, spring[i].spring_end, spring_thick, spring[i].color);
        //     }
        // }

        if (spring[0].active && holding) {
            DrawLineEx(GetMousePosition(), ball[0].position, spring_thick, BLACK);
        }

        EndDrawing();
    }

    // delete[] ball;
    // delete[] pBall;
    // delete[] border;
    // delete[] pBorder;
    // delete[] spring;
    // delete[] pSpring;

    CloseWindow();
    return 0;
}