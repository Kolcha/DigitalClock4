#include "var_translucency.h"

void VarTranslucency::Init(const QMap<Options, QVariant>& current_settings, QWidget*) {
  old_opacity_ = current_settings[OPT_OPACITY].toReal();
  cur_opacity_ = 1.0;
  op_stap_ = -0.01;
}

void VarTranslucency::Start() {
  emit OptionChanged(OPT_OPACITY, cur_opacity_);
}

void VarTranslucency::Stop() {
  emit OptionChanged(OPT_OPACITY, old_opacity_);
}

void VarTranslucency::TimeUpdateListener(const QString&) {
  if (qAbs(cur_opacity_ - 1.0) < qAbs(op_stap_ / 2)) op_stap_ = -0.01;
  if (qAbs(cur_opacity_ - 0.1) < qAbs(op_stap_ / 2)) op_stap_ = +0.01;
  cur_opacity_ += op_stap_;
  emit OptionChanged(OPT_OPACITY, cur_opacity_);
}
