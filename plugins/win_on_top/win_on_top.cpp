#include "win_on_top.h"

namespace win_on_top {

WinOnTop::WinOnTop() {
  InitTranslator(QLatin1String(":/win_on_top/win_on_top_"));
  info_.display_name = tr("\"Always on top\"");
  info_.description = tr("Add \"Always on top\" menu item to system menu for any window.");
}

void WinOnTop::Start() {
}

void WinOnTop::Stop() {
}

} // namespace win_on_top
