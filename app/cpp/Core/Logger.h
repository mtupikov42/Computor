#pragma once

#include <QObject>
#include <QFile>

class Logger final : public QObject {
	Q_OBJECT

public:
	explicit Logger(QObject* parent = nullptr);
	~Logger();

	static QString logsDirName();

private:
	static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

	QFile m_logFile;
};
