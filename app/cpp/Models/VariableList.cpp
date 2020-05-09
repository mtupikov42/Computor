#include "VariableList.h"

#include <QLoggingCategory>

namespace L {
Q_LOGGING_CATEGORY(variable_list, "app.variable.list", QtInfoMsg);
} // end namespace L

VariableList::VariableList(QObject* parent) : QmlListModel(parent) {}

QVariant VariableList::data(const QModelIndex& index, int role) const {
	if (!isValid(index)) {
		return {};
	}

	const auto idx = static_cast<size_t>(index.row());
	const auto& d = m_varModels[idx];

	switch (role) {
	case Role::VarName: return d->name();
	case Role::Result: return d->resultString();
	case Role::Type: return QVariant::fromValue(d->type()).toString();
	case Role::ErrorString: return d->errorString();
	default:
		qFatal("unhandled role");
		break;
	}

	return {};
}

bool VariableList::setData(const QModelIndex&, const QVariant&, int) {
	return false;
}

int VariableList::rowCount(const QModelIndex&) const {
	return static_cast<int>(m_varModels.size());
}

QHash<int, QByteArray> VariableList::roleNames() const {
	static const decltype(roleNames()) role_names{
		{ Role::VarName, "varNameRole" },
		{ Role::Result, "resultRole" },
		{ Role::Type, "typeRole" },
		{ Role::ErrorString, "errorStringRole" }
	};

	return role_names;
}

const VariableModel::Ptr& VariableList::at(int index) const {
	Q_ASSERT(index >= 0 && index < m_varModels.size());

	return m_varModels[index];
}

void VariableList::add(const VariableModel::Ptr& model) {
	for (auto& f : m_varModels) {
		if (f->name().toLower() == model->name().toLower()) {
			f = model;
			notifyRowDataChanged(m_varModels.indexOf(f), { Role::Result, Role::Type, Role::ErrorString });
			return;
		}
	}

	beginInsertRows({}, rowCount(), rowCount());
	m_varModels.push_back(model);
	endInsertRows();
}

void VariableList::remove(int index) {
	if (index < 0 || index >= m_varModels.size()) {
		qCWarning(L::variable_list) << "Invalid index" << index;
		return;
	}

	beginRemoveRows({}, index, index);
	m_varModels.remove(index);
	endRemoveRows();
}

QMap<QString, QString> VariableList::validVariablesList() const {
	QMap<QString, QString> result;

	for (const auto& model : m_varModels) {
		if (model->isValid()) {
			result.insert(model->name(), model->resultString());
		}
	}

	return result;
}

void VariableList::resetModelData() {}
