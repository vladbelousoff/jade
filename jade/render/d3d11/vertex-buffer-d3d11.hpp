#pragma once

#include <d3d11.h>
#include <jade/render/vertex-buffer.hpp>

namespace jade {

  class VertexBufferD3D11 final : public VertexBuffer
  {
  public:
    VertexBufferD3D11(
      ID3D11Device* device, ID3D11DeviceContext* device_context, unsigned int stride, unsigned int offset);
    ~VertexBufferD3D11() override;

    void set_data(const void* data, std::size_t size) override;
    void bind() override;

  private:
    ID3D11Device* device;
    ID3D11DeviceContext* device_context;
    ID3D11Buffer* vbo;
    UINT offset;
    UINT stride;
  };

} // namespace jade

