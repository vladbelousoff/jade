#pragma once

#include <jade/render/vertex-buffer.hpp>
#include <GL/gl3w.h>

namespace jade {

  class VertexBufferOpenGL final : public VertexBuffer
  {
  public:
    VertexBufferOpenGL(unsigned int stride, unsigned int offset);
    ~VertexBufferOpenGL() override;

    void set_data(const void* data, std::size_t size) override;
    void bind() override;

  private:
    GLuint vbo = 0;
    GLuint vao = 0;
    unsigned int stride;
    unsigned int offset;
  };

} 