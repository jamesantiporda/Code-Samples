/******************************************************************************
 * Homework 1 - Cameras
 * Antiporda, Banal, Labre, 2024
 *
 * Input filename of settings file on console (i.e. settings.txt, config.ini, etc.)
 * settings format:
 *      image file
 *      object 1 name and position
 *      object 2 name and position
 *      object 3 name and position
 *      object 4 name and position
 *      object 5 name and position
 *      camera edges
 *      camera drift
 * Press spacebar to toggle hacks/hints (toggles object hitbox and camera boundary)
 * Press F to forcefully finish even if not done
 *
 * Note: Yellow Box is a dead zone (if mouse is in dead zone, camera will not move)
 *****************************************************************************/

// g++ hw.cpp -o out -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm


#include <raylib.h>
#include <raymath.h>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// declare window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float FPS = 60;
const float TIMESTEP = 1 / FPS; // Sets the timestep to 1 / FPS. But timestep can be any ver small value.

std::string img_name;
std::vector<std::string> obj_name;
std::vector<Vector2> obj_pos, cam_edges;
float camwin_width, camwin_height, cam_drift;

struct Text {
    std::string text;
    Color color;
    bool status;
};

std::vector<std::string> split(std::string s)
{
    std::vector<std::string> str;
    std::string element = "";

    for(int i=0; i<s.length(); i++)
    {
        std::string comp;
        if (s[i]) {
            comp += s[i];
            if (comp != " ")
            {
                element += s[i];
            }
            else
            {
                str.push_back(element);
                element = "";
            }
            if (i == s.length()-1)
            {
                str.push_back(element);
                element = "";
            }
        }
    }

    return str;
}

void parseFile(std::string filename)
{
    std::string in, ce, cd, cdrift;
    std::string ob[5];
    std::vector<std::string> to_split;

    std::fstream MyReadFile(filename);
        getline(MyReadFile, in);
        getline(MyReadFile, ob[0]);
        getline(MyReadFile, ob[1]);
        getline(MyReadFile, ob[2]);
        getline(MyReadFile, ob[3]);
        getline(MyReadFile, ob[4]);
        getline(MyReadFile, ce);
        getline(MyReadFile, cd);
        getline(MyReadFile, cdrift);
    MyReadFile.close();

    // IMAGE NAME
    img_name = in;

    // 5 OBJECTS
    for(int i=0; i<5; i++)
    {
        to_split = split(ob[i]);
        Vector2 pos = {std::stof(to_split[1]), std::stof(to_split[2])};

        obj_name.push_back(to_split[0]);
        obj_pos.push_back(pos);
    }

    // CAMERA EDGES
    to_split = split(ce);
    Vector2 min = {std::stof(to_split[0]), std::stof(to_split[1])};
    Vector2 max = {std::stof(to_split[2]), std::stof(to_split[3])};
    cam_edges.push_back(min);
    cam_edges.push_back(max);

    // CAMERA WINDOW
    to_split = split(cd);
    camwin_width = std::stof(to_split[0]);
    camwin_height = std::stof(to_split[1]);

    // CAMERA DRIFT
    cam_drift = std::stof(cdrift);
}

void CameraMoveTowardsCursor(Camera2D *camera, Vector2 cursorPosition, float speed, float deltatime)
{
    camera->target = Vector2Add(camera->target, Vector2Scale(Vector2Scale(Vector2Normalize(Vector2Subtract(cursorPosition, camera->target)), speed * 0.75), deltatime));
}

bool OutsideBoundingBox(Camera2D camera, Vector2 objectPosition, Vector2 boundingBox)
{
    if(camera.target.x - objectPosition.x + 10 >= boundingBox.x/2 || camera.target.x - objectPosition.x - 10 <= -boundingBox.x/2 || camera.target.y - objectPosition.y + 10 >= boundingBox.y/2 || camera.target.y - objectPosition.y - 10 <= -boundingBox.y/2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main() {
    std::string filename, arg;

    std::cout << "Input filename (default is settings.txt): ";
    if(std::getline(std::cin, arg))
    {
        if(arg=="") filename="settings.txt";
        else filename = arg;
    }

    parseFile(filename);
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "AntipordaBanalLabre_Homework01");
    SetTargetFPS(FPS);

    Texture2D background = LoadTexture(img_name.c_str());

    // PRINT CONFIG
    std::cout << "IMG: " << img_name << std::endl;
    for(int i=0; i<5; i++) {
        std::cout << "OBJECT " << i << ": " << obj_name[i] << "    at    " << obj_pos[i].x << ", " << obj_pos[i].y << std::endl;
    }

    std::cout << "CAM EDGE X: " << cam_edges[0].x << ", " << cam_edges[0].y << std::endl;
    std::cout << "CAM EDGE Y: " << cam_edges[1].x << ", " << cam_edges[1].y << std::endl;

    std::cout << "CAM DIMENSION: " << camwin_width << " x " << camwin_height << std::endl;
    std::cout << "TOKYO DRIFT: " << cam_drift << std::endl;
    

    // for initializing the different
    // camera types
    int cam_type;
    // to initialize the camera view
    // this is where we set view points
    Camera2D camera_view = { 0 };

    // the camera-edge coordinates
    // EDGE_X[0] = left, EDGE_Y[0] = top
    float EDGE_X[2], EDGE_Y[2];

    // DEFAULT (-200, 1000) x (0, 500)
    EDGE_X[0] = -200.0f;
    EDGE_X[1] = 1000.0f;
    EDGE_Y[0] = 0.0f;
    EDGE_Y[1] = 500.0f;
    if(!cam_edges.empty()) {
        EDGE_X[0] = cam_edges[0].x;
        EDGE_X[1] = cam_edges[0].y;
        EDGE_Y[0] = cam_edges[1].x;
        EDGE_Y[1] = cam_edges[1].y;
    }

    Vector2 playerPosition = {WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
    float playerSpeed = 100;
    Vector2 playerDirection = {0, 0};
    Vector2 playerVelocity = Vector2Scale(playerDirection, playerSpeed);

    Vector2 deadZoneDimensions = {50, 50};
    Vector2 cameraBoxDimensions = {200, 200};
    if(camwin_width && !camwin_height) cameraBoxDimensions.x = camwin_width;
    else if(camwin_height && !camwin_width) cameraBoxDimensions.y = camwin_height;
    else if(camwin_width && camwin_height) cameraBoxDimensions = {camwin_width, camwin_height};

    // camera’s focus
    camera_view.target = playerPosition;
    // camera’s offset
    camera_view.offset = {WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
    // camera’s zoom
    camera_view.zoom = 1.0f;

    Text objects[5];
    for(int i=0; i<5; i++)
    {
        objects[i].text = obj_name[i];
        objects[i].color = BLACK;
        objects[i].status = false;
    }

    bool isMoveable = true;
    int game_state = 0;
    int score = 0;

    float timer = 0;

    SetMousePosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    while(!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        playerDirection = {0, 0};

        if(IsKeyPressed(KEY_SPACE))
        {
            if(game_state==0) game_state=1;
            else if(game_state==1) game_state=0;
        }

        if(score == 5 || IsKeyPressed(KEY_F))
        {
            game_state = 2;
        }

        if(camera_view.target.x < EDGE_X[0] || camera_view.target.x > EDGE_X[1])
        {
            camera_view.target.x = Clamp(camera_view.target.x, EDGE_X[0], EDGE_X[1]);
        }

        if(camera_view.target.y < EDGE_Y[0] || camera_view.target.y > EDGE_Y[1])
        {
            camera_view.target.y = Clamp(camera_view.target.y, EDGE_Y[0], EDGE_Y[1]);
        }

        if(OutsideBoundingBox(camera_view, playerPosition, deadZoneDimensions) && isMoveable)
        {
            CameraMoveTowardsCursor(&camera_view, playerPosition, (playerSpeed*cam_drift), delta_time);
        }

        if(OutsideBoundingBox(camera_view, playerPosition, cameraBoxDimensions) && isMoveable)
        {
            playerSpeed = 200;
        }
        else
        {
            playerSpeed = 100;
        }

        if(isMoveable==false)
        {
            timer+=delta_time;

            for(int i=0; i<5; i++)
            {
                if(CheckCollisionPointCircle(playerPosition, obj_pos[i], 100.0f)) {
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && timer >= 0.2) {
                        // std::cout << playerPosition.x << ", " << playerPosition.y << std::endl;
                        objects[i].color = PINK;
                        objects[i].status = true;
                    }
                }
            }
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if(isMoveable)
            {
                camera_view.zoom = 2.0f;
                isMoveable = false;
                //std::cout << camera_view.zoom << std::endl;
            }
            //std::cout << camera_view.zoom << std::endl;

            //if(!isMoveable) std::cout << playerPosition.x << ", " << playerPosition.y << std::endl;
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if(!isMoveable)
            {
                camera_view.zoom = 1.0f;
                isMoveable = true;
                timer = 0;
            }
        }
        

        score = 0;
        for(int i=0; i<5; i++)
        {
            if(objects[i].status) score++;
        }

        playerPosition = GetScreenToWorld2D(GetMousePosition(), camera_view);
        playerVelocity = Vector2Scale(playerDirection, playerSpeed);
        playerPosition = Vector2Add(playerPosition, Vector2Scale(playerVelocity, delta_time));

        // DRAW
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera_view);
        //Draw Things Here
        DrawTextureEx(background, {-400, -225}, 0.0f, 2.0f, WHITE);
        
        for(auto i=obj_pos.begin(); i!=obj_pos.end(); ++i)
        {
            Vector2 pos = *i;
            DrawCircleV(pos, 100.0f, {0, 0, 255, 0});
        }

        if (game_state == 1)
        {
            for(auto i=obj_pos.begin(); i!=obj_pos.end(); ++i)
            {
                Vector2 pos = *i;
                DrawCircleV(pos, 100.0f, {0, 0, 255, 50});
            }
            DrawRectangleLines(EDGE_X[0],EDGE_Y[0],abs(EDGE_X[0])+abs(EDGE_X[1]),abs(EDGE_Y[0])+abs(EDGE_Y[1]),YELLOW);
        }

        DrawRectangleV({playerPosition.x-5.0f, playerPosition.y-5.0f}, {10.0f, 10.0f}, RED);

        EndMode2D();


        if(isMoveable)
        {
            DrawRectangleLines(WINDOW_WIDTH/2 - deadZoneDimensions.x/2, WINDOW_HEIGHT/2 - deadZoneDimensions.y/2, deadZoneDimensions.x, deadZoneDimensions.y, YELLOW);
            DrawRectangleLines(WINDOW_WIDTH/2 - cameraBoxDimensions.x/2, WINDOW_HEIGHT/2 - cameraBoxDimensions.y/2, cameraBoxDimensions.x, cameraBoxDimensions.y, WHITE);
        }

        for(int i=0; i<5; i++)
        {
            DrawText(objects[i].text.c_str(), 10, (i*25)+10, 25, objects[i].color);
        }

        if(game_state == 2)
        {
            DrawRectangleV({0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT}, RAYWHITE);
            if(score==5)
            {
                std::string sc = std::to_string(score) + " / 5";
                DrawText("YOU WIN!", (WINDOW_WIDTH/2 - (MeasureText("YOU WIN!", 100))/2), (WINDOW_HEIGHT/2) - 100, 100, BLUE);
                DrawText(sc.c_str(), (WINDOW_WIDTH/2 - (MeasureText(sc.c_str(), 50))/2), (WINDOW_HEIGHT/2) + 100, 50, BLACK);
            }
            else
            {
                std::string sc = std::to_string(score) + " / 5";
                DrawText("YOU LOSE", (WINDOW_WIDTH/2 - (MeasureText("YOU LOSE", 100))/2), (WINDOW_HEIGHT/2) - 100, 100, RED);
                DrawText(sc.c_str(), (WINDOW_WIDTH/2 - (MeasureText(sc.c_str(), 50))/2), (WINDOW_HEIGHT/2) + 100, 50, BLACK);
            }
        }
        
        EndDrawing();

    }

    CloseWindow();
    return 0;
}