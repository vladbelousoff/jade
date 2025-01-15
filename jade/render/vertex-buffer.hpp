#pragma once

#include <string>

namespace jade {

  class VertexBuffer
  {
   public:
    virtual ~VertexBuffer() = default;
    virtual void set_data(const void* data, std::size_t size) = 0;
    virtual void bind() = 0;
  };

}  // namespace jade
