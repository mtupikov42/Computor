#pragma once

#include <QStringList>

#include "qmlplugins/QmlListModel.h"

class InputHistoryList : public QmlListModel {
	Q_OBJECT

public:
	enum Role {
		Model = Qt::UserRole + 1
	};
	Q_ENUM(Role)

	explicit InputHistoryList(QObject* parent = nullptr);

	QVariant data(const QModelIndex& index, int role) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	int rowCount(const QModelIndex& = {}) const override;
	QHash<int, QByteArray> roleNames() const override;
	Q_INVOKABLE void add(const QString& model);

private:
	void resetModelData() override;

	QStringList m_modelData;
};
