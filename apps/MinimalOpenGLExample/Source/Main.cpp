/** 
 * Include as regular header file as it defines all the functions with macros and for some reason doesn't
 * allow to import itself as module header unit - take a try with `import <GL/glew.h>;`
 */
#include <GL/glew.h>

import std;

import <GLFW/glfw3.h>;

import OpenGL;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "AppTemplate - OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
    	std::cout << "Glew failed to initialize!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}