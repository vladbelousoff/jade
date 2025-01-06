#include "uniform-buffer-d3d11.hpp"

#ifdef JADE_D3D11_SUPPORT

#include "render-context-d3d11.hpp"

#include <jade/utils/assert.hpp>

jade::UniformBufferD3D11::UniformBufferD3D11(const RenderContextD3D11* render_context, const std::size_t buffer_size)
{
  buffer.resize(buffer_size);

  D3D11_BUFFER_DESC desc = {};
  desc.Usage = D3D11_USAGE_DYNAMIC;
  desc.ByteWidth = static_cast<UINT>(buffer_size);
  desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  auto* device = render_context->get_device();
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
jade::UniformBufferD3D11::bind(const RenderContext* render_context, const ShaderType shader_type) const
{
  D3D11_MAPPED_SUBRESOURCE mapped_resource;

  auto* render_context_d3d11 = reinterpret_cast<const RenderContextD3D11*>(render_context);
  auto* device_context = render_context_d3d11->get_device_context();

  const HRESULT hr = device_context->Map(constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
  JADE_ASSERT(!FAILED(hr));

  memcpy(mapped_resource.pData, buffer.data(), buffer.size());
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
