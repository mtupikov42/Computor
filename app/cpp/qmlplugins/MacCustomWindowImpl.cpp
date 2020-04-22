#include "StubNativeAreaImpl.h"
#include <QCoreApplication>
#include <QCursor>
#include <QScreen>

StubNativeAreaImpl::StubNativeAreaImpl(QObject *parent) : NativeAreaImpl(parent)
{
	qApp->installEventFilter(this);
}

bool StubNativeAreaImpl::eventFilter(QObject *obj, QEvent *evt)
{
	if (obj == window()) {
		switch (evt->type()) {
		case QEvent::MouseButtonPress:
			return onMouseButtonPressed(static_cast<QMouseEvent*>(evt));

		case QEvent::MouseButtonRelease:
			return onMouseButtonReleased(static_cast<QMouseEvent*>(evt));

		case QEvent::MouseButtonDblClick:
			return onMouseButtonDblClicked(static_cast<QMouseEvent*>(evt));

		case QEvent::MouseMove:
			return onMouseMoved(static_cast<QMouseEvent*>(evt));

		default:
			break;
		}
	}
	return qApp->eventFilter(obj, evt);
}

bool StubNativeAreaImpl::onMouseButtonPressed(QMouseEvent *evt)
{
	m_mousePressPos = evt->pos();

	m_resizeAreaType = getNativeAreaType(evt->pos());
	if (m_resizeAreaType == Area::Client) {
		return false;
	}

	if (m_resizeAreaType == Area::Caption) {
		emit titlePressed(evt->x(), evt->y(), evt->button());
	}

	m_resizeActive = true;
	return true;
}

bool StubNativeAreaImpl::onMouseButtonReleased(QMouseEvent *evt)
{
	m_resizeAreaType = getNativeAreaType(evt->pos());
	if (m_resizeAreaType == Area::Caption) {
		emit titleReleased(evt->x(), evt->y(), evt->button());
	}

	if (m_resizeActive) {
		m_resizeActive = false;
		return true;
	}
	return false;
}

bool StubNativeAreaImpl::onMouseButtonDblClicked(QMouseEvent *evt)
{
	if (m_resizeAreaType == Area::Caption) {
		emit titleDoubleClicked(evt->x(), evt->y(), evt->button());
	}

	return false;
}

bool StubNativeAreaImpl::onMouseMoved(QMouseEvent *evt)
{
	if (!m_resizeActive)
		return false;

	if (m_resizeAreaType == Area::Caption) {
		const QPoint newPos = calcNewWindowPosition(evt->pos());
		window()->setPosition(newPos);
		return true;
	} else {
		const QRect newGeometry = calcNewWindowGeometry();
		window()->setGeometry(newGeometry);
	}

	return false;
}

StubNativeAreaImpl::Area::Type StubNativeAreaImpl::getNativeAreaType(const QPoint &pos) const
{
	if (titleButtonsArea().contains(pos))
		return Area::Client;

	const bool matchLeftBorder   = pos.x() < borderWidth();
	const bool matchTopBorder    = pos.y() < borderWidth();
	const bool matchRightBorder  = pos.x() > window()->width() - borderWidth();
	const bool matchBottomBorder = pos.y() > window()->height() - borderWidth();

	if (matchLeftBorder && matchTopBorder)
		return Area::TopLeftCorner;

	if (matchRightBorder && matchTopBorder)
		return Area::TopRightCorner;

	if (matchLeftBorder && matchBottomBorder)
		return Area::BottomLeftCorner;

	if (matchRightBorder && matchBottomBorder)
		return Area::BottomRightCorner;

	if (matchLeftBorder)
		return Area::LeftBorder;

	if (matchRightBorder)
		return Area::RightBorder;

	if (matchTopBorder)
		return Area::TopBorder;

	if (matchBottomBorder)
		return Area::BottomBorder;

	if (pos.y() < titleHeight())
		return Area::Caption;

	return Area::Client;
}

QPoint StubNativeAreaImpl::calcNewWindowPosition(const QPoint &mousePos) const
{
	const QRect screenGeometry = window()->screen()->availableGeometry();
	const QPoint deltaMousePos = mousePos - m_mousePressPos;

	QPoint windowPos = window()->position();
	windowPos += deltaMousePos;

	if (windowPos.y() < screenGeometry.top()) {
		windowPos.setY(screenGeometry.top());
	} else if (windowPos.y() + titleHeight() > screenGeometry.bottom()) {
		windowPos.setY(screenGeometry.bottom() - titleHeight());
	}

	return windowPos;
}

QRect StubNativeAreaImpl::calcNewWindowGeometry() const
{
	const QRect screenGeometry = window()->screen()->availableGeometry();
	const QPoint newPos = QCursor::pos();

	QRect geometry = window()->geometry();

	switch (m_resizeAreaType) {
	case Area::TopLeftCorner:     geometry.setTopLeft(newPos);     break;
	case Area::TopRightCorner:    geometry.setTopRight(newPos);    break;
	case Area::BottomLeftCorner:  geometry.setBottomLeft(newPos);  break;
	case Area::BottomRightCorner: geometry.setBottomRight(newPos); break;
	case Area::LeftBorder:        geometry.setLeft(newPos.x());    break;
	case Area::RightBorder:       geometry.setRight(newPos.x());   break;
	case Area::TopBorder:         geometry.setTop(newPos.y());     break;
	case Area::BottomBorder:      geometry.setBottom(newPos.y());  break;
	default: break;
	}

	const int minWidth  = window()->minimumWidth();
	const int minHeight = window()->minimumHeight();
	const int maxWidth  = window()->maximumWidth();
	const int maxHeight = window()->maximumHeight();

	if (Area::isAnyTopBorder(m_resizeAreaType)) {
		if (geometry.top() < screenGeometry.top()) {
			geometry.setTop(screenGeometry.top());
		} else if (geometry.top() > screenGeometry.bottom() - borderWidth()) {
			geometry.setTop(screenGeometry.bottom() - borderWidth());
		}

		if (geometry.height() < minHeight) {
			geometry.setTop(geometry.top() - minHeight + geometry.height());
		} else if (geometry.height() > maxHeight) {
			geometry.setTop(geometry.top() + geometry.height() - maxHeight);
		}
	} else if (Area::isAnyBottomBorder(m_resizeAreaType)) {
		if (geometry.bottom() > screenGeometry.bottom()) {
			geometry.setBottom(screenGeometry.bottom());
		} else if (geometry.bottom() < screenGeometry.top() + borderWidth()) {
			geometry.setBottom(screenGeometry.top() + borderWidth());
		}

		if (geometry.height() < minHeight) {
			geometry.setBottom(geometry.top() + minHeight - 1);
		} else if (geometry.height() > maxHeight) {
			geometry.setBottom(geometry.bottom() - geometry.height() + maxHeight);
		}
	}

	if (Area::isAnyLeftBorder(m_resizeAreaType)) {
		if (geometry.left() < screenGeometry.left()) {
			geometry.setLeft(screenGeometry.left());
		} else if (geometry.left() > screenGeometry.right() - borderWidth()) {
			geometry.setLeft(screenGeometry.right() - borderWidth());
		}

		if (geometry.width() < minWidth) {
			geometry.setLeft(geometry.left() - minWidth + geometry.width());
		} else if (geometry.width() > maxWidth) {
			geometry.setLeft(geometry.left() + geometry.width() - maxWidth);
		}
	} else if (Area::isAnyRightBorder(m_resizeAreaType)) {
		if (geometry.right() > screenGeometry.right()) {
			geometry.setRight(screenGeometry.right());
		} else if (geometry.right() < screenGeometry.left() + borderWidth()) {
			geometry.setRight(screenGeometry.left() + borderWidth());
		}

		if (geometry.width() < minWidth) {
			geometry.setRight(geometry.left() + minWidth - 1);
		} else if (geometry.width() > maxWidth) {
			geometry.setRight(geometry.right() - geometry.width() + maxWidth);
		}
	}

	return geometry;
}
