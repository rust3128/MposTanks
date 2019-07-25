#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LoggingCategories/loggingcategories.h"
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);




    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::on_pushButton_clicked);
    timer->start(ui->spinBox->value()*60000); // И запустим таймер
    headers << "АЗС"
            << "Отзвон"
            << "Топливо"
            << "№\nрез."
            << "Книга\nНачало"
            << "Приход"
            << "Расход"
            << "Остаток"
            << "Книга\nНачало"
            << "Приход"
            << "Расход"
            << "Остаток"
            << "Добавить\nПриход"
            << "Всего";

    ui->mainToolBar->addWidget(ui->toolButtonExcel);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(ui->labelRefresh);
    ui->mainToolBar->addWidget(ui->spinBox);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(ui->pushButton);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(ui->labelInfo);
    createModel();
    createUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUI()
{
    ui->tableView->setModel(colorModel);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->verticalHeader()->setDefaultSectionSize(ui->tableView->verticalHeader()->minimumSectionSize());
    ui->tableView->resizeColumnsToContents();
    ui->labelInfo->setText("Последнее обновение: "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));

}

void MainWindow::createModel()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM P1C_GET_FUELS_REST");
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    colorModel = new MyModel();
    colorModel->setSourceModel(model);
}

void MainWindow::on_pushButton_clicked()
{
    model->setQuery("SELECT * FROM P1C_GET_FUELS_REST");
    ui->labelInfo->setText("Последнее обновение: "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
}


void MainWindow::on_spinBox_valueChanged(int interval)
{
    timer->stop();
    timer->setInterval(interval*60000);
    timer->start();
}


