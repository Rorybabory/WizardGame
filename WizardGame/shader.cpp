#include "shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include "GL/glew.h"
#include "mesh.h"
extern int screenInverted;
void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
std::string LoadShader(const std::string& fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);
Shader::Shader() {

}
void Shader::InitPPShader(const std::string& fileName) {
  m_program = glCreateProgram();

  m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
  m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

  for(unsigned int i = 0; i < NUM_SHADERS; i++) {
    glAttachShader(m_program, m_shaders[i]);
  }
  glBindAttribLocation(m_program, 0, "position");
  glBindAttribLocation(m_program, 1, "texCoord");

  glLinkProgram(m_program);
  CheckShaderError(m_program, GL_LINK_STATUS, true, "ERROR: PROGRAM LINKING FAILED: ");

  glValidateProgram(m_program);
  CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "ERROR: PROGRAM VALIDATING FAILED: ");
  std::vector<char> FragmentShaderError;
}
void Shader::InitShader(const std::string& fileName){
  m_program = glCreateProgram();

  m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
  m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

  for(unsigned int i = 0; i < NUM_SHADERS; i++) {
    glAttachShader(m_program, m_shaders[i]);
  }
  glBindAttribLocation(m_program, 0, "position");
  glBindAttribLocation(m_program, 1, "texCoord");
  glBindAttribLocation(m_program, 2, "normal");
  glBindAttribLocation(m_program, 3, "color");
  glLinkProgram(m_program);
  CheckShaderError(m_program, GL_LINK_STATUS, true, "ERROR: PROGRAM LINKING FAILED: ");

  glValidateProgram(m_program);
  CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "ERROR: PROGRAM VALIDATING FAILED: ");
  std::vector<char> FragmentShaderError;
  m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");

}
Shader::Shader(const std::string& fileName) {
  m_program = glCreateProgram();

  m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
  m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

  for(unsigned int i = 0; i < NUM_SHADERS; i++) {
    glAttachShader(m_program, m_shaders[i]);
  }
  glBindAttribLocation(m_program, 0, "position");
  glBindAttribLocation(m_program, 1, "texCoord");
  glBindAttribLocation(m_program, 2, "normal");

  glLinkProgram(m_program);
  CheckShaderError(m_program, GL_LINK_STATUS, true, "ERROR: PROGRAM LINKING FAILED: ");

  glValidateProgram(m_program);
  CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "ERROR: PROGRAM VALIDATING FAILED: ");

  m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
}
Shader::~Shader() {
  for(unsigned int i = 0; i < NUM_SHADERS; i++) {
    glDetachShader(m_program, m_shaders[i]);
    glDeleteShader(m_shaders[i]);
  }
  glDeleteProgram(m_program);
}
static GLuint CreateShader(const std::string& text, GLenum shaderType) {
  GLuint shader = glCreateShader(shaderType);

  if (shader==0) {
    std::cerr << "ERROR: SHADER CREATION SHADER" << '\n';
  }
  const GLchar* shaderSourceStrings[1];
  GLint shaderSourceStringLengths[1];

  shaderSourceStrings[0] = text.c_str();
  shaderSourceStringLengths[0] = text.length();
  glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
  glCompileShader(shader);

  //CheckShaderError(shader, GL_COMPILE_STATUS, false, "ERROR: SHADER COMPILATION FAILED: ");

  return shader;
}
void Shader::Bind() {
  glUseProgram(m_program);
  int colorLocation = glGetUniformLocation(m_program,"color");
  glUniform4f(colorLocation, 1.0f,0.0f,1.0f,1.0f);
  int BoolLocation = glGetUniformLocation(m_program,"inverted");
  if (screenInverted == true) {glUniform1i(BoolLocation, 1);}
  if (screenInverted == false) {glUniform1i(BoolLocation, 0);}

}
void Shader::TextBind(int x, int y) {
  glUseProgram(m_program);
  int colorLocation = glGetUniformLocation(m_program,"color");
  glUniform4f(colorLocation, 1.0f,0.0f,1.0f,1.0f);
  int getX = glGetUniformLocation(m_program,"x");
  glUniform1i(getX, x);
  int getY = glGetUniformLocation(m_program,"y");
  glUniform1i(getY, y);
}
void Shader::Bind(glm::vec4 color) {
  glUseProgram(m_program);
  int colorLocation = glGetUniformLocation(m_program,"color");
  glUniform4f(colorLocation, color.x,color.y,color.z,color.w);
  int BoolLocation = glGetUniformLocation(m_program,"inverted");
  glUniform1i(BoolLocation, screenInverted);
}
void Shader::UnBind() {
  glUseProgram(0);
}
void Shader::Update(const Transform& transform, const Camera& camera) {

  glm::mat4 model = camera.GetViewProjection() * transform.GetModel();

  glUniformMatrix4fv(m_uniforms[TRANSFORM_U],1,GL_FALSE, &model[0][0]);
}

std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			      output.append(line + "\n");
        }
    }
    else
    {
		    std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;
}
void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}
