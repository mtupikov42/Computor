#include "Theme.h"

#include <QQmlEngine>

#include "SizeProvider.h"

void Theme::RegisterType() {
	static const auto instantiator = [](QQmlEngine* engine, QJSEngine*) -> QObject* {
		return new Theme(engine);
	};

	qmlRegisterSingletonType<Theme>("Theme", 1, 0, "Theme", instantiator);
}

Theme::Theme(QObject* parent)
    : QObject(parent)
    , defaultBorderRadius{ SizeProvider::scaledMetric(4) }
    , defaultBorderWidth{ SizeProvider::scaledMetric(1.) }
    , defaultButtonHeight{ SizeProvider::scaledMetric(25.) }
    , defaultTextFieldHeight{ SizeProvider::scaledMetric(25.) }
    , iconButtonHeight{ SizeProvider::scaledMetric(15.) }
    , smallButtonHeight{ SizeProvider::scaledMetric(18.) }
    , windowBorderSize{ SizeProvider::scaledMetric(5.)}
    , windowTitleButtonSize{ SizeProvider::scaledMetric(30.)}
    , windowTitleHeight{ SizeProvider::scaledMetric(30.) }
    , iconFontBrands(iconFontFamilyBrands, largeIconFontPointSize)
    , iconFontRegular(iconFontFamilyRegular, iconFontPointSize)
    , iconFontSolid(iconFontFamilySolid, iconFontPointSize)
{
}
