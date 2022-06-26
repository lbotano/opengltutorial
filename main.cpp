#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  //
  // Vertex shader
  //
  const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Print error if there is one
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  //
  // Fragment shader
  //
  const char *fragmentShaderOrangeSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";
  const char *fragmentShaderYellowSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
    "}\0";

  unsigned int fragmentShaderOrange;
  fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderOrange, 1, &fragmentShaderOrangeSource, NULL);
  glCompileShader(fragmentShaderOrange);

  // Print error if there is one
  glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderOrange, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  unsigned int fragmentShaderYellow;
  fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderYellow, 1, &fragmentShaderYellowSource, NULL);
  glCompileShader(fragmentShaderYellow);

  // Print error if there is one
  glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  //
  // Create shader program
  //
  unsigned int orangeShaderProgram, yellowShaderProgram;
  orangeShaderProgram = glCreateProgram();
  yellowShaderProgram = glCreateProgram();

  glAttachShader(orangeShaderProgram, vertexShader);
  glAttachShader(orangeShaderProgram, fragmentShaderOrange);
  glLinkProgram(orangeShaderProgram);
  glAttachShader(yellowShaderProgram, vertexShader);
  glAttachShader(yellowShaderProgram, fragmentShaderYellow);
  glLinkProgram(yellowShaderProgram);

  // Check for errors
  glGetProgramiv(orangeShaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(orangeShaderProgram, 512, NULL, infoLog);
  }
  glGetProgramiv(yellowShaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(yellowShaderProgram, 512, NULL, infoLog);
  }

  // We don't need the shaders anymore
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShaderOrange);
  glDeleteShader(fragmentShaderYellow);

  float vertices[2][9] = {{
    -0.25f,  0.50f, 0.0f, // top
    -0.50f, -0.50f, 0.0f, // bottom left
     0.00f, -0.50f, 0.0f  // bottom right
  },
  {
    0.25f,  0.50f, 0.0f, // top
    0.00f, -0.50f, 0.0f, // bottom left
    0.50f, -0.50f, 0.0f  // bottom right
  }};

  // Define vertex buffer and vertex array objects
  unsigned int VBOs[2], VAOs[2];
  glGenBuffers(2, VBOs);
  glGenVertexArrays(2, VAOs);

  // Use the shader program
  //glUseProgram(shaderProgram);

  // Do the bindings for each triangle
  for (int i = 0; i < 2; i++) {
    glBindVertexArray(VAOs[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
  }

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the object
    glUseProgram(orangeShaderProgram);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(yellowShaderProgram);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

