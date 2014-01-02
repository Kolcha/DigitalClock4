#include <Windows.h>
#include "win_on_top.h"

#pragma data_seg(".SHARED")
HWND last_top_wnd = NULL;
WCHAR item_name[32] = L"new item";
#pragma data_seg()
#pragma comment(linker,"/section:.SHARED,rws")

HHOOK hook = NULL;   // installed hook handle

#define SC_ONTOP     0x01  // command code for addd menu item

namespace win_on_top {

// hook processor function
LRESULT CALLBACK GetMsgProc(int ncode, WPARAM wparam, LPARAM lparam) {
  if (ncode < 0) return CallNextHookEx(hook, ncode, wparam, lparam);

  LPMSG msg = (LPMSG)lparam;

  switch (msg->message) {
    case WM_NCRBUTTONDOWN:
    {
      // add 'Alway on Top' menu item if it doesn't exists for current window
      HMENU menu = GetSystemMenu(msg->hwnd, false);
      if (GetMenuItemID(menu, GetMenuItemCount(menu) - 1) != SC_ONTOP) {
        AppendMenu(menu, MF_SEPARATOR, 0, 0);
        AppendMenu(menu, MF_UNCHECKED, SC_ONTOP, item_name);
      }
      break;
    }

    case WM_SYSCOMMAND:
      // 'Always on Top' selected
      if (msg->wParam == SC_ONTOP) {
        // message was recieved from new window ...
        if (msg->hwnd != last_top_wnd) {
          if (last_top_wnd) {
            // disable 'Always on Top' for previous window
            HMENU menu = GetSystemMenu(last_top_wnd, false);
            CheckMenuItem(menu, SC_ONTOP, MF_UNCHECKED);
            SetWindowPos(last_top_wnd, HWND_NOTOPMOST, 0, 0, 10, 10,
                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);
          }

          // enable 'Always on Top' for current window
          HMENU menu = GetSystemMenu(msg->hwnd, false);
          CheckMenuItem(menu, SC_ONTOP, MF_CHECKED);
          SetWindowPos(msg->hwnd, HWND_TOPMOST, 0, 0, 10, 10,
                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);
          // save current windows as last on top window
          last_top_wnd = msg->hwnd;
          return 0;
        }

        // message was recieved from currently on top window
        if (msg->hwnd == last_top_wnd) {
          // disable 'Always on Top' for current window
          HMENU menu = GetSystemMenu(msg->hwnd, false);
          CheckMenuItem(menu, SC_ONTOP, MF_UNCHECKED);
          SetWindowPos(msg->hwnd, HWND_NOTOPMOST, 0, 0, 10, 10,
                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);
          // reset saved last on top window
          last_top_wnd = NULL;
          return 0;
        }
      }
      break;
  }

  return CallNextHookEx(hook, ncode, wparam, lparam);
}


WinOnTop::WinOnTop() {
  InitTranslator(QLatin1String(":/win_on_top/win_on_top_"));
  info_.display_name = tr("\"Always on top\" for any window");
  info_.description = tr("Adds \"Always on Top\" menu item to system menu for any window.");
  tr("Always on Top").toWCharArray(item_name);
}

void WinOnTop::Start() {
  HINSTANCE mod_inst = NULL;
  GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCWSTR)&GetMsgProc, &mod_inst);
  hook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, mod_inst, 0);
}

void WinOnTop::Stop() {
  UnhookWindowsHookEx(hook);
}

} // namespace win_on_top
