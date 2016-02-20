#include "plugin_base.h"

#include <QTranslator>
#include <QLocale>
#include <QApplication>
#include <QIcon>

#include "plugin_settings.h"

PluginBase::PluginBase() : translator_(0) {
}

PluginBase::~PluginBase() {
  if (translator_) QApplication::removeTranslator(translator_);
}

const TPluginGUIInfo& PluginBase::GetInfo() const {
  return info_;
}

void PluginBase::InitSettings(SettingsStorage* backend)
{
  Q_ASSERT(backend);
  config_ = new PluginSettings(backend, this);
}

void PluginBase::InitTranslator(const QLatin1String& prefix) {
  translator_ = new QTranslator(this);
  QStringList ui_languages = QLocale::system().uiLanguages();
  foreach (QString locale, ui_languages) {
    locale = QLocale(locale).name();
    if (translator_->load(prefix + locale)) {
      QApplication::installTranslator(translator_);
      break;
    } else if (locale == QLatin1String("C") /* overrideLanguage == "English" */) {
      // use built-in
      break;
    } else if (locale.startsWith(QLatin1String("en")) /* "English" is built-in */) {
      // use built-in
      break;
    }
  }
}

void PluginBase::InitIcon(const QString& file_path) {
  QIcon ico(file_path);
  info_.icon = ico.pixmap(48);
}
