#pragma once

#include <jade/render/shader-manager.hpp>
#include <jade/strings/string-manager.hpp>

#include <string_view>
#include <unordered_map>

namespace jade {

  class UniformBuffer
  {
  public:
    virtual ~UniformBuffer() = default;

    // bind the buffer to the pipeline
    virtual void bind(ShaderType shader_type) = 0;

    // add data to the buffer
    void set_data(std::string_view name, const void* data, std::size_t size);

  protected:
    std::size_t buffer_size = 0;
    std::unordered_map<StringId, std::size_t> offsets;
    std::vector<std::byte> buffer;
  };

} // namespace jade
