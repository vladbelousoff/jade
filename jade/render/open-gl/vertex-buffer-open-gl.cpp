#include "vertex-buffer-open-gl.hpp"
#include <jade/utils/assert.hpp>

namespace jade {

  VertexBufferOpenGL::VertexBufferOpenGL(const unsigned int stride, const unsigned int offset)
    : stride(stride)
    , offset(offset)
  {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
  }

  VertexBufferOpenGL::~VertexBufferOpenGL()
  {
    if (vbo != 0) {
      glDeleteBuffers(1, &vbo);
    }
    if (vao != 0) {
      glDeleteVertexArrays(1, &vao);
    }
  }

  void VertexBufferOpenGL::set_data(const void* data, const std::size_t size)
  {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(size), data, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<int>(stride), reinterpret_cast<void*>(offset));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void VertexBufferOpenGL::bind()
  {
    glBindVertexArray(vao);
  }
} // namespace jade
