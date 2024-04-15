// THE MEOWERS
// James Christian P. Antiporda, 190299
// Jose Vernon A. Banal Jr., 190516
// May 2, 2023
// We certify that this submission complies with the DISCS Academic Integrity
// Policy.
// If we have discussed our C++ language code with anyone other than
// our instructor(s), our groupmate(s), the teaching assistant(s),
// the extent of each discussion has been clearly noted along with a proper
// citation in the comments of our program.
// If any C++ language code or documentation used in our program
// was obtained from another source, either modified or unmodified, such as a
// textbook, website, or another individual, the extent of its use has been
// clearly noted along with a proper citation in the comments of my/our program.
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////
// https://learnopengl.com/Advanced-OpenGL/Blending
// https://www.youtube.com/watch?v=21UsMuFTN0k&ab_channel=ThinMatrix
// https://www.youtube.com/watch?v=0NH9k4zTAqk&t=6s&ab_channel=ThinMatrix
// https://www.youtube.com/watch?v=GADTasvDOX4&t=422s&ab_channel=ThinMatrix
// https://learning.oreilly.com/library/view/opengl-build/9781788296724/ch03s04.html
// https://learnopengl.com/Advanced-OpenGL/Framebuffers
// Minecraft Textures from Minecraft 1.19.3 - https://www.minecraft.net/en-us
// https://static.wixstatic.com/media/38ebb6_50c433eba3864b91b73979c9028d21c2~mv2.jpg/v1/fill/w_261,h_261,q_90/38ebb6_50c433eba3864b91b73979c9028d21c2~mv2.jpg
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////

// Controls
// Movement: WASD
// Looking Around: Mouse
// Place Torch/Recall Torch: F key
// Ambient Light intensity: T and Y keys
// Specular Shininess: G and H keys
// Attenuation: < and > keys to switch between attenuation distances
// Toggle Shadows: Spacebar
// Shadow Sample Amount: 9 and 0 keys
// Shadow Sample Area: 7 and 8 keys

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Minecraft with Shaders"
GLFWwindow *pWindow;

// Movement variables
const float movementSpeed = 5.0f;

// =========== CONE SETUP =========== //
const int sides = 32;
const float length = 4.0f;
float segmentSize = 1.0f;

// model
float verticesCube[] =
    {
        // position (x, y, z)    normal (x, y, z)     tangent (x, y, z)    texture coordinates (s, t)

        // ground plane
        -4.00f, 0.00f, 4.00f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        4.00f, 0.00f, 4.00f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        4.00f, 0.00f, -4.00f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -4.00f, 0.00f, 4.00f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        4.00f, 0.00f, -4.00f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -4.00f, 0.00f, -4.00f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // // cube top
        // -1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        //  1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        //  1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        // -1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        //  1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        // -1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // // cube bottom
        // -1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        //  1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        //  1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        // -1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        //  1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        // -1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // // cube front
        // -1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        //  1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        //  1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        // -1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        //  1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        // -1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // // cube back
        //  1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        // -1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        // -1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        //  1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        // -1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        //  1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // // cube right
        //  1.00f, -1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        //  1.00f, -1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        //  1.00f,  1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        //  1.00f, -1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        //  1.00f,  1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        //  1.00f,  1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        // // cube left
        // -1.00f, -1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        // -1.00f, -1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        // -1.00f,  1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        // -1.00f, -1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        // -1.00f,  1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        // -1.00f,  1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f
};

// Cube Model
float vertices[] =
    {
    // cube top
        -1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // cube bottom
        -1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // cube front
        -1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // cube back
         1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // cube right
         1.00f, -1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         1.00f, -1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         1.00f,  1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         1.00f, -1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         1.00f,  1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         1.00f,  1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        // cube left
        -1.00f, -1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        -1.00f, -1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        -1.00f,  1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -1.00f, -1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        -1.00f,  1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -1.00f,  1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f
};


// Torch Model
float verticesTorch[] =
    {
    // cube top
        -1.00f,  3.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.8f,
         1.00f,  3.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.8f,
         1.00f,  3.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f,  3.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.8f,
         1.00f,  3.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f,  3.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // cube bottom
        -1.00f, -3.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.00f, -3.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         1.00f, -3.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f, -3.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.00f, -3.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f, -3.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // cube front
        -1.00f, -3.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.00f, -3.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         1.00f,  3.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f, -3.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.00f,  3.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.00f,  3.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // cube back
         1.00f, -3.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -1.00f, -3.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -1.00f,  3.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         1.00f, -3.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -1.00f,  3.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         1.00f,  3.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // cube right
         1.00f, -3.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         1.00f, -3.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         1.00f,  3.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         1.00f, -3.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         1.00f,  3.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         1.00f,  3.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        // cube left
        -1.00f, -3.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        -1.00f, -3.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        -1.00f,  3.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -1.00f, -3.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        -1.00f,  3.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -1.00f,  3.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f
};

const int vertices2Count = (sides + 1) * 33;
float vertices2[vertices2Count * 2];

glm::vec3 coneVertsOnly[sides + 1];
glm::vec3 coneUnderVertsOnly[sides + 1];

// OpenGL object IDs
GLuint vao, vao2, vaoCube, vaoTorch;
GLuint vbo, vbo2, vboCube, vboTorch;
GLuint shader;
GLuint texture[17];

// helper struct for defining spherical polar coordinates
struct polar
{
    float radius = 8.0f;        // distance from the origin
    float inclination = -20.0f; // angle on the YZ vertical plane
    float azimuth = 45.0f;      // angle on the XZ horizontal plane

    // sanity ranges to prevent strange behavior like flipping axes, etc.
    // (you can change these as you see fit)
    static constexpr float minRadius = 0.1f;
    static constexpr float maxRadius = 20.0f;
    static constexpr float minInclination = -89.0f;
    static constexpr float maxInclination = 89.0f;

    // restricts the coordinates to sanity ranges
    void clamp()
    {
        if (radius < minRadius)
            radius = minRadius;
        if (radius > maxRadius)
            radius = maxRadius;
        if (inclination < minInclination)
            inclination = minInclination;
        if (inclination > maxInclination)
            inclination = maxInclination;
    }

    // converts the spherical polar coordinates to a vec3 in Cartesian coordinates
    glm::vec3 toCartesian()
    {
        glm::mat4 mat = glm::mat4(1.0f); // set to identity first
        mat = glm::rotate(mat, glm::radians(azimuth), glm::vec3(0.0f, 1.0f, 0.0f));
        mat = glm::rotate(mat, glm::radians(inclination), glm::vec3(1.0f, 0.0f, 0.0f));
        return mat * glm::vec4(0.0f, 0.0f, radius, 1.0f);
    }
};

struct cameraStruct
{
    // position is where your eye is
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    // front is where you're looking at (relative to position)
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    // up is just always (0, 1, 0) for simplicity
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = -90.0f; // manipulates the front vector
    float pitch = 0.0f; //
};

// variables for tracking camera and light position
cameraStruct camera;
cameraStruct reflectionCamera;

float lastXPos = WINDOW_WIDTH/2, lastYPos = WINDOW_WIDTH/2;

glm::vec3 lightPosition = glm::vec3(0.1f, 5.0f, 0.0f);
glm::vec3 lightPlacedPosition;
double previousTime = 0.0;
bool lightPlaced = false;

// variables for tracking spotlight values
// glm::vec3 spotlightDirection = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
// float innerAngle = 20.0f;
// float outerAngle = 30.0f;


float uniformAmbientIntensity = 0.15f;
float specularShininess = 128.0f;

float linearCoeffs[7] = {0.7f, 0.35f, 0.22f, 0.14f, 0.09f, 0.07f, 0.045f};
float quadraticCoeffs[7] = {1.8f, 0.44f, 0.20f, 0.07f, 0.032f, 0.017f, 0.0075f};
int currentCoeff = 4;
float currentLinearCoeff = linearCoeffs[currentCoeff];
float currentQuadraticCoeff = quadraticCoeffs[currentCoeff];
bool drawShadows = true;

///////////////////////////////////////////////////////////////////////////////
// REFLECTION CODE

GLuint reflectionFbo;     // reflection framebuffer object
GLuint reflectionTexture; // reflection texture
GLuint reflectionRbo;
GLuint reflectionShader; // shadow map shader

bool setupReflectionFrameBuffer()
{
    // create the FBO for rendering reflections
    glGenFramebuffers(1, &reflectionFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, reflectionFbo);
    glGenRenderbuffers(1, &reflectionRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, reflectionRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, WINDOW_WIDTH, WINDOW_HEIGHT);

    // attach a texture object to the framebuffer
    glGenTextures(1, &reflectionTexture);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WINDOW_WIDTH, WINDOW_HEIGHT,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, reflectionRbo);
    

    // check if we did everything right
    if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "REFLECTION Could not create custom framebuffer.\n";
        return false;
    }

    // load the shader program for drawing the shadow map
    reflectionShader = gdevLoadShader("waterref.vs", "waterref.fs");
    if (!reflectionShader)
        return false;

    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // set the texture back to the default texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

glm::vec4 clipPlane = glm::vec4(0.0, 1.0, 0.0, 0.5);

bool renderReflection()
{
    // use the shadow framebuffer for drawing the reflection map
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFbo);

    // the viewport should be the size of the reflection map
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // clear the shadow map
    // (we don't have a color buffer attachment, so no need to clear that)
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    // using the shadow map shader...
    glUseProgram(reflectionShader);

    // glm::vec3 mirrorPos = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 mirrorNormal = glm::vec3(0.0f, 1.0f, 0.0f);

    glUniform4f(glGetUniformLocation(reflectionShader, "plane"), clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);

    // ... set up the projection matrix...
    glm::mat4 projectionTransform;
    projectionTransform = glm::perspective(glm::radians(45.0f),                 // fov
                                           (float)WINDOW_WIDTH / WINDOW_HEIGHT, // aspect ratio
                                           0.1f,                                // near plane
                                           100.0f);                             // far plane
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "projectionTransform"),
                       1, GL_FALSE, glm::value_ptr(projectionTransform));

    // ... set up the view matrix...
    glm::mat4 viewTransform;
    viewTransform = glm::lookAt(reflectionCamera.position,               // eye position
                                reflectionCamera.position + reflectionCamera.front,  // center position
                                glm::vec3(0.0f, 1.0f, 0.0f)); // up vector
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "viewTransform"),
                       1, GL_FALSE, glm::value_ptr(viewTransform));
    
    // // ... set up the model matrix...
    // glm::mat4 modelTransform = glm::mat4(1.0f); // set to identity first
    // modelTransform = glm::translate(modelTransform,
    //                                 glm::vec3(0.0f, 0.0f, 0.0f)); // translate xyz
    // modelTransform = glm::rotate(modelTransform,
    //                              glm::radians(270.0f),
    //                              glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    // modelTransform = glm::scale(modelTransform,
    //                             glm::vec3(1.0f, -1.0f, -1.0f)); // scale x and y
    // glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
    //                    1, GL_FALSE, glm::value_ptr(modelTransform));


    // // ... then draw our triangles
    // glBindVertexArray(vao2);
    // glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // ... set up the model matrix... (just identity for this demo)
    // ... set up the model matrix...
    // glm::mat4 modelTransform = glm::mat4(1.0f); // set to identity first
    // modelTransform = glm::translate(modelTransform,
    //                                 glm::vec3(0.0f, 0.0f, 0.0f)); // translate xyz
    // modelTransform = glm::rotate(modelTransform,
    //                              glm::radians(0.0f),
    //                              glm::vec3(0.0f, 0.0f, 0.0f)); // rotate around z
    // modelTransform = glm::scale(modelTransform,
    //                             glm::vec3(1.0f, -1.0f, 1.0f)); // scale x and y
    // glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
    //                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // // ... then draw our triangles
    // glBindVertexArray(vao);
    // glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ... set up the model matrix...
    glm::mat4 modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f, 0.0f, 0.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(270.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(1.0f, 1.0f, 1.0f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // black floor
    float xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 20; i++)
    {
        // ... set up the model matrix...

        if(i % 5 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 5 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-2.0f + xDisplacement, 2.0f, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// black ceil
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 20; i++)
    {
        // ... set up the model matrix...

        if(i % 5 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 5 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-2.0f + xDisplacement, 7.0f, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r1 wall
    float yDisplacement = 0, rowCount = -1;
    xDisplacement = 0;

    for (size_t i = 0; i < 16; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        xDisplacement = (i - 4 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f + xDisplacement, 2.0f + yDisplacement, 4.0f)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // blue floor
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 8; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, 1.0f, -2.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // blue ceil out
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f  + xDisplacement, 6.0f, -2.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // blue ceil in
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f + xDisplacement, 5.0f, -4.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r2 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 16; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 4 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-2.0f, 2.0f + yDisplacement, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r3 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 10; i++)
    {
        // ... set up the model matrix...

        if(i % 2 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 2 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f, 1.0f + yDisplacement, -1.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r4 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 45; i++)
    {
        // ... set up the model matrix...

        if(i % 9 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 9 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(3.0f, 1.0f + yDisplacement, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r5 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 40; i++)
    {
        // ... set up the model matrix...

        if(i % 10 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 10 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f, 0.0f + yDisplacement, -3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r6 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 32; i++)
    {
        // ... set up the model matrix...

        if(i % 8 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 8 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(4.0f, 0.0f + yDisplacement, -6.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r7 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 15; i++)
    {
        // ... set up the model matrix...

        if(i % 5 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 5 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f, 0.0f + yDisplacement, -13.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r8 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 4 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(5.0f, 0.0f + yDisplacement, -14.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // green floor
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 48; i++)
    {
        // ... set up the model matrix...

        if(i % 12 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 12 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, 0.0f, -6.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// green ceil 1 shadow ?
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 4.0f, -8.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // green ceil 2
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 30; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 6 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 3.0f, -12.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }
    
    // water floor
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 36; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 6 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, -1.0f, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// s1 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 24; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f, -1.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // s2 wall
    yDisplacement = 0, rowCount = -1;
    xDisplacement = 0;

    for (size_t i = 0; i < 24; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        xDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f + xDisplacement, -1.0f + yDisplacement, -24.0f)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// s3 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 24; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(7.0f, -1.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// p1 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 4; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 1 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(6.0f, -1.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// p2 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 2; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 1 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(6.0f, -1.0f + yDisplacement, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// p3 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 4; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 1 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f, -1.0f + yDisplacement, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 1
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 5; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 0.0f, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 2
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 0.0f, -19.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 3
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 2; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(4.0f + xDisplacement, 0.0f, -19.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 4
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 0.0f, -22.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 5
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f + xDisplacement, 0.0f, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 5
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(4.0f + xDisplacement, 0.0f, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore 1

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(4.0f + xDisplacement, 1.0f, -23.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 1a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(3.0f + xDisplacement, 0.0f, -23.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 2

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 1.0f, -23.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 3

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 1.0f, -22.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 3b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 0.0f, -22.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 4a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 1.0f, -19.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 4b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 1.0f, -18.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 5a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(4.0f + xDisplacement, 1.0f, -18.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 5b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(5.0f + xDisplacement, 1.0f, -19.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 1.0f, -6.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 1.0f, -7.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6ac

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 5.0f, -6.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6bd

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 5.0f, -7.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 7a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(-1.0f + xDisplacement, 2.0f, -2.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 7b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(-1.0f + xDisplacement, 5.0f, -3.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
 

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 8a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 3.0f, 3.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 8b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 3.0f, 2.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 8c

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 4.0f, 3.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);


    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));


    // cone 1
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f, 6.5f, -1.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.35f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[5]);


    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // cone 2
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(-1.0f, 6.5f, 0.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
 

    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // cone 3
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(3.0f, 3.5f, -11.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.35f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
 

    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // cone 3
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(6.0f, 3.5f, -20.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.35f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    

    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // water ceiling
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 36; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 6 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, 4.0f, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // water ceiling inner left
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 6; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f, 2.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // water ceiling inner back
    yDisplacement = 0, rowCount = -1;
    xDisplacement = 0;

    for (size_t i = 0; i < 6; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        xDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f + xDisplacement, 2.0f + yDisplacement, -23.0f)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // water ceiling inner right
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 2; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(6.0f, 2.0f + yDisplacement, -21.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ... set up the model matrix... (just identity for this demo)
    modelTransform = glm::mat4(1.0f);

    if(lightPlaced)
    {
        modelTransform = glm::translate(modelTransform,
                                    lightPosition - glm::vec3(0.0, 0.5, 0.0)); // translate xyz
    }
    else
    {
        modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f, 0.0f, 0.0f)); // translate xyz
    }
    modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.1f, 0.1f, 0.1f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(reflectionShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));
    

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[14]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[15]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[16]);

    // ... then draw our triangles
    glBindVertexArray(vaoTorch);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesTorch) / (11 * sizeof(float)));


    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // before drawing the final scene, we need to set drawing to the whole window
    int width, height;
    glfwGetFramebufferSize(pWindow, &width, &height);
    glViewport(0, 0, width, height);

    // we will need the light transformation matrix again in the main rendering code
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// SHADOW MAPPING CODE

#define SHADOW_SIZE 1024
GLuint shadowMapFbo;     // shadow map framebuffer object
GLuint shadowMapTexture; // shadow map texture
GLuint shadowMapRbo;
GLuint shadowMapShader; // shadow map shader
int sampleSide = 3;
float sampleAreaSize = 1.0f;

bool setupShadowMap()
{
    // create the FBO for rendering shadows
    glGenFramebuffers(1, &shadowMapFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFbo);

    // attach a texture object to the framebuffer
    glGenTextures(1, &shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);

    // check if we did everything right
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Could not create custom framebuffer.\n";
        return false;
    }

    // load the shader program for drawing the shadow map
    shadowMapShader = gdevLoadShader("demo8s.vs", "demo8s.fs");
    if (!shadowMapShader)
        return false;

    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

glm::mat4 renderShadowMap()
{
    // use the shadow framebuffer for drawing the shadow map
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFbo);

    // the viewport should be the size of the shadow map
    glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);

    // clear the shadow map
    // (we don't have a color buffer attachment, so no need to clear that)
    glClear(GL_DEPTH_BUFFER_BIT);

    // using the shadow map shader...
    glUseProgram(shadowMapShader);

    // ... set up the light space matrix...
    // (note that if you use a spot light, the FOV and the center position
    // vector should be set to your spotlight's outer radius and focus point,
    // respectively)
    glm::mat4 lightTransform;
    lightTransform = glm::perspective(glm::radians(90.0f), // fov
                                      1.0f,                     // aspect ratio
                                      0.1f,                     // near plane
                                      100.0f);                  // far plane
    lightTransform *= glm::lookAt(lightPosition,                // eye position
                                  glm::vec3(0.0f, 0.0f, 0.0f),  // center position
                                  glm::vec3(0.0f, 1.0f, 0.0f)); // up vector
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "lightTransform"),
                       1, GL_FALSE, glm::value_ptr(lightTransform));

    glm::mat4 modelTransform = glm::mat4(1.0f); // set to identity
    
    // black floor
    float xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 20; i++)
    {
        // ... set up the model matrix...

        if(i % 5 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 5 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-2.0f + xDisplacement, 2.0f, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// black ceil
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 20; i++)
    {
        // ... set up the model matrix...

        if(i % 5 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 5 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-2.0f + xDisplacement, 7.0f, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r1 wall
    float yDisplacement = 0, rowCount = -1;
    xDisplacement = 0;

    for (size_t i = 0; i < 16; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        xDisplacement = (i - 4 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f + xDisplacement, 2.0f + yDisplacement, 4.0f)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // blue floor
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 8; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, 1.0f, -2.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // blue ceil out
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f  + xDisplacement, 6.0f, -2.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // blue ceil in
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f + xDisplacement, 5.0f, -4.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r2 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 16; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 4 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-2.0f, 2.0f + yDisplacement, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r3 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 10; i++)
    {
        // ... set up the model matrix...

        if(i % 2 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 2 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f, 1.0f + yDisplacement, -1.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r4 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 45; i++)
    {
        // ... set up the model matrix...

        if(i % 9 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 9 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(3.0f, 1.0f + yDisplacement, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r5 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 40; i++)
    {
        // ... set up the model matrix...

        if(i % 10 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 10 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f, 0.0f + yDisplacement, -3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r6 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 32; i++)
    {
        // ... set up the model matrix...

        if(i % 8 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 8 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(4.0f, 0.0f + yDisplacement, -6.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r7 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 15; i++)
    {
        // ... set up the model matrix...

        if(i % 5 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 5 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f, 0.0f + yDisplacement, -13.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r8 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 4 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(5.0f, 0.0f + yDisplacement, -14.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // green floor
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 48; i++)
    {
        // ... set up the model matrix...

        if(i % 12 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 12 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, 0.0f, -6.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// green ceil 1 shadow ?
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 4.0f, -8.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // green ceil 2
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 30; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 6 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 3.0f, -12.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }
    
    // water floor
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 36; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 6 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, -1.0f, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// s1 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 24; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f, -1.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // s2 wall
    yDisplacement = 0, rowCount = -1;
    xDisplacement = 0;

    for (size_t i = 0; i < 24; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        xDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f + xDisplacement, -1.0f + yDisplacement, -24.0f)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// s3 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 24; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(7.0f, -1.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// p1 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 4; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 1 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(6.0f, -1.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// p2 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 2; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 1 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(6.0f, -1.0f + yDisplacement, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// p3 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 4; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 1 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f, -1.0f + yDisplacement, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 1
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 5; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 0.0f, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 2
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 0.0f, -19.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 3
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 2; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(4.0f + xDisplacement, 0.0f, -19.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 4
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 0.0f, -22.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 5
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f + xDisplacement, 0.0f, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 5
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(4.0f + xDisplacement, 0.0f, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore 1

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(4.0f + xDisplacement, 1.0f, -23.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 1a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(3.0f + xDisplacement, 0.0f, -23.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 2

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 1.0f, -23.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 3

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 1.0f, -22.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 3b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 0.0f, -22.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 4a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 1.0f, -19.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 4b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 1.0f, -18.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 5a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(4.0f + xDisplacement, 1.0f, -18.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 5b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(5.0f + xDisplacement, 1.0f, -19.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 1.0f, -6.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 1.0f, -7.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6ac

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 5.0f, -6.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6bd

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 5.0f, -7.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 7a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(-1.0f + xDisplacement, 2.0f, -2.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 7b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(-1.0f + xDisplacement, 5.0f, -3.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 8a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 3.0f, 3.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 8b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 3.0f, 2.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 8c

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 4.0f, 3.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));



    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));


    // cone 1
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f, 6.5f, -1.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.35f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));


    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // cone 2
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(-1.0f, 6.5f, 0.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));


    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // cone 3
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(3.0f, 3.5f, -11.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.35f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));


    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // cone 3
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(6.0f, 3.5f, -20.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.35f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));


    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // water ceiling
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 36; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 6 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, 4.0f, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // water ceiling inner left
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 6; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f, 2.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // water ceiling inner back
    yDisplacement = 0, rowCount = -1;
    xDisplacement = 0;

    for (size_t i = 0; i < 6; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        xDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f + xDisplacement, 2.0f + yDisplacement, -23.0f)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // water ceiling inner right
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 2; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(6.0f, 2.0f + yDisplacement, -21.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));



        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }


    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // before drawing the final scene, we need to set drawing to the whole window
    int width, height;
    glfwGetFramebufferSize(pWindow, &width, &height);
    glViewport(0, 0, width, height);

    // we will need the light transformation matrix again in the main rendering code
    return lightTransform;
}

// SHADOW MAPPING CODE
///////////////////////////////////////////////////////////////////////////////

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // cone vertices making
    int vertCounter = 0;


    float textureXcoordinateA = 0.0625;
    float textureXcoordinateB = 0.0f;
    float textureXcoordinateC = 0.125;

    glm::vec3 vecA, vecB, vecC, vecNormal, vecTangent;

    float coneVertX, coneVertY, coneVertZ;
    for (int i = 0; i < sides + 1; i++)
    {
        if (i == sides)
        {
            coneVertsOnly[i] = glm::vec3(0.0f, 0.0f, length);
            continue;
        }

        coneVertX = segmentSize * cos(2 * 3.14 * i / sides);
        coneVertY = segmentSize * sin(2 * 3.14 * i / sides);
        coneVertZ = 0.0f;
        coneVertsOnly[i] = glm::vec3(coneVertX, coneVertY, coneVertZ);
    }

    for (int i = 0; i < sides + 1; i++)
    {
        if (i == sides)
        {
            coneUnderVertsOnly[i] = glm::vec3(0.0f, 0.0f, 0.0f);
            continue;
        }

        coneVertX = segmentSize * cos(2 * 3.14 * i / sides);
        coneVertY = segmentSize * sin(2 * 3.14 * i / sides);
        coneVertZ = 0.0f;
        coneUnderVertsOnly[i] = glm::vec3(coneVertX, coneVertY, coneVertZ);
    }

    for (int i = 0; i < sides * 33; i += 33)
    {
        vecA = coneVertsOnly[sides];
        vecB = coneVertsOnly[vertCounter];
        if (vertCounter == sides - 1)
        {
            vecC = coneVertsOnly[0];
        }
        else
        {
            vecC = coneVertsOnly[vertCounter + 1];
        }

        vecNormal = glm::normalize(glm::cross((vecB - vecA), (vecC - vecA)));
        vecTangent = glm::normalize(vecB - vecA);

        // vec A
        vertices2[i] = vecA.x;
        vertices2[i + 1] = vecA.y;
        vertices2[i + 2] = vecA.z;
        vertices2[i + 3] = vecNormal.x;
        vertices2[i + 4] = vecNormal.y;
        vertices2[i + 5] = vecNormal.z;
        vertices2[i + 6] = vecTangent.x;
        vertices2[i + 7] = vecTangent.y;
        vertices2[i + 8] = vecTangent.z;
        vertices2[i + 9] = textureXcoordinateA;
        vertices2[i + 10] = 1.0f;

        // vec B
        vertices2[i + 11] = vecB.x;
        vertices2[i + 12] = vecB.y;
        vertices2[i + 13] = vecB.z;
        vertices2[i + 14] = vecNormal.x;
        vertices2[i + 15] = vecNormal.y;
        vertices2[i + 16] = vecNormal.z;
        vertices2[i + 17] = vecTangent.x;
        vertices2[i + 18] = vecTangent.y;
        vertices2[i + 19] = vecTangent.z;
        vertices2[i + 20] = textureXcoordinateB;
        vertices2[i + 21] = 0.0f;

        // vec C
        vertices2[i + 22] = vecC.x;
        vertices2[i + 23] = vecC.y;
        vertices2[i + 24] = vecC.z;
        vertices2[i + 25] = vecNormal.x;
        vertices2[i + 26] = vecNormal.y;
        vertices2[i + 27] = vecNormal.z;
        vertices2[i + 28] = vecTangent.x;
        vertices2[i + 29] = vecTangent.y;
        vertices2[i + 30] = vecTangent.z;
        vertices2[i + 31] = textureXcoordinateC;
        vertices2[i + 32] = 0.0f;

        textureXcoordinateA += 0.125;
        textureXcoordinateB += 0.125;
        textureXcoordinateC += 0.125;
        vertCounter++;
    }

    vertCounter = 0;
    for (int i = (sides + 1) * 33; i < vertices2Count * 2; i += 33)
    {
        vecA = coneUnderVertsOnly[sides];
        vecC = coneUnderVertsOnly[vertCounter];
        if (vertCounter == sides - 1)
        {
            vecB = coneVertsOnly[0];
        }
        else
        {
            vecB = coneVertsOnly[vertCounter + 1];
        }

        vecNormal = glm::normalize(glm::cross((vecB - vecA), (vecC - vecA)));
        vecTangent = glm::normalize(vecB - vecA);

        // vec A
        vertices2[i] = vecA.x;
        vertices2[i + 1] = vecA.y;
        vertices2[i + 2] = vecA.z;
        vertices2[i + 3] = vecNormal.x;
        vertices2[i + 4] = vecNormal.y;
        vertices2[i + 5] = vecNormal.z;
        vertices2[i + 6] = vecTangent.x;
        vertices2[i + 7] = vecTangent.y;
        vertices2[i + 8] = vecTangent.z;
        vertices2[i + 9] = textureXcoordinateA;
        vertices2[i + 10] = 1.0f;

        // vec B
        vertices2[i + 11] = vecB.x;
        vertices2[i + 12] = vecB.y;
        vertices2[i + 13] = vecB.z;
        vertices2[i + 14] = vecNormal.x;
        vertices2[i + 15] = vecNormal.y;
        vertices2[i + 16] = vecNormal.z;
        vertices2[i + 17] = vecTangent.x;
        vertices2[i + 18] = vecTangent.y;
        vertices2[i + 19] = vecTangent.z;
        vertices2[i + 20] = textureXcoordinateB;
        vertices2[i + 21] = 0.0f;

        // vec C
        vertices2[i + 22] = vecC.x;
        vertices2[i + 23] = vecC.y;
        vertices2[i + 24] = vecC.z;
        vertices2[i + 25] = vecNormal.x;
        vertices2[i + 26] = vecNormal.y;
        vertices2[i + 27] = vecNormal.z;
        vertices2[i + 28] = vecTangent.x;
        vertices2[i + 29] = vecTangent.y;
        vertices2[i + 30] = vecTangent.z;
        vertices2[i + 31] = textureXcoordinateC;
        vertices2[i + 32] = 0.0f;

        textureXcoordinateA += 0.125;
        textureXcoordinateB += 0.125;
        textureXcoordinateC += 0.125;
        vertCounter++;
    }

    // upload the model to the GPU (explanations omitted for brevity)
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(9 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // upload the model to the GPU (explanations omitted for brevity)
    glGenVertexArrays(1, &vao2);
    glGenBuffers(1, &vbo2);
    glBindVertexArray(vao2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(9 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // upload the cube model to the GPU (explanations omitted for brevity)
    glGenVertexArrays(1, &vaoCube);
    glGenBuffers(1, &vboCube);
    glBindVertexArray(vaoCube);
    glBindBuffer(GL_ARRAY_BUFFER, vboCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(9 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // upload the torch model to the GPU (explanations omitted for brevity)
    glGenVertexArrays(1, &vaoTorch);
    glGenBuffers(1, &vboTorch);
    glBindVertexArray(vaoTorch);
    glBindBuffer(GL_ARRAY_BUFFER, vboTorch);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTorch), verticesTorch, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(9 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // load our shader program
    shader = gdevLoadShader("demo5n.vs", "demo5n.fs");
    if (!shader)
        return false;

    // since we now use multiple textures, we need to set the texture channel for each texture
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(shader, "normalMap"), 1);
    glUniform1i(glGetUniformLocation(shader, "specularMap"), 2);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "reflectionMap"), 4);

    // load our textures
    texture[0] = gdevLoadTexture("waffleicing.jpeg", GL_REPEAT, true, true);
    texture[1] = gdevLoadTexture("waffleicingnormal.png", GL_REPEAT, true, true);
    texture[2] = gdevLoadTexture("waffleicingspecularpaint.jpg", GL_REPEAT, true, true);

    texture[3] = gdevLoadTexture("stone_ore.png", GL_REPEAT, true, true);
    texture[4] = gdevLoadTexture("stone_ore_normal.png", GL_REPEAT, true, true);
    texture[5] = gdevLoadTexture("stone_ore_specular.png", GL_REPEAT, true, true);

    // Water Textures
    texture[6] = gdevLoadTexture("water_normal.jpg", GL_REPEAT, true, true);
    texture[7] = gdevLoadTexture("water_specular.png", GL_REPEAT, true, true);

    // Ores and Other Textures
    texture[8] = gdevLoadTexture("redstone_ore.png", GL_REPEAT, true, true);
    texture[9] = gdevLoadTexture("gold_ore.png", GL_REPEAT, true, true);
    texture[10] = gdevLoadTexture("iron_ore.png", GL_REPEAT, true, true);

    texture[11] = gdevLoadTexture("diamond_ore.png", GL_REPEAT, true, true);
    texture[12] = gdevLoadTexture("diamond_ore_normal.png", GL_REPEAT, true, true);
    texture[13] = gdevLoadTexture("diamond_ore_specular.png", GL_REPEAT, true, true);

    texture[14] = gdevLoadTexture("torch.png", GL_REPEAT, true, true);
    texture[15] = gdevLoadTexture("torch_normal.png", GL_REPEAT, true, true);
    texture[16] = gdevLoadTexture("torch_specular.png", GL_REPEAT, true, true);

    if (!texture[0] || !texture[1] || !texture[2] || !texture[3] || !texture[4] || !texture[5] || !texture[6] || !texture[7] || !texture[9] || !texture[10] || !texture[11] || !texture[12] || !texture[13] || !texture[14] || !texture[15] || !texture[16])
        return false;

    // enable z-buffer depth testing and face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    ///////////////////////////////////////////////////////////////////////////
    // setup shadow rendering
    if (!setupShadowMap())
        return false;
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // setup reflection rendering
    if (!setupReflectionFrameBuffer())
        return false;
    ///////////////////////////////////////////////////////////////////////////

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // find the elapsed time since the last frame
    double currentTime = glfwGetTime();
    double elapsedTime = (currentTime - previousTime);
    previousTime = currentTime;

    

    // // handle key events for camera
    // if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
    //     camera.radius -= translationAmount;
    // if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
    //     camera.radius += translationAmount;
    // if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
    //     camera.azimuth -= rotationAmount;
    // if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
    //     camera.azimuth += rotationAmount;
    // if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS)
    //     camera.inclination += rotationAmount;
    // if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)
    //     camera.inclination -= rotationAmount;
    // camera.clamp();
    // moving forward is just adding the front vector to the position vector


    // get the Cartesian coordinates of the camera
    glm::vec3 cameraPosition = camera.position;
    if(!lightPlaced)
    {
        lightPlacedPosition = cameraPosition;
        lightPosition = cameraPosition + glm::vec3(0.25f, 0.0f, 0.0f);
    }
    else
    {
        lightPosition = lightPlacedPosition;
    }

    // get a "forward" vector for controlling the light position
    // glm::vec3 lightForward = glm::normalize(glm::vec3(-cameraPosition.x, 0.0f, -cameraPosition.z));

    // if (glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_PRESS)
    //     lightPosition += lightForward * translationAmount;
    // if (glfwGetKey(pWindow, GLFW_KEY_K) == GLFW_PRESS)
    //     lightPosition -= lightForward * translationAmount;
    // if (glfwGetKey(pWindow, GLFW_KEY_J) == GLFW_PRESS)
    //     lightPosition -= glm::cross(lightForward, glm::vec3(0.0f, 1.0f, 0.0f)) * translationAmount;
    // if (glfwGetKey(pWindow, GLFW_KEY_L) == GLFW_PRESS)
    //     lightPosition += glm::cross(lightForward, glm::vec3(0.0f, 1.0f, 0.0f)) * translationAmount;
    // if (glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_PRESS)
    //     lightPosition -= glm::vec3(0.0f, 1.0f, 0.0f) * translationAmount;
    // if (glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS)
    //     lightPosition += glm::vec3(0.0f, 1.0f, 0.0f) * translationAmount;

    // // Outer Angle Controls
    // if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS)
    //     outerAngle += 1.0f;
    // if (outerAngle >= 89.0f)
    //     outerAngle = 89.0f;
    // if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS && outerAngle > innerAngle + 1.0f)
    //     outerAngle -= 1.0f;
    // if (outerAngle <= 0.0f)
    //     outerAngle = 0.0f;

    // // Inner Angle Controls
    // if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS && innerAngle < outerAngle - 1.0f)
    //     innerAngle += 1.0f;
    // if (innerAngle >= 89.0f)
    //     innerAngle = 89.0f;
    // if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
    //     innerAngle -= 1.0f;
    // if (innerAngle <= 0.0f)
    //     innerAngle = 0.0f;

    // Ambient Intensity Controls
    if (glfwGetKey(pWindow, GLFW_KEY_Y) == GLFW_PRESS)
        uniformAmbientIntensity += 0.01f;
    if (uniformAmbientIntensity >= 1.0f)
        uniformAmbientIntensity = 1.0f;
    if (glfwGetKey(pWindow, GLFW_KEY_T) == GLFW_PRESS)
        uniformAmbientIntensity -= 0.01f;
    if (uniformAmbientIntensity <= 0.0f)
        uniformAmbientIntensity = 0.0f;

    // Specular Shininess Controls
    if (glfwGetKey(pWindow, GLFW_KEY_H) == GLFW_PRESS)
        specularShininess += 1.0f;
    if (specularShininess >= 254.0f)
        specularShininess = 254.0f;
    if (glfwGetKey(pWindow, GLFW_KEY_G) == GLFW_PRESS)
        specularShininess -= 1.0f;
    if (specularShininess <= 0.0f)
        specularShininess = 0.0f;

    // // Spotlight Direction Controls
    // if (glfwGetKey(pWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
    //     spotlightDirection.z += 0.1f;
    // if (glfwGetKey(pWindow, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
    //     spotlightDirection.z -= 0.1f;
    // if (spotlightDirection.z >= 1.0f)
    //     spotlightDirection.z = 1.0f;
    // else if (spotlightDirection.z <= -1.0f)
    //     spotlightDirection.z = -1.0f;
    // if (glfwGetKey(pWindow, GLFW_KEY_BACKSLASH) == GLFW_PRESS)
    //     spotlightDirection.x += 0.1f;
    // if (glfwGetKey(pWindow, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
    //     spotlightDirection.x -= 0.1f;
    // if (spotlightDirection.x >= 1.0f)
    //     spotlightDirection.x = 1.0f;
    // else if (spotlightDirection.x <= -1.0f)
    //     spotlightDirection.x = -1.0f;

    // Shadow Sample Area Controls
    if (glfwGetKey(pWindow, GLFW_KEY_8) == GLFW_PRESS)
        sampleAreaSize += 1.0f;
    if (sampleAreaSize >= 10.0f)
        sampleAreaSize = 10.0f;
    if (glfwGetKey(pWindow, GLFW_KEY_7) == GLFW_PRESS)
        sampleAreaSize -= 1.0f;
    if (sampleAreaSize <= 1.0f)
        sampleAreaSize = 1.0f;
    
    glUniform1i(glGetUniformLocation(shader, "sampleSide"),
                sampleSide);
    glUniform1f(glGetUniformLocation(shader, "sampleAreaSize"),
                sampleAreaSize);

    ///////////////////////////////////////////////////////////////////////////
    // draw the shadow map

    glm::mat4 lightTransform;

    if (drawShadows)
    {
        glCullFace(GL_FRONT);
        lightTransform = renderShadowMap();
        glCullFace(GL_BACK);
    }

    ///////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // render Reflections
    renderReflection();
    /////////////////////////////////////////////////////////////////////////


    // clear the whole frame
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using our shader program...
    glUseProgram(shader);

    // ... set up the projection matrix...
    glm::mat4 projectionTransform;
    projectionTransform = glm::perspective(glm::radians(45.0f),                 // fov
                                           (float)WINDOW_WIDTH / WINDOW_HEIGHT, // aspect ratio
                                           0.1f,                                // near plane
                                           100.0f);                             // far plane
    glUniformMatrix4fv(glGetUniformLocation(shader, "projectionTransform"),
                       1, GL_FALSE, glm::value_ptr(projectionTransform));

    // ... set up the view matrix...
    glm::mat4 viewTransform;
    viewTransform = glm::lookAt(cameraPosition,               // eye position
                                camera.position + camera.front,  // center position
                                camera.up); // up vector
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewTransform"),
                       1, GL_FALSE, glm::value_ptr(viewTransform));

    // ... set up the light position...
    glUniform3fv(glGetUniformLocation(shader, "lightPosition"),
                 1, glm::value_ptr(lightPosition));

    // ... set up spotlight values
    // glUniform3fv(glGetUniformLocation(shader, "spotlightDirection"),
    //              1, glm::value_ptr(spotlightDirection));
    // glUniform1f(glGetUniformLocation(shader, "innerAngle"),
    //             innerAngle);
    // glUniform1f(glGetUniformLocation(shader, "outerAngle"),
    //             outerAngle);
    glUniform1f(glGetUniformLocation(shader, "uniformAmbientIntensity"),
                uniformAmbientIntensity);
    glUniform1f(glGetUniformLocation(shader, "specularShininess"),
                specularShininess);
    glUniform1f(glGetUniformLocation(shader, "currentLinearCoeff"),
                currentLinearCoeff);
    glUniform1f(glGetUniformLocation(shader, "currentQuadraticCoeff"),
                currentQuadraticCoeff);

    //................. SHADOW STUFF ..................//

    int shadowOnUniform;
    if (drawShadows)
    {
        shadowOnUniform = 1;
    }
    else
    {
        shadowOnUniform = 0;
    }

    glUniformMatrix4fv(glGetUniformLocation(shader, "lightTransform"),
                       1, GL_FALSE, glm::value_ptr(lightTransform));

    //................. SHADOW STUFF END ..................//

    int isWater = 0;
    glUniform1i(glGetUniformLocation(shader, "isWater"),
                 isWater);
    
    // ... set up the model matrix...
    glm::mat4 modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f, 0.0f, 0.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(270.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(1.0f, 1.0f, 1.0f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // black floor
    float xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 20; i++)
    {
        // ... set up the model matrix...

        if(i % 5 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 5 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-2.0f + xDisplacement, 2.0f, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// black ceil
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 20; i++)
    {
        // ... set up the model matrix...

        if(i % 5 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 5 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-2.0f + xDisplacement, 7.0f, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r1 wall
    float yDisplacement = 0, rowCount = -1;
    xDisplacement = 0;

    for (size_t i = 0; i < 16; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        xDisplacement = (i - 4 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f + xDisplacement, 2.0f + yDisplacement, 4.0f)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // blue floor
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 8; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, 1.0f, -2.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // blue ceil out
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f  + xDisplacement, 6.0f, -2.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // blue ceil in
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f + xDisplacement, 5.0f, -4.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r2 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 16; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 4 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-2.0f, 2.0f + yDisplacement, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r3 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 10; i++)
    {
        // ... set up the model matrix...

        if(i % 2 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 2 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(-1.0f, 1.0f + yDisplacement, -1.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // r4 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 45; i++)
    {
        // ... set up the model matrix...

        if(i % 9 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 9 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(3.0f, 1.0f + yDisplacement, 3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r5 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 40; i++)
    {
        // ... set up the model matrix...

        if(i % 10 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 10 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f, 0.0f + yDisplacement, -3.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r6 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 32; i++)
    {
        // ... set up the model matrix...

        if(i % 8 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 8 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(4.0f, 0.0f + yDisplacement, -6.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r7 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 15; i++)
    {
        // ... set up the model matrix...

        if(i % 5 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 5 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f, 0.0f + yDisplacement, -13.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// r8 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 4 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(5.0f, 0.0f + yDisplacement, -14.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // green floor
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 48; i++)
    {
        // ... set up the model matrix...

        if(i % 12 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 12 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, 0.0f, -6.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// green ceil 1 shadow ?
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 12; i++)
    {
        // ... set up the model matrix...

        if(i % 4 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 4 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 4.0f, -8.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // green ceil 2
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 30; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 6 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 3.0f, -12.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }
    
    // water floor
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 36; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 6 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, -1.0f, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// s1 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 24; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f, -1.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // s2 wall
    yDisplacement = 0, rowCount = -1;
    xDisplacement = 0;

    for (size_t i = 0; i < 24; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        xDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f + xDisplacement, -1.0f + yDisplacement, -24.0f)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// s3 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 24; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(7.0f, -1.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// p1 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 4; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 1 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(6.0f, -1.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// p2 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 2; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 1 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(6.0f, -1.0f + yDisplacement, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

// p3 wall
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 4; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 1 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f, -1.0f + yDisplacement, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 1
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 5; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 0.0f, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 2
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 0.0f, -19.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 3
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 2; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(4.0f + xDisplacement, 0.0f, -19.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 4
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f + xDisplacement, 0.0f, -22.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 5
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f + xDisplacement, 0.0f, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore floor 5
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 1; i++)
    {
        // ... set up the model matrix...

        if(i % 1 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 1 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(4.0f + xDisplacement, 0.0f, -23.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ore 1

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(4.0f + xDisplacement, 1.0f, -23.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 1a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(3.0f + xDisplacement, 0.0f, -23.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 2

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 1.0f, -23.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 3

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 1.0f, -22.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 3b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 0.0f, -22.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 4a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 1.0f, -19.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 4b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 1.0f, -18.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 5a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(4.0f + xDisplacement, 1.0f, -18.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 5b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(5.0f + xDisplacement, 1.0f, -19.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 1.0f, -6.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 1.0f, -7.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6ac

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 5.0f, -6.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 6bd

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(2.0f + xDisplacement, 5.0f, -7.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 7a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(-1.0f + xDisplacement, 2.0f, -2.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 7b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(-1.0f + xDisplacement, 5.0f, -3.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 8a

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f + xDisplacement, 3.0f, 3.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 8b

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 3.0f, 2.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    // ore 8c

    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(1.0f + xDisplacement, 4.0f, 3.0f - zDisplacement)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                glm::radians(0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));


    // cone 1
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f, 6.5f, -1.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.35f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // cone 2
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(-1.0f, 6.5f, 0.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // cone 3
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(3.0f, 3.5f, -11.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.35f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // cone 3
    // ... set up the model matrix...
    modelTransform = glm::mat4(1.0f); // set to identity first
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(6.0f, 3.5f, -20.0f)); // translate xyz
    modelTransform = glm::rotate(modelTransform,
                                 glm::radians(90.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(0.5f, 0.5f, 0.35f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));

    // water ceiling
    xDisplacement = 0, zDisplacement = 0, columnCount = -1;

    for (size_t i = 0; i < 36; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            xDisplacement += 1.0f;
            columnCount += 1;
        }

        zDisplacement = (i - 6 * columnCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(0.0f  + xDisplacement, 4.0f, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // water ceiling inner left
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 6; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f, 2.0f + yDisplacement, -18.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // water ceiling inner back
    yDisplacement = 0, rowCount = -1;
    xDisplacement = 0;

    for (size_t i = 0; i < 6; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        xDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(1.0f + xDisplacement, 2.0f + yDisplacement, -23.0f)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // water ceiling inner right
    yDisplacement = 0, rowCount = -1;
    zDisplacement = 0;

    for (size_t i = 0; i < 2; i++)
    {
        // ... set up the model matrix...

        if(i % 6 == 0)
        {
            yDisplacement += 1.0f;
            rowCount += 1;
        }

        zDisplacement = (i - 6 * rowCount) * 1.0f;

        modelTransform = glm::mat4(1.0f); // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(6.0f, 2.0f + yDisplacement, -21.0f - zDisplacement)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.5f, 0.5f, 0.5f)); // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active textures...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
        glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));
    }

    // ... set up the model matrix... (just identity for this demo)
    modelTransform = glm::mat4(1.0f);

    if(lightPlaced)
    {
        modelTransform = glm::translate(modelTransform,
                                    lightPosition - glm::vec3(0.0, 0.5, 0.0)); // translate xyz
    }
    else
    {
        modelTransform = glm::translate(modelTransform,
                                    glm::vec3(0.0f, 0.0f, 0.0f)); // translate xyz
    }
    modelTransform = glm::scale(modelTransform,
                                    glm::vec3(0.1f, 0.1f, 0.1f)); // scale x and y
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));
    

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[14]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[15]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[16]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vaoTorch);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesTorch) / (11 * sizeof(float)));

    /////////////////RENDER WATER//////////////////////////////////////////////////////
    isWater = 1;
    glUniform1i(glGetUniformLocation(shader, "isWater"),
                 isWater);

    // ... set up the model matrix... (just identity for this demo)
    modelTransform = glm::mat4(1.0f);
    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(3.0f, 0.4f, -22.0f)); // translate xyz
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));
    

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "shadowOnUniform"), shadowOnUniform);

    // ... then draw our triangles
    glBindVertexArray(vaoCube);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesCube) / (11 * sizeof(float)));
    /////////////////END RENDER WATER//////////////////////////////////////////////////////

    // // ... set up the model matrix...
    // modelTransform = glm::mat4(1.0f); // set to identity first
    // modelTransform = glm::translate(modelTransform,
    //                                 glm::vec3(0.0f, 0.0f, 0.0f)); // translate xyz
    // modelTransform = glm::rotate(modelTransform,
    //                              glm::radians(270.0f),
    //                              glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around z
    // modelTransform = glm::scale(modelTransform,
    //                             glm::vec3(1.0f, -1.0f, -1.0f)); // scale x and y
    // glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
    //                    1, GL_FALSE, glm::value_ptr(modelTransform));

    // // ... set the active textures...
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture[0]);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, texture[1]);
    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, texture[2]);
    // glActiveTexture(GL_TEXTURE3);
    // glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    // glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);

    // // ... then draw our triangles
    // glBindVertexArray(vao2);
    // glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / (11 * sizeof(float)));


    // Camera Controls
 
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        reflectionCamera.position += (float) elapsedTime * reflectionCamera.front * movementSpeed;
        camera.position += (float) elapsedTime * camera.front * movementSpeed;
    }
    // moving backward is just the reverse of moving forward
    if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        reflectionCamera.position -= (float) elapsedTime * reflectionCamera.front * movementSpeed;
        camera.position -= (float) elapsedTime * camera.front * movementSpeed;
    }
    // sidestepping left/right requires determining a vector perpendicular to front and up
    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.position -= (float) elapsedTime *
        glm::normalize(glm::cross(camera.front, camera.up)) * movementSpeed;
        reflectionCamera.position -= (float) elapsedTime *
        glm::normalize(glm::cross(reflectionCamera.front, reflectionCamera.up)) * movementSpeed;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.position += (float) elapsedTime *
        glm::normalize(glm::cross(camera.front, camera.up)) * movementSpeed;
        reflectionCamera.position += (float) elapsedTime *
        glm::normalize(glm::cross(reflectionCamera.front, reflectionCamera.up)) * movementSpeed;
    }

    // implementing the yaw rotation
    // float scaleFactor = 50.0f;
    // if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
    // camera.yaw -= elapsedTime * scaleFactor;
    // if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
    // camera.yaw += elapsedTime * scaleFactor;
    // if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS)
    // camera.pitch += elapsedTime * scaleFactor;
    // if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
    // camera.pitch -= elapsedTime * scaleFactor;

    if(camera.pitch > 89.0f)
    camera.pitch =  89.0f;
    if(camera.pitch < -89.0f)
    camera.pitch = -89.0f;

    if(reflectionCamera.pitch > 89.0f)
    reflectionCamera.pitch =  89.0f;
    if(reflectionCamera.pitch < -89.0f)
    reflectionCamera.pitch = -89.0f;

    // basic trigonometry to determine a new front direction
    glm::vec3 direction;
    glm::vec3 reflectionDirection;
    direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));

    reflectionDirection.x = cos(glm::radians(reflectionCamera.yaw)) * cos(glm::radians(reflectionCamera.pitch));
    reflectionDirection.y = sin(glm::radians(reflectionCamera.pitch));
    reflectionDirection.z = sin(glm::radians(reflectionCamera.yaw)) * cos(glm::radians(reflectionCamera.pitch));

    // actually set the new front direction
    camera.front = glm::normalize(direction);
    reflectionCamera.front = glm::normalize(reflectionDirection);
}

/*****************************************************************************/

// handler called by GLFW when there is a keyboard event
void handleKeys(GLFWwindow *pWindow, int key, int scancode, int action, int mode)
{
    // pressing Esc closes the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);

    if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS && currentCoeff < 6)
    {
        currentCoeff++;
        currentLinearCoeff = linearCoeffs[currentCoeff];
        currentQuadraticCoeff = quadraticCoeffs[currentCoeff];
    }

    if (key == GLFW_KEY_COMMA && action == GLFW_PRESS && currentCoeff > 0)
    {
        currentCoeff--;
        currentLinearCoeff = linearCoeffs[currentCoeff];
        currentQuadraticCoeff = quadraticCoeffs[currentCoeff];
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        if (drawShadows)
        {
            drawShadows = false;
        }
        else
        {
            drawShadows = true;
        }
    }

    // Shadow Sharpness Controls
    if(key == GLFW_KEY_9 && action == GLFW_PRESS)
        sampleSide -= 2;
    if(key == GLFW_KEY_0 && action == GLFW_PRESS)
        sampleSide += 2;
    if(sampleSide % 2 == 0)
        sampleSide -= 1;
    if(sampleSide <= 3)
        sampleSide = 3;
    if(sampleSide >= 9)
        sampleSide = 9;

    // Torch Placing
    if(key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if(lightPlaced)
        {
            lightPlaced = false;
        }
        else
        {
            lightPlaced = true;
            lightPlacedPosition = lightPosition;
        }
    }
}

// handler called by GLFW when the window is resized
void handleResize(GLFWwindow *pWindow, int width, int height)
{
    // tell OpenGL to do its drawing within the entire "client area" (area within the borders) of the window
    glViewport(0, 0, width, height);
}

bool firstMouse = true;
float sensitivity = 0.1f;
// mouse callback function
void mouse_callback(GLFWwindow* pWindow, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastXPos = xpos;
        lastYPos = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastXPos;
    float yoffset = lastYPos - ypos;
    lastXPos = xpos;
    lastYPos = ypos;

    camera.yaw += xoffset * sensitivity;
    camera.pitch += yoffset * sensitivity;
    reflectionCamera.yaw += xoffset * sensitivity;
    reflectionCamera.pitch -= yoffset * sensitivity;

    if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    if(reflectionCamera.pitch > 89.0f)
        reflectionCamera.pitch = 89.0f;
    if(reflectionCamera.pitch < -89.0f)
        reflectionCamera.pitch = -89.0f;
}

// main function
int main(int argc, char **argv)
{
    // initialize GLFW and ask for OpenGL 3.3 core
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create a GLFW window with the specified width, height, and title
    pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!pWindow)
    {
        // gracefully terminate if we cannot create the window
        std::cout << "Cannot create the GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    // enable vertical sync and aspect-ratio correction on the GLFW window,
    // and make the window the current context of subsequent OpenGL commands
    glfwSwapInterval(1);
    glfwSetWindowAspectRatio(pWindow, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwMakeContextCurrent(pWindow);

    // set up callback functions to handle window system events
    glfwSetKeyCallback(pWindow, handleKeys);
    glfwSetFramebufferSizeCallback(pWindow, handleResize);

    // set up mouse callback
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(pWindow, mouse_callback);

    // don't miss any momentary keypresses
    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

    // initialize GLAD, which acts as a library loader for the current OS's native OpenGL library
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // enable clipping planes
    glEnable(GL_CLIP_DISTANCE0);

    camera.position = glm::vec3(0.0f, 4.0f, 3.0f);
    reflectionCamera.position = glm::vec3(0.0f, -4.0f, 3.0f);

    // if our initial setup is successful...
    if (setup())
    {
        // do rendering in a loop until the user closes the window
        while (!glfwWindowShouldClose(pWindow))
        {
            // render our next frame
            // (by default, GLFW uses double-buffering with a front and back buffer;
            // all drawing goes to the back buffer, so the frame does not get shown yet)
            render();

            // swap the GLFW front and back buffers to show the next frame
            glfwSwapBuffers(pWindow);

            // process any window events (such as moving, resizing, keyboard presses, etc.)
            glfwPollEvents();
        }
    }

    // gracefully terminate the program
    glfwTerminate();
    return 0;
}
