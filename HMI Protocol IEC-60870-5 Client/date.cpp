#include "date.h"
#include "ui_date.h"
#include <QMessageBox>

Date::Date(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Date)
{
    ui->setupUi(this);
    dataPilihan = 0;
}

Date::~Date()
{
    delete ui;
}

void Date::on_calendarWidget_selectionChanged()
{

}

void Date::on_calendarWidget_clicked(const QDate &date)
{
    datechoose = date.toString("yyyy-MM-dd");
    ui->selectDate->setText(datechoose);
}

void Date::on_Ok_clicked()
{
    timechoose1 = times1.toString("hh:mm:ss");
    timechoose2 = times2.toString("hh:mm:ss");
    if(((timechoose1 != "") && (timechoose2 != "")))
    {
        if(times1 > times2){
            QMessageBox::information(this,"Error","Time 2 harus lebih besar");
        }
        else{
            //pengecekan tanggal
            if(datechoose != ""){
                seharian = false;
                value = 1;
                this->close();
            }
            else{
                QMessageBox::information(this,"Error","Tanggal Harus di pilih");
            }
        }
    }else{
        if(datechoose != ""){
            seharian = true;
            value = 1;
            this->close();
        }else{
            QMessageBox::information(this,"Error","Tanggal harus di pilih");
        }
    }


}

void Date::on_Cancel_clicked()
{
    value = 0;
    this->close();
}

void Date::on_timeEdit_timeChanged(const QTime &time)
{
    times1 = time;
    QString timeHMI = time.toString("hh:mm:ss");
    ui->selectTime->setText(timeHMI);
}

void Date::on_timeEdit_2_editingFinished()
{

}

void Date::on_timeEdit_2_timeChanged(const QTime &time)
{

}

void Date::on_timeEdit2_timeChanged(const QTime &time)
{
    times2 = time;
    QString timeHMI = time.toString("hh:mm:ss");
    ui->selectTimeUntil->setText(timeHMI);
}

void Date::on_comboBox_currentIndexChanged(int index)
{
    dataPilihan = index;
}
