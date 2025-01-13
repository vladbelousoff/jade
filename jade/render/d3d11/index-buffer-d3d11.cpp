#include "index-buffer-d3d11.hpp"

#ifdef JADE_D3D11_SUPPORT

#include <jade/utils/assert.hpp>

jade::IndexBufferD3D11::IndexBufferD3D11(ID3D11Device* device, ID3D11DeviceContext* device_context)
  : device(device)
  , device_context(device_context)
  , ibo(nullptr)
{
}

jade::IndexBufferD3D11::~IndexBufferD3D11()
{
  if (ibo) {
    ibo->Release();
  }
}

void
jade::IndexBufferD3D11::set_data(const void* data, const std::size_t size)
{
  if (ibo) {
    ibo->Release();
  }

  D3D11_BUFFER_DESC desc = {};
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.ByteWidth = static_cast<UINT>(size);
  desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

  D3D11_SUBRESOURCE_DATA init_data = {};
  init_data.pSysMem = data;

  const HRESULT hr = device->CreateBuffer(&desc, &init_data, &ibo);
  JADE_ASSERT(SUCCEEDED(hr));
}

void
jade::IndexBufferD3D11::bind() const
{
  device_context->IASetIndexBuffer(ibo, DXGI_FORMAT_R32_UINT, 0);
}

void
jade::IndexBufferD3D11::draw(const unsigned int count) const
{
  device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  device_context->DrawIndexed(count, 0, 0);
}

#endif
