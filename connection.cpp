#include "connection.h"
#include "salle.h"
#include "organisation.h"
#include "decorateur.h"

//test tutoriel Git

connection::connection()
{

}
bool connection::createconnection()
{
    bool test=false;
db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("test");//inserer le nom de la source de données ODBC
db.setUserName("siwar");//inserer nom de l'utilisateur
db.setPassword("siwar");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;

    return  test;
}
void connection::closeconnection(){db.close();}
