#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QSharedPointer>
#include <QMenu>
#include <QDesktopServices>
#include "core/skin_manager.h"
#include "core/plugin_manager.h"
#include "core/updater.h"
#include "gui/clock_widget.h"
#include "gui/tray_control.h"
#include "gui/settings_dialog.h"
#include "gui/about_dialog.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setApplicationDisplayName("Digital Clock");
  app.setApplicationName("Digital Clock");
  app.setApplicationVersion("4.2.7");
  app.setOrganizationName("Nick Korotysh");
  app.setWindowIcon(QIcon(":/clock/images/clock.svg"));
  app.setQuitOnLastWindowClosed(false);

  // install app translators
  QTranslator app_translator;
  QTranslator qt_translator;
  QStringList ui_languages = QLocale::system().uiLanguages();
  foreach (QString locale, ui_languages) {
    locale = QLocale(locale).name();
    if (app_translator.load(QLatin1String(":/clock/languages/digital_clock_") + locale)) {
      if (qt_translator.load(QLatin1String("qt_") + locale,
                             QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        app.installTranslator(&app_translator);
        app.installTranslator(&qt_translator);
        break;
      }
      app_translator.load(QString()); // unload()
    } else if (locale == QLatin1String("C") /* overrideLanguage == "English" */) {
      // use built-in
      break;
    } else if (locale.startsWith(QLatin1String("en")) /* "English" is built-in */) {
      // use built-in
      break;
    }
  }

  // create core components
  // skin manager
  QSharedPointer<digital_clock::core::SkinManager> skin_manager(
        new digital_clock::core::SkinManager());
  skin_manager->AddSkinDir(QDir(":/clock/default_skin"));
  skin_manager->AddSkinDir(QDir(app.applicationDirPath() + "/skins"));
  skin_manager->ListSkins();

  // updater
  QSharedPointer<digital_clock::core::Updater> updater(
        new digital_clock::core::Updater());

  // plugin manager
  QSharedPointer<digital_clock::core::PluginManager> plugin_manager(
        new digital_clock::core::PluginManager());
  plugin_manager->AddPluginsDir(QDir(app.applicationDirPath() + "/plugins"));
  plugin_manager->ListAvailable();
  digital_clock::core::TPluginData plugin_data;
//  plugin_data.settings = settings_;

  // create gui components
  // main clock widget
  QSharedPointer<digital_clock::gui::ClockWidget> clock_widget(
        new digital_clock::gui::ClockWidget());
  plugin_data.window = clock_widget.data();
  clock_widget->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
  clock_widget->setAttribute(Qt::WA_TranslucentBackground);
  clock_widget->setContextMenuPolicy(Qt::CustomContextMenu);

  QObject::connect(skin_manager.data(), &digital_clock::core::SkinManager::SkinLoaded,
                   clock_widget.data(), &digital_clock::gui::ClockWidget::ApplySkin);
  QObject::connect(clock_widget.data(), &digital_clock::gui::ClockWidget::SeparatorsChanged,
                   skin_manager.data(), &digital_clock::core::SkinManager::SetSeparators);

  // tray icon
  digital_clock::gui::TrayControl* tray_control(
        new digital_clock::gui::TrayControl(clock_widget.data()));
  plugin_data.tray = tray_control->GetTrayIcon();
  // clock tray/context menu
  QObject::connect(
        clock_widget.data(), &digital_clock::gui::ClockWidget::customContextMenuRequested,
        [=] (const QPoint& p) { tray_control->GetTrayIcon()->contextMenu()->exec(p); }
  );
  // menu actions
  QObject::connect(tray_control, &digital_clock::gui::TrayControl::ShowSettingsDlg, [=] () {
    using digital_clock::gui::SettingsDialog;
    SettingsDialog* dialog = new SettingsDialog(clock_widget.data());
    dialog->show();
  });
  QObject::connect(tray_control, &digital_clock::gui::TrayControl::ShowAboutDlg, [=] () {
    using digital_clock::gui::AboutDialog;
    AboutDialog* dialog = new AboutDialog(clock_widget.data());
    dialog->show();
  });
  QObject::connect(tray_control, &digital_clock::gui::TrayControl::CheckForUpdates,
                   [=] () { updater->CheckForUpdates(true); });
  QObject::connect(tray_control, &digital_clock::gui::TrayControl::AppExit,
                   plugin_manager.data(), &digital_clock::core::PluginManager::UnloadPlugins);
  QObject::connect(
        tray_control, &digital_clock::gui::TrayControl::AppExit, &app, &QApplication::quit);

  // updater messages
  connect(updater.data(), &digital_clock::core::Updater::ErrorMessage, [=] (const QString& msg) {
    QObject::disconnect(tray_control->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control->GetTrayIcon()->showMessage(
          updater->tr("%1 Update").arg(QCoreApplication::applicationName()),
          updater->tr("Update error. %1").arg(msg),
          QSystemTrayIcon::Critical);
  });

  connect(updater.data(), &digital_clock::core::Updater::UpToDate, [=] () {
    QObject::disconnect(tray_control->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control->GetTrayIcon()->showMessage(
          updater->tr("%1 Update").arg(QCoreApplication::applicationName()),
          updater->tr("You already have latest version (%1).").arg(
            QCoreApplication::applicationVersion()),
          QSystemTrayIcon::Information);
  });

  connect(updater, &digital_clock::core::Updater::NewVersion,
          [=] (const QString& version, const QString& link) {
    QObject::disconnect(tray_control->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control->GetTrayIcon()->showMessage(
          updater->tr("%1 Update").arg(QCoreApplication::applicationName()),
          updater->tr("Update available (%1). Click this message to download.").arg(version),
          QSystemTrayIcon::Warning);
    QObject::connect(tray_control->GetTrayIcon(), &QSystemTrayIcon::messageClicked,
                     [=] () { QDesktopServices::openUrl(link); });
  });


  plugin_manager->SetInitData(plugin_data);
  clock_widget->show();

  return app.exec();
}
