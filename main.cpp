#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "helpers.h"

float scale = 1;

float x_off = 0;
float y_off = 0;

float ds = .01;
float v = 0.001;
float a = 0.01;
float cx = 1;
float cy = 0;
float cz = 0;
float speed = 2;
float pitch = 0;
float yaw = 0;


void zoom(GLFWwindow* window, double xoffset, double yoffset)
{
    scale += scale *yoffset * 0.1;
    if (scale <= .1) scale = 0.1;
}
 
void scroll(GLFWwindow* window, double xpos, double ypos)
{
    static double lx, ly;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        yaw += (lx-xpos) / 20;
        pitch += (ypos - ly) / 20;
    }

    std::cout << "pos: " << xpos << ", " << ypos << std::endl;
    lx = xpos;
    ly = ypos;
}

void close(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) && action ==  GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* win = glfwCreateWindow(1000, 1000, "raymarching", NULL, NULL);
    if (!win)
    {
        printf("window creation failed!\n");
        return 0;
    }
    glfwMakeContextCurrent(win);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout <<    "glad died something has gone wrong check dependencies!";
        throw "glad died something has gone wrong check dependencies";
    }
    //glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(win, scroll);
    glfwSetScrollCallback(win, zoom);
    glfwSetKeyCallback(win, close);

    unsigned int shader = shaders::shader_program("shaders/raymarch.vert", "shaders/raymarch.frag");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float points[]{
        1, 1, 0.,
        1, -1, 0.,
        -1, -1, 0.,
        -1, 1, 0.
    };

    unsigned int VAO, VBO, EBO;

      unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
        };  

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);

        double frametime;
        double dt;
    
    int scaleloc = glGetUniformLocation(shader, "scale");
    int offsetloc = glGetUniformLocation(shader, "offset");
    int cposloc = glGetUniformLocation(shader, "cposition");
    int rotloc = glGetUniformLocation(shader, "rot");
    glUseProgram(shader);

    

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    bool dir = true;
    

    while (!glfwWindowShouldClose(win))
    {
        //frametime = glfwGetTime();
        cz += speed*((glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) - (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)) * (1 + (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS));
        cx += speed*((glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) - (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS))* (1 + (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS));
        cy += speed*((glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) - (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS))* (1 + (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS));

        glClearColor(.2f, .2f, .8f, 1.0f); // rgba background
        glClear(GL_COLOR_BUFFER_BIT);

        //v *= 1 + a;
        //ds += v * (dir * 2 - 1);

        //scale += ds * (dir * 2 - 1);

        if (scale >= 1000 || scale <= 1) dir = !dir;

        glUniform1f(scaleloc, scale);
        glUniform2f(offsetloc, x_off, y_off);
        glUniform3f(cposloc, cx, cy, cz);
        glUniform2f(rotloc, yaw, pitch);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
        
        glfwSwapBuffers(win);
        glfwPollEvents();
        //dt = glfwGetTime() - frametime;
        //std::cout << 1 / dt << "\n";
        //std::cout << "x: " << x_off << "scale: " << scale << std::endl;
        //std::cout << "render\n";
    }


    return 0;
    //std::cout << "Hello World";
}
