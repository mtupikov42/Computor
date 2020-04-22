#pragma once

#include <QObject>
#include <QColor>

class GenericColorPalette : public QObject {
	Q_OBJECT
	Q_PROPERTY(QColor accent READ accent CONSTANT)
	Q_PROPERTY(QColor black10Percent READ black10Percent CONSTANT)
	Q_PROPERTY(QColor black20Percent READ black20Percent CONSTANT)
	Q_PROPERTY(QColor black50Percent READ black50Percent CONSTANT)
	Q_PROPERTY(QColor black READ black CONSTANT)
	Q_PROPERTY(QColor highlight READ highlight CONSTANT)
	Q_PROPERTY(QColor line READ line CONSTANT)
	Q_PROPERTY(QColor textSelection READ textSelection CONSTANT)
	Q_PROPERTY(QColor transparent READ transparent CONSTANT)
	Q_PROPERTY(QColor white10Percent READ white10Percent CONSTANT)
	Q_PROPERTY(QColor white20Percent READ white20Percent CONSTANT)
	Q_PROPERTY(QColor white50Percent READ white50Percent CONSTANT)
	Q_PROPERTY(QColor white READ white CONSTANT)
	Q_PROPERTY(QColor windowBackground READ windowBackground CONSTANT)
	Q_PROPERTY(QColor windowHeader READ windowHeader CONSTANT)

public:
	explicit GenericColorPalette(QObject* parent = nullptr);

	QColor accent() const;
	QColor black10Percent() const;
	QColor black20Percent() const;
	QColor black50Percent() const;
	QColor black() const;
	QColor highlight() const;
	QColor line() const;
	QColor textSelection() const;
	QColor transparent() const;
	QColor white10Percent() const;
	QColor white20Percent() const;
	QColor white50Percent() const;
	QColor white() const;
	QColor windowBackground() const;
	QColor windowHeader() const;

private:
	const QColor m_accent;
	const QColor m_black;
	const QColor m_black10Percent;
	const QColor m_black20Percent;
	const QColor m_black50Percent;
	const QColor m_textSelection;
	const QColor m_transparent;
	const QColor m_white;
	const QColor m_white10Percent;
	const QColor m_white20Percent;
	const QColor m_white50Percent;
	const QColor m_highlight;
	const QColor m_line;
	const QColor m_windowBackground;
	const QColor m_windowHeader;
};
