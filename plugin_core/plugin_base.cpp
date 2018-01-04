/*
    Digital Clock: clock common library
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

#include "plugin_base.h"

#include <QTranslator>
#include <QLocale>
#include <QCoreApplication>
#include <QIcon>

#include "plugin_settings.h"

PluginBase::PluginBase() : settings_(nullptr), translator_(nullptr)
{
}

PluginBase::~PluginBase()
{
  if (translator_) QCoreApplication::removeTranslator(translator_);
}

const TPluginGUIInfo& PluginBase::GetInfo() const
{
  return info_;
}

void PluginBase::InitSettings(SettingsStorage* backend, const QString& name)
{
  Q_ASSERT(backend);
  settings_ = new PluginSettings(backend, name, this);
}

void PluginBase::InitTranslator(const QLatin1String& prefix)
{
  translator_ = new QTranslator(this);
  QStringList ui_languages = QLocale::system().uiLanguages();
  foreach (QString locale, ui_languages) {
    locale = QLocale(locale).name();
    if (locale == QLatin1String("C") ||               // overrideLanguage == "English"
        locale.startsWith(QLatin1String("en")))       // "English" is built-in
      break;                                          // use built-in

    if (locale.contains("ua", Qt::CaseInsensitive))   // Ukrainian,
      locale = QLatin1String("ru");                   // use Russian

    if (translator_->load(prefix + locale)) {
      QCoreApplication::installTranslator(translator_);
      break;
    }
  }
}

void PluginBase::InitIcon(const QString& file_path)
{
  QIcon ico(file_path);
  info_.icon = ico.pixmap(48);
}
