//
// Created by kaloka on 14/11/2020.
//

#include "helpers.h"

// this code base was originally created for a hangman project I did with a friend of mine.
// was my first CPP project and both our intro to open gl. a lot of this is ... deprecated , unused ect
// same goes for event, though I wrote that later.

namespace helpers {
    void framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height) {
        glViewport(0, 0, width, height);
    }
}
// this used to be a generalized but unfinished input system for the hangman thing...
/*
namespace input {
    void setupGlfwInputCallbacks(GLFWwindow *window, inputHandler *handler) {
        if (!inputHandler::glfwBound) {
            glfwSetKeyCallback(window, inputHandler::keypressEventCaller);
            glfwSetCharCallback(window, inputHandler::characterEventCaller);
        }
    }


    inputHandler::inputHandler() :
    inputActionHolder() {
        inputHandler::keypressEvent.add(&inputHandler::keypressEventCaller, this);
        inputHandler::characterEvent.add(&inputHandler::characterEventCaller, this);
    };
    bool inputHandler::glfwBound = false;
    event<GLFWwindow *, int, int, int, int> inputHandler::keypressEvent =
            event<GLFWwindow *, int, int, int, int>();
    event<GLFWwindow *, unsigned int> inputHandler::characterEvent =
            event<GLFWwindow *, unsigned int>();

    inputHandler::~inputHandler() {
        inputHandler::keypressEvent.sub(&inputHandler::keypressCallback, this);
        inputHandler::characterEvent.sub(&inputHandler::characterCallback, this);
    }

    void inputHandler::keypressEventCaller(GLFWwindow *window, int key, int scancode, int action, int mods) {
        keypressEvent(window, key, scancode, action, mods);
    }

    void inputHandler::characterEventCaller(GLFWwindow *window, unsigned int codepoint) {
        characterEvent(window, codepoint);
    }

    void inputHandler::mouseButtonEventCaller(GLFWwindow *window, int key, int action, int mods) {
        mouseButtonEvent(window, key, action, mods);
    }

    void inputHandler::mouseMoveEventCaller(GLFWwindow *window, double posX, double posY) {
        mouseMovementEvent(window, posX, posY);
    }

    PerformableInputActionsBase *inputHandler::getInputActionHolder() const {
        return inputActionHolder;
    }

    void inputHandler::setInputActionHolder(PerformableInputActionsBase *inputActionHolder) {
        inputHandler::inputActionHolder->unbind();
        inputHandler::inputActionHolder = inputActionHolder;
        inputHandler::inputActionHolder->bind();
    }

    PerformableInputActionsBase::PerformableInputActionsBase()
    {}

    void
    PerformableInputActionsBase::keypressCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    }

    void PerformableInputActionsBase::characterCallback(GLFWwindow *window, unsigned int codepoint) {

    }

    void PerformableInputActionsBase::mouseButtonCallback(GLFWwindow *window, int key, int action, int mods) {

    }

    void PerformableInputActionsBase::mouseMoveCallback(GLFWwindow *window, double posX, double posY) {

    }

    void PerformableInputActionsBase::bind() {

    }

    void PerformableInputActionsBase::unbind() {

    }
}
*/
// this code was not originally written by me. though I can explain what it does
// a lot of this is also straight forom the learopengl tutorial
namespace shaders
{
    unsigned int load_shader(const char* filename, int shadertype)
    {
        std::ifstream file(filename, std::ios::in | std::ios::binary);

        file.seekg(0, file.end); // get end

        int len = file.tellg(); // how many chars in to file?

        file.seekg(file.beg); // go back to start

        char* code = new char[len + 1]; // out buffer for shader code

        file.read(code, len); // load shader code

        code[len] = '\0';

        unsigned int out = glCreateShader(shadertype);

        glShaderSource(out, 1, &code, NULL); // buffer shader

        glCompileShader(out); // compile

        // printf(filename);
        // printf(" :\n");
        // printf(code);
        // printf("\n");

        file.close(); // no longer need code source

        int  success; // shader done
        glGetShaderiv(out, GL_COMPILE_STATUS, &success);
        if(!success) // if not print error
        {
            char infoLog[512];
            glGetShaderInfoLog(out, 512, NULL, infoLog);
            printf("compilation of  %s  failed\n%s\n\n%s\n", filename, infoLog, code);
            delete[] code;
            throw "shader compilation failed"; // and throw... this is borked... // it does throw just not how we want it ot
        }
        delete[] code;

        return out;
    }


    unsigned int shader_program(const char *vertShaderLoc, const char *fragShaderLoc)
    {
        unsigned int shader[2]; // two shaders

        unsigned int out = glCreateProgram();
        shader[0] = load_shader(vertShaderLoc, GL_VERTEX_SHADER);
        glAttachShader(out, shader[0]);
        shader[1] = load_shader(fragShaderLoc, GL_FRAGMENT_SHADER);
        glAttachShader(out, shader[1]);
        glLinkProgram(out); // make program // done

        glDeleteShader(shader[0]);
        glDeleteShader(shader[1]); // no longer need those we have program

        GLint success; // error check same as load shader
        glGetProgramiv(out, GL_LINK_STATUS, &success);

        if(success == GL_FALSE) {
            char infoLog[512];
            printf("shaderporgramm linking didn't work: ");
            glGetProgramInfoLog(out, 512, nullptr, infoLog);
            printf("%s\n", infoLog);
            throw "ShaderPorgram didn't work";
        }

        printf("shader linked successfully!\n");
        success = 0;
        return out;
    }

}


namespace sprites
{

    unsigned int Text::VBO, Text::VAO, Text::EBO;

    unsigned int Text::textids[CALIBRI_FRAME_COUNT];
    bool Text::initflag = false;

    void Text::TextInit()
    {
        if (initflag) return;

        glGenTextures(CALIBRI_FRAME_COUNT, textids);
        for (int i = 0; i < CALIBRI_FRAME_COUNT; i++)
        {
            glBindTexture(GL_TEXTURE_2D, textids[i]);  

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CALIBRI_FRAME_WIDTH, CALIBRI_FRAME_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, calibri_data[i]);
            //glGenerateMipmap(GL_TEXTURE_2D);
        }

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        initflag = true;
    }

    Text::Text()
    {
        if (!initflag)
        {
            printf("\nTextsystem not initialized!\n");
            TextInit();
        } 
        x = 0;
        y = 0;

        h = 0.05;
        w = text.size() * h;

        text = "";
    }

    Text::Text(std::string text, float posx, float posy, float height): text(text)
    {
        if (!initflag)
        {
            printf("\nTextsystem not initialized!\n");
            TextInit();
        } 

        h = height;
        x = posx;
        y = posy;
        w = text.size() * h;
    }

    void Text::setText(std::string newtext)
    {
        text = newtext;
        w = text.size() * h;
    }

    std::string Text::getText()
    {
        return text;
    }

    void Text::draw(unsigned int shader)
    { // oh god this code.... y...
        glUseProgram(shader);
        float vertices[] = {
        // positions          // colors           // texture coords
        x + h,  y + h, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
        x + h,      y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
            x,      y, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
            x,  y + h, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // top left 
        };
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


        for (int i = 0; i < text.size(); i++)
        {
            if(text[i] == '\n' || vertices[0] > 1)
            {
                vertices[0] = x + h;
                vertices[8] = x + h;
                vertices[16] = x;
                vertices[24] = x;

                vertices[1] -= h;
                vertices[9] -= h;
                vertices[17] -= h;
                vertices[25] -= h;

                if (text[i] == '\n') continue;
            }
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
            glBindTexture(GL_TEXTURE_2D, textids[text[i]]);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            vertices[0] += h;
            vertices[8] += h;
            vertices[16] += h;
            vertices[24] += h;
        }
    }

};