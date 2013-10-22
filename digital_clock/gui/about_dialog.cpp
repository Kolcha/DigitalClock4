#include "about_dialog.h"
#include "ui_about_dialog.h"

static QString compilerString() {
#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU too
    QString isAppleString;
#if defined(__apple_build_version__) // Apple clang has other version numbers
    isAppleString = QLatin1String(" (Apple)");
#endif
    return QLatin1String("Clang " ) + QString::number(__clang_major__) + QLatin1Char('.')
            + QString::number(__clang_minor__) + isAppleString;
#elif defined(Q_CC_GNU)
    return QLatin1String("GCC " ) + QLatin1String(__VERSION__);
#elif defined(Q_CC_MSVC)
    if (_MSC_VER >= 1500) // 1500: MSVC 2008, 1600: MSVC 2010, ...
        return QLatin1String("MSVC ") + QString::number(2008 + 2 * ((_MSC_VER / 100) - 15));
#endif
    return QLatin1String("<unknown compiler>");
}

AboutDialog::AboutDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::AboutDialog) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(":/images/about.svg"));

  ui->name_value->setText(QCoreApplication::applicationName());
  ui->version_value->setText(tr("version: ") + QCoreApplication::applicationVersion());
  ui->rights_value->setText("© 2013 " + QCoreApplication::organizationName());
  ui->qt_value->setText(QString("Qt %1 (%2, %3 bit)").arg(qVersion(), compilerString(),
                                                          QString::number(QSysInfo::WordSize)));
  setFixedSize(sizeHint());
}

AboutDialog::~AboutDialog() {
  delete ui;
}
