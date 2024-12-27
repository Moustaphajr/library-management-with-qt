#include "usercontroller.h"

class UserControllerData : public QSharedData
{
public:
};



UserController::UserController(UserWindow *window, QObject *parent)
    : QObject(parent), m_userWindow(window)
{
}

UserController::UserController(const UserController &rhs)
    : data{rhs.data}
{}

UserController &UserController::operator=(const UserController &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

UserController::~UserController() {}
