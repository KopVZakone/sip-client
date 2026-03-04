#include "historyproxymodel.h"

#include "historymodel.h"

HistoryProxyModel::HistoryProxyModel(QSqlTableModel *model) : QSortFilterProxyModel{}
{
    setSourceModel(model);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setDynamicSortFilter(true);
}

void HistoryProxyModel::setFilterText(const QString &text)
{
    if (m_filterText == text)
        return;
    m_filterText = text;
    invalidateFilter();
    emit filterChanged();
}

QString HistoryProxyModel::filterText() const
{
    return m_filterText;
}

QString HistoryProxyModel::filterStatus() const
{
    return m_filterStatus;
}

void HistoryProxyModel::setFilterStatus(const QString &status)
{
    if (m_filterStatus == status)
        return;
    m_filterStatus = status;
    invalidateFilter();
    emit filterChanged();
}

bool HistoryProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex idxFrom = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex idxTo = sourceModel()->index(source_row, 2, source_parent);
    QString from = sourceModel()->data(idxFrom).toString();
    QString to = sourceModel()->data(idxTo).toString();

    if (!m_filterText.isEmpty() && !from.contains(m_filterText, Qt::CaseInsensitive)
        && !to.contains(m_filterText, Qt::CaseInsensitive)) {
        return false;
    }

    if (!m_filterStatus.isEmpty()) {
        QModelIndex idxStatus = sourceModel()->index(source_row, 3, source_parent);
        if (sourceModel()->data(idxStatus).toString() != m_filterStatus) {
            return false;
        }
    }

    return true;
}
