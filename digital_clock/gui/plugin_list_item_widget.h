/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_GUI_PLUGIN_LIST_ITEM_WIDGET_H
#define DIGITAL_CLOCK_GUI_PLUGIN_LIST_ITEM_WIDGET_H

#include <QWidget>
#include "core/plugin_info.h"

namespace digital_clock {
namespace gui {

namespace Ui {
class PluginListItemWidget;
}

class PluginListItemWidget : public QWidget
{
  Q_OBJECT

public:
  explicit PluginListItemWidget(QWidget* parent = 0);
  ~PluginListItemWidget();

  QString GetName() const;

public slots:
  void SetDisplayName(const QString& name);
  void SetVersion(const QString& version);
  void SetConfigurable(bool configable);
  void SetChecked(bool checked);
  void SetInfo(const TPluginInfo& info);

signals:
  void StateChanged(const QString& name, bool new_state);
  void ConfigureRequested(const QString& name);

private slots:
  void on_name_check_box_toggled(bool checked);
  void on_config_btn_clicked();
  void on_info_btn_clicked();

private:
  Ui::PluginListItemWidget* ui;
  TPluginInfo info_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_PLUGIN_LIST_ITEM_WIDGET_H
