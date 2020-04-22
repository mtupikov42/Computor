#pragma once

#include <memory>

class QQmlApplicationEngine;
class UIController;

class UIManager final {
public:
	explicit UIManager();
	~UIManager();

	void initEngine();

private:
	std::unique_ptr<QQmlApplicationEngine> m_engine;
	std::unique_ptr<UIController> m_uiController;
};
