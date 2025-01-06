#pragma once

#include "shader-manager.hpp"

#include <SDL_video.h>
#include <functional>
#include <memory>
#include <utility>

namespace jade {

  class UniformBuffer;
  class VertexBuffer;
  class IndexBuffer;

  enum class RenderInterface
  {
    OpenGL,
    Direct3D11,
  };

  class RenderContext
  {
  public:
    explicit RenderContext(SDL_Window* window)
      : window(window)
    {
    }

    virtual ~RenderContext() = default;

    virtual void draw_scene(const std::function<void()>& callback) = 0;
    virtual auto get_drawable_size() -> std::pair<int, int> = 0;
    virtual void viewport(int x, int y, int w, int h) = 0;
    virtual void clear(float r, float g, float b, float a) = 0;
    virtual auto create_uniform_buffer(std::size_t buffer_size) -> std::shared_ptr<UniformBuffer> = 0;
    virtual auto create_vertex_buffer(unsigned int stride, unsigned int offset) -> std::shared_ptr<VertexBuffer> = 0;

    virtual auto create_index_buffer() -> std::shared_ptr<IndexBuffer>
    {
      return nullptr;
    }

    template<typename T>
    auto create_uniform_buffer() -> std::shared_ptr<UniformBuffer>
    {
      return create_uniform_buffer(sizeof(T));
    }

    auto create_shader(ShaderType type, const char* buffer) const -> ShaderHandle;
    void delete_shader(ShaderHandle shader_handle) const;

    auto create_program(std::initializer_list<ShaderHandle> shader_handles) const -> ShaderProgramHandle;
    void delete_program(ShaderProgramHandle program_handle) const;

    void bind_program(ShaderProgramHandle program_handle) const;

  protected:
    SDL_Window* window = nullptr;
    std::unique_ptr<ShaderManager> shader_manager;
  };

} // namespace jade
