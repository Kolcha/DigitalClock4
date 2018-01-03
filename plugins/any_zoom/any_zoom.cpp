/*
    Digital Clock: any zoom plugin
    Copyright (C) 2013-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "any_zoom.h"

#include "plugin_settings.h"

#include "any_zoom_settings.h"
#include "zoom_dialog.h"

namespace any_zoom {

AnyZoom::AnyZoom()
{
  is_enabled_ = false;
  last_zoom_ = 1.0;

  InitTranslator(QLatin1String(":/any_zoom/any_zoom_"));
  info_.display_name = tr("Any zoom");
  info_.description = tr("Allows to set any clock zoom, even different values for X and Y axis.");
  InitIcon(":/any_zoom/icon.svg.p");
}

void AnyZoom::Init(const QMap<Option, QVariant>& current_settings)
{
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);

  settings_->Load();
  last_zoom_ = current_settings[OPT_ZOOM].toReal();
}

void AnyZoom::Start()
{
  is_enabled_ = true;
  emit OptionChanged(OPT_ZOOM, QVariant::fromValue(LoadZoom()));
}

void AnyZoom::Stop()
{
  is_enabled_ = false;
  emit OptionChanged(OPT_ZOOM, last_zoom_);
}

void AnyZoom::Configure()
{
  ZoomDialog* settings_dlg = new ZoomDialog();
  settings_dlg->setAttribute(Qt::WA_DeleteOnClose);
  settings_dlg->setModal(true);

  settings_dlg->setZoom(LoadZoom());

  connect(settings_dlg, &ZoomDialog::destroyed, this, &AnyZoom::configured);
  connect(settings_dlg, &ZoomDialog::zoomChanged, this, &AnyZoom::TrackChange);
  connect(settings_dlg, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(settings_dlg, SIGNAL(rejected()), this, SLOT(RevertSettings()));

  settings_dlg->show();
}

void AnyZoom::TrackChange(const skin_draw::Zoom& zoom)
{
  settings_->SetOption(OPT_CURRENT_ZOOM_X, zoom.zoom_x * 100);
  settings_->SetOption(OPT_CURRENT_ZOOM_Y, zoom.zoom_y * 100);
  if (is_enabled_) emit OptionChanged(OPT_ZOOM, QVariant::fromValue(zoom));
}

void AnyZoom::RevertSettings()
{
  settings_->Load();
  if (is_enabled_) emit OptionChanged(OPT_ZOOM, QVariant::fromValue(LoadZoom()));
}

skin_draw::Zoom AnyZoom::LoadZoom() const
{
  ::skin_draw::Zoom curr_zoom;
  curr_zoom.zoom_x = settings_->GetOption(OPT_CURRENT_ZOOM_X).toInt() / 100.;
  curr_zoom.zoom_y = settings_->GetOption(OPT_CURRENT_ZOOM_Y).toInt() / 100.;
  return curr_zoom;
}

void AnyZoom::SettingsListener(Option option, const QVariant& value)
{
  if (option == OPT_ZOOM && is_enabled_) {
    // ignore zoom changes from other plugins (and own change too)
    QString sender_name = sender()->metaObject()->className();
    if (sender_name.contains("PluginManager")) return;
    last_zoom_ = value.toReal();
    emit OptionChanged(OPT_ZOOM, QVariant::fromValue(LoadZoom()));
  }
}

} // namespace any_zoom
