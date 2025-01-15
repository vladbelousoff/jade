#include <jade/args/args-processor.hpp>
#include <jade/engine/application-context.hpp>
#include <jade/render/index-buffer.hpp>
#include <jade/render/uniform-buffer.hpp>
#include <jade/render/vertex-buffer.hpp>

#include <spdlog/spdlog.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
  glm::vec3 position;
};

// Define the triangle vertices
std::vector<Vertex> vertices = {
  { { 0.0f, 0.5f, 0.0f } },    // Top
  { { 0.5f, -0.5f, 0.0f } },   // Right
  { { -0.5f, -0.5f, 0.0f } },  // Left
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
    vsh_handle = context->create_shader(jade::ShaderType::VERT, "examples/001-triangle_shader.vert.hlsl");
    fsh_handle = context->create_shader(jade::ShaderType::FRAG, "examples/001-triangle_shader.frag.hlsl");
    program_handle = context->create_program({ vsh_handle, fsh_handle });

    matrix_buffer = context->create_uniform_buffer<MatrixBuffer>();
    const auto model_view_projection = glm::ortho<float>(-0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 1.0f);
    matrix_buffer->set_data("model_view_projection", &model_view_projection, sizeof(model_view_projection));

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
    context->clear(0.2f, 0.3f, 0.3f, 1.0f);  // Clear to dark gray
    context->bind_program(program_handle);
    matrix_buffer->bind(context, jade::ShaderType::VERT);
    vertex_buffer->bind();
    index_buffer->bind();
    index_buffer->draw(std::size(indices));
  }
};

int main(const int argc, char* argv[])
{
  auto& arg_processor = jade::ArgsProcessor::get_instance();
  arg_processor.process_args(argc, argv);

  jade::ApplicationContextSettings settings;
  settings.width = 640;
  settings.height = 640;
  if (arg_processor.is_set("-opengl")) {
    settings.render_interface = jade::RenderInterface::OPEN_GL;
  }

  const auto app = std::make_unique<TriangleApplicationContext>();
  return run_app(app.get(), settings);
}
