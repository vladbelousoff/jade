#pragma once

#include <jade/render/render-context.hpp>
#include <jade/strings/string-manager.hpp>

namespace jade {

  struct ApplicationContextSettings
  {
    RenderInterface render_interface =
#ifdef JADE_D3D11_SUPPORT
      RenderInterface::Direct3D11;
#else
      RenderInterface::OpenGL;
#endif
    std::string_view title = "Jade";
    int width = 1280;
    int height = 720;
  };

  class IApplicationContext
  {
    friend int run_app(IApplicationContext*, const ApplicationContextSettings&);

  public:
    virtual ~IApplicationContext() = default;

  private:
    virtual void on_init(RenderContext* context) = 0;
    virtual void on_term(RenderContext* context) = 0;
    virtual void on_update(float dt) = 0;
    virtual void on_render(RenderContext* context) = 0;
  };

  int run_app(IApplicationContext* application_context, const ApplicationContextSettings& settings = {});

} // namespace jade
