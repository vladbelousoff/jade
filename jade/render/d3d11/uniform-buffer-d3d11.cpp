#include "uniform-buffer-d3d11.hpp"

#ifdef JADE_D3D11_SUPPORT

#include <jade/utils/assert.hpp>

jade::UniformBufferD3D11::UniformBufferD3D11(
  ID3D11Device* device, ID3D11DeviceContext* device_context, const std::size_t buffer_size)
  : device(device)
  , device_context(device_context)
  , constant_buffer(nullptr)
{
  this->buffer_size = buffer_size;
  buffer.resize(buffer_size);

  D3D11_BUFFER_DESC desc = {};
  desc.Usage = D3D11_USAGE_DYNAMIC;
  desc.ByteWidth = static_cast<UINT>(buffer_size);
  desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  const HRESULT hr = device->CreateBuffer(&desc, nullptr, &constant_buffer);
  JADE_ASSERT(!FAILED(hr));
}

jade::UniformBufferD3D11::~UniformBufferD3D11()
{
  if (constant_buffer != nullptr) {
    constant_buffer->Release();
  }
}

void
jade::UniformBufferD3D11::bind(const ShaderType shader_type)
{
  D3D11_MAPPED_SUBRESOURCE mapped_resource;

  const HRESULT hr = device_context->Map(constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
  JADE_ASSERT(!FAILED(hr));

  memcpy(mapped_resource.pData, buffer.data(), buffer_size);
  device_context->Unmap(constant_buffer, 0);

  switch (shader_type) {
    case ShaderType::Vertex:
      device_context->VSSetConstantBuffers(0, 1, &constant_buffer);
      break;
    case ShaderType::Fragment:
      device_context->PSSetConstantBuffers(0, 1, &constant_buffer);
      break;
    default:
      JADE_ASSERT(false);
  }
}

#endif
