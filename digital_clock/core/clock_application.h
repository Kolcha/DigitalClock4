/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_CORE_CLOCK_APPLICATION_H
#define DIGITAL_CLOCK_CORE_CLOCK_APPLICATION_H

#include <QObject>
#include <QVector>
#include <QScreen>
#include <QTimer>

#include "settings_keys.h"

class SettingsStorage;

namespace digital_clock {

class MouseTracker;

namespace gui {
class TrayControl;
class ClockWindow;
}

namespace core {

class ClockSettings;
class ClockState;
class SkinManager;
class Updater;
class PluginManager;

class ClockApplication : public QObject
{
  Q_OBJECT

public:
  explicit ClockApplication(ClockSettings* config, QObject* parent = nullptr);
  ~ClockApplication();

private slots:
  void UpdateVisibilityAction();

  void Reset();
  void ApplyOption(const Option opt, const QVariant& value);

  void ShowSettingsDialog();
  void ShowAboutDialog();

  void OnScreenRemoved(QScreen* screen);

  // temporary, will be changed later
  void InitPluginSystem();
  void ShutdownPluginSystem();

private:
  void CreateWindows();
  void ConnectTrayMessages();

  SettingsStorage* config_backend_;

  core::ClockSettings* app_config_;
  core::ClockState* state_;
  core::SkinManager* skin_manager_;
  core::Updater* updater_;
  core::PluginManager* plugin_manager_;

  gui::TrayControl* tray_control_;
  MouseTracker* mouse_tracker_;

  QTimer timer_;

  QVector<gui::ClockWindow*> clock_windows_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_CLOCK_APPLICATION_H
