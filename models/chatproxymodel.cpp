#include "chatproxymodel.h"

ChatProxyModel::ChatProxyModel(QSqlTableModel* model): QSortFilterProxyModel{}
{
    setSourceModel(model);
    setFilterCaseSensitivity(Qt::CaseSensitive);
    setDynamicSortFilter(true);
}

QString ChatProxyModel::filterNumber() const
{
    return m_number;
}

void ChatProxyModel::setFilterNumber(const QString &number)
{
    if (m_number != number) {
        m_number = number;
        setFilterFixedString(number);
        emit filterNumberChanged();
    }
}

bool ChatProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // Фильтрация по первой колонке - remoteNumber
    QModelIndex index = sourceModel()->index(source_row, 1, source_parent);
    return sourceModel()->data(index).toString() == m_number;
}
