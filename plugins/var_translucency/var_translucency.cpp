#include "var_translucency.h"

namespace var_translucency {

VarTranslucency::VarTranslucency() {
  InitTranslator(QLatin1String(":/var_translucency/var_translucency_"));
  info_.display_name = tr("Variable translucency");
  info_.description = tr("Changes clock opacity level during time.");
}

void VarTranslucency::Init(const QMap<Option, QVariant>& current_settings) {
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

void VarTranslucency::TimeUpdateListener() {
  if (qAbs(cur_opacity_ - 1.0) < qAbs(op_stap_ / 2)) op_stap_ = -0.01;
  if (qAbs(cur_opacity_ - 0.1) < qAbs(op_stap_ / 2)) op_stap_ = +0.01;
  cur_opacity_ += op_stap_;
  emit OptionChanged(OPT_OPACITY, cur_opacity_);
}

} // namespace var_translucency
