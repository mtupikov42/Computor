#pragma once

#include "NativeAreaImpl.h"
#include <QPoint>
#include <QRect>

class QMouseEvent;

class StubNativeAreaImpl : public NativeAreaImpl
{
	Q_OBJECT

	struct Area
	{
		enum Type {
			LeftBorder,
			RightBorder,
			TopBorder,
			BottomBorder,
			TopLeftCorner,
			TopRightCorner,
			BottomLeftCorner,
			BottomRightCorner,
			Caption,
			Client
		};

		static bool isAnyTopBorder(Type a)
		{
			return a == TopBorder || a == TopLeftCorner || a == TopRightCorner;
		}

		static bool isAnyBottomBorder(Type a)
		{
			return a == BottomBorder || a == BottomLeftCorner || a == BottomRightCorner;
		}

		static bool isAnyLeftBorder(Type a)
		{
			return a == LeftBorder || a == TopLeftCorner || a == BottomLeftCorner;
		}

		static bool isAnyRightBorder(Type a)
		{
			return a == RightBorder || a == TopRightCorner || a == BottomRightCorner;
		}
	};

public:
	explicit StubNativeAreaImpl(QObject *parent = nullptr);

protected:
	bool eventFilter(QObject *obj, QEvent *evt);

	bool onMouseButtonPressed(QMouseEvent *evt);
	bool onMouseButtonReleased(QMouseEvent *evt);
	bool onMouseButtonDblClicked(QMouseEvent *evt);
	bool onMouseMoved(QMouseEvent* evt);

private:
	Area::Type getNativeAreaType(const QPoint& pos) const;
	QPoint calcNewWindowPosition(const QPoint &mousePos) const;
	QRect calcNewWindowGeometry() const;

private:
	QPoint m_mousePressPos;
	bool m_resizeActive = false;
	Area::Type m_resizeAreaType = Area::Client;
};
