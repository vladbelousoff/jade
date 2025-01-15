#include "render-context.hpp"

#include <jade/utils/assert.hpp>

auto jade::RenderContext::create_shader(const ShaderType type, const char* shader_path) const -> ShaderHandle
{
  return shader_manager->create_shader(type, shader_path);
}

void jade::RenderContext::delete_shader(const ShaderHandle shader_handle) const
{
  JADE_ASSERT(shader_manager->is_valid(shader_handle));
  shader_manager->delete_shader(shader_handle);
}

auto jade::RenderContext::create_program(const std::initializer_list<ShaderHandle> shader_handles) const
  -> ShaderProgramHandle
{
  return shader_manager->create_program(shader_handles);
}

void jade::RenderContext::delete_program(const ShaderProgramHandle program_handle) const
{
  JADE_ASSERT(shader_manager->is_valid(program_handle));
  shader_manager->delete_program(program_handle);
}

void jade::RenderContext::bind_program(const ShaderProgramHandle program_handle) const
{
  JADE_ASSERT(shader_manager->is_valid(program_handle));
  shader_manager->bind_program(program_handle);
}
