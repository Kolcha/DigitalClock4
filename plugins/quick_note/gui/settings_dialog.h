#ifndef IP_ADDRESS_SETTINGS_DIALOG_H
#define IP_ADDRESS_SETTINGS_DIALOG_H

#include <QDialog>
#include <QSettings>

namespace quick_note {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void Init(const QSettings::SettingsMap& settings);

signals:
    void OptionChanged(const QString& key, const QVariant& new_value);

private slots:
    void on_note_edit_textEdited(const QString& arg1);
    void on_edit_btn_enabled_clicked(bool checked);

private:
    Ui::SettingsDialog *ui;
};


} // namespace quick_note

#endif // IP_ADDRESS_SETTINGS_DIALOG_H
