#include "GenericColorPalette.h"

namespace {

QColor setAlpha(QColor color, double alfa) {
	color.setAlphaF(alfa);
	return color;
}

} // end anonymous namespace

GenericColorPalette::GenericColorPalette(QObject* parent)
    : QObject(parent)
    , m_accent("#adff00")
    , m_black("#000000")
    , m_black10Percent(setAlpha(m_black, 0.1))
    , m_black20Percent(setAlpha(m_black, 0.1))
    , m_black50Percent(setAlpha(m_black, 0.1))
    , m_error(setAlpha(Qt::red, 0.8))
    , m_textSelection(setAlpha("#ffffff", 0.3))
    , m_transparent(Qt::transparent)
    , m_white("#ffffff")
    , m_white10Percent(setAlpha(m_white, 0.1))
    , m_white20Percent(setAlpha(m_white, 0.2))
    , m_white50Percent(setAlpha(m_white, 0.5))
    , m_highlight(m_white)
    , m_line(m_white10Percent)
    , m_windowBackground("#212226")
    , m_windowHeader("#3b3c40")
{
}

QColor GenericColorPalette::accent() const {
	return m_accent;
}

QColor GenericColorPalette::black10Percent() const {
	return m_black10Percent;
}

QColor GenericColorPalette::black20Percent() const {
	return m_black20Percent;
}

QColor GenericColorPalette::black50Percent() const {
	return m_black50Percent;
}

QColor GenericColorPalette::black() const {
	return m_black;
}

QColor GenericColorPalette::error() const {
	return m_error;
}

QColor GenericColorPalette::highlight() const {
	return m_highlight;
}

QColor GenericColorPalette::line() const {
	return m_line;
}

QColor GenericColorPalette::textSelection() const {
	return m_textSelection;
}

QColor GenericColorPalette::transparent() const {
	return m_transparent;
}

QColor GenericColorPalette::white10Percent() const {
	return m_white10Percent;
}

QColor GenericColorPalette::white20Percent() const {
	return m_white20Percent;
}

QColor GenericColorPalette::white50Percent() const {
	return m_white50Percent;
}

QColor GenericColorPalette::white() const {
	return m_white;
}

QColor GenericColorPalette::windowBackground() const {
	return m_windowBackground;
}

QColor GenericColorPalette::windowHeader() const {
	return m_windowHeader;
}

