#include "ControlsColorPalette.h"

ControlsColorPalette::ControlsColorPalette(const ControlsPalette& palette, QObject* parent)
    : QObject(parent)
    , m_background(palette.background)
    , m_border(palette.border)
    , m_icon(palette.icon)
    , m_indication(palette.indication)
    , m_text(palette.text)
{
}

QColor ControlsColorPalette::background() const {
	return m_background;
}

QColor ControlsColorPalette::border() const {
	return m_border;
}

QColor ControlsColorPalette::icon() const {
	return m_icon;
}

QColor ControlsColorPalette::indication() const {
	return m_indication;
}

QColor ControlsColorPalette::text() const {
	return m_text;
}
