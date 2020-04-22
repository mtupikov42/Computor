#pragma once

#include <QPointer>
#include <QQuickItem>
#include <QWindow>

class CustomWindowImpl;

class CustomWindow : public QQuickItem {
	Q_OBJECT
	Q_PROPERTY(QQuickWindow *childWindow READ childWindow WRITE setChildWindow NOTIFY childWindowChanged)
	Q_PROPERTY(QWindow* window READ window CONSTANT)
	Q_PROPERTY(int windowX READ windowX WRITE setWindowX NOTIFY windowXChanged)
	Q_PROPERTY(int windowY READ windowY WRITE setWindowY NOTIFY windowYChanged)
	Q_PROPERTY(int windowHeight READ windowHeight WRITE setWindowHeight NOTIFY windowHeightChanged)
	Q_PROPERTY(int windowWidth READ windowWidth WRITE setWindowWidth NOTIFY windowWidthChanged)
	Q_PROPERTY(int minimumWidth READ minimumWidth WRITE setMinimumWidth NOTIFY minimumWidthChanged)
	Q_PROPERTY(int minimumHeight READ minimumHeight WRITE setMinimumHeight NOTIFY minimumHeightChanged)
	Q_PROPERTY(int maximumWidth READ maximumWidth WRITE setMaximumWidth NOTIFY maximumWidthChanged)
	Q_PROPERTY(int maximumHeight READ maximumHeight WRITE setMaximumHeight NOTIFY maximumHeightChanged)
	Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
	Q_PROPERTY(int titleHeight READ titleHeight WRITE setTitleHeight NOTIFY titleHeightChanged)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
	Q_PROPERTY(QWindow::Visibility visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged)
	Q_PROPERTY(QRect titleButtonsArea READ titleButtonsArea WRITE setTitleButtonsArea NOTIFY titleButtonsAreaChanged)

public:
	static void RegisterType(const char* uri);

	explicit CustomWindow(QQuickItem* parent = nullptr);

	QQuickWindow* childWindow() const;
	void setChildWindow(QQuickWindow* window);

	QWindow* window() const;

	int windowX() const;
	void setWindowX(int windowX);

	int windowY() const;
	void setWindowY(int windowY);

	int windowHeight() const;
	void setWindowHeight(int value);

	int windowWidth() const;
	void setWindowWidth(int value);

	int minimumWidth() const;
	void setMinimumWidth(int windowWidth);

	int minimumHeight() const;
	void setMinimumHeight(int windowHeight);

	QString title();
	void setTitle(const QString& title);

	int borderWidth() const;
	void setBorderWidth(int value);

	int titleHeight() const;
	void setTitleHeight(int value);

	QRect titleButtonsArea() const;
	void setTitleButtonsArea(const QRect& rect);

	QWindow::Visibility visibility() const;
	void setVisibility(const QWindow::Visibility& visibility);

	int maximumWidth() const;
	void setMaximumWidth(int maximumWidth);

	int maximumHeight() const;
	void setMaximumHeight(int maximumHeight);

	Q_INVOKABLE void setFlag(Qt::WindowType flag);
	Q_INVOKABLE void restoreWindowState(const QVariantMap& data);
	Q_INVOKABLE QVariantMap saveWindowState();

public slots:
	void raise();
	void close();

signals:
	void childWindowChanged(QQuickWindow* window);
	void windowXChanged(int windowX);
	void windowYChanged(int windowY);
	void windowHeightChanged(int windowHeight);
	void windowWidthChanged(int windowWidth);
	void minimumWidthChanged(int windowWidth);
	void minimumHeightChanged(int windowHeight);
	void maximumWidthChanged(int maximumWidth);
	void maximumHeightChanged(int maximumHeight);
	void titleChanged(const QString& title);
	void borderWidthChanged(int borderWidth);
	void titleHeightChanged(int titleHeight);
	void visibilityChanged(const QWindow::Visibility& visibility);
	void closing();
	void titleButtonsAreaChanged(const QRect &rect);
	void titleAreaClicked();

protected:
	CustomWindowImpl *m_pImpl;
};
