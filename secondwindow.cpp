#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "QNetworkProxy"
#include "mainwindow.h"
#include "QSettings"
secondwindow::secondwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::secondwindow)
{
    ui->setupUi(this);
    QSettings *set_proxy = new QSettings("Setproxy.ini",QSettings::IniFormat,0);
     set_proxy->beginGroup("Setproxy");
  ui->lineEdit_4->setText(set_proxy->value("HostName","0").toString());
  ui->lineEdit_3->setText(set_proxy->value("Port","0").toString());
  ui->lineEdit_5->setText(set_proxy->value("User","0").toString());
  ui->lineEdit_6->setText(set_proxy->value("Password","0").toString());
  set_proxy->endGroup();

  QSettings *set = new QSettings ("setting.ini",QSettings::IniFormat,0);
  set->beginGroup("Settings");
  ui->lineEdit->setText( set->value("Login","0").toString());
  ui->lineEdit_2->setText(set->value("Pass","0").toString());
  set->endGroup();
}

secondwindow::~secondwindow()
{
    delete ui;
}

void secondwindow::on_checkBox_clicked(bool checked)
{
    if (checked)
    {
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(ui->lineEdit_4->text());
        proxy.setPort(ui->lineEdit_3->text().toInt());
        proxy.setUser(ui->lineEdit_5->text());
        proxy.setPassword(ui->lineEdit_6->text());
        QNetworkProxy::setApplicationProxy(proxy);

        QSettings *set_proxy = new QSettings("Setproxy.ini",QSettings::IniFormat,0);
        set_proxy->beginGroup("Setproxy");
        set_proxy->setValue("HostName",ui->lineEdit_4->text());
        set_proxy->setValue("Port",ui->lineEdit_3->text());
        set_proxy->setValue("User",ui->lineEdit_5->text());
        set_proxy->setValue("Password",ui->lineEdit_6->text());
        set_proxy->endGroup();
    }
    else
    {
        qDebug() << "else";
        QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);

    }
}

void secondwindow::on_pushButton_clicked()
{
    QSettings *set_proxy = new QSettings("Setproxy.ini",QSettings::IniFormat,0);
     set_proxy->beginGroup("Setproxy");
  ui->lineEdit_4->setText(set_proxy->value("HostName","0").toString());
  ui->lineEdit_3->setText(set_proxy->value("Port","0").toString());
  ui->lineEdit_5->setText(set_proxy->value("User","0").toString());
  ui->lineEdit_6->setText(set_proxy->value("Password","0").toString());
  set_proxy->endGroup();

  QSettings *set = new QSettings ("setting.ini",QSettings::IniFormat,0);
  set->beginGroup("Settings");
  ui->lineEdit->setText( set->value("Login","0").toString());
  ui->lineEdit_2->setText(set->value("Pass","0").toString());
  set->endGroup();

}

void secondwindow::on_pushButton_2_clicked()

{
    Login = ui->lineEdit->text();
    Pass = ui->lineEdit_2->text();

    QSettings *s = new QSettings("setting.ini",QSettings::IniFormat,0);
    s->beginGroup("Settings");
      s->setValue("Login", Login);
      s->setValue("Pass",  Pass);
      s->endGroup();

      // ui->lineEdit->setText(Login);
       //ui->lineEdit_2->setText(Pass);
}

