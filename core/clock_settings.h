#ifndef CLOCK_SETTINGS_H
#define CLOCK_SETTINGS_H

#include <QMap>
#include <QString>
#include <QColor>
#include <QSettings>

class ClockSettings : public QObject {
  Q_OBJECT
public:
  explicit ClockSettings(QObject *parent = 0);

signals:
  void SkinNameChanged(const QString& skin_name);
  void ZoomChanged(qreal zoom);
  void ColorChanged(const QColor& color);
  void TextureChanged(const QString& filename);
  void TexturePerElementChanged(bool set);

public slots:
  void Load();
  void Save();

  void SetSkinName(const QString& skin_name);
  void SetZoom(qreal zoom);
  void SetColor(const QColor& color);
  void SetTexture(const QString& filename);
  void SetTexturePerElement(bool set);

private:
  void EmitSignals();
  QMap<QString, QVariant> values_;
  QSettings settings_;
};

#endif // CLOCK_SETTINGS_H
