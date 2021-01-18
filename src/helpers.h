//
// Created by kaloka on 14/11/2020.
//

#ifndef OGLHANGMAN_HELPERS_H
#define OGLHANGMAN_HELPERS_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <typeinfo>
#include "calibri.c"
#include <ctime>
#include "functional"
#include <fstream>

namespace helpers {
    void framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height);
}

namespace shaders
{
    unsigned int load_shader(const char* filename, int shadertype);
    unsigned int shader_program(const char *vertShaderLoc, const char *fragShaderLoc);
}

namespace sprites
{
    class Sprite
    {
    public:
        float x;
        float y;
        float w;
        float h;

        virtual void draw(unsigned int shader) = 0;
    };

    class Text: public Sprite
    {
    public:

        static unsigned int textids[CALIBRI_FRAME_COUNT];
        static bool initflag;

        static unsigned int VBO, VAO, EBO;

        static void TextInit();
        
        Text();

        Text(std::string text, float posx = 0, float posy = 0, float height = 0.05);
        void setText(std::string newtext);
        std::string getText();

        void draw(unsigned int shader) override;

    private:
        std::string text = "";

    };

};
#endif //OGLHANGMAN_HELPERS_H
