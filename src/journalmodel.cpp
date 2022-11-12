
#include "journalmodel.h"

#include <QErrorMessage>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>

Q_LOGGING_CATEGORY(model, "bill.journalmodel")

QHash< int, QByteArray > JournalModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    // record() returns an empty QSqlRecord
    for (int i = 0; i < this->record().count(); i ++) {
        roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8());
    }
    return roles;
}

QVariant JournalModel::data ( const QModelIndex& index, int role ) const
{
    if (!index.isValid()) {
        return {};
    }

    return QSqlTableModel::data(indexForRole(index, role), Qt::EditRole);
}

bool JournalModel::setData ( const QModelIndex& index, const QVariant& value, int role )
{
    if (!index.isValid()) {
        return false;
    }

    return QSqlTableModel::setData(indexForRole(index, role), value, Qt::EditRole);
}

QModelIndex JournalModel::indexForRole ( const QModelIndex& index, int role ) const
{
    int row = index.row();
    int column = index.column();
    if (role > Qt::UserRole) {
        column = role - Qt::UserRole - 1;
    }
    return this->index(row, column);
}
