#ifndef DATE_SETTINGS_DIALOG_H
#define DATE_SETTINGS_DIALOG_H

#include <QDialog>
#include <QMap>
#include <QSettings>

namespace date {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

  void Init(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(const QString&, const QVariant&);

private slots:
  void on_int_type_button_toggled(bool checked);
  void on_str_type_button_toggled(bool checked);
  void on_int_type_box_currentIndexChanged(int index);
  void on_str_type_box_currentTextChanged(const QString& arg1);

private:
  Ui::SettingsDialog* ui;
  QMap<Qt::DateFormat, QString> date_formats_;
};

} // namespace date

#endif // DATE_SETTINGS_DIALOG_H
