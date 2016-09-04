#include "base_settings_widget.h"
#include "ui_base_settings_widget.h"

#include <QFontDialog>

namespace plugin {

BaseSettingsWidget::BaseSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseSettingsWidget)
{
    ui->setupUi(this);
    layout()->setMargin(0);
}

BaseSettingsWidget::~BaseSettingsWidget()
{
    delete ui;
}

void BaseSettingsWidget::InitWidgets(const QMap<WidgetPluginOption, QVariant>& cur_values)
{
    for (auto iter = cur_values.constBegin(); iter != cur_values.constEnd(); ++iter) {
        switch (iter.key()) {
        case OPT_USE_CLOCK_FONT:
            ui->clock_font_button->setChecked(iter.value().toBool());
            ui->custom_font_button->setChecked(!iter.value().toBool());
            break;

        case OPT_CUSTOM_FONT:
            last_font_ = iter.value().value<QFont>();
            break;

        case OPT_ZOOM_MODE:
            ZoomMode mode = static_cast<ZoomMode>(iter.value().toInt());
            ui->not_zoom->setChecked(mode == ZoomMode::ZM_NOT_ZOOM);
            ui->font_autosize->setChecked(mode == ZoomMode::ZM_AUTOSIZE);
            ui->clock_zoom->setChecked(mode == ZoomMode::ZM_CLOCK_ZOOM);
            break;
        }
    }
}

void BaseSettingsWidget::on_clock_font_button_toggled(bool checked)
{
    emit OptionChanged(OPT_USE_CLOCK_FONT, checked);
}

void BaseSettingsWidget::on_choose_font_button_clicked()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, last_font_, this);
    if (ok) {
      emit OptionChanged(OPT_CUSTOM_FONT, font);
      last_font_ = font;
    }
}

void BaseSettingsWidget::on_not_zoom_clicked()
{
    emit OptionChanged(OPT_ZOOM_MODE, static_cast<int>(ZoomMode::ZM_NOT_ZOOM));
}

void BaseSettingsWidget::on_font_autosize_clicked()
{
    emit OptionChanged(OPT_ZOOM_MODE, static_cast<int>(ZoomMode::ZM_AUTOSIZE));
}

void BaseSettingsWidget::on_clock_zoom_clicked()
{
    emit OptionChanged(OPT_ZOOM_MODE, static_cast<int>(ZoomMode::ZM_CLOCK_ZOOM));
}

} // namespace plugin
