#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>

namespace Ui {
class secondwindow;
}

class secondwindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit secondwindow(QWidget *parent = 0);
    ~secondwindow();
    
private slots:
    void on_checkBox_clicked(bool checked);

    void on_pushButton_clicked();

private:
    Ui::secondwindow *ui;
};

#endif // SECONDWINDOW_H
