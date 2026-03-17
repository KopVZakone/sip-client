#ifndef CHATPROXYMODEL_H
#define CHATPROXYMODEL_H

#include <QObject>
#include <qsortfilterproxymodel.h>
#include <qsqltablemodel.h>
class ChatProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterNumber READ filterNumber NOTIFY filterNumberChanged)
public:
    ChatProxyModel(QSqlTableModel* model);
    QString filterNumber() const;
    void setFilterNumber(const QString &number);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
signals:
    void filterNumberChanged();
private:
    QString m_number;
};

#endif // CHATPROXYMODEL_H
