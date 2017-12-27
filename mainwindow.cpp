#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include <qprocess.h>
#include <qtextcodec.h>
#include <time.h>
#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  
    this->setWindowTitle(tr("哈夫曼压缩工具"));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_BT_Select_clicked()
{
    Init();
    memset(filename,NULL,256);
    memset(htfname,NULL,256);
    hfm=nullptr;

    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"),
                                                       NULL,
                                                       tr("anyFile (*)"));
    current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");

    ui->textEdit->append(current_date+"\t选择文件 "+fileName+"\n");
    ui->lineEdit->setText(fileName);
    QByteArray cdata = fileName.toLocal8Bit();
    string str=string(cdata);
    const char *pStr=str.data();
    strcpy(filename,pStr);

    statusBar()->showMessage(tr("打开文件中..."));
    double time_Start=(double)clock();
    if(OpenFile(filename,hfm)==false)
    {
        ui->textEdit->append(current_date+"\t打开文件失败！\n");
        statusBar()->showMessage(tr("打开文件失败"),3000);
        return;
    }
    double time_End=(double)clock();
    ui->textEdit->append(current_date+tr("\t成功打开 ")+fileName+"\n");
    QString t=QString::number((time_End-time_Start)/1000,10,3);
    statusBar()->showMessage(tr("打开文件完成,用时")+t+tr("s"),3000);
}

void MainWindow::on_BT_Compress_clicked()
{
    saveHfmFile();
    if(path=="")
    {
        QMessageBox::warning(this, tr("警告"),
                             tr("请选择一个huffman文件！"));
        statusBar()->showMessage(tr("没有选择压缩文件"),3000);
    }
    else
    {
        ui->ED_CmpName->setText(path);
        QByteArray cdata =path.toLocal8Bit();
        string str=string(cdata);
        const char *pStr=str.data();
        strcpy(htfname,pStr);

        statusBar()->showMessage(tr("文件压缩中..."));
        double time_Start=(double)clock();
        Compress(hfm,filename,htfname);
        double time_End=(double)clock();
        QString t=QString::number((time_End-time_Start)/1000,10,3);
        QMessageBox::information(this,tr("Information"),tr("文件压缩成功！"));
        current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
        ui->textEdit->append(current_date+tr("\t成功压缩文件 ")+path+"\n");
        statusBar()->showMessage(tr("文件压缩完成,用时")+t+tr("s"),3000);
    }

}

void MainWindow::saveHfmFile()
{
    path = QFileDialog::getSaveFileName(this,
                                       tr("保存文件"),
                                       ".",
                                       tr("Huffman Files(*.hfm)"));

    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            current_date_time = QDateTime::currentDateTime();
            QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
            ui->textEdit->append(current_date+"\t不能打开 "+path+"\n");
            QMessageBox::warning(this, tr("Write File"),
                                       tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        file.close();
    } else {
        QMessageBox::warning(this, tr("警告"),
                             tr("你还没有选择任何文件"));
    }
}

void MainWindow::on_BT_UnCompress_clicked()
{
    //选择hfm文件
    memset(htfname,NULL,256);
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"),
                                                       NULL,
                                                       tr("HuffmanFile(*.hfm)"));

    current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
    ui->textEdit->append(current_date+"\t打开文件 "+fileName+"\n");
    QByteArray cdata =fileName.toLocal8Bit();
    string str=string(cdata);
    const char *pStr=str.data();
    strcpy(htfname,pStr);


    //选择解压路径
    saveExtraceFile();

    if(path=="")
    {
        QMessageBox::warning(this, tr("警告"),
                             tr("请选择一个解压文件"));
        statusBar()->showMessage(tr("没有选择解压文件"),3000);
        return;
    }
    cdata=path.toLocal8Bit();
    str=string(cdata);
    ui->lineEdit_2->setText(tr(str.data()));
    current_date_time = QDateTime::currentDateTime();

    statusBar()->showMessage(tr("解压文件中..."));
    double time_Start=(double)clock();
    if(Extract(htfname,str)==false)
    {
        current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
        ui->textEdit->append(current_date+"\t文件解压失败！\n");

        QMessageBox::warning(this, tr("Warning"),
                             tr("文件解压失败！"));
        current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
       //ui->textEdit->append(current_date+"\tFaild to extrace huffman file\n");
        statusBar()->showMessage(tr("解压失败"),3000);
    }
    else{
        double time_End=(double)clock();
        QString t=QString::number((time_End-time_Start)/1000,10,3);
        current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
        ui->textEdit->append(current_date+"\t文件解压成功,保存路径为 "+path+"\n");
        QMessageBox::information(this,tr("提示"),tr("文件解压成功！"));
        statusBar()->showMessage(tr("解压文件完成,用时")+t+tr("s"),3000);
    }
}

void MainWindow::saveExtraceFile()
{
    path = QFileDialog::getSaveFileName(this,
                                       tr("保存文件"),
                                       ".",
                                       "");

    if(!path.isEmpty()) {
        QFile file(path);

        current_date_time = QDateTime::currentDateTime();
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {

            QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
            ui->textEdit->append(current_date+"\tCannot open file "+path+"\n");
            QMessageBox::warning(this, tr("Write File"),
                                       tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        file.close();
    } else {
        QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
        ui->textEdit->append(current_date+"\t没有选择任何文件\n");
        QMessageBox::warning(this, tr("警告"),
                             tr("没有选择任何文件！"));
    }
}

void MainWindow::on_pushButton_clicked()
{
    QFile logfile("log.txt");

    current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");

    QString log=ui->textEdit->toPlainText();
    if(!logfile.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        ui->textEdit->append(current_date+"\t打开log.txt失败\n");
        QMessageBox::warning(this, tr("Log"),
                                   tr("打开log.txt失败"));
    }
    else
    {
        logfile.write(log.toUtf8());
        logfile.close();
        ui->textEdit->append(current_date+"\t成功保存信息到log.txt\n");
        QMessageBox::warning(this, tr("Log"),
                                   tr("成功保存log!"));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QProcess* process = new QProcess();
    QString notepadPath = "notepad.exe  log.txt";
        process->start(notepadPath);
}
