#include "editformuser.h"
#include "ui_editformuser.h"
#include <QMessageBox>
#include "dbaccess.h"


EditformUser::EditformUser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditformUser)
{
    ui->setupUi(this);
}

EditformUser::~EditformUser()
{
    delete ui;
}



void EditformUser::setUserData( const QString &name, const QString date, const QString &email, const QString &sexe){
    ui->nomLineEdit_2->setText(name);
    ui->dateDInscriptionLineEdit_2->setText(date);
    ui->emailLineEdit_2->setText(email);
    ui->sexeLineEdit_2->setText(sexe);
}


QString EditformUser::userName() const{

    return ui->nomLineEdit_2->text();

}

QString EditformUser::userAddDate() const{
    return ui->dateDInscriptionLineEdit_2->text();
}


QString EditformUser::userEmail() const {
    return ui->emailLineEdit_2->text();
}

QString  EditformUser::userSexe() const {
    return ui->sexeLineEdit_2->text();
}







void EditformUser::on_pushButton_2_clicked()
{
    emit updateClick();
    accept();
}

