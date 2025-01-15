#include "uniform-buffer.hpp"

#include <ranges>

void jade::UniformBuffer::set_data(const std::string_view name, const void* data, const std::size_t size)
{
  const auto name_id = StringId(name);

  // Calculate offset or assign a new one
  std::size_t current_offset = 0;
  if (!offsets.contains(name_id)) {
    for (const auto offset : offsets | std::views::values) {
      current_offset = std::max(current_offset, offset);
    }
    offsets[name_id] = current_offset + size;
  } else {
    current_offset = offsets[name_id];
  }

  memcpy(buffer.data() + current_offset, data, size);
}
