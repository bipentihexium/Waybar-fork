#pragma once

#include <fmt/format.h>
#include <sys/statvfs.h>

#include <fstream>

#include "AGraphLabel.hpp"
#include "util/format.hpp"
#include "util/sleeper_thread.hpp"

namespace waybar::modules {

class DiskRW : public AGraphLabel<2> {
 public:
  DiskRW(const std::string&, const Json::Value&);
  virtual ~DiskRW() = default;
  auto update() -> void override;

 private:
  util::SleeperThread thread_;
  std::string unit_;
  uint64_t lr_, lw_;
};

}  // namespace waybar::modules
