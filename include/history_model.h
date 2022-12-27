#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractTableModel>
#include <QBrush>
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

    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return _items.size();
    }

    Q_INVOKABLE int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return column;
    }

    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (index.isValid() == false)
            return {};
        else if (index.row() >= _items.size() || index.row() < 0)
            return {};

        if (role == Qt::DisplayRole)
        {
            const auto &item = _items[index.row()];

            switch (index.column())
            {
            case 0:
                return item.datetime;
                break;

            case 1:
                return item.interval;
                break;

            default:
                return {};
                break;
            }
        }
        else if (role == Qt::TextAlignmentRole)
        {
            return Qt::AlignCenter;
        }
        else if (role == Qt::ForegroundRole)
        {
            switch (index.column())
            {
            case 0:
                return QColor(Qt::white);
                break;

            case 1:
                return QColor(255, 135, 0);
                break;

            default:
                return {};
                break;
            }
        }

        return {};
    }

    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        return {};
    }

    Q_INVOKABLE bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (row < 0 || count <= 0)
            return false;

        beginInsertRows(parent, row, row + count - 1);

        for (int i = row; i < row + count; ++i)
        {
            _items.insert(i, {});
        }

        endInsertRows();

        return true;
    }

    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (row < 0 || count <= 0)
            return false;

        beginRemoveRows(parent, row, row + count - 1);

        _items.erase(std::next(std::begin(_items), row), std::next(std::begin(_items), row + count));

        endRemoveRows();

        return true;
    }

    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override
    {
        if (index.isValid() == false || value.isValid() == false)
            return false;
        else if (role != Qt::EditRole)
            return false;

        auto &item = _items[index.row()];

        switch (index.column())
        {
        case 0:
            item.datetime = value.toString();
            break;

        case 1:
            item.interval = value.toString();
            break;

        default:
            return false;
            break;
        }

        return true;
    }

private:
    const int column = 2;

    struct Item
    {
        QString datetime, interval;
    };

    QList<Item> _items;
};
QT_END_NAMESPACE

#endif // HISTORYMODEL_H
