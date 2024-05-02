#pragma once

#include <gtkmm/box.h>
#include <gtkmm/image.h>

#include <array>
#include <initializer_list>

#include "ALabel.hpp"

namespace waybar {

template<size_t N = 1>
class AGraphLabel : public ALabel {
 public:
  inline AGraphLabel(const Json::Value &config, const std::string &s1, const std::string &s2, const std::string &format,
         uint16_t interval = 0, bool ellipsize = false, bool enable_click = false, bool enable_scroll = false) :
      waybar::ALabel(config,s1,s2,format,interval,ellipsize,enable_click, enable_scroll),
      width_(config.get("width", Json::UInt64(20)).asUInt64()),
      height_(config.get("height", Json::UInt64(16)).asUInt64()) {
	if (width_ > 0 && height_ > 0) {
      event_box_.remove();
      box_.set_orientation(Gtk::Orientation::ORIENTATION_HORIZONTAL);
      box_.set_spacing(0);
      box_.add(label_);
	  for (size_t i = 0; i < N; i++) {
        surfs_[i] = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width_, height_);
        box_.add(images_[i]);
        surfs_[i+N] = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width_, height_);
	  }
      event_box_.add(box_);
	}
  }
  inline virtual ~AGraphLabel() {
	for (size_t i = 0; i < 2*N; i++) {
      cairo_surface_destroy(surfs_[i]);
	}
  }
  auto updateGraph(const std::initializer_list<std::array<double, 4>> &data, size_t i=0) -> void {
    if (height_ > 0 && width_ > 0) {
      size_t j = i*2;
      cairo_t *cairo = cairo_create(surfs_[j]);
      cairo_set_source_rgb(cairo, 0.1, 0.1, 0.1);
      cairo_paint(cairo);
      cairo_set_source_surface(cairo, surfs_[j+1], -1, 0);
      cairo_rectangle(cairo, 0, 0, width_-1, height_);
      cairo_fill(cairo);
      for (const auto &v : data) {
        cairo_set_source_rgb(cairo, v[1], v[2], v[3]);
        cairo_rectangle(cairo, width_-1, height_ * (1-v[0]), 1, height_ * v[0]);
        cairo_fill(cairo);
      }
      cairo_destroy(cairo);
      std::swap(surfs_[j], surfs_[j+1]);
      gtk_image_set_from_surface(images_[i].gobj(), surfs_[j+1]);
    }
  }

 private:
  unsigned int width_, height_;
  std::array<cairo_surface_t *, 2*N> surfs_;
  std::array<Gtk::Image, N> images_;
  Gtk::Box box_;
};

}  // namespace waybar

