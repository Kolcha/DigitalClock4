#include "clock_settings.h"

// skin settings
#define KEY_SKIN_NAME           "skin/name"
#define KEY_ZOOM                "skin/zoom"
#define KEY_COLOR               "skin/color"
#define KEY_TEXTURE             "skin/texture"
#define KEY_TEXTURE_PER_ELEMENT "skin/texture_per_element"
// clock settings
#define KEY_OPACITY             "clock/opacity"
#define KEY_STAY_ON_TOP         "clock/stay_on_top"
#define KEY_TRANSP_FOR_INPUT    "clock/transp_for_input"
#define KEY_SEPARATOR_FLASH     "clock/separator_flash"

ClockSettings::ClockSettings(QObject *parent)
  : QObject(parent) {
}

void ClockSettings::Load() {
  values_[KEY_SKIN_NAME] = settings_.value(KEY_SKIN_NAME, "default");
  values_[KEY_ZOOM] = settings_.value(KEY_ZOOM, "1.0");
  values_[KEY_COLOR] = settings_.value(KEY_COLOR);
  values_[KEY_TEXTURE] = settings_.value(KEY_TEXTURE);
  values_[KEY_TEXTURE_PER_ELEMENT] = settings_.value(KEY_TEXTURE_PER_ELEMENT);

  values_[KEY_OPACITY] = settings_.value(KEY_OPACITY);
  values_[KEY_STAY_ON_TOP] = settings_.value(KEY_STAY_ON_TOP);
  values_[KEY_TRANSP_FOR_INPUT] = settings_.value(KEY_TRANSP_FOR_INPUT);
  values_[KEY_SEPARATOR_FLASH] = settings_.value(KEY_SEPARATOR_FLASH);

  EmitSignals();
}

void ClockSettings::Save() {
  settings_.setValue(KEY_SKIN_NAME, values_[KEY_SKIN_NAME]);
  settings_.setValue(KEY_ZOOM, values_[KEY_ZOOM]);
  settings_.setValue(KEY_COLOR, values_[KEY_COLOR]);
  settings_.setValue(KEY_TEXTURE, values_[KEY_TEXTURE]);
  settings_.setValue(KEY_TEXTURE_PER_ELEMENT, values_[KEY_TEXTURE_PER_ELEMENT]);

  settings_.setValue(KEY_OPACITY, values_[KEY_OPACITY]);
  settings_.setValue(KEY_STAY_ON_TOP, values_[KEY_STAY_ON_TOP]);
  settings_.setValue(KEY_TRANSP_FOR_INPUT, values_[KEY_TRANSP_FOR_INPUT]);
  settings_.setValue(KEY_SEPARATOR_FLASH, values_[KEY_SEPARATOR_FLASH]);
}

void ClockSettings::SetSkinName(const QString& skin_name) {
  values_[KEY_SKIN_NAME] = skin_name;
  emit SkinNameChanged(skin_name);
}

void ClockSettings::SetZoom(qreal zoom) {
  values_[KEY_ZOOM] = zoom;
  emit ZoomChanged(zoom);
}

void ClockSettings::SetColor(const QColor& color) {
  values_[KEY_COLOR] = color;
  emit ColorChanged(color);
}

void ClockSettings::SetTexture(const QString& filename) {
  values_[KEY_TEXTURE] = filename;
  emit TextureChanged(filename);
}

void ClockSettings::SetTexturePerElement(bool set) {
  values_[KEY_TEXTURE_PER_ELEMENT] = set;
  emit TexturePerElementChanged(set);
}

void ClockSettings::SetOpacity(qreal opacity) {
  values_[KEY_OPACITY] = opacity;
  emit OpacityChanged(opacity);
}

void ClockSettings::SetStayOnTop(bool set) {
  values_[KEY_STAY_ON_TOP] = set;
  emit StayOnTopChanged(set);
}

void ClockSettings::SetTransparentForInput(bool set) {
  values_[KEY_TRANSP_FOR_INPUT] = set;
  emit TransparentForInputChanged(set);
}

void ClockSettings::SetSeparatorFlash(bool set) {
  values_[KEY_SEPARATOR_FLASH] = set;
  emit SeparatorFlashChanged(set);
}

void ClockSettings::EmitSignals() {
  emit SkinNameChanged(values_[KEY_SKIN_NAME].toString());
  emit ZoomChanged(values_[KEY_ZOOM].toReal());
  emit ColorChanged(values_[KEY_COLOR].value<QColor>());
  emit TextureChanged(values_[KEY_TEXTURE].toString());
  emit TexturePerElementChanged(values_[KEY_TEXTURE_PER_ELEMENT].toBool());

  emit OpacityChanged(values_[KEY_OPACITY].toReal());
  emit StayOnTopChanged(values_[KEY_STAY_ON_TOP].toBool());
  emit TransparentForInputChanged(values_[KEY_TRANSP_FOR_INPUT].toBool());
  emit SeparatorFlashChanged(values_[KEY_SEPARATOR_FLASH].toBool());
}
