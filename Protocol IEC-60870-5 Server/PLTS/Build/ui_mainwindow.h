/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *lIP;
    QLineEdit *leIPRemoto;
    QLabel *label;
    QLineEdit *leDbHost;
    QLabel *label_2;
    QLineEdit *leDbName;
    QLabel *label_4;
    QLineEdit *leDbPass_2;
    QLabel *label_3;
    QLineEdit *leDbPass;
    QPushButton *pbConnect_2;
    QLabel *label_6;
    QLabel *label_status;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *lDb;
    QLineEdit *leAsduAddr;
    QLabel *lIP_2;
    QSpinBox *sb;
    QLabel *lDb_2;
    QLineEdit *leMasterAddr;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(271, 384);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(0, 120));
        MainWindow->setMaximumSize(QSize(16000, 16000));
        QIcon icon;
        icon.addFile(QStringLiteral("../Hmi_IEC_104/img/LEN.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 130, 251, 205));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lIP = new QLabel(groupBox);
        lIP->setObjectName(QStringLiteral("lIP"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lIP->sizePolicy().hasHeightForWidth());
        lIP->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(lIP, 0, 0, 1, 1);

        leIPRemoto = new QLineEdit(groupBox);
        leIPRemoto->setObjectName(QStringLiteral("leIPRemoto"));
        sizePolicy.setHeightForWidth(leIPRemoto->sizePolicy().hasHeightForWidth());
        leIPRemoto->setSizePolicy(sizePolicy);

        gridLayout->addWidget(leIPRemoto, 0, 1, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        leDbHost = new QLineEdit(groupBox);
        leDbHost->setObjectName(QStringLiteral("leDbHost"));
        sizePolicy.setHeightForWidth(leDbHost->sizePolicy().hasHeightForWidth());
        leDbHost->setSizePolicy(sizePolicy);

        gridLayout->addWidget(leDbHost, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        leDbName = new QLineEdit(groupBox);
        leDbName->setObjectName(QStringLiteral("leDbName"));
        sizePolicy.setHeightForWidth(leDbName->sizePolicy().hasHeightForWidth());
        leDbName->setSizePolicy(sizePolicy);

        gridLayout->addWidget(leDbName, 2, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        leDbPass_2 = new QLineEdit(groupBox);
        leDbPass_2->setObjectName(QStringLiteral("leDbPass_2"));
        sizePolicy.setHeightForWidth(leDbPass_2->sizePolicy().hasHeightForWidth());
        leDbPass_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(leDbPass_2, 3, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        leDbPass = new QLineEdit(groupBox);
        leDbPass->setObjectName(QStringLiteral("leDbPass"));
        sizePolicy.setHeightForWidth(leDbPass->sizePolicy().hasHeightForWidth());
        leDbPass->setSizePolicy(sizePolicy);

        gridLayout->addWidget(leDbPass, 4, 1, 1, 1);

        pbConnect_2 = new QPushButton(groupBox);
        pbConnect_2->setObjectName(QStringLiteral("pbConnect_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pbConnect_2->sizePolicy().hasHeightForWidth());
        pbConnect_2->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(pbConnect_2, 5, 1, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 6, 0, 1, 1);

        label_status = new QLabel(groupBox);
        label_status->setObjectName(QStringLiteral("label_status"));

        gridLayout->addWidget(label_status, 6, 1, 1, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 251, 111));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        lDb = new QLabel(groupBox_2);
        lDb->setObjectName(QStringLiteral("lDb"));
        sizePolicy.setHeightForWidth(lDb->sizePolicy().hasHeightForWidth());
        lDb->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(lDb, 0, 0, 1, 1);

        leAsduAddr = new QLineEdit(groupBox_2);
        leAsduAddr->setObjectName(QStringLiteral("leAsduAddr"));
        sizePolicy.setHeightForWidth(leAsduAddr->sizePolicy().hasHeightForWidth());
        leAsduAddr->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(leAsduAddr, 0, 1, 1, 1);

        lIP_2 = new QLabel(groupBox_2);
        lIP_2->setObjectName(QStringLiteral("lIP_2"));
        sizePolicy1.setHeightForWidth(lIP_2->sizePolicy().hasHeightForWidth());
        lIP_2->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(lIP_2, 2, 0, 1, 1);

        sb = new QSpinBox(groupBox_2);
        sb->setObjectName(QStringLiteral("sb"));
        sizePolicy.setHeightForWidth(sb->sizePolicy().hasHeightForWidth());
        sb->setSizePolicy(sizePolicy);
        sb->setMaximum(1);
        sb->setValue(1);

        gridLayout_2->addWidget(sb, 2, 1, 1, 1);

        lDb_2 = new QLabel(groupBox_2);
        lDb_2->setObjectName(QStringLiteral("lDb_2"));
        sizePolicy.setHeightForWidth(lDb_2->sizePolicy().hasHeightForWidth());
        lDb_2->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(lDb_2, 3, 0, 1, 1);

        leMasterAddr = new QLineEdit(groupBox_2);
        leMasterAddr->setObjectName(QStringLiteral("leMasterAddr"));
        sizePolicy.setHeightForWidth(leMasterAddr->sizePolicy().hasHeightForWidth());
        leMasterAddr->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(leMasterAddr, 3, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(sb, leDbHost);
        QWidget::setTabOrder(leDbHost, leDbName);
        QWidget::setTabOrder(leDbName, leDbPass_2);
        QWidget::setTabOrder(leDbPass_2, leDbPass);
        QWidget::setTabOrder(leDbPass, pbConnect_2);
        QWidget::setTabOrder(pbConnect_2, leIPRemoto);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Protocol IEC 60870-5-104", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "DATABASE CONFIGURATION", Q_NULLPTR));
        lIP->setText(QApplication::translate("MainWindow", "IP Address", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Database Host", Q_NULLPTR));
        leDbHost->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "Database Name", Q_NULLPTR));
        leDbName->setText(QString());
        label_4->setText(QApplication::translate("MainWindow", "Username", Q_NULLPTR));
        leDbPass_2->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "Password", Q_NULLPTR));
        leDbPass->setText(QString());
        pbConnect_2->setText(QApplication::translate("MainWindow", "Save Config", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Status Database :", Q_NULLPTR));
        label_status->setText(QApplication::translate("MainWindow", "-", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "RTU CONFIGURATION", Q_NULLPTR));
        lDb->setText(QApplication::translate("MainWindow", "ASDU addr/Sector", Q_NULLPTR));
        leAsduAddr->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lIP_2->setText(QApplication::translate("MainWindow", "Mode", Q_NULLPTR));
        lDb_2->setText(QApplication::translate("MainWindow", "Local Link Addr (OA)", Q_NULLPTR));
        leMasterAddr->setText(QApplication::translate("MainWindow", "2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
