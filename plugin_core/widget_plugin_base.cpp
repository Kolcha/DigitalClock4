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

#include "widget_plugin_base.h"

#include <QFontMetricsF>

#include "skin_drawer.h"
#include "text_skin.h"

#include "plugin_settings.h"

#include "base_settings_widget.h"
#include "widget_plugin_base_private.h"

namespace plugin {

WidgetPluginBase::WidgetPluginBase() : avail_width_(0), private_(new WidgetPluginBasePrivate(this, this))
{
}

void WidgetPluginBase::InitSettings(SettingsStorage* backend, const QString& name)
{
  PluginBase::InitSettings(backend, name);
  QSettings::SettingsMap defaults;
  private_->InitBaseSettingsDefaults(&defaults);
  InitSettingsDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
}

void WidgetPluginBase::Init(const QMap<Option, QVariant>& current_settings)
{
  for (auto iter = current_settings.begin(); iter != current_settings.end(); ++iter) {
    switch (iter.key()) {
      case OPT_FONT:
        private_->clock_font_ = iter.value().value<QFont>();
        break;

      case OPT_COLOR:
        private_->clock_color_ = iter.value().value<QColor>();
        private_->SetColor(private_->clock_color_);
        break;

      case OPT_TEXTURE:
        private_->SetTexture(iter.value().toString());
        break;

      case OPT_TEXTURE_TYPE:
        private_->clock_customization_ = iter.value().value< ::skin_draw::SkinDrawer::CustomizationType>();
        private_->SetCustomizationType(private_->clock_customization_);
        break;

      case OPT_TEXTURE_PER_ELEMENT:
        private_->SetTexturePerElement(iter.value().toBool());
        break;

      case OPT_TEXTURE_DRAW_MODE:
        private_->SetTextureDrawMode(iter.value().value< ::skin_draw::SkinDrawer::DrawMode>());
        break;

      case OPT_CUSTOMIZATION:
      {
        Customization cust = static_cast<Customization>(iter.value().toInt());
        switch (cust) {
          case Customization::C_NONE:
          case Customization::C_COLORIZE:
            private_->clock_customization_ = ::skin_draw::SkinDrawer::CT_NONE;
            private_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
            break;

          default:
            break;
        }
        break;
      }

      default:
        break;
    }
  }
  private_->SetSpacing(2);
}

void WidgetPluginBase::Init(QWidget* main_wnd)
{
  private_->AddClockWidget(main_wnd);

  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          private_, SLOT(SettingsChangeListener(QString,QVariant)));

  avail_width_ = private_->CalculateAvailableSpace();
}

void WidgetPluginBase::Start()
{
  private_->CreateWidgets();
  settings_->Load();
}

void WidgetPluginBase::Stop()
{
  private_->DestroyWidgets();
}

void WidgetPluginBase::SettingsListener(Option option, const QVariant& new_value)
{
  if (private_->plg_widgets_.isEmpty()) return;  // not started

  switch (option) {
    case OPT_SKIN_NAME:
      avail_width_ = private_->CalculateAvailableSpace();

      switch (static_cast<ZoomMode>(settings_->GetOption(OptionKey(OPT_ZOOM_MODE)).toInt())) {
        case ZoomMode::ZM_NOT_ZOOM:
          private_->SetZoom(1.0);
          break;

        case ZoomMode::ZM_AUTOSIZE:
          private_->last_text_ = "-";
          TimeUpdateListener();         // force redraw
          break;
      }

      break;

    case OPT_FONT:
    {
      private_->clock_font_ = new_value.value<QFont>();
      if (!settings_->GetOption(OptionKey(OPT_USE_CLOCK_FONT)).toBool()) break;
      private_->font_ = private_->clock_font_;
      if (settings_->GetOption(OptionKey(OPT_USE_CLOCK_SKIN)).toBool()) break;
      private_->ApplySkin(skin_draw::ISkin::SkinPtr(new ::skin_draw::TextSkin(private_->font_)));
      break;
    }

    case OPT_ZOOM:
      switch (static_cast<ZoomMode>(settings_->GetOption(OptionKey(OPT_ZOOM_MODE)).toInt())) {
        case ZoomMode::ZM_NOT_ZOOM:
          private_->SetZoom(1.0);
          break;

        case ZoomMode::ZM_AUTOSIZE:
          avail_width_ = private_->CalculateAvailableSpace();
          private_->SetZoom(CalculateZoom(private_->last_text_));
          break;
      }

      break;

    case OPT_COLOR:
      private_->clock_color_ = new_value.value<QColor>();
      if (settings_->GetOption(OptionKey(OPT_USE_CUSTOM_COLOR)).toBool()) break;
      private_->SetColor(private_->clock_color_);
      break;

    case OPT_TEXTURE:
      private_->SetTexture(new_value.toString());
      break;

    case OPT_TEXTURE_TYPE:
      private_->clock_customization_ = new_value.value< ::skin_draw::SkinDrawer::CustomizationType>();
      if (settings_->GetOption(OptionKey(OPT_USE_CUSTOM_COLOR)).toBool()) break;
      private_->SetCustomizationType(private_->clock_customization_);
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      private_->SetTexturePerElement(new_value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      private_->SetTextureDrawMode(new_value.value< ::skin_draw::SkinDrawer::DrawMode>());
      break;

    case OPT_CUSTOMIZATION:
    {
      Customization cust = static_cast<Customization>(new_value.toInt());
      switch (cust) {
        case Customization::C_NONE:
        case Customization::C_COLORIZE:
          private_->clock_customization_ = ::skin_draw::SkinDrawer::CT_NONE;
          if (settings_->GetOption(OptionKey(OPT_USE_CUSTOM_COLOR)).toBool()) break;
          private_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
          break;

        default:
          break;
      }
      break;
    }

    default:
      break;
  }
}

void WidgetPluginBase::TimeUpdateListener()
{
  if (private_->plg_widgets_.isEmpty()) return;  // not started

  int cur_avail_width = private_->CalculateAvailableSpace();

  switch (static_cast<ZoomMode>(settings_->GetOption(OptionKey(OPT_ZOOM_MODE)).toInt())) {
    case ZoomMode::ZM_NOT_ZOOM:
      break;

    case ZoomMode::ZM_AUTOSIZE:
      cur_avail_width = private_->CalculateAvailableSpace();
      break;
  }

  QString cur_text = GetWidgetText();
  if (cur_text.isEmpty()) return;

  // optimization: redraw only if needed
  if (cur_text == private_->last_text_ && cur_avail_width == avail_width_) return;

  avail_width_ = cur_avail_width;

  switch (static_cast<ZoomMode>(settings_->GetOption(OptionKey(OPT_ZOOM_MODE)).toInt())) {
    case ZoomMode::ZM_NOT_ZOOM:
      break;

    case ZoomMode::ZM_AUTOSIZE:
      private_->DrawText(QString());  // set empty string to do not redraw twice
      private_->SetZoom(CalculateZoom(cur_text));
      break;
  }

  private_->DrawText(cur_text);
}

void WidgetPluginBase::SetSkin(skin_draw::ISkin::SkinPtr skin)
{
  private_->clock_skin_ = skin;
  if (settings_->GetOption(OptionKey(OPT_USE_CLOCK_SKIN)).toBool()) private_->ApplySkin(skin);
}

void WidgetPluginBase::InitSettingsDefaults(QSettings::SettingsMap* defaults)
{
  Q_UNUSED(defaults);
}

QWidget* WidgetPluginBase::InitConfigWidget(QWidget* parent)
{
  BaseSettingsWidget* cfg_widget = new BaseSettingsWidget(parent);
  QMap<WidgetPluginOption, QVariant> cur_values;
  InitDefaults(&cur_values);
  for (auto iter = cur_values.begin(); iter != cur_values.end(); ++iter)
    *iter = settings_->GetOption(OptionKey(iter.key()));
  cfg_widget->InitWidgets(cur_values);
  connect(cfg_widget, &BaseSettingsWidget::OptionChanged, private_, &WidgetPluginBasePrivate::onBaseOptionChanged);
  return cfg_widget;
}

QSize WidgetPluginBase::GetImageSize(const QString& text, qreal zoom) const
{
  QStringList ss = text.split('\n');
  int tw = 0;
  int th = 0;
  const int space = private_->spacing();

  for (auto& s : qAsConst(ss)) {
    int lw = 0;
    int lh = 0;
    for (int i = 0; i < s.length(); ++i) {
      QPixmap img = private_->currentSkin()->GetImage(s, i, zoom, true);
      if (!img) continue;
      lw += img.width() + space;
      lh = qMax(lh, img.height());
    }
    tw = qMax(tw, lw);
    th += lh + space;
  }
  return QSize(tw, th);
}

qreal WidgetPluginBase::CalculateZoom(const QString& text) const
{
  Q_ASSERT(!text.isEmpty());
  int iw_loc = settings_->GetOption(OptionKey(OPT_WIDGET_LOCATION)).toInt();
  WidgetLocation w_loc = static_cast<WidgetLocation>(iw_loc);

  qreal tw = w_loc == WidgetLocation::WL_RIGHT ?  GetImageSize(text, 1.0).height() : GetImageSize(text, 1.0).width();
  qreal avail_width = avail_width_;
  avail_width *= 0.01 * settings_->GetOption(OptionKey(OPT_SPACE_PERCENT)).toInt();
  qreal c_zoom = avail_width / tw;

  int c_img_w = w_loc == WidgetLocation::WL_RIGHT ? GetImageSize(text, c_zoom).height() : GetImageSize(text, c_zoom).width();
  while (c_img_w > avail_width) {
    c_zoom *= (1 - (0.5*(c_img_w - avail_width)) / avail_width);
    c_img_w = w_loc == WidgetLocation::WL_RIGHT ? GetImageSize(text, c_zoom).height() : GetImageSize(text, c_zoom).width();
  }

  return c_zoom;
}

} // namespace plugin
