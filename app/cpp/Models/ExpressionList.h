#pragma once

#include "ExpressionModel.h"

#include "qmlplugins/QmlListModel.h"

class ExpressionList : public QmlListModel {
	Q_OBJECT

public:
	enum Role {
		Degree = Qt::UserRole + 1,
		ErrorColumn,
		Solutions,
		RawExpressionString,
		InfixString,
		InfixWithParenthesesString,
		PostfixString,
		PrefixString,
		ErrorString
	};
	Q_ENUM(Role)

	explicit ExpressionList(QObject* parent = nullptr);

	QVariant data(const QModelIndex& index, int role) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	int rowCount(const QModelIndex& = {}) const override;
	QHash<int, QByteArray> roleNames() const override;
	void add(const ExpressionModel::Ptr& model);
	Q_INVOKABLE void remove(int index);

private:
	void resetModelData() override;

	ExpressionModels m_expModels;
};
