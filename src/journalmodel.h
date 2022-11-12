
#pragma once

#include <QLoggingCategory>
#include <QSqlTableModel>

Q_DECLARE_LOGGING_CATEGORY(model)

class JournalModel : public QSqlTableModel
{
    Q_OBJECT

public:
    QHash< int, QByteArray > roleNames() const override;
    QVariant data ( const QModelIndex & index, int role ) const override;
    bool setData ( const QModelIndex & index, const QVariant & value, int role ) override;

private:
    QModelIndex indexForRole(const QModelIndex& index, int role) const;
};
