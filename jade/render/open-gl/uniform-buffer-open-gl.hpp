#pragma once

#include <jade/render/uniform-buffer.hpp>
#include <GL/gl3w.h>

namespace jade {
  class UniformBufferOpenGL final : public UniformBuffer
  {
  public:
    explicit UniformBufferOpenGL(std::size_t buffer_size);
    ~UniformBufferOpenGL() override;

    void bind(const RenderContext* render_context, ShaderType shader_type) const override;

  private:
    GLuint ubo = 0;
  };
} 