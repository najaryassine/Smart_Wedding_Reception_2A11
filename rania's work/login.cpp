#include "login.h"
#include <QSqlQuery>
#include<QDebug>
#include <QMessageBox>
#include "mainwindow.h"

login::login()
{
    this->username = "";
    this->password = "";
    this->role= "";
}

login::login(QString username1,QString password,QString r)
{
    this->username = username1;
    this->password = password;
    this->role = r;

}


bool login::connect(){
    query.prepare("SELECT * from LOGIN WHERE username = :username and password = :password and role=:role");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":role",role);
    if (query.exec())
    {
        if (query.next())
        {
            int tab[5];

            QString username = query.value(0).toString();
            for(int i=0;i<5;i++){
                tab[i] = query.value((i+1)).toInt();
            }
            qDebug()  << username << "is logged in";
            return true;
        }
        else
        {
            qDebug() << "Login failed. Invalid username or password.";
            return false;

        }
    }
}



