#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 800;
const float PI = 3.1415926f;
const float toRadians = PI / 180.0f;

// This correspond to IDs of "objects" in the graphic memory
unsigned int VAO, VBO, IBO, shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0035f;

float curAngle = 0.0f;

// Vertex shader
static const char *vShader = "                             \
#version 330                                               \n\
layout (location = 0) in vec3 pos;                         \n\
out vec4 vCol;                                             \n\
uniform mat4 model;                                        \n\
uniform mat4 projection;                                   \n\
void main(){                                               \n\
  gl_Position = projection * model * vec4(pos, 1.0);       \n\
  vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);               \n\
}                                                          \n\
";

// Fragment shader
static const char *fShader = "        \
#version 330                          \n\
in vec4 vCol;                         \n\
out vec4 colour;                      \n\
void main(){                          \n\
  colour = vCol;                      \n\
}                                     \n\
";

void createTriangle() {
  // Indices from the vertices array
  unsigned int indices[] = {0, 3, 1, 1, 3, 2, 2, 3, 0, 0, 1, 2};

  float vertices[] = {-1.0f, -1.0f, 0.0f,  0.0f, -1.0f, 1.0f,
                      1.0f,  -1.0f, -0.0f, 0.0f, 1.0f,  0.0f};

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // unbind the buffer and vertex array
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void addShader(GLuint program, const char *shaderCode, GLenum shaderType) {
  unsigned int shader = glCreateShader(shaderType);
  const char *code[1];
  code[0] = shaderCode;

  int codeLength[1];
  codeLength[0] = strlen(shaderCode);

  glShaderSource(shader, 1, code, codeLength);
  glCompileShader(shader);

  int result = 0;
  char eLog[1024] = {0};

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

  uniformModel = glGetUniformLocation(shader, "model");
  uniformProjection = glGetUniformLocation(shader, "projection");
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

  glEnable(GL_DEPTH_TEST);

  // Setup viewport size
  glViewport(0, 0, bufferWidth, bufferWidth);

  createTriangle();
  compileShaders();

  glm::mat4 projection = glm::perspective(
      45.0f, (GLfloat)bufferWidth / bufferHeight, 0.1f, 100.0f);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(mainWindow)) {
    // Poll for and process events
    glfwPollEvents();

    if (direction) {
      triOffset += triIncrement;
    } else {
      triOffset -= triIncrement;
    }

    if (abs(triOffset) >= triMaxOffset) {
      direction = !direction;
    };

    curAngle += 0.11f;
    if (curAngle >= 360) {
      curAngle -= 360;
    }

    // Render here
    glClearColor(0.0f, 0.0f, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    // model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
    model =
        glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

    // Swap front and back buffers
    glfwSwapBuffers(mainWindow);
  }

  glfwTerminate();
  return 0;
}
