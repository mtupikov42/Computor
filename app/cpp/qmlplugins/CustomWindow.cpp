#include "CustomWindow.h"

#include "CustomWindowImpl.h"

#include <QCoreApplication>

void CustomWindow::RegisterType(const char* uri) {
	qmlRegisterType<CustomWindow>(uri, 1, 0, "CustomWindow");
}

CustomWindow::CustomWindow(QQuickItem *parent)
    : QQuickItem(parent)
    , m_pImpl(CustomWindowImpl::create(this))
{
	connect(this, &CustomWindow::windowChanged, m_pImpl, &CustomWindowImpl::setChildWindow);

	connect(m_pImpl, &CustomWindowImpl::childWindowChanged, this, &CustomWindow::childWindowChanged);
	connect(m_pImpl, &CustomWindowImpl::xChanged, this, &CustomWindow::windowXChanged);
	connect(m_pImpl, &CustomWindowImpl::yChanged, this, &CustomWindow::windowYChanged);
	connect(m_pImpl, &CustomWindowImpl::heightChanged, this, &CustomWindow::windowHeightChanged);
	connect(m_pImpl, &CustomWindowImpl::widthChanged, this, &CustomWindow::windowWidthChanged);
	connect(m_pImpl, &CustomWindowImpl::minimumWidthChanged, this, &CustomWindow::minimumWidthChanged);
	connect(m_pImpl, &CustomWindowImpl::minimumHeightChanged, this, &CustomWindow::minimumHeightChanged);
	connect(m_pImpl, &CustomWindowImpl::titleChanged, this, &CustomWindow::titleChanged);
	connect(m_pImpl, &CustomWindowImpl::borderWidthChanged, this, &CustomWindow::borderWidthChanged);
	connect(m_pImpl, &CustomWindowImpl::titleHeightChanged, this, &CustomWindow::titleHeightChanged);
	connect(m_pImpl, &CustomWindowImpl::visibilityChanged, this, &CustomWindow::visibilityChanged);
	connect(m_pImpl, &CustomWindowImpl::closing, this, &CustomWindow::closing);
	connect(m_pImpl, &CustomWindowImpl::titleButtonsAreaChanged, this, &CustomWindow::titleButtonsAreaChanged);
	connect(m_pImpl, &CustomWindowImpl::titleAreaClicked, this, &CustomWindow::titleAreaClicked);
	connect(qApp, &QCoreApplication::aboutToQuit, m_pImpl, &CustomWindowImpl::close);
}

QQuickWindow* CustomWindow::childWindow() const {
	return m_pImpl->childWindow();
}

void CustomWindow::setChildWindow(QQuickWindow* window) {
	m_pImpl->setChildWindow(window);
}

QWindow* CustomWindow::window() const {
	return m_pImpl->window();
}

int CustomWindow::windowX() const {
	return m_pImpl->x();
}

void CustomWindow::setWindowX(int x) {
	m_pImpl->setX(x);
}

int CustomWindow::windowY() const {
	return m_pImpl->y();
}

void CustomWindow::setWindowY(int y) {
	m_pImpl->setY(y);
}

int CustomWindow::windowHeight() const {
	return m_pImpl->height();
}

void CustomWindow::setWindowHeight(int value) {
	m_pImpl->setHeight(value);
}

int CustomWindow::windowWidth() const {
	return m_pImpl->width();
}

void CustomWindow::setWindowWidth(int value) {
	m_pImpl->setWidth(value);
}

int CustomWindow::minimumWidth() const {
	return m_pImpl->minimumWidth();
}

void CustomWindow::setMinimumWidth(int width) {
	m_pImpl->setMinimumWidth(width);
}

int CustomWindow::minimumHeight() const {
	return m_pImpl->minimumHeight();
}

void CustomWindow::setMinimumHeight(int height) {
	m_pImpl->setMinimumHeight(height);
}

QString CustomWindow::title() {
	return m_pImpl->title();
}

void CustomWindow::setTitle(const QString& title) {
	m_pImpl->setTitle(title);
}

int CustomWindow::borderWidth() const {
	return m_pImpl->borderWidth();
}

void CustomWindow::setBorderWidth(int value) {
	m_pImpl->setBorderWidth(value);
}

int CustomWindow::titleHeight() const {
	return m_pImpl->titleHeight();
}

void CustomWindow::setTitleHeight(int value) {
	m_pImpl->setTitleHeight(value);
}

QRect CustomWindow::titleButtonsArea() const {
	return m_pImpl->titleButtonsArea();
}

void CustomWindow::setTitleButtonsArea(const QRect& rect) {
	m_pImpl->setTitleButtonsArea(rect);
}

QWindow::Visibility CustomWindow::visibility() const {
	return m_pImpl->visibility();
}

void CustomWindow::setVisibility(const QWindow::Visibility& visibility) {
	m_pImpl->setVisibility(visibility);
}

int CustomWindow::maximumWidth() const {
	return m_pImpl->maximumWidth();
}

void CustomWindow::setMaximumWidth(int maximumWidth) {
	m_pImpl->setMaximumWidth(maximumWidth);
}

int CustomWindow::maximumHeight() const {
	return m_pImpl->maximumHeight();
}

void CustomWindow::setMaximumHeight(int maximumHeight) {
	m_pImpl->setMaximumHeight(maximumHeight);
}

void CustomWindow::setFlag(Qt::WindowType flag) {
	window()->setFlag(flag);
}

QVariantMap CustomWindow::saveWindowState() {
	return m_pImpl->saveWindowState();
}

void CustomWindow::restoreWindowState(const QVariantMap& data) {
	m_pImpl->restoreWindowState(data);
}

void CustomWindow::raise() {
	m_pImpl->raise();
}

void CustomWindow::close() {
	m_pImpl->close();
}
