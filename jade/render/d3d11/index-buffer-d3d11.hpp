#pragma once

#ifdef JADE_D3D11_SUPPORT

#include <d3d11.h>
#include <string>

#include <jade/render/index-buffer.hpp>

namespace jade {

  class IndexBufferD3D11 final : public IndexBuffer
  {
   public:
    IndexBufferD3D11(ID3D11Device* device, ID3D11DeviceContext* device_context);
    ~IndexBufferD3D11() override;

    void set_data(const void* data, std::size_t size) override;
    void bind() const override;
    void draw(unsigned int count) const override;

   private:
    ID3D11Device* device;
    ID3D11DeviceContext* device_context;
    ID3D11Buffer* ibo;
  };

}  // namespace jade

#endif
