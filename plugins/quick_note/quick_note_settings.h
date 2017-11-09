/*
    Digital Clock: quick note plugin
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef QUICK_NOTE_SETTINGS_H
#define QUICK_NOTE_SETTINGS_H

#include <QSettings>

namespace quick_note {

// quick note plugin settings keys
const char* const OPT_QUICK_NOTE_MSG = "message";

inline void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_QUICK_NOTE_MSG, QString("quick note, click to edit it!"));
}

} // namespace quick_note

#endif // QUICK_NOTE_SETTINGS_H
