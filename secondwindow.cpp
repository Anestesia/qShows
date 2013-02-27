#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "QNetworkProxy"

secondwindow::secondwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::secondwindow)
{
    ui->setupUi(this);
}

secondwindow::~secondwindow()
{
    delete ui;
}

void secondwindow::on_checkBox_clicked(bool checked)
{
    if (checked)
    { QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(ui->lineEdit_4->text());
        proxy.setPort(ui->spinBox->value());
        proxy.setUser(ui->lineEdit_5->text());
        proxy.setPassword(ui->lineEdit_6->text());
        QNetworkProxy::setApplicationProxy(proxy);
    }
    else
    {
        qDebug() << "else";
        QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);

    }
}

void secondwindow::on_pushButton_clicked()
{
    this->hide();
}
