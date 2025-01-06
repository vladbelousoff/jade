#pragma once

#include <jade/render/render-context.hpp>

namespace jade {

  class RenderContextOpenGL final : public RenderContext
  {
  public:
    explicit RenderContextOpenGL(SDL_Window* window);
    ~RenderContextOpenGL() override;

    void draw_scene(const std::function<void()>& callback) override;
    auto get_drawable_size() -> std::pair<int, int> override;
    void viewport(int x, int y, int w, int h) override;
    void clear(float r, float g, float b, float a) override;
    auto create_uniform_buffer(std::size_t buffer_size) -> std::shared_ptr<UniformBuffer> override;
    auto create_vertex_buffer(unsigned int stride, unsigned int offset) -> std::shared_ptr<VertexBuffer> override;

  private:
    SDL_GLContext gl_context = nullptr;
  };

} // namespace jade
