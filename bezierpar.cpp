/******************************************************************************
 * Homework 2 - Moving Along a Curve
 * Antoporda, Banal, Labre 2024
 *
 * Input R, S, V, N, and each control point coordinate
 * note: control point coordinates input can be multiple lines or 1 line
 *
 * Drag yellow control points with LEFT mouse button
 * Press R to respawn Red rectangle
 * Press E to spawn Blue rectangle moving in opposite direction
 *
 * press SPACEBAR to toggle debug mode
 * debug mode: displays number of control point, red bezier curve points, and console log of computations for bezier points
 * note: debug mode lags tremendously on large input (i.e. sample input 2); to toggle off, press SPACEBAR again
 * In debug mode:
 * press D to add 1 step to curve
 * press A to subtract 1 step from curve
 * 
 *
 * Outputs parametric bezier curve given inputs with adjustable control points
 *****************************************************************************/

// g++ Main.cpp -o out -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm


#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>

// GLOBALS
bool error = false;
int debug = 0;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int FPS = 60;

int R, S, V, N;
float x, y;
std::vector<Vector2> points;

// STRUCTS and SETUP
struct ControlPoint
{
    Vector2 position;
    float radius;
    Color color;
    bool active;
};

struct RectObject
{
    Vector2 position, size, direction;
    Color color;
    int type;
    float speed;
};

void setControlPointProperties(ControlPoint* cp, Vector2 center, float r, Color col, bool a)
{
    cp->position = center;
    cp->radius = r;
    cp->color = col;
    cp->active = a;
}

void setRectObjectProperties(RectObject* ro, Vector2 pos, Vector2 size, Vector2 d, Color col, int t, float speed)
{
    ro->position = pos;
    ro->size = size;
    ro->direction = d;
    ro->color = col;
    ro->type = t;
    ro->speed = speed;
}

// UTILS
void ParseInput()
{
    std::cout << "Order: ";
    std::cin >> R;
    try {
        if (R > 0) {
            std::cout << "Steps: ";
            std::cin >> S;
            try {
                if (S > 0) {
                    std::cout << "Tangent and Normal per Curve: ";
                    std::cin >> V;
                    try {
                        if (V > 0) {
                            std::cout << "Number of Control Points: ";
                            std::cin >> N;
                            try {
                                float condition = (N-(R+1)) % R;
                                if(N >= 3 && condition == 0) {
                                    std::cout << "Control Point Coordinates:\n";
                                    for(int i=0; i<N; i++) {
                                        std::cin >> x >> y;
                                        Vector2 P = {x, y};
                                        points.push_back(P);
                                    }
                                }
                                else {
                                    throw(N);
                                }
                            }
                            catch (...) {
                                error = true;
                                std::cout << "Invalid number of Control Points" << std::endl;
                            }
                        }
                        else {
                            throw(V);
                        }
                    }
                    catch (...) {
                        error = true;
                        std::cout << "Invalid number of times to draw Tangents & Normals" << std::endl;
                    }
                }
                else {
                    throw(S);
                }
            }
            catch (...) {
                error = true;
                std::cout << "Invalid number of steps" << std::endl;
            }
        }
        else {
            throw(R);
        }
    }
    catch (...) {
        error = true;
        std::cout << "Invalid order" << std::endl;
    }
}

int factorial(int x)
{
    return ((x==0) ? 1 : x * factorial(x-1));
}

float DegreesToRadians(float degrees)
{
    return degrees * (PI / 180);
}

float RadiansToDegrees(float radians)
{
    return radians * (180 / PI);
}

int GetRandomType(int chance)
{
    int max = 101;
    int r = rand() % max;

    return ((r <= chance) ? 1 : 0);
}

int GetRandomType2(int chance)
{
    int max = 101;
    int r = rand() % max;

    return ((r <= chance) ? 1 : 2);
}

Vector2 lerp(Vector2 v0, Vector2 v1, float t)
{
    float new_x = v0.x + t * (v1.x - v0.x);
    float new_y = v0.y + t * (v1.y - v0.y);
    return {new_x, new_y};
}

// BEZIER
int GetPascalTriangleCoefficient(int order, int index)
{
    int numerator = factorial(order);
    int denominator = factorial(order - index);
    denominator = denominator * factorial(index);

    return numerator/denominator;
}

std::vector<int> GeneratePascalTriangleRow(int order)
{
    std::vector<int> pt;
    
    for(int i = 0; i < order + 1; i++)
    {
        pt.push_back(GetPascalTriangleCoefficient(order, i));
    }

    return pt;
}

Vector2 GetBezierPoint(int nCurves, float t, std::vector<int> ptrow)
{
    Vector2 result = Vector2Zero();

    for(int i=0; i<=R; i++)
    {
        result = Vector2Add(result, Vector2Scale(points[(nCurves*R)+i], ((float) ptrow.at(i)) * pow((1-t), (R-i)) * pow(t, i)));
        if(debug) std::cout << "ORDER: " << i << "    STEP: " << t << "    COEFF: " << ptrow.at(i) << "    RESULT: " << result.x << ", " << result.y << std::endl;
    }

    return result;
}

std::vector<Vector2> ComputeBezier(int n_Curves, std::vector<Vector2> p, std::vector<int> pt, float S)
{
    std::vector<Vector2> d;

    for(int i=0; i<n_Curves; i++)
    {
        for(int j=0; j<(S+1); j++)
        {
            float step = (j * 1.0f) / (S * 1.0f);
            Vector2 point_Curve = GetBezierPoint(i, step, pt);
            d.push_back(point_Curve);

            if(debug) std::cout << "Bezier Point on Curve " << i << ": " << point_Curve.x << ", " << point_Curve.y << std::endl;
        }
    }

    return d;
}

Vector2 GetBezierDerivative(int nCurves, float t, std::vector<int> ptrow)
{
    Vector2 result = Vector2Zero();

    for(int i=0; i <= R - 1; i++)
    {
        result = Vector2Add(result, Vector2Scale(Vector2Subtract(points[(nCurves*R)+i+1], points[(nCurves*R)+i]), ((float) ptrow.at(i)) * pow((1-t), (R-i)) * pow(t, i)));
        if(debug) std::cout << "[DERIVATIVE] ORDER: " << i << "    STEP: " << t << "    COEFF: " << ptrow.at(i) << "    RESULT: " << result.x << ", " << result.y << std::endl;
    }

    result = Vector2Normalize(result);

    return Vector2Scale(result, 50);
}

std::vector<Vector2> ComputeBezierDerivatives(int n_Curves, std::vector<Vector2> p, std::vector<int> pt)
{
    std::vector<Vector2> d;

    for(int i=0; i<n_Curves; i++)
    {
        for(int j=0; j <= V; j++)
        {
            float step = (j * 1.0f) / (V * 1.0f);
            Vector2 point_Curve = GetBezierDerivative(i, step, pt);
            d.push_back(point_Curve);

            if(debug) std::cout << "Bezier Tangent on Curve " << i << ": " << point_Curve.x << ", " << point_Curve.y << std::endl;
        }
    }

    return d;
}

std::vector<Vector2> ComputeTangentPoints(int n_Curves, std::vector<Vector2> p, std::vector<int> pt)
{
    std::vector<Vector2> d;

    for(int i=0; i<n_Curves; i++)
    {
        for(int j=0; j<(V+1); j++)
        {
            float step = (j * 1.0f) / (V * 1.0f);
            Vector2 point_Curve = GetBezierPoint(i, step, pt);
            d.push_back(point_Curve);

            if(debug) std::cout << "Tangent Point on Curve " << i << ": " << point_Curve.x << ", " << point_Curve.y << std::endl;
        }
    }

    return d;
}

std::vector<Vector2> ComputeNormals(std::vector<Vector2> t)
{
    std::vector<Vector2> d;

    for(int i = 0; i < t.size(); i++)
    {
        d.push_back(Vector2Rotate(t.at(i), DegreesToRadians(-90)));
    }

    return d;
}

int main() {
    std::vector<Vector2> draw, MoveRect;
    std::vector<int> ptCoefficients;

    // ptcoefficients for derivative
    std::vector<int> ptCoefficientsDer;
    std::vector<Vector2> tangents;
    std::vector<Vector2> tangentPoints;
    std::vector<Vector2> normals;
    float delta_time;

    ParseInput();
    if (error) return 0;
    else
    {
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "AntipordaBanalLabre_Homework02");
        SetTargetFPS(FPS);
    }

    ControlPoint cp[N];
    ControlPoint* pCP[N];
    for(int i=0; i<N; i++) {
        pCP[i] = &cp[i];
        setControlPointProperties(pCP[i], points[i], 10.0f, YELLOW, false);
    }

    int n_Curves = (N - 1) / R;
    ptCoefficients = GeneratePascalTriangleRow(R);
    ptCoefficientsDer = GeneratePascalTriangleRow(R - 1); 
    draw = ComputeBezier(n_Curves, points, ptCoefficients, S);

    MoveRect = ComputeBezier(n_Curves, points, ptCoefficients, 100);


    // compute tangents on the curve
    tangents = ComputeBezierDerivatives(n_Curves, points, ptCoefficientsDer);
    tangentPoints = ComputeTangentPoints(n_Curves, points, ptCoefficients);

    // compute normals on the curve
    normals = ComputeNormals(tangents);

    // Index of Active Control Point
    int activeCp;

    // Set active Control point out of range
    activeCp = -1;

    // Create rectangle object
    RectObject rectangle;
    RectObject* pRectangle;

    pRectangle = &rectangle;

    setRectObjectProperties(pRectangle, cp[0].position, {20, 10}, Vector2Normalize(Vector2Subtract(MoveRect[1], MoveRect[0])), RED, 0, 100);

    // extra rectangle
    RectObject SpawnRect;
    RectObject* pSpawnRect;
    pSpawnRect = &SpawnRect;

    int currentPointIndex = 0;
    int currentTangentIndex = 0;
    int reversePointIndex = draw.size()-1;
    int reverseTangentIndex = tangentPoints.size()-1;

    float timer = 0.0f;
    float timerMax = 1.0f/100;

    float reverseTimer = 0.0f;

    while(!WindowShouldClose()) {
        delta_time = GetFrameTime();

        if(timer > timerMax)
        {
            timer = 0.0f;
            currentPointIndex++;
        }
        else
        {
            timer += delta_time;
        }

        if(reverseTimer > timerMax)
        {
            reverseTimer = 0.0f;
            reversePointIndex--;
        }
        else
        {
            reverseTimer += delta_time;
        }


        // reset rectangle
        if(IsKeyPressed(KEY_R))
        {
            currentPointIndex = 0;
            currentTangentIndex = 0;
            setRectObjectProperties(pRectangle, cp[0].position, {20, 10}, Vector2Normalize(Vector2Subtract(MoveRect[1], MoveRect[0])), RED, 0, 100);
        }

        // spawn reverse
        if(IsKeyPressed(KEY_E))
        {
            reversePointIndex = MoveRect.size()-1;
            reverseTangentIndex = tangentPoints.size()-1;
            setRectObjectProperties(pSpawnRect, cp[N-1].position, {20, 10}, Vector2Normalize(Vector2Subtract(MoveRect[MoveRect.size()-2], MoveRect[MoveRect.size()-1])), BLUE, 2, 100);
        }
        
        // if normal rectangle
        if(rectangle.type == 0)
        {
            rectangle.position = MoveRect[currentPointIndex];

            if(Vector2Distance(rectangle.position, tangentPoints[currentTangentIndex+1]) <= 1.0f)
            {
                currentTangentIndex += 1;
                rectangle.type = GetRandomType(5);
            }

            // remove rectangle
            if(Vector2Distance(rectangle.position, cp[N-1].position) <= 1.0f)
            {
                rectangle.type = 3;
            }
        }

        // if flying rectangle
        if(rectangle.type == 1)
        {
            rectangle.direction = Vector2Normalize(Vector2Subtract(Vector2Add(normals[currentTangentIndex], tangentPoints[currentTangentIndex]), tangentPoints[currentTangentIndex]));
            rectangle.position = Vector2Add(rectangle.position, Vector2Scale(Vector2Scale(rectangle.direction, rectangle.speed), delta_time));

            // remove rectangle
            if(rectangle.position.x<0 || rectangle.position.x>WINDOW_WIDTH || rectangle.position.y<0 || rectangle.position.y>WINDOW_HEIGHT) rectangle.type = 3;
        }
        if(SpawnRect.type == 1)
        {
            SpawnRect.direction = Vector2Normalize(Vector2Subtract(Vector2Add(normals[reverseTangentIndex], tangentPoints[reverseTangentIndex]), tangentPoints[reverseTangentIndex]));
            SpawnRect.position = Vector2Add(SpawnRect.position, Vector2Scale(Vector2Scale(SpawnRect.direction, SpawnRect.speed), delta_time));

            // remove rectangle
            if(SpawnRect.position.x<0 || SpawnRect.position.x>WINDOW_WIDTH || SpawnRect.position.y<0 || SpawnRect.position.y>WINDOW_HEIGHT) SpawnRect.type = 3;
        }

        // if reverse rectangle
        if(SpawnRect.type == 2)
        {
            SpawnRect.position = MoveRect[reversePointIndex];

            if(Vector2Distance(SpawnRect.position, tangentPoints[reverseTangentIndex-1]) <= 1.0f)
            {
                reverseTangentIndex -= 1;
                SpawnRect.type = GetRandomType2(5);
            }

            // remove rectangle
            if(Vector2Distance(SpawnRect.position, cp[0].position) <= 1.0f)
            {
                SpawnRect.type = 3;
            }
        }

        // enable debug mode
        if(IsKeyPressed(KEY_SPACE)) debug = (debug==0) ? 1:0;

        // Check Control Point Collision and Active Control Point
        for(int i=0; i<N; i++) {
            if(CheckCollisionPointCircle(GetMousePosition(), cp[i].position, cp[i].radius)) {
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) activeCp = i;
                if(IsMouseButtonUp(MOUSE_BUTTON_LEFT)) activeCp = -1;
            }

            for(int j=0; j<N; j++) {
                float distance = Vector2Distance(cp[i].position, cp[j].position);
                if((i != j ) && (distance <= cp[i].radius + cp[j].radius)) {
                    if(cp[i].position.x <= cp[j].position.x) cp[j].position.x += 100*delta_time;
                    if(cp[i].position.x >= cp[j].position.x) cp[j].position.x -= 100*delta_time;
                    if(cp[i].position.y <= cp[j].position.y) cp[j].position.y += 100*delta_time;
                    if(cp[i].position.y >= cp[j].position.y) cp[j].position.y -= 100*delta_time;
                    points[j] = cp[j].position;
                }
            }
        }

        // Move Active Control Point
        if(activeCp >= 0) {
                cp[activeCp].position = GetMousePosition();
                points[activeCp] = cp[activeCp].position;
                draw = ComputeBezier(n_Curves, points, ptCoefficients, S);
                tangents = ComputeBezierDerivatives(n_Curves, points, ptCoefficientsDer);
                tangentPoints = ComputeTangentPoints(n_Curves, points, ptCoefficients);
                normals = ComputeNormals(tangents);
                MoveRect = ComputeBezier(n_Curves, points, ptCoefficients, 100);


                if (GetMousePosition().x >= WINDOW_WIDTH) cp[activeCp].position.x = WINDOW_WIDTH - cp[activeCp].radius;
                if (GetMousePosition().x <= 0) cp[activeCp].position.x = 0 + cp[activeCp].radius;
                if (GetMousePosition().y >= WINDOW_HEIGHT) cp[activeCp].position.y = WINDOW_HEIGHT - cp[activeCp].radius;
                if (GetMousePosition().y <= 0) cp[activeCp].position.y = 0 + cp[activeCp].radius;
        }

        // Debug Mode Utilities
        if(debug) {
            // Adjust Step Count
            if(IsKeyPressed(KEY_D)) {
                S++;
                draw = ComputeBezier(n_Curves, points, ptCoefficients, S);
                tangents = ComputeBezierDerivatives(n_Curves, points, ptCoefficientsDer);
                tangentPoints = ComputeTangentPoints(n_Curves, points, ptCoefficients);
                normals = ComputeNormals(tangents);
                MoveRect = ComputeBezier(n_Curves, points, ptCoefficients, 100);
            }
            if(IsKeyPressed(KEY_A)) {
                S--;
                draw = ComputeBezier(n_Curves, points, ptCoefficients, S);
                tangents = ComputeBezierDerivatives(n_Curves, points, ptCoefficientsDer);
                tangentPoints = ComputeTangentPoints(n_Curves, points, ptCoefficients);
                normals = ComputeNormals(tangents);
                MoveRect = ComputeBezier(n_Curves, points, ptCoefficients, 100);
            }
        }

        // DRAW
        BeginDrawing();
        ClearBackground(BLACK);

        for(int i=0; i<N; i++) {
            DrawCircleV(cp[i].position, cp[i].radius, cp[i].color);
        }

        for(int i=0; i<draw.size() - 1; i++) {
            DrawLineV(draw[i], draw[i+1], WHITE);
        }

        for(int i=0; i<tangents.size(); i++) {
            DrawLineV(tangentPoints[i], Vector2Add(tangentPoints[i], tangents[i]), BLUE);
        }

        for(int i=0; i<normals.size(); i++) {
            DrawLineV(tangentPoints[i], Vector2Add(tangentPoints[i], normals[i]), PURPLE);
        }

        // draw normal rectangle
        if(rectangle.type == 0)
        {
            DrawRectanglePro({rectangle.position.x, rectangle.position.y, rectangle.size.x, rectangle.size.y}, {10, 5}, RadiansToDegrees(Vector2Angle(rectangle.direction, tangents[currentTangentIndex])), RED);
        }

        // draw flying rectangle
        if(rectangle.type == 1)
        {
            DrawRectanglePro({rectangle.position.x, rectangle.position.y, rectangle.size.x, rectangle.size.y}, {10, 5}, RadiansToDegrees(Vector2Angle(rectangle.direction, normals[currentTangentIndex])), RED);
        }
        if(SpawnRect.type == 1)
        {
            DrawRectanglePro({SpawnRect.position.x, SpawnRect.position.y, SpawnRect.size.x, SpawnRect.size.y}, {10, 5}, RadiansToDegrees(Vector2Angle(SpawnRect.direction, normals[reverseTangentIndex])), BLUE);
        }

        // draw reverse rectangle
        if(SpawnRect.type == 2)
        {
            DrawRectanglePro({SpawnRect.position.x, SpawnRect.position.y, SpawnRect.size.x, SpawnRect.size.y}, {10, 5}, RadiansToDegrees(Vector2Angle(SpawnRect.direction, tangents[reverseTangentIndex])), BLUE);
        }

        if(debug) {
            // Draw Control Point Number
            for(int i=0; i<N; i++) {
                DrawText(std::to_string(i).c_str(), cp[i].position.x - (cp[i].radius * std::cos(45)), cp[i].position.y - (cp[i].radius * std::sin(45)), 20, BLUE);
            }

            // Draw computed Bezier Points
            for(auto i=draw.begin(); i!=draw.end(); ++i) {
                Vector2 pos = *i;
                DrawCircleV(pos, 2.0f, RED);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}