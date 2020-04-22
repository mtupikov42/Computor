#include "UIManager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QLoggingCategory>

#include "UIController.h"

namespace L {
Q_LOGGING_CATEGORY(ui_manager, "app.ui.manager", QtInfoMsg);
} // end namespace L

UIManager::UIManager() {}

UIManager::~UIManager() = default;

void UIManager::initEngine() {
	m_uiController = std::make_unique<UIController>();
	m_engine = std::make_unique<QQmlApplicationEngine>();

	auto* rootContext = m_engine->rootContext();
	rootContext->setContextProperty("UIController", m_uiController.get());
	m_engine->load(QUrl(QStringLiteral("qrc:/main.qml")));

	qCInfo(L::ui_manager) << "UI has been initialized";
}
