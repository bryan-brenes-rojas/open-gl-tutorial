#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

const GLint WIDTH = 800, HEIGHT = 600;

int main(void) {
  GLFWwindow *mainWindow;

  /* Initialize the library */
  if (!glfwInit()) {
    printf("GLFW initialization failed\n");
    glfwTerminate();
    return -1;
  }

  // Setup GLFW window properties
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Core profile = No backward compatibility
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Allow forward compatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  /* Create a windowed mode window and its OpenGL context */
  mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);
  if (!mainWindow) {
    glfwTerminate();
    printf("GLFW window creation failed\n");
    return -1;
  }

  // Get the buffer size dimensions
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Set the context for GLEW to use
  glfwMakeContextCurrent(mainWindow);

  // allow modern extension features
  glewExperimental = GL_TRUE;

  // Initialize glew
  if(glewInit() != GLEW_OK){
    printf("GLEW initialization failed\n");
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return -1;
  }

  // Setup viewport size
  glViewport(0,0, bufferWidth, bufferWidth);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(mainWindow)) {
    // Poll for and process events
    glfwPollEvents();

    // Render here
    glClearColor(0.0f, 0.0f, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    // Swap front and back buffers
    glfwSwapBuffers(mainWindow);
  }

  glfwTerminate();
  return 0;
}
