#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include "plugin_list_item.h"

PluginListItem::PluginListItem(const QString& text, bool enabled, bool congurable, QWidget* parent)
  : QWidget(parent) {
  QCheckBox* chk = new QCheckBox(this);
  QPushButton* btn = new QPushButton("Config", this);
  btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  QHBoxLayout* hlt = new QHBoxLayout(this);
  hlt->addWidget(chk);
  hlt->addWidget(btn);
  setLayout(hlt);
  connect(chk, SIGNAL(toggled(bool)), this, SIGNAL(StateChanged(bool)));
  connect(btn, SIGNAL(clicked()), this, SIGNAL(ConfigureRequest()));
}
