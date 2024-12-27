#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H


#include <QObject>
#include <QWidget>
#include "userwindow.ui"

class UserController : public QObject
{
    Q_OBJECT

public:
    explicit UserController(UserWindow *window, QObject *parent = nullptr);
    void initialize();

private:
    UserWindow *m_userWindow;
};






#endif // USERCONTROLLER_H
