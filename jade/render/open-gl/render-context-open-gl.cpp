#include "render-context-open-gl.hpp"
#include "shader-manager-open-gl.hpp"
#include "uniform-buffer-open-gl.hpp"
#include "vertex-buffer-open-gl.hpp"
#include "index-buffer-open-gl.hpp"

#include <GL/gl3w.h>

namespace jade {

  RenderContextOpenGL::RenderContextOpenGL(SDL_Window* window)
    : RenderContext(window)
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
      return;
    }

    // Enable VSYNC
    SDL_GL_SetSwapInterval(1);

    if (gl3wInit()) {
      return;
    }

    shader_manager = std::make_unique<ShaderManagerOpenGL>();
  }

  RenderContextOpenGL::~RenderContextOpenGL()
  {
    SDL_GL_DeleteContext(gl_context);
  }

  auto RenderContextOpenGL::get_drawable_size() -> std::pair<int, int>
  {
    int w, h;
    SDL_GL_GetDrawableSize(window, &w, &h);
    return { w, h };
  }

  void RenderContextOpenGL::viewport(const int x, const int y, const int w, const int h)
  {
    glViewport(x, y, w, h);
  }

  void RenderContextOpenGL::clear(const float r, const float g, const float b, const float a)
  {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  auto RenderContextOpenGL::create_uniform_buffer(std::size_t buffer_size) -> std::shared_ptr<UniformBuffer>
  {
    return std::make_shared<UniformBufferOpenGL>(buffer_size);
  }

  auto RenderContextOpenGL::create_vertex_buffer(unsigned int stride, unsigned int offset)
    -> std::shared_ptr<VertexBuffer>
  {
    return std::make_shared<VertexBufferOpenGL>(stride, offset);
  }

  auto RenderContextOpenGL::create_index_buffer() -> std::shared_ptr<IndexBuffer>
  {
    return std::make_shared<IndexBufferOpenGL>();
  }

  void RenderContextOpenGL::draw_scene(const std::function<void()>& callback)
  {
    callback();
    SDL_GL_SwapWindow(window);
  }

} // namespace jade
