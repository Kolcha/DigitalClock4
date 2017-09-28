#ifndef ANY_ZOOM_ZOOM_DIALOG_H
#define ANY_ZOOM_ZOOM_DIALOG_H

#include <QDialog>

#include "zoom_type.h"

namespace any_zoom {

namespace Ui {
class ZoomDialog;
}

class ZoomDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(::skin_draw::Zoom zoom READ zoom WRITE setZoom NOTIFY zoomChanged)

public:
    explicit ZoomDialog(QWidget *parent = 0);
    ~ZoomDialog();

    ::skin_draw::Zoom zoom() const Q_DECL_NOEXCEPT { return m_zoom; }

public slots:
    void setZoom(const ::skin_draw::Zoom& zoom);

signals:
    void zoomChanged(const ::skin_draw::Zoom& zoom);

private slots:
    void onZoomValueChanged();

private:
    Ui::ZoomDialog *ui;
    ::skin_draw::Zoom m_zoom;
};

} // namespace any_zoom

#endif // ANY_ZOOM_ZOOM_DIALOG_H
