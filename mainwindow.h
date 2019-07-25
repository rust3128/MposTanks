#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "mymodel.h"

#include <QSortFilterProxyModel>
#include <QTimer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();
    void on_spinBox_valueChanged(int interval);

private:
    void createUI();
    void createModel();
private:
    Ui::MainWindow *ui;
    QSqlQueryModel *model;
    MyModel *colorModel;

    QTimer *timer;
    QStringList headers;


};

#endif // MAINWINDOW_H
