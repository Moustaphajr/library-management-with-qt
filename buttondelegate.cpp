#include "buttondelegate.h"
#include <QApplication>
#include <QStyle>

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    if (index.column() == 5) // Assuming buttons are in column 5
    {
        QStyleOptionButton editButton;
        editButton.rect = option.rect.adjusted(0, 0, -50, 0); // Adjust size and position
        editButton.text = "Edit";
        editButton.state = QStyle::State_Enabled;
        QApplication::style()->drawControl(QStyle::CE_PushButton, &editButton, painter);

        QStyleOptionButton deleteButton;
        deleteButton.rect = option.rect.adjusted(-50, 0, 0, 0); // Adjust size and position
        deleteButton.text = "Delete";
        deleteButton.state = QStyle::State_Enabled;
        QApplication::style()->drawControl(QStyle::CE_PushButton, &deleteButton, painter);
    }
}

bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease && index.column() == 5)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QRect editButtonRect = option.rect.adjusted(0, 0, -50, 0);
        QRect deleteButtonRect = option.rect.adjusted(-50, 0, 0, 0);

        if (editButtonRect.contains(mouseEvent->pos()))
        {
            emit editButtonClicked(index.row());
            return true;
        }
        else if (deleteButtonRect.contains(mouseEvent->pos()))
        {
            emit deleteButtonClicked(index.row());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
