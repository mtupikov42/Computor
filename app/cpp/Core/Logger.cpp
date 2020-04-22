#include "Logger.h"

#include <QDateTime>
#include <QTextStream>
#include <QDir>
#include <QDebug>

#include <StandardPaths.h>

namespace {

QtMessageHandler prevMessageHandler = nullptr;
Logger* currentMessagehandler = nullptr;

} // end anonymous namespace

Logger::Logger(QObject* parent) : QObject(parent) {
	const auto fileName = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH-mm-ss") + ".log";
	const auto filePath = StandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QDir::separator() + logsDirName();
	const auto fullPath = filePath + QDir::separator() + fileName;

	m_logFile.setFileName(fullPath);
	m_logFile.open(QIODevice::WriteOnly);

	const auto format("%{time} "
	                  "%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}"
	                  "%{if-fatal}F%{endif} %{if-category}%{category}: %{endif}%{message}");

	qSetMessagePattern(format);
	prevMessageHandler = qInstallMessageHandler(messageHandler);
	currentMessagehandler = this;
}

QString Logger::logsDirName() {
	return "logs";
}

Logger::~Logger() {
	m_logFile.close();
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
	const auto formattedMsg = qFormatLogMessage(type, context, msg);

	Q_ASSERT(currentMessagehandler && prevMessageHandler);

	QTextStream stream(&currentMessagehandler->m_logFile);
	stream << formattedMsg << endl;

	prevMessageHandler(type, context, msg);
}
