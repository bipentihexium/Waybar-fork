#pragma once

#include <gtkmm/box.h>
#include <gtkmm/image.h>

#include <fmt/format.h>

#include <array>
#include <fstream>
#include <unordered_map>

#include "AGraphLabel.hpp"
#include "util/sleeper_thread.hpp"

namespace waybar::modules {

class Memory : public AGraphLabel<2> {
 public:
  Memory(const std::string&, const Json::Value&);
  virtual ~Memory() = default;
  auto update() -> void override;

 private:
  void parseMeminfo();

  std::unordered_map<std::string, unsigned long> meminfo_;

  util::SleeperThread thread_;
};

}  // namespace waybar::modules
