#pragma once

#include <QObject>

class SizeProvider final : public QObject {
    Q_OBJECT

public:
    static void RegisterType();
    static qreal scaledMetric(qreal m);
    static int scaledMetric(int m);

    Q_INVOKABLE int metric(int m) const;
    Q_INVOKABLE qreal metric(qreal m) const;

private:
    explicit SizeProvider(QObject* parent = nullptr);

    static int baseDotsPerInch();
    static qreal pixelRatio();
};
