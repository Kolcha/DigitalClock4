#include "clock_text_skin.h"

ClockTextSkin::ClockTextSkin(const QFont& font)
  : TextSkin(font) {
  char_map_[' '] = ':';
}
