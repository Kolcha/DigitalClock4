#include "spectrum_clock.h"

void SpectrumClock::Init(const QMap<Options, QVariant>& current_settings) {
  old_color_ = current_settings[OPT_COLOR].value<QColor>();
  cur_color_ = Qt::red;
}

void SpectrumClock::Start() {
  emit OptionChanged(OPT_COLOR, cur_color_);
}

void SpectrumClock::Stop() {
  emit OptionChanged(OPT_COLOR, old_color_);
}

void SpectrumClock::TimeUpdateListener() {
  int r = cur_color_.red();
  int g = cur_color_.green();
  int b = cur_color_.blue();
  int i = 5;

  if (r == 255 && g  < 255 && b ==  0 ) { g += i; }
  if (r  >  0  && g == 255 && b ==  0 ) { r -= i; }
  if (r ==  0  && g == 255 && b  < 255) { b += i; }
  if (r ==  0  && g  >  0  && b == 255) { g -= i; }
  if (r  < 255 && g ==  0  && b == 255) { r += i; }
  if (r == 255 && g ==  0  && b  >  0 ) { b -= i; }

  cur_color_.setRgb(r, g, b);
  emit OptionChanged(OPT_COLOR, cur_color_);
}
