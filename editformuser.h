#ifndef EDITFORMUSER_H
#define EDITFORMUSER_H

#include <QDialog>

namespace Ui {
class EditformUser;
}

class EditformUser : public QDialog
{
    Q_OBJECT

public:
    explicit EditformUser(QWidget *parent = nullptr);
    ~EditformUser();

      void setUserData( const QString &name, const QString date, const QString &email, const QString &sexe);

    QString userName () const;
    QString userAddDate() const;
    QString userEmail() const;
    QString userSexe() const;


signals:
    void updateClick();


private slots:
    void on_pushButton_2_clicked();

private:
    Ui::EditformUser *ui;
};

#endif // EDITFORMUSER_H
