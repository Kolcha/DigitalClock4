#include "zoom_dialog.h"
#include "ui_zoom_dialog.h"

namespace any_zoom {

ZoomDialog::ZoomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ZoomDialog)
{
    ui->setupUi(this);
    connect(ui->x_zoom_slider, &QSlider::valueChanged, this, &ZoomDialog::onZoomValueChanged);
    connect(ui->y_zoom_slider, &QSlider::valueChanged, this, &ZoomDialog::onZoomValueChanged);
}

ZoomDialog::~ZoomDialog()
{
    delete ui;
}

void ZoomDialog::setZoom(const skin_draw::Zoom &zoom)
{
    if (m_zoom == zoom) return;

    ui->x_zoom_edit->setValue(zoom.zoom_x * 100);
    ui->y_zoom_edit->setValue(zoom.zoom_y * 100);

    m_zoom = zoom;
    emit zoomChanged(m_zoom);
}

void ZoomDialog::onZoomValueChanged()
{
    emit zoomChanged(::skin_draw::Zoom(ui->x_zoom_edit->value() / 100., ui->y_zoom_edit->value() / 100.));
}

} // namespace any_zoom
