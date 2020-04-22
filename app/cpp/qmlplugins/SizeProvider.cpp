#include "SizeProvider.h"

#include <QQmlEngine>
#include <QGuiApplication>
#include <QScreen>

void SizeProvider::RegisterType() {
    static const auto instantiator = [](QQmlEngine* engine, QJSEngine*) -> QObject* {
        return new SizeProvider(engine);
    };

	qmlRegisterSingletonType<SizeProvider>("SizeProvider", 1, 0, "SizeProvider", instantiator);
}

qreal SizeProvider::scaledMetric(qreal m) {
    return m * pixelRatio();
}

int SizeProvider::scaledMetric(int m) {
    return static_cast<int>(m * pixelRatio());
}

SizeProvider::SizeProvider(QObject* parent) : QObject(parent) {}

int SizeProvider::metric(int m) const {
    return scaledMetric(m);
}

qreal SizeProvider::metric(qreal m) const {
    return scaledMetric(m);
}

int SizeProvider::baseDotsPerInch() {
    return 96;
}

qreal SizeProvider::pixelRatio() {
    if (!QGuiApplication::instance()) return 1.;
    return QGuiApplication::primaryScreen()->logicalDotsPerInch() / baseDotsPerInch();
}

#include "moc_SizeProvider.cpp"
