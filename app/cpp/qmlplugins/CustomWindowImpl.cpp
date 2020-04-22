#include "CustomWindowImpl.h"

#ifdef Q_OS_WIN
    #include "WinCustomWindowImpl.h"
#else
    #include "MacCustomWindowImpl.h"
#endif

CustomWindowImpl* CustomWindowImpl::create(QObject* parent) {
#ifdef Q_OS_WIN
	return new WinCustomWindowImpl(parent);
#else
	return new MacCustomWindowImpl(parent);
#endif
}

CustomWindowImpl::CustomWindowImpl(QObject* parent) : QObject(parent) {}

int CustomWindowImpl::minimumWidth() const {
	return m_minimumSize.width();
}

void CustomWindowImpl::setMinimumWidth(int width) {
	if (m_minimumSize.width() == width) {
		return;
	}

	m_minimumSize.setWidth(width);
	emit minimumWidthChanged(width);
}

int CustomWindowImpl::minimumHeight() const {
	return m_minimumSize.height();
}

void CustomWindowImpl::setMinimumHeight(int height) {
	if (m_minimumSize.height() == height) {
		return;
	}

	m_minimumSize.setHeight(height);
	emit minimumHeightChanged(height);
}

int CustomWindowImpl::maximumWidth() const {
	return m_maximumSize.width();
}

void CustomWindowImpl::setMaximumWidth(int width) {
	if (m_maximumSize.width() == width) {
		return;
	}

	m_maximumSize.setWidth(width);
	emit maximumWidthChanged(width);
}

int CustomWindowImpl::maximumHeight() const {
	return m_maximumSize.height();
}

void CustomWindowImpl::setMaximumHeight(int height) {
	if (m_maximumSize.height() == height) {
		return;
	}

	m_maximumSize.setHeight(height);
	emit maximumHeightChanged(height);
}

int CustomWindowImpl::borderWidth() const {
	return m_borderWidth;
}

void CustomWindowImpl::setBorderWidth(int value) {
	if (m_borderWidth == value) {
		return;
	}

	m_borderWidth = value;
	emit borderWidthChanged(value);
}

int CustomWindowImpl::titleHeight() const {
	return m_titleHeight;
}

void CustomWindowImpl::setTitleHeight(int value) {
	if (m_titleHeight == value) {
		return;
	}

	m_titleHeight = value;
	emit titleHeightChanged(value);
}

QRect CustomWindowImpl::titleButtonsArea() const {
	return m_buttonsArea;
}

void CustomWindowImpl::setTitleButtonsArea(const QRect& rect) {
	if (rect == m_buttonsArea) {
		return;
	}

	m_buttonsArea = rect;
	emit titleButtonsAreaChanged(rect);
}

void CustomWindowImpl::setGeometry(const QRect& newGeometry) {
	setX(newGeometry.x());
	setY(newGeometry.y());
	setHeight(newGeometry.height());
	setWidth(newGeometry.width());
}

QRect CustomWindowImpl::geometry() const {
	return QRect(QPoint(x(), y()), QSize(width(), height()));
}

QVariantMap CustomWindowImpl::saveWindowState() {
	return {};
}

void CustomWindowImpl::restoreWindowState(const QVariantMap& data) {
	Q_UNUSED(data)
}

QWindow::Visibility CustomWindowImpl::visibility() const {
	return m_visibility;
}

void CustomWindowImpl::raise() {
	if (m_isClosed) {
		m_isClosed = false;
	}
}

void CustomWindowImpl::close() {
	if (m_isClosed) {
		return;
	}

	m_isClosed = true;
}

int CustomWindowImpl::validateValue(int value, int min, int max) {
	int newValue;
	if (max > 0) {
		newValue = std::min(value, max);
	} else {
		newValue = value;
	}

	if (min > 0) {
		return std::max(newValue, min);
	} else {
		return newValue;
	}
}
