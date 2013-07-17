#include "core/clock_settings.h"
#include "gui/digital_clock.h"
#include "spectrum_clock.h"

SpectrumClock::SpectrumClock() {
  info_[PI_NAME] = "Spectrum Clock";
  info_[PI_VERSION] = "2.0";
  info_[PI_AUTHOR] = "Nick Korotysh";
  info_[PI_EMAIL] = "nick.korotysh@gmail.com";
  info_[PI_COMMENT] = "clock changes color";
}

void SpectrumClock::Init(const TPluginData& data) {
  data_ = data;
  cur_color_ = Qt::red;
}

void SpectrumClock::Config() {
}

void SpectrumClock::Start() {
  old_color_ = data_.settings->GetOption(OPT_COLOR).value<QColor>();
  connect(data_.clock, SIGNAL(ImageNeeded(QString)), this, SLOT(TimeoutHandler(QString)));
}

void SpectrumClock::Stop() {
  disconnect(data_.clock, SIGNAL(ImageNeeded(QString)), this, SLOT(TimeoutHandler(QString)));
  data_.settings->SetOption(OPT_COLOR, old_color_);
}

const TPluginInfo&SpectrumClock::GetInfo() const {
  return info_;
}

void SpectrumClock::TimeoutHandler(const QString&) {
  int r = cur_color_.red();
  int g = cur_color_.green();
  int b = cur_color_.blue();
  int i = 1;

  if (r == 255 && g  < 255 && b ==  0 ) { g += i; }
  if (r  >  0  && g == 255 && b ==  0 ) { r -= i; }
  if (r ==  0  && g == 255 && b  < 255) { b += i; }
  if (r ==  0  && g  >  0  && b == 255) { g -= i; }
  if (r  < 255 && g ==  0  && b == 255) { r += i; }
  if (r == 255 && g ==  0  && b  >  0 ) { b -= i; }

  cur_color_.setRgb(r, g, b);
  data_.settings->SetOption(OPT_COLOR, cur_color_);
}
