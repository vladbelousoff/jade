#include "shader-manager.hpp"

#include <jade/utils/assert.hpp>

auto
jade::ShaderManager::is_valid(const ShaderHandle shader_handle) const -> bool
{
  return shader_handle.id != 0 && shaders.contains(shader_handle.id);
}

auto
jade::ShaderManager::is_valid(const ShaderProgramHandle program_handle) const -> bool
{
  return program_handle.id != 0 && programs.contains(program_handle.id);
}

auto
jade::ShaderManager::create_shader_handle(Shader* shader) -> ShaderHandle
{
  ShaderHandle::IdType id = shader_next_index++;
  shaders.insert({ id, shader });

  ShaderHandle shader_handle;
  shader_handle.id = id;

  return shader_handle;
}

void
jade::ShaderManager::delete_shader_handle(const ShaderHandle shader_handle)
{
  JADE_ASSERT(is_valid(shader_handle));
  auto it = shaders.find(shader_handle.id);
  JADE_ASSERT(it != shaders.end());
  JADE_ASSERT(it->second != nullptr);
  delete it->second;
  shaders.erase(it);
}

auto
jade::ShaderManager::create_program_handle(ShaderProgram* shader_program) -> ShaderProgramHandle
{
  ShaderProgramHandle::IdType id = program_next_index++;
  programs.insert({ id, shader_program });

  ShaderProgramHandle program_handle;
  program_handle.id = id;

  return program_handle;
}

void
jade::ShaderManager::delete_program_handle(const ShaderProgramHandle program_handle)
{
  JADE_ASSERT(is_valid(program_handle));
  auto it = programs.find(program_handle.id);
  JADE_ASSERT(it != programs.end());
  JADE_ASSERT(it->second != nullptr);
  delete it->second;
  programs.erase(it);
}
