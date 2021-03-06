#pragma once

#include <memory>

class QQmlApplicationEngine;
class UIController;
class InputHistoryList;
class InputModel;
class ExpressionList;
class ComputorCreateController;
class FunctionList;
class VariableList;

class UIManager final {
public:
	explicit UIManager();
	~UIManager();

	void initEngine();

private:
	void setupUiModels();

	std::unique_ptr<QQmlApplicationEngine> m_engine;
	std::unique_ptr<UIController> m_uiController;
	std::unique_ptr<InputHistoryList> m_inputHistoryList;
	std::unique_ptr<InputModel> m_inputModel;
	std::unique_ptr<ExpressionList> m_expressionList;
	std::unique_ptr<FunctionList> m_functionList;
	std::unique_ptr<VariableList> m_variableList;
	std::unique_ptr<ComputorCreateController> m_createController;
};
