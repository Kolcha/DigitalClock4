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

#include "widget_plugin_base.h"

#include <QGridLayout>
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

void WidgetPluginBase::Init(const QMap<Option, QVariant>& current_settings)
{
  for (auto iter = current_settings.begin(); iter != current_settings.end(); ++iter) {
    switch (iter.key()) {
      case OPT_FONT:
        private_->clock_font_ = iter.value().value<QFont>();
        break;

      case OPT_COLOR:
        private_->clock_color_ = iter.value().value<QColor>();
        private_->drawer_->SetColor(private_->clock_color_);
        break;

      case OPT_TEXTURE:
        private_->drawer_->SetTexture(iter.value().toString());
        break;

      case OPT_TEXTURE_TYPE:
        private_->clock_customization_ = static_cast<skin_draw::SkinDrawer::CustomizationType>(iter.value().toInt());
        private_->drawer_->SetCustomizationType(private_->clock_customization_);
        break;

      case OPT_TEXTURE_PER_ELEMENT:
        private_->drawer_->SetTexturePerElement(iter.value().toBool());
        break;

      case OPT_TEXTURE_DRAW_MODE:
        private_->drawer_->SetTextureDrawMode(static_cast<skin_draw::SkinDrawer::DrawMode>(iter.value().toInt()));
        break;

      case OPT_CUSTOMIZATION:
      {
        Customization cust = static_cast<Customization>(iter.value().toInt());
        switch (cust) {
          case Customization::C_NONE:
          case Customization::C_COLORIZE:
            private_->clock_customization_ = ::skin_draw::SkinDrawer::CT_NONE;
            private_->drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
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
  private_->drawer_->SetSpace(0);
}

void WidgetPluginBase::Init(QWidget* main_wnd)
{
  private_->main_layout_ = qobject_cast<QGridLayout*>(main_wnd->layout());
  private_->main_wnd_ = main_wnd;

  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          private_, SLOT(SettingsChangeListener(QString,QVariant)));

  QSettings::SettingsMap defaults;
  private_->InitBaseSettingsDefaults(&defaults);
  InitSettingsDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);

  avail_width_ = private_->CalculateAvailableSpace();
}

void WidgetPluginBase::Start()
{
  QGridLayout* layout = private_->main_layout_;
  private_->plg_widget_ = InitWidget(private_->main_layout_);
  if (private_->main_layout_->indexOf(private_->plg_widget_) == -1) {
    int w_loc = settings_->GetOption(OptionKey(OPT_WIDGET_LOCATION, plg_name_)).toInt();
    if (static_cast<WidgetLocation>(w_loc) == WidgetLocation::WL_RIGHT) {
      layout->addWidget(private_->plg_widget_, 0, layout->columnCount(), 1, 1);
    } else {
      layout->addWidget(private_->plg_widget_, layout->rowCount(), 0, 1, layout->columnCount());
    }
  }
  connect(private_->drawer_, &skin_draw::SkinDrawer::DrawingFinished, [=] (const QImage& img) {
    this->DisplayImage(img);
    private_->plg_widget_->adjustSize();
    if (private_->plg_widget_->isHidden()) private_->plg_widget_->show();
    private_->main_wnd_->adjustSize();
  });

  settings_->Load();
}

void WidgetPluginBase::Stop()
{
  disconnect(private_->drawer_, &skin_draw::SkinDrawer::DrawingFinished, 0, 0);
  private_->main_layout_->removeWidget(private_->plg_widget_);
  private_->main_wnd_->adjustSize();
  delete private_->plg_widget_;
}

void WidgetPluginBase::SettingsListener(Option option, const QVariant& new_value)
{
  if (!private_->plg_widget_) return;  // not started

  switch (option) {
    case OPT_SKIN_NAME:
      private_->plg_widget_->hide();
      private_->main_wnd_->adjustSize();
      avail_width_ = private_->CalculateAvailableSpace();

      switch (static_cast<ZoomMode>(settings_->GetOption(OptionKey(OPT_ZOOM_MODE, plg_name_)).toInt())) {
        case ZoomMode::ZM_NOT_ZOOM:
          private_->drawer_->SetZoom(1.0);
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
      if (!settings_->GetOption(OptionKey(OPT_USE_CLOCK_FONT, plg_name_)).toBool()) break;
      private_->font_ = private_->clock_font_;
      skin_draw::ISkin::SkinPtr txt_skin(new ::skin_draw::TextSkin(private_->font_));
      txt_skin->SetDevicePixelRatio(private_->main_wnd_->devicePixelRatioF());
      private_->drawer_->ApplySkin(txt_skin);
      break;
    }

    case OPT_ZOOM:
      switch (static_cast<ZoomMode>(settings_->GetOption(OptionKey(OPT_ZOOM_MODE, plg_name_)).toInt())) {
        case ZoomMode::ZM_NOT_ZOOM:
          private_->drawer_->SetZoom(1.0);
          break;

        case ZoomMode::ZM_AUTOSIZE:
          private_->plg_widget_->hide();
          private_->main_wnd_->adjustSize();
          avail_width_ = private_->CalculateAvailableSpace();
          private_->drawer_->SetZoom(CalculateZoom(private_->last_text_));
          break;
      }

      break;

    case OPT_COLOR:
      private_->clock_color_ = new_value.value<QColor>();
      if (settings_->GetOption(OptionKey(OPT_USE_CUSTOM_COLOR, plg_name_)).toBool()) break;
      private_->drawer_->SetColor(private_->clock_color_);
      break;

    case OPT_TEXTURE:
      private_->drawer_->SetTexture(new_value.toString());
      break;

    case OPT_TEXTURE_TYPE:
      private_->clock_customization_ = static_cast<skin_draw::SkinDrawer::CustomizationType>(new_value.toInt());
      if (settings_->GetOption(OptionKey(OPT_USE_CUSTOM_COLOR, plg_name_)).toBool()) break;
      private_->drawer_->SetCustomizationType(private_->clock_customization_);
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      private_->drawer_->SetTexturePerElement(new_value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      private_->drawer_->SetTextureDrawMode(static_cast<skin_draw::SkinDrawer::DrawMode>(new_value.toInt()));
      break;

    case OPT_CUSTOMIZATION: {
      Customization cust = static_cast<Customization>(new_value.toInt());
      switch (cust) {
        case Customization::C_NONE:
        case Customization::C_COLORIZE:
          private_->clock_customization_ = ::skin_draw::SkinDrawer::CT_NONE;
          if (settings_->GetOption(OptionKey(OPT_USE_CUSTOM_COLOR, plg_name_)).toBool()) break;
          private_->drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
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
  if (!private_->plg_widget_) return;  // not started

  int cur_avail_width = private_->CalculateAvailableSpace();

  switch (static_cast<ZoomMode>(settings_->GetOption(OptionKey(OPT_ZOOM_MODE, plg_name_)).toInt())) {
    case ZoomMode::ZM_NOT_ZOOM:
      break;

    case ZoomMode::ZM_AUTOSIZE:
      private_->plg_widget_->hide();
      private_->main_wnd_->adjustSize();
      cur_avail_width = private_->CalculateAvailableSpace();
      private_->plg_widget_->show();
      break;
  }

  QString cur_text = GetWidgetText();

  // optimization: redraw only if needed
  if (cur_text == private_->last_text_ && cur_avail_width == avail_width_) return;

  avail_width_ = cur_avail_width;

  switch (static_cast<ZoomMode>(settings_->GetOption(OptionKey(OPT_ZOOM_MODE, plg_name_)).toInt())) {
    case ZoomMode::ZM_NOT_ZOOM:
      break;

    case ZoomMode::ZM_AUTOSIZE:
      private_->drawer_->SetString(QString());  // set empty string to do not redraw twice
      private_->drawer_->SetZoom(CalculateZoom(cur_text));
      break;
  }

  private_->drawer_->SetString(cur_text);
  private_->last_text_ = cur_text;
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
    *iter = settings_->GetOption(OptionKey(iter.key(), plg_name_));
  cfg_widget->InitWidgets(cur_values);
  connect(cfg_widget, &BaseSettingsWidget::OptionChanged, private_, &WidgetPluginBasePrivate::onBaseOptionChanged);
  return cfg_widget;
}

QSize WidgetPluginBase::GetImageSize(const QString& text, qreal zoom) const
{
  QStringList ss = text.split('\n');
  int tw = 0;
  int th = 0;

  skin_draw::TextSkin tmp_skin(private_->font_);
  tmp_skin.SetDevicePixelRatio(private_->main_wnd_->devicePixelRatioF());
  for (auto& s : ss) {
    int lw = 0;
    int lh = 0;
    for (int i = 0; i < s.length(); ++i) {
      ::skin_draw::ISkin::QPixmapPtr img = tmp_skin.GetImage(s, i, zoom, true);
      if (!img) continue;
      lw += img->width();
      lh = qMax(lh, img->height());
    }
    tw = qMax(tw, lw);
    th += lh;
  }
  return QSize(tw, th);
}

qreal WidgetPluginBase::CalculateZoom(const QString& text) const
{
  int iw_loc = settings_->GetOption(OptionKey(OPT_WIDGET_LOCATION, plg_name_)).toInt();
  WidgetLocation w_loc = static_cast<WidgetLocation>(iw_loc);

  qreal tw = w_loc == WidgetLocation::WL_RIGHT ?  GetImageSize(text, 1.0).height() : GetImageSize(text, 1.0).width();
  qreal avail_width = avail_width_ * private_->plg_widget_->devicePixelRatioF();
  avail_width *= 0.01 * settings_->GetOption(OptionKey(OPT_SPACE_PERCENT, plg_name_)).toInt();
  qreal c_zoom = avail_width / tw;

  int c_img_w = w_loc == WidgetLocation::WL_RIGHT ? GetImageSize(text, c_zoom).height() : GetImageSize(text, c_zoom).width();
  while (c_img_w > avail_width) {
    c_zoom *= (1 - (0.5*(c_img_w - avail_width)) / avail_width);
    c_img_w = w_loc == WidgetLocation::WL_RIGHT ? GetImageSize(text, c_zoom).height() : GetImageSize(text, c_zoom).width();
  }

  return c_zoom;
}

} // namespace plugin
