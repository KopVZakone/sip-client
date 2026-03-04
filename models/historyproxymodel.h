#ifndef HISTORYPROXYMODEL_H
#define HISTORYPROXYMODEL_H

#include "qsqltablemodel.h"
#include <QObject>
#include <qsortfilterproxymodel.h>

class HistoryProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterChanged)
    Q_PROPERTY(QString filterStatus READ filterStatus WRITE setFilterStatus NOTIFY filterChanged)
public:
    HistoryProxyModel(QSqlTableModel* model);
    void setFilterText(const QString &text);
    QString filterText() const;
    QString filterStatus() const;
    void setFilterStatus(const QString &status);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
signals:
    void filterChanged();
private:
    QString m_filterText;
    QString m_filterStatus;
};

#endif // HISTORYPROXYMODEL_H
