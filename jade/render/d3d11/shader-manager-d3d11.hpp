#pragma once

#ifdef JADE_D3D11_SUPPORT

#include <SDL_system.h>
#include <d3d11.h>

#include <jade/render/shader-manager.hpp>

namespace jade {

  class ShaderManagerD3D11 final : public ShaderManager
  {
  public:
    explicit ShaderManagerD3D11(ID3D11Device* device, ID3D11DeviceContext* context)
      : device(device)
      , context(context)
    {
    }

    ~ShaderManagerD3D11() override = default;

    auto create_shader(ShaderType type, const char* shader_path) -> ShaderHandle override;
    void delete_shader(ShaderHandle shader_handle) override;
    auto create_program(std::initializer_list<ShaderHandle> shader_handles) -> ShaderProgramHandle override;
    void delete_program(ShaderProgramHandle program_handle) override;
    void bind_program(ShaderProgramHandle program_handle) override;

  private:
    ID3D11Device* device;
    ID3D11DeviceContext* context;
  };

} // namespace jade

#endif
