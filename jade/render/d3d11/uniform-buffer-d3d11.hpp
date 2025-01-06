#pragma once

#ifdef JADE_D3D11_SUPPORT

#include <jade/render/uniform-buffer.hpp>

#include <d3d11.h>

namespace jade {

  class UniformBufferD3D11 final : public UniformBuffer
  {
  public:
    explicit UniformBufferD3D11(ID3D11Device* device, ID3D11DeviceContext* device_context, std::size_t buffer_size);
    ~UniformBufferD3D11() override;

    void bind(ShaderType shader_type) override;

  private:
    ID3D11Device* device;
    ID3D11DeviceContext* device_context;
    ID3D11Buffer* constant_buffer;
  };

} // namespace jade

#endif
