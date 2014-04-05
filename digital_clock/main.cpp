#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QSharedPointer>
#include <QMenu>
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

  // create gui components
  // main clock widget
  QSharedPointer<digital_clock::gui::ClockWidget> clock_widget(
        new digital_clock::gui::ClockWidget());
  clock_widget->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
  clock_widget->setAttribute(Qt::WA_TranslucentBackground);
  clock_widget->setContextMenuPolicy(Qt::CustomContextMenu);

  // tray icon
  digital_clock::gui::TrayControl* tray_control(
        new digital_clock::gui::TrayControl(clock_widget.data()));
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
  QObject::connect(tray_control, &digital_clock::gui::TrayControl::CheckForUpdates, [] () {});
  QObject::connect(
        tray_control, &digital_clock::gui::TrayControl::AppExit, &app, &QApplication::quit);


  clock_widget->show();

  return app.exec();
}
