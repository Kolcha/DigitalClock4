#include <functional>
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QSharedPointer>
#include <QMenu>
#include <QDesktopServices>
#include "core/clock_settings.h"
#include "core/skin_manager.h"
#include "core/plugin_manager.h"
#include "core/updater.h"
#include "gui/clock_widget.h"
#include "gui/clock_display.h"
#include "gui/tray_control.h"
#include "gui/settings_dialog.h"
#include "gui/about_dialog.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setApplicationDisplayName("Digital Clock");
  app.setApplicationName("Digital Clock");
  app.setApplicationVersion("4.4.2");
  app.setOrganizationName("Nick Korotysh");
  app.setOrganizationDomain("digitalclock4.sourceforge.net");
  app.setWindowIcon(QIcon(":/clock/images/clock.svg"));
  app.setQuitOnLastWindowClosed(false);
  app.setAttribute(Qt::AA_UseHighDpiPixmaps);

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
  // settings manager
  QSharedPointer<digital_clock::core::ClockSettings> settings(
        new digital_clock::core::ClockSettings());
  QObject::connect(settings.data(), SIGNAL(SettingsImported()),
                   settings.data(), SLOT(EmitSettings()));

  // skin manager
  QSharedPointer<digital_clock::core::SkinManager> skin_manager(
        new digital_clock::core::SkinManager());
  QList<QDir> default_skin_dirs;
  default_skin_dirs.append(QDir(":/clock/default_skins"));
#ifdef Q_OS_OSX
  default_skin_dirs.append(QDir(app.applicationDirPath() + "/../Resources/skins"));
#else
  default_skin_dirs.append(QDir(app.applicationDirPath() + "/skins"));
#endif
#ifdef Q_OS_LINUX
  default_skin_dirs.append(QDir("/usr/share/digital_clock/skins"));
  default_skin_dirs.append(QDir("/usr/local/share/digital_clock/skins"));
  default_skin_dirs.append(QDir(QDir::homePath() + "/.local/share/digital_clock/skins"));
#endif
  skin_manager->ResetSearchDirs(default_skin_dirs);
  skin_manager->ListSkins();
  skin_manager->SetFallbackSkin("Electronic (default)");

  // updater
  QSharedPointer<digital_clock::core::Updater> updater(new digital_clock::core::Updater());
  updater->SetAutoupdate(false);

  // plugin manager
  QSharedPointer<digital_clock::core::PluginManager> plugin_manager(
        new digital_clock::core::PluginManager());
  QList<QDir> default_plugin_dirs;
#ifdef Q_OS_OSX
  default_plugin_dirs.append(QDir(app.applicationDirPath() + "/../PlugIns"));
#else
  default_plugin_dirs.append(QDir(app.applicationDirPath() + "/plugins"));
#endif
#ifdef Q_OS_LINUX
  default_plugin_dirs.append(QDir("/usr/share/digital_clock/plugins"));
  default_plugin_dirs.append(QDir("/usr/local/share/digital_clock/plugins"));
  default_plugin_dirs.append(QDir(QDir::homePath() + "/.local/share/digital_clock/plugins"));
#endif
  plugin_manager->ResetSearchDirs(default_plugin_dirs);
  plugin_manager->ListAvailable();
  digital_clock::core::TPluginData plugin_data;
  plugin_data.settings = settings.data();

  // create gui components
  // main clock widget
  QSharedPointer<digital_clock::gui::ClockWidget> clock_widget(
        new digital_clock::gui::ClockWidget());
  plugin_data.window = clock_widget.data();
  clock_widget->setWindowFlags(Qt::FramelessWindowHint);
#ifdef Q_OS_OSX
  clock_widget->setWindowFlags(clock_widget->windowFlags() | Qt::NoDropShadowWindowHint);
#else
  clock_widget->setWindowFlags(clock_widget->windowFlags() | Qt::Tool);
#endif
  clock_widget->setAttribute(Qt::WA_TranslucentBackground);
  clock_widget->setContextMenuPolicy(Qt::CustomContextMenu);

  QObject::connect(skin_manager.data(), &digital_clock::core::SkinManager::SkinLoaded,
                   clock_widget.data(), &digital_clock::gui::ClockWidget::ApplySkin);
  QObject::connect(clock_widget.data(), &digital_clock::gui::ClockWidget::SeparatorsChanged,
                   skin_manager.data(), &digital_clock::core::SkinManager::SetSeparators);
  QObject::connect(clock_widget->GetDisplay(), &digital_clock::gui::ClockDisplay::ImageNeeded,
                   updater.data(), &digital_clock::core::Updater::TimeoutHandler);

  // tray icon
  digital_clock::gui::TrayControl* tray_control(
        new digital_clock::gui::TrayControl(clock_widget.data()));
  plugin_data.tray = tray_control->GetTrayIcon();
  // clock tray/context menu
  QObject::connect(
        clock_widget.data(), &digital_clock::gui::ClockWidget::customContextMenuRequested,
        [&] (const QPoint& p) {
          tray_control->GetTrayIcon()->contextMenu()->exec(clock_widget->mapToParent(p));
        }
  );
  // menu actions
  QObject::connect(tray_control, &digital_clock::gui::TrayControl::ShowSettingsDlg, [&] () {
    using digital_clock::gui::SettingsDialog;
    static QPointer<SettingsDialog> dialog;
    if (dialog) {
      dialog->activateWindow();
    } else {
      // create settings dialog and connect all need signals
      // (settings dialog will be deleted automatically)
      dialog = new SettingsDialog();
      QObject::connect(skin_manager.data(), SIGNAL(SearchFinished(QStringList)),
                       dialog.data(), SLOT(SetSkinList(QStringList)));
      skin_manager->ListSkins();
      QObject::connect(skin_manager.data(), &digital_clock::core::SkinManager::SkinInfoLoaded,
                       dialog.data(), &SettingsDialog::DisplaySkinInfo);
      QObject::connect(plugin_manager.data(),
                       SIGNAL(SearchFinished(QList<QPair<TPluginInfo,bool> >)),
                       dialog.data(), SLOT(SetPluginsList(QList<QPair<TPluginInfo,bool> >)));
      plugin_manager->ListAvailable();

      dialog->SetCurrentSettings(settings->GetSettings());
      dialog->DisplaySkinInfo(skin_manager->CurrentSkin()->GetInfo());
      settings->TrackChanges(true);
      dialog->show();
      std::function<void()> reload_plugins = [=] () {
        plugin_manager->UnloadPlugins();
        plugin_manager->LoadPlugins(settings->GetOption(OPT_PLUGINS).toStringList());
      };
      using digital_clock::core::ClockSettings;
      // connect main logic signals: change/save/discard settings
      QObject::connect(dialog.data(), SIGNAL(OptionChanged(Option,QVariant)),
                       settings.data(), SLOT(SetOption(Option,QVariant)));
      QObject::connect(dialog.data(), &SettingsDialog::PluginStateChanged,
                       plugin_manager.data(), &digital_clock::core::PluginManager::EnablePlugin);
      QObject::connect(dialog.data(), SIGNAL(PluginConfigureRequest(QString)),
                       plugin_manager.data(), SLOT(ConfigurePlugin(QString)));
      QObject::connect(dialog.data(), &SettingsDialog::ResetSettings,
                       settings.data(), &ClockSettings::LoadDefaults);
      QObject::connect(dialog.data(), SIGNAL(accepted()), settings.data(), SLOT(Save()));
      QObject::connect(dialog.data(), SIGNAL(rejected()), settings.data(), SLOT(Load()));
      QObject::connect(dialog.data(), &SettingsDialog::rejected, reload_plugins);
      // export/import settings
      QObject::connect(dialog.data(), &SettingsDialog::ExportSettings, [&] (const QString& fn) {
        QMap<QString, QSettings::SettingsMap> all_settings;
        plugin_manager->ExportPluginsSettings(&all_settings);
        QSettings::SettingsMap& clock_settings = all_settings["core_settings"];
        settings->ExportSettings(&clock_settings);

        QFile file(fn);
        if (!file.open(QIODevice::WriteOnly)) return;
        QDataStream stream(&file);
        stream << all_settings;
        file.close();
      });

      QObject::connect(dialog.data(), &SettingsDialog::ImportSettings, [&] (const QString& fn) {
        QFile file(fn);
        if (!file.open(QIODevice::ReadOnly)) return;
        QDataStream stream(&file);
        QMap<QString, QSettings::SettingsMap> all_settings;
        stream >> all_settings;
        file.close();

        plugin_manager->ImportPluginsSettings(all_settings);
        settings->ImportSettings(all_settings["core_settings"]);
      });

      QObject::connect(settings.data(), &ClockSettings::SettingsImported,
                       [&] () { dialog->SetCurrentSettings(settings->GetSettings()); });
      QObject::connect(settings.data(), &ClockSettings::SettingsImported, reload_plugins);

      QObject::connect(dialog.data(), &SettingsDialog::CheckForUpdates,
                       [=] () { updater->CheckForUpdates(true); });

      QObject::connect(dialog.data(), &SettingsDialog::destroyed, [=] () {
        clock_widget->PreviewMode(false);
        settings->TrackChanges(false);
      });
      clock_widget->PreviewMode(true);
    }
  });
  QObject::connect(tray_control, &digital_clock::gui::TrayControl::ShowAboutDlg, [&] () {
    // don't need to delete AboutDialog, it will destroyed on self close
    static QPointer<digital_clock::gui::AboutDialog> dialog;
    if (dialog) {
      dialog->activateWindow();
    } else {
      dialog = new digital_clock::gui::AboutDialog(clock_widget.data());
      dialog->show();
    }
  });
  QObject::connect(tray_control, &digital_clock::gui::TrayControl::CheckForUpdates,
                   [=] () { updater->CheckForUpdates(true); });
  QObject::connect(tray_control, &digital_clock::gui::TrayControl::AppExit,
                   [&] () { plugin_manager->UnloadPlugins(); app.quit(); });

  // updater messages
  QObject::connect(updater.data(), &digital_clock::core::Updater::ErrorMessage,
                   [&] (const QString& msg) {
    QObject::disconnect(tray_control->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control->GetTrayIcon()->showMessage(
          QObject::tr("%1 Update").arg(app.applicationName()),
          QObject::tr("Update error. %1").arg(msg),
          QSystemTrayIcon::Critical);
  });

  QObject::connect(updater.data(), &digital_clock::core::Updater::UpToDate, [&] () {
    QObject::disconnect(tray_control->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control->GetTrayIcon()->showMessage(
          QObject::tr("%1 Update").arg(app.applicationName()),
          QObject::tr("You already have latest version (%1).").arg(app.applicationVersion()),
          QSystemTrayIcon::Information);
  });

  QObject::connect(updater.data(), &digital_clock::core::Updater::NewVersion,
                   [&] (const QString& version, const QString& link) {
    QObject::disconnect(tray_control->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control->GetTrayIcon()->showMessage(
          QObject::tr("%1 Update").arg(app.applicationName()),
          QObject::tr("Update available (%1). Click this message to download.").arg(version),
          QSystemTrayIcon::Warning);
    QObject::connect(tray_control->GetTrayIcon(), &QSystemTrayIcon::messageClicked,
                     [=] () { QDesktopServices::openUrl(link); });
  });

  // skin_manager messages
  QObject::connect(skin_manager.data(), &digital_clock::core::SkinManager::ErrorMessage,
                   [&] (const QString& msg) {
    QObject::disconnect(tray_control->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control->GetTrayIcon()->showMessage(
          QObject::tr("%1 Error").arg(app.applicationName()), msg, QSystemTrayIcon::Warning);
  });

  // settings load
  QObject::connect(settings.data(), &digital_clock::core::ClockSettings::OptionChanged,
                   clock_widget.data(), &digital_clock::gui::ClockWidget::ApplyOption);
  QObject::connect(settings.data(), &digital_clock::core::ClockSettings::OptionChanged,
                   [=] (Option option, const QVariant& value) {
    switch (option) {
      case OPT_SKIN_NAME:
        skin_manager->LoadSkin(value.toString());
        break;

      case OPT_FONT:
        skin_manager->SetFont(value.value<QFont>());
        break;

      case OPT_USE_AUTOUPDATE:
        updater->SetAutoupdate(value.toBool());
        break;

      case OPT_UPDATE_PERIOD:
        updater->SetUpdatePeriod(value.value<qint64>());
        break;

      case OPT_CHECK_FOR_BETA:
        updater->SetCheckForBeta(value.toBool());
        break;

      default:
        break;
    }
  });

  settings->TrackChanges(true);
  settings->Load();
  clock_widget->show();
  clock_widget->setWindowOpacity(settings->GetOption(OPT_OPACITY).toReal());
  settings->TrackChanges(false);
  plugin_manager->SetInitData(plugin_data);
  plugin_manager->LoadPlugins(settings->GetOption(OPT_PLUGINS).toStringList());

  return app.exec();
}
