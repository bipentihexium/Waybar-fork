#pragma once

#include <fmt/format.h>

#include <fstream>

#include "AGraphLabel.hpp"
#include "util/sleeper_thread.hpp"

namespace waybar::modules {

class Temperature : public AGraphLabel<> {
 public:
  Temperature(const std::string&, const Json::Value&);
  virtual ~Temperature() = default;
  auto update() -> void override;

 private:
  float getTemperature();
  bool isCritical(uint16_t);

  std::string file_path_;
  util::SleeperThread thread_;
};

}  // namespace waybar::modules
