/*
    Digital Clock: plugin core library
    Copyright (C) 2016-2018  Nick Korotysh <nick.korotysh@gmail.com>

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
#include <QLabel>

#include "plugin_settings.h"

#include "text_skin.h"

#include "widget_plugin_base.h"

namespace plugin {

WidgetPluginBasePrivate::WidgetPluginBasePrivate(WidgetPluginBase* obj, QObject* parent) :
  QObject(parent),
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
    defaults->insert(OptionKey(iter.key()), iter.value());
}

int WidgetPluginBasePrivate::CalculateAvailableSpace() const
{
  int w_loc = obj_->settings_->GetOption(OptionKey(OPT_WIDGET_LOCATION)).toInt();
  if (static_cast<WidgetLocation>(w_loc) == WidgetLocation::WL_RIGHT) {
    return main_layouts_[0]->itemAtPosition(0, 0)->sizeHint().height();
  } else {
    return main_layouts_[0]->itemAtPosition(0, 0)->sizeHint().width();
  }
}

void WidgetPluginBasePrivate::onBaseOptionChanged(const WidgetPluginOption opt, const QVariant& value)
{
  obj_->settings_->SetOption(OptionKey(opt), value);
}

void WidgetPluginBasePrivate::SettingsChangeListener(const QString& key, const QVariant& value)
{
  if (plg_widgets_.isEmpty()) return;  // not started

  if (key == OptionKey(OPT_USE_CLOCK_FONT)) {
    QString font_key = OptionKey(OPT_CUSTOM_FONT);
    SettingsChangeListener(font_key, value.toBool() ?
                           clock_font_ : obj_->settings_->GetOption(font_key).value<QFont>());
  }
  if (key == OptionKey(OPT_CUSTOM_FONT)) {
    font_ = value.value<QFont>();
    if (!obj_->settings_->GetOption(OptionKey(OPT_USE_CLOCK_SKIN)).toBool())
      ApplySkin(CreateTextSkin(font_));
  }
  if (key == OptionKey(OPT_ZOOM_MODE)) {
    switch (static_cast<ZoomMode>(value.toInt())) {
      case ZoomMode::ZM_NOT_ZOOM:
        drawer_->SetZoom(1.0);
        break;

      case ZoomMode::ZM_AUTOSIZE:
        obj_->avail_width_ = CalculateAvailableSpace();
        drawer_->SetZoom(obj_->CalculateZoom(last_text_));
        break;
    }
  }
  if (key == OptionKey(OPT_SPACE_PERCENT)) {
    int c_zoom_mode = obj_->settings_->GetOption(OptionKey(OPT_ZOOM_MODE)).toInt();
    if (static_cast<ZoomMode>(c_zoom_mode) == ZoomMode::ZM_AUTOSIZE) {
      drawer_->SetZoom(obj_->CalculateZoom(last_text_));
    }
  }
  if (key == OptionKey(OPT_WIDGET_LOCATION)) {
    Q_ASSERT(main_layouts_.size() == plg_widgets_.size());
    for (int i = 0; i < plg_widgets_.size(); ++i) {
      QGridLayout* layout = main_layouts_[i];
      QWidget* plg_widget = plg_widgets_[i];
      layout->removeWidget(plg_widget);
      switch (static_cast<WidgetLocation>(value.toInt())) {
        case WidgetLocation::WL_BOTTOM:
          obj_->avail_width_ = layout->itemAtPosition(0, 0)->sizeHint().width();
          layout->addWidget(plg_widget, layout->rowCount(), 0, 1, layout->columnCount());
          break;

        case WidgetLocation::WL_RIGHT:
          obj_->avail_width_ = layout->itemAtPosition(0, 0)->sizeHint().height();
          layout->addWidget(plg_widget, 0, layout->columnCount(), 1, 1);
          break;
      }
    }

    int c_zoom_mode = obj_->settings_->GetOption(OptionKey(OPT_ZOOM_MODE)).toInt();
    if (static_cast<ZoomMode>(c_zoom_mode) == ZoomMode::ZM_AUTOSIZE) {
      drawer_->SetZoom(obj_->CalculateZoom(last_text_));
    }
  }
  if (key == OptionKey(OPT_ALIGNMENT)) {
    for (auto& plg_widget : plg_widgets_) {
      QLabel* lbl = qobject_cast<QLabel*>(plg_widget);
      if (lbl) lbl->setAlignment(static_cast<Qt::Alignment>(value.toInt()));
    }
  }
  if (key == OptionKey(OPT_USE_CUSTOM_COLOR)) {
    drawer_->SetString(QString());    // set empty string to do not redraw twice
    if (value.toBool()) {
      drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_COLOR);
      drawer_->SetColor(obj_->settings_->GetOption(OptionKey(OPT_CUSTOM_COLOR)).value<QColor>());
    } else {
      drawer_->SetCustomizationType(clock_customization_);
      drawer_->SetColor(clock_color_);
    }
    if (last_text_ != QString("-")) drawer_->SetString(last_text_);
  }
  if (key == OptionKey(OPT_CUSTOM_COLOR)) {
    if (obj_->settings_->GetOption(OptionKey(OPT_USE_CUSTOM_COLOR)).toBool()) {
      drawer_->SetColor(value.value<QColor>());
    }
  }
  if (key == OptionKey(OPT_USE_CLOCK_SKIN)) {
    ApplySkin(value.toBool() ? clock_skin_ : CreateTextSkin(font_));
  }
}

void WidgetPluginBasePrivate::AddClockWidget(QWidget* main_wnd)
{
  main_layouts_.append(qobject_cast<QGridLayout*>(main_wnd->layout()));
  main_wnds_.append(main_wnd);
}

void WidgetPluginBasePrivate::CreateWidgets()
{
  for (auto layout : main_layouts_) {
    plg_widgets_.append(obj_->InitWidget(layout));
    if (layout->indexOf(plg_widgets_.last()) == -1) {
      int w_loc = obj_->settings_->GetOption(OptionKey(OPT_WIDGET_LOCATION)).toInt();
      if (static_cast<WidgetLocation>(w_loc) == WidgetLocation::WL_RIGHT) {
        layout->addWidget(plg_widgets_.last(), 0, layout->columnCount(), 1, 1);
      } else {
        layout->addWidget(plg_widgets_.last(), layout->rowCount(), 0, 1, layout->columnCount());
      }
    }
  }
  connect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, [this] (const QImage& img) {
    for (auto& widget : plg_widgets_) obj_->DisplayImage(widget, img);
  });
}

void WidgetPluginBasePrivate::DestroyWidgets()
{
  disconnect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, 0, 0);
  Q_ASSERT(plg_widgets_.size() <= main_layouts_.size());
  Q_ASSERT(main_wnds_.size() == main_layouts_.size());
  for (int i = 0; i < plg_widgets_.size(); ++i) {
    main_layouts_[i]->removeWidget(plg_widgets_[i]);
    delete plg_widgets_[i];
  }
  plg_widgets_.clear();
  main_layouts_.clear();
  main_wnds_.clear();
}

skin_draw::ISkin::SkinPtr WidgetPluginBasePrivate::CreateTextSkin(const QFont& fnt)
{
  skin_draw::ISkin::SkinPtr txt_skin(new ::skin_draw::TextSkin(fnt));
  txt_skin->SetDevicePixelRatio(main_wnds_[0]->devicePixelRatioF());
  return txt_skin;
}

void WidgetPluginBasePrivate::ApplySkin(skin_draw::ISkin::SkinPtr skin)
{
  drawer_->SetString(QString());
  drawer_->ApplySkin(skin);
  last_text_ = "-";             // reset last date to recalculate zoom
  obj_->TimeUpdateListener();   // force redraw
}

} // namespace plugin
