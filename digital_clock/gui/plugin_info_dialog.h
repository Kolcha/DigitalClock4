#ifndef DIGITAL_CLOCK_GUI_PLUGIN_INFO_DIALOG_H
#define DIGITAL_CLOCK_GUI_PLUGIN_INFO_DIALOG_H

#include <QDialog>
#include "core/plugin_info.h"

namespace digital_clock {
namespace gui {

namespace Ui {
class PluginInfoDialog;
}

class PluginInfoDialog : public QDialog
{
  Q_OBJECT

public:
  PluginInfoDialog(QWidget* parent = 0);
  ~PluginInfoDialog();

public slots:
  void SetInfo(const TPluginInfo& info);

private:
  Ui::PluginInfoDialog* ui;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_PLUGIN_INFO_DIALOG_H
