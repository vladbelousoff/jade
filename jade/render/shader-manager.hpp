#pragma once

#include <unordered_map>

namespace jade {

  enum class ShaderType
  {
    VERT,
    FRAG,
  };

  class ShaderManager;

  class ShaderHandle
  {
    friend class ShaderManager;

  public:
    using IdType = std::size_t;

  private:
    IdType id = 0;
  };

  class ShaderProgramHandle
  {
    friend class ShaderManager;

  public:
    using IdType = std::size_t;

  private:
    IdType id = 0;
  };

  class Shader
  {
    friend class ShaderManager;

  public:
    explicit Shader(const ShaderType type)
      : type(type)
    {
    }

    virtual ~Shader() = default;

  protected:
    ShaderType type;
  };

  class ShaderProgram
  {
    friend class ShaderManager;

  public:
    virtual ~ShaderProgram() = default;
  };

  class ShaderManager
  {
  public:
    virtual ~ShaderManager() = default;

    virtual auto create_shader(ShaderType type, const char* shader_path) -> ShaderHandle = 0;
    virtual void delete_shader(ShaderHandle shader_handle) = 0;

    virtual auto create_program(std::initializer_list<ShaderHandle> shader_handles) -> ShaderProgramHandle = 0;
    virtual void delete_program(ShaderProgramHandle program_handle) = 0;

    virtual void bind_program(ShaderProgramHandle program_handle) = 0;

    auto is_valid(ShaderHandle shader_handle) const -> bool;
    auto is_valid(ShaderProgramHandle program_handle) const -> bool;

  protected:
    template<typename T>
    static auto get_handle_id(T handle)
    {
      return handle.id;
    }

    auto create_shader_handle(Shader* shader) -> ShaderHandle;
    void delete_shader_handle(ShaderHandle shader_handle);

    auto create_program_handle(ShaderProgram* shader_program) -> ShaderProgramHandle;
    void delete_program_handle(ShaderProgramHandle program_handle);

    // No smart pointers on purpose, because the pointers
    // are always stored inside the system
    std::unordered_map<ShaderHandle::IdType, Shader*> shaders;
    std::unordered_map<ShaderProgramHandle::IdType, ShaderProgram*> programs;

  private:
    // Shader counter
    ShaderHandle::IdType shader_next_index = 1;

    // Program counter
    ShaderProgramHandle::IdType program_next_index = 1;
  };

} // namespace jade
