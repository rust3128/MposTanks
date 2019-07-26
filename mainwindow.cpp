#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LoggingCategories/loggingcategories.h"
#include <QDateTime>
#include <QDir>
#include <QDesktopServices>
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;


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
    ui->mainToolBar->addWidget(ui->toolButtonPrint);
    ui->mainToolBar->addSeparator();
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



void MainWindow::on_toolButtonPrint_clicked()
{
    QString printStr;
        if (ui->tableView) {

            QTextStream out(&printStr);

            const int rowCount = ui->tableView->model()->rowCount();
            const int columnCount = ui->tableView->model()->columnCount();

            out <<  "<html>\n"
                    "<head>\n"
                    "<meta Content=\"Text/html; charset=utf-8\">\n"
                <<  QString("<title>%1</title>\n").arg("Данные на "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"))
                <<  "<style>"
                    "p {"
                    "font-family: Verdana, Arial, Helvetica, sans-serif; "
                      "font-size: 6pt;"
                     "}"
                    "</style>"
                <<  "</head>\n"
                    "<body bgcolor=#ffffff link=#5000A0>\n"
                <<  QString("<h2 align='center'>%1</h2>").arg("Данные на "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"))
                <<    "<table border=1 cellspacing=0 cellpadding=2>\n";

            // headers
            out << "<tr bgcolor=#f0f0f0>";
            for (int column = 0; column < columnCount; column++)
                if (!ui->tableView->isColumnHidden(column))
                    out << QString("<th>%1</th>").arg(ui->tableView->model()->headerData(column, Qt::Horizontal).toString());
            out << "</tr>\n";
            for (int row = 0; row < rowCount; row++) {
                out << "<tr>";
                for (int column = 0; column < columnCount; column++) {
                    if (!ui->tableView->isColumnHidden(column)) {
                        QString data = ui->tableView->model()->data(ui->tableView->model()->index(row, column)).toString().simplified();

                        out << QString("<td align='right' bkcolor=0><p>%1</p></td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                    }
                }
                out << "</tr>\n";
            }
            out <<  "</table>\n"
                "</body>\n"
                "</html>\n";
        }
        document = new QTextDocument();
        document->setHtml(printStr);
        printer.setOrientation(QPrinter::Landscape);
        QPrintPreviewDialog  *prDialog = new QPrintPreviewDialog(&printer, this);
        connect(prDialog,SIGNAL(paintRequested(QPrinter*)),this,SLOT(printPreviewNeeds(QPrinter*)));

        prDialog->showMaximized();
        prDialog->exec();
}

void MainWindow::printPreviewNeeds(QPrinter *)
{
    document->print(&printer);
}

void MainWindow::on_toolButtonExcel_clicked()
{
    QXlsx::Document xlsx;
    Format formatTitle, formatFuelsAZS, formatFuelSklad;

    formatTitle.setHorizontalAlignment(Format::AlignHCenter);
    formatTitle.setVerticalAlignment(Format::AlignVCenter);
    formatTitle.setFontBold(true);

    formatFuelsAZS.setPatternBackgroundColor(QColor("#A9BCF5"));
    formatFuelsAZS.setNumberFormatIndex(2);
    formatFuelSklad.setPatternBackgroundColor(QColor("#F78181"));
    formatFuelSklad.setNumberFormatIndex(2);




    const int columnCount = headers.size()+1;
    const int rowCount = model->rowCount()+3;


//    xlsx.mergeCells(CellRange(1,1,2,5));

    xlsx.setRowHeight(2,30);
    for(int i =1; i<columnCount; ++i){
        if(i <= 4 || i == 14 ){
            xlsx.write(1,i, headers.at(i-1),formatTitle);
            xlsx.mergeCells(CellRange(1,i,2,i));
            continue;
        }
        if(i >= 5 && i <= 13) {
            xlsx.write(2,i, headers.at(i-1),formatTitle);
        }
    }
    xlsx.mergeCells("E1:H1");
    xlsx.write("E1","АЗС",formatTitle);
    xlsx.mergeCells("I1:M1");
    xlsx.write("I1","СКЛАД",formatTitle);

    for(int row = 3; row < rowCount; ++row){
        for(int col =1; col<columnCount; ++col){
            if(col >=5 && col <= 8 ) {
                xlsx.write(row,col, model->data(model->index(row-3,col-1),Qt::DisplayRole).toFloat(),formatFuelsAZS);
                continue;
            }
            if(col >=9 && col <= 13 ) {
                xlsx.write(row,col, model->data(model->index(row-3,col-1),Qt::DisplayRole).toFloat(),formatFuelSklad);
                continue;
            }
            if(col == 2){
                xlsx.write(row,col, model->data(model->index(row-3,col-1),Qt::DisplayRole).toDateTime().toString("dd.MM.yyyy hh:mm:ss"));
                continue;
            }
            if(col == 14){
                xlsx.write(row,col, model->data(model->index(row-3,col-1),Qt::DisplayRole).toFloat());
                continue;
            }
            xlsx.write(row,col, model->data(model->index(row-3,col-1),Qt::DisplayRole).toString());
        }
    }




    QString fn = QDir::currentPath() + "//" + "Tanks"+QDateTime::currentDateTime().toString("yyyyMMddhhmm")+".xlsx";
    xlsx.saveAs(fn); // save the document as 'Test.xlsx'
    QDesktopServices::openUrl(QUrl("file://"+fn, QUrl::TolerantMode));



}
