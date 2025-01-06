#include "shader-manager-d3d11.hpp"

#include "jade/utils/assert.hpp"

#ifdef JADE_D3D11_SUPPORT

#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>

#include <spdlog/spdlog.h>

class ShaderD3D11 final : public jade::Shader
{
  friend class jade::ShaderManagerD3D11;

public:
  explicit ShaderD3D11(const jade::ShaderType type, ID3D10Blob* blob)
    : Shader(type)
    , blob(blob)
  {
  }

  ~ShaderD3D11() override = default;

private:
  ID3DBlob* blob = nullptr;

  union
  {
    ID3D11PixelShader* pixel_shader = nullptr;
    ID3D11VertexShader* vertex_shader;
  };
};

class ShaderProgramD3D11 final : public jade::ShaderProgram
{
  friend class jade::ShaderManagerD3D11;

public:
  ~ShaderProgramD3D11() override = default;

  ShaderProgramD3D11(const std::initializer_list<jade::ShaderHandle> handles)
  {
    shader_handles.insert(shader_handles.end(), handles.begin(), handles.end());
  }

private:
  std::vector<jade::ShaderHandle> shader_handles;
};

auto
jade::ShaderManagerD3D11::create_shader(const ShaderType type, const char* buffer) -> ShaderHandle
{
  const char* shader_target = type == ShaderType::Fragment ? "ps_5_0" : "vs_5_0";
  auto* shader = new ShaderD3D11(type, nullptr);

  {
    ID3DBlob* error_blob = nullptr;
    const HRESULT hr = D3DCompile(buffer, strlen(buffer),
      nullptr,       // source name
      nullptr,       // defines
      nullptr,       // include handler
      "main",        // entry point
      shader_target, // shader target
      D3DCOMPILE_ENABLE_STRICTNESS,
      0, // effect flags
      &shader->blob, &error_blob);

    if (FAILED(hr) && error_blob) {
      spdlog::error("{}", static_cast<char*>(error_blob->GetBufferPointer()));
      error_blob->Release();
      JADE_ASSERT(false);
      return {};
    }
  }

  D3D11_INPUT_ELEMENT_DESC layout[] = { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,
    0 } };

  switch (type) {
    case ShaderType::Vertex: {
      HRESULT hr = device->CreateVertexShader( //
        shader->blob->GetBufferPointer(),      //
        shader->blob->GetBufferSize(),         //
        nullptr,                               //
        &shader->vertex_shader);
      JADE_ASSERT(!FAILED(hr));

      ID3D11InputLayout* input_layout = nullptr;
      hr = device->CreateInputLayout(layout, // Pointer to the layout array
        _countof(layout),                    // Number of elements in the array
        shader->blob->GetBufferPointer(), shader->blob->GetBufferSize(), &input_layout);
      JADE_ASSERT(!FAILED(hr));

      context->IASetInputLayout(input_layout);
    } break;
    case ShaderType::Fragment: {
      const HRESULT hr = device->CreatePixelShader( //
        shader->blob->GetBufferPointer(),           //
        shader->blob->GetBufferSize(),              //
        nullptr,                                    //
        &shader->pixel_shader);
      JADE_ASSERT(!FAILED(hr));
    } break;
    default:
      JADE_ASSERT(false);
  }

  return create_shader_handle(shader);
}

void
jade::ShaderManagerD3D11::delete_shader(const ShaderHandle shader_handle)
{
  delete_shader_handle(shader_handle);
}

auto
jade::ShaderManagerD3D11::create_program(const std::initializer_list<ShaderHandle> shader_handles)
  -> ShaderProgramHandle
{
  return create_program_handle(new ShaderProgramD3D11(shader_handles));
}

void
jade::ShaderManagerD3D11::delete_program(const ShaderProgramHandle program_handle)
{
  delete_program_handle(program_handle);
}

void
jade::ShaderManagerD3D11::bind_program(const ShaderProgramHandle program_handle)
{
  const auto it_program = programs.find(get_handle_id(program_handle));
  if (it_program == programs.end()) {
    return;
  }

  const auto* shader_program = dynamic_cast<ShaderProgramD3D11*>(it_program->second);
  for (const auto& shader_handle : shader_program->shader_handles) {
    const auto it_shader = shaders.find(get_handle_id(shader_handle));
    if (it_shader == shaders.end()) {
      continue;
    }

    const auto* shader = dynamic_cast<ShaderD3D11*>(it_shader->second);
    switch (shader->type) {
      case ShaderType::Vertex: {
        context->VSSetShader(shader->vertex_shader, nullptr, 0);
        break;
      }
      case ShaderType::Fragment: {
        context->PSSetShader(shader->pixel_shader, nullptr, 0);
        break;
      }
      default:
        JADE_ASSERT(false);
    }
  }
}

#endif
