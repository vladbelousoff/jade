#include "vertex-buffer-d3d11.hpp"

#ifdef JADE_D3D11_SUPPORT

#include <jade/utils/assert.hpp>

jade::VertexBufferD3D11::VertexBufferD3D11(
  ID3D11Device* device, ID3D11DeviceContext* device_context, const unsigned int stride, const unsigned int offset)
    : device(device)
    , device_context(device_context)
    , vbo(nullptr)
    , offset(offset)
    , stride(stride)
{
}

jade::VertexBufferD3D11::~VertexBufferD3D11()
{
  if (vbo) {
    vbo->Release();
  }
}

void jade::VertexBufferD3D11::set_data(const void* data, const std::size_t size)
{
  if (vbo) {
    vbo->Release();
  }

  D3D11_BUFFER_DESC desc = {};
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.ByteWidth = static_cast<UINT>(size);
  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  D3D11_SUBRESOURCE_DATA init_data = {};
  init_data.pSysMem = data;

  const HRESULT hr = device->CreateBuffer(&desc, &init_data, &vbo);
  JADE_ASSERT(SUCCEEDED(hr));
}

void jade::VertexBufferD3D11::bind()
{
  device_context->IASetVertexBuffers(0, 1, &vbo, &stride, &offset);
}

#endif
