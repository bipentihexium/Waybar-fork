#include "modules/diskrw.hpp"

#include <sstream>

waybar::modules::DiskRW::DiskRW(const std::string& id, const Json::Value& config)
    : AGraphLabel(config, "diskrw", id, "{}%", 30), lr_(0), lw_(0) {
  thread_ = [this] {
    dp.emit();
    thread_.sleep_for(interval_);
  };
  if (config["unit"].isString()) {
    unit_ = config["unit"].asString();
  } else {
	unit_ = "sda";
  }
}

auto waybar::modules::DiskRW::update() -> void {
  uint64_t r = 0;
  uint64_t w = 0;
  {
    const std::string data_dir = "/proc/diskstats";
    std::ifstream info(data_dir);
    if (!info.is_open()) {
      throw std::runtime_error("Can't open " + data_dir);
    }
    std::string ln;
    while (std::getline(info, ln)) {
      std::istringstream lns(ln);
      std::string x;
      lns >> x >> x >> x;
      if (x == unit_) {
        lns >> x >> x >> r >> x >> x >> x >> w;
        goto found;
      }
    }
    event_box_.hide();
	return;
  found:;
  }
  double rd = (r - lr_) / 8192.0 / interval_.count();
  double wd = (w - lw_) / 8192.0 / interval_.count();
  lr_ = r;
  lw_ = w;
  updateGraph({{ log2(rd + 1) / 4, 0.6, 1.0, 0.2 }}, 0);
  updateGraph({{ log2(wd + 1) / 4, 1.0, 0.8, 0.2 }}, 1);

  auto format = format_;
  if (format.empty()) {
    event_box_.hide();
  } else {
    event_box_.show();
    label_.set_markup(fmt::format(fmt::runtime(format),
	  fmt::arg("read", rd), fmt::arg("write", wd), fmt::arg("unit", unit_)));
  }

  if (tooltipEnabled()) {
    std::string tooltip_format = "{unit}: {read:.2f}MiB/s R {write:.2f}MiB/s W";
    if (config_["tooltip-format"].isString()) {
      tooltip_format = config_["tooltip-format"].asString();
    }
    label_.set_tooltip_text(fmt::format(fmt::runtime(tooltip_format),
	  fmt::arg("read", rd), fmt::arg("write", wd), fmt::arg("unit", unit_)));
  }
  ALabel::update();
}
