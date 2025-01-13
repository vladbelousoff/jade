#pragma once

#include <jade/render/shader-manager.hpp>

namespace jade {

  class ShaderManagerOpenGL final : public ShaderManager
  {
  public:
    explicit ShaderManagerOpenGL() = default;
    ~ShaderManagerOpenGL() override = default;

    auto create_shader(ShaderType type, const char* shader_path) -> ShaderHandle override;
    void delete_shader(ShaderHandle shader_handle) override;
    auto create_program(std::initializer_list<ShaderHandle> shader_handles) -> ShaderProgramHandle override;
    void delete_program(ShaderProgramHandle program_handle) override;
    void bind_program(ShaderProgramHandle program_handle) override;
  };

} // namespace jade
