#include <QFontMetrics>
#include "clock_text_skin.h"

namespace digital_clock {
namespace core {

ClockTextSkin::ClockTextSkin(const QFont& font)
  : TextSkin(font) {
  char_map_[' '] = ':';
  // set skin info
  info_[SI_NAME] = "Text Skin";
  info_[SI_VERSION] = "1.0";
  info_[SI_AUTHOR] = "Nick Korotysh";
  info_[SI_EMAIL] = "nick.korotysh@gmail.com";
  info_[SI_COMMENT] = "Text Skin. Allows use any font as skin.";
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

} // namespace core
} // namespace digital_clock
