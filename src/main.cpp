#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>

const GLint WIDTH = 800, HEIGHT = 600;

// This correspond to IDs of "objects" in the graphic memory
GLuint VAO, VBO, shader;

// Vertex shader
static const char *vShader = "            \
#version 330                              \n\
layout (location = 0) in vec3 pos;        \n\
void main(){                              \n\
  gl_Position = vec4(0.5 * pos, 1.0);     \n\
}                                         \n\
";

// Fragment shader
static const char *fShader = "        \
#version 330                          \n\
out vec4 colour;                      \n\
void main(){                          \n\
  colour = vec4(1.0, 0.0, 0.0, 1.0);  \n\
}                                     \n\
";

void createTriangle() {
  GLfloat vertices[] = {-1.0f, -1.0f, 0.0f, 1.0f, -1.0f,
                        -0.0f, 0.0f,  1.0f, 0.0f};

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // unbind the buffer and vertex array
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void addShader(GLuint program, const char *shaderCode, GLenum shaderType) {
  GLuint shader = glCreateShader(shaderType);
  const GLchar *code[1];
  code[0] = shaderCode;

  GLint codeLength[1];
  codeLength[0] = strlen(shaderCode);

  glShaderSource(shader, 1, code, codeLength);
  glCompileShader(shader);

  GLint result = 0;
  GLchar eLog[1024] = {0};

  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error adding shader %u program: '%s'\n", shaderType, eLog);
    return;
  }
  glAttachShader(program, shader);
  return;
}

void compileShaders() {
  shader = glCreateProgram();
  if (!shader) {
    printf("Error creating shader program\n");
    return;
  }
  addShader(shader, vShader, GL_VERTEX_SHADER);
  addShader(shader, fShader, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar eLog[1024] = {0};

  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error linking shader program: '%s'\n", eLog);
    return;
  }

  glValidateProgram(shader);
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error validating shader program: '%s'\n", eLog);
    return;
  }
}

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
  if (glewInit() != GLEW_OK) {
    printf("GLEW initialization failed\n");
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return -1;
  }

  // Setup viewport size
  glViewport(0, 0, bufferWidth, bufferWidth);

  createTriangle();
  compileShaders();

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(mainWindow)) {
    // Poll for and process events
    glfwPollEvents();

    // Render here
    glClearColor(0.0f, 0.0f, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glUseProgram(0);

    // Swap front and back buffers
    glfwSwapBuffers(mainWindow);
  }

  glfwTerminate();
  return 0;
}
