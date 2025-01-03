#include <jade/engine/application-context.hpp>

#include <spdlog/spdlog.h>

static auto vertex_shader_source = R"(
cbuffer MatrixBuffer : register(b0) {
    matrix worldViewProjection;
};

struct VSInput {
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.position = mul(worldViewProjection, float4(input.position, 1.0));
    output.color = input.color;
    return output;
}
)";

static auto pixel_shader_source = R"(
struct PSInput {
    float4 color : COLOR;
};

float4 main(PSInput input) : SV_TARGET {
    return input.color;
}
)";

class TriangleApplicationContext final : public jade::IApplicationContext
{
public:
  ~TriangleApplicationContext() override = default;

private:
  jade::ShaderHandle vsh_handle;
  jade::ShaderHandle fsh_handle;

  void on_init(jade::RenderContext* context) override
  {
    vsh_handle = context->create_shader(jade::ShaderType::Vertex, vertex_shader_source);
    fsh_handle = context->create_shader(jade::ShaderType::Fragment, pixel_shader_source);
  }

  void on_term(jade::RenderContext* context) override
  {
    context->delete_shader(vsh_handle);
    context->delete_shader(fsh_handle);
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
  const auto app = std::make_unique<TriangleApplicationContext>();
  return run_app(app.get());
}
