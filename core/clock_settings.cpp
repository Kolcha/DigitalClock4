#include "clock_settings.h"

// skin settings
#define KEY_SKIN_NAME           "skin/name"
#define KEY_ZOOM                "skin/zoom"
#define KEY_COLOR               "skin/color"
#define KEY_TEXTURE             "skin/texture"
#define KEY_TEXTURE_PER_ELEMENT "skin/texture_per_element"

ClockSettings::ClockSettings(QObject *parent)
  : QObject(parent) {
}

void ClockSettings::Load() {
  values_[KEY_SKIN_NAME] = settings_.value(KEY_SKIN_NAME, "default");
  values_[KEY_ZOOM] = settings_.value(KEY_ZOOM, "1.0");
  values_[KEY_COLOR] = settings_.value(KEY_COLOR);
  values_[KEY_TEXTURE] = settings_.value(KEY_TEXTURE);
  values_[KEY_TEXTURE_PER_ELEMENT] = settings_.value(KEY_TEXTURE_PER_ELEMENT);
  EmitSignals();
}

void ClockSettings::Save() {
  settings_.setValue(KEY_SKIN_NAME, values_[KEY_SKIN_NAME]);
  settings_.setValue(KEY_ZOOM, values_[KEY_ZOOM]);
  settings_.setValue(KEY_COLOR, values_[KEY_COLOR]);
  settings_.setValue(KEY_TEXTURE, values_[KEY_TEXTURE]);
  settings_.setValue(KEY_TEXTURE_PER_ELEMENT, values_[KEY_TEXTURE_PER_ELEMENT]);
}

void ClockSettings::SetSkinName(const QString& skin_name) {
  values_[KEY_SKIN_NAME] = skin_name;
  SkinNameChanged(skin_name);
}

void ClockSettings::SetZoom(qreal zoom) {
  values_[KEY_ZOOM] = zoom;
  ZoomChanged(zoom);
}

void ClockSettings::SetColor(const QColor& color) {
  values_[KEY_COLOR] = color;
  ColorChanged(color);
}

void ClockSettings::SetTexture(const QString& filename) {
  values_[KEY_TEXTURE] = filename;
  TextureChanged(filename);
}

void ClockSettings::SetTexturePerElement(bool set) {
  values_[KEY_TEXTURE_PER_ELEMENT] = set;
  TexturePerElementChanged(set);
}

void ClockSettings::EmitSignals() {
  emit SkinNameChanged(values_[KEY_SKIN_NAME].toString());
  emit ZoomChanged(values_[KEY_ZOOM].toReal());
  emit ColorChanged(values_[KEY_COLOR].value<QColor>());
  emit TextureChanged(values_[KEY_TEXTURE].toString());
  emit TexturePerElementChanged(values_[KEY_TEXTURE_PER_ELEMENT].toBool());
}
