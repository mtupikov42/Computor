#include "QmlListModel.h"

#include "algorithmUtils.h"

#include <QQmlEngine>

void QmlListModel::RegisterType(const char* uri) {
	qmlRegisterUncreatableType<QmlListModel>(uri, 1, 0, "QmlListModel", "QmlListModel must be created on C++ side");
}

QmlListModel::QmlListModel(QObject* parent) : QAbstractListModel(parent) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

void QmlListModel::reset() {
    beginResetModel();

    const auto old_count = rowCount();
    resetModelData();

    endResetModel();

    if (old_count != rowCount()) {
        emit countChanged(rowCount());
    }
}

bool QmlListModel::setProperty(int idx, const QString& property, const QVariant& value) {
    QModelIndex model_idx = index(idx, 0);
    if (isValid(model_idx)) {
        const auto role_names = roleNames();
        auto it = Utils::Find(role_names, property);
        if (it != role_names.end()) {
            setData(model_idx, value, it.key());
        } else {
            Q_ASSERT(false && "unhandled property");
        }
    }

    return false;
}

QVariant QmlListModel::getProperty(int idx, const QString& property) const {
    QModelIndex model_idx = index(idx, 0);
    if (isValid(model_idx)) {
        const auto role_names = roleNames();
        auto it = Utils::Find(role_names, property);
        if (it != role_names.end()) {
            return data(model_idx, it.key());
        } else {
            Q_ASSERT(false && "unhandled property");
        }
    }

    return {};
}

QString QmlListModel::roleName(int role) const {
    return roleNames().value(role);
}

bool QmlListModel::isValid(const QModelIndex& idx) const {
    return idx.isValid() && (idx.model() == this) && (idx.row() < rowCount());
}

bool QmlListModel::isValid(int idx) const {
    return index(idx).isValid() && (idx < rowCount());
}

void QmlListModel::notifyRowDataChanged(int row, const QVector<int>& roles) {
    const auto model_idx = index(row, 0);
    Q_ASSERT(isValid(model_idx));
    emit dataChanged(model_idx, model_idx, roles);
}

void QmlListModel::notifyRowsDataChanged(int row_from, int row_to, const QVector<int>& roles) {
    const auto model_idx_from = index(row_from, 0);
    const auto model_idx_to = index(row_to, 0);
    Q_ASSERT(isValid(model_idx_from) && isValid(model_idx_to));
    emit dataChanged(model_idx_from, model_idx_to, roles);
}
