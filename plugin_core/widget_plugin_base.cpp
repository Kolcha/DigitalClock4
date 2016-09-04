#include "widget_plugin_base.h"

#include <QGridLayout>
#include <QFontMetricsF>

#include "skin_drawer.h"
#include "text_skin.h"

#include "plugin_settings.h"

#include "base_settings_widget.h"

namespace plugin {

WidgetPluginBase::WidgetPluginBase() : avail_width_(0), clock_zoom_(1.0), last_text_("----")
{
    drawer_ = new skin_draw::SkinDrawer(this);
}

void WidgetPluginBase::Init(const QMap<Option, QVariant>& current_settings)
{
    for (auto iter = current_settings.begin(); iter != current_settings.end(); ++iter) {
        switch (iter.key()) {
        case OPT_FONT:
            clock_font_ = iter.value().value<QFont>();
            break;

        case OPT_ZOOM:
            clock_zoom_ = iter.value().toReal();
            break;

        case OPT_COLOR:
            drawer_->SetColor(iter.value().value<QColor>());
            break;

        case OPT_TEXTURE:
            drawer_->SetTexture(iter.value().toString());
            break;

        case OPT_TEXTURE_TYPE:
            drawer_->SetCustomizationType(static_cast<skin_draw::SkinDrawer::CustomizationType>(iter.value().toInt()));
            break;

        case OPT_TEXTURE_PER_ELEMENT:
            drawer_->SetTexturePerElement(iter.value().toBool());
            break;

        case OPT_TEXTURE_DRAW_MODE:
            drawer_->SetTextureDrawMode(static_cast<skin_draw::SkinDrawer::DrawMode>(iter.value().toInt()));
            break;

        case OPT_CUSTOMIZATION:
        {
            Customization cust = static_cast<Customization>(iter.value().toInt());
            switch (cust) {
            case Customization::C_NONE:
            case Customization::C_COLORIZE:
                drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
                break;

            default:
                break;
            }
            break;
        }

        default:
            break;
        }
    }
    drawer_->SetSpace(0);
}

void WidgetPluginBase::Init(QWidget* main_wnd)
{
    main_layout_ = qobject_cast<QGridLayout*>(main_wnd->layout());
    main_wnd_ = main_wnd;
    avail_width_ = main_layout_->cellRect(0, 0).width();

    connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
            this, SLOT(SettingsChangeListener(QString,QVariant)));

    QSettings::SettingsMap defaults;
    InitBaseSettingsDefaults(&defaults);
    InitSettingsDefaults(&defaults);
    settings_->SetDefaultValues(defaults);
    settings_->TrackChanges(true);
}

void WidgetPluginBase::Start()
{
    plg_widget_ = InitWidget(main_layout_);
    connect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, [=] (const QImage& img) {
        this->DisplayImage(img);
        plg_widget_->adjustSize();
        if (plg_widget_->isHidden()) plg_widget_->show();
        main_wnd_->adjustSize();
    });

    settings_->Load();
}

void WidgetPluginBase::Stop()
{
    disconnect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, 0, 0);
    main_layout_->removeWidget(plg_widget_);
    main_wnd_->adjustSize();
    delete plg_widget_;
}

void WidgetPluginBase::SettingsListener(Option option, const QVariant& new_value)
{
    if (!plg_widget_) return;  // not started

    switch (option) {
    case OPT_SKIN_NAME:
        plg_widget_->hide();
        main_wnd_->adjustSize();
        avail_width_ = main_layout_->cellRect(0, 0).width();

        switch (static_cast<ZoomMode>(settings_->GetOption(OptionKey(OPT_ZOOM_MODE, plg_name_)).toInt())) {
        case ZoomMode::ZM_NOT_ZOOM:
            drawer_->SetZoom(1.0);
            break;

        case ZoomMode::ZM_AUTOSIZE:
            last_text_ = "-";
            TimeUpdateListener();         // force redraw
            break;

        case ZoomMode::ZM_CLOCK_ZOOM:
            drawer_->SetZoom(clock_zoom_);
            break;
        }

        break;

    case OPT_FONT:
        clock_font_ = new_value.value<QFont>();
        if (!settings_->GetOption(OptionKey(OPT_USE_CLOCK_FONT, plg_name_)).toBool()) break;
        font_ = clock_font_;
        drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
        break;

    case OPT_ZOOM:
        clock_zoom_ = new_value.toReal();

        switch (static_cast<ZoomMode>(settings_->GetOption(OptionKey(OPT_ZOOM_MODE, plg_name_)).toInt())) {
        case ZoomMode::ZM_NOT_ZOOM:
            drawer_->SetZoom(1.0);
            break;

        case ZoomMode::ZM_AUTOSIZE:
            plg_widget_->hide();
            main_wnd_->adjustSize();
            avail_width_ = main_layout_->cellRect(0, 0).width();
            drawer_->SetZoom(CalculateZoom(last_text_));
            break;

        case ZoomMode::ZM_CLOCK_ZOOM:
            drawer_->SetZoom(clock_zoom_);
            break;
        }

        break;

    case OPT_COLOR:
        drawer_->SetColor(new_value.value<QColor>());
        break;

    case OPT_TEXTURE:
        drawer_->SetTexture(new_value.toString());
        break;

    case OPT_TEXTURE_TYPE:
        drawer_->SetCustomizationType(static_cast<skin_draw::SkinDrawer::CustomizationType>(new_value.toInt()));
        break;

    case OPT_TEXTURE_PER_ELEMENT:
        drawer_->SetTexturePerElement(new_value.toBool());
        break;

    case OPT_TEXTURE_DRAW_MODE:
        drawer_->SetTextureDrawMode(static_cast<skin_draw::SkinDrawer::DrawMode>(new_value.toInt()));
        break;

    case OPT_CUSTOMIZATION:
    {
        Customization cust = static_cast<Customization>(new_value.toInt());
        switch (cust) {
        case Customization::C_NONE:
        case Customization::C_COLORIZE:
            drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
            break;

        default:
            break;
        }
        break;
    }

    default:
        break;
    }
}

void WidgetPluginBase::TimeUpdateListener()
{
    if (!plg_widget_) return;  // not started

    int cur_avail_width = main_layout_->cellRect(0, 0).width();

    QString cur_text = GetWidgetText();

    // optimization: redraw only if needed
    if (cur_text == last_text_ && cur_avail_width == avail_width_) return;

    avail_width_ = cur_avail_width;

    switch (static_cast<ZoomMode>(settings_->GetOption(OptionKey(OPT_ZOOM_MODE, plg_name_)).toInt())) {
    case ZoomMode::ZM_NOT_ZOOM:
        break;

    case ZoomMode::ZM_AUTOSIZE:
        drawer_->SetString(QString());  // set empty string to do not redraw twice
        drawer_->SetZoom(CalculateZoom(cur_text));
        break;

    case ZoomMode::ZM_CLOCK_ZOOM:
        break;
    }

    drawer_->SetString(cur_text);
    last_text_ = cur_text;
}

void WidgetPluginBase::InitSettingsDefaults(QSettings::SettingsMap* defaults)
{
    Q_UNUSED(defaults);
}

QWidget* WidgetPluginBase::InitConfigWidget(QWidget* parent)
{
    BaseSettingsWidget* cfg_widget = new BaseSettingsWidget(parent);
    QMap<WidgetPluginOption, QVariant> cur_values;
    InitDefaults(&cur_values);
    for (auto iter = cur_values.begin(); iter != cur_values.end(); ++iter)
        *iter = settings_->GetOption(OptionKey(iter.key(), plg_name_));
    cfg_widget->InitWidgets(cur_values);
    connect(cfg_widget, &BaseSettingsWidget::OptionChanged, this, &WidgetPluginBase::onBaseOptionChanged);
    return cfg_widget;
}

QSize WidgetPluginBase::GetImageSize(const QString& text, qreal zoom) const
{
    QStringList ss = text.split('\n');
    int tw = 0;
    int th = 0;

    skin_draw::TextSkin tmp_skin(font_);
    for (auto& s : ss) {
        int lw = 0;
        int lh = 0;
        for (int i = 0; i < s.length(); ++i) {
            ::skin_draw::ISkin::QPixmapPtr img = tmp_skin.GetImage(s[i], zoom, true);
            lw += img->width();
            lh = qMax(lh, img->height());
        }
        tw = qMax(tw, lw);
        th += lh;
    }
    return QSize(tw, th);
}

void WidgetPluginBase::onBaseOptionChanged(const WidgetPluginOption opt, const QVariant& value)
{
    settings_->SetOption(OptionKey(opt, plg_name_), value);
}

void WidgetPluginBase::SettingsChangeListener(const QString& key, const QVariant& value)
{
    if (!plg_widget_) return;  // not started

    if (key == OptionKey(OPT_USE_CLOCK_FONT, plg_name_)) {
        QString font_key = OptionKey(OPT_CUSTOM_FONT, plg_name_);
        SettingsChangeListener(font_key, value.toBool() ?
                             clock_font_ : settings_->GetOption(font_key).value<QFont>());
    }
    if (key == OptionKey(OPT_CUSTOM_FONT, plg_name_)) {
        font_ = value.value<QFont>();
        drawer_->SetString(QString());
        drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
        last_text_ = "-";      // reset last date to recalculate zoom
        TimeUpdateListener();  // on redraw if needed
    }
    if (key == OptionKey(OPT_ZOOM_MODE, plg_name_)) {
        switch (static_cast<ZoomMode>(value.toInt())) {
        case ZoomMode::ZM_NOT_ZOOM:
            drawer_->SetZoom(1.0);
            break;

        case ZoomMode::ZM_AUTOSIZE:
            plg_widget_->hide();
            main_wnd_->adjustSize();
            avail_width_ = main_layout_->cellRect(0, 0).width();
            drawer_->SetZoom(CalculateZoom(last_text_));
            break;

        case ZoomMode::ZM_CLOCK_ZOOM:
            drawer_->SetZoom(clock_zoom_);
            break;
        }
    }
}

void WidgetPluginBase::InitBaseSettingsDefaults(QSettings::SettingsMap *defaults)
{
    QMap<WidgetPluginOption, QVariant> base_defaults;
    InitDefaults(&base_defaults);
    for (auto iter = base_defaults.begin(); iter != base_defaults.end(); ++iter)
        defaults->insert(OptionKey(iter.key(), plg_name_), iter.value());
}

qreal WidgetPluginBase::CalculateZoom(const QString& text) const
{
    qreal tw = GetImageSize(text, 1.0).width();
    qreal c_zoom = avail_width_ / tw;

    int c_img_w = GetImageSize(text, c_zoom).width();
    while (c_img_w > avail_width_) {
        c_zoom *= (1 - (0.5*(c_img_w - avail_width_)) / avail_width_);
        c_img_w = GetImageSize(text, c_zoom).width();
    }

    return c_zoom;
}

} // namespace plugin
