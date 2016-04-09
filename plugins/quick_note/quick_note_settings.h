#ifndef QUICK_NOTE_SETTINGS_H
#define QUICK_NOTE_SETTINGS_H

#include <QSettings>

namespace quick_note {

// quick note plugin settings keys
#define OPT_QUICK_NOTE_MSG      "plugins/quick_note/message"
#define OPT_SHOW_EDIT_BTN       "plugins/quick_note/show_edit_btn"

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_QUICK_NOTE_MSG, QString("quick note, click to edit it!"));
  defaults->insert(OPT_SHOW_EDIT_BTN, false);
}

} // namespace quick_note

#endif // QUICK_NOTE_SETTINGS_H
