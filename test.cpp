#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main(){

    // Init GLFW
    if(!glfwInit()){
        std::cerr << "Failed to inititialise GLFW!";
        return -1;
    }

    // Init GLEW
    if(!glewInit()){
        std::cerr << "Failed to initalise GLEW!";
        return -1;
    }

    GLFWwindow* gameWindow = glfwCreateWindow(800, 600, "CraftMine", NULL, NULL);
    if (gameWindow == NULL){
        std::cerr << "Failed to create GLFW screen!";
        glfwTerminate();
        return -1;
    }

    // Add opengl context to screen
    glfwMakeContextCurrent(gameWindow);

    glViewport(0, 0, 800, 600);


    while(!glfwWindowShouldClose(gameWindow)){
        glfwSwapBuffers(gameWindow);
        glfwPollEvents();    
    }


    return 0;
}