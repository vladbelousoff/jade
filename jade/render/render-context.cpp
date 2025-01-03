#pragma once

#include "render-context.hpp"

auto
jade::RenderContext::create_shader(ShaderType type, const char* buffer) const -> ShaderHandle
{
  return shader_manager->create_shader(type, buffer);
}

void
jade::RenderContext::delete_shader(const ShaderHandle shader_handle) const
{
  shader_manager->delete_shader(shader_handle);
}

auto
jade::RenderContext::create_program(const std::initializer_list<ShaderHandle> shader_handles) const
  -> ShaderProgramHandle
{
  return shader_manager->create_program(shader_handles);
}

void
jade::RenderContext::delete_program(const ShaderProgramHandle program_handle) const
{
  shader_manager->delete_program(program_handle);
}
