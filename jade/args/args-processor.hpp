#pragma once

#include <jade/strings/string-manager.hpp>

#include <unordered_map>
#include <vector>

namespace jade {

  class ArgsProcessor
  {
   public:
    static ArgsProcessor& get_instance()
    {
      static ArgsProcessor instance;
      return instance;
    }

    void process_args(int argc, char* argv[]);
    auto get_value(std::string_view arg) const -> std::string_view;
    auto is_set(std::string_view arg) const -> bool;

   private:
    ArgsProcessor() = default;

    std::vector<std::string_view> args{};
    std::unordered_map<std::string_view, int> args_map{};
  };

}  // namespace jade
