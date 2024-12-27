#include "editbookform.h"
#include "ui_editbookform.h"


/**
 * @brief Constructeur de la classe EditBookForm.
 *
 * Initialise la fenêtre de dialogue en utilisant le fichier d'interface utilisateur
 * généré par Qt Designer. Ce constructeur configure l'interface utilisateur et
 * permet à la fenêtre de dialogue de fonctionner correctement dans le contexte
 * d'une application Qt.
 *
 * @param parent Le widget parent pour cette fenêtre de dialogue, ou nullptr si
 *               cette fenêtre est indépendante.
 *
**/

EditBookForm::EditBookForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBookForm)
{
    ui->setupUi(this);
}

EditBookForm::~EditBookForm()
{
    delete ui;
}

void EditBookForm::setBookData( const QString &title, const QString &author, const QString &genre, const QString &date)
{

    ui->titreLineEdit->setText(title);
    ui->auteurLineEdit->setText(author);
    ui->genreLineEdit->setText(genre);
    ui->dateDAjoutLineEdit->setText(date);
}



QString EditBookForm::bookTitle() const
{
    return ui->titreLineEdit->text();
}

QString EditBookForm::bookAuthor() const
{
    return ui->auteurLineEdit->text();
}

QString EditBookForm::bookGenre() const
{
    return ui->genreLineEdit->text();
}

QString EditBookForm::bookDate() const
{
    return ui->dateDAjoutLineEdit->text();
}

void EditBookForm::on_pushButton_clicked()
{
    emit saveClicked();
    accept(); // Close the form
}

