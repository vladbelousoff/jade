#include "index-buffer-open-gl.hpp"
#include <jade/utils/assert.hpp>

namespace jade {

  IndexBufferOpenGL::IndexBufferOpenGL()
  {
    glGenBuffers(1, &ibo);
  }

  IndexBufferOpenGL::~IndexBufferOpenGL()
  {
    if (ibo != 0) {
      glDeleteBuffers(1, &ibo);
    }
  }

  void IndexBufferOpenGL::set_data(const void* data, const std::size_t size)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void IndexBufferOpenGL::bind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  }

  void IndexBufferOpenGL::draw(const unsigned int count) const
  {
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
  }
}  // namespace jade
