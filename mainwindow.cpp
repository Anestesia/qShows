#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_secondwindow.h"
#include "QString"
#include "QUrl"
#include "QLabel"
#include "qxtjson.h"
#include "title.h"
#include "serie.h"
#include "QNetworkProxy"
#include "QMessageBox"
#include "QFileDialog"
#include "md5.h"
#include <QObject>
#include "secondwindow.h"
#include <QFile>
#include "QSettings"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    firstPic = true;
    isLogged=false;
    firstPush = true;
    ui->centralWidget->setWindowTitle(QString::fromUtf8("Serial detective"));
    ui->statusBar->showMessage(APP_REVISION);
    ui->statusBar->show();
    doLogin();
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    secondwindow *Settings = new secondwindow(this);
    Settings->setModal(true);
    Settings->show();
}

void MainWindow::doLogin(QString Login, QString Pass)
{
    login = Login;
    hash = MD5(Pass);
    url.setUrl("http://api.myshows.ru/profile/login?login="+login+"&password="+hash);
    request.setUrl(url);
    reply = networkManager.get(request);
    if (firstPush)
    {
        connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onLoginResult(QNetworkReply*)));
    }
    firstPush=false;
}

void MainWindow::doLogin()
{
    QSettings *s = new QSettings ("setting.ini",QSettings::IniFormat,0);
    s->beginGroup("Settings");
    login = s->value("Login","0").toString();
    QString pass = s->value("Pass","0").toString();
    s->endGroup();
    doLogin(login,pass);
}

void MainWindow::onLoginResult(QNetworkReply *reply)
{
    int v = (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)).toInt();
    if (v==200)
    {
        isLogged = true;
    }
    disconnect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onLoginResult(QNetworkReply*)));
}

void MainWindow::GetSerialsByName()
{
    //if (firstPush)
    //{
        connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResult(QNetworkReply*)));
    //}
    firstPush=false;
    qDebug()<<ui->lineEdit->text();
    QString uri = "http://api.myshows.ru/shows/search/?q=";
    uri.append(ui->lineEdit->text());
    url.setUrl(uri);
    request.setUrl(url);
    reply = networkManager.get(request);
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem(QString::fromUtf8("Title")));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem(QString::fromUtf8("Id")));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem(QString::fromUtf8("Year")));
    ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem(QString::fromUtf8("Ended")));
}

void MainWindow::renewTable()
{
    ui->tableWidget->setRowCount(0);
    for (int i=0;i<titles.size();i++)
    {
        QLabel *label = new QLabel(this);
        label->setOpenExternalLinks(true);
        label->setTextFormat(Qt::RichText);
        label->setTextInteractionFlags(Qt::TextBrowserInteraction);
        label->setProperty("row",i);
        label->setProperty("column",1);

        QString text(QString::fromUtf8("<a href=\"http://myshows.ru/view/"));
        text.append(QString::number(titles[i].id));
        text.append(QString::fromUtf8("/\">myshows.ru<\\a>"));

        label->setText(text);

        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(titles[i].uktitle));
        ui->tableWidget->setCellWidget(i,1,label);
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(titles[i].year)));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(titles[i].ended));
    }
}

void MainWindow::onResult(QNetworkReply* reply)
{
    const QByteArray rawdata = reply->readAll();
    qDebug()<<rawdata;
    QxtJSON parser = QxtJSON();
    QVariantMap map = parser.parse(QString::fromUtf8(rawdata.data())).toMap();
    QMapIterator<QString, QVariant> it(map);
    while (it.hasNext())
    {
        it.next();
        title tit;
        tit.id = it.key().toInt();
        QVariantMap map2 = it.value().toMap();
        tit.country = map2["country"].toString();
        tit.uktitle = map2["title"].toString();
        tit.ended = map2["ended"].toString();
        tit.year = map2["year"].toInt();
        tit.image = map2["image"].toString();
        tit.genres = map2["genres"].toString();
        tit.imdbid = map2["imdbid"].toInt();
        tit.kinopoiskid = map2["kinopoiskid"].toInt();
        tit.rating = map2["rating"].toFloat();
        tit.status = map2["status"].toString();
        this->titles.append(tit);
    }
    disconnect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResult(QNetworkReply*)));
    renewTable();
}


void MainWindow::onFileResult(QNetworkReply* reply)
{
    if (titles.size()>=1)
    {
        titles.remove(0,titles.size());
    }
    const QByteArray rawdata = reply->readAll();
    QxtJSON parser = QxtJSON();
    QVariantMap map = parser.parse(QString::fromUtf8(rawdata.data())).toMap();
    QVariant match = map["match"];
    if (match.toInt()==85 || match.toInt()==100)
    {
        title tit;
        QVariantMap map2 = map["show"].toMap();
        QVariant country = map2["country"];
        tit.country = country.toString();
        QVariant title = map2["title"];
        tit.id = map2["id"].toInt();
        tit.uktitle = title.toString();
        QVariant year = map2["year"];
        QVariant ended = map2["ended"];
        tit.ended = ended.toString();
        tit.year = year.toInt();
        tit.image = map2["image"].toString();
        this->titles.append(tit);
    }
    disconnect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFileResult(QNetworkReply*)));
    renewTable();
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if (titles[row].image!="")
    {
        QUrl imageUrl(titles[row].image);
        m_pImgCtrl = new FileDownloader(imageUrl, this);
    }
    {
        connect(m_pImgCtrl,SIGNAL(downloaded()),SLOT(loadImage()));
    }
    firstPic=false;
}

void MainWindow::loadImage()
{
    QPixmap poster;
    poster.loadFromData(m_pImgCtrl->downloadedData());
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addPixmap(poster);
    ui->graphicsView->setScene(scene);
}

void MainWindow::GetSerialsByFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.*)"));
    if (firstPush)
    {
        connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFileResult(QNetworkReply*)));
    }
    firstPush=false;
    QFileInfo fi(fileName);
    QString base = fi.fileName();
    url.setUrl("http://api.myshows.ru/shows/search/file/?q="+base);
    request.setUrl(url);
    reply = networkManager.get(request);
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem(QString::fromUtf8("Title")));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem(QString::fromUtf8("Id")));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem(QString::fromUtf8("Year")));
    ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem(QString::fromUtf8("Ended")));
};

void MainWindow::on_pushButton_2_clicked()
{
    GetSerialsByFile();
}


void MainWindow::on_pushButton_4_clicked()
{
    qDebug()<<login;
    ui->tableWidget_2->clear();
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(4);
    ui->tableWidget_2->setHorizontalHeaderItem(0,new QTableWidgetItem(QString::fromUtf8("Title")));
    ui->tableWidget_2->setHorizontalHeaderItem(1,new QTableWidgetItem(QString::fromUtf8("Serial")));
    ui->tableWidget_2->setHorizontalHeaderItem(2,new QTableWidgetItem(QString::fromUtf8("Air Date")));
    ui->tableWidget_2->setHorizontalHeaderItem(3,new QTableWidgetItem(QString::fromUtf8("Season")));
    if (isLogged)
    {
        url.setUrl("http://api.myshows.ru/profile/episodes/unwatched/");
        connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(GetSeries(QNetworkReply*)));
        request.setUrl(url);
        reply = networkManager.get(request);
    }
    else
    {
        doLogin();
    }

}

void MainWindow::GetSeries(QNetworkReply* reply)
{
    const QByteArray rawdata = reply->readAll();
    qDebug()<<rawdata;
    QxtJSON parser = QxtJSON();

    QVariantMap map = parser.parse(QString::fromUtf8(rawdata.data())).toMap();
    QMapIterator<QString, QVariant> it(map);
    while (!series.empty())
    {
        series.remove(0);
    }
    while (it.hasNext())
    {
        it.next();
        serie ep;
        ep.episodeId = it.key().toInt();
        QVariantMap map2 = it.value().toMap();
        ep.airDate = map2["airDate"].toString();
        ep.episodeNumber = map2["episodeNumber"].toInt();
        ep.seasonNumber = map2["seasonNumber"].toInt();
        ep.showId = map2["showId"].toInt();
        ep.title = map2["title"].toString();
        this->series.append(ep);
    }
    renewEpTable();
}

void MainWindow::renewEpTable()
{
    ui->tableWidget_2->setRowCount(0);
    for (int i=0;i<series.size();i++)
    {
        QLabel *label = new QLabel(this);
        label->setOpenExternalLinks(true);
        label->setTextFormat(Qt::RichText);
        label->setTextInteractionFlags(Qt::TextBrowserInteraction);
        label->setProperty("row",i);
        label->setProperty("column",1);
        QString text(QString::fromUtf8("<a href=\"http://myshows.ru/view/"));
        text.append(QString::number(series[i].showId));
        text.append(QString::fromUtf8("/\">myshows.ru<\\a>"));
        label->setText(text);
        ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);
        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(series[i].title));
        ui->tableWidget_2->setCellWidget(i,1,label);
        ui->tableWidget_2->setItem(i,2,new QTableWidgetItem(series[i].airDate));
        ui->tableWidget_2->setItem(i,3,new QTableWidgetItem(QString::number(series[i].seasonNumber)));
    }
}



void MainWindow::on_lineEdit_lostFocus()
{
}

void MainWindow::on_lineEdit_returnPressed()
{
    GetSerialsByName();
}
