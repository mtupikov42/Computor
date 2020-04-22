#pragma once

#include "CustomWindowImpl.h"

#include <QAbstractNativeEventFilter>
#include <QWindow>
#include <QPointer>
#include <QtWinExtras/QtWin>
#include <QSet>

class WindowsContext {
public:
	static WindowsContext& instance();
	~WindowsContext();

	void registerWindowClass(const WNDCLASSEX* wcx);
	void unregisterAllClasses();

private:
	WindowsContext() = default;

	QSet<LPCSTR> m_registeredClasses;
};

class WinCustomWindowImpl : public CustomWindowImpl, QAbstractNativeEventFilter {
	Q_OBJECT

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	explicit WinCustomWindowImpl(QObject* parent = nullptr);
	~WinCustomWindowImpl();

	QQuickWindow* childWindow() const override;
	void setChildWindow(QQuickWindow* window) override;

	QWindow* window() const override;

	int x() const override;
	void setX(int x) override;

	int y() const override;
	void setY(int y) override;

	int height() const override;
	void setHeight(int value) override;

	int width() const override;
	void setWidth(int value) override;

	QString title() const override;
	void setTitle(const QString& title) override;

	void setVisibility(const QWindow::Visibility& visibility) override;

	HWND handle();
	void setWindowsIcon(const QIcon& icon);

	QVariantMap saveWindowState() override;
	void restoreWindowState(const QVariantMap& data) override;

public slots:
	void raise() override;
	void close() override;
	void hide();

protected:
	bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;

private:
	bool onNCHitTest(MSG* message, long* result);
	int getCustomWindowType(const QPoint& position, int width, int height) const;
	void showWindow(QWindow::Visibility newVisibility);
	void updateVisibility(const QWindow::Visibility& newVisibility);
	void updatePosition(const QPoint& pos);
	void destroyIcon();
	void beforeSynchronizing();
	LRESULT hitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam);

	QList<QMetaObject::Connection> m_connections;
	QPointer<QWindow> m_window = nullptr;
	QPointer<QQuickWindow> m_childWindow = nullptr;
	HICON m_smallIcon = 0;
	HICON m_bigIcon = 0;
	QRect m_savedGeometry;
	QWindow::Visibility m_savedVisibility;
	bool m_isResizingOrMoving = false;
	bool m_isChildWindowReady = false;
};
