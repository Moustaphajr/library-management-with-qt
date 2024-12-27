#ifndef EDITBOOKFORM_H
#define EDITBOOKFORM_H

#include <QDialog>

namespace Ui {
class EditBookForm;
}

class EditBookForm : public QDialog
{
    Q_OBJECT

public:
    explicit EditBookForm(QWidget *parent = nullptr);
    ~EditBookForm();

    void setBookData( const QString &title, const QString &author, const QString &genre, const QString &date);


    QString bookTitle() const;
    QString bookAuthor() const;
    QString bookGenre() const;
    QString bookDate() const;

signals:
    void saveClicked();

private slots:
    void on_pushButton_clicked();

private:
    Ui::EditBookForm *ui;
};

#endif // EDITBOOKFORM_H
