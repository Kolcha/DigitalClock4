#include "spectrum_clock.h"

void SpectrumClock::Init(const QMap<Options, QVariant>& current_settings) {
  old_color_ = current_settings[OPT_COLOR].value<QColor>();
  cur_color_ = Qt::red;
}

void SpectrumClock::GetInfo(TPluginInfo* info) {
  info->insert(PI_NAME, "Spectrum clock");
  info->insert(PI_TYPE, "settings");
  info->insert(PI_VERSION, "2.0");
  info->insert(PI_AUTHOR, "Nick Korotysh");
  info->insert(PI_EMAIL, "nick.korotysh@gmail.com");
  info->insert(PI_COMMENT, "Clock change color during time.");
  info->insert(PI_CONFIG, "false");
}

void SpectrumClock::Start() {
  emit OptionChanged(OPT_COLOR, cur_color_);
  emit started();
}

void SpectrumClock::Stop() {
  emit OptionChanged(OPT_COLOR, old_color_);
  emit stopped();
}

void SpectrumClock::TimeUpdateListener(const QString&) {
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
