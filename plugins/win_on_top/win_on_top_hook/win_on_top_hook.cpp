/*
    window on top hook library
    Copyright (C) 2014-2019  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Windows.h>
#include <Strsafe.h>

#pragma data_seg(".SHARED")
HWND last_top_wnd = NULL;
WCHAR item_name[32] = L"Always On Top";
#pragma data_seg()
#pragma comment(linker,"/section:.SHARED,rws")

HHOOK hook = NULL;   // installed hook handle

#define SC_ONTOP     0x01  // command code for addd menu item

// hook processor function
LRESULT CALLBACK GetMsgProc(int ncode, WPARAM wparam, LPARAM lparam)
{
  if (ncode < 0) return CallNextHookEx(hook, ncode, wparam, lparam);

  LPMSG msg = (LPMSG)lparam;

  switch (msg->message) {
    case WM_NCRBUTTONDOWN:
    {
      // add 'Alway on Top' menu item if it doesn't exists for current window
      HMENU menu = GetSystemMenu(msg->hwnd, false);
      if (GetMenuItemID(menu, GetMenuItemCount(menu) - 1) != SC_ONTOP) {
        AppendMenuW(menu, MF_SEPARATOR, 0, 0);
        AppendMenuW(menu, MF_UNCHECKED, SC_ONTOP, item_name);
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


extern "C" {

__declspec(dllexport) void SetItemName(LPCWSTR name)
{
  memset(item_name, 0, 64);
  StringCchCopyW(item_name, 32, name);
}

__declspec(dllexport) void Start()
{
  HMODULE mod_inst = NULL;
  GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCWSTR)&GetMsgProc, &mod_inst);
  hook = SetWindowsHookExW(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, mod_inst, 0);
}

__declspec(dllexport) void Stop()
{
  UnhookWindowsHookEx(hook);
}

}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  (void)hModule;
  (void)ul_reason_for_call;
  (void)lpReserved;
  return TRUE;
}
