#pragma once

#include <string>

namespace jade {

  class IndexBuffer
  {
   public:
    virtual ~IndexBuffer() = default;
    virtual void set_data(const void* data, std::size_t size) = 0;
    virtual void bind() const = 0;
    virtual void draw(unsigned int count) const = 0;
  };

}  // namespace jade
