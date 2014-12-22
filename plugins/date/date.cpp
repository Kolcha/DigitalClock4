#include <QLabel>
#include <QGridLayout>
#include <QFontMetricsF>
#include <QDate>
#include "skin_drawer.h"
#include "text_skin.h"
#include "plugin_settings.h"
#include "date_settings.h"
#include "gui/settings_dialog.h"
#include "date.h"

namespace date {

Date::Date() : avail_width_(0), last_zoom_(1.0), last_date_("-") {
  drawer_ = new skin_draw::SkinDrawer(this);
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          this, SLOT(SettingsListener(QString,QVariant)));

  InitTranslator(QLatin1String(":/date/date_"));
  info_.display_name = tr("Date");
  info_.description = tr("Allows to display current date under clock.");
  InitIcon(":/date/icon.svg");
}

void Date::Init(const QMap<Options, QVariant>& current_settings) {
  for (auto iter = current_settings.begin(); iter != current_settings.end(); ++iter) {
    switch (iter.key()) {
      case OPT_FONT:
        clock_font_ = iter.value().value<QFont>();
        break;

      case OPT_ZOOM:
        last_zoom_ = iter.value().toReal();
        break;

      case OPT_COLOR:
        drawer_->SetColor(iter.value().value<QColor>());
        break;

      case OPT_TEXTURE:
        drawer_->SetTexture(iter.value().toString());
        break;

      case OPT_TEXTURE_PER_ELEMENT:
        drawer_->SetTexturePerElement(iter.value().toBool());
        break;

      case OPT_TEXTURE_DRAW_MODE:
        drawer_->SetTextureDrawMode((skin_draw::SkinDrawer::DrawMode)iter.value().toInt());
        break;

      case OPT_CUSTOMIZATION:
        drawer_->SetCustomizationType((skin_draw::SkinDrawer::CustomizationType)iter.value().toInt());
        break;

      default:
        break;
    }
  }
  drawer_->SetSpace(0);
}

void Date::Init(QWidget* main_wnd) {
  main_layout_ = qobject_cast<QGridLayout*>(main_wnd->layout());
  main_wnd_ = main_wnd;
  last_main_wnd_size_ = main_wnd->size();
  avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 7;

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
}

void Date::Start() {
  msg_label_ = new QLabel();
  msg_label_->setAlignment(Qt::AlignCenter);
  main_layout_->addWidget(msg_label_, main_layout_->rowCount(), 0, 1, main_layout_->columnCount());
  connect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, [=] (const QImage& img) {
    msg_label_->setPixmap(QPixmap::fromImage(img));
    main_wnd_->adjustSize();
    last_main_wnd_size_ = main_wnd_->size();
  });

  settings_->Load();
}

void Date::Stop() {
  main_layout_->removeWidget(msg_label_);
  main_wnd_->adjustSize();
  disconnect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, 0, 0);
  delete msg_label_;
}

void Date::Configure() {
  SettingsDialog* dialog = new SettingsDialog();
  // load current settings to dialog
  dialog->Init(settings_->GetSettingsMap());
  // connect main signals/slots
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  dialog->show();
}

void Date::SettingsListener(Options option, const QVariant& new_value) {
  if (!msg_label_) return;  // not started

  switch (option) {
    case OPT_SKIN_NAME:
      avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 7;
      switch ((ZoomMode)settings_->GetOption(OPT_ZOOM_MODE).toInt()) {
        case ZoomMode::ZM_NOT_ZOOM:
          msg_label_->setAlignment(Qt::AlignCenter);
          break;

        case ZoomMode::ZM_AUTOSIZE:
          last_date_ = "-";
          msg_label_->setAlignment(Qt::AlignLeft);
          TimeUpdateListener();
          break;

        case ZoomMode::ZM_CLOCK_ZOOM:
          drawer_->SetZoom(last_zoom_);
          msg_label_->setAlignment(Qt::AlignCenter);
          break;
      }
      break;

    case OPT_FONT:
      clock_font_ = new_value.value<QFont>();
      if (!settings_->GetOption(OPT_USE_CLOCK_FONT).toBool()) break;
      font_ = clock_font_;
      drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
      break;

    case OPT_ZOOM:
      last_zoom_ = new_value.toReal();
      switch ((ZoomMode)settings_->GetOption(OPT_ZOOM_MODE).toInt()) {
        case ZoomMode::ZM_NOT_ZOOM:
          msg_label_->setAlignment(Qt::AlignCenter);
          break;

        case ZoomMode::ZM_AUTOSIZE:
        {
          QFontMetricsF fmf(font_);
          QStringList ss = last_date_.split('\n');
          qreal tw = 0;
          for (auto& s : ss) tw = qMax(tw, fmf.width(s));
          drawer_->SetZoom(avail_width_ * last_zoom_ / tw);
          msg_label_->setAlignment(Qt::AlignLeft);
          break;
        }

        case ZoomMode::ZM_CLOCK_ZOOM:
          drawer_->SetZoom(last_zoom_);
          msg_label_->setAlignment(Qt::AlignCenter);
          break;
      }
      break;

    case OPT_COLOR:
      drawer_->SetColor(new_value.value<QColor>());
      break;

    case OPT_TEXTURE:
      drawer_->SetTexture(new_value.toString());
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_->SetTexturePerElement(new_value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      drawer_->SetTextureDrawMode((skin_draw::SkinDrawer::DrawMode)new_value.toInt());
      break;

    case OPT_CUSTOMIZATION:
      drawer_->SetCustomizationType((skin_draw::SkinDrawer::CustomizationType)new_value.toInt());
      break;

    default:
      break;
  }
}

void Date::TimeUpdateListener() {
  if (!msg_label_) return;  // not started

  QString date;
  QDate d_date = QDate::currentDate();

  switch ((FormatType)settings_->GetOption(OPT_DATE_FORMAT_TYPE).toInt()) {
    case FormatType::FT_INT:
      date = d_date.toString((Qt::DateFormat)settings_->GetOption(OPT_DATE_FORMAT_INT).toInt());
      break;

    case FormatType::FT_STR:
    {
      QString format = settings_->GetOption(OPT_DATE_FORMAT_STR).toString();

      int prev = -1;
      bool is_start = false;
      while (true) {
        int pos = format.indexOf('\'', prev + 1);
        QString substr = format.mid(prev + 1, pos - prev - 1);
        if (is_start) {
          int s_pos = substr.indexOf("\\n");
          if (s_pos != -1) substr.replace(s_pos, 2, '\n');
          is_start = false;
        } else {
          // parse 'WW' or 'W' -- week number
          int s_pos = substr.indexOf("WW", 0, Qt::CaseInsensitive);
          if (s_pos != -1) {
            substr.replace(s_pos, 2, QString("%1").arg(d_date.weekNumber(), 2, 10, QChar('0')));
          } else {
            s_pos = substr.indexOf('W', 0, Qt::CaseInsensitive);
            if (s_pos != -1) substr.replace(s_pos, 1, QString::number(d_date.weekNumber()));
          }
          // parse 'J' -- day of year
          s_pos = substr.indexOf('J', 0, Qt::CaseSensitive);
          if (s_pos != -1) substr.replace(s_pos, 1, QString::number(d_date.dayOfYear()));
          is_start = true;
        }
        format.replace(prev + 1, pos != -1 ? pos - prev - 1 : format.length() - prev - 1, substr);
        pos = format.indexOf('\'', prev + 1);
        if (pos == -1) break;
        prev = pos;
      }

      date = d_date.toString(format);
      break;
    }
  }

  if (date == last_date_ && last_main_wnd_size_ == main_wnd_->size()) return;

  switch ((ZoomMode)settings_->GetOption(OPT_ZOOM_MODE).toInt()) {
    case ZoomMode::ZM_NOT_ZOOM:
      break;

    case ZoomMode::ZM_AUTOSIZE:
    {
      drawer_->SetString(QString());
      QFontMetricsF fmf(font_);
      QStringList ss = date.split('\n');
      qreal tw = 0;
      for (auto& s : ss) tw = qMax(tw, fmf.width(s));
      drawer_->SetZoom(avail_width_ * last_zoom_ / tw);
      break;
    }

    case ZoomMode::ZM_CLOCK_ZOOM:
      break;
  }
  drawer_->SetString(date);
  last_date_ = date;
}

void Date::SettingsListener(const QString& key, const QVariant& value) {
  if (!msg_label_) return;  // not started

  if (key == OPT_USE_CLOCK_FONT) {
    SettingsListener(OPT_DATE_FONT, value.toBool() ?
                       clock_font_ : settings_->GetOption(OPT_DATE_FONT).value<QFont>());
  }
  if (key == OPT_DATE_FONT) {
    font_ = value.value<QFont>();
    if (settings_->GetOption(OPT_USE_CLOCK_FONT).toBool()) font_ = clock_font_;
    drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
    last_date_ = "-";      // reset last date to recalculate zoom
    TimeUpdateListener();  // on redraw if needed
  }
  if (key == OPT_ZOOM_MODE) {
    switch ((ZoomMode)value.toInt()) {
      case ZoomMode::ZM_NOT_ZOOM:
        msg_label_->setAlignment(Qt::AlignCenter);
        drawer_->SetZoom(1.0);
        break;

      case ZoomMode::ZM_AUTOSIZE:
        SettingsListener(OPT_SKIN_NAME, QString("Text Skin"));
        break;

      case ZoomMode::ZM_CLOCK_ZOOM:
        SettingsListener(OPT_ZOOM, last_zoom_);
        break;
    }
  }
}

} // namespace date
