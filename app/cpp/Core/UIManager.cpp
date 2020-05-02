#include "UIManager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QLoggingCategory>

#include "UIController.h"

#include "Core/ComputorCreateController.h"

#include "Models/InputModel.h"
#include "Models/ExpressionList.h"
#include "Models/FunctionList.h"

namespace L {
Q_LOGGING_CATEGORY(ui_manager, "app.ui.manager", QtInfoMsg);
} // end namespace L

UIManager::UIManager() {}

UIManager::~UIManager() = default;

void UIManager::initEngine() {
	m_uiController = std::make_unique<UIController>();
	m_engine = std::make_unique<QQmlApplicationEngine>();

	setupUiModels();

	auto* rootContext = m_engine->rootContext();
	rootContext->setContextProperty("UIController", m_uiController.get());
	rootContext->setContextProperty("InputModel", m_inputModel.get());
	rootContext->setContextProperty("ExpressionList", m_expressionList.get());
	rootContext->setContextProperty("FunctionList", m_functionList.get());
	m_engine->load(QUrl(QStringLiteral("qrc:/main.qml")));

	qCInfo(L::ui_manager) << "UI has been initialized";
}

void UIManager::setupUiModels() {
	m_inputModel = std::make_unique<InputModel>();
	m_expressionList = std::make_unique<ExpressionList>();
	m_functionList = std::make_unique<FunctionList>();
	m_createController = std::make_unique<ComputorCreateController>();

	QObject::connect(m_inputModel.get(), &InputModel::expressionInserted, m_createController.get(), &ComputorCreateController::createExpression);
	QObject::connect(m_inputModel.get(), &InputModel::functionInserted, m_createController.get(), &ComputorCreateController::createFunction);
	QObject::connect(m_createController.get(), &ComputorCreateController::expressionModelCreated, m_expressionList.get(), &ExpressionList::add);
	QObject::connect(m_createController.get(), &ComputorCreateController::functionModelCreated, m_functionList.get(), &FunctionList::add);
}
