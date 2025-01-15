#pragma once

#include <shared_mutex>
#include <string>
#include <string_view>
#include <unordered_map>

namespace jade {

  struct StringManager;

  class StringId
  {
    friend struct std::hash<StringId>;
    friend struct StringManager;

   public:
    explicit StringId();
    explicit StringId(std::string_view string);

    auto data() const -> std::string_view;

    auto operator==(const StringId& rhs) const -> bool
    {
      return id == rhs.id;
    }

    auto operator!=(const StringId& rhs) const -> bool
    {
      return id != rhs.id;
    }

   private:
    std::size_t id;
  };

  struct StringManager
  {
    friend class StringId;

   private:
    static auto get_string_by_id(StringId id) -> std::string_view;
    static auto get_id_by_string(std::string_view string) -> StringId;

    static std::shared_mutex mutex;
    static std::size_t string_counter;
    static std::unordered_map<StringId, std::string> id_to_string;
    static std::unordered_map<std::string_view, StringId> string_to_id;
  };

}  // namespace jade

template <>
struct std::hash<jade::StringId>
{
  std::size_t operator()(const jade::StringId& string_id) const noexcept
  {
    return std::hash<std::size_t>{}(string_id.id);
  }
};
