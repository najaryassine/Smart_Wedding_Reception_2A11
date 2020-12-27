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
#include <QPrintDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>
#include <iostream>
#include "qpainter.h"
#include "salle.h"
#include "decorateur.h"
#include <QModelIndex>
#include<QSqlQueryModel>
#include "smtp.h"
#include <QPrintDialog>
#include"QPdfWriter"
#include"QtPrintSupport/QPrinter"
#include"QPainter"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    /*int ret=A.connect_arduino();//lancer la cnx a arduino
    switch(ret)
    {case(0):qDebug()<<"arduino is available and connected to :"<<A.getarduino_port_name();
        break;
    case(1):qDebug()<<"arduino is avaiable but not connected to :"<<A.getarduino_port_name();
        break;
    case(-1):qDebug()<<"arduino is not available";
    }
    QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));*/


    ui->tablesalle->setModel(stmp.afficher());
    ui->tabledeco->setModel(dtmp.afficher());
    //photo on pushbutton
    ui->trie_prix->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/money.jpg"));
    ui->trie_capacite_2->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/capacite.jpg"));
     ui->suppression_salle->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/poubelle.jpg"));
     ui->salle_email->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/update.jpg"));
     ui->supprimer_deco->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/poubelle.jpg"));
      ui->update_s->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/update.jpg"));
      ui->suppression_a->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/mail.jpg"));
       ui->reserver_salle->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/reserver.jpg"));
         ui->recherche_deco->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/cherr.jpg"));
       ui->trie_id->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/id.jpg"));
       ui->trie_lieu->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/loca.jpg"));
          ui->a_retour->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/retour.jpg"));
          ui->style_recherche->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/style.jpg"));
           ui->idd_recherche->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/id.jpg"));
           ui->lieu_recherche_2->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/loca.jpg"));
           ui->backk->setIcon(QIcon("C:/Users/ASUS/Documents/Organisation/resource/retour.jpg"));
           //controle de saisie
        ui->lineEdit_7->setValidator(new QIntValidator (0,99999999));
        ui->lineEdit_18->setValidator(new QIntValidator (0,99999999));
       ui->lineEdit_id->setValidator(new QIntValidator (0,999999999));
       ui->lineEdit_ideco->setValidator(new QIntValidator(0,9999));
       ui->lineEdit_19->setValidator(new QRegExpValidator(QRegExp("[A-Za-z_]{0,20}"),this));
       //send mail
       connect(ui->sendBtn, SIGNAL(clicked()),this, SLOT(sendMail()));
       connect(ui->browseBtn, SIGNAL(clicked()), this, SLOT(browse()));
       //song
       player= new QMediaPlayer(this) ;
       player->setMedia(QUrl::fromLocalFile("C:/Users/ASUS/Documents/Organisation/resource/")) ;
       player->play() ;


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
     ui->pages->setCurrentIndex(15);
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



void MainWindow::on_gerer_salle_clicked()
{
     ui->pages->setCurrentIndex(5);
}

void MainWindow::on_gerer_decorateur_clicked()
{
    ui->pages->setCurrentIndex(6);
}

void MainWindow::on_quittersalledeco_clicked()
{

ui->pages->setCurrentIndex(15);
}

void MainWindow::on_liste_des_salle_clicked()
{
    ui->tablesalle->setModel(stmp.afficher());
    ui->pages->setCurrentIndex(10);
}

void MainWindow::on_ajouter_salle_clicked()
{
    ui->pages->setCurrentIndex(8);
}

void MainWindow::on_quittersalle_clicked()
{
    ui->pages->setCurrentIndex(15);
}

void MainWindow::on_retoursalle_clicked()
{
    ui->pages->setCurrentIndex(4);
}

void MainWindow::on_liste_de_decorateur_clicked()
{
    ui->tabledeco->setModel(dtmp.afficher());
    ui->pages->setCurrentIndex(9);
}

void MainWindow::on_ajouter_decorateur_clicked()
{
    ui->pages->setCurrentIndex(7);
}

void MainWindow::on_quitterdeco_clicked()
{
    ui->pages->setCurrentIndex(15);
}

void MainWindow::on_retourdeco_clicked()
{
    ui->pages->setCurrentIndex(4);
}

void MainWindow::on_ajouter_decorateur_2_clicked()
{
    ui->pages->setCurrentIndex(9);

    int id=ui->lineEdit_ideco->text().toInt();
    QString nom=ui->lineEdit_nom->text();
    QString style=ui->lineEdit_style->text();
    int num_telephone=ui->lineEdit_18->text().toInt();
    QString email=ui->lineEdit_21->text();
    QString lieu=ui->lineEdit_22->text();


    decorateur d(id,nom,style,num_telephone,email,lieu);
    bool test= d.ajouter();
    if(test)
    {
        ui->tabledeco->setModel(dtmp.afficher());
        QMessageBox::information(nullptr,"Ajout decorateur","decorateur ajoute");
}
    else
    {
        QMessageBox::warning(nullptr,"Ajout decorateur","decorateur non ajoute");
}
}



void MainWindow::on_return11_clicked()
{
     ui->pages->setCurrentIndex(6);
}


void MainWindow::on_ajoutersalle_clicked()
{
    ui->pages->setCurrentIndex(10);
    int id=ui->lineEdit_id->text().toInt();
    QString lieu=ui->lineEdit_lieu->text();
    int capacite=ui->lineEdit_capacite->text().toInt();
    int prix=ui->lineEdit_prix->text().toInt();
    salle s(lieu,id,capacite,prix);
    bool test= s.ajouter();
    if(test)
    {
        ui->tablesalle->setModel(stmp.afficher());
        QMessageBox::information(nullptr,"Ajout salle","salle ajoute");
}
    else
    {
        QMessageBox::warning(nullptr,"Ajout salle","salle non ajoute");
}
}


void MainWindow::on_retouur_clicked()
{
    ui->pages->setCurrentIndex(5);
}

void MainWindow::on_a_retour_clicked()
{
       ui->tabledeco->setModel(dtmp.afficher());
}

void MainWindow::on_lieu_recherche_2_clicked()
{
    QMessageBox msgBox ;
            QSqlQueryModel *model = new QSqlQueryModel();
            QString co;
            co = ui->lineEdit_24->text();
            model->setQuery("Select * from decorateur where lieu_decorateur = '"+co+"' ");
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
            model->setHeaderData(2, Qt::Horizontal, QObject::tr("style"));
            model->setHeaderData(3, Qt::Horizontal, QObject::tr("num_telephone"));
            model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
            model->setHeaderData(5, Qt::Horizontal, QObject::tr("lieu"));
            ui->tabledeco->setModel(model);
            ui->tabledeco->show();
            msgBox.setText("decorateur trouver");
            msgBox.exec();
            ui->lineEdit_24->clear();
}

void MainWindow::on_style_recherche_clicked()
{
    QMessageBox msgBox ;
            QSqlQueryModel *model = new QSqlQueryModel();
            QString code;
            code = ui->lineEdit_24->text();
            model->setQuery("Select * from decorateur where style_decorateur = '"+code+"' ");
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
            model->setHeaderData(2, Qt::Horizontal, QObject::tr("style"));
            model->setHeaderData(3, Qt::Horizontal, QObject::tr("num_telephone"));
            model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
            model->setHeaderData(5, Qt::Horizontal, QObject::tr("lieu"));
            ui->tabledeco->setModel(model);
            ui->tabledeco->show();
            msgBox.setText("decorateur trouver");
            msgBox.exec();
            ui->lineEdit_24->clear();
}

void MainWindow::on_idd_recherche_clicked()
{
    QMessageBox msgBox ;
            QSqlQueryModel *model = new QSqlQueryModel();
            QString cod;
            cod = ui->lineEdit_24->text();
            model->setQuery("Select * from decorateur where id_decorateur = '"+cod+"' ");
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
            model->setHeaderData(2, Qt::Horizontal, QObject::tr("style"));
            model->setHeaderData(3, Qt::Horizontal, QObject::tr("num_telephone"));
            model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
            model->setHeaderData(5, Qt::Horizontal, QObject::tr("lieu"));
            ui->tabledeco->setModel(model);
            ui->tabledeco->show();
            msgBox.setText("decorateur trouver");
            msgBox.exec();
            ui->lineEdit_24->clear();
}

void MainWindow::on_suppression_a_clicked()
{
     ui->pages->setCurrentIndex(12);
}

void MainWindow::on_supprimer_deco_clicked()
{
    int id =ui->lineEdit_ideco2->text().toInt();
    bool test=dtmp.supprimer(id);
    if(test)
    {
        ui->tabledeco->setModel(dtmp.afficher());
        QMessageBox::information(nullptr,"suppression decorateur","decorateur supprimé");
}
    else
        QMessageBox::warning(nullptr,"suppression decorateur","decorateur non supprimé");
}

void MainWindow::on_update_s_clicked()
{
    ui->pages->setCurrentIndex(14);
}

void MainWindow::on_exporteer_pdff_clicked()
{
    QString strStream;
                QTextStream out(&strStream);

                 const int rowCount = ui->tabledeco->model()->rowCount();
                 const int columnCount = ui->tabledeco->model()->columnCount();
                out <<  "<html>\n"
                "<head>\n"
                                 "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                                 <<  QString("<title>%1</title>\n").arg("strTitle")
                                 <<  "</head>\n"
                                 "<body bgcolor=#ffffff link=#5000A0>\n"

                                //     "<align='right'> " << datefich << "</align>"
                                 "<center> <H1>Liste des decorateur</H1></br></br><table border=1 cellspacing=0 cellpadding=2>\n";

                             // headers
                             out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
                             out<<"<cellspacing=10 cellpadding=3>";
                             for (int column = 0; column < columnCount; column++)
                                 if (!ui->tabledeco->isColumnHidden(column))
                                     out << QString("<th>%1</th>").arg(ui->tabledeco->model()->headerData(column, Qt::Horizontal).toString());
                             out << "</tr></thead>\n";

                             // data table
                             for (int row = 0; row < rowCount; row++) {
                                 out << "<tr> <td bkcolor=0>" << row+1 <<"</td>";
                                 for (int column = 0; column < columnCount; column++) {
                                     if (!ui->tabledeco->isColumnHidden(column)) {
                                         QString data = ui->tabledeco->model()->data(ui->tabledeco->model()->index(row, column)).toString().simplified();
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

void MainWindow::on_home_clicked()
{
    ui->pages->setCurrentIndex(15);
}

void MainWindow::on_retouir_clicked()
{
    ui->pages->setCurrentIndex(6);
}

void MainWindow::on_trie_capacite_2_clicked()
{
    ui->tablesalle->setModel(stmp.afficher_tri_capacite());
}

void MainWindow::on_trie_id_clicked()
{
    ui->tablesalle->setModel(stmp.afficher_tri_id());
}

void MainWindow::on_trie_prix_clicked()
{
      ui->tablesalle->setModel(stmp.afficher_tri_prix());
}

void MainWindow::on_trie_lieu_clicked()
{
    ui->tablesalle->setModel(stmp.afficher_tri_lieu());
}

void MainWindow::on_suppression_salle_clicked()
{
    int id =ui->lineEdit_supp->text().toInt();
    bool test=stmp.supprimer(id);
    if(test)
    {
        ui->tablesalle->setModel(stmp.afficher());
        QMessageBox::information(nullptr,"suppression salle","salle supprimé");

}
    else
        QMessageBox::warning(nullptr,"suppression salle","salle non supprimé");
}

void MainWindow::on_reserver_salle_clicked()
{
    ui->pages->setCurrentIndex(13);
}

void MainWindow::on_salle_email_clicked()
{
    ui->pages->setCurrentIndex(11);
}

void MainWindow::on_export_excel_salle_clicked()
{
    QTableView *table;
    table = ui->tablesalle;

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

void MainWindow::on_retouur5_clicked()
{
    ui->pages->setCurrentIndex(5);
}

void MainWindow::on_homme_clicked()
{
    ui->pages->setCurrentIndex(4);
}

void MainWindow::on_okk_clicked()
{
    ui->pages->setCurrentIndex(10);


   int id=ui->lineEdit_13->text().toInt();
   QString lieu=ui->lineEdit_19->text();
   QString capacite=ui->lineEdit_3->text();
   QString prix=ui->lineEdit_4->text();
   bool test=stmp.modifier(id,lieu,capacite,prix);
   if(test)
   {
       ui->tablesalle->setModel(stmp.afficher());
       QMessageBox::information(nullptr,"modifier salle","salle modifier");
}
   else
   {
       QMessageBox::warning(nullptr,"modifier salle","salle non modifie");
}

}

void MainWindow::on_back_clicked()
{
    ui->pages->setCurrentIndex(10);
}

void MainWindow::on_backk_clicked()
{
    ui->pages->setCurrentIndex(9);
}

void MainWindow::on_imprimer_resrv_clicked()
{
    QPrinter printer ;
    printer.setPrinterName("esprit") ;
    QPrintDialog dialog(&printer,this) ;
    if( dialog.exec() == QDialog::Rejected) return ;
}

void MainWindow::on_backkk_clicked()
{
    ui->pages->setCurrentIndex(10);
}

void MainWindow::on_okkkkk_clicked()
{
    ui->pages->setCurrentIndex(9);


   int id=ui->lineEdit_16->text().toInt();
   QString nom=ui->lineEdit_17->text();
   QString style=ui->lineEdit_6->text();
   int num_telephone=ui->lineEdit_7->text().toInt();
   QString email=ui->lineEdit_8->text();
   QString lieu=ui->lineEdit_9->text();
   bool test=dtmp.modifier(id,nom,style,num_telephone,email,lieu);
   if(test)
   {
       ui->tabledeco->setModel(dtmp.afficher());
       QMessageBox::information(nullptr,"modifier decorateur","decorateur modifier");
}
   else
   {
       QMessageBox::warning(nullptr,"modifier decorateur","decorateur non modifie");
}

}

void MainWindow::on_bacck_clicked()
{
    ui->pages->setCurrentIndex(9);
}

void MainWindow::on_tabledeco_activated(const QModelIndex &index)
{
    QString val=ui->tabledeco->model()->data(index).toString();
    QSqlQuery query;
    query.prepare(" SELECT * FROM decorateur where ID_DECORATEUR='"+val+"'");
    if (query.exec())
    {
        while(query.next())
        {
         ui->lineEdit_16->setText(query.value(0).toString());
         ui->lineEdit_ideco2->setText(query.value(0).toString());
         ui->lineEdit_17->setText(query.value(1).toString());
         ui->lineEdit_6->setText(query.value(2).toString());
         ui->lineEdit_7->setText(query.value(3).toString());
         ui->lineEdit_8->setText(query.value(4).toString());
         ui->lineEdit_9->setText(query.value(5).toString());
        }
    }
}

void MainWindow::on_tablesalle_activated(const QModelIndex &index)
{
    QString val=ui->tablesalle->model()->data(index).toString();
    QSqlQuery query;
    query.prepare(" SELECT * FROM salle where ID_SALLE='"+val+"'");
    if (query.exec())
    {
        while(query.next())
        {
         ui->lineEdit_supp->setText(query.value(0).toString());
         ui->lineEdit_13->setText(query.value(0).toString());
         ui->lineEdit_19->setText(query.value(1).toString());
         ui->lineEdit_3->setText(query.value(2).toString());
         ui->lineEdit_4->setText(query.value(3).toString());
         ui->lineEdit_12->setText(query.value(0).toString());
         ui->lineEdit_11->setText(query.value(1).toString());
         ui->lineEdit_10->setText(query.value(2).toString());
         ui->lineEdit_5->setText(query.value(3).toString());
        }
    }
}

void MainWindow::on_a_retour_toggled(bool ed)
{
    if(ed)
    {
         ui->a_retour->setIcon(QIcon("C:/Users/ASUS/Desktop/retour.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_recherche_deco_toggled(bool y)
{
    if(y)
    {
         ui->recherche_deco->setIcon(QIcon("C:/Users/ASUS/Desktop/cherr.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_lieu_recherche_2_toggled(bool p)
{
    if(p)
    {
         ui->lieu_recherche_2->setIcon(QIcon("C:/Users/ASUS/Desktop/loca.png"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_style_recherche_toggled(bool g)
{
    if(g)
    {
         ui->style_recherche->setIcon(QIcon("C:/Users/ASUS/Desktop/style.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_idd_recherche_toggled(bool s)
{
    if(s)
    {
         ui->idd_recherche->setIcon(QIcon("C:/Users/ASUS/Desktop/id.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_suppression_a_toggled(bool m)
{
    if(m)
    {
         ui->suppression_a->setIcon(QIcon("C:/Users/ASUS/Desktop/mail.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_supprimer_deco_toggled(bool k)
{
    if(k)
    {
         ui->supprimer_deco->setIcon(QIcon("C:/Users/ASUS/Desktop/poubelle.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_update_s_toggled(bool h)
{
    if(h)
    {
         ui->update_s->setIcon(QIcon("C:/Users/ASUS/Desktop/update.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_trie_capacite_2_toggled(bool check)
{
    if(check)
    {
         ui->trie_capacite_2->setIcon(QIcon("C:/Users/ASUS/Desktop/capacite.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_trie_id_toggled(bool p)
{
    if(p)
    {
         ui->trie_id->setIcon(QIcon("C:/Users/ASUS/Desktop/id.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_trie_prix_toggled(bool checked)
{
    if(checked)
    {
         ui->trie_prix->setIcon(QIcon("C:/Users/ASUS/Desktop/money.jpg"));
    }
}

void MainWindow::on_trie_lieu_toggled(bool v)
{
    if(v)
    {
         ui->trie_lieu->setIcon(QIcon("C:/Users/ASUS/Desktop/loca.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_suppression_salle_toggled(bool chec)
{
    if(chec)
    {
         ui->suppression_salle->setIcon(QIcon("C:/Users/ASUS/Desktop/poubelle.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_reserver_salle_toggled(bool u)
{
    if(u)
    {
         ui->reserver_salle->setIcon(QIcon("C:/Users/ASUS/Desktop/reserver.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_salle_email_toggled(bool ch)
{
    if(ch)
    {
         ui->salle_email->setIcon(QIcon("C:/Users/ASUS/Desktop/update.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_backk_toggled(bool a)
{
    if(a)
    {
         ui->backk->setIcon(QIcon("C:/Users/ASUS/Desktop/retour.jpg"));
        qDebug() <<"Button checked-- ON--";
    }
}

void MainWindow::on_organisation_3_clicked()
{
    ui->pages->setCurrentIndex(4);
}

void MainWindow::on_organisation_2_clicked()
{
    ui->pages->setCurrentIndex(4);
}