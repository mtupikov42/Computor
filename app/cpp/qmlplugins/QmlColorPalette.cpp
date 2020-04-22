#include "QmlColorPalette.h"

#include <QQmlEngine>

namespace {

ControlsColorPalette* createNormal(QObject* parent) {
	ControlsPalette palette;
	palette.background = "#3b3c40";
	palette.border = "#54555a";
	palette.icon = "#a7a8aa";
	palette.indication = "#c8c9c7";
	palette.text = "#a7a8aa";

	return new ControlsColorPalette(palette, parent);
}

ControlsColorPalette* createActive(QObject* parent) {
	ControlsPalette palette;
	palette.background = "#303236";
	palette.border = "#ffffff";
	palette.icon = "#c8c9c7";
	palette.indication = "#ffffff";
	palette.text = "#c8c9c7";

	return new ControlsColorPalette(palette, parent);
}

ControlsColorPalette* createDisabled(QObject* parent) {
	ControlsPalette palette;
	palette.background = Qt::transparent;
	palette.border = "#54555a";
	palette.icon = "#54555a";
	palette.indication = "#54555a";
	palette.text = "#54555a";

	return new ControlsColorPalette(palette, parent);
}

ControlsColorPalette* createHovered(QObject* parent) {
	ControlsPalette palette;
	palette.background = "#303236";
	palette.border = "#ffffff";
	palette.icon = "#ebebeb";
	palette.indication = "#ffffff";
	palette.text = "#ebebeb";

	return new ControlsColorPalette(palette, parent);
}

} // end anonymous namepsace

void QmlColorPalette::RegisterType() {
	static const auto instantiator = [](QQmlEngine* engine, QJSEngine*) -> QObject* {
		return new QmlColorPalette(engine);
	};

	qmlRegisterSingletonType<QmlColorPalette>("QmlColorPalette", 1, 0, "QmlColorPalette", instantiator);
	qmlRegisterUncreatableType<GenericColorPalette>(
	    "QmlColorPalette",
	    1,
	    0,
	    "GenericColorPalette",
	    "Can't create GenericColorPalette via QML"
	);
	qmlRegisterUncreatableType<ControlsColorPalette>(
	    "QmlColorPalette",
	    1,
	    0,
	    "ControlsColorPalette",
	    "Can't create ControlsColorPalette via QML"
	);
}

QmlColorPalette::QmlColorPalette(QObject* parent)
    : QObject(parent)
    , m_paletteMap(createPaletteMap())
    , m_genericPalette(new GenericColorPalette(this))
{}

QMap<QmlColorPalette::ColorGroup, ControlsColorPalette*> QmlColorPalette::createPaletteMap() {
	const QMap<ColorGroup, ControlsColorPalette*> theme = {
	    { ColorGroup::Normal, createNormal(this) },
	    { ColorGroup::Active, createActive(this) },
	    { ColorGroup::Disabled, createDisabled(this) },
	    { ColorGroup::Hovered, createHovered(this) }
	};

	return theme;
}

ControlsColorPalette* QmlColorPalette::normalPalette() const {
	return m_paletteMap[ColorGroup::Normal];
}

ControlsColorPalette* QmlColorPalette::activePalette() const {
	return m_paletteMap[ColorGroup::Active];
}

ControlsColorPalette* QmlColorPalette::disabledPalette() const {
	return m_paletteMap[ColorGroup::Disabled];
}

ControlsColorPalette* QmlColorPalette::hoveredPalette() const {
	return m_paletteMap[ColorGroup::Hovered];
}

GenericColorPalette* QmlColorPalette::genericPalette() const {
	return m_genericPalette;
}
