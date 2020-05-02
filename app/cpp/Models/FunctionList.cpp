#include "FunctionList.h"

#include <QLoggingCategory>

namespace L {
Q_LOGGING_CATEGORY(function_list, "app.function.list", QtInfoMsg);
} // end namespace L

FunctionList::FunctionList(QObject* parent) : QmlListModel(parent) {}

QVariant FunctionList::data(const QModelIndex& index, int role) const {
	if (!isValid(index)) {
		return {};
	}

	const auto idx = static_cast<size_t>(index.row());
	const auto& d = m_funModels[idx];

	switch (role) {
	case Role::FunctionString: return d->toString();
	case Role::ErrorColumn: return d->errorColumn();
	case Role::ErrorString: return d->errorString();
	default:
		qFatal("unhandled role");
		break;
	}

	return {};
}

bool FunctionList::setData(const QModelIndex&, const QVariant&, int) {
	return false;
}

int FunctionList::rowCount(const QModelIndex&) const {
	return static_cast<int>(m_funModels.size());
}

QHash<int, QByteArray> FunctionList::roleNames() const {
	static const decltype(roleNames()) role_names{
		{ Role::FunctionString, "functionStringRole" },
		{ Role::ErrorColumn, "errorColumnRole" },
		{ Role::ErrorString, "errorStringRole" }
	};

	return role_names;
}

void FunctionList::add(const FunctionModel::Ptr& model) {
	beginInsertRows({}, rowCount(), rowCount());
	m_funModels.push_back(model);
	endInsertRows();
}

void FunctionList::remove(int index) {
	if (index < 0 || index >= m_funModels.size()) {
		qCWarning(L::function_list) << "Invalid index" << index;
		return;
	}

	beginRemoveRows({}, index, index);
	m_funModels.remove(index);
	endRemoveRows();
}

void FunctionList::resetModelData() {}
