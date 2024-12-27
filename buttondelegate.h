#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QStyleOptionButton>
#include <QPainter>

class ButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ButtonDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void editButtonClicked(int row);
    void deleteButtonClicked(int row);
};

#endif // BUTTONDELEGATE_H
