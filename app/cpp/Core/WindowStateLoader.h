#pragma once

#include <QObject>
#include <QVariantMap>

class WindowStateLoader : public QObject {
	Q_OBJECT

public:
	static void RegisterType(const char* uri);

	explicit WindowStateLoader(QObject* parent = nullptr);

public slots:
	QVariantMap restoreWindowState();
	void saveWindowState(const QVariantMap& data);
};
