#ifndef DATE_SETTINGS_DIALOG_H
#define DATE_SETTINGS_DIALOG_H

#include <QDialog>
#include <QMap>

namespace date {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

public slots:
  void SettingsListener(const QString& key, const QVariant& value);

signals:
  void OptionChanged(const QString&, const QVariant&);

private slots:
  void on_int_type_button_toggled(bool checked);
  void on_str_type_button_toggled(bool checked);
  void on_int_type_box_currentIndexChanged(int index);
  void on_str_type_box_currentTextChanged(const QString& arg1);
  void on_clock_font_button_toggled(bool checked);
  void on_choose_font_button_clicked();
  void on_not_zoom_clicked();
  void on_font_autosize_clicked();
  void on_clock_zoom_clicked();

private:
  Ui::SettingsDialog* ui;
  QMap<Qt::DateFormat, QString> date_formats_;
  QFont last_font_;
};

} // namespace date
#endif // DATE_SETTINGS_DIALOG_H
