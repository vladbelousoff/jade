#pragma once

#include <GL/gl3w.h>
#include <jade/render/index-buffer.hpp>

namespace jade {

  class IndexBufferOpenGL final : public IndexBuffer
  {
   public:
    IndexBufferOpenGL();
    ~IndexBufferOpenGL() override;

    void set_data(const void* data, std::size_t size) override;
    void bind() const override;
    void draw(unsigned int count) const override;

   private:
    GLuint ibo = 0;
  };

}  // namespace jade
