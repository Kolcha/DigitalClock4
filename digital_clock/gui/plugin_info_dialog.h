#ifndef PLUGIN_INFO_DIALOG_H
#define PLUGIN_INFO_DIALOG_H

#include "centered_dialog.h"
#include "../core/plugin_info.h"

namespace Ui {
class PluginInfoDialog;
}

class PluginInfoDialog : public CenteredDialog {
  Q_OBJECT

public:
  PluginInfoDialog(QWidget* parent = 0);
  ~PluginInfoDialog();

public slots:
  void SetInfo(const TPluginInfo& info);

private:
  Ui::PluginInfoDialog* ui;
};

#endif // PLUGIN_INFO_DIALOG_H
