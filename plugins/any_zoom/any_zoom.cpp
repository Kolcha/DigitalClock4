#include <QInputDialog>
#include "plugin_settings.h"
#include "any_zoom_settings.h"
#include "any_zoom.h"

namespace any_zoom {

AnyZoom::AnyZoom() {
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  is_enabled_ = false;
  last_zoom_ = 1.0;

  InitTranslator(QLatin1String(":/any_zoom/any_zoom_"));
  info_.display_name = tr("Any zoom");
  info_.description = tr("Allows to set any clock zoom.");
  InitIcon(":/any_zoom/icon.svg");
}

void AnyZoom::Init(const QMap<Options, QVariant>& current_settings) {
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);

  settings_->Load();
  last_zoom_ = current_settings[OPT_ZOOM].toReal();
}

void AnyZoom::ExportSettings(QSettings::SettingsMap* settings) {
  if (!settings) return;
  *settings = settings_->GetSettingsMap();
}

void AnyZoom::ImportSettings(const QSettings::SettingsMap& settings) {
  settings_->SetValues(settings);
  settings_->Save();
}

void AnyZoom::Start() {
  is_enabled_ = true;
  emit OptionChanged(OPT_ZOOM, settings_->GetOption(OPT_CURRENT_ZOOM).toInt() / 100.);
}

void AnyZoom::Stop() {
  is_enabled_ = false;
  emit OptionChanged(OPT_ZOOM, last_zoom_);
}

void AnyZoom::Configure() {
  QInputDialog* settings_dlg = new QInputDialog();
  settings_dlg->setAttribute(Qt::WA_DeleteOnClose);
  settings_dlg->setModal(true);

  settings_dlg->setWindowTitle(tr("Any zoom settings"));
  settings_dlg->setLabelText(tr("zoom:"));

  settings_dlg->setInputMode(QInputDialog::IntInput);
  settings_dlg->setIntRange(1, 1000000);
  settings_dlg->setIntValue(settings_->GetOption(OPT_CURRENT_ZOOM).toInt());

  connect(settings_dlg, SIGNAL(intValueChanged(int)), this, SLOT(TrackChange(int)));
  connect(settings_dlg, SIGNAL(intValueSelected(int)), this, SLOT(TrackChange(int)));
  connect(settings_dlg, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(settings_dlg, SIGNAL(rejected()), this, SLOT(RevertSettings()));

  settings_dlg->show();
}

void AnyZoom::TrackChange(int new_zoom) {
  settings_->SetOption(OPT_CURRENT_ZOOM, new_zoom);
  if (is_enabled_) emit OptionChanged(OPT_ZOOM, new_zoom / 100.);
}

void AnyZoom::RevertSettings() {
  settings_->Load();
  if (is_enabled_)
    emit OptionChanged(OPT_ZOOM, settings_->GetOption(OPT_CURRENT_ZOOM).toInt() / 100.);
}

void AnyZoom::SettingsListener(Options option, const QVariant& value) {
  if (option == OPT_ZOOM && is_enabled_) {
    // ignore zoom changes from other plugins (and own change too)
    QString sender_name = sender()->metaObject()->className();
    if (sender_name.contains("PluginManager")) return;
    last_zoom_ = value.toReal();
    emit OptionChanged(OPT_ZOOM, settings_->GetOption(OPT_CURRENT_ZOOM).toInt() / 100.);
  }
}

} // namespace any_zoom
