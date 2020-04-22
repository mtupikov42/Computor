#pragma once

#include <QObject>
#include <QString>
#include <QFont>

class Theme final : public QObject {
	Q_OBJECT
	Q_PROPERTY(int defaultBorderRadius MEMBER(defaultBorderRadius) CONSTANT)
	Q_PROPERTY(int defaultBorderWidth MEMBER(defaultBorderWidth) CONSTANT)
	Q_PROPERTY(int defaultFontSize MEMBER(defaultFontPointSize) CONSTANT)
	Q_PROPERTY(int defaultTooltipDelay MEMBER(defaultTooltipDelay) CONSTANT)
	Q_PROPERTY(int headerFontSize MEMBER(headerFontPointSize) CONSTANT)
	Q_PROPERTY(int iconFontSize MEMBER(iconFontPointSize) CONSTANT)
	Q_PROPERTY(int largeIconFontSize MEMBER(largeIconFontPointSize) CONSTANT)
	Q_PROPERTY(QFont iconFontBrands MEMBER(iconFontBrands) CONSTANT)
	Q_PROPERTY(QFont iconFontRegular MEMBER(iconFontRegular) CONSTANT)
	Q_PROPERTY(QFont iconFontSolid MEMBER(iconFontSolid) CONSTANT)
	Q_PROPERTY(qreal defaultButtonHeight MEMBER(defaultButtonHeight) CONSTANT)
	Q_PROPERTY(qreal defaultTextFieldHeight MEMBER(defaultTextFieldHeight) CONSTANT)
	Q_PROPERTY(qreal ico7nButtonHeight MEMBER(iconButtonHeight) CONSTANT)
	Q_PROPERTY(qreal smallButtonHeight MEMBER(smallButtonHeight) CONSTANT)
	Q_PROPERTY(qreal windowBorderSize MEMBER(windowBorderSize) CONSTANT)
	Q_PROPERTY(qreal windowTitleButtonSize MEMBER(windowTitleButtonSize) CONSTANT)
	Q_PROPERTY(qreal windowTitleHeight MEMBER(windowTitleHeight) CONSTANT)
	Q_PROPERTY(QString defaultBlackFontFamily MEMBER(defaultBlackFontFamily) CONSTANT)
	Q_PROPERTY(QString defaultFontFamily MEMBER(defaultFontFamily) CONSTANT)
	Q_PROPERTY(QString iconFontFamilyBrands MEMBER(iconFontFamilyBrands) CONSTANT)
	Q_PROPERTY(QString iconFontFamilyRegular MEMBER(iconFontFamilyRegular) CONSTANT)
	Q_PROPERTY(QString iconFontFamilySolid MEMBER(iconFontFamilySolid) CONSTANT)

public:
	enum Position {
		Left,
		Top,
		Right,
		Bottom
	};
	Q_ENUM(Position)

	static void RegisterType();

private:
	Theme(QObject* parent = nullptr);

	const int defaultBorderRadius;
	const int defaultFontPointSize{ 10 };
	const int defaultTooltipDelay{ 500 };
	const int headerFontPointSize{ 12 };
	const int iconFontPointSize{ 10 };
	const int largeIconFontPointSize{ 14 };
	const qreal defaultBorderWidth;
	const qreal defaultButtonHeight;
	const qreal defaultTextFieldHeight;
	const qreal iconButtonHeight;
	const qreal smallButtonHeight;
	const qreal windowBorderSize;
	const qreal windowTitleButtonSize;
	const qreal windowTitleHeight;
	const QString defaultBlackFontFamily{ "Roboto Black" };
	const QString defaultFontFamily{ "Roboto" };
	const QString iconFontFamilyBrands{ "Font Awesome 5 Brands Regular" };
	const QString iconFontFamilyRegular{ "Font Awesome 5 Free Regular" };
	const QString iconFontFamilySolid{ "Font Awesome 5 Free Solid" };

	QFont iconFontBrands;
	QFont iconFontRegular;
	QFont iconFontSolid;
};
