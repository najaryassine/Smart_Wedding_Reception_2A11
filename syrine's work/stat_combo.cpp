#include "stat_combo.h"
#include "ui_stat_combo.h"

stat_combo::stat_combo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stat_combo)
{
    ui->setupUi(this);
}

stat_combo::~stat_combo()
{
    delete ui;
}


//bar chart
void stat_combo::choix_bar()
{
QSqlQuery q1,q2,q3;
qreal tot=0,c1=0,c2=0;

q1.prepare("SELECT * FROM TACHE");
q1.exec();

q2.prepare("SELECT * FROM TACHE WHERE COMBO='REALISE'");
q2.exec();

q3.prepare("SELECT * FROM CRUD WHERE COMBO='NON REALISE'");
q3.exec();

while (q1.next()){tot++;}
while (q2.next()){c1++;}
while (q3.next()){c2++;}

c1=c1/tot;
c2=c2/tot;



// Assign names to the set of bars used
        QBarSet *set0 = new QBarSet("Realise");
        QBarSet *set1 = new QBarSet("non realise");

        // Assign values for each bar
        *set0 << c1;
        *set1 << c2;




        // Add all sets of data to the chart as a whole
        // 1. Bar Chart
        QBarSeries *series = new QBarSeries();

        // 2. Stacked bar chart
        series->append(set0);
        series->append(set1);



        // Used to define the bar chart to display, title
        // legend,
        QChart *chart = new QChart();

        // Add the chart
        chart->addSeries(series);


        // Adds categories to the axes
       // QBarCategoryAxis *axis = new QBarCategoryAxis();



        // 1. Bar chart
       // chart->setAxisX(axis, series);

        // Used to change the palette
        QPalette pal = qApp->palette();

        // Change the color around the chart widget and text
        pal.setColor(QPalette::Window, QRgb(0xffffff));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));

        // Apply palette changes to the application
        qApp->setPalette(pal);


// Used to display the chart
chartView = new QChartView(chart,ui->label_stat);
chartView->setRenderHint(QPainter::Antialiasing);
chartView->setMinimumSize(400,400);

chartView->show();
}

//pie chart
    void stat_combo::choix_pie()
    {
    QSqlQuery q1,q2,q3;
    qreal tot=0,c1=0,c2=0;

    q1.prepare("SELECT * FROM TACHE");
    q1.exec();

    q2.prepare("SELECT * FROM CRUD WHERE COMBO='REALISE'");
    q2.exec();

    q3.prepare("SELECT * FROM CRUD WHERE COMBO='NON REALISE'");
    q3.exec();



    while (q1.next()){tot++;}
    while (q2.next()){c1++;}
    while (q3.next()){c2++;}


    c1=c1/tot;
    c2=c2/tot;


    // Define slices and percentage of whole they take up
    QPieSeries *series = new QPieSeries();
    series->append("REALISE",c1);
    series->append("NON REALISE",c2);





    // Create the chart widget
    QChart *chart = new QChart();

    // Add data to chart with title and show legend
    chart->addSeries(series);
    chart->legend()->show();


    // Used to display the chart
    chartView = new QChartView(chart,ui->label_stat);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400,400);

    chartView->show();
    }

