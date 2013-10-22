#include <QCheckBox>
#include <QToolButton>
#include <QHBoxLayout>
#include "plugin_list_item.h"

PluginListItem::PluginListItem(QWidget* parent)
  : QWidget(parent) {
  Init();
}

PluginListItem::PluginListItem(const QString& text, bool enabled, bool congurable, QWidget* parent)
  : QWidget(parent) {
  Init();
  SetText(text);
  SetChecked(enabled);
  SetConfigurable(congurable);
}

QString PluginListItem::GetText() const {
  return chk_->text();
}

bool PluginListItem::IsChecked() const {
  return chk_->isChecked();
}

bool PluginListItem::IsConfigurable() const {
  return btn_->isEnabled();
}

void PluginListItem::SetText(const QString& text) {
  chk_->setText(text);
}

void PluginListItem::SetChecked(bool checked) {
  chk_->setChecked(checked);
}

void PluginListItem::SetConfigurable(bool configurable) {
  btn_->setEnabled(configurable);
}

void PluginListItem::CheckBoxToggled(bool checked) {
  emit StateChanged(chk_->text(), checked);
}

void PluginListItem::BtnClicked() {
  emit ConfigureRequest(chk_->text());
}

void PluginListItem::Init() {
  chk_ = new QCheckBox(this);
  btn_ = new QToolButton(this);
  btn_->setIcon(QIcon(":/images/settings.svg"));
  btn_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  QHBoxLayout* hlt = new QHBoxLayout(this);
  hlt->addWidget(chk_);
  hlt->addWidget(btn_);
  hlt->setMargin(2);
  setLayout(hlt);

  connect(chk_, SIGNAL(toggled(bool)), this, SLOT(CheckBoxToggled(bool)));
  connect(btn_, SIGNAL(clicked()), this, SLOT(BtnClicked()));
}
