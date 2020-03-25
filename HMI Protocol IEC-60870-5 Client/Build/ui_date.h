/********************************************************************************
** Form generated from reading UI file 'date.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATE_H
#define UI_DATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTimeEdit>

QT_BEGIN_NAMESPACE

class Ui_Date
{
public:
    QCalendarWidget *calendarWidget;
    QPushButton *Ok;
    QPushButton *Cancel;
    QLabel *label;
    QLabel *selectDate;
    QTimeEdit *timeEdit;
    QLabel *label_2;
    QLabel *selectTime;
    QLabel *label_3;
    QTimeEdit *timeEdit2;
    QLabel *selectTime_2;
    QLabel *selectTimeUntil;
    QComboBox *comboBox;
    QLabel *label_4;

    void setupUi(QDialog *Date)
    {
        if (Date->objectName().isEmpty())
            Date->setObjectName(QStringLiteral("Date"));
        Date->resize(667, 243);
        calendarWidget = new QCalendarWidget(Date);
        calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
        calendarWidget->setGeometry(QRect(40, 30, 296, 183));
        Ok = new QPushButton(Date);
        Ok->setObjectName(QStringLiteral("Ok"));
        Ok->setGeometry(QRect(530, 90, 121, 23));
        Cancel = new QPushButton(Date);
        Cancel->setObjectName(QStringLiteral("Cancel"));
        Cancel->setGeometry(QRect(530, 120, 121, 23));
        label = new QLabel(Date);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(400, 170, 71, 16));
        selectDate = new QLabel(Date);
        selectDate->setObjectName(QStringLiteral("selectDate"));
        selectDate->setGeometry(QRect(410, 190, 71, 16));
        timeEdit = new QTimeEdit(Date);
        timeEdit->setObjectName(QStringLiteral("timeEdit"));
        timeEdit->setGeometry(QRect(370, 110, 118, 22));
        timeEdit->setDateTime(QDateTime(QDate(2000, 1, 1), QTime(0, 0, 0)));
        timeEdit->setMinimumTime(QTime(0, 0, 0));
        timeEdit->setTime(QTime(0, 0, 0));
        label_2 = new QLabel(Date);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(410, 90, 51, 16));
        selectTime = new QLabel(Date);
        selectTime->setObjectName(QStringLiteral("selectTime"));
        selectTime->setGeometry(QRect(350, 210, 71, 16));
        label_3 = new QLabel(Date);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(406, 130, 61, 16));
        timeEdit2 = new QTimeEdit(Date);
        timeEdit2->setObjectName(QStringLiteral("timeEdit2"));
        timeEdit2->setGeometry(QRect(370, 150, 118, 22));
        timeEdit2->setDateTime(QDateTime(QDate(2000, 1, 1), QTime(0, 0, 0)));
        timeEdit2->setMinimumTime(QTime(0, 0, 0));
        timeEdit2->setTime(QTime(0, 0, 0));
        selectTime_2 = new QLabel(Date);
        selectTime_2->setObjectName(QStringLiteral("selectTime_2"));
        selectTime_2->setGeometry(QRect(430, 210, 16, 16));
        selectTimeUntil = new QLabel(Date);
        selectTimeUntil->setObjectName(QStringLiteral("selectTimeUntil"));
        selectTimeUntil->setGeometry(QRect(460, 210, 71, 16));
        comboBox = new QComboBox(Date);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(370, 60, 121, 22));
        label_4 = new QLabel(Date);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(410, 40, 51, 16));

        retranslateUi(Date);

        QMetaObject::connectSlotsByName(Date);
    } // setupUi

    void retranslateUi(QDialog *Date)
    {
        Date->setWindowTitle(QApplication::translate("Date", "Dialog", Q_NULLPTR));
        Ok->setText(QApplication::translate("Date", "OK", Q_NULLPTR));
        Cancel->setText(QApplication::translate("Date", "Cancel", Q_NULLPTR));
        label->setText(QApplication::translate("Date", "Selected Date", Q_NULLPTR));
        selectDate->setText(QApplication::translate("Date", "XXXX-XX-XX", Q_NULLPTR));
        timeEdit->setDisplayFormat(QApplication::translate("Date", "hh:mm:ss", Q_NULLPTR));
        label_2->setText(QApplication::translate("Date", "Mulai jam", Q_NULLPTR));
        selectTime->setText(QApplication::translate("Date", "XXXX-XX-XX", Q_NULLPTR));
        label_3->setText(QApplication::translate("Date", "Sampai jam", Q_NULLPTR));
        timeEdit2->setDisplayFormat(QApplication::translate("Date", "hh:mm:ss", Q_NULLPTR));
        selectTime_2->setText(QApplication::translate("Date", "  -", Q_NULLPTR));
        selectTimeUntil->setText(QApplication::translate("Date", "XXXX-XX-XX", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("Date", "PLTMH", Q_NULLPTR)
         << QApplication::translate("Date", "PLTS", Q_NULLPTR)
         << QApplication::translate("Date", "LOAD", Q_NULLPTR)
         << QApplication::translate("Date", "INVERTER", Q_NULLPTR)
        );
        label_4->setText(QApplication::translate("Date", "Pilih data", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Date: public Ui_Date {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATE_H
