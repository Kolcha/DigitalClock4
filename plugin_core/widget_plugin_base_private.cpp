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
        SetZoom(1.0);
        break;

      case ZoomMode::ZM_AUTOSIZE:
        obj_->avail_width_ = CalculateAvailableSpace();
        SetZoom(obj_->CalculateZoom(last_text_));
        break;
    }
  }
  if (key == OptionKey(OPT_SPACE_PERCENT)) {
    int c_zoom_mode = obj_->settings_->GetOption(OptionKey(OPT_ZOOM_MODE)).toInt();
    if (static_cast<ZoomMode>(c_zoom_mode) == ZoomMode::ZM_AUTOSIZE) {
      SetZoom(obj_->CalculateZoom(last_text_));
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
      SetZoom(obj_->CalculateZoom(last_text_));
    }
  }
  if (key == OptionKey(OPT_ALIGNMENT)) {
    for (auto& plg_widget : plg_widgets_) {
      QLabel* lbl = qobject_cast<QLabel*>(plg_widget);
      if (lbl) lbl->setAlignment(static_cast<Qt::Alignment>(value.toInt()));
    }
  }
  if (key == OptionKey(OPT_USE_CUSTOM_COLOR)) {
    DrawText(QString());    // set empty string to do not redraw twice
    if (value.toBool()) {
      SetCustomizationType(::skin_draw::SkinDrawer::CT_COLOR);
      SetColor(obj_->settings_->GetOption(OptionKey(OPT_CUSTOM_COLOR)).value<QColor>());
    } else {
      SetCustomizationType(clock_customization_);
      SetColor(clock_color_);
    }
    if (last_text_ != QString("-")) DrawText(last_text_);
  }
  if (key == OptionKey(OPT_CUSTOM_COLOR)) {
    if (obj_->settings_->GetOption(OptionKey(OPT_USE_CUSTOM_COLOR)).toBool()) {
      SetColor(value.value<QColor>());
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
    QWidget* widget = obj_->InitWidget(layout);
    plg_widgets_.append(widget);
    if (layout->indexOf(plg_widgets_.last()) == -1) {
      int w_loc = obj_->settings_->GetOption(OptionKey(OPT_WIDGET_LOCATION)).toInt();
      if (static_cast<WidgetLocation>(w_loc) == WidgetLocation::WL_RIGHT) {
        layout->addWidget(plg_widgets_.last(), 0, layout->columnCount(), 1, 1);
      } else {
        layout->addWidget(plg_widgets_.last(), layout->rowCount(), 0, 1, layout->columnCount());
      }
    }
    ::skin_draw::SkinDrawer* drawer = new ::skin_draw::SkinDrawer(widget);
    drawer->SetDevicePixelRatio(widget->devicePixelRatioF());
    connect(drawer, &skin_draw::SkinDrawer::DrawingFinished, [this, widget] (const QImage& img) {
      obj_->DisplayImage(widget, img);
    });
    drawer->ApplySkin(skin_);
    drawer->SetColor(color_);
    drawer->SetTexture(texture_);
    drawer->SetCustomizationType(customization_);
    drawer->SetTexturePerElement(texture_per_element_);
    drawer->SetTextureDrawMode(draw_mode_);
    drawer->SetSpace(spacing_);
    drawer->SetZoom(zoom_);
    drawers_.append(drawer);
  }
}

void WidgetPluginBasePrivate::DestroyWidgets()
{
  Q_ASSERT(drawers_.size() == plg_widgets_.size());
  Q_ASSERT(plg_widgets_.size() <= main_layouts_.size());
  Q_ASSERT(main_wnds_.size() == main_layouts_.size());
  for (int i = 0; i < plg_widgets_.size(); ++i) {
    delete drawers_[i];
    main_layouts_[i]->removeWidget(plg_widgets_[i]);
    delete plg_widgets_[i];
  }
  drawers_.clear();
  plg_widgets_.clear();
  main_layouts_.clear();
  main_wnds_.clear();
}

void WidgetPluginBasePrivate::ApplySkin(skin_draw::ISkin::SkinPtr skin)
{
  skin_ = skin;
  for (auto& drawer : drawers_) {
    drawer->SetString(QString());
    drawer->ApplySkin(skin);
  }
  last_text_ = "-";             // reset last date to recalculate zoom
  obj_->TimeUpdateListener();   // force redraw
}

void WidgetPluginBasePrivate::SetColor(const QColor& color)
{
  if (!color.isValid() || color_ == color) return;
  color_ = color;
  for (auto& drawer : drawers_) drawer->SetColor(color);
}

void WidgetPluginBasePrivate::SetTexture(const QString& texture)
{
  if (texture.isEmpty() || texture_ == texture) return;
  texture_ = texture;
  for (auto& drawer : drawers_) drawer->SetTexture(texture);
}

void WidgetPluginBasePrivate::SetCustomizationType(const skin_draw::SkinDrawer::CustomizationType ct)
{
  if (customization_ == ct) return;
  customization_ = ct;
  for (auto& drawer : drawers_) drawer->SetCustomizationType(ct);
}

void WidgetPluginBasePrivate::SetTexturePerElement(bool enable)
{
  if (texture_per_element_ == enable) return;
  texture_per_element_ = enable;
  for (auto& drawer : drawers_) drawer->SetTexturePerElement(enable);
}

void WidgetPluginBasePrivate::SetTextureDrawMode(const skin_draw::SkinDrawer::DrawMode dm)
{
  if (draw_mode_ == dm) return;
  draw_mode_ = dm;
  for (auto& drawer : drawers_) drawer->SetTextureDrawMode(dm);
}

void WidgetPluginBasePrivate::SetSpacing(const int spacing)
{
  if (spacing_ != spacing) return;
  spacing_ = spacing;
  for (auto& drawer : drawers_) drawer->SetSpace(spacing);
}

void WidgetPluginBasePrivate::SetZoom(const qreal zoom)
{
  if (qFuzzyIsNull(zoom) || qFuzzyCompare(zoom_, zoom)) return;
  zoom_ = zoom;
  for (auto& drawer : drawers_) drawer->SetZoom(zoom);
}

void WidgetPluginBasePrivate::DrawText(const QString& text)
{
  if (!text.isEmpty()) last_text_ = text;
  for (auto& drawer : drawers_) drawer->SetString(text);
}

skin_draw::ISkin::SkinPtr WidgetPluginBasePrivate::CreateTextSkin(const QFont& fnt)
{
  return skin_draw::ISkin::SkinPtr(new ::skin_draw::TextSkin(fnt));
}

} // namespace plugin
