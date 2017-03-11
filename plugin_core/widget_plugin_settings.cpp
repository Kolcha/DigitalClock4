/*
    Digital Clock: plugin core library
    Copyright (C) 2016-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "widget_plugin_settings.h"

#include <QFont>
#include <QColor>

namespace plugin {

QString OptionKey(const plugin::WidgetPluginOption opt, const QString& plg_name)
{
  QString fmt_str("plugins/%1/%2");

  switch (opt) {
    case OPT_USE_CLOCK_FONT:      return fmt_str.arg(plg_name, "use_clock_font");
    case OPT_CUSTOM_FONT:         return fmt_str.arg(plg_name, "custom_font");
    case OPT_ZOOM_MODE:           return fmt_str.arg(plg_name, "zoom_mode");
    case OPT_SPACE_PERCENT:       return fmt_str.arg(plg_name, "space_percent");
    case OPT_ALIGNMENT:           return fmt_str.arg(plg_name, "alignment");
    case OPT_USE_CUSTOM_COLOR:    return fmt_str.arg(plg_name, "use_custom_color");
    case OPT_CUSTOM_COLOR:        return fmt_str.arg(plg_name, "custom_color");
  }

  Q_ASSERT(false);
  return QString();
}

void InitDefaults(QMap<WidgetPluginOption, QVariant>* defaults)
{
  Q_ASSERT(defaults);
  defaults->insert(OPT_USE_CLOCK_FONT, true);
  defaults->insert(OPT_CUSTOM_FONT, QFont());
  defaults->insert(OPT_ZOOM_MODE, static_cast<int>(ZoomMode::ZM_AUTOSIZE));
  defaults->insert(OPT_SPACE_PERCENT, 100);
  defaults->insert(OPT_ALIGNMENT, static_cast<int>(Qt::AlignCenter));
  defaults->insert(OPT_USE_CUSTOM_COLOR, false);
  defaults->insert(OPT_CUSTOM_COLOR, QColor(0, 170, 255));
}

} // namespace plugin
