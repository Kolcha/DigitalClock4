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

#include "widget_plugin_base_private.h"

#include <QWidget>
#include <QGridLayout>

#include "plugin_settings.h"

#include "text_skin.h"

#include "widget_plugin_base.h"

namespace plugin {

WidgetPluginBasePrivate::WidgetPluginBasePrivate(WidgetPluginBase* obj, QObject* parent) :
  QObject(parent),
  main_layout_(nullptr), main_wnd_(nullptr),
  clock_zoom_(1.0),
  clock_customization_(::skin_draw::SkinDrawer::CT_COLOR),
  clock_color_(0, 170, 255),
  last_text_("-----"),
  drawer_(new ::skin_draw::SkinDrawer(this)),
  obj_(obj)
{
}

void WidgetPluginBasePrivate::InitBaseSettingsDefaults(QSettings::SettingsMap* defaults)
{
  QMap<WidgetPluginOption, QVariant> base_defaults;
  InitDefaults(&base_defaults);
  for (auto iter = base_defaults.begin(); iter != base_defaults.end(); ++iter)
    defaults->insert(OptionKey(iter.key(), obj_->plg_name_), iter.value());
}

void WidgetPluginBasePrivate::onBaseOptionChanged(const WidgetPluginOption opt, const QVariant& value)
{
  obj_->settings_->SetOption(OptionKey(opt, obj_->plg_name_), value);
}

void WidgetPluginBasePrivate::SettingsChangeListener(const QString& key, const QVariant& value)
{
  if (!plg_widget_) return;  // not started

  if (key == OptionKey(OPT_USE_CLOCK_FONT, obj_->plg_name_)) {
    QString font_key = OptionKey(OPT_CUSTOM_FONT, obj_->plg_name_);
    SettingsChangeListener(font_key, value.toBool() ?
                           clock_font_ : obj_->settings_->GetOption(font_key).value<QFont>());
  }
  if (key == OptionKey(OPT_CUSTOM_FONT, obj_->plg_name_)) {
    font_ = value.value<QFont>();
    drawer_->SetString(QString());
    skin_draw::ISkin::SkinPtr txt_skin(new ::skin_draw::TextSkin(font_));
    txt_skin->SetDevicePixelRatio(main_wnd_->devicePixelRatioF());
    drawer_->ApplySkin(txt_skin);
    last_text_ = "-";             // reset last date to recalculate zoom
    obj_->TimeUpdateListener();   // on redraw if needed
  }
  if (key == OptionKey(OPT_ZOOM_MODE, obj_->plg_name_)) {
    switch (static_cast<ZoomMode>(value.toInt())) {
      case ZoomMode::ZM_NOT_ZOOM:
        drawer_->SetZoom(1.0);
        break;

      case ZoomMode::ZM_AUTOSIZE:
        plg_widget_->hide();
        main_wnd_->adjustSize();
        obj_->avail_width_ = main_layout_->cellRect(0, 0).width();
        drawer_->SetZoom(obj_->CalculateZoom(last_text_));
        break;

      case ZoomMode::ZM_CLOCK_ZOOM:
        drawer_->SetZoom(clock_zoom_);
        break;
    }
  }
  if (key == OptionKey(OPT_USE_CUSTOM_COLOR, obj_->plg_name_)) {
    drawer_->SetString(QString());    // set empty string to do not redraw twice
    if (value.toBool()) {
      drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_COLOR);
      drawer_->SetColor(obj_->settings_->GetOption(OptionKey(OPT_CUSTOM_COLOR, obj_->plg_name_)).value<QColor>());
    } else {
      drawer_->SetCustomizationType(static_cast<::skin_draw::SkinDrawer::CustomizationType>(clock_customization_));
      drawer_->SetColor(clock_color_);
    }
    drawer_->SetString(last_text_);
  }
  if (key == OptionKey(OPT_CUSTOM_COLOR, obj_->plg_name_)) {
    if (obj_->settings_->GetOption(OptionKey(OPT_USE_CUSTOM_COLOR, obj_->plg_name_)).toBool()) {
      drawer_->SetColor(value.value<QColor>());
    }
  }
}

} // namespace plugin
