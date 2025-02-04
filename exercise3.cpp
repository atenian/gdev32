/******************************************************************************
 * This demo draws a triangle by defining its vertices in 3 dimensions
 * (the 3rd dimension is currently ignored and is just set to 0).
 *
 * The drawing is accomplished by:
 * - Uploading the vertices to the GPU using a Vertex Buffer Object (VBO).
 * - Specifying the vertices' format using a Vertex Array Object (VAO).
 * - Using a GLSL shader program (consisting of a simple vertex shader and a
 *   simple fragment shader) to actually draw the vertices as a triangle.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE  "SUPER SMAAAAAAAAASH BROTHERSSSS!!"
GLFWwindow *pWindow;

// define a vertex array to hold our vertices
// Background Vertices
float backgroundVertices[] =
{
    // position (x, y, z) color (r, g, b) normal (x, y, z) texture coordinates (s,t)
    -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

    1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
};

//Main Platform Vertices
float mainPlatformVertices[] =
{
// position (x, y, z) color (r, g, b) normal (x, y, z) texture coordinates (s,t)
// Main platform vertices
// Top vertices
    // Triangle 1
    -0.5f, 0.0f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    0.5f,  0.0f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.0f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    // Triangle 2
    -0.5f, 0.0f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 1.0f,
    0.5f,  0.0f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    0.5f,  0.0f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    

// Front vertices
    // Triangle 1
    -0.5f, 0.0f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.43f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    -0.46f,  -0.3f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    // Triangle 2
    -0.4f, -0.4f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    -0.46f,  -0.3f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.43f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // Triangle 3
    -0.4f, -0.4f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.43f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    -0.35f,  -0.5f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    // Triangle 4
    -0.35f, -0.5f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.43f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    -0.3f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 

    // Triangle 5
    -0.35f, -0.5f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    -0.3f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    -0.3f,  -0.55f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // Triangle 6
    -0.27f, -0.53f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.3f,  -0.55f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    -0.3f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 

    // Triangle 7
    -0.27f, -0.53f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    -0.3f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    -0.2f,  -0.53f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    
    // Triangle 8
    -0.17f, -0.51f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.2f,  -0.53f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    -0.3f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 

    // Triangle 9
    -0.17f, -0.51f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    -0.3f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    -0.15f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    
    // Triangle 10
    -0.17f, -0.51f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.15f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    -0.1f,  -0.58f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 
    
    // Triangle 11
    -0.06f, -0.6f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    -0.1f,  -0.58f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    -0.15f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // Triangle 12
    -0.06f, -0.6f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.15f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.0f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 

    // Triangle 13
    -0.06f, -0.6f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    0.0f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    -0.02f,  -0.64f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // Triangle 14
    0.05f, -0.7f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.02f,  -0.64f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.0f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 

    // Triangle 15
    0.05f, -0.7f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    0.0f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    0.1f,  -0.58f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // Triangle 16
    0.2f, -0.66f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    0.1f,  -0.58f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.0f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 

    // Triangle 17
    0.2f, -0.66f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    0.0f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    0.2f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // Triangle 18
    0.2f, -0.66f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    0.2f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.3f,  -0.5f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 

    // Triangle 19
    0.32f, 0.0f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    0.3f,  -0.5f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    0.2f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // Triangle 20
    0.32f, 0.0f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    0.35f,  -0.36f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.3f,  -0.4f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 

    // Triangle 21
    0.32f, 0.0f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    0.4f,  -0.3f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    0.35f,  -0.36f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // Triangle 22
    0.32f, 0.0f, -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    0.5f,  0.0f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.4f,  -0.3f,  -0.5f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 

// Back vertices
    // Triangle 1
    -0.5f, 0.0f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.46f,  -0.3f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.43f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,

    // Triangle 2
    -0.4f, -0.4f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    -0.43f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.46f,  -0.3f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    // Triangle 3
    -0.4f, -0.4f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.35f,  -0.5f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.43f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,

    // Triangle 4
    -0.35f, -0.5f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.3f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 
    -0.43f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    
    // Triangle 5
    -0.35f, -0.5f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    -0.3f,  -0.55f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.3f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 

    // Triangle 6
    -0.27f, -0.53f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.3f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.3f,  -0.55f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 

    // Triangle 7
    -0.27f, -0.53f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    -0.2f,  -0.53f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.3f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    
    // Triangle 8
    -0.17f, -0.51f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.3f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.2f,  -0.53f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 

    // Triangle 9
    -0.17f, -0.51f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    -0.15f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.3f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    
    // Triangle 10
    -0.17f, -0.51f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    -0.1f,  -0.58f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.15f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 
    
    // Triangle 11
    -0.06f, -0.6f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    -0.15f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.1f,  -0.58f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 

    // Triangle 12
    -0.06f, -0.6f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    0.0f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.15f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 

    // Triangle 13
    -0.06f, -0.6f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    -0.02f,  -0.64f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.0f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 

    // Triangle 14
    0.05f, -0.7f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    0.0f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.02f,  -0.64f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 

    // Triangle 15
    0.05f, -0.7f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    0.1f,  -0.58f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.0f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 

    // Triangle 16
    0.2f, -0.66f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    0.0f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.1f,  -0.58f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 

    // Triangle 17
    0.2f, -0.66f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    0.2f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.0f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 

    // Triangle 18
    0.2f, -0.66f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    0.3f,  -0.5f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.2f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 

    // Triangle 19
    0.32f, 0.0f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    0.2f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.3f,  -0.5f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 

    // Triangle 20
    0.32f, 0.0f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    0.3f,  -0.4f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.35f,  -0.36f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 

    // Triangle 21
    0.32f, 0.0f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    0.35f,  -0.36f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.4f,  -0.3f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 

    // Triangle 22
    0.32f, 0.0f, -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,
    0.4f,  -0.3f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f,  0.0f,  -0.3f, 0.3f, 0.93f, 0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 

// Bottom vertices
    // Rectangle 1
    -0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f, 0.9917f, 0.1322f, 0.0f, 0.5f, 1.0f,
    -0.46f,  -0.3f, -0.5f, 1.0f, 0.0f, 0.0f, 0.9917f, 0.1322f, 0.0f, 0.0f, 0.0f,    
    -0.5f, 0.0f, -0.3f, 1.0f, 0.0f, 0.0f, 0.9917f, 0.1322f, 0.0f, 1.0f, 0.0f,

    -0.5f, 0.0f, -0.3f, 1.0f, 0.0f, 0.0f, -0.9917f, -0.1322f, 0.0f, 0.5f, 1.0f,
    -0.46f,  -0.3f, -0.5f, 1.0f, 0.0f, 0.0f, -0.9917f, -0.1322f, 0.0f, 1.0f, 0.0f,
    -0.46f,  -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, -0.9917f, -0.1322f, 0.0f, 0.0f, 0.0f,

    // Rectangle 2
    -0.4f, -0.4f, -0.5f, 1.0f, 0.0f, 0.0f, -0.858f, -0.515f, 0.0f, 1.0f, 0.0f,
    -0.4f, -0.4f, -0.3f, 1.0f, 0.0f, 0.0f, -0.858f, -0.515f, 0.0f, 0.5f, 1.0f,
    -0.46f,  -0.3f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.858f, -0.515f, 0.0f, 0.0f, 0.0f,

    -0.4f, -0.4f, -0.3f, 1.0f, 0.0f, 0.0f, 0.858f, 0.515f, 0.0f, 0.5f, 1.0f,
    -0.46f,  -0.3f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.858f, 0.515f, 0.0f, 0.0f, 0.0f,
    -0.46f,  -0.3f,  -0.5f, 1.0f, 0.0f, 0.858f, 0.515f, 0.0f, 0.253f, 1.0f, 0.0f,

    // Rectangle 3
    -0.4f, -0.4f, -0.5f, 1.0f, 0.0f, 0.0f, 0.894f, 0.447f, 0.0f, 0.0f, 0.0f,
    -0.35f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.894f, 0.447f, 0.0f, 0.5f, 1.0f,
    -0.4f, -0.4f, -0.3f, 1.0f, 0.0f, 0.0f, 0.894f, 0.447f, 0.0f, 1.0f, 0.0f,

    -0.4f, -0.4f, -0.3f, 1.0f, 0.0f, 0.0f, -0.894f, -0.447f, 0.0f, 0.5f, 1.0f,
    -0.35f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, -0.894f, -0.447f, 0.0f, 0.0f, 0.0f,
    -0.35f, -0.5f, -0.3f, 1.0f, 0.0f, 0.0f, -0.894f, -0.447f, 0.0f, 1.0f, 0.0f,

    // Rectangle 4
    -0.35f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, -0.707f, -0.707f, 0.0f, 0.5f, 1.0f,
    -0.3f,  -0.55f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.707f, -0.707f, 0.0f, 0.0f, 0.0f,
    -0.3f,  -0.55f,  -0.3f, 1.0f, 0.0f, 0.0f, -0.707f, -0.707f, 0.0f, 1.0f, 0.0f, 

    -0.35f, -0.5f, -0.3f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f, 0.0f, 0.0f, 0.0f,
    -0.35f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f, 0.0f, 0.5f, 1.0f,     
    -0.3f,  -0.55f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f, 0.0f, 1.0f, 0.0f,

    // Rectangle 5
    -0.27f, -0.53f, -0.5f, 1.0f, 0.0f, 0.0f, -0.554f, 0.832f, 0.0f, 0.5f, 1.0f,
    -0.3f,  -0.55f,  -0.3f, 1.0f, 0.0f, 0.0f, -0.554f, 0.832f, 0.0f, 0.0f, 0.0f, 
    -0.3f,  -0.55f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.554f, 0.832f, 0.0f, 1.0f, 0.0f,

    -0.27f, -0.53f, -0.3f, 1.0f, 0.0f, 0.0f, 0.554f, -0.832f, 0.0f, 0.0f, 0.0f,
    -0.3f,  -0.55f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.554f, -0.832f, 0.0f, 1.0f, 0.0f,
    -0.27f, -0.53f, -0.5f, 1.0f, 0.0f, 0.0f, 0.554f, -0.832f, 0.0f, 0.5f, 1.0f, 

    // Rectangle 6
    -0.27f, -0.53f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.2f,  -0.53f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.27f, -0.53f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 1.0f,

    -0.27f, -0.53f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    -0.2f,  -0.53f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.2f,  -0.53f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 

    // Rectangle 7
    -0.17f, -0.51f, -0.5f, 1.0f, 0.0f, 0.0f, -0.556f, 0.833f, 0.0f, 0.5f, 1.0f,
    -0.2f,  -0.53f,  -0.3f, 1.0f, 0.0f, 0.0f, -0.556f, 0.833f, 0.0f, 0.0f, 0.0f,
    -0.2f,  -0.53f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.556f, 0.833f, 0.0f, 1.0f, 0.0f,

    -0.17f, -0.51f, -0.3f, 1.0f, 0.0f, 0.0f, 0.556f, -0.833f, 0.0f, 0.0f, 0.0f,
    -0.2f,  -0.53f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.556f, -0.833f, 0.0f, 1.0f, 0.0f,
    -0.17f, -0.51f, -0.5f, 1.0f, 0.0f, 0.0f, 0.556f, -0.833f, 0.0f, 0.5f, 1.0f,

    // Rectangle 8
    -0.17f, -0.51f, -0.5f, 1.0f, 0.0f, 0.0f, 0.7071f, 0.7071f, 0.0f, 0.5f, 1.0f,
    -0.1f,  -0.58f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.7071f, 0.7071f, 0.0f, 0.0f, 0.0f, 
    -0.17f, -0.51f, -0.3f, 1.0f, 0.0f, 0.0f, 0.7071f, 0.7071f, 0.0f, 1.0f, 0.0f,

    -0.17f, -0.51f, -0.3f, 1.0f, 0.0f, 0.0f, -0.7071f, -0.7071f, 0.0f, 0.5f, 1.0f,
    -0.1f,  -0.58f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.7071f, -0.7071f, 0.0f, 0.0f, 0.0f, 
    -0.1f,  -0.58f,  -0.3f, 1.0f, 0.0f, 0.0f, -0.7071f, -0.7071f, 0.0f, 1.0f, 0.0f, 

    // Rectangle 9
    -0.06f, -0.6f, -0.5f, 1.0f, 0.0f, 0.0f, -0.447f, 0.894f, 0.0f, 0.0f, 0.0f,
    -0.06f, -0.6f, -0.3f, 1.0f, 0.0f, 0.0f, -0.447f, 0.894f, 0.0f, 0.5f, 1.0f,
    -0.1f,  -0.58f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.447f, 0.894f, 0.0f, 1.0f, 0.0f, 

    -0.06f, -0.6f, -0.3f, 1.0f, 0.0f, 0.0f, 0.447f, -0.894f, 0.0f, 0.5f, 1.0f,
    -0.1f,  -0.58f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.447f, -0.894f, 0.0f, 0.0f, 0.0f, 
    -0.1f,  -0.58f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.447f, -0.894f, 0.0f, 1.0f, 0.0f, 

    // Rectangle 10
    -0.06f, -0.6f, -0.5f, 1.0f, 0.0f, 0.0f, -0.7071f, -0.7071f, 0.0f, 0.5f, 1.0f,
    -0.02f,  -0.64f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.7071f, -0.7071f, 0.0f, 1.0f, 0.0f,
    -0.06f, -0.6f, -0.3f, 1.0f, 0.0f, 0.0f, -0.7071f, -0.7071f, 0.0f, 0.0f, 0.0f,

    -0.06f, -0.6f, -0.3f, 1.0f, 0.0f, 0.0f, 0.7071f, 0.7071f, 0.0f, 0.5f, 1.0f,
    -0.02f,  -0.64f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.7071f, 0.7071f, 0.0f, 0.0f, 0.0f,
    -0.02f,  -0.64f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.7071f, 0.7071f, 0.0f, 1.0f, 0.0f,

    // Rectangle 11
    0.05f, -0.7f, -0.5f, 1.0f, 0.0f, 0.0f, 0.652f, 0.759f, 0.0f, 0.5f, 1.0f,
    -0.02f,  -0.64f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.652f, 0.759f, 0.0f, 0.0f, 0.0f, 
    -0.02f,  -0.64f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.652f, 0.759f, 0.0f, 1.0f, 0.0f,

    0.05f, -0.7f, -0.3f, 1.0f, 0.0f, 0.0f, -0.652f, -0.759f, 0.0f, 0.5f, 1.0f,
    -0.02f, -0.64f, -0.3f, 1.0f, 0.0f, 0.0f, -0.652f, -0.759f, 0.0f, 1.0f, 0.0f,
    0.05f, -0.7f, -0.5f, 1.0f, 0.0f, 0.0f, -0.652f, -0.759f, 0.0f, 0.0f, 0.0f,

    // Rectangle 12
    0.05f, -0.7f, -0.5f, 1.0f, 0.0f, 0.0f, 0.921f, -0.384f, 0.0f, 0.5f, 1.0f,
    0.1f,  -0.58f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.921f, -0.384f, 0.0f, 1.0f, 0.0f,
    0.05f, -0.7f, -0.3f, 1.0f, 0.0f, 0.0f, 0.921f, -0.384f, 0.0f, 0.0f, 0.0f, 

    0.05f, -0.7f, -0.3f, 1.0f, 0.0f, 0.0f, -0.921f, 0.384f, 0.0f, 0.5f, 1.0f,
    0.1f,  -0.58f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.921f, 0.384f, 0.0f, 0.0f, 0.0f, 
    0.1f,  -0.58f,  -0.3f, 1.0f, 0.0f, 0.0f, -0.921f, 0.384f, 0.0f, 1.0f, 0.0f,

    // Rectangle 13
    0.2f, -0.66f, -0.5f, 1.0f, 0.0f, 0.0f, 0.625f, 0.781f, 0.0f, 0.5f, 1.0f,
    0.2f, -0.66f, -0.3f, 1.0f, 0.0f, 0.0f, 0.625f, 0.781f, 0.0f, 0.0f, 0.0f,
    0.1f,  -0.58f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.625f, 0.781f, 0.0f, 1.0f, 0.0f,

    0.2f, -0.66f, -0.3f, 1.0f, 0.0f, 0.0f, -0.625f, -0.781f, 0.0f, 0.5f, 1.0f,
    0.1f,  -0.58f,  -0.3f, 1.0f, 0.0f, 0.0f, -0.625f, -0.781f, 0.0f, 0.0f, 0.0f,
    0.1f,  -0.58f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.625f, -0.781f, 0.0f, 1.0f, 0.0f,

    // Rectangle 14
    0.2f, -0.66f, -0.5f, 1.0f, 0.0f, 0.0f, -0.85f, 0.53f, 0.0f, 0.5f, 1.0f,
    0.3f,  -0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.85f, 0.53f, 0.0f, 0.0f, 0.0f, 
    0.2f, -0.66f, -0.3f, 1.0f, 0.0f, 0.0f, -0.85f, 0.53f, 0.0f, 1.0f, 0.0f,

    0.2f, -0.66f, -0.3f, 1.0f, 0.0f, 0.0f, 0.85f, -0.53f, 0.0f, 0.5f, 1.0f,
    0.3f,  -0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.85f, -0.53f, 0.0f, 0.0f, 0.0f, 
    0.3f,  -0.5f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.85f, -0.53f, 0.0f, 1.0f, 0.0f, 

    // Rectangle 15
    0.3f,  -0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
    0.3f,  -0.4f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f, 
    0.3f,  -0.5f,  -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 

    0.3f,  -0.5f,  -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
    0.3f,  -0.4f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f, 
    0.3f,  -0.4f,  -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    // Rectangle 16
    0.35f,  -0.36f,  -0.3f, 1.0f, 0.0f, 0.0f, -0.625f, 0.781f, 0.0f, 0.0f, 0.0f,
    0.3f,  -0.4f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.625f, 0.781f, 0.0f, 0.5f, 1.0f, 
    0.35f,  -0.36f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.625f, 0.781f, 0.0f, 1.0f, 0.0f,

    0.3f, -0.4f, -0.5f, 1.0f, 0.0f, 0.0f, 0.625f, -0.781f, 0.0f, 0.0f, 0.0f, 
    0.35f, -0.36f, -0.3f, 1.0f, 0.0f, 0.0f, 0.625f, -0.781f, 0.0f, 0.5f, 1.0f,
    0.3f, -0.4f, -0.3f, 1.0f, 0.0f, 0.0f, 0.625f, -0.781f, 0.0f, 1.0f, 0.0f, 

    // Rectangle 17
    0.35f,  -0.36f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.769f, -0.641f, 0.0f, 0.0f, 0.0f,
    0.35f,  -0.36f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.769f, -0.641f, 0.0f, 0.5f, 1.0f,
    0.4f,  -0.3f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.769f, -0.641f, 0.0f, 1.0f, 0.0f, 

    0.4f,  -0.3f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.769f, 0.641f, 0.0f, 0.0f, 0.0f, 
    0.4f,  -0.3f,  -0.3f, 1.0f, 0.0f, 0.0f, -0.769f, 0.641f, 0.0f, 0.5f, 1.0f, 
    0.35f,  -0.36f,  -0.3f, 1.0f, 0.0f, 0.0f, -0.769f, 0.641f, 0.0f, 1.0f, 0.0f,

    // Rectangle 18
    0.5f,  0.0f,  -0.3f, 1.0f, 0.0f, 0.0f, -0.949f, 0.316f, 0.0f, 0.0f, 0.0f,
    0.4f,  -0.3f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.949f, 0.316f, 0.0f, 0.5f, 1.0f,
    0.5f,  0.0f,  -0.5f, 1.0f, 0.0f, 0.0f, -0.949f, 0.316f, 0.0f, 1.0f, 0.0f, 

    0.4f,  -0.3f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.949f, -0.316f, 0.0f, 0.0f, 0.0f, 
    0.5f,  0.0f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.949f, -0.316f, 0.0f, 0.5f, 1.0f,
    0.4f,  -0.3f,  -0.3f, 1.0f, 0.0f, 0.0f, 0.949f, -0.316f, 0.0f, 1.0f, 0.0f, 
};

//Platform Vertices
float platformVertices[] =
{
    // position (x, y, z) color (r, g, b) normal (x, y, z) texture coordinates (s,t)
// Platform vertices
// Top vertices
    // Triangle 1
    0.0f, 0.0f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.5f,  0.0f,  -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.0f,  0.0f,  -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

    // Triangle 2
    0.0f, 0.0f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.5f,  0.0f,  -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f,  0.0f,  -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

// Front vertices
    // Triangle 1
    0.0f, 0.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.1f, -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    0.0f, -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

    // Triangle 2
    0.0f, 0.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.1f,  0.0f,  -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    0.1f,  -0.03f,  -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

    // Triangle 3
    0.1f,  -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    
    0.35f, -0.1f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.15f,  -0.1f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

    // Triangle 4
    0.1f,  -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,    
    0.1f,  0.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    0.35f, -0.1f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,

    // Triangle 5    
    0.1f,  0.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    0.4f, -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.35f,  -0.1f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

    // Triangle 6    
    0.1f,  0.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    0.4f, 0.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.4f,  -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

    // Triangle 7    
    0.4f, 0.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f,  -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    0.4f,  -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

    // Triangle 8    
    0.4f, 0.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f,  0.0f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f,  -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

// Back vertices
    // Triangle 1
    0.0f, 0.0f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.0f, -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.1f, -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

    // Triangle 2
    0.0f, 0.0f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.1f,  -0.03f,  -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.1f,  0.0f,  -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

    // Triangle 3
    0.1f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.15f,  -0.1f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,    
    0.35f, -0.1f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    // Triangle 4
    0.1f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.35f, -0.1f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,    
    0.1f,  0.0f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

    // Triangle 5    
    0.1f,  0.0f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    0.35f,  -0.1f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.4f, -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    // Triangle 6    
    0.1f,  0.0f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    0.4f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.4f, 0.0f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    // Triangle 7    
    0.4f, 0.0f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.4f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

    // Triangle 8    
    0.4f, 0.0f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    0.5f,  0.0f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

// Bottom Vertices
    // Rectangle 1
    0.0f, 0.0f, -0.3f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.5f, 1.0f,
    0.0f, -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

    0.0f, 0.0f, -0.3f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.0f, -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

    // Rectangle 2
    0.1f, -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.5f, 1.0f,
    0.1f, -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

    0.0f, -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    0.1f, -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.5f, 1.0f,
    0.0f, -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

    // Rectangle 3
    0.1f,  -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.8139f, 0.5819f, 0.0f, 0.0f, 0.0f,
    0.15f,  -0.1f, -0.5f, 1.0f, 1.0f, 1.0f, 0.8139f, 0.5819f, 0.0f, 0.5f, 1.0f,    
    0.1f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.8139f, 0.5819f, 0.0f, 1.0f, 1.0f,

    0.1f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.8139f, 0.5819f, 0.0f, 0.0f, 0.0f,
    0.15f,  -0.1f, -0.5f, 1.0f, 1.0f, 1.0f, 0.8139f, 0.5819f, 0.0f, 0.5f, 1.0f,
    0.15f,  -0.1f, -0.3f, 1.0f, 1.0f, 1.0f, 0.8139f, 0.5819f, 0.0f, 1.0f, 0.0f,    

    // Rectangle 4
    0.15f,  -0.1f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    0.15f,  -0.1f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.5f, 1.0f,  
    0.35f, -0.1f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

    0.35f, -0.1f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,    
    0.35f, -0.1f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.5f, 1.0f,
    0.15f,  -0.1f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

    // Rectangle 5
    0.35f,  -0.1f, -0.3f, 1.0f, 1.0f, 1.0f, 0.814f, -0.581f, 0.0f, 0.0f, 0.0f,
    0.35f,  -0.1f, -0.5f, 1.0f, 1.0f, 1.0f, 0.814f, -0.581f, 0.0f, 0.5f, 1.0f,
    0.4f, -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.814f, -0.581f, 0.0f, 1.0f, 0.0f,

    0.4f, -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.814f, -0.581f, 0.0f, 0.0f, 0.0f,
    0.4f, -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.814f, -0.581f, 0.0f, 0.5f, 1.0f,
    0.35f,  -0.1f, -0.3f, 1.0f, 1.0f, 1.0f, 0.814f, -0.581f, 0.0f, 1.0f, 0.0f,

    // Rectangle 6 
    0.4f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    0.4f,  -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.5f, 1.0f,
    0.5f,  -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

    0.4f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    0.5f,  -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.5f, 1.0f,

    // Rectangle 7
    0.5f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f,  -0.03f, -0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f,  0.0f, -0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

    0.5f,  0.0f, -0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f,  0.0f, -0.3f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f,  -0.03f, -0.3f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.5f, 1.0f,

};

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU

// vertex array objects (store the render state for our vertex array)
GLuint vao1;        // vao for Main Platform Vertices
GLuint vao2;        // vao for Platform Vertices
GLuint vao3;        // vao for third triangle

// vertex buffer objects (reserve GPU memory for our vertex array)
GLuint vbo1;        // vbo for Main Platform Vertices
GLuint vbo2;        // vbo for Platform Vertices
GLuint vbo3;        // vbo for third triangle

// combined vertex and fragment shaders
GLuint shader1;     // shader for Main Platform Vertices
GLuint shader2;     // shader for Platform Vertices
GLuint shader3;     // shader for triangle 3

// textures
GLuint bgtexture;   // background texture
GLuint texture0;    // first texture - main platform
GLuint texture1;    // second texture - small platform

glm::vec3 eyePosition, forwardVector, upVector; // parameters of lookAt function

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// mouse variables
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

bool firstMouse = true;
float lastX = 640.0f / 2.0;
float lastY = 360.0f / 2.0;


// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // setup for first triangle

    // generate the VAO and VBO objects and store their IDs in vao and vbo, respectively
    glGenVertexArrays(1, &vao1);
    glGenBuffers(1, &vbo1);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vao1);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mainPlatformVertices), mainPlatformVertices, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - layout location 0...
    // - ... shall consist of 3 GL_FLOATs (corresponding to x, y, and z coordinates)
    // - ... its values will NOT be normalized (GL_FALSE)
    // - ... the stride length is the number of bytes of all 8 floats of each vertex (hence, 8 * sizeof(float))
    // - ... and we start at the beginning of the array (hence, (void*) 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) 0);

    // for layout location 1, these correspond to the color values (r, g, b)
    //  it has an offset of 3 * sizeof(float) since it is after the 3 float values from the initial vertices
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (3 * sizeof(float)));

    // for layout location 2, these correspond to the normal
    //  it has an offset of 6 * sizeof(float) since it is after the 3 float values from the initial vertices
    //  and the 3 float values that indicate the color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (6 * sizeof(float)));

    // for layout location 3, these correspond to the texture coordinates (s, t)
    //  it has an offset of 6 * sizeof(float) since it is after the 3 float values from the initial vertices
    //  and the 3 float values that indicate the color
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (9 * sizeof(float)));

    // enable the newly-created layout location 0
    // this shall be used by our vertex shader to draw the triangles
    glEnableVertexAttribArray(0);

    // enable the colors as well
    glEnableVertexAttribArray(1);

    // and also the normal vectors
    glEnableVertexAttribArray(2);

    // and also the texture coordinates
    glEnableVertexAttribArray(3);


    // setup for second triangle

    // generate the VAO and VBO objects and store their IDs in vao and vbo, respectively
    glGenVertexArrays(2, &vao2);
    glGenBuffers(2, &vbo2);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vao2);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(platformVertices), platformVertices, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - layout location 0...
    // - ... shall consist of 3 GL_FLOATs (corresponding to x, y, and z coordinates)
    // - ... its values will NOT be normalized (GL_FALSE)
    // - ... the stride length is the number of bytes of all 8 floats of each vertex (hence, 8 * sizeof(float))
    // - ... and we start at the beginning of the array (hence, (void*) 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) 0);

    // for layout location 1, these correspond to the color values (r, g, b)
    //  it has an offset of 3 * sizeof(float) since it is after the 3 float values from the initial vertices
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (3 * sizeof(float)));

    // for layout location 2, these correspond to the normal
    //  it has an offset of 6 * sizeof(float) since it is after the 3 float values from the initial vertices
    //  and the 3 float values that indicate the color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (6 * sizeof(float)));

    // for layout location 3, these correspond to the texture coordinates (s, t)
    //  it has an offset of 6 * sizeof(float) since it is after the 3 float values from the initial vertices
    //  and the 3 float values that indicate the color
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (9 * sizeof(float)));

    // enable the newly-created layout location 0
    // this shall be used by our vertex shader to draw the triangles
    glEnableVertexAttribArray(0);

    // enable the colors as well
    glEnableVertexAttribArray(1);

    // and also the normal vectors
    glEnableVertexAttribArray(2);

    // and also the texture coordinates
    glEnableVertexAttribArray(3);

    // setup for triangle 3

    // generate the VAO and VBO objects and store their IDs in vao and vbo, respectively
    glGenVertexArrays(3, &vao3);
    glGenBuffers(3, &vbo3);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vao3);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(platformVertices), platformVertices, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - layout location 0...
    // - ... shall consist of 3 GL_FLOATs (corresponding to x, y, and z coordinates)
    // - ... its values will NOT be normalized (GL_FALSE)
    // - ... the stride length is the number of bytes of all 8 floats of each vertex (hence, 8 * sizeof(float))
    // - ... and we start at the beginning of the array (hence, (void*) 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) 0);

    // for layout location 1, these correspond to the color values (r, g, b)
    //  it has an offset of 3 * sizeof(float) since it is after the 3 float values from the initial vertices
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (3 * sizeof(float)));

    // for layout location 2, these correspond to the normal
    //  it has an offset of 6 * sizeof(float) since it is after the 3 float values from the initial vertices
    //  and the 3 float values that indicate the color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (6 * sizeof(float)));

    // for layout location 3, these correspond to the texture coordinates (s, t)
    //  it has an offset of 6 * sizeof(float) since it is after the 3 float values from the initial vertices
    //  and the 3 float values that indicate the color
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (9 * sizeof(float)));

    // enable the newly-created layout location 0
    // this shall be used by our vertex shader to draw the triangles
    glEnableVertexAttribArray(0);

    // enable the colors as well
    glEnableVertexAttribArray(1);

    // and also the normal vectors
    glEnableVertexAttribArray(2);

    // and also the texture coordinates
    glEnableVertexAttribArray(3);

    // load our shader program for first triangle
    shader1 = gdevLoadShader("exercise3-1.vs", "exercise3.fs");
    if (! shader1)
        return false;

    // load our shader program for second triangle
    shader2 = gdevLoadShader("exercise3-2.vs", "exercise3.fs");
    if (! shader2)
        return false;
    
    // load our shader program for third triangle
    shader3 = gdevLoadShader("exercise3-3.vs", "exercise3.fs");
    if (! shader3)
        return false;

    // load first texture
    texture0 = gdevLoadTexture("grass.jpg", GL_REPEAT, true, true);
    if (! texture0)
        return false;

    // load second texture
    texture1 = gdevLoadTexture("way.png", GL_REPEAT, true, true);
    if (! texture1)
        return false;
    
    // setting camera position to (-2, 0, 3), forward vector to (0,0,-1), and up vector (0,1,0)
    eyePosition = glm::vec3(-1.0f, 0.5f, 3.0f);
    forwardVector = glm::vec3(0.0f, 0.0f, -1.0f);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // time logic for camera
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // base matrix = multiplying projection matrix and view transform matrix
    glm::mat4 baseMatrix;

    // calculate projection matrix
    baseMatrix = glm::perspective(glm::radians(fov),
                    (float) WINDOW_WIDTH / WINDOW_HEIGHT,
                    0.1f,
                    100.0f);
    
    // calculating view transform matrix
    glm::mat4 viewTransform;
    viewTransform = glm::lookAt(eyePosition, eyePosition + forwardVector, upVector);

    // multiplying perspective matrix by view transform matrix
    baseMatrix = baseMatrix * viewTransform;

    // ... enable cull face ...
    glEnable(GL_CULL_FACE);
        
    // ... enable OpenGL's hidden surface removal ...
    glEnable(GL_DEPTH_TEST); 

    // using our shader program for the first triangle...
    glUseProgram(shader1);

    // ... enable cull face ...
    glEnable(GL_CULL_FACE);
        
    // ... enable OpenGL's hidden surface removal ...
    glEnable(GL_DEPTH_TEST); 

    // ... calculate matrix for first triangle ...
    glm::mat4 matrix1;
    matrix1 = glm::translate(baseMatrix, glm::vec3(-1.0f, 0.2f, 0.0f));

    // matrix1 = glm::rotate(matrix1, glm::radians(60.0f),
    //         glm::vec3(0.0f, 0.0f, 1.0f));

    // matrix1 = glm::rotate(matrix1, glm::radians(rotationAngle),
    //         glm::vec3(1.0f, 0.0f, 0.0f));

    matrix1 = glm::scale(matrix1, glm::vec3(4.0f, 1.0f, 2.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader1, "matrix1"),
            1, GL_FALSE, glm::value_ptr(matrix1));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0); 

    // ... connect each texture unit to a sampler2D in the fragment shader ...
    glUniform1i(glGetUniformLocation(shader1, "shaderTexture0"), 0);


    // ... draw our triangles
    glBindVertexArray(vao1);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(mainPlatformVertices) / (11 * sizeof(float)));


    // using our shader program for the second triangle...
    glUseProgram(shader2);

    // ... enable cull face ...
    glEnable(GL_CULL_FACE);
        
    // ... enable OpenGL's hidden surface removal ...
    glEnable(GL_DEPTH_TEST); 

    // ... calculate matrix for second triangle ...
    glm::mat4 matrix2;
    matrix2 = glm::translate(baseMatrix, glm::vec3(-0.5f, 1.5f, -0.2f));

    //matrix2 = glm::rotate(matrix2, glm::radians(15.0f),
            //glm::vec3(1.0f, 0.0f, 0.0f));

    //matrix2 = glm::rotate(matrix2, glm::radians(rotationAngle),
            //glm::vec3(0.0f, 1.0f, 0.0f));

    matrix2 = glm::scale(matrix2, glm::vec3(2.0f, 1.0f, 1.5f));

    glUniformMatrix4fv(glGetUniformLocation(shader2, "matrix2"),
            1, GL_FALSE, glm::value_ptr(matrix2));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1); 

    // ... connect each texture unit to a sampler2D in the fragment shader ...
    glUniform1i(glGetUniformLocation(shader2, "shaderTexture0"), 0);

    // ... draw our triangles
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(mainPlatformVertices) / (11 * sizeof(float)));


    // using our shader program for the third triangle...
    glUseProgram(shader3);

    // ... enable cull face ...
    glEnable(GL_CULL_FACE);
        
    // ... enable OpenGL's hidden surface removal ...
    glEnable(GL_DEPTH_TEST); 

    // ... calculate matrix for third triangle ...
    glm::mat4 matrix3;
    matrix3 = glm::translate(baseMatrix, glm::vec3(-2.5f, 1.5f, -0.2f));

    // matrix3 = glm::rotate(matrix3, glm::radians(105.0f),
    //         glm::vec3(0.0f, 1.0f, 0.0f));

    // matrix3 = glm::rotate(matrix3, glm::radians(rotationAngle),
    //         glm::vec3(0.0f, 0.0f, 1.0f));

    matrix3 = glm::scale(matrix3, glm::vec3(2.0f, 1.0f, 1.5f));

    glUniformMatrix4fv(glGetUniformLocation(shader3, "matrix3"),
            1, GL_FALSE, glm::value_ptr(matrix3));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1); 

    // ... connect each texture unit to a sampler2D in the fragment shader ...
    glUniform1i(glGetUniformLocation(shader3, "shaderTexture0"), 0);

    // ... draw our triangles
    glBindVertexArray(vao3);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(mainPlatformVertices) / (11 * sizeof(float)));
}

/*****************************************************************************/

// handler called by GLFW when there is a keyboard event
void handleKeys(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    // pressing Esc closes the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
}

// handler called by GLFW when the window is resized
void handleResize(GLFWwindow* pWindow, int width, int height)
{
    // tell OpenGL to do its drawing within the entire "client area" (area within the borders) of the window
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* pWindow, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.025f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forwardVector = glm::normalize(front);
}


void scroll_callback(GLFWwindow* pWindow, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

// main function
int main(int argc, char** argv)
{
    // initialize GLFW and ask for OpenGL 3.3 core
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create a GLFW window with the specified width, height, and title
    pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (! pWindow)
    {
        // gracefully terminate if we cannot create the window
        std::cout << "Cannot create the GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    // make the window the current context of subsequent OpenGL commands,
    // and enable vertical sync and aspect-ratio correction on the GLFW window
    glfwMakeContextCurrent(pWindow);
    glfwSwapInterval(1);
    glfwSetWindowAspectRatio(pWindow, WINDOW_WIDTH, WINDOW_HEIGHT);

    // set up callback functions to handle window system events
    glfwSetKeyCallback(pWindow, handleKeys);
    glfwSetFramebufferSizeCallback(pWindow, handleResize);

    // mouse set up
    glfwSetCursorPosCallback(pWindow, mouse_callback);
    glfwSetScrollCallback(pWindow, scroll_callback);
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // don't miss any momentary keypresses
    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

    // initialize GLAD, which acts as a library loader for the current OS's native OpenGL library
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // if our initial setup is successful...
    if (setup())
    {
        // do rendering in a loop until the user closes the window
        while (! glfwWindowShouldClose(pWindow))
        {
            // render our next frame
            // (by default, GLFW uses double-buffering with a front and back buffer;
            // all drawing goes to the back buffer, so the frame does not get shown yet)

            //Keyboard Controls
            float cameraSpeed = 2.5 * deltaTime;
            if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
                eyePosition += cameraSpeed * forwardVector;
            if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
                eyePosition -= cameraSpeed * forwardVector;
            if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
                eyePosition -= glm::normalize(glm::cross(forwardVector, upVector)) * cameraSpeed;
            if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
                eyePosition += glm::normalize(glm::cross(forwardVector, upVector)) * cameraSpeed;

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
