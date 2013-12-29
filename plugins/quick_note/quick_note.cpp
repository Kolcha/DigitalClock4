#include <QLabel>
#include <QGridLayout>
#include <QInputDialog>
#include "plugin_settings.h"
#include "quick_note_settings.h"
#include "quick_note.h"

namespace quick_note {

QuickNote::QuickNote() {
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);

  InitTranslator(QLatin1String(":/quick_note/quick_note_"));
  info_.display_name = tr("Quick note");
  info_.description = tr("Allows to display any short message under clock.");
}

void QuickNote::Init(QWidget* main_wnd) {
  main_layout_ = qobject_cast<QGridLayout*>(main_wnd->layout());
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->Load();
}

void QuickNote::Start() {
  msg_label_ = new QLabel("test message");
  main_layout_->addWidget(msg_label_, main_layout_->rowCount(), 0, 1, main_layout_->columnCount());
}

void QuickNote::Stop() {
  main_layout_->removeWidget(msg_label_);
  qobject_cast<QWidget*>(main_layout_->parent())->adjustSize();
  delete msg_label_;
}

void QuickNote::Configure() {
  QInputDialog* settings_dlg = new QInputDialog();
  settings_dlg->setAttribute(Qt::WA_DeleteOnClose);
  settings_dlg->setModal(true);

  settings_dlg->setWindowTitle(tr("Quick note settings"));
  settings_dlg->setLabelText(tr("message:"));

  settings_dlg->setInputMode(QInputDialog::TextInput);
  settings_dlg->setTextValue(settings_->GetOption(OPT_QUICK_NOTE_MSG).toString());

  connect(settings_dlg, SIGNAL(textValueSelected(QString)), this, SLOT(ApplyString(QString)));
  connect(settings_dlg, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(settings_dlg, &QInputDialog::rejected, [=] () {
    settings_->Load();
    this->ApplyString(settings_->GetOption(OPT_QUICK_NOTE_MSG).toString());
  });

  settings_dlg->show();
}

void QuickNote::SettingsListener(Options option, const QVariant& new_value) {
}

void QuickNote::ApplyString(const QString& str) {
  settings_->SetOption(OPT_QUICK_NOTE_MSG, str);
}

} // namespace quick_note
