#include "WinCustomWindowImpl.h"

#include <QCoreApplication>
#include <QLoggingCategory>
#include <QThread>

#include <dwmapi.h>
#include <Windowsx.h>

namespace {

namespace L {
Q_LOGGING_CATEGORY(customWindow, "app.custom_window.win", QtInfoMsg)
} // end namespace L

constexpr auto windowClassName = TEXT("AppCustomWindow");
const DWORD aeroStyle = WS_OVERLAPPEDWINDOW;
const DWORD basicStyle = WS_POPUP;

DWORD windowStyle() {
	BOOL cmpzEnabled = false;
	DwmIsCompositionEnabled(&cmpzEnabled);

	if (cmpzEnabled) {
		return aeroStyle;
	}

	return basicStyle;
}

HICON convertToHIcon(const QIcon& icon, int width, int height) {
	if (!icon.isNull()) {
		const QPixmap pm = icon.pixmap(icon.actualSize(QSize(width, height)));
		if (!pm.isNull()) {
			return QtWin::toHICON(pm);
		}
	}

	return 0;
}

QSize sizeOfRect(const RECT& rect) {
	return QSize(rect.right -rect.left, rect.bottom - rect.top);
}

QRect rectFromRECT(const RECT& rect) {
	return QRect(QPoint(rect.left, rect.top), sizeOfRect(rect));
}

RECT RECTfromQRect(const QRect& rect) {
	const int x = rect.left();
	const int y = rect.top();
	RECT result = {x, y, x + rect.width(), y + rect.height()};
	return result;
}

RECT getMonitorRect(HMONITOR monitor) {
	MONITORINFOEX info;
	info.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(monitor, &info);
	return info.rcMonitor;
}

int marginsWidth(const QMargins& margins) {
	return margins.left() + margins.right();
}

int marginsHeight(const QMargins& margins) {
	return margins.top() + margins.bottom();
}

QWindow* createBorderlessWindow() {
	WNDCLASSEX wcx = {};
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.hInstance = GetModuleHandle(0);
	wcx.lpfnWndProc = WinCustomWindowImpl::WndProc;
	wcx.lpszClassName = windowClassName;
	WindowsContext::instance().registerWindowClass(&wcx);

	auto hwnd = CreateWindow(windowClassName, windowClassName, windowStyle(), 0, 0, 0, 0, 0, 0, wcx.hInstance, nullptr);
	return QWindow::fromWinId(reinterpret_cast<WId>(hwnd));
}

QWindow::Visibility visibilityFromSWCmd(UINT swCmd) {
	switch (swCmd) {
	case SW_SHOWNORMAL:
	case SW_SHOWNOACTIVATE:
	case SW_RESTORE:
		return QWindow::Visibility::Windowed;
	case SW_SHOWMAXIMIZED:
		return QWindow::Visibility::Maximized;
	case SW_MINIMIZE:
	case SW_SHOWMINIMIZED:
	case SW_SHOWMINNOACTIVE:
	case SW_FORCEMINIMIZE:
		return QWindow::Visibility::Minimized;
	}

	return QWindow::Visibility::AutomaticVisibility;
}

} // end anonymous namespace

WindowsContext& WindowsContext::instance() {
	static WindowsContext instance;
	return instance;
}

WindowsContext::~WindowsContext() {
	unregisterAllClasses();
}

void WindowsContext::registerWindowClass(const WNDCLASSEX* wcx) {
	if (!wcx || m_registeredClasses.contains(wcx->lpszClassName)) {
		return;
	}

	if (!RegisterClassEx(wcx)) {
		qWarning(L::customWindow) << "Failed to register window class " << wcx->lpszClassName;
	} else {
		m_registeredClasses.insert(wcx->lpszClassName);
	}
}

void WindowsContext::unregisterAllClasses() {
	const auto appInstance = static_cast<HINSTANCE>(GetModuleHandle(0));

	for (const auto className : m_registeredClasses) {
		if (!UnregisterClass(className, appInstance)) {
			qWarning(L::customWindow) << "Failed to unregister window class " << className;
		}
	}
}

LRESULT CALLBACK WinCustomWindowImpl::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	const auto value = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	auto window = reinterpret_cast<WinCustomWindowImpl*>(value);

	if (!window) {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	switch (message) {
	case WM_NCCALCSIZE: {
		BOOL cmpEnabled = false;
		DwmIsCompositionEnabled(&cmpEnabled);

		if (!cmpEnabled) {
			return DefWindowProc(hWnd, message, wParam, lParam);
		} else if (wParam == TRUE) {
			NCCALCSIZE_PARAMS *params = (NCCALCSIZE_PARAMS *) lParam;
			WINDOWPLACEMENT wp;
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wp);
			if (visibilityFromSWCmd(wp.showCmd) == QWindow::Visibility::Maximized) {
				// Workaround that needed because of Windows spesific behaviour.
				// When window is maximizing this function receive rectangle that overlapping the display, so window become visible on other displays.
				// More info https://devblogs.microsoft.com/oldnewthing/20120326-00/?p=8003
				int padding = GetSystemMetrics(SM_CXPADDEDBORDER);
				int borderWidth = GetSystemMetrics(SM_CXSIZEFRAME) + padding;
				int borderHeight = GetSystemMetrics(SM_CYSIZEFRAME) + padding;
				params->rgrc[0].left += borderWidth;
				params->rgrc[0].top += borderHeight;
				params->rgrc[0].right -= borderWidth;
				params->rgrc[0].bottom -= borderHeight;
			}
		}

		return 0;
	}

	case WM_DESTROY: {
		return 0;
	}

	case WM_CLOSE: {
		window->close();
		return 0;
	}

	case WM_ENTERSIZEMOVE: {
		window->m_isResizingOrMoving = true;
		return 0;
	}

	case WM_EXITSIZEMOVE: {
		window->m_isResizingOrMoving = false;
		return 0;
	}

	case WM_SIZE: {
		WINDOWPLACEMENT wp;
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hWnd, &wp);
		window->updateVisibility(visibilityFromSWCmd(wp.showCmd));

		if (wParam != SIZE_RESTORED && wParam != SIZE_MAXIMIZED) {
			return 0;
		}

		WORD newHeight = validateValue(
		    GET_Y_LPARAM(lParam),
		    window->m_minimumSize.height(),
		    window->m_maximumSize.height()
		);

		WORD newWidth = validateValue(
		    GET_X_LPARAM(lParam),
		    window->m_minimumSize.width(),
		    window->m_maximumSize.width()
		);

		if (window->m_childWindow) {
			window->m_childWindow->setHeight(newHeight);
			window->m_childWindow->setWidth(newWidth);
		}

		return 0;
	}

	case WM_MOVE: {
		window->updatePosition(QPoint(static_cast<int>(GET_X_LPARAM(lParam)), static_cast<int>(GET_Y_LPARAM(lParam))));
		return 0;
	}

	case WM_GETMINMAXINFO: {
		const auto lpMMI = reinterpret_cast<LPMINMAXINFO>(lParam);
		if (window->m_minimumSize.width() > 0) {
			lpMMI->ptMinTrackSize.x = window->m_minimumSize.width();
		}

		if (window->m_minimumSize.height() > 0) {
			lpMMI->ptMinTrackSize.y = window->m_minimumSize.height();
		}

		if (window->m_maximumSize.width() > 0) {
			lpMMI->ptMaxTrackSize.x = window->m_maximumSize.width();
		}

		if (window->m_maximumSize.height() > 0) {
			lpMMI->ptMaxTrackSize.y = window->m_maximumSize.height();
		}

		return 0;
	}

	case WM_NCHITTEST: {		
		QRect windowArea(0, 0, window->width(), window->height());
		const auto point = QPoint(static_cast<short>(GET_X_LPARAM(lParam)), static_cast<short>(GET_Y_LPARAM(lParam)));
		const auto mousePos = window->m_window->mapFromGlobal(point);
		if (windowArea.contains(mousePos, true)) {
			return window->getCustomWindowType(mousePos, window->width(), window->height());
		}

		return HTNOWHERE;
	}

	case WM_NCLBUTTONDOWN: {
		emit window->titleAreaClicked();
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	case WM_SETFOCUS: {
		if (window->m_childWindow) {
			SetFocus(reinterpret_cast<HWND>(window->m_childWindow->winId()));
		}

		return 0;
	}

	case WM_DWMCOMPOSITIONCHANGED : {
		auto style = windowStyle();
		if (IsWindowVisible(hWnd)) {
			style |= WS_VISIBLE;
		}

		SetWindowLongPtr(hWnd, GWL_STYLE, style);

		SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		return 0;
	}

	default: break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

WinCustomWindowImpl::WinCustomWindowImpl(QObject* parent)
    : CustomWindowImpl(parent)
    , m_window(createBorderlessWindow())
{
	qApp->installNativeEventFilter(this);
	SetWindowLongPtr(handle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

WinCustomWindowImpl::~WinCustomWindowImpl() {
	destroyIcon();
	m_childWindow = nullptr;
	m_window->deleteLater();

	if (IsWindow(handle())) {
		DestroyWindow(handle());
	}
}

QQuickWindow* WinCustomWindowImpl::childWindow() const {
	return m_childWindow;
}

void WinCustomWindowImpl::setChildWindow(QQuickWindow* window) {
	if (window != m_childWindow && window) {
		if (m_childWindow) {
			m_childWindow->setParent(nullptr);
			for(const auto& connection : m_connections) {
				disconnect(connection);
			}

			m_connections.clear();
		}

		m_childWindow = window;
		m_childWindow->setParent(m_window);
		m_isChildWindowReady = false;
		m_savedGeometry = geometry();
		m_savedVisibility = m_visibility;
		m_connections << connect(m_childWindow, &QQuickWindow::widthChanged, this, &WinCustomWindowImpl::widthChanged);
		m_connections << connect(m_childWindow, &QQuickWindow::heightChanged, this, &WinCustomWindowImpl::heightChanged);
		// Old connection style used as a workaround. QQuickCloseEvent is an 'incomplete type'.
		// https://bugreports.qt.io/browse/QTBUG-55722
		m_connections << connect(m_childWindow, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(close()));
		m_connections << connect(m_childWindow, &QQuickWindow::beforeSynchronizing, this, &WinCustomWindowImpl::beforeSynchronizing, Qt::DirectConnection);
		setWindowsIcon(m_childWindow->icon());
		m_childWindow->setGeometry(0, 0, width(), height());
		emit childWindowChanged(m_childWindow);
	}
}

QWindow* WinCustomWindowImpl::window() const {
	return m_window;
}

int WinCustomWindowImpl::x() const {
	return m_window->x() - m_window->frameMargins().left();
}

void WinCustomWindowImpl::setX(int x) {
	if (m_isResizingOrMoving) {
		return;
	}

	m_window->setX(x + m_window->frameMargins().left());
}

int WinCustomWindowImpl::y() const {
	return m_window->y() - m_window->frameMargins().top();
}

void WinCustomWindowImpl::setY(int y) {
	if (m_isResizingOrMoving) {
		return;
	}

	m_window->setY(y + m_window->frameMargins().top());
}

int WinCustomWindowImpl::height() const {
	return m_window->height() + marginsHeight(m_window->frameMargins());
}

void WinCustomWindowImpl::setHeight(int value) {
	const auto newHeight = validateValue(value, m_minimumSize.height(), m_maximumSize.height());
	if (height() == newHeight || m_isResizingOrMoving) {
		return;
	}

	m_window->setHeight(newHeight - marginsHeight(m_window->frameMargins()));
}

int WinCustomWindowImpl::width() const {
	return m_window->width() + marginsWidth(m_window->frameMargins());
}

void WinCustomWindowImpl::setWidth(int value) {
	const auto newWidth = validateValue(value, m_minimumSize.width(), m_maximumSize.width());
	if (width() == newWidth || m_isResizingOrMoving) {
		return;
	}

	m_window->setWidth(newWidth - marginsWidth(m_window->frameMargins()));
}

QString WinCustomWindowImpl::title() const {
	return m_window->title();
}

void WinCustomWindowImpl::setTitle(const QString& title) {
	if (m_window->title() == title) {
		return;
	}

	m_window->setTitle(title);
	emit titleChanged(title);
}

void WinCustomWindowImpl::setVisibility(const QWindow::Visibility& visibility) {
	if (m_visibility == visibility) {
		return;
	}

	m_visibility = visibility;

	showWindow(m_visibility);
	if (m_isChildWindowReady) {
		emit visibilityChanged(m_visibility);
	}
}

HWND WinCustomWindowImpl::handle() {
	return reinterpret_cast<HWND>(m_window->winId());
}

void WinCustomWindowImpl::setWindowsIcon(const QIcon& icon) {
	destroyIcon();

	m_smallIcon = convertToHIcon(icon, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));
	m_bigIcon = convertToHIcon(icon, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));

	if (!m_smallIcon) {
		return;
	}

	SendMessage(handle(), WM_SETICON, 0 /* ICON_SMALL */, LPARAM(m_smallIcon));
	if (m_bigIcon) {
		SendMessage(handle(), WM_SETICON, 1 /* ICON_BIG */, LPARAM(m_bigIcon));
	} else {
		SendMessage(handle(), WM_SETICON, 1 /* ICON_BIG */, LPARAM(m_smallIcon));
	}
}

QVariantMap WinCustomWindowImpl::saveWindowState() {
	const auto monitor = MonitorFromWindow(handle(), MONITOR_DEFAULTTONULL);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(handle(), &wp);
	QVariantMap data;

	if (monitor != NULL) {
		data.insert(rectKey, rectFromRECT(getMonitorRect(monitor)));
	} else {
		data.insert(rectKey, QVariant());
	}

	if (m_visibility == QWindow::Visibility::Maximized) {
		data.insert(geometryKey, rectFromRECT(wp.rcNormalPosition));
	} else {
		data.insert(geometryKey, QRect(QPoint(x(), y()), QSize(width(), height())));
	}

	data.insert(visibilityKey, m_visibility);
	return data;
}

void WinCustomWindowImpl::restoreWindowState(const QVariantMap& data) {
	const auto hasRect = data.contains(rectKey) && data.value(rectKey).canConvert<QRect>();
	const auto hasGeometry = data.contains(geometryKey) && data.value(geometryKey).canConvert<QRect>();
	const auto hasVisibility = data.contains(visibilityKey) && data.value(visibilityKey).canConvert<QWindow::Visibility>();

	if (!hasRect || !hasGeometry || !hasVisibility) {
		return;
	}

	const auto geometryRect = data.value(geometryKey).toRect();
	const auto rect = RECTfromQRect(geometryRect);
	const auto monitor = MonitorFromRect(&rect, MONITOR_DEFAULTTONULL);
	if (monitor == NULL) {
		return;
	}

	const auto monitorRect = data.value(rectKey).toRect();
	if (monitorRect != rectFromRECT(getMonitorRect(monitor))) {
		return;
	}

	setGeometry(geometryRect);
	setVisibility(data.value(visibilityKey).value<QWindow::Visibility>());
}

void WinCustomWindowImpl::raise() {
	if (m_isClosed) {
		SetWindowLongPtr(handle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	}

	CustomWindowImpl::raise();
	ShowWindow(handle(), SW_SHOW);
	if (m_childWindow) {
		m_childWindow->show();
	}
	BringWindowToTop(handle());
}

void WinCustomWindowImpl::close() {
	if (m_isClosed) {
		return;
	}

	SetWindowLongPtr(handle(), GWLP_USERDATA, 0);
	CustomWindowImpl::close();
	emit closing();

	m_window->close();
}

void WinCustomWindowImpl::hide() {
	ShowWindow(handle(), SW_HIDE);
	if (m_childWindow) {
		m_childWindow->hide();
	}
}

bool WinCustomWindowImpl::nativeEventFilter(const QByteArray& eventType, void *message, long* result) {
	if (eventType != "windows_generic_MSG") {
		return false;
	}

	const auto pMSG = static_cast<MSG*>(message);
	if (!pMSG) {
		return false;
	}

	if (!m_childWindow) {
		return false;
	}

	switch (pMSG->message) {
	case WM_NCHITTEST: return onNCHitTest(pMSG, result);
	default: return false;
	}
}

bool WinCustomWindowImpl::onNCHitTest(MSG* message, long* result) {
	if (reinterpret_cast<HWND>(m_childWindow->winId()) != message->hwnd) {
		return false;
	}

	const QVector<long> transparentHTTypes = {
	    HTTOPLEFT,
	    HTTOPRIGHT,
	    HTBOTTOMLEFT,
	    HTBOTTOMRIGHT,
	    HTLEFT,
	    HTRIGHT,
	    HTTOP,
	    HTBOTTOM,
	    HTCAPTION
	};

	LRESULT lRet = 0;
	lRet = hitTestNCA(message->hwnd, message->wParam, message->lParam);

	if (transparentHTTypes.contains(lRet)) {
		*result = HTTRANSPARENT;
		return true;
	} else {
		*result = lRet;
		return false;
	}
}

int WinCustomWindowImpl::getCustomWindowType(const QPoint& position, int width, int height) const {
	if (titleButtonsArea().contains(position)) {
		return HTCLIENT;
	}

	if (m_visibility != QWindow::Visibility::Maximized) {
		bool matchLeftBorder = position.x() < borderWidth();
		bool matchRightBorder = position.x() > width - borderWidth();
		bool matchTopBorder = position.y() < borderWidth();
		bool matchBottomBorder = position.y() > height - borderWidth();

		if (matchLeftBorder && matchTopBorder) {
			return HTTOPLEFT;
		}

		if (matchRightBorder && matchTopBorder) {
			return HTTOPRIGHT;
		}

		if (matchBottomBorder && matchLeftBorder) {
			return HTBOTTOMLEFT;
		}

		if (matchBottomBorder && matchRightBorder) {
			return HTBOTTOMRIGHT;
		}

		if (matchLeftBorder) {
			return HTLEFT;
		}

		if (matchRightBorder) {
			return HTRIGHT;
		}

		if (matchTopBorder) {
			return HTTOP;
		}

		if (matchBottomBorder) {
			return HTBOTTOM;
		}
	}

	if (position.y() < titleHeight()) {
		return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT WinCustomWindowImpl::hitTestNCA(HWND hWnd, WPARAM, LPARAM lParam) {
	Q_ASSERT(m_childWindow);

	HWND windowHandle = reinterpret_cast<HWND>(m_childWindow->winId());
	POINT ptMouse = { static_cast<int>(GET_X_LPARAM(lParam)), static_cast<int>(GET_Y_LPARAM(lParam)) };

	if (hWnd != windowHandle) {
		return HTNOWHERE;
	}

	QRect windowArea(0, 0, m_childWindow->width(), m_childWindow->height());
	QPoint mousePos = m_childWindow->mapFromGlobal(QPoint(ptMouse.x, ptMouse.y));
	if (windowArea.contains(mousePos, true)) {
		return getCustomWindowType(mousePos, m_childWindow->width(), m_childWindow->height());
	}

	return HTNOWHERE;
}

void WinCustomWindowImpl::showWindow(QWindow::Visibility newVisibility) {
	switch (newVisibility) {
	case QWindow::Visibility::Hidden:
		hide();
		break;
	case QWindow::Visibility::AutomaticVisibility:
		ShowWindow(handle(), SW_SHOWNORMAL);
		break;
	case QWindow::Visibility::Windowed:
		ShowWindow(handle(), SW_RESTORE);
		break;
	case QWindow::Visibility::Minimized:
		ShowWindow(handle(), SW_SHOWMINIMIZED);
		break;
	case QWindow::Visibility::FullScreen:
	case QWindow::Visibility::Maximized: {
		ShowWindow(handle(), SW_SHOWMAXIMIZED);
		break;
	}
	}
}

void WinCustomWindowImpl::updateVisibility(const QWindow::Visibility& newVisibility) {
	if (newVisibility == QWindow::Visibility::AutomaticVisibility || m_visibility == newVisibility) {
		return;
	}

	m_visibility = newVisibility;
	if (m_isChildWindowReady) {
		emit visibilityChanged(m_visibility);
	}
}

void WinCustomWindowImpl::updatePosition(const QPoint& pos) {
	if (!m_isChildWindowReady) {
		return;
	}

	emit xChanged(pos.x());
	emit yChanged(pos.y());
}

void WinCustomWindowImpl::destroyIcon() {
	if (m_bigIcon) {
		DestroyIcon(m_bigIcon);
		m_bigIcon = 0;
	}

	if (m_smallIcon) {
		DestroyIcon(m_smallIcon);
		m_smallIcon = 0;
	}
}

void WinCustomWindowImpl::beforeSynchronizing() {
	m_isChildWindowReady = true;

	if (x() != m_savedGeometry.x()) {
		emit xChanged(x());
	}

	if (y() != m_savedGeometry.y()) {
		emit yChanged(y());
	}

	if (width() != m_savedGeometry.width()) {
		emit widthChanged(width());
	}

	if (height() != m_savedGeometry.height()) {
		emit heightChanged(height());
	}

	if (m_savedVisibility != m_visibility) {
		showWindow(m_visibility);
		emit visibilityChanged(m_visibility);
	}

	disconnect(m_childWindow, &QQuickWindow::beforeSynchronizing, this, &WinCustomWindowImpl::beforeSynchronizing);
}
