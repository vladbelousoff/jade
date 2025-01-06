#include <jade/engine/application-context.hpp>
#include <jade/render/index-buffer.hpp>
#include <jade/render/uniform-buffer.hpp>
#include <jade/render/vertex-buffer.hpp>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

static auto vertex_shader_source = R"(
cbuffer MatrixBuffer : register(b0) {
    matrix modelViewProjection;
};

struct VS_INPUT {
    float3 position : POSITION;
};

struct PS_INPUT {
    float4 position : SV_POSITION;
};

PS_INPUT main(VS_INPUT input) {
    PS_INPUT output;
    output.position = mul(modelViewProjection, float4(input.position, 1.0));
    return output;
}
)";

static auto pixel_shader_source = R"(
struct PS_INPUT {
    float4 position : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET {
    return float4(1.0, 0.0, 0.0, 1.0); // Solid red color
}
)";

struct Vertex
{
  glm::vec3 position;
};

// Define the triangle vertices
std::vector<Vertex> vertices = {
  { { 0.0f, 0.5f, 0.0f } },   // Top
  { { 0.5f, -0.5f, 0.0f } },  // Right
  { { -0.5f, -0.5f, 0.0f } }, // Left
};

static unsigned int indices[] = { 0, 1, 2 };

struct MatrixBuffer
{
  glm::mat4 model_view_projection;
};

class TriangleApplicationContext final : public jade::IApplicationContext
{
public:
  ~TriangleApplicationContext() override = default;

private:
  jade::ShaderHandle vsh_handle;
  jade::ShaderHandle fsh_handle;
  jade::ShaderProgramHandle program_handle;
  std::shared_ptr<jade::UniformBuffer> matrix_buffer;
  std::shared_ptr<jade::VertexBuffer> vertex_buffer;
  std::shared_ptr<jade::IndexBuffer> index_buffer;

  void on_init(jade::RenderContext* context) override
  {
    vsh_handle = context->create_shader(jade::ShaderType::Vertex, vertex_shader_source);
    fsh_handle = context->create_shader(jade::ShaderType::Fragment, pixel_shader_source);
    program_handle = context->create_program({ vsh_handle, fsh_handle });

    matrix_buffer = context->create_uniform_buffer<MatrixBuffer>();
    constexpr auto model_view_projection = glm::mat4(1.0f);
    matrix_buffer->set_data("modelViewProjection", &model_view_projection, sizeof(model_view_projection));

    vertex_buffer = context->create_vertex_buffer(sizeof(Vertex), 0);
    vertex_buffer->set_data(vertices.data(), vertices.size() * sizeof(Vertex));

    index_buffer = context->create_index_buffer();
    index_buffer->set_data(indices, sizeof(indices));
  }

  void on_term(jade::RenderContext* context) override
  {
    context->delete_program(program_handle);
    context->delete_shader(vsh_handle);
    context->delete_shader(fsh_handle);
  }

  void on_update(float dt) override
  {
  }

  void on_render(jade::RenderContext* context) override
  {
    context->clear(0.2f, 0.3f, 0.3f, 1.0f); // Clear to dark gray
    context->bind_program(program_handle);
    matrix_buffer->bind(jade::ShaderType::Vertex);
    vertex_buffer->bind();
    index_buffer->bind();
    index_buffer->draw(std::size(indices));
  }
};

int
main()
{
  jade::ApplicationContextSettings settings;
  settings.width = 640;
  settings.height = 640;

  const auto app = std::make_unique<TriangleApplicationContext>();
  return run_app(app.get(), settings);
}
