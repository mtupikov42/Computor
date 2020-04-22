#include "UIController.h"

#include <QGuiApplication>

UIController::UIController(QObject* parent) : QObject(parent) {}

void UIController::quit() {
	qApp->quit();
}
