/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "core/build_date.h"

namespace digital_clock {
namespace gui {

static QString compilerString()
{
#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU too
  QString isAppleString;
#if defined(__apple_build_version__) // Apple clang has other version numbers
  isAppleString = QLatin1String(" (Apple)");
#endif
  return QString("Clang %1.%2").arg(__clang_major__).arg(__clang_minor__) + isAppleString;
#elif defined(Q_CC_GNU)
  return QString("GCC %1.%2.%3").arg(__GNUC__).arg(__GNUC_MINOR__).arg(__GNUC_PATCHLEVEL__);
#elif defined(Q_CC_MSVC)
  return QString("MSVC %1").arg(2015 + 2 * (_MSC_VER / 10 - 190));
#else
  return QLatin1String("<unknown compiler>");
#endif
}

AboutDialog::AboutDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::AboutDialog),
  click_counter_(0)
{
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);

  ui->logo_lbl->setPixmap(QApplication::windowIcon().pixmap(logoSize()));

  QString version = QCoreApplication::applicationVersion();
  QString build_type = version[version.length() - 1].isDigit() ? "stable" : "testing";
  ui->app_name_lbl->setText(QCoreApplication::applicationName());
  ui->app_ver_lbl->setText(tr("version: %1 (%2)").arg(version).arg(build_type));
  ui->build_qt_lbl->setText(QString("Qt %1 (%2, %3 bit)").arg(qVersion(), compilerString()).arg(QSysInfo::WordSize));
  QDate build_date = core::build_date();
  ui->build_date_lbl->setText(tr("build date: %1").arg(build_date.toString(Qt::DefaultLocaleShortDate)));
  resize(sizeHint());
}

AboutDialog::~AboutDialog()
{
  delete ui;
}

void AboutDialog::on_logo_lbl_clicked()
{
  switch (++click_counter_) {
    case 1:
      ui->logo_lbl->setPixmap(QIcon(":/clock/about/about-1.png").pixmap(logoSize()));
      break;

    case 2:
      ui->logo_lbl->setPixmap(QIcon(":/clock/about/about-2.png").pixmap(logoSize()));
      break;

    default:
      ui->logo_lbl->setPixmap(QApplication::windowIcon().pixmap(logoSize()));
      click_counter_ = 0;
      break;
  }
}

int AboutDialog::logoSize() const Q_DECL_NOEXCEPT
{
  Q_DECL_CONSTEXPR const int logo_size = 128;
#ifdef Q_OS_LINUX
  return qRound(this->logicalDpiY() / 96.0 * logo_size);
#else
  return logo_size;
#endif
}

} // namespace gui
} // namespace digital_clock
