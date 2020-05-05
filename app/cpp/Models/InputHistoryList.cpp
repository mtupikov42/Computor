#include "InputHistoryList.h"

#include <QLoggingCategory>

namespace L {
Q_LOGGING_CATEGORY(input_history_list, "app.input.history_list", QtInfoMsg);
} // end namespace L

InputHistoryList::InputHistoryList(QObject* parent) : QmlListModel(parent) {}

QVariant InputHistoryList::data(const QModelIndex& index, int role) const {
	if (!isValid(index)) {
		return {};
	}

	const auto idx = static_cast<size_t>(index.row());
	const auto& d = m_modelData[idx];

	switch (role) {
	case Role::Model: return d;
	default:
		qFatal("unhandled role");
		break;
	}

	return {};
}

bool InputHistoryList::setData(const QModelIndex&, const QVariant&, int) {
	return false;
}

int InputHistoryList::rowCount(const QModelIndex&) const {
	return static_cast<int>(m_modelData.size());
}

QHash<int, QByteArray> InputHistoryList::roleNames() const {
	static const decltype(roleNames()) role_names{
		{ Role::Model, "modelRole" }
	};

	return role_names;
}

void InputHistoryList::add(const QString& model) {
	beginInsertRows({}, rowCount(), rowCount());
	m_modelData.push_back(model);
	endInsertRows();

	qCDebug(L::input_history_list) << model << " added to input history";
}

void InputHistoryList::resetModelData() {}
