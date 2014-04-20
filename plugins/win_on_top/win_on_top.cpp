#include <QCoreApplication>
#include "win_on_top.h"

namespace win_on_top {

typedef void (*pSetItemName)(LPCWSTR);
typedef void (*pStartFunc)(void);
typedef void (*pStopFunc)(void);

WinOnTop::WinOnTop() {
  InitTranslator(QLatin1String(":/win_on_top/win_on_top_"));
  info_.display_name = tr("\"Always on top\" for any window");
  info_.description = tr("Adds \"Always on Top\" menu item to system menu for any window.");
}

void WinOnTop::Start() {
  std::wstring lib_path = QCoreApplication::applicationDirPath().toStdWString();
  lib_path += L"/plugins/special/win_on_top.dll";
  lib_ = LoadLibrary(lib_path.c_str());
  if (!lib_) return;

  WCHAR item_name[32];
  memset(item_name, 0, 64);
  tr("Always on Top").toWCharArray(item_name);
  pSetItemName set_name = (pSetItemName)GetProcAddress(lib_, "SetItemName");
  set_name(item_name);

  pStartFunc start = (pStartFunc)GetProcAddress(lib_, "Start");
  start();
}

void WinOnTop::Stop() {
  if (!lib_) return;
  pStopFunc stop = (pStopFunc)GetProcAddress(lib_, "Stop");
  stop();
  FreeLibrary(lib_);
}

} // namespace win_on_top
