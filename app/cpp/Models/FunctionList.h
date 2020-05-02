#pragma once

#include "FunctionModel.h"

#include "qmlplugins/QmlListModel.h"

class FunctionList : public QmlListModel {
	Q_OBJECT

public:
	enum Role {
		FunctionString = Qt::UserRole + 1,
		ErrorColumn,
		ErrorString
	};
	Q_ENUM(Role)

	explicit FunctionList(QObject* parent = nullptr);

	QVariant data(const QModelIndex& index, int role) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	int rowCount(const QModelIndex& = {}) const override;
	QHash<int, QByteArray> roleNames() const override;
	void add(const FunctionModel::Ptr& model);
	Q_INVOKABLE void remove(int index);

private:
	void resetModelData() override;

	FunctionModels m_funModels;
};
