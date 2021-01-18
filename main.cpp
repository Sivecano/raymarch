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

void zoom(GLFWwindow* window, double xoffset, double yoffset)
{
    scale += scale *yoffset * 0.1;
    if (scale <= .5) scale = 0.5;
}

void scroll(GLFWwindow* window, double xpos, double ypos)
{
    static double lx, ly;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        x_off += 3.5 * (lx - xpos) /(1920 * scale);
        y_off += 2 *(ypos - ly) /(1080 * scale);
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


    GLFWwindow* win = glfwCreateWindow(1920, 1080, "the mandelbrot set", glfwGetPrimaryMonitor(), NULL);
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

    unsigned int shader = shaders::shader_program("shaders/mandelbrot.vert", "shaders/mandelbrot.frag");

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
    glUseProgram(shader);

    

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    bool dir = true;
    

    while (!glfwWindowShouldClose(win))
    {
        //frametime = glfwGetTime();

        glClearColor(.2f, .2f, .8f, 1.0f); // rgba background
        glClear(GL_COLOR_BUFFER_BIT);

        //v *= 1 + a;
        //ds += v * (dir * 2 - 1);

        //scale += ds * (dir * 2 - 1);

        if (scale >= 1000 || scale <= 1) dir = !dir;

        glUniform1f(scaleloc, scale);
        glUniform2f(offsetloc, x_off, y_off);

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
