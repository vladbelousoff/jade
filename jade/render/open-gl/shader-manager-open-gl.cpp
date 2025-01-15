#include "shader-manager-open-gl.hpp"

#include <GL/gl3w.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <vector>

class ShaderOpenGL final : public jade::Shader
{
  friend class jade::ShaderManagerOpenGL;

 public:
  explicit ShaderOpenGL(const jade::ShaderType type)
      : Shader(type)
      , shader_id(0)
  {
  }

  ~ShaderOpenGL() override
  {
    if (shader_id != 0) {
      glDeleteShader(shader_id);
    }
  }

 private:
  GLuint shader_id;
};

class ShaderProgramOpenGL final : public jade::ShaderProgram
{
  friend class jade::ShaderManagerOpenGL;

 public:
  explicit ShaderProgramOpenGL()
      : program_id(0)
  {
  }

  ~ShaderProgramOpenGL() override
  {
    if (program_id != 0) {
      glDeleteProgram(program_id);
    }
  }

 private:
  GLuint program_id;
};

auto jade::ShaderManagerOpenGL::create_shader(const ShaderType type, const char* shader_path) -> ShaderHandle
{
  // Load SPIR-V binary from file
  std::ifstream file(std::string(shader_path) + ".spv", std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    spdlog::error("Failed to open SPIR-V file: {}", shader_path);
    return {};
  }

  const std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  if (!file.read(buffer.data(), size)) {
    spdlog::error("Failed to read SPIR-V file: {}", shader_path);
    return {};
  }
  file.close();

  // Create a new shader object
  const GLenum shader_type = type == ShaderType::FRAG ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER;
  auto* shader = new ShaderOpenGL(type);
  shader->shader_id = glCreateShader(shader_type);

  // Load the SPIR-V binary into OpenGL
  glShaderBinary(
    1, &shader->shader_id, GL_SHADER_BINARY_FORMAT_SPIR_V, buffer.data(), static_cast<GLsizei>(buffer.size()));
  glSpecializeShader(shader->shader_id, "main", 0, nullptr, nullptr);

  // Check for compilation errors
  GLint success = 0;
  glGetShaderiv(shader->shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar info_log[512];
    glGetShaderInfoLog(shader->shader_id, sizeof(info_log), nullptr, info_log);
    spdlog::error("Shader SPIR-V loading failed: {}", info_log);
    delete shader;
    return {};
  }

  return create_shader_handle(shader);
}

void jade::ShaderManagerOpenGL::delete_shader(const ShaderHandle shader_handle)
{
  delete_shader_handle(shader_handle);
}

auto jade::ShaderManagerOpenGL::create_program(const std::initializer_list<ShaderHandle> shader_handles)
  -> ShaderProgramHandle
{
  auto* program = new ShaderProgramOpenGL();
  program->program_id = glCreateProgram();

  for (const auto& shader_handle : shader_handles) {
    const auto it = shaders.find(get_handle_id(shader_handle));
    if (it == shaders.end()) {
      continue;
    }

    const auto* shader = dynamic_cast<ShaderOpenGL*>(it->second);
    glAttachShader(program->program_id, shader->shader_id);
  }

  glLinkProgram(program->program_id);

  GLint success = 0;
  glGetProgramiv(program->program_id, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar info_log[512];
    glGetProgramInfoLog(program->program_id, sizeof(info_log), nullptr, info_log);
    spdlog::error("Program linking failed: {}", info_log);
    delete program;
    return {};
  }

  return create_program_handle(program);
}

void jade::ShaderManagerOpenGL::delete_program(const ShaderProgramHandle program_handle)
{
  delete_program_handle(program_handle);
}

void jade::ShaderManagerOpenGL::bind_program(const ShaderProgramHandle program_handle)
{
  const auto it = programs.find(get_handle_id(program_handle));
  if (it == programs.end()) {
    return;
  }

  const auto* program = dynamic_cast<ShaderProgramOpenGL*>(it->second);
  glUseProgram(program->program_id);
}
