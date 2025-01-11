#include "uniform-buffer-open-gl.hpp"
#include <jade/utils/assert.hpp>

namespace jade {

  UniformBufferOpenGL::UniformBufferOpenGL(const std::size_t buffer_size)
  {
    buffer.resize(buffer_size);

    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  UniformBufferOpenGL::~UniformBufferOpenGL()
  {
    if (ubo != 0) {
      glDeleteBuffers(1, &ubo);
    }
  }

  void
  UniformBufferOpenGL::bind(const RenderContext* render_context, const ShaderType shader_type) const
  {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer.size(), buffer.data());
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
  }
} 