#include <QFontMetrics>
#include "clock_text_skin.h"

ClockTextSkin::ClockTextSkin(const QFont& font)
  : TextSkin(font) {
  char_map_[' '] = ':';
}

void ClockTextSkin::ProcSeparators() {
  if (seps_.isEmpty()) return;

  QFontMetrics fm(font_);
  int max_w = 0;
  QChar widest = seps_[0];
  for (int i = 0; i < seps_.length(); ++i) {
    int w = fm.width(QString(seps_[i]));
    if (w > max_w) {
      max_w = w;
      widest = seps_[i];
    }
  }

  char_map_[' '] = widest;
  for (int i = 0; i < seps_.length(); ++i) {
    if (seps_[i] != widest) char_map_[seps_[i]] = widest;
  }
}
