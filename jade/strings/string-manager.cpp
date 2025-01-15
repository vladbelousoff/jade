#include "string-manager.hpp"

#include <jade/utils/assert.hpp>

std::shared_mutex jade::StringManager::mutex;
std::unordered_map<jade::StringId, std::string> jade::StringManager::id_to_string;
std::unordered_map<std::string_view, jade::StringId> jade::StringManager::string_to_id;
std::size_t jade::StringManager::string_counter = 0;

auto jade::StringManager::get_string_by_id(const StringId id) -> std::string_view
{
  std::shared_lock lock(mutex);
  const auto it = id_to_string.find(id);
  if (it != id_to_string.end()) {
    return it->second;
  }

  return "";
}

auto jade::StringManager::get_id_by_string(std::string_view string) -> StringId
{
  {
    std::shared_lock lock(mutex);
    const auto it = string_to_id.find(string);
    if (it != string_to_id.end()) {
      return it->second;
    }
  }

  auto id = StringId{};

  {
    std::unique_lock lock(mutex);
    JADE_ASSERT(string_counter < std::numeric_limits<std::size_t>::max());
    id.id = string_counter++;
    string_to_id.insert(std::make_pair(string, id));
    id_to_string.insert(std::make_pair(id, string));
  }

  return id;
}

jade::StringId::StringId()
    : id(0)
{
}

jade::StringId::StringId(const std::string_view string)
    : id(StringManager::get_id_by_string(string).id)
{
}

auto jade::StringId::data() const -> std::string_view
{
  return StringManager::get_string_by_id(*this);
}
