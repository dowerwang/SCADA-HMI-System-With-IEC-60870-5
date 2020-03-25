#ifndef DATE_H
#define DATE_H

#include <QDialog>
#include <QTime>

namespace Ui {
class Date;
}

class Date : public QDialog
{
    Q_OBJECT

public:
    explicit Date(QWidget *parent = 0);
    ~Date();
    int valueReturn() const {return value;}
    bool seharianReturn() const { return seharian;}
    QString data1() const {return timechoose1;}
    QString date2() const {return timechoose2;}
    QString tanggal() const {return datechoose;}
    int pilihan() const {return dataPilihan;}
private slots:
    void on_calendarWidget_selectionChanged();

    void on_calendarWidget_clicked(const QDate &date);

    void on_Ok_clicked();

    void on_Cancel_clicked();

    void on_timeEdit_timeChanged(const QTime &time);

    void on_timeEdit_2_editingFinished();

    void on_timeEdit_2_timeChanged(const QTime &time);

    void on_timeEdit2_timeChanged(const QTime &time);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Date *ui;
    QString datechoose,timechoose1,timechoose2;
    int value,dataPilihan;
    bool seharian;
    QTime times1,times2;
};

#endif // DATE_H
