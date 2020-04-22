#pragma once

#include <QAbstractListModel>

class QmlListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
	static void RegisterType(const char* uri);

	explicit QmlListModel(QObject* parent = nullptr);

    Q_INVOKABLE virtual void reset() final;
    Q_INVOKABLE bool setProperty(int idx, const QString& property, const QVariant& value);
    Q_INVOKABLE QVariant getProperty(int idx, const QString& property) const;
    Q_INVOKABLE QString roleName(int role) const;
    bool isValid(const QModelIndex& idx) const;
    bool isValid(int idx) const;

signals:
    void countChanged(int count);

protected:
    virtual void resetModelData() = 0;
    void notifyRowDataChanged(int row, const QVector<int>& roles = {});
    void notifyRowsDataChanged(int row_from, int row_to, const QVector<int>& roles = {});
};
