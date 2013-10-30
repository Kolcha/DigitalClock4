#include "extra_zoom_settings.h"
#include "extra_zoom.h"

ExtraZoom::ExtraZoom() {
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  is_enabled_ = false;
}

void ExtraZoom::Init(const QMap<Options, QVariant>& current_settings, QWidget* parent) {
  parent_ = parent;

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);

  settings_->Load();

  if (current_settings[OPT_ZOOM].toReal() < settings_->GetOption(OPT_PREVIOUS_ZOOM).toReal()) {
    settings_->SetOption(OPT_PREVIOUS_ZOOM, current_settings[OPT_ZOOM]);
    settings_->Save();
  }
}

void ExtraZoom::GetInfo(TPluginInfo* info) {
  info->insert(PI_NAME, "Extra zoom");
  info->insert(PI_TYPE, "settings");
  info->insert(PI_VERSION, "1.0");
  info->insert(PI_AUTHOR, "Nick Korotysh");
  info->insert(PI_EMAIL, "nick.korotysh@gmail.com");
  info->insert(PI_COMMENT, "Can set > 400% clock zoom.");
  info->insert(PI_CONFIG, "true");
}

void ExtraZoom::Start() {
  is_enabled_ = true;
  emit OptionChanged(OPT_ZOOM, settings_->GetOption(OPT_CURRENT_ZOOM));
}

void ExtraZoom::Stop() {
  is_enabled_ = false;
  emit OptionChanged(OPT_ZOOM, settings_->GetOption(OPT_PREVIOUS_ZOOM));
}

void ExtraZoom::Configure() {
  if (settings_dlg_) {
    settings_dlg_->activateWindow();
  } else {
    settings_dlg_ = new QInputDialog(parent_);
    settings_dlg_->setAttribute(Qt::WA_DeleteOnClose);

    settings_dlg_->setWindowTitle("Any zoom settings");
    settings_dlg_->setLabelText("zoom:");

    settings_dlg_->setInputMode(QInputDialog::IntInput);
    settings_dlg_->setIntRange(401, 1000000);
    settings_dlg_->setIntValue(settings_->GetOption(OPT_CURRENT_ZOOM).toReal() * 100);

    connect(settings_dlg_, SIGNAL(intValueChanged(int)), this, SLOT(TrackChange(int)));
    connect(settings_dlg_, SIGNAL(intValueSelected(int)), this, SLOT(TrackChange(int)));
    connect(settings_dlg_, SIGNAL(accepted()), settings_, SLOT(Save()));
    connect(settings_dlg_, SIGNAL(rejected()), this, SLOT(RevertSettings()));

    settings_dlg_->show();
  }
}

void ExtraZoom::TrackChange(int new_zoom) {
  settings_->SetOption(OPT_CURRENT_ZOOM, new_zoom / 100.);
  if (is_enabled_) emit OptionChanged(OPT_ZOOM, new_zoom / 100.);
}

void ExtraZoom::RevertSettings() {
  settings_->Load();
  if (is_enabled_) emit OptionChanged(OPT_ZOOM, settings_->GetOption(OPT_PREVIOUS_ZOOM));
}
