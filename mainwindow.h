#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtNetwork/QNetworkReply"
#include <title.h>
#include "serie.h"
#include "filedownloader.h"
#include "secondwindow.h"
#include "QSqlDatabase"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public slots:
    void onResult(QNetworkReply*);
    void onFileResult(QNetworkReply*);
    void onLoginResult(QNetworkReply*);
    void GetSeries(QNetworkReply*);
    void GetSerialsByName();

public:

    secondwindow *f2;
    explicit MainWindow(QWidget *parent = 0);
    QNetworkAccessManager networkManager;
    QVector<title> titles;
    QVector<serie> series;
    QString login;
    QString hash;
    void renewTable();
    QUrl url;
    QNetworkRequest request;
    QNetworkReply *reply;
    ~MainWindow();

private:
    QSqlDatabase dbase;
    bool isOpened;
    bool isLogged;
    FileDownloader *m_pImgCtrl;
    bool firstPic;
    void renewEpTable();

    void GetSerialsByFile();
    bool firstPush;
    void doLogin(QString login, QString pass);
private slots:
    void on_pushButton_clicked();

    void loadImage();

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_2_clicked();

   // void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_pushButton_5_clicked();

    //void on_lineEdit_4_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
