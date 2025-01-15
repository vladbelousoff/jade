#pragma once

#ifdef JADE_D3D11_SUPPORT

#include <jade/render/uniform-buffer.hpp>

#include <d3d11.h>

namespace jade {
  class RenderContextD3D11;

  class UniformBufferD3D11 final : public UniformBuffer
  {
   public:
    explicit UniformBufferD3D11(const RenderContextD3D11* render_context, std::size_t buffer_size);
    ~UniformBufferD3D11() override;

    void bind(const RenderContext* render_context, ShaderType shader_type) const override;

   private:
    ID3D11Buffer* constant_buffer = nullptr;
  };

}  // namespace jade

#endif
