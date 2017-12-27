#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "compress.h"
#include <QMessageBox>
#include <qtime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void saveHfmFile();
    void saveExtraceFile();

private slots:
    void on_BT_Select_clicked();

    void on_BT_Compress_clicked();

    void on_BT_UnCompress_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    char sel;
    char filename[256];
    char htfname[256];
    HuffTree hfm;
    QString path;
    QDateTime current_date_time;
};

#endif // MAINWINDOW_H
