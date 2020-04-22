#pragma once

#include <QColor>
#include <QObject>

struct ControlsPalette final {
	QColor background;
	QColor border;
	QColor icon;
	QColor indication;
	QColor text;
};

class ControlsColorPalette : public QObject {
	Q_OBJECT
	Q_PROPERTY(QColor background READ background CONSTANT)
	Q_PROPERTY(QColor border READ border CONSTANT)
	Q_PROPERTY(QColor icon READ icon CONSTANT)
	Q_PROPERTY(QColor indication READ indication CONSTANT)
	Q_PROPERTY(QColor text READ text CONSTANT)

public:
	explicit ControlsColorPalette(const ControlsPalette& palette, QObject* parent = nullptr);

	QColor background() const;
	QColor border() const;
	QColor icon() const;
	QColor indication() const;
	QColor text() const;

private:
	QColor m_background;
	QColor m_border;
	QColor m_icon;
	QColor m_indication;
	QColor m_text;
};
