#pragma once

#include <QQuickWindow>

class CustomWindowImpl : public QObject {
	Q_OBJECT

public:
	static CustomWindowImpl* create(QObject* parent = nullptr);

	virtual QQuickWindow* childWindow() const = 0;
	virtual void setChildWindow(QQuickWindow* window) = 0;

	virtual QWindow* window() const = 0;

	virtual int x() const = 0;
	virtual void setX(int x) = 0;

	virtual int y() const = 0;
	virtual void setY(int y) = 0;

	virtual int height() const = 0;
	virtual void setHeight(int value) = 0;

	virtual int width() const = 0;
	virtual void setWidth(int value) = 0;

	virtual int minimumWidth() const;
	virtual void setMinimumWidth(int width);

	virtual int minimumHeight() const;
	virtual void setMinimumHeight(int height);

	virtual int maximumWidth() const;
	virtual void setMaximumWidth(int width);

	virtual int maximumHeight() const;
	virtual void setMaximumHeight(int height);

	virtual QString title() const = 0;
	virtual void setTitle(const QString& title) = 0;

	int borderWidth() const;
	void setBorderWidth(int value);

	int titleHeight() const;
	void setTitleHeight(int value);

	QRect titleButtonsArea() const;
	void setTitleButtonsArea(const QRect& rect);

	virtual QWindow::Visibility visibility() const;
	virtual void setVisibility(const QWindow::Visibility& visibility) = 0;

	void setGeometry(const QRect& newGeometry);
	QRect geometry() const;

	virtual QVariantMap saveWindowState();
	virtual void restoreWindowState(const QVariantMap& data);

public slots:
	virtual void raise();
	virtual void close();

signals:
	void childWindowChanged(QQuickWindow* window);
	void xChanged(int x);
	void yChanged(int y);
	void heightChanged(int height);
	void widthChanged(int width);
	void minimumWidthChanged(int width);
	void minimumHeightChanged(int height);
	void maximumWidthChanged(int width);
	void maximumHeightChanged(int height);
	void titleChanged(const QString& title);
	void borderWidthChanged(int borderWidth);
	void titleHeightChanged(int titleHeight);
	void visibilityChanged(const QWindow::Visibility& visibility);
	void closing();
	void titleButtonsAreaChanged(const QRect& rect);
	void titleAreaClicked();

protected:
	explicit CustomWindowImpl(QObject *parent);
	static int validateValue(int value, int min, int max);

	bool m_isClosed = true;
	int m_borderWidth = 0;
	int m_titleHeight = 0;
	QRect m_buttonsArea;
	QSize m_minimumSize;
	QSize m_maximumSize;
	QWindow::Visibility m_visibility = QWindow::Hidden;

	static constexpr auto rectKey = "rect";
	static constexpr auto geometryKey = "geometry";
	static constexpr auto visibilityKey = "visibility";
};
