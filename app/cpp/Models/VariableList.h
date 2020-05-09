#pragma once

#include "qmlplugins/QmlListModel.h"

#include "VariableModel.h"

class VariableList : public QmlListModel {
	Q_OBJECT

public:
	enum Role {
		VarName = Qt::UserRole + 1,
		Result,
		Type,
		ErrorString
	};
	Q_ENUM(Role)

	explicit VariableList(QObject* parent = nullptr);

	QVariant data(const QModelIndex& index, int role) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	int rowCount(const QModelIndex& = {}) const override;
	QHash<int, QByteArray> roleNames() const override;
	const VariableModel::Ptr& at(int index) const;
	void add(const VariableModel::Ptr& model);
	Q_INVOKABLE void remove(int index);
	QMap<QString, QString> validVariablesList() const;

private:
	void resetModelData() override;

	VariableModels m_varModels;
};
