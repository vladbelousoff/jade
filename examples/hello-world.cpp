#include <jade/engine/application-context.hpp>

#include <spdlog/spdlog.h>

class HelloWorldApplicationContext final : public jade::IApplicationContext
{
public:
  ~HelloWorldApplicationContext() override = default;

private:
  void on_init() override
  {
    spdlog::info("Hello World!");
  }

  void on_term() override
  {
    spdlog::info("Hello World! Terminated!");
  }

  void on_update(float dt) override
  {
  }

  void on_render(jade::RenderContext* context) override
  {
  }
};

int
main()
{
  const auto app = std::make_unique<HelloWorldApplicationContext>();
  return run_app(app.get());
}
