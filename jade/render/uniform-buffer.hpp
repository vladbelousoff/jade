#pragma once

#include <jade/render/shader-manager.hpp>
#include <jade/strings/string-manager.hpp>

#include <string_view>
#include <unordered_map>

namespace jade {

  class RenderContext;

  class UniformBuffer
  {
  public:
    virtual ~UniformBuffer() = default;

    // bind the buffer to the pipeline
    virtual void bind(const RenderContext* render_context, ShaderType shader_type) const = 0;

    // add data to the buffer
    void set_data(std::string_view name, const void* data, std::size_t size);

  protected:
    std::unordered_map<StringId, std::size_t> offsets;
    std::vector<std::byte> buffer;
  };

} // namespace jade
