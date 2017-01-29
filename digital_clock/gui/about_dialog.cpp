/*
    Digital Clock - beautiful customizable clock with plugins
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

#include "about_dialog.h"
#include "ui_about_dialog.h"

#include <QDate>

#include "core/build_defs.h"

namespace digital_clock {
namespace gui {

const char c_build_date[] = {
  BUILD_DAY_CH0, BUILD_DAY_CH1,
  '-',
  BUILD_MONTH_CH0, BUILD_MONTH_CH1,
  '-',
  BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
  '\0'
};

static QString compilerString()
{
#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU too
  QString isAppleString;
#if defined(__apple_build_version__) // Apple clang has other version numbers
  isAppleString = QLatin1String(" (Apple)");
#endif
  return QLatin1String("Clang " ) + QString::number(__clang_major__) + QLatin1Char('.')
         + QString::number(__clang_minor__) + isAppleString;
#elif defined(Q_CC_GNU)
  return QString("GCC %1.%2.%3").arg(__GNUC__).arg(__GNUC_MINOR__).arg(__GNUC_PATCHLEVEL__);
#elif defined(Q_CC_MSVC)
  if (_MSC_VER >= 1500) { // 1500: MSVC 2008, 1600: MSVC 2010, ...
    int ver = 2008 + 2 * ((_MSC_VER / 100) - 15);
    if (_MSC_VER >= 1800) ver -= 1;
    return QLatin1String("MSVC ") + QString::number(ver);
  }
#endif
  return QLatin1String("<unknown compiler>");
}

AboutDialog::AboutDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::AboutDialog)
{
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(":/clock/images/about.svg"));

  ui->logo_lbl->setPixmap(QApplication::windowIcon().pixmap(128));

  QString version = QCoreApplication::applicationVersion();
  QString build_type = version[version.length() - 1].isDigit() ? "stable" : "testing";
  ui->app_name_lbl->setText(QCoreApplication::applicationName());
  ui->app_ver_lbl->setText(tr("version: %1 (%2)").arg(version).arg(build_type));
  ui->build_qt_lbl->setText(QString("Qt %1 (%2, %3 bit)").arg(qVersion(), compilerString()).arg(QSysInfo::WordSize));
  QDate build_date = QDate::fromString(QLatin1String(c_build_date), "dd-MM-yyyy");
  ui->build_date_lbl->setText(tr("build date: %1").arg(build_date.toString(Qt::DefaultLocaleShortDate)));
  setFixedSize(sizeHint());
}

AboutDialog::~AboutDialog()
{
  delete ui;
}

} // namespace gui
} // namespace digital_clock
