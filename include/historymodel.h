#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractTableModel>
#include <QList>

QT_BEGIN_NAMESPACE
class HistoryModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit HistoryModel(QObject *parent = nullptr)
        : QAbstractTableModel{parent}
    {
    }

    ~HistoryModel() override = default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return _items.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return column;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role == Qt::DisplayRole)
        {
            return _items[index.row()][index.column()];
        }

        return {};
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        return {};
    }

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (count == 0)
            return false;

        beginInsertRows(parent, row, row + count - 1);

        _items.push_back({});

        endInsertRows();

        return true;
    }

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (count == 0)
            return false;

        beginRemoveRows(parent, row, row + count - 1);

        _items.erase(std::next(std::begin(_items), row), std::next(std::begin(_items), row + count - 1));

        endRemoveRows();

        return true;
    }

private:
    const int column = 3;

    struct Item
    {
        QString data, time, interval;
        const QString empty{};

        constexpr const QString &operator[](size_t index) const noexcept
        {
            if (index == 0)
                return data;
            else if (index == 1)
                return time;
            else if (index == 2)
                return interval;
            else
                return empty;
        }
    };

    QList<Item> _items;
};
QT_END_NAMESPACE

#endif // HISTORYMODEL_H
