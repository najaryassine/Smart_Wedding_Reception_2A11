#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connexion.h"
#include "login.h"
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include <QMediaPlayer>
#include "invites.h"
#include "ceremonies.h"
#include "stat.h"
#include "QDebug"
#include <QString>
#include <QDateEdit>
#include <QFile>
#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QSystemTrayIcon>
#include <QPixmap>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>
#include <iostream>
#include "qpainter.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pages->setCurrentIndex(0);
    ui->pagesTable->setCurrentIndex(0);

    QMediaPlayer *music=new QMediaPlayer();
   // music->setMedia(QUrl("qrc:/ressource/Click.wav"));
    music->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_login_clicked()
{
    QMediaPlayer *music=new QMediaPlayer();
    music->setMedia(QUrl("qrc:/ressource/Click.wav"));
    music->play();

    QString username = ui->login_name->text();
    QString password = ui->login_pass->text();
    QString role = ui->login_role->currentText();
    QString etat="connecté(e)";
           if((username !=  "")&&(password!="")&&(role !="")) {
login h(username,password,role);
bool test=h.connect();

if(test)
{

    if( role=="EMPLOYE" )
    {
     ui->pages->setCurrentIndex(3);
    }

    else if( role=="ADMIN" )
    {
     ui->pages->setCurrentIndex(4);
    }
ui->tableViewInvites->setModel(tmpinvite.afficher());
ui->tableViewCerems->setModel(tmpcerem.afficher());
}


else{
QMessageBox::warning(this,"Connection", "Vérifier svp ");
}
           }}

void MainWindow::on_ajouterinvite_clicked()
{
    int Id_invitation=ui->invi_id->text().toInt();
    QString nom =ui->invi_nom->text();
    QString prenom =ui->invi_prenom->text();

    qDebug()<<Id_invitation;
        qDebug()<<nom;
            qDebug()<<prenom;

    invites invi(Id_invitation,prenom,nom);
    bool test=invi.ajouter();
    if(test)
{
    ui->tableViewInvites->setModel(tmpinvite.afficher());
    QMessageBox::information(nullptr, QObject::tr("Ajouter un invite"),
                      QObject::tr("invite ajouté.\n"
                                  "Click Cancel to exit."), QMessageBox::Cancel);
    ui->invi_id->setText("");
    ui->invi_nom->setText("");
    ui->invi_prenom->setText("");

    }
      else
       { QMessageBox::critical(nullptr, QObject::tr("Ajouter un invite"),
                      QObject::tr("Erreur !.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);}

}

void MainWindow::on_tableViewInvites_activated(const QModelIndex &index)
{
    QString val=ui->tableViewInvites->model()->data(index).toString();
    QSqlQuery q;
    q.prepare("select * from INVITES where IDINVI='"+val+"'");
    if(q.exec())
    {
        while (q.next()) {
            ui->invi_id->setText(q.value(0).toString());
            ui->invi_nom->setText(q.value(1).toString());
            ui->invi_prenom->setText(q.value(2).toString());

        }
    }
}


void MainWindow::on_supprimerinvite_clicked()
{
    QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "vehicule", "supprimer?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
          qDebug() << "Yes was clicked";
          int Id_invitation=ui->invi_id->text().toInt();
          qDebug()<<Id_invitation;

        bool test=tmpinvite.supprimer(Id_invitation);
        ui->invi_id->setText("");
        ui->invi_nom->setText("");
        ui->invi_prenom->setText("");
        ui->tableViewInvites->setModel(tmpinvite.afficher());

        } else {
          qDebug() << "Yes was *not* clicked";
        }
}

void MainWindow::on_modifierinvite_clicked()
{
    int Id_invitation=ui->invi_id->text().toInt();
    QString nom =ui->invi_nom->text();
    QString prenom =ui->invi_prenom->text();

    invites invi(Id_invitation,prenom,nom);
       bool test=invi.modifier(Id_invitation);
       if(test)
     {
           ui->invi_id->setText("");
           ui->invi_nom->setText("");
           ui->invi_prenom->setText("");
    ui->tableViewInvites->setModel(tmpinvite.afficher());//afficher tableau invi
     QMessageBox::information(nullptr, QObject::tr("modifier"),
                       QObject::tr("modifié.\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);

     }
       else
           QMessageBox::critical(nullptr, QObject::tr("modifier"),
                       QObject::tr("non modifié !.\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);

}



void MainWindow::on_ajouterMariage_clicked()
{
    int Id_emplacement=ui->mariage_id->text().toInt();
    int code_mariage=ui->mariage_code->text().toInt();
    int Nb_invites=ui->mariage_nbinvit->text().toInt();
   // QString Date =ui->mariage_date->text();
    float Budget=ui->mariage_budget->text().toInt();
     QString type;
    qDebug()<<code_mariage;
        qDebug()<<Id_emplacement;
            qDebug()<<Nb_invites;
           // qDebug()<<Date;
            qDebug()<<Budget;

          QString Budget_string=QString :: number(Budget);
            if (Budget_string.length() == 3)
                         type = "faible";
                       if (Budget_string.length() == 4)
                         type = "moyen";
                       if (Budget_string.length() == 5)
                         type = "haut";
        QSqlQuery *q=new QSqlQuery;
        q->prepare("UPDATE MARIAGE SET TYPE =:type WHERE CODEMARIAGE=:code_mariage");
        q->bindValue(":type",type);
        q->bindValue(":code_mariage",code_mariage);
        q->exec();

    ceremonies cerem(Id_emplacement,code_mariage,Nb_invites,Budget,type);
    bool test=cerem.ajouter();
    if(test)
{
    ui->tableViewCerems->setModel(tmpcerem.afficher());
    QMessageBox::information(nullptr, QObject::tr("Ajouter un mariage"),
                      QObject::tr("mariage ajouté.\n"
                                  "Click Cancel to exit."), QMessageBox::Cancel);
    ui->mariage_code->setText("");
    ui->mariage_id->setText("");
    ui->mariage_nbinvit->setText("");
    //ui->mariage_date->setText("");
    ui->mariage_budget->setText("");
    }
      else
       { QMessageBox::critical(nullptr, QObject::tr("Ajouter un mariage"),
                      QObject::tr("Erreur !.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);}

}

void MainWindow::on_tableViewCerems_activated(const QModelIndex &index)
{
    QString val=ui->tableViewCerems->model()->data(index).toString();
    QSqlQuery q;
    q.prepare("select * from MARIAGE where CODEMARIAGE='"+val+"'");
    if(q.exec())
    {
        while (q.next()) {
            ui->mariage_code->setText(q.value(0).toString());
            ui->mariage_id->setText(q.value(1).toString());
            ui->mariage_nbinvit->setText(q.value(2).toString());
           // ui->mariage_date->setText(q.value(3).toString());
            ui->mariage_budget->setText(q.value(3).toString());


        }
    }

}

void MainWindow::on_modifierMariage_clicked()
{
    int Id_emplacement=ui->mariage_id->text().toInt();
    int code_mariage=ui->mariage_code->text().toInt();
    int Nb_invites=ui->mariage_nbinvit->text().toInt();
    // QString Date =ui->mariage_date->text();
    float Budget=ui->mariage_budget->text().toInt();
   QString type;
    ceremonies cerem(Id_emplacement,code_mariage,Nb_invites,Budget,type);
       bool test=cerem.modifier(code_mariage);
       if(test)
     {
           ui->mariage_id->setText("");
           ui->mariage_code->setText("");
           ui->mariage_nbinvit->setText("");
           //ui->mariage_date->setText("");
           ui->mariage_budget->setText("");
    ui->tableViewCerems->setModel(tmpcerem.afficher());//afficher tableau Mariage
     QMessageBox::information(nullptr, QObject::tr("modifier"),
                       QObject::tr("modifié.\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);

     }
       else
           QMessageBox::critical(nullptr, QObject::tr("modifier"),
                       QObject::tr("non modifié !.\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::on_supprimerMariage_clicked()
{
    QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "vehicule", "supprimer?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
          qDebug() << "Yes was clicked";
          int code_mariage=ui->mariage_code->text().toInt();
          qDebug()<<code_mariage;

       bool test=tmpcerem.supprimer(code_mariage);
        ui->mariage_id->setText("");
        ui->mariage_code->setText("");
        ui->mariage_nbinvit->setText("");
        //ui->mariage_date->setText("");
        ui->mariage_budget->setText("");
        ui->tableViewCerems->setModel(tmpcerem.afficher());

        } else {
          qDebug() << "Yes was *not* clicked";
        }
}

void MainWindow::on_btn_rech_invite_clicked()
{
    QString rech=ui->recherche_invite->text();
 if(rech!="")  { ui->tableViewInvites->setModel(tmpinvite.recherche(rech)); }
 else{ QMessageBox::information(this,"Pour chercher il Faut","tapez quelque chose !");
 ui->tableViewInvites->setModel(tmpinvite.afficher());
 }
}

void MainWindow::on_btn_rechfMariage_clicked()
{
    QString rech=ui->recherche_Mariage->text();
 if(rech!="")  { ui->tableViewCerems->setModel(tmpcerem.recherche(rech)); }
 else{ QMessageBox::information(this,"Pour chercher il Faut","tapez quelque chose !");
 ui->tableViewCerems->setModel(tmpcerem.afficher());
 }

}



void MainWindow::on_btn_triefdescMariage_clicked()
{
    QString typetri=ui->trie_mariage->currentText();
    if(typetri=="select")
    {
            QMessageBox Box;
                        Box.setText("Please select type de trie ");
                        Box.exec();
    }
    else
    {
    ui->tableViewCerems->setModel(tmpcerem.tri_ceremDesc(typetri));
    }
}

void MainWindow::on_btn_trieDec_clicked()
{
    QString typetri=ui->trie_invite->currentText();
    if(typetri=="select")
    {
            QMessageBox Box;
                        Box.setText("Please select type de trie ");
                        Box.exec();
    }
    else
    {
    ui->tableViewInvites->setModel(tmpinvite.tri_inviDesc(typetri));
    }
}

void MainWindow::on_btn_trieAsc_clicked()
{
    QString typetri=ui->trie_invite->currentText();
    if(typetri=="select")
    {
            QMessageBox Box;
                        Box.setText("Please select type de trie ");
                        Box.exec();
    }
    else
    {
    ui->tableViewInvites->setModel(tmpinvite.tri_inviAsc(typetri));
    }
}

void MainWindow::on_btn_triefascMariage_clicked()
{
    QString typetri=ui->trie_mariage->currentText();
    if(typetri=="select")
    {
            QMessageBox Box;
                        Box.setText("Please select type de trie ");
                        Box.exec();
    }
    else
    {
    ui->tableViewCerems->setModel(tmpcerem.tri_ceremAsc(typetri));
    }
}

void MainWindow::on_ExcelMariage_clicked()
{
    QTableView *table;
    table = ui->tableViewCerems;

    QString filters("CSV files (*.csv);;All files (*.*)");
    QString defaultFilter("CSV files (*.csv)");
    QString fileName = QFileDialog::getSaveFileName(0, "Save file", QCoreApplication::applicationDirPath(),
                       filters, &defaultFilter);
    QFile file(fileName);

    QAbstractItemModel *model =  table->model();
    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream data(&file);
        QStringList strList;
        for (int i = 0; i < model->columnCount(); i++) {
            if (model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString().length() > 0)
                strList.append("\"" + model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() + "\"");
            else
                strList.append("");
        }
        data << strList.join(";") << "\n";
        for (int i = 0; i < model->rowCount(); i++) {
            strList.clear();
            for (int j = 0; j < model->columnCount(); j++) {

                if (model->data(model->index(i, j)).toString().length() > 0)
                    strList.append("\"" + model->data(model->index(i, j)).toString() + "\"");
                else
                    strList.append("");
            }
            data << strList.join(";") + "\n";
        }
        file.close();
        QMessageBox::information(this,"Exporter To Excel","Exporter En Excel Avec Succées ");
    }
}

void MainWindow::on_Pdf_invitation_clicked()
{
    QString strStream;
                QTextStream out(&strStream);

                 const int rowCount = ui->tableViewInvites->model()->rowCount();
                 const int columnCount = ui->tableViewInvites->model()->columnCount();
                out <<  "<html>\n"
                "<head>\n"
                                 "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                                 <<  QString("<title>%1</title>\n").arg("strTitle")
                                 <<  "</head>\n"
                                 "<body bgcolor=#ffffff link=#5000A0>\n"

                                //     "<align='right'> " << datefich << "</align>"
                                 "<center> <H1>Liste des invités</H1></br></br><table border=1 cellspacing=0 cellpadding=2>\n";

                             // headers
                             out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
                             out<<"<cellspacing=10 cellpadding=3>";
                             for (int column = 0; column < columnCount; column++)
                                 if (!ui->tableViewInvites->isColumnHidden(column))
                                     out << QString("<th>%1</th>").arg(ui->tableViewInvites->model()->headerData(column, Qt::Horizontal).toString());
                             out << "</tr></thead>\n";

                             // data table
                             for (int row = 0; row < rowCount; row++) {
                                 out << "<tr> <td bkcolor=0>" << row+1 <<"</td>";
                                 for (int column = 0; column < columnCount; column++) {
                                     if (!ui->tableViewInvites->isColumnHidden(column)) {
                                         QString data = ui->tableViewInvites->model()->data(ui->tableViewInvites->model()->index(row, column)).toString().simplified();
                                         out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                                     }
                                 }
                                 out << "</tr>\n";
                             }
                             out <<  "</table> </center>\n"
                                 "</body>\n"
                                 "</html>\n";

                       QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Sauvegarder en PDF", QString(), "*.pdf");
                         if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

                        QPrinter printer (QPrinter::PrinterResolution);
                         printer.setOutputFormat(QPrinter::PdfFormat);
                        printer.setPaperSize(QPrinter::A4);
                       printer.setOutputFileName(fileName);

                        QTextDocument doc;
                         doc.setHtml(strStream);
                         doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
                         doc.print(&printer);
}

void MainWindow::on_pushButton_clicked()
{
    int res;
            statistiques w(this);
            w.setWindowTitle("Statistiques des operateurs");

            res = w.exec();
            qDebug() << res;
            if (res == QDialog::Rejected)
              return;
}

void MainWindow::on_pushButton_2_clicked()
{
    QPrinter printer;
    
                printer.setPrinterName("desiered printer name");
    
              QPrintDialog dialog(&printer,this);
    
                if(dialog.exec()== QDialog::Rejected)
    
                    return;
}





void MainWindow::on_ceremonie_3_clicked()
{
    ui->pages->setCurrentIndex(2);
}

void MainWindow::on_ceremonie_2_clicked()
{
    ui->pages->setCurrentIndex(2);
}


