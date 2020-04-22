#pragma once

#include <QObject>
#include <QMap>

#include "ControlsColorPalette.h"
#include "GenericColorPalette.h"

class QmlColorPalette : public QObject {
	Q_OBJECT
	Q_PROPERTY(ControlsColorPalette* normal READ normalPalette CONSTANT)
	Q_PROPERTY(ControlsColorPalette* active READ activePalette CONSTANT)
	Q_PROPERTY(ControlsColorPalette* disabled READ disabledPalette CONSTANT)
	Q_PROPERTY(ControlsColorPalette* hovered READ hoveredPalette CONSTANT)
	Q_PROPERTY(GenericColorPalette* generic READ genericPalette CONSTANT)

public:
	static void RegisterType();

private:
	enum class ColorGroup {
		Normal,
		Active,
		Disabled,
		Hovered
	};

	explicit QmlColorPalette(QObject* parent = nullptr);

	QMap<ColorGroup, ControlsColorPalette*> createPaletteMap();

	ControlsColorPalette* normalPalette() const;
	ControlsColorPalette* activePalette() const;
	ControlsColorPalette* disabledPalette() const;
	ControlsColorPalette* hoveredPalette() const;
	GenericColorPalette* genericPalette() const;

	QMap<ColorGroup, ControlsColorPalette*> m_paletteMap;
	GenericColorPalette* m_genericPalette = nullptr;
};
