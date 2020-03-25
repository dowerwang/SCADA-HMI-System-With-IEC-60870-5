#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "drawing.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QImage>
#include <QMessageBox>
#include <QElapsedTimer>
#include "QDialog"
#include "QIODevice"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ui->stackedWidget->setEnabled(0);
    QString style = "QMainWindow { background-color: #fff; }";  // or other color
    this->setStyleSheet(style); // assuming you
    ui->twStatusMetering->setIconSize(QSize(100, 100));
    init();



    set_menu(0,true);
    //ListSignal();
    timer = new QTimer();
    refresh = new QTimer();
    timerPing = new QTimer();
    //date time
    connect(refresh,SIGNAL(timeout()), this, SLOT(waktu()));


    connect(refresh, SIGNAL(timeout()), this, SLOT(RealtimeDB()));

    connect(timer, SIGNAL(timeout()), this, SLOT(view_curve()));

    connect(timerPing, SIGNAL(timeout()), this, SLOT(pingHost()));
    //read dataout
    connect(timer,SIGNAL(timeout()),this,SLOT(ReadAllDataFrmDB()));
    //disable button when invalid
    //  connect(refresh, SIGNAL(timeout()), this, SLOT(()));
    connect(ui->cb_Startup, SIGNAL(stateChanged(int)), this, SLOT(AutoStartupOnWindows()));

    // QSlider *slider = new QSlider(Qt::Horizontal, this);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(value(int)));
    connect(ui->horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(value2(int)));

    connect(this, SIGNAL(triggerClose()), this, SLOT(close()));
    refresh->start(); // Interval 0 means to refresh as fast as possible
    timer->start(1000);
    timerPing->start(10000);
    ui->pbTrenReak->setEnabled(false);
    ui->pbTrenHis->setEnabled(true);
    realChart = 0;
    connectToDatabase2 =false;
    nilaiVarAuto = 0;
    for(int b = 1; b <=5;b++){
        for (int a=1; a<24; a++)
        {
            ui->twStatusMetering->item(a,b)->setText("-");
        }
    }
}

MainWindow::~MainWindow(){delete ui;}

void MainWindow::Index()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void MainWindow::Indexplts()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->action->setText("PLTS");
}

void MainWindow::init()
{
    view_curve();
    View_TrendingUI();
    //  ui->stackedWidget->setCurrentIndex(11);

    //validator hanya angka aja ga bisa huruf
    QRegExp rx("[0-9]\\d{1,5}");
    QRegExp rx2("[0-9]\\d{0,5}\\.\\d{1,5}");
    QValidator *valid = new QRegExpValidator(rx, this);
    QValidator *valid2 = new QRegExpValidator(rx2, this);
    ui->leInvC1->setValidator(valid);
    ui->leInvC2->setValidator(valid);
    ui->leInvM1->setValidator(valid);
    ui->leInvM2->setValidator(valid);
    //  ui->leInvVa->setValidator(valid);
    // ui->leInvW->setValidator(valid);
    //ui->leLoadVar->setValidator(valid);
    //ui->leLoadW->setValidator(valid);
    ui->lePltmhVar->setValidator(valid);
    ui->lePltmhW->setValidator(valid2);

    ui->stackedWidget->setCurrentIndex(0);
    ui->action->setText("Overview");

    //table widget
    ui->twEventOverview->horizontalHeader()->setVisible(true);
    ui->twEventOverview->verticalHeader()->setVisible(true);
    ui->twEventList->horizontalHeader()->setVisible(true);
    ui->twEventList->verticalHeader()->setVisible(true);
    ui->twEventPltmh->horizontalHeader()->setVisible(true);
    ui->twEventPltmh->verticalHeader()->setVisible(true);
    ui->twEventPlts->horizontalHeader()->setVisible(true);
    ui->twEventPlts->verticalHeader()->setVisible(true);
    ui->twEventLoad->horizontalHeader()->setVisible(true);
    ui->twEventLoad->verticalHeader()->setVisible(true);
    ui->twEventInverter->horizontalHeader()->setVisible(true);
    ui->twEventInverter->verticalHeader()->setVisible(true);
    ui->twSimbol->horizontalHeader()->setVisible(true);
    ui->twEventSimbol->horizontalHeader()->setVisible(true);
    ui->twEventSimbol->verticalHeader()->setVisible(true);

    //widget
    ui->wOverview->set_OverviewBTR(false,true);
    ui->wOverview->set_OverviewBDI(false,true);
    ui->wOverview->set_OverviewPltmh(false,true);
    ui->wOverview->set_OverviewPlts(false,true);
    ui->wOverview->set_OverviewLoad(false,true);
    ui->wOverview->set_OverviewSWT(false);
    ui->wOverview->set_OverviewBDIindi(false);
    ui->wOverview->set_OverviewBTRindi(false);
    ui->wPlts->set_PltsInv1(false,true);
    ui->wPlts->set_PltsInv2(false,true);
    ui->wPlts->set_PltsInv3(false,true);
    ui->wPlts->set_PltsOut(false,true);
    ui->wPltmh->set_Pltmh(false,true);
    ui->wLoad->set_Loadr(false,false);
    ui->wLoad->set_Loads(false,false);
    ui->wLoad->set_Loadt(false,true);
    ui->wInverter->set_InvR(false,true);
    ui->wInverter->set_InvRSw(false);
    ui->wInverter->set_InvRindi(true);
    ui->wInverter->set_InvS(false,true);
    ui->wInverter->set_InvSSw(false);
    ui->wInverter->set_InvSindi(false);
    ui->wInverter->set_InvT(false,true);
    ui->wInverter->set_InvTSw(false);
    ui->wInverter->set_InvTindi(false);
    ui->wBattery->set_Btr(false,false);
    ui->wBattery->set_Btrindi(false);



    //init battery
    //ui->twCell1->item(0,0).setStyleSheet("border:1px solid grey; border-bottom-left-radius: 5px;border-bottom-right-radius: 5px;");
}


void MainWindow::on_pbTrenHis_clicked()
{
    //ui->pbTrenReak->setEnabled(true);
    //ui->pbTrenHis->setEnabled(false);
    //realChart = 1;
    openDateWindow();
    //readTren();
}

void MainWindow::on_pbTrenReak_clicked()
{
    ui->pbTrenReak->setEnabled(false);
    ui->pbTrenHis->setEnabled(true);
    ui->comboBox->setEnabled(true);
    realChart = 0;
    //readTren();
}
void MainWindow::on_pbTrenOk_clicked()
{

    realChart = 0;
}


void MainWindow::on_pbTrenReset_clicked()
{

}

void MainWindow::waktu(){
    //dateTime
    QDateTime dtime = QDateTime::currentDateTime();
    QString dtxt = dtime.toString("dddd, dd-MM-yy hh:mm:ss");
    ui->txtDateTime->setText(dtxt);
}



void MainWindow::GetAllStatusRTU(bool Activated)
{
    if (Activated!=false)
    {
        Get_StatusRTU_PLTS("select * from in_cb_rtu_plts order by id desc limit 1"  ,
                           "select * from in_cb_plts order by id desc limit 1"      ,
                           "select * from in_plts order by id desc limit 1 "        ,
                           1);

        Get_StatusRTU_PLTMH("select * from in_cb_rtu_pltmh order by id desc limit 1",
                            "select * from in_cb_pltmh order by id desc limit 1",
                            "select * from in_pltmh order by id desc limit 1",2   );



        Get_StatusRTU_LOAD("select * from in_cb_rtu_load order by id desc limit 1",
                           "select * from in_cb_load order by id desc limit 1",
                           "select * from in_load order by id desc limit 1",4);

        Get_StatusRTU_INVERTER("select * from in_cb_rtu_inverter order by id desc limit 1",
                               "select * from in_cb_inverter order by id desc limit 1",
                               "select * from in_inverter order by id desc limit 1 "  ,3 );

        //GetStatusRTU_ALARMBATTERY("select aBattery from in_cb_battery","select value from in_cb_rtu_inverter order by id desc limit 1");
    }
}
//void MainWindow::GetStatusRTU_ALARMBATTERY(QString query,QString query2){

//}

void MainWindow::Get_StatusRTU_PLTS(QString Query_RTU,QString Query_ALARM,QString QueryCB,int Column)
{
    QString Plts,varPlts;
    QString cbPlts,cbPlts1,cbPlts2,cbPlts3;
    static int RowCb=35;
    static int RowRTU=36;
    static int RowAlarm1=37;
    QTableWidgetItem *PLTS = new QTableWidgetItem;
    QTableWidgetItem *PLTSCB = new QTableWidgetItem;
    QTableWidgetItem *PLTS_ALARM = new QTableWidgetItem;
    //HMI
    QString ON="image: url(:/img/ok.png)";
    QString OFF="image: url(:/img/nok.png)";
    QString OFFQ="image: url(:/img/nokQuestion.png)";
    QString DC="image: url(:/img/offline.png)";

    //CB
    QIcon ImageOpen(":/img/openCB.png");
    QIcon ImageClose(":/img/closeCB.png");
    QIcon ImageInvalid(":/img/invalidCB.png");
    QIcon ImageCBDC(":/img/offlineCB.png");
    QIcon ImageINVALIDDC(":/img/offlineCB.png");
    //CB HMI CONTROL
    QString CBCONTROLQUESTION="image: url(:/img/ControlQuestion.png)";
    QString CBOPEN="image: url(:/img/ControlOpen.png)";
    QString CBOPEN2="image: url(:/img/ControlOpen.png)";
    QString CBCLOSE="image: url(:/img/ControlClose.png)";
    QString CBCLOSE2="image: url(:/img/ControlClose.png)";
    QString CBINVALID="image: url(:/img/ControlOffline.png)";
    QString CBINVALID2="image: url(:/img/ControlOffline.png)";
    QString CBCONTROLINVALID="image: url(:/img/ControlInvalid.png)";
    QString CBCONTROLINVALID2="image: url(:/img/ControlInvalid.png)";
    QString CBINVALIDCLOSE="image: url(:/img/InvalidCBCLOSE.png)";
    //METERING
    QIcon ImageOn(":/img/ok.png");
    QIcon ImageOff(":/img/nok.png");
    QIcon ImageDC(":/img/offline.png");
    //QIcon ImageINVALIDCLOSE(":/img/invalidClose.png");
    QSqlQuery qryrtu,qryalarm,qrycbplts,qrycbplts_1,qrycbplts_2,qrycbplts_3;

    qryrtu.prepare(Query_RTU);
    qryrtu.exec();
    if (qryrtu.next())
    {
        Plts=qryrtu.value(3).toString();
        //==============================
        //when RTU Plts ON
        //==============================
        if (Plts=="1")
        {
            //set to hmi
            //set status online rtu
            dataPLTSStatus = 1;
            ui->lblindiplts_RTU->setStyleSheet(ON);
            removeRowAlarmTable("RTU PLTS Condition is ON !!!");
            //set to hmi network
            ui->lbRTU_PLTS->setStyleSheet(ON);
            removeRowAlarmTable("Status Network PLTS Condition Is Offline");
            if(!NetPlts){
                dataout[28] = "Status Network PLTS Condition Is Normal";
                InsertDataControl_CB(17);
            }
            NetPlts = true;
            ui->lblStatusRTUPLTS->setText ("ONLINE");
            //set to metering
            PLTS->setIcon(ImageOn);
            ui->twStatusMetering->setItem(RowRTU, Column, PLTS);
            // get color when rtu ON
            Get_ColorWidget_PLTS(true,1,"#FF69FF");

            //==============================
            //when Plts  ALARM ON
            //==============================
            qryalarm.prepare(Query_ALARM);
            qryalarm.exec();
            if (qryalarm.next())
            {

                varPlts=qryalarm.value(3).toString();
                //==============================
                //when ALARM ON
                //==============================
                if (varPlts=="1")
                {
                    //set status hmi
                    ui->lblindiplts->setStyleSheet(ON);
                    removeRowAlarmTable("CB ALARM PLTS Condition is ON !!!");
                    removeRowAlarmTable("CB ALARM PLTS Condition is OFFLINE !!!");
                    if(dataAlarmPLTS != 1){
                        dataout[28] = "CB ALARM PLTS Condition is Normal !!!";
                        InsertDataControl_CB(11);
                    }
                    dataAlarmPLTS = 1;
                    //set status metering
                    PLTS_ALARM->setIcon(ImageOn);
                    ui->twStatusMetering->setItem(RowAlarm1, Column, PLTS_ALARM);

                    ui->pbPltsmon->setEnabled(1);
                    ui->pbPltsControl->setEnabled(1);

                }
                else if (varPlts=="0")
                {
                    ui->lblindiplts->setStyleSheet(OFF);
                    ui->StsGTI1->setStyleSheet(OFF);
                    ui->StsGTI2->setStyleSheet(OFF);
                    ui->StsGTI3->setStyleSheet(OFF);
                    //STATUS NETWORK
                    ui->RGTI1->setStyleSheet(OFF);
                    ui->RGTI2->setStyleSheet(OFF);
                    ui->RGTI3->setStyleSheet(OFF);
                    ui->LRGTI1->setText("OFFLINE");
                    ui->LRGTI2->setText("OFFLINE");
                    ui->LRGTI3->setText("OFFLINE");
                    addRowAlarmTable("ASDU 3","-","Server Protocol","CB ALARM PLTS Condition is ON !!!");
                    if(dataAlarmPLTS != 0){
                        dataout[28] = "CB ALARM PLTS Condition is ON !!!";
                        InsertDataControl_CB(11);
                    }
                    dataAlarmPLTS = 0;
                    PLTS_ALARM->setIcon(ImageOff);
                    ui->twStatusMetering->setItem(RowAlarm1, Column, PLTS_ALARM);

                    ui->pbPltsmon->setEnabled(0);
                    ui->pbPltsControl->setEnabled(0);

                    ui->Lbl_ControlInv1->setStyleSheet(CBCONTROLINVALID);
                    ui->Lbl_ControlInv2->setStyleSheet(CBCONTROLINVALID);
                    ui->Lbl_ControlInv3->setStyleSheet(CBCONTROLINVALID);

                    // when off invalid  cb
                    ui->Lbl_ControlPlts->setStyleSheet(CBCONTROLINVALID);
                    ui->Ov_ControlPlts->setStyleSheet(CBCONTROLINVALID);
                    ui->ArrowPlts->setVisible(0);
                    ui->OvArrowPlts->setVisible(0);
                    dataConnectionPLTS = 2;

                    PLTSCB->setIcon(ImageInvalid);
                    ui->twStatusMetering->setItem(RowCb, Column, PLTSCB);

                }
            }
            else{
                ui->lblindiplts->setStyleSheet(OFFQ);
            }


            //==============================
            //when CB STATUS IN PLTS
            //==============================
            qrycbplts.prepare(QueryCB);
            qrycbplts.exec();
            if (qrycbplts.next())
            {
                //int no;
                //no=qrycbplts.value(0).toInt();
                cbPlts=qrycbplts.value(3).toString();
                if (cbPlts=="1"  && dataAlarmPLTS == 1)
                {
                    PLTSCB->setIcon(ImageOpen);
                    PLTSCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowCb, Column, PLTSCB);
                    ui->Lbl_ControlPlts->setStyleSheet(CBOPEN);
                    ui->Ov_ControlPlts->setStyleSheet(CBOPEN);
                    if(dataConnectionPLTS != 1){
                        dataout[28] = "Status CB PLTS Condition is opened";
                        InsertDataControl_CB(11);
                    }
                    dataConnectionPLTS = 1;
                    ui->ArrowPlts->setVisible(0);
                    ui->OvArrowPlts->setVisible(0);
                    GetAllEventList(true,"EventList");
                    GetAllEventList(true,"PLTS");

                }
                else if(cbPlts=="2" && dataAlarmPLTS == 1)
                {
                    PLTSCB->setIcon(ImageClose);
                    PLTSCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowCb, Column, PLTSCB);
                    ui->Lbl_ControlPlts->setStyleSheet(CBCLOSE);
                    ui->Ov_ControlPlts->setStyleSheet(CBCLOSE);
                    if(dataConnectionPLTS != 0 ){
                        dataout[28] = "Status CB PLTS Condition is closed";
                        InsertDataControl_CB(11);
                    }
                    dataConnectionPLTS = 0;
                    ui->ArrowPlts->setVisible(1);
                    ui->OvArrowPlts->setVisible(1);
                    GetAllEventList(true,"EventList");
                    GetAllEventList(true,"PLTS");

                }
                else if ((cbPlts=="3" || cbPlts=="0") && dataAlarmPLTS == 1)
                {
                    PLTSCB->setIcon(ImageInvalid);
                    PLTSCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowCb, Column, PLTSCB);
                    ui->Lbl_ControlPlts->setStyleSheet(CBCONTROLINVALID);
                    ui->Ov_ControlPlts->setStyleSheet(CBCONTROLINVALID);
                    if(dataConnectionPLTS != 2 ){
                        dataout[28] = "Status CB PLTS Condition Is Invalid";
                        InsertDataControl_CB(11);
                    }
                    dataConnectionPLTS = 2;

                    ui->ArrowPlts->setVisible(0);
                    ui->OvArrowPlts->setVisible(0);
                    GetAllEventList(true,"EventList");
                    GetAllEventList(true,"PLTS");
                }

            }
            else{
                ui->Lbl_ControlPlts->setStyleSheet(CBCONTROLQUESTION);
                ui->Ov_ControlPlts->setStyleSheet(CBCONTROLQUESTION);
            }
            //==============================
            //when CB STATUS IN PLTS 1
            //&& dataAlarmPLTS == 1
            //==============================
            getAllDataGTI(1);
            //status gti normal
            if(dataGti[0] == "1"){
                removeRowAlarmTable("STATUS KOMUNIKASI SPV ALARM GTI 1 Condition is ON !!!");
                removeRowAlarmTable("STATUS KOMUNIKASI SPV ALARM GTI 1 Condition is OFFLINE !!!");
                removeRowAlarmTable("STATUS KOMUNIKASI SPV ALARM GTI 1 Condition is NULL !!!");
                if(dataAlarmPLTS1 != 1){
                    dataout[28] = "STATUS KOMUNIKASI SPV ALARM GTI 1 Condition is Normal !!!";
                    InsertDataControl_CB(11);
                }
                dataAlarmPLTS1 = 1;
                ui->StsGTI1->setStyleSheet(ON);
                ui->RGTI1->setStyleSheet(ON);
                ui->LRGTI1->setText("ONLINE");
                qrycbplts_1.prepare("select * from in_plts_control1 order by id desc limit 1");
                qrycbplts_1.exec();
                if (qrycbplts_1.next())
                {
                    //int no;
                    //no=qrycbplts_1.value(0).toInt();
                    cbPlts1=qrycbplts_1.value(3).toString();

                    if (cbPlts1=="1")
                    {
                        ui->Lbl_ControlInv1->setStyleSheet(CBOPEN2);
                        if(dataConnectionPLTS1 != 1 ){
                            dataout[28] = "Status CB PLTS R Condition is opened";
                            InsertDataControl_CB(11);
                        }
                        dataConnectionPLTS1 = 1;
                        ui->ArrowPlts1A->setVisible(0);
                        ui->ArrowPlts1B->setVisible(0);
                    }
                    else if(cbPlts1=="2")
                    {
                        ui->Lbl_ControlInv1->setStyleSheet(CBCLOSE2);
                        if(dataConnectionPLTS1 != 0 ){
                            dataout[28] = "Status CB PLTS R Condition is closed";
                            InsertDataControl_CB(11);
                        }
                        dataConnectionPLTS1 = 0;
                        ui->ArrowPlts1A->setVisible(1);
                        ui->ArrowPlts1B->setVisible(1);
                    }
                    else if (cbPlts1=="3" || cbPlts1=="0")
                    {
                        ui->Lbl_ControlInv1->setStyleSheet(CBCONTROLINVALID2);
                        if(dataConnectionPLTS1 != 2 ){
                            dataout[28] = "Status CB PLTS R Condition Is Invalid";
                            InsertDataControl_CB(11);
                        }
                        dataConnectionPLTS1 = 2;
                        ui->ArrowPlts1A->setVisible(0);
                        ui->ArrowPlts1B->setVisible(0);
                    }

                }
                else{
                    ui->Lbl_ControlInv1->setStyleSheet(CBCONTROLQUESTION);
                }
                //==============================
                //Fault
                //==============================
                //fault normal
                if(dataGti[1] == "0"){
                    removeRowAlarmTable("FAULT ALARM GTI 1 Condition is ON !!!");
                    removeRowAlarmTable("FAULT ALARM GTI 1 Condition is NULL !!!");
                    ui->pltsFault1->setStyleSheet(ON);
                    if(dataFaultAlarmPLTS1 != 1){
                        dataout[28] = "FAULT ALARM GTI 1 Condition is Normal !!!";
                        InsertDataControl_CB(11);
                    }
                    dataFaultAlarmPLTS1 = 1;
                }
                //fault error
                else if(dataGti[1] == "1"){
                    addRowAlarmTable("ASDU 3","-","Server Protocol","FAULT ALARM GTI 1 Condition is ON !!!");
                    ui->pltsFault1->setStyleSheet(OFF);
                    if(dataFaultAlarmPLTS1 != 2){
                        dataout[28] = "FAULT ALARM GTI 1 Condition is ON !!!";
                        InsertDataControl_CB(11);
                    }
                    dataFaultAlarmPLTS1 = 2;
                }
                //database null
                else{
                    addRowAlarmTable("ASDU 3","-","Server Protocol","FAULT ALARM GTI 1 Condition is NULL !!!");
                    ui->pltsFault1->setStyleSheet(OFFQ);
                    if(dataFaultAlarmPLTS1 != 3){
                        dataout[28] = "FAULT ALARM GTI 1 Condition is NULL !!!";
                        InsertDataControl_CB(11);
                    }
                    dataFaultAlarmPLTS1 = 3;
                }

                //==============================
                //WVA power & block
                //==============================
                ui->OutWInverterR->display(dataGti[2]);
                ui->lnWInverterR->display(dataGti[3]);
                ui->lnVInverterR->display(dataGti[4]);
                ui->lnAInverterR->display(dataGti[5]);

                ui->OutWInverterR->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnWInverterR->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnVInverterR->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnAInverterR->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->inpltsAR->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");



            }
            //status gti error
            else if(dataGti[0] == "0"){
                addRowAlarmTable("ASDU 3","-","Server Protocol","STATUS KOMUNIKASI SPV ALARM GTI 1 Condition is ON !!!");
                if(dataAlarmPLTS1 != 2){
                    dataout[28] = "STATUS KOMUNIKASI SPV ALARM GTI 1 Condition is ON !!!";
                    InsertDataControl_CB(11);
                }
                dataAlarmPLTS1 = 2;
                ui->StsGTI1->setStyleSheet(OFF);
                ui->RGTI1->setStyleSheet(OFF);
                ui->LRGTI1->setText("OFFLINE");
                ui->pltsFault1->setStyleSheet(DC);
                ui->OutWInverterR->display("0");
                ui->inpltsAR->display("0");
                ui->lnWInverterR->display("0");
                ui->lnVInverterR->display("0");
                ui->lnAInverterR->display("0");

                ui->Lbl_ControlInv1->setStyleSheet(CBINVALID);

                dataConnectionPLTS1 = 2;
                ui->ArrowPlts1A->setVisible(0);
                ui->ArrowPlts1B->setVisible(0);
                ui->OutWInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->inpltsAR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnWInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnVInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnAInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            }
            //status gti null
            else {
                ui->StsGTI1->setStyleSheet(OFFQ);
                ui->RGTI1->setStyleSheet(OFFQ);
                ui->LRGTI1->setText("OFFLINE");
                if(dataAlarmPLTS1 != 3){
                    dataout[28] = "STATUS KOMUNIKASI SPV ALARM GTI 1 Condition is ON !!!";
                    InsertDataControl_CB(11);
                }
                dataAlarmPLTS1 = 3;
                addRowAlarmTable("ASDU 3","-","Server Protocol","STATUS KOMUNIKASI SPV ALARM GTI 1 Condition is NULL !!!");
                //ui->pltsFault1->setStyleSheet(OFFQ);
                ui->OutWInverterR->display("");
                ui->lnWInverterR->display("");
                ui->lnVInverterR->display("");
                ui->lnAInverterR->display("");
            }
            //==============================
            //when CB STATUS IN PLTS 2
            //&& dataAlarmPLTS == 1
            //==============================
            getAllDataGTI(2);
            //status gti normal
            if(dataGti[0] == "1"){
                removeRowAlarmTable("STATUS KOMUNIKASI SPV ALARM GTI 2 Condition is ON !!!");
                removeRowAlarmTable("STATUS KOMUNIKASI SPV ALARM GTI 2 Condition is OFFLINE !!!");
                removeRowAlarmTable("STATUS KOMUNIKASI SPV ALARM GTI 2 Condition is NULL !!!");
                if(dataAlarmPLTS2 != 1){
                    dataout[28] = "STATUS KOMUNIKASI SPV ALARM GTI 2 Condition is Normal !!!";
                    InsertDataControl_CB(11);
                }
                dataAlarmPLTS2 = 1;
                ui->StsGTI2->setStyleSheet(ON);
                ui->RGTI2->setStyleSheet(ON);
                ui->LRGTI2->setText("ONLINE");
                qrycbplts_2.prepare("select * from in_plts_control2 order by id desc limit 1");
                qrycbplts_2.exec();
                if (qrycbplts_2.next())
                {
                    //int no,temp;
                    //no=qrycbplts_2.value(0).toInt();
                    cbPlts2=qrycbplts_2.value(3).toString();
                    if (cbPlts2=="1" )
                    {
                        ui->Lbl_ControlInv2->setStyleSheet(CBOPEN2);
                        dataConnectionPLTS2 = 1;
                        ui->ArrowPlts2A->setVisible(0);
                        ui->ArrowPlts2B->setVisible(0);
                    }
                    else if(cbPlts2=="2")
                    {
                        ui->Lbl_ControlInv2->setStyleSheet(CBCLOSE2);
                        dataConnectionPLTS2 = 0;
                        ui->ArrowPlts2A->setVisible(1);
                        ui->ArrowPlts2B->setVisible(1);
                    }
                    else if (cbPlts2=="3" || cbPlts2=="0")
                    {
                        ui->Lbl_ControlInv2->setStyleSheet(CBCONTROLINVALID2);
                        dataConnectionPLTS2 = 2;
                        ui->ArrowPlts2A->setVisible(0);
                        ui->ArrowPlts2B->setVisible(0);
                    }

                }
                else{
                    ui->Lbl_ControlInv2->setStyleSheet(CBCONTROLQUESTION);
                }
                //==============================
                //Fault
                //==============================
                //fault normal
                if(dataGti[1] == "0"){
                    removeRowAlarmTable("FAULT ALARM GTI 2 Condition is ON !!!");
                    removeRowAlarmTable("FAULT ALARM GTI 2 Condition is NULL !!!");
                    ui->pltsFault2->setStyleSheet(ON);
                    if(dataFaultAlarmPLTS2 != 1){
                        dataout[28] = "FAULT ALARM GTI 2 Condition is Normal !!!";
                        InsertDataControl_CB(11);
                    }
                    dataFaultAlarmPLTS2 = 1;
                }
                //fault error
                else if(dataGti[1] == "1"){
                    addRowAlarmTable("ASDU 3","-","Server Protocol","FAULT ALARM GTI 2 Condition is ON !!!");
                    ui->pltsFault2->setStyleSheet(OFF);
                    if(dataFaultAlarmPLTS2 != 2){
                        dataout[28] = "FAULT ALARM GTI 2 Condition is Normal !!!";
                        InsertDataControl_CB(11);
                    }
                    dataFaultAlarmPLTS2 = 2;
                }
                //database null
                else{
                    addRowAlarmTable("ASDU 3","-","Server Protocol","FAULT ALARM GTI 2 Condition is NULL !!!");
                    ui->pltsFault2->setStyleSheet(OFFQ);
                    if(dataFaultAlarmPLTS2 != 3){
                        dataout[28] = "FAULT ALARM GTI 2 Condition is Normal !!!";
                        InsertDataControl_CB(11);
                    }
                    dataFaultAlarmPLTS2 = 3;
                }

                //==============================
                //WVA power & block
                //==============================
                ui->OutWInverterS->display(dataGti[2]);
                ui->lnWInverterS->display(dataGti[3]);
                ui->lnVInverterS->display(dataGti[4]);
                ui->lnAInverterS->display(dataGti[5]);

                ui->OutWInverterS->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnWInverterS->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnVInverterS->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnAInverterS->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->inpltsAS->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            }
            //status gti error
            else if(dataGti[0] == "0" ){
                addRowAlarmTable("ASDU 3","-","Server Protocol","STATUS KOMUNIKASI SPV ALARM GTI 2 Condition is ON !!!");
                if(dataAlarmPLTS2 != 2){
                    dataout[28] = "STATUS KOMUNIKASI SPV ALARM GTI 2 Condition is ON !!!";
                    InsertDataControl_CB(11);
                }
                dataAlarmPLTS2 = 2;
                ui->StsGTI2->setStyleSheet(OFF);
                ui->RGTI2->setStyleSheet(OFF);
                ui->LRGTI2->setText("OFFLINE");
                ui->pltsFault2->setStyleSheet(DC);
                ui->OutWInverterS->display("0");
                ui->inpltsAS->display("0");
                //ui->OutVInverterS->display("");
                //ui->OutAInverterS->display("");
                ui->lnWInverterS->display("0");
                ui->lnVInverterS->display("0");
                ui->lnAInverterS->display("0");

                ui->OutWInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->inpltsAS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnWInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnVInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnAInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->Lbl_ControlInv2->setStyleSheet(CBINVALID);
                dataConnectionPLTS2 = 2;
                ui->ArrowPlts2A->setVisible(0);
                ui->ArrowPlts2B->setVisible(0);

            }
            //status gti null
            else {
                if(dataAlarmPLTS2 != 3){
                    dataout[28] = "STATUS KOMUNIKASI SPV ALARM GTI 2 Condition is NULL !!!";
                    InsertDataControl_CB(11);
                }
                dataAlarmPLTS2 = 3;
                addRowAlarmTable("ASDU 3","-","Server Protocol","STATUS KOMUNIKASI SPV ALARM GTI 2 Condition is NULL !!!");
                ui->StsGTI2->setStyleSheet(OFFQ);
                ui->RGTI2->setStyleSheet(OFFQ);
                ui->LRGTI2->setText("OFFLINE");
                //ui->pltsFault1->setStyleSheet(OFFQ);
                ui->OutWInverterS->display("");
                //ui->OutVInverterS->display("");
                //ui->OutAInverterS->display("");
                ui->lnWInverterS->display("");
                ui->lnVInverterS->display("");
                ui->lnAInverterS->display("");
            }
            //==============================
            //when CB STATUS IN PLTS 3
            //&& dataAlarmPLTS == 1
            //==============================
            getAllDataGTI(3);
            //status gti normal
            if(dataGti[0] == "1"){
                removeRowAlarmTable("STATUS KOMUNIKASI SPV ALARM GTI 3 Condition is ON !!!");
                removeRowAlarmTable("STATUS KOMUNIKASI SPV ALARM GTI 3 Condition is OFFLINE !!!");
                removeRowAlarmTable("STATUS KOMUNIKASI SPV ALARM GTI 3 Condition is NULL !!!");
                if(dataAlarmPLTS3 != 1){
                    dataout[28] = "STATUS KOMUNIKASI SPV ALARM GTI 3 Condition is Normal !!!";
                    InsertDataControl_CB(11);
                }
                dataAlarmPLTS3 = 1;
                ui->StsGTI3->setStyleSheet(ON);
                ui->RGTI3->setStyleSheet(ON);
                ui->LRGTI3->setText("ONLINE");
                qrycbplts_3.prepare("select * from in_plts_control3 order by id desc limit 1");
                qrycbplts_3.exec();
                if (qrycbplts_3.next())
                {
                    int no;
                    no=qrycbplts_3.value(0).toInt();
                    cbPlts3=qrycbplts_3.value(3).toString();

                    if (cbPlts3=="1" )
                    {
                        ui->Lbl_ControlInv3->setStyleSheet(CBOPEN2);
                        dataConnectionPLTS3 = 1;
                        ui->ArrowPlts3A->setVisible(0);
                        ui->ArrowPlts3B->setVisible(0);
                    }
                    else if(cbPlts3=="2")
                    {
                        ui->Lbl_ControlInv3->setStyleSheet(CBCLOSE2);
                        dataConnectionPLTS3 = 0;
                        ui->ArrowPlts3A->setVisible(1);
                        ui->ArrowPlts3B->setVisible(1);
                    }
                    else if (cbPlts3=="3" || cbPlts3=="0")
                    {
                        ui->Lbl_ControlInv3->setStyleSheet(CBCONTROLINVALID2);
                        dataConnectionPLTS3 = 2;
                        ui->ArrowPlts3A->setVisible(0);
                        ui->ArrowPlts3B->setVisible(0);
                    }

                }
                else{
                    ui->Lbl_ControlInv3->setStyleSheet(CBCONTROLQUESTION);
                }
                //==============================
                //Fault
                //==============================
                //fault normal
                if(dataGti[1] == "0"){
                    removeRowAlarmTable("FAULT ALARM GTI 3 Condition is ON !!!");
                    removeRowAlarmTable("FAULT ALARM GTI 3 Condition is NULL !!!");
                    ui->pltsFault3->setStyleSheet(ON);
                    if(dataFaultAlarmPLTS3 != 1){
                        dataout[28] = "FAULT ALARM GTI 3 Condition is Normal !!!";
                        InsertDataControl_CB(11);
                    }
                    dataFaultAlarmPLTS3 = 1;
                }
                //fault error
                else if(dataGti[1] == "1"){
                    addRowAlarmTable("ASDU 3","-","Server Protocol","FAULT ALARM GTI 3 Condition is ON !!!");
                    if(dataFaultAlarmPLTS3 != 2){
                        dataout[28] = "FAULT ALARM GTI 3 Condition is ON !!!";
                        InsertDataControl_CB(11);
                    }
                    dataFaultAlarmPLTS3 = 2;
                    ui->pltsFault3->setStyleSheet(OFF);
                }
                //database null
                else{
                    addRowAlarmTable("ASDU 3","-","Server Protocol","FAULT ALARM GTI 3 Condition is NULL !!!");
                    if(dataFaultAlarmPLTS3 != 3){
                        dataout[28] = "FAULT ALARM GTI 3 Condition is NULL !!!";
                        InsertDataControl_CB(11);
                    }
                    dataFaultAlarmPLTS3 = 3;
                    ui->pltsFault3->setStyleSheet(OFFQ);
                }

                //==============================
                //WVA power & block
                //==============================
                ui->OutWInverterT->display(dataGti[2]);
                ui->lnWInverterT->display(dataGti[3]);
                ui->lnVInverterT->display(dataGti[4]);
                ui->lnAInverterT->display(dataGti[5]);

                ui->OutWInverterT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnWInverterT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnVInverterT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnAInverterT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->inpltsAT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            }
            //status gti error
            else if(dataGti[0] == "0"){
                addRowAlarmTable("ASDU 3","-","Server Protocol","STATUS KOMUNIKASI SPV ALARM GTI 3 Condition is ON !!!");
                if(dataAlarmPLTS3 != 2){
                    dataout[28] = "STATUS KOMUNIKASI SPV ALARM GTI 3 Condition is ON !!!";
                    InsertDataControl_CB(11);
                }
                dataAlarmPLTS3 = 2;
                ui->StsGTI3->setStyleSheet(OFF);
                ui->RGTI3->setStyleSheet(OFF);
                ui->LRGTI3->setText("OFFLINE");
                ui->pltsFault3->setStyleSheet(DC);
                ui->OutWInverterT->display("0");
                ui->inpltsAT->display("0");
                //ui->OutVInverterT->display("0");
                //ui->OutAInverterT->display("0");
                ui->lnWInverterT->display("0");
                ui->lnVInverterT->display("0");
                ui->lnAInverterT->display("0");

                ui->Lbl_ControlInv3->setStyleSheet(CBINVALID);
                dataConnectionPLTS3 = 2;
                ui->ArrowPlts3A->setVisible(0);
                ui->ArrowPlts3B->setVisible(0);
                ui->OutWInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->inpltsAT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnWInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnVInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
                ui->lnAInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            }
            //status gti null
            else {
                addRowAlarmTable("ASDU 3","-","Server Protocol","STATUS KOMUNIKASI SPV ALARM GTI 3 Condition is NULL !!!");
                if(dataAlarmPLTS3 != 3){
                    dataout[28] = "STATUS KOMUNIKASI SPV ALARM GTI 3 Condition is NULL !!!";
                    InsertDataControl_CB(11);
                }
                dataAlarmPLTS3 = 3;
                ui->StsGTI3->setStyleSheet(OFFQ);
                ui->RGTI3->setStyleSheet(OFFQ);
                ui->LRGTI3->setText("OFFLINE");
                //ui->pltsFault1->setStyleSheet(OFFQ);
                ui->OutWInverterT->display("");
                ui->lnWInverterT->display("");
                ui->lnVInverterT->display("");
                ui->lnAInverterT->display("");
            }

        }
        //==============================
        //when Plts OFF ALARM PINK
        //==============================
        else if(Plts=="0")
        {
            //HMI
            ui->lblindiplts_RTU->setStyleSheet(OFF);
            addRowAlarmTable("ASDU 3","-","Server Protocol","RTU PLTS Condition is ON !!!");
            //FAULT
            ui->pltsFault1->setStyleSheet(DC);
            addRowAlarmTable("ASDU 3","-","Server Protocol","FAULT ALARM GTI 1 Condition is ON !!!");
            ui->pltsFault2->setStyleSheet(DC);
            addRowAlarmTable("ASDU 3","-","Server Protocol","FAULT ALARM GTI 2 Condition is ON !!!");
            ui->pltsFault3->setStyleSheet(DC);
            addRowAlarmTable("ASDU 3","-","Server Protocol","FAULT ALARM GTI 3 Condition is ON !!!");
            //NETWORK
            ui->lbRTU_PLTS->setStyleSheet(OFF);
            addRowAlarmTable("Not Available","-","Server Protocol","Status Network PLTS Condition Is Offline");
            ui->lblStatusRTUPLTS->setText("OFFLINE");
            if(NetPlts){
                dataout[28] = "Status Network PLTS Condition Is Offline";
                InsertDataControl_CB(17);
            }
            NetPlts = false;

            ui->StsGTI1->setStyleSheet(DC);
            ui->StsGTI2->setStyleSheet(DC);
            ui->StsGTI3->setStyleSheet(DC);
            ui->RGTI1->setStyleSheet(DC);
            ui->RGTI2->setStyleSheet(DC);
            ui->RGTI3->setStyleSheet(DC);
            ui->LRGTI1->setText("OFFLINE");
            ui->LRGTI2->setText("OFFLINE");
            ui->LRGTI3->setText("OFFLINE");
            ui->OutWInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnWInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnVInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnAInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->OutWInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnWInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnVInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnAInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->OutWInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnWInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnVInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnAInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->lnPltsOutW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->inpltsVR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsVS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsVT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->lnPltsMatahari->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->OvPltsW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");



            //RTU METERING
            PLTS->setIcon(ImageOff);
            PLTS->setSizeHint(QSize(100, 100));
            ui->twStatusMetering->setItem(RowRTU, Column, PLTS);
            //==============================
            //when CB STATUS IN PLTS
            //==============================
            QSqlQuery qryPlts;
            qryPlts.prepare(QueryCB);
            qryPlts.exec();
            if (qryPlts.next())
            {
                //int no;
                QString cb_plts_val;
                //no=qryPlts.value(0).toInt();
                cb_plts_val=qryPlts.value(3).toString();


                if (cb_plts_val=="1" )
                {
                    //CB ALARM METERING
                    ui->lblindiplts->setStyleSheet(DC);

                    addRowAlarmTable("ASDU 3","-","Server Protocol","CB ALARM PLTS Condition is OFFLINE !!!");

                    PLTS_ALARM->setIcon(ImageDC);
                    ui->twStatusMetering->setItem(RowAlarm1, Column, PLTS_ALARM);
                    //CB ALARM METERING
                    PLTSCB->setIcon(ImageCBDC);
                    PLTSCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowCb, Column, PLTSCB);
                    // get color when rtu OFF+ arrow
                    Get_ColorWidget_PLTS(true,0,"#FF69FF");
                }
                else if(cb_plts_val=="2")
                {
                    //CB ALARM METERING
                    ui->lblindiplts->setStyleSheet(DC);
                    addRowAlarmTable("ASDU 3","-","Server Protocol","CB ALARM PLTS Condition is OFFLINE !!!");

                    PLTS_ALARM->setIcon(ImageDC);
                    ui->twStatusMetering->setItem(RowAlarm1, Column, PLTS_ALARM);
                    //CB ALARM METERING
                    PLTSCB->setIcon(ImageINVALIDDC);
                    PLTSCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowCb, Column, PLTSCB);
                    //CB ALL
                    ui->Ov_ControlPlts->setStyleSheet(CBINVALID); //CBINVALIDCLOSE
                    ui->Lbl_ControlPlts->setStyleSheet(CBINVALID); // CBINVALIDCLOSE
                    dataConnectionPLTS = 2;

                    ui->pbPltsControl->setEnabled(0);
                    ui->pbPltsmon->setEnabled(0);
                    Get_ColorWidget_PLTS(true,2,"#FF69FF");

                }
                else if (cb_plts_val=="3" || cb_plts_val=="0")
                {
                    //CB ALARM METERING
                    ui->lblindiplts->setStyleSheet(DC);
                    addRowAlarmTable("ASDU 3","-","Server Protocol","CB ALARM PLTS Condition is OFFLINE !!!");

                    PLTSCB->setIcon(ImageInvalid);
                    PLTSCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowCb, Column, PLTSCB);
                    ui->Lbl_ControlPlts->setStyleSheet(CBINVALID);
                    ui->Ov_ControlPlts->setStyleSheet(CBINVALID);
                    dataConnectionPLTS = 2;

                }
                if(dataPLTSStatus == 1){
                    dataout[28] = "CB ALARM PLTS Condition is OFFLINE !!!";
                    InsertDataControl_CB(11);
                }
                dataPLTSStatus = 2;
                ui->ArrowPlts->setVisible(0);
                ui->OvArrowPlts->setVisible(0);

            }
            //==============================
            //when CB STATUS IN PLTS 1
            //==============================
            qrycbplts_1.prepare("select id,gti1 from in_plts_control1 order by id desc limit 1");
            qrycbplts_1.exec();
            if (qrycbplts_1.next())
            {
                //int no;
                QString pltstemp;
                //no=qrycbplts_1.value(0).toInt();
                pltstemp=qrycbplts_1.value(1).toString();

                if (pltstemp=="1" )
                {
                    ui->Lbl_ControlInv1->setStyleSheet(CBINVALID);
                    dataConnectionPLTS1 = 2;
                    ui->ArrowPlts1A->setVisible(0);
                    ui->ArrowPlts1B->setVisible(0);
                }
                else if(pltstemp=="2")
                {
                    ui->Lbl_ControlInv1->setStyleSheet(CBINVALID);
                    dataConnectionPLTS1 = 2;
                    ui->ArrowPlts1A->setVisible(0);
                    ui->ArrowPlts1B->setVisible(0);
                }
                else if (pltstemp=="3" || pltstemp=="0")
                {
                    ui->Lbl_ControlInv1->setStyleSheet(CBINVALID);
                    dataConnectionPLTS1 = 2;
                    ui->ArrowPlts1A->setVisible(0);
                    ui->ArrowPlts1B->setVisible(0);
                }
                addRowAlarmTable("ASDU 3","-","Server Protocol","STATUS KOMUNIKASI SPV ALARM GTI 1 Condition is OFFLINE !!!");
                //ui->pbPltsmon_R->setEnabled(0);

            }
            //==============================
            //when CB STATUS IN PLTS 2
            //==============================
            qrycbplts_2.prepare("select id,gti2 from in_plts_control2 order by id desc limit 1");
            qrycbplts_2.exec();
            if (qrycbplts_2.next())
            {
                //int no;
                //no=qrycbplts_2.value(0).toInt();
                cbPlts2=qrycbplts_2.value(1).toString();

                if (cbPlts2=="1" )
                {
                    ui->Lbl_ControlInv2->setStyleSheet(CBINVALID);
                    ui->ArrowPlts2A->setVisible(0);
                    ui->ArrowPlts2B->setVisible(0);
                }
                else if(cbPlts2=="2")
                {
                    ui->Lbl_ControlInv2->setStyleSheet(CBINVALID);
                    ui->ArrowPlts2A->setVisible(0);
                    ui->ArrowPlts2B->setVisible(0);

                }
                else if (cbPlts2=="3" || cbPlts2=="0")
                {
                    ui->Lbl_ControlInv2->setStyleSheet(CBINVALID);
                    ui->ArrowPlts2A->setVisible(0);
                    ui->ArrowPlts2B->setVisible(0);
                }

                addRowAlarmTable("ASDU 3","-","Server Protocol","STATUS KOMUNIKASI SPV ALARM GTI 2 Condition is OFFLINE !!!");
                //ui->pbPltsmon_T->setEnabled(0);

            }
            //==============================
            //when CB STATUS IN PLTS 3
            //==============================
            qrycbplts_3.prepare("select id,gti3 from in_plts_control3 order by id desc limit 1");
            qrycbplts_3.exec();
            if (qrycbplts_3.next())
            {
                //no=qrycbplts_3.value(0).toInt();
                cbPlts3=qrycbplts_3.value(1).toString();


                if (cbPlts3=="1" )
                {
                    ui->Lbl_ControlInv3->setStyleSheet(CBINVALID);
                    ui->ArrowPlts3A->setVisible(0);
                    ui->ArrowPlts3B->setVisible(0);
                }
                else if(cbPlts3=="2")
                {
                    ui->Lbl_ControlInv3->setStyleSheet(CBINVALID);
                    ui->ArrowPlts3A->setVisible(0);
                    ui->ArrowPlts3B->setVisible(0);
                }
                else if (cbPlts3=="3" || cbPlts3=="0")
                {
                    ui->Lbl_ControlInv3->setStyleSheet(CBINVALID);
                    ui->ArrowPlts3A->setVisible(0);
                    ui->ArrowPlts3B->setVisible(0);
                }
                addRowAlarmTable("ASDU 3","-","Server Protocol","STATUS KOMUNIKASI SPV ALARM GTI 3 Condition is OFFLINE !!!");
                //ui->pbPltsmon_S->setEnabled(0);


            }

        }
    }
    //database kosong
    else{
        ui->lblindiplts_RTU->setStyleSheet(OFFQ);
    }
}

void MainWindow::Get_StatusRTU_PLTMH(QString Query_RTU, QString Query_ALARM,QString QueryCB,int Column)
{
    QString Pltmh;
    QString CBPltmh,CBPltmh2;
    QString varPltmh,varPltmh_b;
    static int RowRTU=35;
    static int RowCb=36;
    static int RowAlarm1=37;
    static int RowAlarm2=38;
    QTableWidgetItem *PLTMH = new QTableWidgetItem;
    QTableWidgetItem *PLTMHCB = new QTableWidgetItem;
    //QTableWidgetItem *PLTMHCB2 = new QTableWidgetItem;
    QTableWidgetItem *PLTMHRTU= new QTableWidgetItem;
    QTableWidgetItem *PLTMH_ALARM = new QTableWidgetItem;
    QTableWidgetItem *PLTMH_ALARM_2 = new QTableWidgetItem;
    //HMI
    QString ON="image: url(:/img/ok.png)";
    QString OFF="image: url(:/img/nok.png)";
    QString OFFQ="image: url(:/img/nokQuestion.png)";
    QString DC="image: url(:/img/offline.png)";
    //CB
    QIcon ImageOpen(":/img/openCB.png");
    QIcon ImageClose(":/img/closeCB.png");
    QIcon ImageInvalid(":/img/invalidCB.png");
    QIcon ImageCBDC(":/img/offlineCB.png");
    QIcon ImageINVALIDDC(":/img/offlineCB.png");
    //CB HMI CONTROL
    QString CBCONTROLQUESTION="image: url(:/img/ControlQuestion.png)";
    QString CBCONTROLINVALID="image: url(:/img/ControlInvalid.png)";
    QString CBOPEN="image: url(:/img/ControlOpen.png)";
    QString CBCLOSE="image: url(:/img/ControlClose.png)";
    QString CBINVALID="image: url(:/img/ControlOffline.png)";
    QString CBINVALIDCLOSE="image: url(:/img/InvalidCBCLOSE.png)";

    //METERING
    QIcon ImageOn(":/img/ok.png");
    QIcon ImageOff(":/img/nok.png");
    QIcon ImageDC(":/img/offline.png");
    QSqlQuery qryrtu,qryalarm,qrycbpltmh;

    qryrtu.prepare(Query_RTU);
    qryrtu.exec();
    if (qryrtu.next())
    {
        Pltmh=qryrtu.value(3).toString();
        //==============================
        //when Pltmh ON
        //==============================
        if (Pltmh=="1")
        {
            //hmi
            ui->lblindipltmh_RTU->setStyleSheet(ON);
            removeRowAlarmTable("RTU PLTMH Condition is ON !!!");
            removeRowAlarmTable("CB ALARM PLTMH 2 Condition is OFFLINE !!!");
            removeRowAlarmTable("CB ALARM PLTMH 1 Condition is OFFLINE !!!");
            //network
            ui->lbRTU_PLTMH->setStyleSheet(ON);
            ui->lblStatusRTUPLTMH->setText("ONLINE");
            removeRowAlarmTable("Status Network PLTMH Condition Is Offline");
            if(!NetPltmh){
                dataout[28] = "Status Network PLTMH Condition Is Normal";
                InsertDataControl_CB(17);
            }
            NetPltmh = true;
            //image pltmh
            Get_ColorWidget_PLTMH(true,1,"#FF69FF");
            PLTMH->setIcon(ImageOn);
            ui->twStatusMetering->setItem(RowCb, Column, PLTMH);
            //==============================
            //Check alarm
            //==============================
            qryalarm.prepare(Query_ALARM);
            qryalarm.exec();
            if (qryalarm.next())
            {
                varPltmh=qryalarm.value(3).toString();
                varPltmh_b=qryalarm.value(4).toString();
                //==============================
                //when ALARM ON
                //==============================
                if (varPltmh=="1")
                {
                    ui->lblindipltmh->setStyleSheet(ON);
                    removeRowAlarmTable("CB ALARM PLTMH 1 Condition is ON !!!");
                    if(dataAlarmPLTMH1 == 1){
                        dataout[28] = "CB ALARM PLTMH 1 Condition is Normal !!!";
                        InsertDataControl_CB(12);
                    }
                    dataAlarmPLTMH1 = 0;
                    PLTMH_ALARM->setIcon(ImageOn);
                    ui->twStatusMetering->setItem(RowAlarm1, Column, PLTMH_ALARM);

                    ui->pbPltmhControl->setEnabled(1);
                    ui->pbPltmh->setEnabled(1);

                }
                else if(varPltmh=="0")
                {
                    ui->lblindipltmh->setStyleSheet(OFF);
                    addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 1 Condition is ON !!!");
                    if(dataAlarmPLTMH1 == 0){
                        dataout[28] = "CB ALARM PLTMH 1 Condition is ON !!!";
                        InsertDataControl_CB(12);
                    }
                    dataAlarmPLTMH1 = 1;
                    PLTMH_ALARM->setIcon(ImageOff);
                    ui->twStatusMetering->setItem(RowAlarm1, Column, PLTMH_ALARM);

                    ui->pbPltmhControl->setEnabled(0);
                    ui->pbPltmh->setEnabled(0);

                    ui->Ov_ControlPltmh->setStyleSheet(CBCONTROLINVALID); // CBINVALID
                    ui->Lbl_ControlPltmh->setStyleSheet(CBCONTROLINVALID);

                    ui->ArrowPLTMH->setVisible(0);
                    ui->OvArrowPltmh->setVisible(0);

                    dataConnectionPLMH = 2;
                }
                if(varPltmh_b=="1")
                {
                    ui->lblindipltmh_2->setStyleSheet(ON);
                    removeRowAlarmTable("CB ALARM PLTMH 2 Condition is ON !!!");
                    if(dataAlarmPLTMH2 == 1){
                        dataout[28] = "CB ALARM PLTMH 2 Condition is Normal !!!";
                        InsertDataControl_CB(12);
                    }
                    dataAlarmPLTMH2 = 0;
                    PLTMH_ALARM_2->setIcon(ImageOn);
                    ui->twStatusMetering->setItem(RowAlarm2, Column, PLTMH_ALARM_2);

                    ui->pbPltmhControl->setEnabled(1);
                    ui->pbPltmh->setEnabled(1);
                }
                else if(varPltmh_b=="0")
                {
                    ui->lblindipltmh_2->setStyleSheet(OFF);
                    addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 2 Condition is ON !!!");
                    if(dataAlarmPLTMH2 == 0){
                        dataout[28] = "CB ALARM PLTMH 2 Condition is ON !!!";
                        InsertDataControl_CB(12);
                    }
                    dataAlarmPLTMH2 = 1;
                    PLTMH_ALARM_2->setIcon(ImageOff);
                    ui->twStatusMetering->setItem(RowAlarm2, Column, PLTMH_ALARM_2);

                    ui->pbPltmhControl->setEnabled(0);
                    ui->pbPltmh->setEnabled(0);

                    ui->Ov_ControlPltmh->setStyleSheet(CBCONTROLINVALID);
                    ui->Lbl_ControlPltmh->setStyleSheet(CBCONTROLINVALID);

                    ui->ArrowPLTMH->setVisible(0);
                    ui->OvArrowPltmh->setVisible(0);
                    if(dataConnectionPLMH != 2){
                        dataout[28] = "3";
                        InsertDataControl_CB(12);
                    }
                    dataConnectionPLMH = 2;
                }
            }
            else{
                ui->lblindipltmh->setStyleSheet(OFFQ);
                ui->lblindipltmh_2->setStyleSheet(OFFQ);
            }
            //==============================
            //Check CB
            //==============================
            qrycbpltmh.prepare(QueryCB);
            qrycbpltmh.exec();
            if (qrycbpltmh.next())
            {

                CBPltmh=qrycbpltmh.value(3).toString();

                //Jika kondisi rtu On dan pengecekan di cbpltmh 1
                //QMessageBox::information(this,"Information CB",CBPltmh+ "  " +QString::number(dataAlarmPLTMH1)+ "  " +QString::number(dataAlarmPLTMH2)+ " data");
                if (CBPltmh == "1" && dataAlarmPLTMH1 == 0 && dataAlarmPLTMH2 == 0)
                {

                    PLTMHCB->setIcon(ImageOpen);
                    PLTMHCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowRTU, Column, PLTMHCB);
                    ui->Lbl_ControlPltmh->setStyleSheet(CBOPEN);
                    ui->Ov_ControlPltmh->setStyleSheet(CBOPEN);
                    if(dataConnectionPLMH != 1 ){

                        dataout[28] = "Status CB PLTMH Condition is opened";
                        InsertDataControl_CB(12);
                    }
                    dataConnectionPLMH = 1;
                    ui->ArrowPLTMH->setVisible(0);
                    ui->OvArrowPltmh->setVisible(0);
                    GetAllEventList(true,"EventList");
                    GetAllEventList(true,"PLTMH");
                }
                else if(CBPltmh=="2" && dataAlarmPLTMH1 == 0 && dataAlarmPLTMH2 == 0 )
                {
                    PLTMHCB->setIcon(ImageClose);
                    PLTMHCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowRTU, Column, PLTMHCB);
                    ui->Lbl_ControlPltmh->setStyleSheet(CBCLOSE);
                    ui->Ov_ControlPltmh->setStyleSheet(CBCLOSE);
                    if(dataConnectionPLMH != 0){
                        dataout[28] = "Status CB PLTMH Condition is closed";
                        InsertDataControl_CB(12);
                    }
                    dataConnectionPLMH = 0;

                    ui->ArrowPLTMH->setVisible(1);
                    ui->OvArrowPltmh->setVisible(1);
                    GetAllEventList(true,"EventList");
                    GetAllEventList(true,"PLTMH");
                }
                else if ((CBPltmh=="3" || CBPltmh=="0") && dataAlarmPLTMH1 == 0 && dataAlarmPLTMH2 == 0)
                {
                    PLTMHCB->setIcon(ImageInvalid);
                    PLTMHCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowRTU, Column, PLTMHCB);
                    ui->Lbl_ControlPltmh->setStyleSheet(CBCONTROLINVALID);
                    ui->Ov_ControlPltmh->setStyleSheet(CBCONTROLINVALID);

                    ui->ArrowPLTMH->setVisible(0);
                    ui->OvArrowPltmh->setVisible(0);
                    GetAllEventList(true,"EventList");
                    GetAllEventList(true,"PLTMH");
                    if(dataConnectionPLMH != 2){
                        dataout[28] = "Status CB PLTMH Condition Is Invalid";
                        InsertDataControl_CB(12);
                    }
                    dataConnectionPLMH = 2;
                }

            }
            else{
                ui->Lbl_ControlPltmh->setStyleSheet(CBCONTROLQUESTION);
                ui->Ov_ControlPltmh->setStyleSheet(CBCONTROLQUESTION);
            }


        }
        //==============================
        //when Pltmh OFF
        //==============================
        else if (Pltmh=="0")
        {
            //network
            dataConnectionPLMH = 2;
            ui->lbRTU_PLTMH->setStyleSheet(OFF);
            ui->lblStatusRTUPLTMH->setText("OFFLINE");
            addRowAlarmTable("Not Available","-","System HMI","Status Network PLTMH Condition Is Offline");
            if(NetPltmh){
                dataout[28] = "Status Network PLTMH Condition Is Offline";
                InsertDataControl_CB(17);
            }
            NetPltmh = false;
            //hmi
            ui->lblindipltmh_RTU->setStyleSheet(OFF);
            addRowAlarmTable("ASDU 1","-","Server Protocol","RTU PLTMH Condition is ON !!!");
            //            PLTMH->setIcon(ImageOff);
            //            ui->twStatusMetering->setItem(RowCb, Column, PLTMH);
            // CB METERING
            PLTMHRTU->setIcon(ImageOff);
            PLTMHRTU->setSizeHint(QSize(100, 100));
            ui->twStatusMetering->setItem(RowCb, Column, PLTMHRTU);\
            ui->lblindipltmh->setStyleSheet(DC);
            addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 1 Condition is OFFLINE !!!");
            PLTMH_ALARM->setIcon(ImageDC);
            ui->twStatusMetering->setItem(RowAlarm1, Column, PLTMH_ALARM);

            ui->lblindipltmh_2->setStyleSheet(DC);
            addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 2 Condition is OFFLINE !!!");
            PLTMH_ALARM_2->setIcon(ImageDC);
            ui->twStatusMetering->setItem(RowAlarm2, Column, PLTMH_ALARM_2);
            // PLTMH->setIcon(ImageDC);

            PLTMHCB->setIcon(ImageCBDC);
            PLTMHCB->setSizeHint(QSize(100, 100));
            ui->twStatusMetering->setItem( RowRTU ,Column, PLTMHCB);

            ui->Ov_ControlPltmh->setStyleSheet(CBINVALID);  //CBINVALIDCLOSE
            ui->Lbl_ControlPltmh->setStyleSheet(CBINVALID); //CBINVALIDCLOSE


            Get_ColorWidget_PLTMH(true,0,"#FF69FF");
            QSqlQuery qrycbpltmh_var;
            QString Var;
            qrycbpltmh_var.prepare(QueryCB);
            qrycbpltmh_var.exec();
            if (qrycbpltmh_var.next())
            {
                Var=qrycbpltmh_var.value(3).toString();

                if (Var=="1" )
                {
                    //HMI + METERING
                    ui->lblindipltmh->setStyleSheet(DC);
                    addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 1 Condition is OFFLINE !!!");
                    PLTMH_ALARM->setIcon(ImageDC);
                    ui->twStatusMetering->setItem(RowAlarm1, Column, PLTMH_ALARM);

                    ui->lblindipltmh_2->setStyleSheet(DC);
                    addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 2 Condition is OFFLINE !!!");
                    PLTMH_ALARM_2->setIcon(ImageDC);
                    ui->twStatusMetering->setItem(RowAlarm2, Column, PLTMH_ALARM_2);
                    // PLTMH->setIcon(ImageDC);

                    PLTMHCB->setIcon(ImageCBDC);
                    PLTMHCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem( RowRTU ,Column, PLTMHCB);

                    ui->Ov_ControlPltmh->setStyleSheet(CBINVALID);  //CBINVALIDCLOSE
                    ui->Lbl_ControlPltmh->setStyleSheet(CBINVALID); //CBINVALIDCLOSE


                    Get_ColorWidget_PLTMH(true,0,"#FF69FF");
                }
                else if(Var=="2")
                {
                    //HMI + METERING
                    ui->lblindipltmh->setStyleSheet(DC);
                    addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 1 Condition is OFFLINE !!!");
                    PLTMH_ALARM->setIcon(ImageDC);
                    ui->twStatusMetering->setItem(RowAlarm1, Column, PLTMH_ALARM);

                    ui->lblindipltmh_2->setStyleSheet(DC);
                    addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 2 Condition is OFFLINE !!!");
                    PLTMH_ALARM_2->setIcon(ImageDC);
                    ui->twStatusMetering->setItem(RowAlarm2, Column, PLTMH_ALARM_2);

                    PLTMHCB->setIcon(ImageINVALIDDC);
                    PLTMHCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowRTU, Column, PLTMHCB);

                    ui->Ov_ControlPltmh->setStyleSheet(CBINVALID);  //CBINVALIDCLOSE
                    ui->Lbl_ControlPltmh->setStyleSheet(CBINVALID); //CBINVALIDCLOSE


                    ui->pbPltmhControl->setEnabled(0);
                    ui->pbPltmh->setEnabled(0);

                    Get_ColorWidget_PLTMH(true,2,"#FF69FF");

                }
                else if (Var=="3" || Var=="0")
                {
                    //HMI + METERING
                    ui->lblindipltmh->setStyleSheet(DC);
                    addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 1 Condition is OFFLINE !!!");
                    PLTMH_ALARM->setIcon(ImageDC);
                    ui->twStatusMetering->setItem(RowAlarm1, Column, PLTMH_ALARM);

                    ui->lblindipltmh_2->setStyleSheet(DC);
                    addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 2 Condition is OFFLINE !!!");
                    PLTMH_ALARM_2->setIcon(ImageDC);
                    ui->twStatusMetering->setItem(RowAlarm2, Column, PLTMH_ALARM_2);

                    PLTMHCB->setIcon(ImageInvalid);
                    PLTMHCB->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowRTU, Column, PLTMHCB);

                    ui->Ov_ControlPltmh->setStyleSheet(CBINVALID);
                    ui->Lbl_ControlPltmh->setStyleSheet(CBINVALID);

                    Get_ColorWidget_PLTMH(true,0,"#FF69FF");
                }

                ui->ArrowPLTMH->setVisible(0);
                ui->OvArrowPltmh->setVisible(0);


            }

            //CB CONTROL

        }

    }
    else{
        ui->lblindipltmh_RTU->setStyleSheet(OFFQ);
    }
}

void MainWindow::Get_StatusRTU_INVERTER(QString Query_RTU, QString Query_ALARM,QString QueryCB,int Column)
{

    QString Inverter,varInverter,CBBattery;
    static int RowRTU=36;
    static int RowCb=35;
    static int RowAlarm1=37;
    QTableWidgetItem *INVERTER = new QTableWidgetItem;
    QTableWidgetItem *CBINVERTER = new QTableWidgetItem;
    //QTableWidgetItem *INVERTER_ALARM = new QTableWidgetItem;
    QTableWidgetItem *BATTERY = new QTableWidgetItem;
    QTableWidgetItem *CBBATTERY = new QTableWidgetItem;
    QTableWidgetItem *BATTERY_ALARM = new QTableWidgetItem;
    //HMI
    QString ON="image: url(:/img/ok.png)";
    QString OFF="image: url(:/img/nok.png)";
    QString OFFQ="image: url(:/img/nokQuestion.png)";
    QString DC="image: url(:/img/offline.png)";
    //CB
    QIcon ImageOpen(":/img/openCB.png");
    QIcon ImageClose(":/img/closeCB.png");
    QIcon ImageInvalid(":/img/invalidCB.png");
    QIcon ImageCBDC(":/img/offlineCB.png");

    //CB HMI CONTROL
    QString CBCONTROLQUESTION="image: url(:/img/ControlQuestion.png)";
    QString SWITCHOPEN="image: url(:/img/kanan.png)";
    QString SWITCHCLOSE="image: url(:/img/kanan_kiri_Switch.png)";




    QString CBCONTROLINVALID="image: url(:/img/ControlInvalid.png)";
    QString CBOPEN="image: url(:/img/ControlOpen.png)";
    QString CBCLOSE="image: url(:/img/ControlClose.png)";
    QString CBINVALID="image: url(:/img/ControlOffline.png)";
    //  QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    //ui->Lbl_ControlOvBattery2->setStyleSheet(CBCLOSE);
    //METERING
    QIcon ImageOn(":/img/ok.png");
    QIcon ImageOff(":/img/nok.png");
    QIcon ImageDC(":/img/offline.png");
    QSqlQuery w1,w2,w3,w4,w5,w6,v1,v2,v3,v4,v5,v6,qryrtu,qryalarm,qrycbinverter,qrycbinverter_1,qrycbinverter_2,qrycbinverter_3,qrycbbattery,qryalarmbatt,qryswitch1,qryswitch2,qryswitch3,qryswitch4,qryswitch5,qryswitch6,qryswitch7,qryswitch8,qryswitch9,qryswitch10,qryswitch11,qryswitch12,qryswitch,stsBatSvp,qryStskomBdi1,qryStskomBdi2,qryStskomBdi3,qryStskomBdi4,qryStskomBdi5,qryStskomBdi6,qryAlarmAC1,qryAlarmAC2,qryAlarmAC3,qryAlarmInvFault1,qryAlarmInvFault2,qryAlarmInvFault3,qryAlarmInvFault4,qryAlarmInvFault5,qryAlarmInvFault6,qryKomunikasiRabbit,qrystate1,qrystate2,qrystate3,qrystate4,qrystate5,qrystate6;

    QString dw1,dw2,dw3,dw4,dw5,dw6,dv1,dv2,dv3,dv4,dv5,dv6,dataRTU,dataAlarmInv,dataCBInv,dataAlarmBat,dataCBBattry,dataSwitch,dataSwitch1,dataSwitch2,dataSwitch3,dataSwitch4,dataSwitch5,dataSwitch6,dataSwitch7,dataSwitch8,dataSwitch9,dataSwitch10,dataSwitch11,dataSwitch12,dataSvpBat,dataStsKomBdi1,dataStsKomBdi2,dataStsKomBdi3,dataStsKomBdi4,dataStsKomBdi5,dataStsKomBdi6,dataAlarmInvFault1,dataAlarmInvFault2,dataAlarmInvFault3,dataAlarmInvFault4,dataAlarmInvFault5,dataAlarmInvFault6,dataKomunikasiRabbit,datastate1,datastate2,datastate3,datastate4,datastate5,datastate6;

    //

    //==============================
    //get all data
    //==============================
    qryrtu.prepare(Query_RTU);
    qryrtu.exec();
    if(qryrtu.next()){
        dataRTU = qryrtu.value(3).toString();
    }
    else{
        dataRTU="5";
    }
    qryalarm.prepare(Query_ALARM);
    qryalarm.exec();
    if (qryalarm.next())
    {
        dataAlarmInv = qryalarm.value(3).toString();
    }else{
        dataAlarmInv = "5";
    }
    qrycbinverter.prepare(QueryCB);
    qrycbinverter.exec();
    if(qrycbinverter.next()){
        dataCBInv = qrycbinverter.value(3).toString();
    }else{
        dataCBInv = "5";
    }

    qryalarmbatt.prepare("select aBattery from in_cb_battery order by id desc limit 1");
    qryalarmbatt.exec();
    if(qryalarmbatt.next()){
        dataAlarmBat = qryalarmbatt.value(0).toString();
    }else{
        dataAlarmBat = "5";
    }

    qrycbbattery.prepare("select battery from in_battery order by id desc limit 1");
    qrycbbattery.exec();
    if(qrycbbattery.next()){
        dataCBBattry = qrycbbattery.value(0).toString();
    }else{
        dataCBBattry = "5";
    }

    stsBatSvp.prepare("select value from in_cb_battery_spv order by id desc limit 1");
    stsBatSvp.exec();
    if (stsBatSvp.next()){
        dataSvpBat = stsBatSvp.value(0).toString();
    }else{
        dataSvpBat = "5";
    }
    qryStskomBdi1.prepare("select value from in_cb_inverter_spv1 order by id desc limit 1");
    qryStskomBdi1.exec();
    if (qryStskomBdi1.next()){
        dataStsKomBdi1 = qryStskomBdi1.value(0).toString();
    }else{
        dataStsKomBdi1 = "5";
    }
    qryStskomBdi2.prepare("select value from in_cb_inverter_spv2 order by id desc limit 1");
    qryStskomBdi2.exec();
    if (qryStskomBdi2.next()){
        dataStsKomBdi2 = qryStskomBdi2.value(0).toString();
    }else{
        dataStsKomBdi2 = "5";
    }
    qryStskomBdi3.prepare("select value from in_cb_inverter_spv3 order by id desc limit 1");
    qryStskomBdi3.exec();
    if (qryStskomBdi3.next()){
        dataStsKomBdi3 = qryStskomBdi3.value(0).toString();
    }else{
        dataStsKomBdi3 = "5";
    }
    qryStskomBdi4.prepare("select value from in_cb_inverter_spv4 order by id desc limit 1");
    qryStskomBdi4.exec();
    if (qryStskomBdi4.next()){
        dataStsKomBdi4 = qryStskomBdi4.value(0).toString();
    }else{
        dataStsKomBdi4 = "5";
    }
    qryStskomBdi5.prepare("select value from in_cb_inverter_spv5 order by id desc limit 1");
    qryStskomBdi5.exec();
    if (qryStskomBdi5.next()){
        dataStsKomBdi5 = qryStskomBdi5.value(0).toString();
    }else{
        dataStsKomBdi5 = "5";
    }
    qryStskomBdi6.prepare("select value from in_cb_inverter_spv6 order by id desc limit 1");
    qryStskomBdi6.exec();
    if (qryStskomBdi6.next()){
        dataStsKomBdi6 = qryStskomBdi6.value(0).toString();
    }else{
        dataStsKomBdi6 = "5";
    }

    qryKomunikasiRabbit.prepare("select value from in_cb_battery_com order by id desc limit 1");
    qryKomunikasiRabbit.exec();
    if (qryKomunikasiRabbit.next()){
        dataKomunikasiRabbit = qryKomunikasiRabbit.value(0).toString();
    }else{
        dataKomunikasiRabbit = "5";
    }

    //QString dataRTU,dataAlarmInv,dataCBInv,dataAlarmBat,dataCBBattry;
    //RTU on
    if(dataRTU == "1"){
        //change status metering
        //inverter
        INVERTER->setIcon(ImageOn);
        ui->twStatusMetering->setItem(RowRTU, Column, INVERTER);
        //battery nanti berdasarkan spv
        //get data alarm fault

        w1.prepare("select invW,invVar from in_power_inverter_r order by id desc limit 1");
        w1.exec();
        if (w1.next()){
            dw1 = w1.value(0).toString();
            dv1 = w1.value(1).toString();
        }

        w2.prepare("select invW,invVar from in_power_inverter_r1 order by id desc limit 1");
        w2.exec();
        if (w2.next()){
            dw2 = w2.value(0).toString();
            dv2 = w2.value(1).toString();
        }

        w3.prepare("select invW,invVar from in_power_inverter_s order by id desc limit 1");
        w3.exec();
        if (w3.next()){
            dw3 = w3.value(0).toString();
            dv3 = w3.value(1).toString();
        }

        w4.prepare("select invW,invVar from in_power_inverter_s1 order by id desc limit 1");
        w4.exec();
        if (w4.next()){
            dw4 = w4.value(0).toString();
            dv4 = w4.value(1).toString();
        }

        w5.prepare("select invW,invVar from in_power_inverter_t order by id desc limit 1");
        w5.exec();
        if (w5.next()){
            dw5 = w5.value(0).toString();
            dv5 = w5.value(1).toString();
        }

        w6.prepare("select invW,invVar from in_power_inverter_t1 order by id desc limit 1");
        w6.exec();
        if (w6.next()){
            dw6 = w6.value(0).toString();
            dv6 = w6.value(1).toString();
        }

        ui->lnInvRW->display(dw1);
        ui->lnInvRVar->display(dv1);

        ui->lnInvSW->display(dw2);
        ui->lnInvSVar->display(dv2);

        ui->lnInvTW->display(dw3);
        ui->lnInvTVar->display(dv3);

        ui->lnInvRW_2->display(dw4);
        ui->lnInvRVar_2->display(dv4);

        ui->lnInvSW_2->display(dw5);
        ui->lnInvSVar_2->display(dv5);

        ui->lnInvTW_2->display(dw6);
        ui->lnInvTVar_2->display(dv6);

        qryAlarmInvFault1.prepare("select value from in_cb_inverter_fault1 order by id desc limit 1");
        qryAlarmInvFault1.exec();
        if (qryAlarmInvFault1.next()){
            dataAlarmInvFault1 = qryAlarmInvFault1.value(0).toString();
        }else{
            dataAlarmInvFault1 = "5";
        }

        qryAlarmInvFault2.prepare("select value from in_cb_inverter_fault2 order by id desc limit 1");
        qryAlarmInvFault2.exec();
        if (qryAlarmInvFault2.next()){
            dataAlarmInvFault2 = qryAlarmInvFault2.value(0).toString();
        }else{
            dataAlarmInvFault2 = "5";
        }
        qryAlarmInvFault3.prepare("select value from in_cb_inverter_fault3 order by id desc limit 1");
        qryAlarmInvFault3.exec();
        if (qryAlarmInvFault3.next()){
            dataAlarmInvFault3 = qryAlarmInvFault3.value(0).toString();
        }else{
            dataAlarmInvFault3 = "5";
        }
        qryAlarmInvFault4.prepare("select value from in_cb_inverter_fault4 order by id desc limit 1");
        qryAlarmInvFault4.exec();
        if (qryAlarmInvFault4.next()){
            dataAlarmInvFault4 = qryAlarmInvFault4.value(0).toString();
        }else{
            dataAlarmInvFault4 = "5";
        }

        qryAlarmInvFault5.prepare("select value from in_cb_inverter_fault5 order by id desc limit 1");
        qryAlarmInvFault5.exec();
        if (qryAlarmInvFault5.next()){
            dataAlarmInvFault5 = qryAlarmInvFault5.value(0).toString();
        }else{
            dataAlarmInvFault5 = "5";
        }
        qryAlarmInvFault6.prepare("select value from in_cb_inverter_fault6 order by id desc limit 1");
        qryAlarmInvFault6.exec();
        if (qryAlarmInvFault6.next()){
            dataAlarmInvFault6 = qryAlarmInvFault6.value(0).toString();
        }else{
            dataAlarmInvFault6 = "5";
        }
        //get data switch
        qryswitch.prepare("select bdimonsw from in_inverter_switch order by id desc limit 1");
        qryswitch.exec();
        if (qryswitch.next()){
            dataSwitch = qryswitch.value(0).toString();
        }else{
            dataSwitch = "5";
        }

        //get data switch
        qryswitch1.prepare("select bdi1sw from in_inverter_switch_1 order by id desc limit 1");
        qryswitch1.exec();
        if (qryswitch1.next()){
            dataSwitch1 = qryswitch1.value(0).toString();
        }else{
            dataSwitch1 = "5";
        }
        //get data switch
        qryswitch2.prepare("select bdi2sw from in_inverter_switch_2 order by id desc limit 1");
        qryswitch2.exec();
        if (qryswitch2.next()){
            dataSwitch2 = qryswitch2.value(0).toString();
        }else{
            dataSwitch2 = "5";
        }
        //get data switch
        qryswitch3.prepare("select bdi3sw from in_inverter_switch_3 order by id desc limit 1");
        qryswitch3.exec();
        if (qryswitch3.next()){
            dataSwitch3 = qryswitch3.value(0).toString();
        }else{
            dataSwitch3 = "5";
        }

        qryswitch4.prepare("select value from in_inverter_switch_4 order by id desc limit 1");
        qryswitch4.exec();
        if (qryswitch4.next()){
            dataSwitch4 = qryswitch4.value(0).toString();
        }else{
            dataSwitch4 = "5";
        }
        //get data switch
        qryswitch5.prepare("select value from in_inverter_switch_5 order by id desc limit 1");
        qryswitch5.exec();
        if (qryswitch5.next()){
            dataSwitch5 = qryswitch5.value(0).toString();
        }else{
            dataSwitch5 = "5";
        }
        //get data switch
        qryswitch6.prepare("select value from in_inverter_switch_6 order by id desc limit 1");
        qryswitch6.exec();
        if (qryswitch6.next()){
            dataSwitch6 = qryswitch6.value(0).toString();
        }else{
            dataSwitch6 = "5";
        }


        //get data switch
        qryswitch7.prepare("select value from in_inverter_switch_7 order by id desc limit 1");
        qryswitch7.exec();
        if (qryswitch7.next()){
            dataSwitch7 = qryswitch7.value(0).toString();
        }else{
            dataSwitch7 = "5";
        }
        //get data switch
        qryswitch8.prepare("select value from in_inverter_switch_8 order by id desc limit 1");
        qryswitch8.exec();
        if (qryswitch8.next()){
            dataSwitch8 = qryswitch8.value(0).toString();
        }else{
            dataSwitch8 = "5";
        }
        //get data switch
        qryswitch9.prepare("select value from in_inverter_switch_9 order by id desc limit 1");
        qryswitch9.exec();
        if (qryswitch9.next()){
            dataSwitch9 = qryswitch9.value(0).toString();
        }else{
            dataSwitch9 = "5";
        }

        qryswitch10.prepare("select value from in_inverter_switch_10 order by id desc limit 1");
        qryswitch10.exec();
        if (qryswitch10.next()){
            dataSwitch10 = qryswitch10.value(0).toString();
        }else{
            dataSwitch10 = "5";
        }
        //get data switch
        qryswitch11.prepare("select value from in_inverter_switch_11 order by id desc limit 1");
        qryswitch11.exec();
        if (qryswitch11.next()){
            dataSwitch11 = qryswitch11.value(0).toString();
        }else{
            dataSwitch11 = "5";
        }
        //get data switch
        qryswitch12.prepare("select value from in_inverter_switch_12 order by id desc limit 1");
        qryswitch12.exec();
        if (qryswitch12.next()){
            dataSwitch12 = qryswitch12.value(0).toString();
        }else{
            dataSwitch12 = "5";
        }


        qrystate1.prepare("select value from in_cb_inverter_state_r1 order by id desc limit 1");
        qrystate1.exec();
        if (qrystate1.next()){
            datastate1 = qrystate1.value(0).toString();
            if (datastate1=="1")
            {
                ui->StatusR1->setText("Stanby");
            }
            else if (datastate1=="5")
            {
                ui->StatusR1->setText("Stand Alone");
            }
            else if (datastate1=="4")
            {
                ui->StatusR1->setText("Connected");
            }
            else
            {
                ui->StatusR1->setText("");
            }
        }

        qrystate2.prepare("select value from in_cb_inverter_state_r2 order by id desc limit 1");
        qrystate2.exec();
        if (qrystate2.next()){
            datastate2 = qrystate2.value(0).toString();
            if (datastate2=="1")
            {
                ui->StatusR2->setText("Stanby");
            }
            else if (datastate2=="5")
            {
                ui->StatusR2->setText("Stand Alone");
            }
            else if (datastate2=="4")
            {
                ui->StatusR2->setText("Connected");
            }
            else
            {
                ui->StatusR2->setText("");
            }
        }

        qrystate3.prepare("select value from in_cb_inverter_state_s1 order by id desc limit 1");
        qrystate3.exec();
        if (qrystate3.next()){
            datastate3 = qrystate3.value(0).toString();
            if (datastate3=="1")
            {
                ui->StatusS1->setText("Stanby");
            }
            else if (datastate3=="5")
            {
                ui->StatusS1->setText("Stand Alone");
            }
            else if (datastate3=="4")
            {
                ui->StatusS1->setText("Connected");
            }
            else
            {
                ui->StatusS1->setText("");
            }
        }

        qrystate4.prepare("select value from in_cb_inverter_state_s2 order by id desc limit 1");
        qrystate4.exec();
        if (qrystate4.next()){
            datastate4 = qrystate4.value(0).toString();
            if (datastate4=="1")
            {
                ui->StatusS2->setText("Stanby");
            }
            else if (datastate4=="5")
            {
                ui->StatusS2->setText("Stand Alone");
            }
            else if (datastate4=="4")
            {
                ui->StatusS2->setText("Connected");
            }
            else
            {
                ui->StatusS2->setText("");
            }
        }

        qrystate5.prepare("select value from in_cb_inverter_state_t1 order by id desc limit 1");
        qrystate5.exec();
        if (qrystate5.next()){
            datastate5 = qrystate5.value(0).toString();
            if (datastate5=="1")
            {
                ui->StatusT1->setText("Stanby");
            }
            else if (datastate5=="5")
            {
                ui->StatusT1->setText("Stand Alone");
            }
            else if (datastate5=="4")
            {
                ui->StatusT1->setText("Connected");
            }
            else
            {
                ui->StatusT1->setText("");
            }
        }

        qrystate6.prepare("select value from in_cb_inverter_state_t2 order by id desc limit 1");
        qrystate6.exec();
        if (qrystate6.next()){
            datastate6 = qrystate6.value(0).toString();
            if (datastate6=="1")
            {
                ui->StatusT2->setText("Stanby");
            }
            else if (datastate6=="5")
            {
                ui->StatusT2->setText("Stand Alone");
            }
            else if (datastate6=="4")
            {
                ui->StatusT2->setText("Connected");
            }
            else
            {
                ui->StatusT2->setText("");
            }
        }

        //==============================
        //inverter
        //==============================

        ui->lblindiinverter_RTU->setStyleSheet(ON);
        if(dataAlarmInverterRtu == 1){
            dataout[28] = "ALARM RTU Inverter Condition is Normal !!!";
            InsertDataControl_CB(15);
        }
        dataAlarmInverterRtu = 0;
        Get_ColorWidget_INVERTER(true,1,"#FF69FF");
        //ui->itest
        removeRowAlarmTable("RTU INVERTER Condition is ON !!!");
        removeRowAlarmTable("ALARM INVERTER Condition is OFFLINE !!!");
        //set to hmi network
        ui->lbRTU_INVERTER->setStyleSheet(ON);
        ui->lblStatusRTUINVERTER->setText("ONLINE");
        removeRowAlarmTable("Status Network Inverter Condition Is Offline");
        if(!NetInverter){
            dataout[28] = "Status Network INVERTER Condition Is Normal";
            InsertDataControl_CB(17);
        }
        NetInverter = true;
        //set to metering
        INVERTER->setIcon(ImageOn);
        ui->twStatusMetering->setItem(RowCb, Column, INVERTER);
        //pengecekan cb Inverter
        //==============================
        //invalid
        //==============================

        if (statuscb==1)
        {
            if(dataCBInv == "0" || dataCBInv == "3"){
                CBINVERTER->setIcon(ImageInvalid);
                CBINVERTER->setSizeHint(QSize(100, 100));
                // ui->CBALARMINVERTER->setStyleSheet(OFF);
                ui->twStatusMetering->setItem(RowCb, Column, CBINVERTER);
                ui->Lbl_ControlInverter->setStyleSheet(CBCONTROLINVALID);
                ui->Lbl_OvControlInverter->setStyleSheet(CBCONTROLINVALID);
                if(dataConnectionINVERTER != 2){
                    dataout[28] = "Status CB INVERTER Condition Is Invalid";
                    InsertDataControl_CB(15);
                }
                dataConnectionINVERTER = 2;

                ui->ArrowInverter->setVisible(0);
                ui->ArrowInverter_Ov->setVisible(0);

                ui->pbInverter->setEnabled(0);
                ui->pbInverterControl->setEnabled(0);


            }
            //open
            else if(dataCBInv == "1"){

                CBINVERTER->setIcon(ImageOpen);
                CBINVERTER->setSizeHint(QSize(100, 100));
                //  ui->CBALARMINVERTER->setStyleSheet(ON);

                ui->twStatusMetering->setItem(RowCb, Column, CBINVERTER);
                ui->Lbl_ControlInverter->setStyleSheet(CBOPEN);
                ui->Lbl_OvControlInverter->setStyleSheet(CBOPEN);

                ui->ArrowInverter->setVisible(0);
                ui->ArrowInverter_Ov->setVisible(0);

                ui->pbInverter->setEnabled(1);
                ui->pbInverterControl->setEnabled(1);
                if(dataConnectionINVERTER == 0){
                    dataout[28] = "Status CB INVERTER Condition is opened";
                    InsertDataControl_CB(15);
                }
                dataConnectionINVERTER = 1;


            }
            //close
            else if(dataCBInv == "2"){

                CBINVERTER->setIcon(ImageClose);
                CBINVERTER->setSizeHint(QSize(100, 100));

                ui->twStatusMetering->setItem(RowCb, Column, CBINVERTER);
                ui->Lbl_ControlInverter->setStyleSheet(CBCLOSE);
                ui->Lbl_OvControlInverter->setStyleSheet(CBCLOSE);


                ui->ArrowInverter->setVisible(1);
                ui->ArrowInverter_Ov->setVisible(1);

                ui->pbInverter->setEnabled(1);
                ui->pbInverterControl->setEnabled(1);
                if(dataConnectionINVERTER == 1){
                    dataout[28] = "Status CB INVERTER Condition is closed";
                    InsertDataControl_CB(15);
                }
                dataConnectionINVERTER = 0;

                QString Kiri="image: url(:img/DirectNormal.png)";
                QString Kanan="image: url(:img/right_direct_.png)";
                if(dataarrow >= 0){
                    //panah KIRI
                    ui->ArrowInverter->setStyleSheet(Kanan);
                    ui->ArrowInverter_Ov->setStyleSheet(Kanan);
                }
                else{
                    //panah kanan
                    ui->ArrowInverter->setStyleSheet(Kiri);
                    ui->ArrowInverter_Ov->setStyleSheet(Kiri);
                }
            }
            //tidak ada data
            else{
                CBINVERTER->setIcon(ImageInvalid);
                CBINVERTER->setSizeHint(QSize(100, 100));

                //  ui->CBALARMINVERTER->setStyleSheet(ON);

                ui->twStatusMetering->setItem(RowCb, Column, CBINVERTER);
                ui->Lbl_ControlInverter->setStyleSheet(CBCONTROLQUESTION);
                ui->Lbl_OvControlInverter->setStyleSheet(CBCONTROLQUESTION);
                dataConnectionINVERTER = 2;

                ui->ArrowInverter->setVisible(0);
                ui->ArrowInverter_Ov->setVisible(0);

                ui->pbInverter->setEnabled(0);
                ui->pbInverterControl->setEnabled(0);

            }
        }
        //==============================
        //Check alarm
        //==============================
        //        qryalarm.prepare(Query_ALARM);
        //        qryalarm.exec();
        //        if (qryalarm.next())
        //        {
        //            varInverter=qryalarm.value(3).toString();
        //            //==============================
        //            //when ALARM ON
        //            //==============================
        //            if (varInverter=="1")
        //            {
        //                ui->CBALARMINVERTER->setStyleSheet(ON);
        //                removeRowAlarmTable("CB ALARM INVERTER 1 Condition is ON !!!");
        //                if(dataAlarmPLTMH1 == 1){
        //                    dataout[28] = "CB ALARM INVERTER 1 Condition is Normal !!!";
        //                    InsertDataControl_CB(6);
        //                }
        //                dataAlarmInverter = 0;
        //                INVERTER->setIcon(ImageOn);
        //                ui->twStatusMetering->setItem(RowAlarm1, Column, INVERTER);

        //                ui->pbInverterControl->setEnabled(1);
        //                ui->pbInverter1->setEnabled(1);

        //            }
        //            else if(varInverter=="0")
        //            {
        //                ui->CBALARMINVERTER->setStyleSheet(OFF);
        //                addRowAlarmTable("ASDU 1","-","Server Protocol","CB INVERTER 1 Condition is ON !!!");
        //                if(dataAlarmInverter == 0){
        //                    dataout[28] = "CB ALARM INVERTER 1 Condition is ON !!!";
        //                    InsertDataControl_CB(6);
        //                }
        //                //  dataAlarmPLTMH1 = 1;
        //                INVERTER->setIcon(ImageOff);
        //                ui->twStatusMetering->setItem(RowAlarm1, Column, INVERTER);

        //                ui->pbInverterControl->setEnabled(1);
        //                ui->pbInverter1->setEnabled(1);

        //                ui->Lbl_OvControlInverter->setStyleSheet(CBCONTROLINVALID); // CBINVALID
        //                ui->Lbl_ControlInverter->setStyleSheet(CBCONTROLINVALID);

        //                //   ui->ArrowPLTMH->setVisible(0);
        //                // ui->OvArrowPltmh->setVisible(0);

        //                dataConnectionINVERTER = 2;
        //            }
        //        }
        //        else{
        //            ui->CBALARMINVERTER->setStyleSheet(OFFQ);
        //        }
        //==============================
        //switchbro
        //==============================
        //swtich close
        if(dataSwitch == "1"){
            //  ui->switchInvOv->setStyleSheet(SWITCHCLOSE);
            //  ui->switchInvAlarm->setStyleSheet(SWITCHCLOSE);

            ui->switchInvAlarm->setVisible(false);
            ui->SwitchMainCBDC->setVisible(true);
            ui->switchInvOv->setVisible(false);
            ui->SwitchOnOv->setVisible(true);

            if(dataAlarmInvDc4 == 1){
                dataout[28] = "ALARM Inverter DC 4 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter DC 4 Condition is ON !!!");
            dataAlarmInvDc4 = 0;
            ui->CBDCALARMINVERTER->setStyleSheet(ON);
        }
        //swtich open
        else if(dataSwitch == "0"){
            //            ui->switchInvOv->setStyleSheet(SWITCHOPEN);
            //            ui->switchInvAlarm->setStyleSheet(SWITCHOPEN);
            ui->switchInvAlarm->setVisible(true);
            ui->SwitchMainCBDC->setVisible(false);
            ui->switchInvOv->setVisible(true);
            ui->SwitchOnOv->setVisible(false);

            ui->CBDCALARMINVERTER->setStyleSheet(OFF);
            if(dataAlarmInvDc4 == 0){
                dataout[28] = "ALARM Inverter DC 4 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter DC 4 Condition is ON !!!");
            dataAlarmInvDc4 = 1;
        }
        //database kosong
        else if(dataSwitch == "5"){
            ui->CBDCALARMINVERTER->setStyleSheet(OFFQ);
        }

        //cek data komunikasi 1
        if(dataStsKomBdi1 == "1"){

            //SET NORMAL  == 1
            //ui->StsKomSPVBDI1->setStyleSheet(ON);

            setKomunikasiBDI1(1);
            ui->StsKomSPVBDI1->setStyleSheet(ON);

            ui->RBDI1->setStyleSheet(ON);
            ui->LRBDI1->setText("ONLINE");
            if(dataKomInvSPV1 == 1){
                dataout[28] = "ALARM Komunikasi SPV BDI 1 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Komunikasi SPV BDI 1 Condition is ON !!!");
            dataKomInvSPV1 = 0;

            //===========================================
            // cek data fault langsung
            //===========================================
            if(dataAlarmInvFault1 == "0"){
                ui->CBAC1ALARM_OV->setStyleSheet(ON);
                if(dataAlarmInvFaultAC1 == 1){
                    dataout[28] = "ALARM Fault AC 1 Condition is Normal !!!";
                    InsertDataControl_CB(15);
                }
                removeRowAlarmTable("ALARM Fault AC 1 Condition is ON !!!");
                dataAlarmInvFaultAC1 = 0;

            }
            else if(dataAlarmInvFault1 > "0"){
                ui->CBAC1ALARM_OV->setStyleSheet(OFF);
                if(dataAlarmInvFaultAC1 == 0){
                    dataout[28] = "ALARM Fault AC 1 Condition is ON !!!";
                    InsertDataControl_CB(15);
                }

                addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Fault AC 1 Condition is ON !!!");
                dataAlarmInvFaultAC1 = 1;

            }
            //            else if(dataAlarmInvFault1 == "5"){
            //                ui->CBAC1ALARM_OV->setStyleSheet(OFFQ);
            //            }



        }else if(dataStsKomBdi1 == "0"){
            //SET PINK == 0
            setKomunikasiBDI1(0);
            ui->StsKomSPVBDI1->setStyleSheet(OFF);
            ui->RBDI1->setStyleSheet(OFF);
            ui->LRBDI1->setText("OFFLINE");
            //event list
            if(dataKomInvSPV1 == 0){
                dataout[28] = "ALARM Komunikasi SPV BDI 1 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Komunikasi SPV BDI 1 Condition is ON !!!");

            dataKomInvSPV1 = 1;
        }else if(dataStsKomBdi1 == "5"){
            //SET PINK == 0
            setKomunikasiBDI1(0);
            ui->StsKomSPVBDI1->setStyleSheet(OFFQ);
            ui->RBDI1->setStyleSheet(OFFQ);
            ui->LRBDI1->setText("OFFLINE");
        }

        //cek data komunikasi 2
        if(dataStsKomBdi2 == "1"){
            //SET NORMAL  == 1
            setKomunikasiBDI2(1);
            ui->StsKomSPVBDI2->setStyleSheet(ON);
            ui->RBDI2->setStyleSheet(ON);
            ui->LRBDI2->setText("ONLINE");
            //event list
            if(dataKomInvSPV2 == 1){
                dataout[28] = "ALARM Komunikasi SPV BDI 2 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Komunikasi SPV BDI 2 Condition is ON !!!");
            dataKomInvSPV2 = 0;
            //PENGECEKAN CB ALARM AC 2


            if(dataAlarmInvFault2 == "0"){
                ui->CBAC2ALARM_OV->setStyleSheet(ON);
                if(dataAlarmInvFaultAC2 == 1){
                    dataout[28] = "ALARM Fault AC 2 Condition is Normal !!!";
                    InsertDataControl_CB(15);
                }
                removeRowAlarmTable("ALARM Fault AC 2 Condition is ON !!!");
                dataAlarmInvFaultAC2 = 0;
            }
            else if(dataAlarmInvFault2 > "0"){
                ui->CBAC2ALARM_OV->setStyleSheet(OFF);
                if(dataAlarmInvFaultAC2 == 0){
                    dataout[28] = "ALARM Fault AC 2 Condition is ON !!!";
                    InsertDataControl_CB(15);
                }

                addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Fault AC 2 Condition is ON !!!");
                dataAlarmInvFaultAC2 = 1;
            }
            //            else if(dataAlarmInvFault2 == "5"){
            //                ui->CBAC2ALARM_OV->setStyleSheet(OFFQ);
            //            }

        }else if(dataStsKomBdi2 == "0"){
            //SET PINK == 0
            setKomunikasiBDI2(0);
            ui->StsKomSPVBDI2->setStyleSheet(OFF);
            ui->RBDI2->setStyleSheet(OFF);
            ui->LRBDI2->setText("OFFLINE");
            //event list
            if(dataKomInvSPV2 == 0){
                dataout[28] = "ALARM Komunikasi SPV BDI 2 Condition is ON !!!";
                InsertDataControl_CB(15);
            }

            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Komunikasi SPV BDI 2 Condition is ON !!!");
            dataKomInvSPV2 = 1;
        }else if(dataStsKomBdi2 == "5"){
            //SET PINK == 0
            setKomunikasiBDI2(0);
            ui->StsKomSPVBDI2->setStyleSheet(OFFQ);
            ui->RBDI2->setStyleSheet(OFFQ);
            ui->LRBDI2->setText("OFFLINE");
        }

        //cek data komunikasi 3
        if(dataStsKomBdi3 == "1"){
            //SET NORMAL  == 1
            setKomunikasiBDI3(1);
            ui->StsKomSPVBDI3->setStyleSheet(ON);
            ui->RBDI3->setStyleSheet(ON);
            ui->LRBDI3->setText("ONLINE");
            //event list
            if(dataKomInvSPV3 == 1){
                dataout[28] = "ALARM Komunikasi SPV BDI 3 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Komunikasi SPV BDI 3 Condition is ON !!!");
            dataKomInvSPV3 = 0;


            //pengecekan fault
            if(dataAlarmInvFault3 == "0"){
                ui->CBAC3ALARM_OV->setStyleSheet(ON);
                if(dataAlarmInvFaultAC3 == 1){
                    dataout[28] = "ALARM Fault AC 3 Condition is Normal !!!";
                    InsertDataControl_CB(15);
                }
                removeRowAlarmTable("ALARM Fault AC 3 Condition is ON !!!");
                dataAlarmInvFaultAC3 = 0;
            }
            else if(dataAlarmInvFault3 > "0"){
                ui->CBAC3ALARM_OV->setStyleSheet(OFF);
                if(dataAlarmInvFaultAC3 == 0){
                    dataout[28] = "ALARM Fault AC 3 Condition is ON !!!";
                    InsertDataControl_CB(15);
                }

                addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Fault AC 3 Condition is ON !!!");
                dataAlarmInvFaultAC3 = 1;
            }
            //            else if(dataAlarmInvFault3 == "5"){
            //                ui->CBAC3ALARM_OV->setStyleSheet(OFFQ);
            //            }



        }else if(dataStsKomBdi3 == "0"){
            //SET PINK == 0
            setKomunikasiBDI3(0);
            ui->StsKomSPVBDI3->setStyleSheet(OFF);
            ui->RBDI3->setStyleSheet(OFF);
            ui->LRBDI3->setText("OFFLINE");
            //event list
            if(dataKomInvSPV3 == 0){
                dataout[28] = "ALARM Komunikasi SPV BDI 3 Condition is ON !!!";
                InsertDataControl_CB(15);
            }

            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Komunikasi SPV BDI 3 Condition is ON !!!");
            dataKomInvSPV3 = 1;


        }else if(dataStsKomBdi3 == "5"){
            //SET PINK == 0
            setKomunikasiBDI3(0);
            ui->StsKomSPVBDI3->setStyleSheet(OFFQ);
            ui->RBDI3->setStyleSheet(OFFQ);
            ui->LRBDI3->setText("OFFLINE");
        }

        if(dataStsKomBdi4 == "1"){
            ui->StsKomSPVBDI4->setStyleSheet(ON);
            ui->RBDI4->setStyleSheet(ON);
            ui->LRBDI4->setText("ONLINE");
            setKomunikasiBDI4(1);
            //event list
            if(dataKomInvSPV4 == 1){
                dataout[28] = "ALARM Komunikasi SPV BDI 4 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Komunikasi SPV BDI 4 Condition is ON !!!");
            dataKomInvSPV4 = 0;

            //pengecekan fault
            if(dataAlarmInvFault4 == "0"){
                ui->CBAC4ALARM_OV->setStyleSheet(ON);
                if(dataAlarmInvFaultAC4 == 1){
                    dataout[28] = "ALARM Fault AC 4 Condition is Normal !!!";
                    InsertDataControl_CB(15);
                }
                removeRowAlarmTable("ALARM Fault AC 4 Condition is ON !!!");
                dataAlarmInvFaultAC4 = 0;
            }
            else if(dataAlarmInvFault4 > "0"){
                ui->CBAC4ALARM_OV->setStyleSheet(OFF);
                if(dataAlarmInvFaultAC4 == 0){
                    dataout[28] = "ALARM Fault AC 4 Condition is ON !!!";
                    InsertDataControl_CB(15);
                }

                addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Fault AC 4 Condition is ON !!!");
                dataAlarmInvFaultAC4 = 1;
            }
            //            else if(dataAlarmInvFault4 == "5"){
            //                ui->CBAC4ALARM_OV->setStyleSheet(OFFQ);
            //            }

        }else if(dataStsKomBdi4 == "0"){
            setKomunikasiBDI4(0);
            ui->StsKomSPVBDI4->setStyleSheet(OFF);
            ui->RBDI4->setStyleSheet(OFF);
            ui->LRBDI4->setText("OFFLINE");
            //event list
            if(dataKomInvSPV4 == 0){
                dataout[28] = "ALARM Komunikasi SPV BDI 4 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Komunikasi SPV BDI 4 Condition is ON !!!");
            dataKomInvSPV4 = 1;

        }
        else if(dataStsKomBdi4 == "5"){
            setKomunikasiBDI4(0);
            ui->StsKomSPVBDI4->setStyleSheet(OFFQ);
        }


        if(dataStsKomBdi5 == "1"){
            ui->StsKomSPVBDI5->setStyleSheet(ON);
            ui->RBDI5->setStyleSheet(ON);
            ui->LRBDI5->setText("ONLINE");
            setKomunikasiBDI5(1);
            //event list
            if(dataKomInvSPV5 == 1){
                dataout[28] = "ALARM Komunikasi SPV BDI 5 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Komunikasi SPV BDI 5 Condition is ON !!!");
            dataKomInvSPV5 = 0;

            //pengecekan fault
            if(dataAlarmInvFault5 == "0"){
                ui->CBAC5ALARM_OV->setStyleSheet(ON);
                if(dataAlarmInvFaultAC5 == 1){
                    dataout[28] = "ALARM Fault AC 5 Condition is Normal !!!";
                    InsertDataControl_CB(15);
                }
                removeRowAlarmTable("ALARM Fault AC 5 Condition is ON !!!");
                dataAlarmInvFaultAC4 = 0;
            }
            else if(dataAlarmInvFault5 > "0"){
                ui->CBAC5ALARM_OV->setStyleSheet(OFF);
                if(dataAlarmInvFaultAC5 == 0){
                    dataout[28] = "ALARM Fault AC 5 Condition is ON !!!";
                    InsertDataControl_CB(15);
                }

                addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Fault AC 5 Condition is ON !!!");
                dataAlarmInvFaultAC5 = 1;
            }
            //            else if(dataAlarmInvFault5 == "5"){
            //                ui->CBAC5ALARM_OV->setStyleSheet(OFFQ);
            //            }

        }else if(dataStsKomBdi5 == "0"){
            setKomunikasiBDI5(0);
            ui->StsKomSPVBDI5->setStyleSheet(OFF);
            ui->RBDI5->setStyleSheet(OFF);
            ui->LRBDI5->setText("OFFLINE");
            //event list
            if(dataKomInvSPV5 == 0){
                dataout[28] = "ALARM Komunikasi SPV BDI 5 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Komunikasi SPV BDI 5 Condition is ON !!!");
            dataKomInvSPV5 = 1;

        }
        else if(dataStsKomBdi5 == "5"){
            setKomunikasiBDI5(0);
            ui->StsKomSPVBDI5->setStyleSheet(OFFQ);
        }


        if(dataStsKomBdi6 == "1"){
            ui->StsKomSPVBDI6->setStyleSheet(ON);
            ui->RBDI6->setStyleSheet(ON);
            ui->LRBDI6->setText("ONLINE");
            setKomunikasiBDI6(1);
            //event list
            if(dataKomInvSPV6 == 1){
                dataout[28] = "ALARM Komunikasi SPV BDI 6 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Komunikasi SPV BDI 6 Condition is ON !!!");
            dataKomInvSPV6 = 0;

            //pengecekan fault
            if(dataAlarmInvFault6 == "0"){
                ui->CBAC6ALARM_OV->setStyleSheet(ON);
                if(dataAlarmInvFaultAC6 == 1){
                    dataout[28] = "ALARM Fault AC 6 Condition is Normal !!!";
                    InsertDataControl_CB(15);
                }
                removeRowAlarmTable("ALARM Fault AC 6 Condition is ON !!!");
                dataAlarmInvFaultAC6 = 0;
            }
            else if(dataAlarmInvFault6 > "0"){
                ui->CBAC6ALARM_OV->setStyleSheet(OFF);
                if(dataAlarmInvFaultAC6 == 0){
                    dataout[28] = "ALARM Fault AC 6 Condition is ON !!!";
                    InsertDataControl_CB(15);
                }

                addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Fault AC 6 Condition is ON !!!");
                dataAlarmInvFaultAC6 = 1;
            }
            //            else if(dataAlarmInvFault6 == "5"){
            //                ui->CBAC6ALARM_OV->setStyleSheet(OFFQ);
            //            }

        }else if(dataStsKomBdi6 == "0"){
            setKomunikasiBDI6(0);
            ui->StsKomSPVBDI6->setStyleSheet(OFF);
            ui->RBDI6->setStyleSheet(OFF);
            ui->LRBDI6->setText("OFFLINE");
            //event list
            if(dataKomInvSPV6 == 0){
                dataout[28] = "ALARM Komunikasi SPV BDI 6 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Komunikasi SPV BDI 6 Condition is ON !!!");
            dataKomInvSPV6 = 1;

        }
        else if(dataStsKomBdi6 == "5"){
            setKomunikasiBDI6(0);
            ui->StsKomSPVBDI6->setStyleSheet(OFFQ);
        }

        //pengecekan switch
        if(dataSwitch1 == "1"){
            //ui->switchInv1A->setStyleSheet(SWITCHCLOSE);
            ui->switchInv1A->setVisible(false);
            ui->SwitchCBDC1->setVisible(true);
            ui->CBDC1ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc1 == 1){
                dataout[28] = "ALARM Inverter DC 1 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter DC 1 Condition is ON !!!");
            dataAlarmInvDc1 = 0;
        }
        else if(dataSwitch1 == "0"){
            //  ui->switchInv1A->setStyleSheet(SWITCHOPEN);
            ui->switchInv1A->setVisible(true);
            ui->SwitchCBDC1->setVisible(false);
            ui->CBDC1ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc1 == 0){
                dataout[28] = "ALARM Inverter DC 1 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter DC 1 Condition is ON !!!");
            dataAlarmInvDc1 = 1;
        }
        else if (dataSwitch1 == "5"){
            ui->CBDC1ALARM->setStyleSheet(OFFQ);
        }

        //pengecekan switch
        if(dataSwitch2 == "1"){
            //ui->switchInv2->setStyleSheet(SWITCHCLOSE);
            ui->switchInv2A->setVisible(false);
            ui->SwitchCBDC2->setVisible(true);

            ui->CBDC2ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc2 == 1){
                dataout[28] = "ALARM Inverter DC 2 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter DC 2 Condition is ON !!!");
            dataAlarmInvDc2 = 0;
        }
        else if(dataSwitch2 == "0"){
            //ui->switchInv2A->setStyleSheet(SWITCHOPEN);
            ui->switchInv2A->setVisible(true);
            ui->SwitchCBDC2->setVisible(false);

            ui->CBDC2ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc2 == 0){
                dataout[28] = "ALARM Inverter DC 2 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter DC 2 Condition is ON !!!");
            dataAlarmInvDc2 = 1;
        }
        else if (dataSwitch2 == "5"){
            ui->CBDC2ALARM->setStyleSheet(OFFQ);
        }

        //pengecekan switch
        if(dataSwitch3 == "1"){
            // ui->switchInv3A->setStyleSheet(SWITCHCLOSE);
            ui->switchInv3A->setVisible(false);
            ui->SwitchCBDC3->setVisible(true);
            ui->CBDC3ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc3 == 1){
                dataout[28] = "ALARM Inverter DC 3 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter DC 3 Condition is ON !!!");
            dataAlarmInvDc3 = 0;
        }
        else if(dataSwitch3 == "0"){
            //ui->switchInv3A->setStyleSheet(SWITCHOPEN);
            ui->switchInv3A->setVisible(true);
            ui->SwitchCBDC3->setVisible(false);
            ui->CBDC3ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc3 == 0){
                dataout[28] = "ALARM Inverter DC 3 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter DC 3 Condition is ON !!!");
            dataAlarmInvDc3 = 1;
        }
        else if (dataSwitch3 == "5"){
            ui->CBDC2ALARM->setStyleSheet(OFFQ);
        }


        //memulai data alarm inv dc 4 dari ke 5 karena 4 sudah dipakai di inverter cb
        //pengecekan switch
        if(dataSwitch4 == "1"){
            // ui->switchInv4A->setStyleSheet(SWITCHCLOSE);
            ui->switchInv4A->setVisible(false);
            ui->SwitchCBDC4->setVisible(true);
            ui->CBDC4ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc4p == 1){
                dataout[28] = "ALARM Inverter DC 4 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter DC 4 Condition is ON !!!");
            dataAlarmInvDc4p = 0;
        }
        else if(dataSwitch4 == "0"){
            //ui->switchInv4A->setStyleSheet(SWITCHOPEN);
            ui->switchInv4A->setVisible(true);
            ui->SwitchCBDC4->setVisible(false);
            ui->CBDC4ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc4p == 0){
                dataout[28] = "ALARM Inverter DC 4 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter DC 4 Condition is ON !!!");
            dataAlarmInvDc4p = 1;
        }
        else if (dataSwitch4 == "5"){
            ui->CBDC4ALARM->setStyleSheet(OFFQ);
        }

        if(dataSwitch5 == "1"){
            // ui->switchInv5A->setStyleSheet(SWITCHCLOSE);
            ui->switchInv5A->setVisible(false);
            ui->SwitchCBDC5->setVisible(true);
            ui->CBDC5ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc5 == 1){
                dataout[28] = "ALARM Inverter DC 5 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter DC 5 Condition is ON !!!");
            dataAlarmInvDc5 = 0;
        }
        else if(dataSwitch5 == "0"){
            //   ui->switchInv5A->setStyleSheet(SWITCHOPEN);
            ui->switchInv5A->setVisible(true);
            ui->SwitchCBDC5->setVisible(false);

            ui->CBDC5ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc5 == 0){
                dataout[28] = "ALARM Inverter DC 5 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter DC 5 Condition is ON !!!");
            dataAlarmInvDc5 = 1;
        }
        else if (dataSwitch5 == "5"){
            ui->CBDC5ALARM->setStyleSheet(OFFQ);
        }

        if(dataSwitch6 == "1"){
            // ui->switchInv6A->setStyleSheet(SWITCHCLOSE);
            ui->switchInv6A->setVisible(false);
            ui->SwitchCBDC6->setVisible(true);

            ui->CBDC6ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc6 == 1){
                dataout[28] = "ALARM Inverter DC 6 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter DC 6 Condition is ON !!!");
            dataAlarmInvDc6 = 0;

        }
        else if(dataSwitch6 == "0"){
            // ui->switchInv6A->setStyleSheet(SWITCHOPEN);
            ui->switchInv6A->setVisible(true);
            ui->SwitchCBDC6->setVisible(false);

            ui->CBDC6ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc6 == 0){
                dataout[28] = "ALARM Inverter DC 6 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter DC 6 Condition is ON !!!");
            dataAlarmInvDc6 = 1;
        }
        else if (dataSwitch6 == "5"){
            ui->CBDC6ALARM->setStyleSheet(OFFQ);
        }

        if(dataSwitch7 == "1"){
            //   ui->switchInv7->setStyleSheet(SWITCHCLOSE);
            ui->switchInv7B->setVisible(false);
            ui->SwitchCBAC1->setVisible(true);

            ui->CBAC1ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc7 == 1){
                dataout[28] = "ALARM Inverter AC 1 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter AC 1 Condition is ON !!!");
            dataAlarmInvDc7 = 0;
        }
        else if(dataSwitch7 == "0"){
            //ui->switchInv7->setStyleSheet(SWITCHOPEN);
            ui->switchInv7B->setVisible(true);
            ui->SwitchCBAC1->setVisible(false);

            ui->CBAC1ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc7 == 0){
                dataout[28] = "ALARM Inverter AC 1 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter AC 1 Condition is ON !!!");
            dataAlarmInvDc7 = 1;
        }
        else if (dataSwitch7 == "5"){
            ui->CBAC1ALARM->setStyleSheet(OFFQ);
        }

        if(dataSwitch8 == "1"){
            //  ui->switchInv8->setStyleSheet(SWITCHCLOSE);
            ui->switchInv8B->setVisible(false);
            ui->SwitchCBAC2->setVisible(true);

            ui->CBAC2ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc8 == 1){
                dataout[28] = "ALARM Inverter AC 2 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter AC 2 Condition is ON !!!");
            dataAlarmInvDc8 = 0;
        }
        else if(dataSwitch8 == "0"){
            // ui->switchInv8->setStyleSheet(SWITCHOPEN);
            ui->switchInv8B->setVisible(true);
            ui->SwitchCBAC2->setVisible(false);

            ui->CBAC2ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc8 == 0){
                dataout[28] = "ALARM Inverter AC 2 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter AC 2 Condition is ON !!!");
            dataAlarmInvDc8 = 1;
        }
        else if (dataSwitch8 == "5"){
            ui->CBAC2ALARM->setStyleSheet(OFFQ);
        }

        if(dataSwitch9 == "1"){
            //  ui->switchInv9->setStyleSheet(SWITCHCLOSE);
            ui->switchInv9B->setVisible(false);
            ui->SwitchCBAC3->setVisible(true);

            ui->CBAC3ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc9 == 1){
                dataout[28] = "ALARM Inverter AC 3 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter AC 3 Condition is ON !!!");
            dataAlarmInvDc9 = 0;
        }
        else if(dataSwitch9 == "0"){
            //  ui->switchInv9->setStyleSheet(SWITCHOPEN);
            ui->switchInv9B->setVisible(true);
            ui->SwitchCBAC3->setVisible(false);

            ui->CBAC3ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc9 == 0){
                dataout[28] = "ALARM Inverter AC 3 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter AC 3 Condition is ON !!!");
            dataAlarmInvDc9 = 1;
        }
        else if (dataSwitch9 == "5"){
            ui->CBAC3ALARM->setStyleSheet(OFFQ);
        }

        if(dataSwitch10 == "1"){
            // ui->switchInv10->setStyleSheet(SWITCHCLOSE);
            ui->switchInv10B->setVisible(false);
            ui->SwitchCBAC4->setVisible(true);

            ui->CBAC4ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc10 == 1){
                dataout[28] = "ALARM Inverter AC 4 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter AC 4 Condition is ON !!!");
            dataAlarmInvDc10 = 0;
        }
        else if(dataSwitch10 == "0"){
            //ui->switchInv10->setStyleSheet(SWITCHOPEN);
            ui->switchInv10B->setVisible(true);
            ui->SwitchCBAC4->setVisible(false);

            ui->CBAC4ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc10 == 0){
                dataout[28] = "ALARM Inverter AC 4 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter AC 4 Condition is ON !!!");
            dataAlarmInvDc10 = 1;
        }
        else if (dataSwitch10 == "5"){
            ui->CBAC4ALARM->setStyleSheet(OFFQ);
        }

        if(dataSwitch11 == "1"){
            //   ui->switchInv11->setStyleSheet(SWITCHCLOSE);
            ui->switchInv11B->setVisible(false);
            ui->SwitchCBAC5->setVisible(true);

            ui->CBAC5ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc11 == 1){
                dataout[28] = "ALARM Inverter AC 5 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter AC 5 Condition is ON !!!");
            dataAlarmInvDc11 = 0;
        }
        else if(dataSwitch11 == "0"){
            // ui->switchInv11->setStyleSheet(SWITCHOPEN);
            ui->switchInv11B->setVisible(true);
            ui->SwitchCBAC5->setVisible(false);

            ui->CBAC5ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc11 == 0){
                dataout[28] = "ALARM Inverter AC 5 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter AC 5 Condition is ON !!!");
            dataAlarmInvDc11 = 1;
        }
        else if (dataSwitch11 == "5"){
            ui->CBAC5ALARM->setStyleSheet(OFFQ);
        }

        if(dataSwitch12 == "1"){
            // ui->switchInv12->setStyleSheet(SWITCHCLOSE);
            ui->switchInv12B->setVisible(false);
            ui->SwitchCBAC6->setVisible(true);

            ui->CBAC6ALARM->setStyleSheet(ON);
            if(dataAlarmInvDc12 == 1){
                dataout[28] = "ALARM Inverter AC 6 Condition is Normal !!!";
                InsertDataControl_CB(15);
            }
            removeRowAlarmTable("ALARM Inverter AC 6 Condition is ON !!!");
            dataAlarmInvDc12 = 0;
        }
        else if(dataSwitch12 == "0"){
            //  ui->switchInv12->setStyleSheet(SWITCHOPEN);
            ui->switchInv12B->setVisible(true);
            ui->SwitchCBAC6->setVisible(false);

            ui->CBAC6ALARM->setStyleSheet(OFF);
            if(dataAlarmInvDc12 == 0){
                dataout[28] = "ALARM Inverter AC 6 Condition is ON !!!";
                InsertDataControl_CB(15);
            }
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM Inverter AC 6 Condition is ON !!!");
            dataAlarmInvDc12 = 1;
        }
        else if (dataSwitch12 == "5"){
            ui->CBAC6ALARM->setStyleSheet(OFFQ);
        }


        //==============================
        //FilterSwitch
        //==============================
        if (dataAlarmInvDc7==1||dataAlarmInvDc8==1||dataAlarmInvDc9==1||dataAlarmInvDc10==1||dataAlarmInvDc11==1||dataAlarmInvDc12==1)
        {
            ui->pbInverter->setEnabled(false);
            ui->pbInverterControl->setEnabled(false);
            ui->Lbl_ControlInverter->setStyleSheet("image: url(:/img/ControlInvalid.png)");
            ui->Lbl_OvControlInverter->setStyleSheet("image: url(:/img/ControlInvalid.png)");
            ui->ArrowInverter->setVisible(0);
            ui->ArrowInverter_Ov->setVisible(0);
            statuscb=0;
        }
        else
        {
            ui->pbInverter->setEnabled(true);
            ui->pbInverterControl->setEnabled(true);
            // ui->ArrowInverter->setVisible(1);
            // ui->ArrowInverter_Ov->setVisible(1);
            statuscb=1;
        }
        //==============================
        //Filter mode Inverter
        //==============================
        if (ui->backDayaMode->value()==1)
        {
            ui->LblBDIMode->setText("Automatic");
            ui->lOvAcBus_10->setText("Automatic");
        }
        else if (ui->backDayaMode->value()==2)
        {
            ui->LblBDIMode->setText("Semi Auto");
            ui->lOvAcBus_10->setText("Semi Auto");
        }
        else if (ui->backDayaMode->value()==3)
        {
            ui->LblBDIMode->setText("Manual");
            ui->lOvAcBus_10->setText("Manual");
        }

        //==============================
        //battery
        //==============================
        //==============================
        //data alarm battery off
        //==============================
        //data status svpbat normal
        if(dataSvpBat == "1"){
            removeRowAlarmTable("ALARM SPV BMS Condition is OFFLINE !!!");
            removeRowAlarmTable("ALARM SPV BMS Condition is NULL !!!");
            BATTERY->setIcon(ImageOn);
            ui->twStatusMetering->setItem(RowRTU, Column+2, BATTERY);
            ui->stsKomSPVBMS->setStyleSheet(ON);
            ui->LRBMS->setText("ONLINE");
            ui->RBMS->setStyleSheet(ON);
            if(dataAlarmBatterySPV != 1){
                dataout[28] = "ALARM SPV BMS Condition is Normal";
                InsertDataControl_CB(14);
            }
            dataAlarmBatterySPV = 1;
            if(dataKomunikasiRabbit == "0"){
                setNormalBattery();
                ui->stsKomRabbitBMS->setStyleSheet(ON);
                ui->RBBM->setStyleSheet(ON);
                ui->LRBBM->setText("ONLINE");
                if(dataKomunikasiRabbitBattery == 1){
                    dataout[28] = "Alarm Internal BMS Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                dataKomunikasiRabbitBattery = 0;
                removeRowAlarmTable("Alarm Internal BMS Condition is OFFLINE !!!");


            }else{
                addRowAlarmTable("ASDU 4","-","Server Protocol","Alarm Internal BMS Condition is OFFLINE !!!");
                if(dataKomunikasiRabbitBattery == 0){
                    dataout[28] = "Alarm Internal BMS Condition is OFFLINE !!!";
                    InsertDataControl_CB(14);
                }
                dataKomunikasiRabbitBattery = 1;
                ui->stsKomRabbitBMS->setStyleSheet(OFF);
                ui->RBBM->setStyleSheet(OFF);
                ui->LRBBM->setText("OFFLINE");
                setPinkBatteryPack();
            }

            if(dataAlarmBat == "1"){
                //pengecekan cb Inverter
                removeRowAlarmTable("ALARM BATTERY Condition is ON !!!");
                BATTERY_ALARM->setIcon(ImageOn);
                ui->twStatusMetering->setItem(RowAlarm1, Column+2, BATTERY_ALARM);
                ui->lblindiplts_2->setStyleSheet(ON);
                //invalid
                if(dataCBBattry == "0" || dataCBBattry == "3"){
                    ui->Lbl_ControlBattery->setStyleSheet(CBCONTROLINVALID);
                    ui->Lbl_ControlOvBattery2->setStyleSheet(CBCONTROLINVALID);
                    CBBATTERY->setIcon(ImageInvalid);
                    ui->twStatusMetering->setItem(RowCb, Column+2, CBBATTERY);
                    if(dataConnectionBATTERY != 2){
                        dataout[28] = "Status CB BATTERY Condition Is Invalid";
                        InsertDataControl_CB(14);
                    }
                    dataConnectionBATTERY = 2;
                    ui->ArrowBattery->setVisible(0);
                    ui->ArrowBatteryOv->setVisible(0);
                }
                //open
                else if(dataCBBattry == "1"){
                    ui->Lbl_ControlBattery->setStyleSheet(CBOPEN);
                    ui->Lbl_ControlOvBattery2->setStyleSheet(CBOPEN);
                    CBBATTERY->setIcon(ImageOpen);
                    ui->twStatusMetering->setItem(RowCb, Column+2, CBBATTERY);
                    if(dataConnectionBATTERY != 1){
                        dataout[28] = "Status CB BATTERY Condition is opened";
                        InsertDataControl_CB(14);
                    }
                    dataConnectionBATTERY = 1;
                    ui->ArrowBattery->setVisible(0);
                    ui->ArrowBatteryOv->setVisible(0);
                }
                //close
                else if(dataCBBattry == "2"){

                    ui->Lbl_ControlBattery->setStyleSheet(CBCLOSE);
                    ui->Lbl_ControlOvBattery2->setStyleSheet(CBCLOSE);
                    CBBATTERY->setIcon(ImageClose);
                    ui->twStatusMetering->setItem(RowCb, Column+2, CBBATTERY);
                    if(dataConnectionBATTERY != 0){
                        dataout[28] = "Status CB BATTERY Condition is closed";
                        InsertDataControl_CB(14);
                    }
                    dataConnectionBATTERY = 0;
                    ui->ArrowBattery->setVisible(1);
                    ui->ArrowBatteryOv->setVisible(1);
                }
                //tidak ada data
                else{
                    ui->Lbl_ControlBattery->setStyleSheet(CBCONTROLQUESTION);
                    ui->Lbl_ControlOvBattery2->setStyleSheet(CBCONTROLQUESTION);
                    dataConnectionBATTERY = 2;
                    ui->ArrowBattery->setVisible(0);
                    ui->ArrowBatteryOv->setVisible(0);
                }
            }
            //==============================
            //data alarm battery on
            //==============================
            else if(dataAlarmBat == "0"){
                addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM BATTERY Condition is ON !!!");
                BATTERY_ALARM->setIcon(ImageOff);
                ui->twStatusMetering->setItem(RowAlarm1, Column+2, BATTERY_ALARM);
                ui->lblindiplts_2->setStyleSheet(OFF);
                ui->Lbl_ControlBattery->setStyleSheet(CBCONTROLINVALID);
                ui->Lbl_ControlOvBattery2->setStyleSheet(CBCONTROLINVALID);
                dataConnectionBATTERY = 2;
                ui->ArrowBattery->setVisible(0);
                ui->ArrowBatteryOv->setVisible(0);
            }
            //database tidak ada
            else{
                ui->lblindiplts_2->setStyleSheet(OFFQ);
                ui->Lbl_ControlBattery->setStyleSheet(CBCONTROLQUESTION);
                ui->Lbl_ControlOvBattery2->setStyleSheet(CBCONTROLQUESTION);
                dataConnectionBATTERY = 2;
                ui->ArrowBattery->setVisible(0);
                ui->ArrowBatteryOv->setVisible(0);
            }


        }
        //data alarm spv merah
        else if(dataSvpBat == "0"){
            //battry rtu off
            BATTERY->setIcon(ImageOff);
            ui->twStatusMetering->setItem(RowRTU, Column+2, BATTERY);
            BATTERY_ALARM->setIcon(ImageDC);
            ui->twStatusMetering->setItem(RowAlarm1, Column+2, BATTERY_ALARM);

            ui->stsKomSPVBMS->setStyleSheet(OFF);
            ui->RBMS->setStyleSheet(OFF);
            ui->LRBMS->setText("OFFLINE");
            ui->stsKomRabbitBMS->setStyleSheet(OFF);
            ui->LRBBM->setText("OFFLINE");
            ui->RBBM->setStyleSheet(OFF);
            if(dataAlarmBatterySPV != 2){
                dataout[28] = "ALARM SPV BMS Condition is OFFLINE !!!";
                InsertDataControl_CB(14);
            }
            dataAlarmBatterySPV = 2;
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM SPV BMS Condition is OFFLINE !!!");
            setPinkBattery();
        }
        //data null
        else{
            ui->stsKomSPVBMS->setStyleSheet(OFFQ);
            ui->RBMS->setStyleSheet(OFFQ);
            ui->LRBMS->setText("OFFLINE");
            if(dataAlarmBatterySPV != 3){
                dataout[28] = "ALARM SPV BMS Condition is NULL !!!";
                InsertDataControl_CB(14);
            }
            dataAlarmBatterySPV = 3;
            addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM SPV BMS Condition is NULL !!!");
        }

    }
    //RTU offline
    else if(dataRTU == "0"){
        //change status metering
        //battery

        BATTERY->setIcon(ImageOff);
        ui->twStatusMetering->setItem(RowRTU, Column+2, BATTERY);
        BATTERY_ALARM->setIcon(ImageDC);
        ui->twStatusMetering->setItem(RowAlarm1, Column+2, BATTERY_ALARM);
        CBBATTERY->setIcon(ImageCBDC);
        ui->twStatusMetering->setItem(RowCb, Column+2, CBBATTERY);
        //battery nanti berdasarkan spv
        //rtuConditionInverter = 0;
        ui->lblindiinverter_RTU->setStyleSheet(OFF);
        if(dataAlarmInverterRtu == 0){
            dataout[28] = "ALARM RTU Inverter Condition is ON !!!";
            InsertDataControl_CB(15);
        }
        dataAlarmInverterRtu = 1;
        addRowAlarmTable("ASDU 4","-","Server Protocol","RTU INVERTER Condition is ON !!!");
        ui->lbRTU_INVERTER->setStyleSheet(OFF);
        ui->lblStatusRTUINVERTER->setText("OFFLINE");
        addRowAlarmTable("Not Available","-","Server Protocol","Status Network Inverter Condition Is Offline");
        if(NetInverter){
            dataout[28] = "Status Network INVERTER Condition Is Offline";
            InsertDataControl_CB(17);
        }
        NetInverter = false;
        INVERTER->setIcon(ImageOff);
        ui->twStatusMetering->setItem(RowRTU, Column, INVERTER);

        //ui->lblindiinverter->setStyleSheet(DC);
        //addRowAlarmTable("1","0","Server Protocol","ALARM INVERTER Condition is OFFLINE !!!");
        //INVERTER_ALARM->setIcon(ImageDC);
        //ui->twStatusMetering->setItem(RowAlarm1, Column, INVERTER_ALARM);

        CBINVERTER->setIcon(ImageCBDC);
        CBINVERTER->setSizeHint(QSize(100, 100));
        ui->twStatusMetering->setItem(RowCb, Column, CBINVERTER);
        dataConnectionBATTERY = 2;
        dataConnectionINVERTER = 2;
        ui->lblindiplts_2->setStyleSheet(DC);
        ui->stsKomSPVBMS->setStyleSheet(DC);
        ui->RBMS->setStyleSheet(DC);
        ui->LRBMS->setText("OFFLINE");
        Get_ColorWidget_INVERTER(true,0,"#FF69FF");

        //FAULT ALARM
        ui->faultVoltageSystemGlobal->setStyleSheet(DC);
        ui->faultCurrentSystemGlobal->setStyleSheet(DC);

        //komunikasi
        ui->StsKomSPVBDI1->setStyleSheet(DC);
        ui->StsKomSPVBDI2->setStyleSheet(DC);
        ui->StsKomSPVBDI3->setStyleSheet(DC);
        ui->StsKomSPVBDI4->setStyleSheet(DC);
        ui->StsKomSPVBDI5->setStyleSheet(DC);
        ui->StsKomSPVBDI6->setStyleSheet(DC);

        //NETWORK
        ui->RBDI1->setStyleSheet(DC);
        ui->RBDI2->setStyleSheet(DC);
        ui->RBDI3->setStyleSheet(DC);
        ui->RBDI4->setStyleSheet(DC);
        ui->RBDI5->setStyleSheet(DC);
        ui->RBDI6->setStyleSheet(DC);
        ui->LRBDI1->setText("OFFLINE");
        ui->LRBDI2->setText("OFFLINE");
        ui->LRBDI3->setText("OFFLINE");
        ui->LRBDI4->setText("OFFLINE");
        ui->LRBDI5->setText("OFFLINE");
        ui->LRBDI6->setText("OFFLINE");

        ui->CBDC1ALARM->setStyleSheet(DC);
        ui->CBDC2ALARM->setStyleSheet(DC);
        ui->CBDC3ALARM->setStyleSheet(DC);
        ui->CBDC4ALARM->setStyleSheet(DC);
        ui->CBDC5ALARM->setStyleSheet(DC);
        ui->CBDC6ALARM->setStyleSheet(DC);

        //alarm
        ui->CBALARMINVERTER->setStyleSheet(DC);
        ui->CBDCALARMINVERTER->setStyleSheet(DC);

        //alarm ac
        ui->CBAC1ALARM->setStyleSheet(DC);
        ui->CBAC2ALARM->setStyleSheet(DC);
        ui->CBAC3ALARM->setStyleSheet(DC);
        ui->CBAC4ALARM->setStyleSheet(DC);
        ui->CBAC5ALARM->setStyleSheet(DC);
        ui->CBAC6ALARM->setStyleSheet(DC);
        // default switch
        ui->switchInvOv->setVisible(true); // overview
        ui->SwitchOnOv->setVisible(false);

        ui->switchInv1A->setVisible(true);
        ui->SwitchCBDC1->setVisible(false);
        ui->switchInv2A->setVisible(true);
        ui->SwitchCBDC2->setVisible(false);
        ui->switchInv3A->setVisible(true);
        ui->SwitchCBDC3->setVisible(false);
        ui->switchInv4A->setVisible(true);
        ui->SwitchCBDC4->setVisible(false);
        ui->switchInv5A->setVisible(true);
        ui->SwitchCBDC5->setVisible(false);
        ui->switchInv6A->setVisible(true);
        ui->SwitchCBDC6->setVisible(false);

        ui->switchInv7B->setVisible(true);
        ui->SwitchCBAC1->setVisible(false);
        ui->switchInv8B->setVisible(true);
        ui->SwitchCBAC2->setVisible(false);
        ui->switchInv9B->setVisible(true);
        ui->SwitchCBAC3->setVisible(false);
        ui->switchInv10B->setVisible(true);
        ui->SwitchCBAC4->setVisible(false);
        ui->switchInv11B->setVisible(true);
        ui->SwitchCBAC5->setVisible(false);
        ui->switchInv12B->setVisible(true);
        ui->SwitchCBAC6->setVisible(false);

        ui->switchInvAlarm->setVisible(true);
        ui->SwitchMainCBDC->setVisible(false);


        //alarm ac ov
        ui->CBAC1ALARM_OV->setStyleSheet(DC);
        ui->CBAC2ALARM_OV->setStyleSheet(DC);
        ui->CBAC3ALARM_OV->setStyleSheet(DC);
        ui->CBAC4ALARM_OV->setStyleSheet(DC);
        ui->CBAC5ALARM_OV->setStyleSheet(DC);
        ui->CBAC6ALARM_OV->setStyleSheet(DC);
        setPinkBattery();

        ui->inputMMode->setEnabled(false);
        ui->inputMWatt->setEnabled(false);
        ui->inputMVar->setEnabled(false);
        ui->inputMGDWatt->setEnabled(false);
        ui->inputMKDW->setEnabled(false);
        ui->inputMGDFrekuensi->setEnabled(false);
        ui->inputMKDFrekuensi->setEnabled(false);
        ui->inputMPLTMHFreq->setEnabled(false);
        ui->inputMPLTMHVar->setEnabled(false);



        ui->leInvW->setEnabled(false);
        ui->leInvVa->setEnabled(false);
        ui->leInvM1->setEnabled(false);
        ui->leInvC1->setEnabled(false);
        ui->leInvM2->setEnabled(false);
        ui->leInvC2->setEnabled(false);

        ui->pbInvC2->setEnabled(false);
        ui->pbInvM2->setEnabled(false);
        ui->pbInvC1->setEnabled(false);
        ui->pbInvM1->setEnabled(false);
        ui->pbInvVar->setEnabled(false);
        ui->pbInvW->setEnabled(false);
        ui->pbMeteringInvW->setEnabled(false);
        ui->pbMeteringInvVar->setEnabled(false);
        ui->pbMeteringInvM1->setEnabled(false);
        ui->pbMeteringInvC1->setEnabled(false);
        ui->pbMeteringInvM2->setEnabled(false);
        ui->pbMeteringInvC2->setEnabled(false);
        manualInverter = true;
        ui->spModeInv->setEnabled(false);
        ui->pbInvMode->setEnabled(false);
        ui->spModeInv->setValue(0);
        ui->labelInvMode->setText("Manual");
    }
    //database tidak ada
    else{
        ui->lblindiinverter_RTU->setStyleSheet(OFFQ);
    }
    GetAllEventList(true,"INVERTER");
    GetAllEventList(true,"BATTERY");

}

void MainWindow::Get_StatusRTU_LOAD(QString Query_RTU, QString Query_ALARM,QString QueryCB,int Column)
{
    QString Load,varLoada,varLoadb,CBLoad;
    static int RowRTU=35;
    static int RowCb=36;
    static int RowAlarm1=37;
    static int RowAlarm2=38;
    QTableWidgetItem *LOAD = new QTableWidgetItem;
    QTableWidgetItem *LOAD2 = new QTableWidgetItem;
    QTableWidgetItem *LOAD3 = new QTableWidgetItem;
    QTableWidgetItem *CBLOAD = new QTableWidgetItem;
    QTableWidgetItem *loadA = new QTableWidgetItem;
    QTableWidgetItem *LoadB = new QTableWidgetItem;
    //HMI
    QString ON="image: url(:/img/ok.png)";
    QString OFF="image: url(:/img/nok.png)";
    QString DC="image: url(:/img/offline.png)";
    //CB
    QIcon ImageOpen(":/img/openCB.png");
    QIcon ImageClose(":/img/closeCB.png");
    QIcon ImageInvalid(":/img/invalidCB.png");
    QIcon ImageCBDC(":/img/offlineCB.png");
    //IMAGE CB
    QString CBCONTROLINVALID="image: url(:/img/ControlInvalid.png)";
    QString CBOPEN="image: url(:/img/ControlOpen.png)";
    QString CBCLOSE="image: url(:/img/ControlClose.png)";
    QString CBINVALID="image: url(:/img/ControlOffline.png)";
    //METERING
    QIcon ImageOn(":/img/ok.png");
    QIcon ImageOff(":/img/nok.png");
    QIcon ImageDC(":/img/offline.png");
    QSqlQuery qryrtu,qryalarm,qryload;

    qryrtu.prepare(Query_RTU);
    qryrtu.exec();
    if (qryrtu.next())
    {
        Load=qryrtu.value(3).toString();
        //when LOAD ON
        //==============================
        if (Load=="1")
        {
            //rtu hmi status
            ui->lblindiload_RTU->setStyleSheet(ON);
            removeRowAlarmTable("RTU LOAD Condition is ON !!!");
            removeRowAlarmTable("ALARM LOAD 2 Condition is OFFLINE !!!");
            removeRowAlarmTable("ALARM LOAD 1 Condition is OFFLINE !!!");
            ui->lbRTU_LOAD->setStyleSheet(ON);
            ui->lblStatusRTULOAD->setText("ONLINE");
            removeRowAlarmTable("Status Network Load Condition Is Offline");
            if(!NetLoad){
                dataout[28] = "Status Network LOAD Condition Is Normal";
                InsertDataControl_CB(17);
            }
            NetLoad = true;
            //status metering rtu
            LOAD->setIcon(ImageOn);
            ui->twStatusMetering->setItem(RowCb, Column, LOAD);
            //get dataimage load
            Get_ColorWidget_LOAD(true,1,"#FF69FF");
            //==============================
            //load alarm on
            //==============================

            qryalarm.prepare(Query_ALARM);
            qryalarm.exec();
            if (qryalarm.next())
            {
                varLoada=qryalarm.value(3).toString();
                varLoadb=qryalarm.value(4).toString();
                if (varLoada=="1")
                {

                    ui->lblindiload->setStyleSheet(ON);
                    removeRowAlarmTable("ALARM LOAD 1 Condition is ON !!!");
                    if(dataAlarmLoad1 == 1){
                        dataout[28] = "ALARM LOAD 1 Condition is Normal !!!";
                        InsertDataControl_CB(13);
                    }
                    dataAlarmLoad1 = 0;
                    loadA->setIcon(ImageOn);
                    ui->twStatusMetering->setItem(RowAlarm1, Column, loadA);
                    //  ui->pbLoadControl->setEnabled(1);
                    //  ui->pbLoad->setEnabled(1);
                }
                else if (varLoada=="0")
                {
                    //load off

                    ui->lblindiload->setStyleSheet(OFF);
                    addRowAlarmTable("ASDU 2","-","Server Protocol","ALARM LOAD 1 Condition is ON !!!");
                    if(dataAlarmLoad1 == 0){
                        dataout[28] = "ALARM LOAD 1 Condition is ON !!!";
                        InsertDataControl_CB(13);
                    }
                    dataAlarmLoad1 = 1;
                    loadA->setIcon(ImageOff);
                    LOAD2->setIcon(ImageInvalid);
                    LOAD2->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowRTU, Column, LOAD2);
                    ui->twStatusMetering->setItem(RowAlarm1, Column, loadA);
                    //                    ui->pbLoadControl->setEnabled(0);
                    //                    ui->pbLoad->setEnabled(0);
                    ui->Ov_ControlLoad->setStyleSheet(CBCONTROLINVALID);
                    ui->Lbl_ControlLoad->setStyleSheet(CBCONTROLINVALID);
                    ui->ArrowLoad->setVisible(0);
                    ui->OvArrowLoad->setVisible(0);
                    dataConnectionLOAD = 2;
                }

                if (varLoadb=="1")
                {

                    ui->lblindiload_2->setStyleSheet(ON);
                    removeRowAlarmTable("ALARM LOAD 2 Condition is ON !!!");
                    if(dataAlarmLoad2 == 1){
                        dataout[28] = "ALARM LOAD 2 Condition is Normal !!!";
                        InsertDataControl_CB(13);
                    }
                    dataAlarmLoad2 = 0;
                    LoadB->setIcon(ImageOn);
                    ui->twStatusMetering->setItem(RowAlarm2, Column, LoadB);
                    //  ui->pbLoadControl->setEnabled(1);
                    //  ui->pbLoad->setEnabled(1);

                }
                else if (varLoadb=="0")
                {
                    //load off

                    ui->lblindiload_2->setStyleSheet(OFF);
                    addRowAlarmTable("ASDU 2","-","Server Protocol","ALARM LOAD 2 Condition is ON !!!");
                    if(dataAlarmLoad2 == 0){
                        dataout[28] = "ALARM LOAD 2 Condition is ON !!!";
                        InsertDataControl_CB(13);
                    }
                    dataAlarmLoad2 = 1;

                    LoadB->setIcon(ImageOff);
                    ui->twStatusMetering->setItem(RowAlarm2, Column, LoadB);

                    LOAD3->setIcon(ImageInvalid);
                    LOAD3->setSizeHint(QSize(100, 100));
                    ui->twStatusMetering->setItem(RowRTU, Column, LOAD3);
                    //                    ui->pbLoadControl->s  etEnabled(0);
                    //                    ui->pbLoad->setEnabled(0);
                    ui->Ov_ControlLoad->setStyleSheet(CBCONTROLINVALID);
                    ui->Lbl_ControlLoad->setStyleSheet(CBCONTROLINVALID);
                    ui->ArrowLoad->setVisible(0);
                    ui->OvArrowLoad->setVisible(0);
                    dataConnectionLOAD = 2;
                }
                // validation control enable or disable
                if (varLoada!="1" || varLoadb!="1")
                {
                    ui->pbLoadControl->setEnabled(0);
                    ui->pbLoad->setEnabled(0);
                }
                else if (varLoada!="1" && varLoadb!="1")
                {
                    ui->pbLoadControl->setEnabled(1);
                    ui->pbLoad->setEnabled(1);
                }
            }


            qryload.prepare(QueryCB);
            qryload.exec();
            if (qryload.next())
            {
                QString no_,temp;
                // no_=qryload.value(0).toInt();
                CBLoad=qryload.value(3).toString();
                if (temp!=CBLoad)
                {
                    if (CBLoad=="1" && dataAlarmLoad1 == 0 && dataAlarmLoad2 == 0)
                    {
                        CBLOAD->setIcon(ImageOpen);
                        CBLOAD->setSizeHint(QSize(100, 100));
                        ui->twStatusMetering->setItem(RowRTU, Column, CBLOAD);
                        ui->Lbl_ControlLoad->setStyleSheet(CBOPEN);
                        ui->Ov_ControlLoad->setStyleSheet(CBOPEN);
                        if(dataConnectionLOAD != 1  ){
                            dataout[28] = "Status CB LOAD Condition is opened";
                            InsertDataControl_CB(13);
                        }
                        dataConnectionLOAD = 1;
                        ui->ArrowLoad->setVisible(0);
                        ui->OvArrowLoad->setVisible(0);
                    }
                    else if(CBLoad=="2" && dataAlarmLoad1 == 0 && dataAlarmLoad2 == 0)
                    {
                        CBLOAD->setIcon(ImageClose);
                        CBLOAD->setSizeHint(QSize(100, 100));
                        ui->twStatusMetering->setItem(RowRTU, Column, CBLOAD);
                        ui->Lbl_ControlLoad->setStyleSheet(CBCLOSE);
                        ui->Ov_ControlLoad->setStyleSheet(CBCLOSE);
                        if(dataConnectionLOAD != 0 && dataAlarmLoad1 == 0 && dataAlarmLoad2 == 0){
                            dataout[28] = "Status CB LOAD Condition is closed";
                            InsertDataControl_CB(13);
                        }
                        dataConnectionLOAD = 0;
                        ui->ArrowLoad->setVisible(1);
                        ui->OvArrowLoad->setVisible(1);
                    }
                    else if ((CBLoad=="3" || CBLoad=="0") && dataAlarmLoad1 == 0 && dataAlarmLoad2 == 0)
                    {
                        CBLOAD->setIcon(ImageInvalid);
                        CBLOAD->setSizeHint(QSize(100, 100));
                        ui->twStatusMetering->setItem(RowRTU, Column, CBLOAD);
                        ui->Lbl_ControlLoad->setStyleSheet(CBCONTROLINVALID);
                        ui->Ov_ControlLoad->setStyleSheet(CBCONTROLINVALID);
                        if(dataConnectionLOAD != 2 && dataAlarmLoad1 == 0 && dataAlarmLoad2 == 0){
                            dataout[28] = "Status CB LOAD Condition Is Invalid";
                            InsertDataControl_CB(13);
                        }
                        dataConnectionLOAD = 2;
                        ui->ArrowLoad->setVisible(0);
                        ui->OvArrowLoad->setVisible(0);
                    }
                }
                temp=CBLoad;
            }



        }
        //==============================
        //when LOAD OFF
        //==============================
        else if(Load=="0")
        {
            //network
            ui->lbRTU_LOAD->setStyleSheet(OFF);
            ui->lblStatusRTULOAD->setText("OFFLINE");
            addRowAlarmTable("Not Available","-","Server Protocol","Status Network Load Condition Is Offline");
            if(NetLoad){
                dataout[28] = "Status Network LOAD Condition Is Offline";
                InsertDataControl_CB(17);
            }
            NetLoad = false;
            //hmi
            ui->lblindiload_RTU->setStyleSheet(OFF);
            addRowAlarmTable("ASDU 2","-","Server Protocol","RTU LOAD Condition is ON !!!");
            LOAD->setIcon(ImageOff);
            ui->twStatusMetering->setItem(RowCb, Column, LOAD);

            ui->lblindiload->setStyleSheet(DC);
            addRowAlarmTable("ASDU 2","-","Server Protocol","ALARM LOAD 1 Condition is OFFLINE !!!");
            ui->lblindiload_2->setStyleSheet(DC);
            addRowAlarmTable("ASDU 2","-","Server Protocol","ALARM LOAD 2 Condition is OFFLINE !!!");

            LoadB->setIcon(ImageDC);
            loadA->setIcon(ImageDC);
            ui->twStatusMetering->setItem(RowAlarm1, Column, loadA);
            ui->twStatusMetering->setItem(RowAlarm2, Column, LoadB);
            // METERING CB
            CBLOAD->setIcon(ImageCBDC);
            CBLOAD->setSizeHint(QSize(100, 100));
            ui->twStatusMetering->setItem(RowRTU, Column, CBLOAD);
            //arrow
            ui->ArrowLoad->setVisible(0);
            ui->OvArrowLoad->setVisible(0);
            //imageload
            Get_ColorWidget_LOAD(true,0,"#FF69FF");
            //CONTROL
            ui->Ov_ControlLoad->setStyleSheet(CBINVALID);
            ui->Lbl_ControlLoad->setStyleSheet(CBINVALID);

            dataConnectionLOAD = 2;
        }

    }
}

void MainWindow::Get_ColorWidget_PLTS(int Activated, int Event,QString CodePaint)
{
    //CB HMI CONTROL
    QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    if (Activated!=false)
    {
        int col=1;
        if (Event==0)
        {
            GetAllEventList(true,"EventList");
            ui->Ov_ControlPlts->setStyleSheet(CBOFFLINE);
            ui->Lbl_ControlPlts->setStyleSheet(CBOFFLINE);

            dataConnectionPLTS = 2;


            ui->ArrowPlts->setVisible(0);
            ui->OvArrowPlts->setVisible(0);


            //control
            ui->pbPltsControl->setEnabled(0);
            ui->pbPltsmon->setEnabled(0);
            for (int a=1; a<24; a++)
            {
                ui->twStatusMetering->item(a,col)->setTextColor(QColor(CodePaint));
                ui->twStatusMetering->item(a,col)->setBackgroundColor(QColor("#FFFFFF"));//white
            }
            //set LCD Value
            ui->lnPltsMatahari->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsVT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsVS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsVR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->OutWInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnWInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnVInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnAInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

        }
        else if (Event==1)
        {
            ui->pbPltsControl->setEnabled(1);
            ui->pbPltsmon->setEnabled(1);
            for (int a=1; a<24; a++)
            {
                ui->twStatusMetering->item(a,col)->setBackgroundColor(QColor("#FFFF99"));//white yellow
                ui->twStatusMetering->item(a,col)->setTextColor(QColor("#000000"));//black color

            }
            //set LCD Value
            ui->lnPltsMatahari->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAR->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAS->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsVT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsVS->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsVR->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->OutWInverterT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->lnWInverterT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnVInverterT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnAInverterT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->OvPltsW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

        }
        else if (Event==2)
        {
            for (int a=1; a<24; a++)
            {
                ui->twStatusMetering->item(a,col)->setTextColor(QColor(CodePaint));
                ui->twStatusMetering->item(a,col)->setBackgroundColor(QColor("#FFFFFF"));//white
            }
            //set LCD Value
            ui->lnPltsMatahari->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsAT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsVT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsVS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->inpltsVR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

        }
    }
}


void MainWindow::Get_ColorWidget_PLTMH(int Activated, int Event,QString CodePaint)
{
    //IMAGE PLTMH
    int col=2;
    //CB HMI CONTROL
    QString CBINVALID="image: url(:/img/ControlOffline.png)";
    QString CBOFFLINE="image: url(:/img/InvalidCBCLOSE.png)";
    if (Activated!=false)
    {
        if (Event==0)
        {
            ui->Lbl_ControlPltmh->setStyleSheet(CBINVALID);
            ui->Ov_ControlPltmh->setStyleSheet(CBINVALID);

            ui->pbPltmhControl->setEnabled(0);
            ui->pbPltmh->setEnabled(0);

            for (int a=1; a<24; a++)
            {
                ui->twStatusMetering->item(a,col)->setTextColor(QColor(CodePaint));
                ui->twStatusMetering->item(a,col)->setBackgroundColor(QColor("#FFFFFF"));//white

            }
            ui->OvPltmhW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->OutPltmhW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->OutPltmhVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhAT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhAS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhAR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->lePltmhW->setEnabled(false);
            ui->lePltmhVar->setEnabled(false);
            ui->pbPltmhW->setEnabled(false);
            ui->pbPltmhVar->setEnabled(false);

        }
        else if (Event==1)
        {
            ui->pbPltmhControl->setEnabled(1);
            ui->pbPltmh->setEnabled(1);
            for (int a=1; a<24; a++)
            {

                ui->twStatusMetering->item(a,col)->setBackgroundColor(QColor("#66CCFF")); //biru langit
                ui->twStatusMetering->item(a,col)->setTextColor(QColor("#000000")); //black
            }
            ui->OvPltmhW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->OutPltmhW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->OutPltmhVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVS->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVR->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhAT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhAS->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhAR->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->lePltmhW->setEnabled(true);
            ui->lePltmhVar->setEnabled(true);
            ui->pbPltmhW->setEnabled(true);
            ui->pbPltmhVar->setEnabled(true);

        }
        else if (Event==2)
        {
            for (int a=1; a<24; a++)
            {
                ui->twStatusMetering->item(a,col)->setTextColor(QColor(CodePaint));
                ui->twStatusMetering->item(a,col)->setBackgroundColor(QColor("#FFFFFF"));//white
            }
            ui->OutPltmhW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->OutPltmhVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhAT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhAS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhAR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

        }
    }
}

void MainWindow::Get_ColorWidget_INVERTER(int Activated, int Event,QString CodePaint)
{
    //IMAGE INVERTER
    QString InvON ="image: url(:/img/MainInverterOn.png)";
    QString InvOFF="image: url(:/img/MainInverterOff.png)";
    QString BatON ="image: url(:/img/BatteryOn.png)";
    QString BatOFF="image: url(:/img/BatteryOff.png)";
    int col=3;
    QString Inv_OFF="image: url(:/img/inverteroff.png)";
    QString Inv_ON="image: url(:/img/inverteron.png)";
    //CB HMI CONTROL
    QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    if (Activated!=false)
    {
        if (Event==0)
        {

            ui->Lbl_OvControlInverter->setStyleSheet(CBOFFLINE);
            dataConnectionINVERTER = 2;

            ui->Lbl_ControlInverter->setStyleSheet(CBOFFLINE);

            ui->Lbl_ControlBattery->setStyleSheet(CBOFFLINE);
            ui->Lbl_ControlOvBattery2->setStyleSheet(CBOFFLINE);
            dataConnectionBATTERY = 2;

            ui->ArrowInverter->setVisible(0);
            ui->ArrowInverter_Ov->setVisible(0);

            ui->ArrowBattery->setVisible(0);
            ui->ArrowBatteryOv->setVisible(0);

            for (int a=1; a<24; a++)
            {
                // inverter
                ui->twStatusMetering->item(a,col)->setTextColor(QColor(CodePaint));
                ui->twStatusMetering->item(a,col)->setBackgroundColor(QColor("#FFFFFF"));//white
                //battery
                ui->twStatusMetering->item(a,5)->setTextColor(QColor(CodePaint));
                ui->twStatusMetering->item(a,5)->setBackgroundColor(QColor("#FFFFFF"));//white

            }
            //battery
            //ui->lnBatteryA->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->OutBatteryVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            //ui->OutBatteryW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVT_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVT_3->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            //inverter
            ui->f1Result->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->f2Result->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->DfDtResult->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->Generated->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvBusbarDC->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvRW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvRVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvSW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvSVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvTW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvTVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->lnInvRW_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvRVar_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvSW_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvSVar_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvTW_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvTVar_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->InvW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->InvVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->backDayaMode->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDayaAktif->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDataReactive->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDataGradian1->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDataConstanta1->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDataGradian2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDataKonstanta2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->lnPltsOutVar_6->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutVar_7->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutVar_8->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutVar_9->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");


            ui->lnInvBusbarAcHz->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvBusbarAcV->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->backDayaAktifOv->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDayaReactiveOv->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

        }

        else if (Event==1)
        {
            for (int a=1; a<24; a++)
            {
                //inverter

                ui->twStatusMetering->item(a,col)->setTextColor(QColor("#000000")); //black

                ui->twStatusMetering->item(a,col)->setBackgroundColor(QColor("#81AC00"));

                //battery

                ui->twStatusMetering->item(a,5)->setTextColor(QColor("#000000")); //black

                ui->twStatusMetering->item(a,5)->setBackgroundColor(QColor("#81AC00"));
            }

            // setpoint
            ui->spModeInv->setEnabled(true);
            ui->pbInvMode->setEnabled(true);
            ui->pbInvW->setEnabled(true);
            ui->pbInvVar->setEnabled(true);
            ui->pbInvM1->setEnabled(true);
            ui->pbInvC1->setEnabled(true);
            ui->pbInvM2->setEnabled(true);
            ui->pbInvC2->setEnabled(true);

            ui->leInvW->setEnabled(true);
            ui->leInvVa->setEnabled(true);
            ui->leInvM1->setEnabled(true);
            ui->leInvC1->setEnabled(true);
            ui->leInvM2->setEnabled(true);
            ui->leInvC2->setEnabled(true);

            //battery
            //ui->lnBatteryA->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->OutBatteryVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            //ui->OutBatteryW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVT_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltmhVT_3->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            //inverter
            ui->f1Result->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->f2Result->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->DfDtResult->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->Generated->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvBusbarDC->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->lnInvRW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvRVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvSW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvSVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvTW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvTVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->lnInvRW_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvRVar_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvSW_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvSVar_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvTW_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvTVar_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->InvW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->InvVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->lnInvBusbarAcHz->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnInvBusbarAcV->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDayaMode->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDayaAktif->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDataReactive->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDataGradian1->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDataConstanta1->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDataGradian2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDataKonstanta2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->backDayaAktifOv->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->backDayaReactiveOv->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->lnPltsOutVar_6->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutVar_7->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutVar_8->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnPltsOutVar_9->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");


        }
    }
}

void MainWindow::Get_ColorWidget_LOAD(int Activated, int Event,QString CodePaint)
{
    //IMAGE PLTMH
    QString IMAGEON="image: url(:/img/LOAD ON.png)";
    QString IMAGEOFF="image: url(:/img/LOAD OFF.png)";
    int col=4;

    //CB HMI CONTROL
    QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    if (Activated!=false)
    {
        if (Event==0)
        {
            //  ui->InLoad->setStyleSheet(IMAGEOFF);
            //(CBOFFLINE);
            //   ui->OvLoad->setStyleSheet(IMAGEOFF);
            ui->Ov_ControlLoad->setStyleSheet(CBOFFLINE);
            dataConnectionLOAD = 2;
            ui->pbLoadControl->setEnabled(0);
            ui->pbLoad->setEnabled(0);

            // int col=4;
            for (int a=1; a<24; a++)
            {
                ui->twStatusMetering->item(a,col)->setTextColor(QColor(CodePaint));
                ui->twStatusMetering->item(a,5)->setBackgroundColor(QColor("#FFFFFF"));
            }
            ui->lnLoadTAR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnLoadTAS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnLoadTAT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnLoadTVR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnLoadTVS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnLoadTVT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->OutLoadW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->OutLoadVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->OvLoadW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        }

        else if (Event==1)
        {
            // ui->InLoad->setStyleSheet(IMAGEON);
            // ui->OvLoad->setStyleSheet(IMAGEON);
            ui-> pbLoadControl->setEnabled(1);
            ui->pbLoad->setEnabled(1);
            //            for (int a=1; a<25; a++)
            //            {
            //                ui->twStatusMetering->item(a,col)->setBackgroundColor(QColor("#FFFFFF"));
            //            }
            for (int a=1; a<24; a++)
            {
                ui->twStatusMetering->item(a,col)->setBackgroundColor(QColor("#CCFF99"));
                ui->twStatusMetering->item(a,col)->setTextColor(QColor("#000000"));
            }
            ui->lnLoadTAR->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnLoadTAS->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnLoadTAT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnLoadTVR->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnLoadTVS->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->lnLoadTVT->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->OutLoadW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
            ui->OutLoadVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

            ui->OvLoadW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        }
    }
}

void MainWindow::getDataRadiasi(QString query)
{
    QSqlQuery qry;
    qry.prepare(query);
    qry.exec();
    float dataRadiasi;
    if (qry.next())
    {
        dataRadiasi = qry.value(0).toFloat();
        if(dataResetF){
            scalaMaxTableDown = dataRadiasi;
            scalaMinTableDown = dataRadiasi;
            dataResetF = false;
        }else{
            if(scalaMaxTableDown < dataRadiasi){
                scalaMaxTableDown = dataRadiasi;
            }\
            if(scalaMinTableDown > dataRadiasi){
                scalaMinTableDown = dataRadiasi;
            }
        }
        ui->lnPltsMatahari->display(dataRadiasi);
        ui->twStatusMetering->item(20,1)->setText(QString::number(dataRadiasi));
        ui->PlotIndexTop->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));
        //double data3 = ui->sb1->value(), data4 = ui->sb2->value();
        //realtime trending
        double time(QTime::currentTime().msec()),now = QDateTime::currentDateTime().toTime_t();

        double key = now+(time/1000);
        //static double lastPointKey = 0;
        //if (key-lastPointKey > 0.002)
        //{
        // add dataout to lines:
        ui->PlotIndexDown->graph(0)->addData(key,dataRadiasi);//pltsW
        //    lastPointKey = key;
        //            qDebug()<<QString::number(key,'g',10);
        //}
        // make key axis range scroll with the dataout (at a constant range size of 8):

        ui->PlotIndexDown->xAxis->setRange(key, 2400, Qt::AlignRight);
        ui->PlotIndexDown->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
        ui->PlotIndexDown->replot();
    }

}


void MainWindow::SetDataCurva(double x, double y)
{
    QVector<double> xI(360), yI(360); // initialize with entries 0..100
    for (int i=0; i<360; i++)
    {
        xI[i] =x+ qCos(i) * 2500;
        yI[i] =y+ qSin(i) * 2500;

    }
    // create graph and assign data to it:
    //ui-> wCurve->addGraph();
    ui-> wCurve->graph(0)->setData(xI, yI);
    ui-> wCurve->replot();
    double dataCurva = qSqrt((x*x)+(y*y));
    //QMessageBox::information(this,"",QString::number(dataCurva));


    if(dataCurva <= 45000){
        if(!dataCurveLast){
            //tidak error
            dataout[28] = "The Operation Point of BDI is Still Within The Capability Curve !!!";
            InsertDataControl_CB(18);
        }
        dataCurveLast = true;
    }else{
        if(dataCurveLast){
            //error
            dataout[28] = "The Operation Point of BDI Exceed The Capability Curve !!!";
            InsertDataControl_CB(18);
        }
        dataCurveLast = false;
    }
}

void MainWindow::setPinkAll()
{
    QString CBINVALID="image: url(:/img/ControlOffline.png)";
    QString ALARMPINK="image: url(:/img/offline.png)";
    QString CBRTU="image: url(:/img/nok.png)";

    ui->Ov_ControlPltmh->setStyleSheet(CBINVALID);
    ui->Ov_ControlPlts->setStyleSheet(CBINVALID);
    dataConnectionPLTS = 2;
    ui->Lbl_OvControlInverter->setStyleSheet(CBINVALID);//harus ganti lagi manual
    dataConnectionINVERTER = 2;
    ui->Lbl_ControlOvBattery2->setStyleSheet(CBINVALID);//harus ganti lagi manual
    dataConnectionBATTERY = 2;
    ui->Ov_ControlLoad->setStyleSheet(CBINVALID);
    dataConnectionLOAD = 2;
    dataConnectionPLTS1 = 2;
    dataConnectionPLTS2 = 2;
    dataConnectionPLTS3 = 2;
    ui->Lbl_ControlPlts->setStyleSheet(CBINVALID);
    ui->Lbl_ControlInv3->setStyleSheet(CBINVALID); //harus ganti lagi
    ui->Lbl_ControlInv2->setStyleSheet(CBINVALID); //harus ganti lagi
    ui->Lbl_ControlInv1->setStyleSheet(CBINVALID); //harus ganti lagi
    ui->Lbl_ControlPltmh->setStyleSheet(CBINVALID);
    ui->Lbl_ControlBattery->setStyleSheet(CBINVALID);
    // ui->Lbl_ControlInverter1->setStyleSheet(CBINVALID);
    dataControlInvAc1 = 2;
    ui->Lbl_ControlInverter2->setStyleSheet(CBINVALID);
    dataControlInvAc2 = 2;
    ui->Lbl_ControlInverter3->setStyleSheet(CBINVALID);
    dataControlInvAc3 = 2;
    ui->Lbl_ControlInverter->setStyleSheet(CBINVALID);
    ui->Lbl_ControlLoad->setStyleSheet(CBINVALID);
    ui->lbRTU_LOAD->setStyleSheet(CBRTU);
    ui->lblStatusRTULOAD->setText("OFFLINE");
    addRowAlarmTable("Not Available","-","Server Protocol","Status Network Load Condition Is Offline");
    NetLoad = false;
    ui->lbRTU_INVERTER->setStyleSheet(CBRTU);
    ui->lblStatusRTUINVERTER->setText("OFFLINE");
    addRowAlarmTable("Not Available","-","Server Protocol","Status Network Inverter Condition Is Offline");
    NetInverter = false;
    ui->lbRTU_PLTMH->setStyleSheet(CBRTU);
    ui->lblStatusRTUPLTMH->setText("OFFLINE");
    addRowAlarmTable("Not Available","-","Server Protocol","Status Network PLTMH Condition Is Offline");
    NetPltmh = false;
    ui->lbRTU_PLTS->setStyleSheet(CBRTU);
    ui->lblStatusRTUPLTS->setText("OFFLINE");
    addRowAlarmTable("Not Available","-","Server Protocol","Status Network PLTS Condition Is Offline");
    NetPlts = false;
    ui->lblindiswitch->setStyleSheet(CBRTU);
    addRowAlarmTable("Not Available","-","Server Protocol","Status Network SWITCH Condition Is Offline");
    NetSwitch = false;
    ui->label_246->setText("OFFLINE");
    ui->stsKomSPVBMS->setStyleSheet(CBRTU);
    ui->RBMS->setStyleSheet(CBRTU);
    ui->LRBMS->setText("OFFLINE");
    ui->faultVoltageSystemGlobal->setStyleSheet(CBRTU);
    ui->faultCurrentSystemGlobal->setStyleSheet(CBRTU);
    ui->CBAC1ALARM_OV->setStyleSheet(ALARMPINK);
    ui->CBAC2ALARM_OV->setStyleSheet(ALARMPINK);
    ui->CBAC3ALARM_OV->setStyleSheet(ALARMPINK);

    ui->CBAC1ALARM->setStyleSheet(ALARMPINK);
    ui->CBAC2ALARM->setStyleSheet(ALARMPINK);
    ui->CBAC3ALARM->setStyleSheet(ALARMPINK);
    ui->CBAC4ALARM->setStyleSheet(ALARMPINK);
    ui->CBAC5ALARM->setStyleSheet(ALARMPINK);
    ui->CBAC6ALARM->setStyleSheet(ALARMPINK);

    ui->CBDC1ALARM->setStyleSheet(ALARMPINK);
    ui->CBDC2ALARM->setStyleSheet(ALARMPINK);
    ui->CBDC3ALARM->setStyleSheet(ALARMPINK);
    ui->CBDC4ALARM->setStyleSheet(ALARMPINK);
    ui->CBDC5ALARM->setStyleSheet(ALARMPINK);
    ui->CBDC6ALARM->setStyleSheet(ALARMPINK);

    ui->StsKomSPVBDI1->setStyleSheet(ALARMPINK);
    ui->StsKomSPVBDI2->setStyleSheet(ALARMPINK);
    ui->StsKomSPVBDI3->setStyleSheet(ALARMPINK);
    ui->StsKomSPVBDI4->setStyleSheet(ALARMPINK);
    ui->StsKomSPVBDI5->setStyleSheet(ALARMPINK);
    ui->StsKomSPVBDI6->setStyleSheet(ALARMPINK);

    ui->RBDI1->setStyleSheet(ALARMPINK);
    ui->RBDI2->setStyleSheet(ALARMPINK);
    ui->RBDI3->setStyleSheet(ALARMPINK);
    ui->RBDI4->setStyleSheet(ALARMPINK);
    ui->RBDI5->setStyleSheet(ALARMPINK);
    ui->RBDI6->setStyleSheet(ALARMPINK);

    ui->LRBDI1->setText("OFFLINE");
    ui->LRBDI2->setText("OFFLINE");
    ui->LRBDI3->setText("OFFLINE");
    ui->LRBDI4->setText("OFFLINE");
    ui->LRBDI5->setText("OFFLINE");
    ui->LRBDI6->setText("OFFLINE");

    ui->stsKomRabbitBMS->setStyleSheet(ALARMPINK);
    //  ui->CBDC4ALARM->setStyleSheet(CBRTU);
    //    ui->stsKomRabbitBMS->setStyleSheet(CBRTU);
    ui->LRBBM->setText("OFFLINE");
    ui->RBBM->setStyleSheet(CBRTU);

    static int RowRTU=35;
    static int RowCb=36;
    static int RowAlarm1=37;
    static int RowAlarm2=38;
    int Column = 1;
    QIcon Imageoffline(":/img/offline.png");
    QTableWidgetItem *OFFLINEALARM = new QTableWidgetItem;
    OFFLINEALARM->setIcon(Imageoffline);

    QIcon ImageofflineCB(":/img/offlineCB.png");
    QTableWidgetItem *OFFLINECB = new QTableWidgetItem;
    OFFLINECB->setIcon(ImageofflineCB);

    QIcon ImageNOK(":/img/nok.png");
    QTableWidgetItem *NOK = new QTableWidgetItem;
    NOK->setIcon(ImageNOK);

    //plts
    ui->lblindiplts_RTU->setStyleSheet(CBRTU);
    addRowAlarmTable("ASDU 3","-","Server Protocol","RTU PLTS Condition is ON !!!");
    //alarm
    ui->lblindiplts->setStyleSheet(ALARMPINK);
    addRowAlarmTable("ASDU 3","-","Server Protocol","CB ALARM PLTS Condition is ON !!!");
    ui->twStatusMetering->setItem(RowRTU, Column, OFFLINECB);
    ui->twStatusMetering->setItem(RowCb, Column, NOK);
    ui->twStatusMetering->setItem(RowAlarm1, Column, OFFLINEALARM);

    for (int a=1; a<24; a++)
    {
        ui->twStatusMetering->item(a,Column)->setText("-");
        ui->twStatusMetering->item(a,Column)->setTextColor(QColor("#FF69FF"));
        ui->twStatusMetering->item(a,Column)->setBackgroundColor(QColor("#FFFFFF"));//white
    }
    //pltmh

    Column = 2;
    QIcon Imageoffline2(":/img/offline.png");
    QTableWidgetItem *OFFLINEALARM2 = new QTableWidgetItem;
    OFFLINEALARM2->setIcon(Imageoffline2);

    QIcon Imageoffline2A(":/img/offline.png");
    QTableWidgetItem *OFFLINEALARM2A = new QTableWidgetItem;
    OFFLINEALARM2A->setIcon(Imageoffline2A);

    QIcon ImageofflineCB2(":/img/offlineCB.png");
    QTableWidgetItem *OFFLINECB2 = new QTableWidgetItem;
    OFFLINECB2->setIcon(ImageofflineCB2);

    QIcon ImageNOK2(":/img/nok.png");
    QTableWidgetItem *NOK2 = new QTableWidgetItem;
    NOK2->setIcon(ImageNOK2);
    ui->lblindipltmh_RTU->setStyleSheet(CBRTU);
    addRowAlarmTable("ASDU 1","-","Server Protocol","RTU PLTMH Condition is ON !!!");
    //alarm
    ui->lblindipltmh->setStyleSheet(ALARMPINK);
    addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 1 Condition is ON !!!");
    ui->lblindipltmh_2->setStyleSheet(ALARMPINK);
    addRowAlarmTable("ASDU 1","-","Server Protocol","CB ALARM PLTMH 2 Condition is ON !!!");
    ui->twStatusMetering->setItem(RowRTU, Column, OFFLINECB2);
    ui->twStatusMetering->setItem(RowCb, Column, NOK2);
    ui->twStatusMetering->setItem(RowAlarm1, Column, OFFLINEALARM2);
    ui->twStatusMetering->setItem(RowAlarm2, Column, OFFLINEALARM2A);
    for (int a=1; a<24; a++)
    {
        ui->twStatusMetering->item(a,Column)->setText("-");
        ui->twStatusMetering->item(a,Column)->setTextColor(QColor("#FF69FF"));
        ui->twStatusMetering->item(a,Column)->setBackgroundColor(QColor("#FFFFFF"));//white
    }
    //inverter
    ui->lblindiinverter_RTU->setStyleSheet(CBRTU);
    dataConnectionINVERTER = 2;
    addRowAlarmTable("ASDU 4","-","Server Protocol","RTU INVERTER Condition is ON !!!");
    //alarm
    //ui->lblindiinverter->setStyleSheet(ALARMPINK);
    //addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM INVERTER Condition is ON !!!");
    Column = 3;

    QIcon Imageoffline3(":/img/offline.png");
    QTableWidgetItem *OFFLINEALARM3 = new QTableWidgetItem;
    OFFLINEALARM3->setIcon(Imageoffline3);

    QIcon ImageofflineCB3(":/img/offlineCB.png");
    QTableWidgetItem *OFFLINECB3 = new QTableWidgetItem;
    OFFLINECB3->setIcon(ImageofflineCB3);

    QIcon ImageNOK3(":/img/nok.png");
    QTableWidgetItem *NOK3 = new QTableWidgetItem;
    NOK3->setIcon(ImageNOK3);

    ui->twStatusMetering->setItem(RowRTU, Column, OFFLINECB3);
    ui->twStatusMetering->setItem(RowCb, Column, NOK3);
    //ui->twStatusMetering->setItem(RowAlarm1, Column, OFFLINEALARM3);
    for (int a=1; a<24; a++)
    {
        ui->twStatusMetering->item(a,Column)->setText("-");
        ui->twStatusMetering->item(a,Column)->setTextColor(QColor("#FF69FF"));
        ui->twStatusMetering->item(a,Column)->setBackgroundColor(QColor("#FFFFFF"));//white
    }
    //load
    Column = 4;

    QIcon Imageoffline4(":/img/offline.png");
    QTableWidgetItem *OFFLINEALARM4 = new QTableWidgetItem;
    OFFLINEALARM4->setIcon(Imageoffline4);

    QIcon Imageoffline4A(":/img/offline.png");
    QTableWidgetItem *OFFLINEALARM4A = new QTableWidgetItem;
    OFFLINEALARM4A->setIcon(Imageoffline4A);

    QIcon ImageofflineCB4(":/img/offlineCB.png");
    QTableWidgetItem *OFFLINECB4 = new QTableWidgetItem;
    OFFLINECB4->setIcon(ImageofflineCB4);

    QIcon ImageNOK4(":/img/nok.png");
    QTableWidgetItem *NOK4 = new QTableWidgetItem;
    NOK4->setIcon(ImageNOK4);
    ui->lblindiload_RTU->setStyleSheet(CBRTU);
    addRowAlarmTable("ASDU 2","-","Server Protocol","RTU LOAD Condition is ON !!!");
    //alarm
    ui->lblindiload->setStyleSheet(ALARMPINK);
    addRowAlarmTable("ASDU 2","-","Server Protocol","ALARM LOAD 1 Condition is ON !!!");
    ui->lblindiload_2->setStyleSheet(ALARMPINK);
    addRowAlarmTable("ASDU 2","-","Server Protocol","ALARM LOAD 2 Condition is ON !!!");

    ui->twStatusMetering->setItem(RowRTU, Column, OFFLINECB4);
    ui->twStatusMetering->setItem(RowCb, Column, NOK4);
    ui->twStatusMetering->setItem(RowAlarm1, Column, OFFLINEALARM4);
    ui->twStatusMetering->setItem(RowAlarm2, Column, OFFLINEALARM4A);
    for (int a=1; a<24; a++)
    {
        ui->twStatusMetering->item(a,Column)->setText("-");
        ui->twStatusMetering->item(a,Column)->setTextColor(QColor("#FF69FF"));
        ui->twStatusMetering->item(a,Column)->setBackgroundColor(QColor("#FFFFFF"));//white
    }
    //batre
    //alarm
    ui->lblindiplts_2->setStyleSheet(ALARMPINK);

    ui->pltsFault1->setStyleSheet(ALARMPINK);
    ui->pltsFault2->setStyleSheet(ALARMPINK);
    ui->pltsFault3->setStyleSheet(ALARMPINK);

    ui->StsGTI1->setStyleSheet(ALARMPINK);
    ui->StsGTI2->setStyleSheet(ALARMPINK);
    ui->StsGTI3->setStyleSheet(ALARMPINK);

    ui->RGTI1->setStyleSheet(ALARMPINK);
    ui->RGTI2->setStyleSheet(ALARMPINK);
    ui->RGTI3->setStyleSheet(ALARMPINK);
    ui->LRGTI1->setText("OFFLINE");
    ui->LRGTI2->setText("OFFLINE");
    ui->LRGTI3->setText("OFFLINE");

    ui->RBDI1->setStyleSheet(ALARMPINK);
    ui->RBDI2->setStyleSheet(ALARMPINK);
    ui->RBDI3->setStyleSheet(ALARMPINK);
    ui->RBDI4->setStyleSheet(ALARMPINK);
    ui->RBDI5->setStyleSheet(ALARMPINK);
    ui->RBDI6->setStyleSheet(ALARMPINK);

    ui->LRBDI1->setText("OFFLINE");
    ui->LRBDI2->setText("OFFLINE");
    ui->LRBDI3->setText("OFFLINE");
    ui->LRBDI4->setText("OFFLINE");
    ui->LRBDI5->setText("OFFLINE");
    ui->LRBDI6->setText("OFFLINE");

    ui->RBMS->setStyleSheet(ALARMPINK);
    ui->LRBMS->setText("OFFLINE");


    Column = 5;
    QIcon Imageoffline3B(":/img/offline.png");
    QTableWidgetItem *OFFLINEALARM3B = new QTableWidgetItem;
    OFFLINEALARM3->setIcon(Imageoffline3B);

    QIcon ImageofflineCB3B(":/img/offlineCB.png");
    QTableWidgetItem *OFFLINECB3B = new QTableWidgetItem;
    OFFLINECB3->setIcon(ImageofflineCB3B);

    QIcon ImageNOK3B(":/img/nok.png");
    QTableWidgetItem *NOK3B = new QTableWidgetItem;
    NOK3->setIcon(ImageNOK3B);

    ui->twStatusMetering->setItem(RowRTU, Column, OFFLINECB3B);
    ui->twStatusMetering->setItem(RowCb, Column, NOK3B);
    ui->twStatusMetering->setItem(RowAlarm1, Column, OFFLINEALARM3B);
    addRowAlarmTable("ASDU 4","-","Server Protocol","ALARM BATTERY Condition is ON !!!");

    //ui->twStatusMetering->setItem(RowRTU, Column, OFFLINECB);
    //ui->twStatusMetering->setItem(RowCb, Column, NOK);
    //ui->twStatusMetering->setItem(RowAlarm1, Column, OFFLINEALARM);
    for (int a=1; a<24; a++)
    {
        ui->twStatusMetering->item(a,Column)->setText("-");
        ui->twStatusMetering->item(a,Column)->setTextColor(QColor("#FF69FF"));
        ui->twStatusMetering->item(a,Column)->setBackgroundColor(QColor("#FFFFFF"));//white
    }

    //disable
    ui->OvArrowPlts->setVisible(0);
    ui->OvArrowPltmh->setVisible(0);
    ui->OvArrowLoad->setVisible(0);
    ui->ArrowInverter_Ov->setVisible(0); //harus ada lagi
    ui->ArrowBatteryOv->setVisible(0); //harus ada lagi
    ui->ArrowPlts->setVisible(0);
    ui->ArrowPlts3A->setVisible(0);
    ui->ArrowPlts2A->setVisible(0); //harus ganti lagi
    ui->ArrowPlts1A->setVisible(0); //harus ganti lagi
    ui->ArrowPLTMH->setVisible(0);
    ui->ArrowBattery->setVisible(0);
    //    ui->OvArrowInverter1->setVisible(0); //harus ganti lagi
    //    ui->OvArrowInverter2->setVisible(0); //harus ganti lagi
    //    ui->OvArrowInverter3->setVisible(0); //harus ganti lagi
    ui->ArrowInverter->setVisible(0); //haruis ganti lagi
    ui->ArrowLoad->setVisible(0);

    //set value 0

    ui->lnPltsOutVar_6->display(0);
    ui->lnPltsOutVar_7->display(0);
    ui->lnPltsOutVar_9->display(0);
    ui->lnPltsOutVar_8->display(0);
    ui->backDayaAktifOv->display(0);
    ui->backDayaReactiveOv->display(0);
    ui->lnPltsOutFreq->display(0);
    ui->lnPltsOutR->display(0);
    ui->OvPltsW->display(0);
    ui->OvPltmhW->display(0);
    ui->OvLoadW->display(0);
    ui->lnPltsOutW->display(0);
    ui->lnPltsOutVar->display(0);
    ui->inpltsAR->display(0);
    ui->inpltsAS->display(0);
    ui->inpltsAT->display(0);
    ui->inpltsVT->display(0);
    ui->inpltsVS->display(0);
    ui->inpltsVR->display(0);
    ui->lnPltsMatahari->display(0);
    ui->lnPltmhAT->display(0);
    ui->lnPltmhVT->display(0);
    ui->lnPltmhVS->display(0);
    ui->lnPltmhVR->display(0);
    ui->lnPltmhAS->display(0);
    ui->lnPltmhAR->display(0);
    ui->OutPltmhVar->display(0);
    ui->OutPltmhW->display(0);
    ui->lnPltmhVT_2->display(0);
    ui->lnPltmhVT_3->display(0);
    ui->OutBatteryVar->display(0);
    //ui->lnBatteryA->display(0);
    //ui->OutBatteryW->display(0);
    //ui->lnBatteryT->display(0);
    ui->DfDtResult->display(0);
    ui->Generated->display(0);
    ui->lnInvBusbarDC->display(0);
    ui->lnInvRW->display(0);
    ui->lnInvRVar->display(0);
    ui->lnInvSW->display(0);
    ui->lnInvSVar->display(0);
    ui->lnInvTW->display(0);
    ui->lnInvTVar->display(0);
    ui->InvW->display(0);
    ui->InvVar->display(0);

    ui->lnInvRW_2->display(0);
    ui->lnInvRVar_2->display(0);
    ui->lnInvSW_2->display(0);
    ui->lnInvSVar_2->display(0);
    ui->lnInvTW_2->display(0);
    ui->lnInvTVar_2->display(0);

    ui->backDayaMode->display(0);
    ui->backDayaAktif->display(0);
    ui->backDataReactive->display(0);

    ui->backDataGradian1->display(0);
    ui->backDataConstanta1->display(0);
    ui->backDataGradian2->display(0);
    ui->backDataKonstanta2->display(0);

    ui->lnInvBusbarAcHz->display(0);
    ui->lnInvBusbarAcV->display(0);
    ui->OutLoadW->display(0);
    ui->OutLoadVar->display(0);
    ui->lnLoadTAR->display(0);
    ui->lnLoadTAS->display(0);
    ui->lnLoadTAT->display(0);
    ui->lnLoadTVR->display(0);
    ui->lnLoadTVS->display(0);
    ui->lnLoadTVT->display(0);

    ui->lnWInverterR->display(0);
    ui->lnVInverterR->display(0);
    ui->lnAInverterR->display(0);
    ui->lnWInverterS->display(0);
    ui->lnVInverterS->display(0);
    ui->lnAInverterS->display(0);
    ui->lnWInverterT->display(0);
    ui->lnVInverterT->display(0);
    ui->lnAInverterT->display(0);
    ui->responseDODMIN->display(0);
    ui->responseDODMAX->display(0);
    ui->responseSOCMIN->display(0);
    ui->responseSOCMAX->display(0);
    ui->BCVoltage1->display(0);
    ui->BCVoltage2->display(0);
    ui->BCVoltage3->display(0);
    ui->BCVoltage4->display(0);
    ui->BCVoltage5->display(0);
    ui->BCVoltage6->display(0);
    ui->BCVoltage7->display(0);
    ui->BCTemperature1->display(0);
    ui->BCTemperature2->display(0);
    ui->BCTemperature3->display(0);
    ui->BCTemperature4->display(0);
    ui->BCTemperature5->display(0);
    ui->BCTemperature6->display(0);
    ui->BCTemperature7->display(0);
    ui->lnBatteryITot->display(0);
    ui->lnBatteryVTot->display(0);
    ui->InBatteryWatt->display(0);

    ui->OutWInverterR->display(0);
    ui->OutWInverterS->display(0);
    ui->OutWInverterT->display(0);
    ui->f1Result->display(0);
    ui->f2Result->display(0);

    ui->lnPltsOutVar_6->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltsOutVar_7->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltsOutVar_9->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltsOutVar_8->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->backDayaAktifOv->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->backDayaReactiveOv->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltsOutFreq->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltsOutR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OvPltsW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OvPltmhW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OvLoadW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltsOutW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltsOutVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->inpltsAR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->inpltsAS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->inpltsAT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->inpltsVT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->inpltsVS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->inpltsVR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltsMatahari->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltmhAT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltmhVT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltmhVS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltmhVR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltmhAS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltmhAR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OutPltmhVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OutPltmhW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltmhVT_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnPltmhVT_3->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OutBatteryVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    //ui->lnBatteryA->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    //ui->OutBatteryW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    //ui->lnBatteryT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->DfDtResult->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->Generated->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->f1Result->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->f2Result->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->lnInvBusbarDC->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvRW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvRVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvSW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvSVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvTW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvTVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->lnInvRW_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvRVar_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvSW_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvSVar_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvTW_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvTVar_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->InvW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->InvVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->lnInvBusbarAcHz->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnInvBusbarAcV->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OutLoadW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OutLoadVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnLoadTAR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnLoadTAS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnLoadTAT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnLoadTVR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnLoadTVS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnLoadTVT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage1->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage3->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage4->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage5->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage6->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage7->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature1->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature3->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature4->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature5->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature6->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature7->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->lnBatteryITot->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnBatteryVTot->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->InBatteryWatt->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->lnWInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnVInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnAInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnWInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnVInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnAInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnWInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnVInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnAInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OutWInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    //ui->OutVInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    //ui->OutAInverterR->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OutWInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    // ui->OutVInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    //ui->OutAInverterS->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OutWInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    //ui->OutVInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    //ui->OutAInverterT->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseDODMIN->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseDODMAX->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseSOCMIN->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseSOCMAX->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->backDayaMode->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->backDayaAktif->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->backDataReactive->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->backDataGradian1->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->backDataConstanta1->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->backDataGradian2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->backDataKonstanta2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    //alarm baterry
    ui->BCOV1->setStyleSheet(ALARMPINK);
    ui->BCOV2->setStyleSheet(ALARMPINK);
    ui->BCOV3->setStyleSheet(ALARMPINK);
    ui->BCOV4->setStyleSheet(ALARMPINK);
    ui->BCOV5->setStyleSheet(ALARMPINK);
    ui->BCOV6->setStyleSheet(ALARMPINK);
    ui->BCOV7->setStyleSheet(ALARMPINK);


    ui->BCOT1->setStyleSheet(ALARMPINK);
    ui->BCOT2->setStyleSheet(ALARMPINK);
    ui->BCOT3->setStyleSheet(ALARMPINK);
    ui->BCOT4->setStyleSheet(ALARMPINK);
    ui->BCOT5->setStyleSheet(ALARMPINK);
    ui->BCOT6->setStyleSheet(ALARMPINK);
    ui->BCOT7->setStyleSheet(ALARMPINK);
}



void MainWindow::removeRowAlarmTable(QString text)
{
    int rows = ui->twAlarmList->rowCount();

    for(int i = 0; i < rows; ++i)
    {
        if(ui->twAlarmList->item(i, 4)->text() == text)
        {
            //we have found our value so we can update 'i' row
            ui->twAlarmList->removeRow(i);
            break;
        }
    }
}

void MainWindow::addRowAlarmTable(QString ioa,QString value,QString op,QString info)
{
    int rows = ui->twAlarmList->rowCount();
    bool found = false;
    for(int i = 0; i < rows; ++i)
    {
        if(ui->twAlarmList->item(i, 4)->text() == info)
        {
            found = true;
            break;
        }
    }
    if(!found)
    {

        for(int i = 0; i < rows; ++i)
        {
            if(ui->twAlarmList->item(i, 4)->text() == "")
            {
                rows = i;
                found = true;
                break;
            }
        }
        QDateTime time = QDateTime::currentDateTime();
        QString date = time.toString("dd-MM-yyyy hh:mm:ss.zzz");
        //QString
        ui->twAlarmList->setItem(rows,0,new QTableWidgetItem(date));
        ui->twAlarmList->setItem(rows,1,new QTableWidgetItem(ioa));
        ui->twAlarmList->setItem(rows,2,new QTableWidgetItem(value));
        ui->twAlarmList->setItem(rows,3,new QTableWidgetItem(op));
        ui->twAlarmList->setItem(rows,4,new QTableWidgetItem(info));
    }

}

QString MainWindow::searchRowTable(int row, int column)
{
    return ui->twAlarmList->item(row,column)->text();
}

//==============================
//RETURN (0) STATUS GTI
//       (1) FAULT
//       (2) Wp
//       (3) Vp
//       (4) Ap
//       (5) Wb
//       (6) Wb
//       (7) Ab
//==============================
void MainWindow::getAllDataGTI(int i)
{
    DOUBLE dataAbs;
    QSqlQuery gti,fault,power,block;
    if(i == 1){
        gti.prepare("select value from in_cb_plts_spv1 order by id desc limit 1");
        fault.prepare("select value from in_cb_plts_fault1 order by id desc limit 1");
        power.prepare("select ActiveR from mon_active_plts order by id desc limit 1");
        block.prepare("select W,V,A from in_power_plts_block_r order by id desc limit 1");
    }
    else if(i == 2){
        gti.prepare("select value from in_cb_plts_spv2 order by id desc limit 1");
        fault.prepare("select value from in_cb_plts_fault2 order by id desc limit 1");
        power.prepare("select ActiveS from mon_active_plts order by id desc limit 1");
        block.prepare("select W,V,A from in_power_plts_block_s order by id desc limit 1");
    }else if(i == 3){
        gti.prepare("select value from in_cb_plts_spv3 order by id desc limit 1");
        fault.prepare("select value from in_cb_plts_fault3 order by id desc limit 1");
        power.prepare("select ActiveT from mon_active_plts order by id desc limit 1");
        block.prepare("select W,V,A from in_power_plts_block_t order by id desc limit 1");
    }
    gti.exec();
    fault.exec();
    power.exec();
    block.exec();
    if(gti.next()){
        dataGti[0] = gti.value(0).toString();
    }else{
        dataGti[0] = "5";
    }
    if(fault.next()){
        dataGti[1] = fault.value(0).toString();
    }else{
        dataGti[1] = "5";
    }
    if(power.next()){
        dataAbs = power.value(0).toDouble();
        dataAbs = std::abs(dataAbs);
        dataGti[2] = QString::number(dataAbs);
    }else{
        dataGti[2] = "";
    }
    if(block.next()){
        dataAbs = block.value(0).toDouble();
        dataAbs = std::abs(dataAbs);
        dataGti[3] =  QString::number(dataAbs);
        dataAbs = block.value(1).toDouble();
        dataAbs = std::abs(dataAbs);
        dataGti[4] = QString::number(dataAbs);
        dataAbs = block.value(2).toDouble();
        dataAbs = std::abs(dataAbs);
        dataGti[5] =  QString::number(dataAbs/1024);
    }else{
        dataGti[3] = "";
        dataGti[4] = "";
        dataGti[5] = "";
    }
}

void MainWindow::setKomunikasiBDI1(int i)
{
    //QTableWidgetItem *CBINVERTER = new QTableWidgetItem;
    QIcon ImageCBDC(":/img/offlineCB.png");
    QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    QString ALARMPINK="image: url(:/img/offline.png)";
    // 1 = normal
    // 0 = pink
    if(i == 0){
        //7segment
        ui->lnInvRW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvRVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

        //panah
        //ui->OvArrowInverter1->setVisible(0);

        //alarm
        ui->CBAC1ALARM_OV->setStyleSheet(ALARMPINK);
        ui->CBAC1ALARM->setStyleSheet(ALARMPINK);
        ui->StsKomSPVBDI1->setStyleSheet(ALARMPINK);
        ui->RBDI1->setStyleSheet(ALARMPINK);
        ui->LRBDI1->setText("OFFLINE");
        //control
        // ui->Lbl_ControlInverter->setStyleSheet(CBOFFLINE);
        dataControlInvAc1 = 2;


        //set control utama inverter
        //CBINVERTER->setIcon(ImageCBDC);
        //CBINVERTER->setSizeHint(QSize(100, 100));
        //ui->twStatusMetering->setItem(35, 3, CBINVERTER);
        //ui->Lbl_ControlInverter->setStyleSheet(CBOFFLINE);
        //ui->Lbl_OvControlInverter->setStyleSheet(CBOFFLINE);
        //dataConnectionINVERTER = 2;
        //ui->ArrowInverter->setVisible(0);
        //ui->ArrowInverter_Ov->setVisible(0);
    }else{
        ui->lnInvRW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvRVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    }
}

void MainWindow::setKomunikasiBDI2(int i)
{
    //QTableWidgetItem *CBINVERTER = new QTableWidgetItem;
    QIcon ImageCBDC(":/img/offlineCB.png");
    QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    QString ALARMPINK="image: url(:/img/offline.png)";
    // 1 = normal
    // 0 = pink
    if(i == 0){
        //7segment
        ui->lnInvSW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvSVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

        //panah
        //   ui->OvArrowInverter2->setVisible(0);

        //alarm
        ui->CBAC2ALARM_OV->setStyleSheet(ALARMPINK);
        ui->CBAC2ALARM->setStyleSheet(ALARMPINK);
        ui->StsKomSPVBDI2->setStyleSheet(ALARMPINK);
        ui->RBDI2->setStyleSheet(ALARMPINK);
        ui->LRBDI2->setText("OFFLINE");
        //control
        //  ui->Lbl_ControlInverter2->setStyleSheet(CBOFFLINE);
        dataControlInvAc2 = 2;
        //set control utama inverter
        //CBINVERTER->setIcon(ImageCBDC);
        //CBINVERTER->setSizeHint(QSize(100, 100));
        //ui->twStatusMetering->setItem(35, 3, CBINVERTER);
        //ui->Lbl_ControlInverter->setStyleSheet(CBOFFLINE);
        //ui->Lbl_OvControlInverter->setStyleSheet(CBOFFLINE);
        //dataConnectionINVERTER = 2;
        //ui->ArrowInverter->setVisible(0);
        //ui->ArrowInverter_Ov->setVisible(0);
    }else{
        ui->lnInvSW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvSVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    }
}

void MainWindow::setKomunikasiBDI3(int i)
{
    //QTableWidgetItem *CBINVERTER = new QTableWidgetItem;
    QIcon ImageCBDC(":/img/offlineCB.png");
    QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    QString ALARMPINK="image: url(:/img/offline.png)";
    // 1 = normal
    // 0 = pink
    if(i == 0){
        //7segment
        ui->lnInvTW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvTVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

        //panah
        //  ui->OvArrowInverter3->setVisible(0);

        //alarm
        ui->CBAC3ALARM_OV->setStyleSheet(ALARMPINK);
        ui->CBAC3ALARM->setStyleSheet(ALARMPINK);
        ui->StsKomSPVBDI3->setStyleSheet(ALARMPINK);
        ui->RBDI3->setStyleSheet(ALARMPINK);
        ui->LRBDI3->setText("OFFLINE");
        //control
        //   ui->Lbl_ControlInverter3->setStyleSheet(CBOFFLINE);
        dataControlInvAc3 = 2;

        //set control utama inverter
        //CBINVERTER->setIcon(ImageCBDC);
        //CBINVERTER->setSizeHint(QSize(100, 100));
        //ui->twStatusMetering->setItem(35, 3, CBINVERTER);
        //ui->Lbl_ControlInverter->setStyleSheet(CBOFFLINE);
        //ui->Lbl_OvControlInverter->setStyleSheet(CBOFFLINE);
        //dataConnectionINVERTER = 2;
        //ui->ArrowInverter->setVisible(0);
        //ui->ArrowInverter_Ov->setVisible(0);
    }else{
        ui->lnInvTW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvTVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    }
}
void MainWindow::setKomunikasiBDI4(int i)
{
    //QTableWidgetItem *CBINVERTER = new QTableWidgetItem;
    QIcon ImageCBDC(":/img/offlineCB.png");
    QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    QString ALARMPINK="image: url(:/img/offline.png)";
    // 1 = normal
    // 0 = pink
    if(i == 0){
        //7segment
        ui->lnInvRW_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvRVar_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

        //panah
        //ui->OvArrowInverter3->setVisible(0);

        //alarm
        ui->CBAC4ALARM_OV->setStyleSheet(ALARMPINK);
        ui->CBAC4ALARM->setStyleSheet(ALARMPINK);
        ui->StsKomSPVBDI4->setStyleSheet(ALARMPINK);
        //ui->RBDI4->setStyleSheet(ALARMPINK);
        //ui->LRBDI3->setText("OFFLINE");
        //control
        //ui->Lbl_ControlInverter4->setStyleSheet(CBOFFLINE);
        //dataControlInvAc5 = 2;

        //set control utama inverter
        //CBINVERTER->setIcon(ImageCBDC);
        //CBINVERTER->setSizeHint(QSize(100, 100));
        //ui->twStatusMetering->setItem(35, 3, CBINVERTER);
        //ui->Lbl_ControlInverter->setStyleSheet(CBOFFLINE);
        //ui->Lbl_OvControlInverter->setStyleSheet(CBOFFLINE);
        //dataConnectionINVERTER = 2;
        //ui->ArrowInverter->setVisible(0);
        //ui->ArrowInverter_Ov->setVisible(0);
    }else{
        ui->lnInvRW_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvRVar_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    }
}
void MainWindow::setKomunikasiBDI5(int i)
{
    //QTableWidgetItem *CBINVERTER = new QTableWidgetItem;
    QIcon ImageCBDC(":/img/offlineCB.png");
    QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    QString ALARMPINK="image: url(:/img/offline.png)";
    // 1 = normal
    // 0 = pink
    if(i == 0){
        //7segment
        ui->lnInvSW_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvSVar_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

        //panah
        //ui->OvArrowInverter3->setVisible(0);

        //alarm
        ui->CBAC5ALARM_OV->setStyleSheet(ALARMPINK);
        ui->CBAC5ALARM->setStyleSheet(ALARMPINK);
        ui->StsKomSPVBDI5->setStyleSheet(ALARMPINK);
        //ui->RBDI4->setStyleSheet(ALARMPINK);
        //ui->LRBDI3->setText("OFFLINE");
        //control
        //ui->Lbl_ControlInverter4->setStyleSheet(CBOFFLINE);
        //dataControlInvAc5 = 2;

        //set control utama inverter
        //CBINVERTER->setIcon(ImageCBDC);
        //CBINVERTER->setSizeHint(QSize(100, 100));
        //ui->twStatusMetering->setItem(35, 3, CBINVERTER);
        //ui->Lbl_ControlInverter->setStyleSheet(CBOFFLINE);
        //ui->Lbl_OvControlInverter->setStyleSheet(CBOFFLINE);
        //dataConnectionINVERTER = 2;
        //ui->ArrowInverter->setVisible(0);
        //ui->ArrowInverter_Ov->setVisible(0);
    }else{
        ui->lnInvSW_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvSVar_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    }
}
void MainWindow::setKomunikasiBDI6(int i)
{
    //QTableWidgetItem *CBINVERTER = new QTableWidgetItem;
    QIcon ImageCBDC(":/img/offlineCB.png");
    QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    QString ALARMPINK="image: url(:/img/offline.png)";
    // 1 = normal
    // 0 = pink
    if(i == 0){
        //7segment
        ui->lnInvTW_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvTVar_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

        //panah
        //ui->OvArrowInverter3->setVisible(0);

        //alarm
        ui->CBAC6ALARM_OV->setStyleSheet(ALARMPINK);
        ui->CBAC6ALARM->setStyleSheet(ALARMPINK);
        ui->StsKomSPVBDI6->setStyleSheet(ALARMPINK);
        //ui->RBDI4->setStyleSheet(ALARMPINK);
        //ui->LRBDI3->setText("OFFLINE");
        //control
        //ui->Lbl_ControlInverter4->setStyleSheet(CBOFFLINE);
        //dataControlInvAc5 = 2;

        //set control utama inverter
        //CBINVERTER->setIcon(ImageCBDC);
        //CBINVERTER->setSizeHint(QSize(100, 100));
        //ui->twStatusMetering->setItem(35, 3, CBINVERTER);
        //ui->Lbl_ControlInverter->setStyleSheet(CBOFFLINE);
        //ui->Lbl_OvControlInverter->setStyleSheet(CBOFFLINE);
        //dataConnectionINVERTER = 2;
        //ui->ArrowInverter->setVisible(0);
        //ui->ArrowInverter_Ov->setVisible(0);
    }else{
        ui->lnInvTW_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
        ui->lnInvTVar_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    }
}


void MainWindow::getDataInverter()
{

    QSqlQuery mtr1,mtr2,mtr3,mtr4,mtr5,fdbk1,fdbk2,fdbk3,fdbk4,fdbk5,fdbk6,fdbk7,freqHz;
    QString DATA2;

    mtr1.prepare("select InvW,InvVar from in_power_inverter order by id desc limit 1");
    mtr1.exec();
    if(mtr1.next()){
        //1 watt = 1
        DATA2 = mtr1.value(0).toString();
        ui->twStatusMetering->item(1, 3)->setText(DATA2);
        ui->InvW->display(DATA2);
        ui->OvInverterW->display(DATA2);
        //2 var = 5
        DATA2 = mtr1.value(1).toString();
        ui->twStatusMetering->item(5, 3)->setText(DATA2);
        ui->InvVar->display(DATA2);
        dataarrow = mtr1.value(0).toDouble(); //invW


    }
    mtr2.prepare("select ActiveR,ActiveS,ActiveT from mon_active_inverter order by id desc limit 1");
    mtr2.exec();
    if(mtr2.next()){
        //1 active r = 2
        DATA2 = mtr2.value(0).toString();
        ui->twStatusMetering->item(2, 3)->setText(DATA2);
        //2 active s = 3
        DATA2 = mtr2.value(1).toString();
        ui->twStatusMetering->item(3, 3)->setText(DATA2);
        //3 active t = 4
        DATA2 = mtr2.value(2).toString();
        ui->twStatusMetering->item(4, 3)->setText(DATA2);

    }
    mtr3.prepare("select ReactiveR,ReactiveS,ReactiveT from mon_reactive_inverter order by id desc limit 1");
    mtr3.exec();
    if(mtr3.next()){
        //1 reactive r = 6
        DATA2 = mtr3.value(0).toString();
        ui->twStatusMetering->item(6, 3)->setText(DATA2);
        //2 reactive s = 7
        DATA2 = mtr3.value(1).toString();
        ui->twStatusMetering->item(7, 3)->setText(DATA2);
        //3 reactive t = 8
        DATA2 = mtr3.value(2).toString();
        ui->twStatusMetering->item(8, 3)->setText(DATA2);
    }
    mtr4.prepare("select invAR,invAS,invAT,invVR,invVS,invVT from mon_inverter order by id desc limit 1");
    mtr4.exec();
    if(mtr4.next()){
        //1 invAR = 10
        DATA2 = mtr4.value(0).toString();
        ui->twStatusMetering->item(10, 3)->setText(DATA2);
        //2 invAS = 11
        DATA2 = mtr4.value(1).toString();
        ui->twStatusMetering->item(11, 3)->setText(DATA2);
        //3 invAT = 12
        DATA2 = mtr4.value(2).toString();
        ui->twStatusMetering->item(12, 3)->setText(DATA2);
        //4 invVR = 14
        DATA2 = mtr4.value(3).toString();
        ui->lnInvBusbarAcV->display(DATA2);
        ui->twStatusMetering->item(14, 3)->setText(DATA2);
        //5 invVS = 15
        DATA2 = mtr4.value(4).toString();
        ui->twStatusMetering->item(15, 3)->setText(DATA2);
        //6 invVT = 16
        DATA2 = mtr4.value(5).toString();
        ui->twStatusMetering->item(16, 3)->setText(DATA2);
    }
    mtr5.prepare("select CosPhiR,CosPhiS,CosPhiT from mon_cosphi_inverter order by id desc limit 1");
    mtr5.exec();
    if(mtr5.next()){
        //1 cosphiR = 17
        DATA2 = mtr5.value(0).toString();
        ui->twStatusMetering->item(17, 3)->setText(DATA2);
        //2 cosphiS = 18
        DATA2 = mtr5.value(1).toString();
        ui->twStatusMetering->item(18, 3)->setText(DATA2);
        //3 cosphiT = 19
        DATA2 = mtr5.value(2).toString();
        ui->twStatusMetering->item(19, 3)->setText(DATA2);
    }
    fdbk1.prepare("select value from in_inverter_setpoint_mode order by id desc limit 1");
    fdbk1.exec();
    if(fdbk1.next()){
        //1 cosphiR = 17
        DATA2 = fdbk1.value(0).toString();
        ui->backDayaMode->display(DATA2);
    }
    fdbk2.prepare("select value from in_inverter_setpoint_w order by id desc limit 1");
    fdbk2.exec();
    if(fdbk2.next()){
        //1 cosphiR = 17
        DATA2 = fdbk2.value(0).toString();
        ui->backDayaAktif->display(DATA2);
        ui->backDayaAktifOv->display(DATA2);
    }
    fdbk3.prepare("select value from in_inverter_setpoint_var order by id desc limit 1");
    fdbk3.exec();
    if(fdbk3.next()){
        //1 cosphiR = 17
        DATA2 = fdbk3.value(0).toString();
        ui->backDataReactive->display(DATA2);
        ui->backDayaReactiveOv->display(DATA2);
    }
    fdbk4.prepare("select value from in_inverter_setpoint_m1 order by id desc limit 1");
    fdbk4.exec();
    if(fdbk4.next()){
        //1 cosphiR = 17
        DATA2 = fdbk4.value(0).toString();
        ui->backDataGradian1->display(DATA2);
    }
    fdbk5.prepare("select value from in_inverter_setpoint_c1 order by id desc limit 1");
    fdbk5.exec();
    if(fdbk5.next()){
        //1 cosphiR = 17
        DATA2 = fdbk5.value(0).toString();
        ui->backDataConstanta1->display(DATA2);
    }
    fdbk6.prepare("select value from in_inverter_setpoint_m2 order by id desc limit 1");
    fdbk6.exec();
    if(fdbk6.next()){
        //1 cosphiR = 17
        DATA2 = fdbk6.value(0).toString();
        ui->backDataGradian2->display(DATA2);
    }
    fdbk7.prepare("select value from in_inverter_setpoint_c2 order by id desc limit 1");
    fdbk7.exec();
    if(fdbk7.next()){
        //1 cosphiR = 17
        DATA2 = fdbk7.value(0).toString();
        ui->backDataKonstanta2->display(DATA2);
    }
    freqHz.prepare("select invfreq from mon_inverter_freq order by id desc limit 1");
    freqHz.exec();
    if(freqHz.next()){
        //1 cosphiR = 17
        DATA2 = freqHz.value(0).toString();
        ui->lnInvBusbarAcHz->display(DATA2);
    }

}
void MainWindow::view_curve()
{
    try {
        ui-> wCurve->xAxis->setRange(-75000, 75000);
        ui-> wCurve->yAxis->setRange(-75000, 75000);
        // generate some data:
        QVector<double> x(360), y(360); // initialize with entries 0..100
        for (int i=0; i<360; i++)
        {
            x[i] =i+ qCos(i) * 5000;
            y[i] =i+ qSin(i) * 5000;

        }
        // create graph and assign data to it:
        ui-> wCurve->addGraph();
        ui-> wCurve->graph(0)->setData(x, y);
        ui-> wCurve->graph(0)->setBrush(QBrush(QColor(0, 0, 255,0)));
        // give the axes some labels:
        //    ui-> wCurve->setBackground(Qt::transparent);

        ui->wCurve->plotLayout()->insertRow(0);
        ui->wCurve->plotLayout()->addElement(0, 0, new QCPTextElement(ui->wCurve, "Capability Curve", QFont("sans", 8, QFont::Bold)));

        ui-> wCurve->xAxis->setLabel("Data P");
        ui-> wCurve->yAxis->setLabel("Data Q");

        // set axes ranges, so we see all data:
        ui-> wCurve->replot();

    } catch (const std::exception& e)
    {
        QString txt;
        QDateTime date;
        txt.append(date.currentDateTime().toString()+" "+"| ViewCurve |"+e.what());
        QFile outFile("C://HMI//LogsError//Log.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << txt<< "\n";
        outFile.flush();
    }

}

void MainWindow::GetAllStatusBatre()
{
    QSqlQuery qry;
    DOUBLE dataBat,batteryMax;
    QString OK="image: url(:/img/ok.png)";
    QString NOK="image: url(:/img/nok.png)";
    QString KIRI="image: url(:/img/DirectNormal.png)";
    QString KANAN="image: url(:/img/right_direct_.png)";
    qry.prepare("select * from in_battery_bank");
    qry.exec();
    int i = 0;
    for(int i = 1 ; i < 24 ; i++){
        ui->twStatusMetering->item(i, 5)->setText("-");
    }
    while(qry.next())
    {
        if(i == 0){
            dataBat = qry.value(2).toDouble() /1000;
            ui->BCVoltage1->display(dataBat);
            batteryMax = dataBat = qry.value(3).toDouble();
            ui->BCTemperature1->display(dataBat);
            dataBat = qry.value(4).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS FAULT VOLTAGE 1 Condition is ON !!!");
                ui->BCOV1->setStyleSheet(OK);
                if(faultVoltage1 == 2){
                    dataout[28] = "STATUS FAULT VOLTAGE 1 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage1 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS FAULT VOLTAGE 1 Condition is ON !!!");
                ui->BCOV1->setStyleSheet(NOK);
                if(faultVoltage1 == 1){
                    dataout[28] = "STATUS FAULT VOLTAGE 1 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage1 = 2;
            }
            dataBat = qry.value(5).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS OVER TEMPERATURE 1 Condition is ON !!!");
                ui->BCOT1->setStyleSheet(OK);
                if(overTemp1 == 2){
                    dataout[28] = "STATUS OVER TEMPERATURE 1 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                overTemp1 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS OVER TEMPERATURE 1 Condition is ON !!!");
                ui->BCOT1->setStyleSheet(NOK);
                if(overTemp1 == 1){
                    dataout[28] = "STATUS OVER TEMPERATURE 1 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                overTemp1 = 2;
            }
        }
        else if(i == 1){
            dataBat = qry.value(2).toDouble() /1000;
            ui->BCVoltage2->display(dataBat);
            dataBat = qry.value(3).toDouble();
            //suTot = suTot + dataBat;
            batteryMax = std::max({dataBat,batteryMax});
            ui->BCTemperature2->display(dataBat);
            dataBat = qry.value(4).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS FAULT VOLTAGE 2 Condition is ON !!!");
                ui->BCOV2->setStyleSheet(OK);
                if(faultVoltage2 == 2){
                    dataout[28] = "STATUS FAULT VOLTAGE 2 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage2 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS FAULT VOLTAGE 2 Condition is ON !!!");
                ui->BCOV2->setStyleSheet(NOK);
                if(faultVoltage2 == 1){
                    dataout[28] = "STATUS FAULT VOLTAGE 2 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage2 = 2;
            }
            dataBat = qry.value(5).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS OVER TEMPERATURE 2 Condition is ON !!!");
                ui->BCOT2->setStyleSheet(OK);
                if(overTemp2 == 2){
                    dataout[28] = "STATUS OVER TEMPERATURE 2 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                overTemp2 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS OVER TEMPERATURE 2 Condition is ON !!!");
                ui->BCOT2->setStyleSheet(NOK);
                if(overTemp2 == 1){
                    dataout[28] = "STATUS OVER TEMPERATURE 2 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                overTemp2 = 2;
            }
        }
        else if(i == 2){
            dataBat = qry.value(2).toDouble() /1000;
            ui->BCVoltage3->display(dataBat);
            dataBat = qry.value(3).toDouble();
            batteryMax = std::max({dataBat,batteryMax});
            ui->BCTemperature3->display(dataBat);
            dataBat = qry.value(4).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS FAULT VOLTAGE 3 Condition is ON !!!");
                ui->BCOV3->setStyleSheet(OK);
                if(faultVoltage3 == 2){
                    dataout[28] = "STATUS FAULT VOLTAGE 3 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage3 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS FAULT VOLTAGE 3 Condition is ON !!!");
                ui->BCOV3->setStyleSheet(NOK);
                if(faultVoltage3 == 1){
                    dataout[28] = "STATUS FAULT VOLTAGE 3 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage3 = 2;
            }
            dataBat = qry.value(5).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS OVER TEMPERATURE 3 Condition is ON !!!");
                ui->BCOT3->setStyleSheet(OK);
                if(overTemp3 == 2){
                    dataout[28] = "STATUS OVER TEMPERATURE 3 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                overTemp3 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS OVER TEMPERATURE 3 Condition is ON !!!");
                ui->BCOT3->setStyleSheet(NOK);
                if(overTemp3 == 1){
                    dataout[28] = "STATUS OVER TEMPERATURE 3 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                overTemp3 = 2;
            }
        }
        else if(i == 3){
            dataBat = qry.value(2).toDouble() /1000;
            ui->BCVoltage4->display(dataBat);
            dataBat = qry.value(3).toDouble();
            batteryMax = std::max({dataBat,batteryMax});
            ui->BCTemperature4->display(dataBat);
            dataBat = qry.value(4).toDouble();
            if(dataBat == 1){
                ui->BCOV4->setStyleSheet(OK);
                removeRowAlarmTable("STATUS FAULT VOLTAGE 4 Condition is ON !!!");
                if(faultVoltage4 == 2){
                    dataout[28] = "STATUS FAULT VOLTAGE 4 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage4 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS FAULT VOLTAGE 4 Condition is ON !!!");
                ui->BCOV4->setStyleSheet(NOK);
                if(faultVoltage4 == 1){
                    dataout[28] = "STATUS FAULT VOLTAGE 4 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage4 = 2;
            }
            dataBat = qry.value(5).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS OVER TEMPERATURE 4 Condition is ON !!!");
                ui->BCOT4->setStyleSheet(OK);
                if(overTemp4 == 2){
                    dataout[28] = "STATUS OVER TEMPERATURE 4 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                overTemp4 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS OVER TEMPERATURE 4 Condition is ON !!!");
                ui->BCOT4->setStyleSheet(NOK);
                if(overTemp4 == 1){
                    dataout[28] = "STATUS OVER TEMPERATURE 4 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                overTemp4 = 2;
            }
        }
        else if(i == 4){
            dataBat = qry.value(2).toDouble() /1000;
            ui->BCVoltage5->display(dataBat);
            dataBat = qry.value(3).toDouble();
            batteryMax = std::max({dataBat,batteryMax});
            ui->BCTemperature5->display(dataBat);
            dataBat = qry.value(4).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS FAULT VOLTAGE 5 Condition is ON !!!");
                ui->BCOV5->setStyleSheet(OK);
                if(faultVoltage5 == 2){
                    dataout[28] = "STATUS FAULT VOLTAGE 5 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage5 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS FAULT VOLTAGE 5 Condition is ON !!!");
                ui->BCOV5->setStyleSheet(NOK);
                if(faultVoltage5 == 1){
                    dataout[28] = "STATUS FAULT VOLTAGE 5 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage5 = 2;
            }
            dataBat = qry.value(5).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS OVER TEMPERATURE 5 Condition is ON !!!");
                ui->BCOT5->setStyleSheet(OK);
                if(overTemp5 == 2){
                    dataout[28] = "STATUS OVER TEMPERATURE 5 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                overTemp5 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","1","Server Protocol","STATUS OVER TEMPERATURE 5 Condition is ON !!!");
                ui->BCOT5->setStyleSheet(NOK);
                if(overTemp5 == 1){
                    dataout[28] = "STATUS OVER TEMPERATURE 5 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                overTemp5 = 2;
            }
        }
        else if(i == 5){
            dataBat = qry.value(2).toDouble() /1000;
            ui->BCVoltage6->display(dataBat);
            dataBat = qry.value(3).toDouble();
            batteryMax = std::max({dataBat,batteryMax});
            ui->BCTemperature6->display(dataBat);
            dataBat = qry.value(4).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS FAULT VOLTAGE 6 Condition is ON !!!");
                ui->BCOV6->setStyleSheet(OK);
                if(faultVoltage6 == 2){
                    dataout[28] = "STATUS FAULT VOLTAGE 6 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage6 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS FAULT VOLTAGE 6 Condition is ON !!!");
                ui->BCOV6->setStyleSheet(NOK);
                if(faultVoltage6 == 1){
                    dataout[28] = "STATUS FAULT VOLTAGE 6 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage6 = 2;
            }
            dataBat = qry.value(5).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS OVER TEMPERATURE 6 Condition is ON !!!");
                ui->BCOT6->setStyleSheet(OK);
                if(overTemp6 == 2){
                    dataout[28] = "STATUS OVER TEMPERATURE 6 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                overTemp6 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS OVER TEMPERATURE 6 Condition is ON !!!");
                ui->BCOT6->setStyleSheet(NOK);
                if(overTemp6 == 1){
                    dataout[28] = "STATUS OVER TEMPERATURE 6 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                overTemp6 = 2;
            }
        }
        else if(i == 6){
            dataBat = qry.value(2).toDouble() /1000;
            ui->BCVoltage7->display(dataBat);
            dataBat = qry.value(3).toDouble();
            batteryMax = std::max({dataBat,batteryMax});
            ui->BCTemperature7->display(dataBat);
            dataBat = qry.value(4).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS FAULT VOLTAGE 7 Condition is ON !!!");
                ui->BCOV7->setStyleSheet(OK);
                if(faultVoltage7 == 2){
                    dataout[28] = "STATUS FAULT VOLTAGE 7 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage7 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS FAULT VOLTAGE 7 Condition is ON !!!");
                ui->BCOV7->setStyleSheet(NOK);
                if(faultVoltage7 == 1){
                    dataout[28] = "STATUS FAULT VOLTAGE 7 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                faultVoltage7 = 2;
            }
            dataBat = qry.value(5).toDouble();
            if(dataBat == 1){
                removeRowAlarmTable("STATUS OVER TEMPERATURE 7 Condition is ON !!!");
                ui->BCOT7->setStyleSheet(OK);
                if(overTemp7 == 2){
                    dataout[28] = "STATUS OVER TEMPERATURE 7 Condition is Normal !!!";
                    InsertDataControl_CB(14);
                }
                overTemp7 = 1;
            }else if(dataBat == 2){
                addRowAlarmTable("ASDU 4","-","Server Protocol","STATUS OVER TEMPERATURE 7 Condition is ON !!!");
                ui->BCOT7->setStyleSheet(NOK);
                if(overTemp7 == 1){
                    dataout[28] = "STATUS OVER TEMPERATURE 7 Condition is ON !!!";
                    InsertDataControl_CB(14);
                }
                overTemp7 = 2;
            }
        }
        i++;
    }
    GetAllEventList(true,"BATTERY");
    //SUHU RATA RATA

    ui->twStatusMetering->item(23, 5)->setText(QString::number(batteryMax));
    QSqlQuery qry2;
    qry2.prepare("select SOC,DOD,V_TOT,VHVDC,ITOT,FaultVSystem,FaultCurSystem from in_battery_status");
    qry2.exec();
    DOUBLE WATT,VHIT;
    i = 0;
    if(qry2.next()){
        //data SOC
        dataBat = qry2.value(0).toDouble();
        ui->lnPltmhVT_3->display(dataBat);
        ui->lnPltsOutVar_7->display(dataBat);

        ui->twStatusMetering->item(21, 5)->setText(QString::number(dataBat));
        //data DOD
        dataBat = qry2.value(1).toDouble();
        ui->lnPltmhVT_2->display(dataBat);
        ui->lnPltsOutVar_6->display(dataBat);

        ui->twStatusMetering->item(22, 5)->setText(QString::number(dataBat));
        //data V_TOT
        WATT = dataBat = qry2.value(2).toDouble() /100;
        ui->lnBatteryVTot->display(dataBat);
        ui->twStatusMetering->item(13, 5)->setText(QString::number(dataBat));
        //data ITOT
        VHIT = dataBat = qry2.value(4).toDouble() /100;
        //ui->lnBatteryA->display(dataBat);
        ui->lnBatteryITot->display(dataBat);
        if (dataBat>0)
        {
            ui->ArrowBattery->setStyleSheet("image: url(:/img/right_direct_.png)");
        }
        else if (dataBat<0)
        {
            ui->ArrowBattery->setStyleSheet("image: url(:/img/DirectNormal.png)");
        }

        ui->twStatusMetering->item(9, 5)->setText(QString::number(dataBat));
        //data Watt
        WATT = WATT * dataBat;
        ui->lnPltsOutVar_9->display(WATT);
        ui->InBatteryWatt->display(WATT);
        ui->twStatusMetering->item(1, 5)->setText(QString::number(WATT));
        if(WATT >= 0){

            ui->ArrowBattery->setStyleSheet(KIRI);
            ui->ArrowBatteryOv->setStyleSheet(KIRI);
        }else{
            ui->ArrowBattery->setStyleSheet(KANAN);
            ui->ArrowBatteryOv->setStyleSheet(KANAN);
        }
        //data VHVDC
        dataBat = qry2.value(3).toDouble() / 100;
        ui->OutBatteryVar->display(dataBat);
        ui->lnInvBusbarDC->display(dataBat);
        ui->lnPltsOutVar_8->display(dataBat);
        //vhvdc * itot
        VHIT = VHIT * dataBat;
        //ui->OutBatteryW->display(VHIT);

        //set faultVsystem
        dataBat = qry2.value(5).toDouble();
        //data normal
        if(dataBat == 1){
            ui->faultVoltageSystemGlobal->setStyleSheet(OK);
        }
        //data over
        else if(dataBat == 2){
            ui->faultVoltageSystemGlobal->setStyleSheet(NOK);
        }

        //set faultCurSystem
        dataBat = qry2.value(6).toDouble();
        if(dataBat == 1){
            ui->faultCurrentSystemGlobal->setStyleSheet(OK);
        }\
        //data over
        else if(dataBat == 2){
            ui->faultCurrentSystemGlobal->setStyleSheet(NOK);
        }
    }
    QSqlQuery qry3;
    qry3.prepare("select MinDod,MaxDod from in_battery_dod order by id desc");
    qry3.exec();
    if(qry3.next()){
        ui->responseDODMAX->display(qry3.value(1).toString());
        ui->responseDODMIN->display(qry3.value(0).toString());
    }
    QSqlQuery qry4;
    qry4.prepare("select MinSoc,MaxSoc from in_battery_soc order by id desc");
    qry4.exec();
    if(qry4.next()){
        ui->responseSOCMAX->display(qry4.value(1).toString());
        ui->responseSOCMIN->display(qry4.value(0).toString());
    }
}

void MainWindow::setPinkBattery()
{
    QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    QString ALARMPINK="image: url(:/img/offline.png)";

    ui->faultVoltageSystemGlobal->setStyleSheet(ALARMPINK);
    ui->faultVoltageSystemGlobal->update();
    ui->faultCurrentSystemGlobal->setStyleSheet(ALARMPINK);
    ui->faultCurrentSystemGlobal->update();
    ui->BCOV1->setStyleSheet(ALARMPINK);
    ui->BCOV2->setStyleSheet(ALARMPINK);
    ui->BCOV3->setStyleSheet(ALARMPINK);
    ui->BCOV4->setStyleSheet(ALARMPINK);
    ui->BCOV5->setStyleSheet(ALARMPINK);
    ui->BCOV6->setStyleSheet(ALARMPINK);
    ui->BCOV7->setStyleSheet(ALARMPINK);
    ui->BCOV1->update();
    ui->BCOV2->update();
    ui->BCOV3->update();
    ui->BCOV4->update();
    ui->BCOV5->update();
    ui->BCOV6->update();
    ui->BCOV7->update();

    ui->BCOT1->setStyleSheet(ALARMPINK);
    ui->BCOT2->setStyleSheet(ALARMPINK);
    ui->BCOT3->setStyleSheet(ALARMPINK);
    ui->BCOT4->setStyleSheet(ALARMPINK);
    ui->BCOT5->setStyleSheet(ALARMPINK);
    ui->BCOT6->setStyleSheet(ALARMPINK);
    ui->BCOT7->setStyleSheet(ALARMPINK);
    ui->BCOT1->update();
    ui->BCOT2->update();
    ui->BCOT3->update();
    ui->BCOT4->update();
    ui->BCOT5->update();
    ui->BCOT6->update();
    ui->BCOT7->update();

    //seven segmen
    //ui->OutBatteryW->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OutBatteryVar->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OutBatteryVar->display("0");

    ui->InBatteryWatt->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->InBatteryWatt->display("0");

    ui->lnBatteryVTot->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnBatteryITot->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseDODMIN->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseDODMAX->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseSOCMIN->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseSOCMAX->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage1->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage3->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage4->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage5->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage6->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage7->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->BCTemperature1->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature3->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature4->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature5->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature6->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature7->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    //dod
    ui->lnPltmhVT_2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    //soc
    ui->lnPltmhVT_3->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    //battery in overview
    // ui->lnPltsOutVar_6->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    // ui->lnPltsOutVar_7->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    // ui->lnPltsOutVar_9->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    //panah
    ui->lnBatteryVTot->display("0");
    ui->lnBatteryITot->display("0");
    ui->responseDODMIN->display("0");
    ui->responseDODMAX->display("0");
    ui->responseSOCMIN->display("0");
    ui->responseSOCMAX->display("0");
    ui->BCVoltage1->display("0");
    ui->BCVoltage2->display("0");
    ui->BCVoltage3->display("0");
    ui->BCVoltage4->display("0");
    ui->BCVoltage5->display("0");
    ui->BCVoltage6->display("0");
    ui->BCVoltage7->display("0");

    ui->BCTemperature1->display("0");
    ui->BCTemperature2->display("0");
    ui->BCTemperature3->display("0");
    ui->BCTemperature4->display("0");
    ui->BCTemperature5->display("0");
    ui->BCTemperature6->display("0");
    ui->BCTemperature7->display("0");

    //dod
    ui->lnPltmhVT_2->display("0");

    //soc
    ui->lnPltmhVT_3->display("0");
    //battery in overview
    ui->lnPltsOutVar_6->display("0");
    ui->lnPltsOutVar_7->display("0");
    ui->lnPltsOutVar_9->display("0");


    ui->ArrowBattery->setVisible(0);
    ui->ArrowBatteryOv->setVisible(0);

    //control
    ui->Lbl_ControlBattery->setStyleSheet(CBOFFLINE);
    ui->Lbl_ControlOvBattery2->setStyleSheet(CBOFFLINE);
    dataConnectionBATTERY= 2;

}
void MainWindow::setNormalBattery()
{
    //ui->OutBatteryW->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->OutBatteryVar->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->InBatteryWatt->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnBatteryVTot->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->lnBatteryITot->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->BCVoltage1->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage3->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage4->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage5->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage6->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage7->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->BCTemperature1->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature3->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature4->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature5->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature6->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature7->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    //dod
    ui->lnPltmhVT_2->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    //soc
    ui->lnPltmhVT_3->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseDODMIN->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseDODMAX->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseSOCMIN->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->responseSOCMAX->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    //battery overview
    ui->lnPltsOutVar_6->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->lnPltsOutVar_7->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    //   ui->lnPltsOutVar_9->setStyleSheet("color: rgb(0, 255, 0);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

}

void MainWindow::setPinkBatteryPack()
{
    QString CBOFFLINE="image: url(:/img/ControlOffline.png)";
    QString ALARMPINK="image: url(:/img/offline.png)";

    ui->BCOV1->setStyleSheet(ALARMPINK);
    ui->BCOV2->setStyleSheet(ALARMPINK);
    ui->BCOV3->setStyleSheet(ALARMPINK);
    ui->BCOV4->setStyleSheet(ALARMPINK);
    ui->BCOV5->setStyleSheet(ALARMPINK);
    ui->BCOV6->setStyleSheet(ALARMPINK);
    ui->BCOV7->setStyleSheet(ALARMPINK);
    ui->BCOV1->update();
    ui->BCOV2->update();
    ui->BCOV3->update();
    ui->BCOV4->update();
    ui->BCOV5->update();
    ui->BCOV6->update();
    ui->BCOV7->update();

    ui->BCOT1->setStyleSheet(ALARMPINK);
    ui->BCOT2->setStyleSheet(ALARMPINK);
    ui->BCOT3->setStyleSheet(ALARMPINK);
    ui->BCOT4->setStyleSheet(ALARMPINK);
    ui->BCOT5->setStyleSheet(ALARMPINK);
    ui->BCOT6->setStyleSheet(ALARMPINK);
    ui->BCOT7->setStyleSheet(ALARMPINK);
    ui->BCOT1->update();
    ui->BCOT2->update();
    ui->BCOT3->update();
    ui->BCOT4->update();
    ui->BCOT5->update();
    ui->BCOT6->update();
    ui->BCOT7->update();


    ui->BCVoltage1->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage3->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage4->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage5->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage6->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCVoltage7->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->BCTemperature1->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature2->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature3->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature4->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature5->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature6->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");
    ui->BCTemperature7->setStyleSheet("color: rgb(255, 105, 255);\nbackground-color: rgb(0, 0, 0);\nbackground-color: qlineargradient(spread:pad, x1:0.5, y1:0.340682, x2:0.5, y2:0, stop:0.323864 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");

    ui->BCVoltage1->display("0");
    ui->BCVoltage2->display("0");
    ui->BCVoltage3->display("0");
    ui->BCVoltage4->display("0");
    ui->BCVoltage5->display("0");
    ui->BCVoltage6->display("0");
    ui->BCVoltage7->display("0");

    ui->BCTemperature1->display("0");
    ui->BCTemperature2->display("0");
    ui->BCTemperature3->display("0");
    ui->BCTemperature4->display("0");
    ui->BCTemperature5->display("0");
    ui->BCTemperature6->display("0");
    ui->BCTemperature7->display("0");

}



void MainWindow::TrendingTop(QString BlueLine, QString RedLine, QString GreenLine,QString judul)
{
    QDateTime waktu[2];
    //"Trending Daya Aktif (PLTS, PLTMH, Bidirectional Inverter, Load)"
    //QMessageBox::information(this,"ok",QString::number(ui->PlotIndexTop->plotLayout()->elementCount()));

    ui->PlotIndexTop->plotLayout()->insertRow(0);
    ui->PlotIndexTop->plotLayout()->addElement(0, 0, new QCPTextElement(ui->PlotIndexTop, judul, QFont("sans", 8, QFont::Bold)));
    ui->PlotIndexTop->addGraph(); // blue line
    ui->PlotIndexTop->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->PlotIndexTop->graph(0)->setName(BlueLine);

    ui->PlotIndexTop->addGraph(); // red line
    ui->PlotIndexTop->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->PlotIndexTop->graph(1)->setName(RedLine);

    ui->PlotIndexTop->addGraph(); // green line
    ui->PlotIndexTop->graph(2)->setPen(QPen(QColor(46 ,255, 133)));
    ui->PlotIndexTop->graph(2)->setName(GreenLine);


    ui->PlotIndexTop->setStyleSheet("background:hsva(255,255,255,0%);");
    ui->PlotIndexTop->setBackground(QBrush(Qt::NoBrush));

    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
    timeTicker->setDateTimeFormat("hh:mm:ss");

    waktu[0] = QDateTime::fromString("2017-09-03 04:00:00","yyyy-MM-dd hh:mm:ss");
    waktu[1] = QDateTime::fromString("2017-09-03 05:00:00","yyyy-MM-dd hh:mm:ss");

    //change datetime time_t to time_t format


    ui->PlotIndexTop->xAxis->setTicker(timeTicker);
    //ui->PlotIndexMid->xAxis->setRange(now[0],now[1]);
    //ui->PlotIndexMid->xAxis->setRange();
    //ui->PlotIndexTop->xAxis->rangeChanged();
    ui->PlotIndexTop->axisRect()->setupFullAxesBox();
    ui->PlotIndexTop->yAxis->setRange(-10000, 10000);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->PlotIndexTop->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexTop->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->PlotIndexTop->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexTop->yAxis2, SLOT(setRange(QCPRange)));
    ui->PlotIndexTop->legend->setVisible(true);
    ui->PlotIndexTop->legend->setBrush(QColor(255, 255, 255, 150));
    ui->PlotIndexTop->replot();

}

void MainWindow::TrendingTopRunning(QString BlueLine, QString RedLine, QString GreenLine, QString YellowLine)
{
    ui->PlotIndexTop->addGraph(); // blue line
    ui->PlotIndexTop->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->PlotIndexTop->graph(0)->setName(BlueLine);

    ui->PlotIndexTop->addGraph(); // red line
    ui->PlotIndexTop->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->PlotIndexTop->graph(1)->setName(RedLine);

    ui->PlotIndexTop->addGraph(); // green line
    ui->PlotIndexTop->graph(2)->setPen(QPen(QColor(46 ,255, 133)));
    ui->PlotIndexTop->graph(2)->setName(GreenLine);
    ui->PlotIndexTop->replot();
}

void MainWindow::TrendingMid(QString BlueLine, QString RedLine, QString GreenLine, QString judul)
{
    QDateTime waktu[2];
    double now[2];
    //ui->PlotIndexMid->plotLayout()->clear();
    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
    timeTicker->setDateTimeFormat("hh:mm:ss");

    ui->PlotIndexMid->plotLayout()->insertRow(0);
    ui->PlotIndexMid->plotLayout()->addElement(0, 0, new QCPTextElement(ui->PlotIndexMid, judul, QFont("sans", 8, QFont::Bold)));
    ui->PlotIndexMid->addGraph(); // blue line
    ui->PlotIndexMid->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->PlotIndexMid->graph(0)->setName(BlueLine);
    ui->PlotIndexMid->addGraph(); // red line
    ui->PlotIndexMid->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->PlotIndexMid->graph(1)->setName(RedLine);
    ui->PlotIndexMid->addGraph(); // green line
    ui->PlotIndexMid->graph(2)->setPen(QPen(QColor(46 ,255, 133)));
    ui->PlotIndexMid->graph(2)->setName(GreenLine);

    waktu[0] = QDateTime::fromString("2017-09-03 04:00:00","yyyy-MM-dd hh:mm:ss");
    waktu[1] = QDateTime::fromString("2017-09-03 05:00:00","yyyy-MM-dd hh:mm:ss");

    //change datetime time_t to time_t format
    now[0] = waktu[0].toTime_t();
    now[1] = waktu[1].toTime_t();

    ui->PlotIndexMid->xAxis->setTicker(timeTicker);
    ui->PlotIndexMid->axisRect()->setupFullAxesBox();
    ui->PlotIndexMid->yAxis->setRange(-10000, 10000);
    ui->PlotIndexMid->xAxis->setRange(now[0],now[1]);
    connect(ui->PlotIndexMid->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexMid->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->PlotIndexMid->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexMid->yAxis2, SLOT(setRange(QCPRange)));
    ui->PlotIndexMid->legend->setVisible(true);
    ui->PlotIndexMid->legend->setBrush(QColor(255, 255, 255, 150));
    ui->PlotIndexMid->replot();
}

void MainWindow::TrendingMidRunning(QString BlueLine, QString RedLine, QString GreenLine, QString YellowLine)
{
    ui->PlotIndexMid->addGraph(); // blue line
    ui->PlotIndexMid->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->PlotIndexMid->graph(0)->setName(BlueLine);
    ui->PlotIndexMid->addGraph(); // red line
    ui->PlotIndexMid->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->PlotIndexMid->graph(1)->setName(RedLine);
    ui->PlotIndexMid->addGraph(); // green line
    ui->PlotIndexMid->graph(2)->setPen(QPen(QColor(46 ,255, 133)));
    ui->PlotIndexMid->graph(2)->setName(GreenLine);
    ui->PlotIndexMid->replot();
}

void MainWindow::TrendingDown(QString BlueLine)
{
    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
    timeTicker->setDateTimeFormat("hh:mm:ss");
    ui->PlotIndexDown->plotLayout()->insertRow(0);
    ui->PlotIndexDown->plotLayout()->addElement(0, 0, new QCPTextElement(ui->PlotIndexDown, "Trending Radiasi Matahari", QFont("sans", 8, QFont::Bold)));
    ui->PlotIndexDown->addGraph(); // blue line
    ui->PlotIndexDown->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->PlotIndexDown->graph(0)->setName(BlueLine);

    ui->PlotIndexDown->xAxis->setTicker(timeTicker);
    ui->PlotIndexDown->axisRect()->setupFullAxesBox();
    ui->PlotIndexDown->yAxis->setRange(-10000, 10000);




    connect(ui->PlotIndexDown->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexDown->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->PlotIndexDown->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexDown->yAxis2, SLOT(setRange(QCPRange)));
    ui->PlotIndexDown->legend->setVisible(true);
    ui->PlotIndexDown->legend->setBrush(QColor(255, 255, 255, 150));
    // ui->PlotIndexDown->replot();

}

void MainWindow::TrendingDownRunning(QString BlueLine)
{
    ui->PlotIndexDown->addGraph(); // blue line
    ui->PlotIndexDown->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->PlotIndexDown->graph(0)->setName(BlueLine);
}

void MainWindow::View_TrendingUI()
{
    TrendingTop("ActiveR","ActiveS","ActiveT","Trending Daya Aktif");
    TrendingMid("ReactiveR","ReactiveS","ReactiveT","Trending Daya Reaktif");
    TrendingDown("Radiasi");
}

void MainWindow::setDataTrending()
{
    float activeR,activeS,activeT,reactiveR,reactiveS,reactiveT;

    QSqlQuery qryActive,qryReactive;

    //selected PLTMH trending
    if(selectedTranding == 0){
        QString active = "select ActiveR,ActiveS,ActiveT from mon_active_pltmh order by id desc limit 1";
        QString reactive = "select ReactiveR,ReactiveS,ReactiveT from mon_reactive_pltmh order by id desc limit 1";

        qryActive.prepare(active);
        qryActive.exec();
        if (qryActive.next())
        {
            activeR = qryActive.value(0).toFloat();
            activeS = qryActive.value(1).toFloat();
            activeT = qryActive.value(2).toFloat();
        }
        qryReactive.prepare(reactive);
        qryReactive.exec();
        if (qryReactive.next())
        {
            reactiveR = qryReactive.value(0).toFloat();
            reactiveS = qryReactive.value(1).toFloat();
            reactiveT = qryReactive.value(2).toFloat();
        }
    }

    //selected PLTS trending
    else if(selectedTranding == 1){
        QString active = "select ActiveR,ActiveS,ActiveT from mon_active_plts order by id desc limit 1";
        QString reactive = "select ReactiveR,ReactiveS,ReactiveT from mon_reactive_plts order by id desc limit 1";

        qryActive.prepare(active);
        qryActive.exec();
        if (qryActive.next())
        {
            activeR = qryActive.value(0).toFloat();
            activeS = qryActive.value(1).toFloat();
            activeT = qryActive.value(2).toFloat();
        }
        qryReactive.prepare(reactive);
        qryReactive.exec();
        if (qryReactive.next())
        {
            reactiveR = qryReactive.value(0).toFloat();
            reactiveS = qryReactive.value(1).toFloat();
            reactiveT = qryReactive.value(2).toFloat();
        }
    }

    //selected Load trending
    else if(selectedTranding == 2){
        QString active = "select ActiveR,ActiveS,ActiveT from mon_active_load order by id desc limit 1";
        QString reactive = "select ReactiveR,ReactiveS,ReactiveT from mon_reactive_load order by id desc limit 1";

        qryActive.prepare(active);
        qryActive.exec();
        if (qryActive.next())
        {
            activeR = qryActive.value(0).toFloat();
            activeS = qryActive.value(1).toFloat();
            activeT = qryActive.value(2).toFloat();
        }
        qryReactive.prepare(reactive);
        qryReactive.exec();
        if (qryReactive.next())
        {
            reactiveR = qryReactive.value(0).toFloat();
            reactiveS = qryReactive.value(1).toFloat();
            reactiveT = qryReactive.value(2).toFloat();
        }
    }

    //selected Inverter trending
    else if(selectedTranding == 3){
        QString active = "select ActiveR,ActiveS,ActiveT from mon_active_inverter order by id desc limit 1";
        QString reactive = "select ReactiveR,ReactiveS,ReactiveT from mon_reactive_inverter order by id desc limit 1";

        qryActive.prepare(active);
        qryActive.exec();
        if (qryActive.next())
        {
            activeR = qryActive.value(0).toFloat();
            activeS = qryActive.value(1).toFloat();
            activeT = qryActive.value(2).toFloat();
        }
        qryReactive.prepare(reactive);
        qryReactive.exec();
        if (qryReactive.next())
        {
            reactiveR = qryReactive.value(0).toFloat();
            reactiveS = qryReactive.value(1).toFloat();
            reactiveT = qryReactive.value(2).toFloat();
        }
    }

    float smallestActive = std::numeric_limits<float>::max();
    smallestActive = std::min({activeR,activeS,activeT});

    float maxActive = std::numeric_limits<float>::max();
    maxActive = std::max({activeR,activeS,activeT});

    float smallestReactive = std::numeric_limits<float>::max();
    smallestReactive = std::min({reactiveR,reactiveS,reactiveT});

    float maxReactive = std::numeric_limits<float>::max();
    maxReactive = std::max({reactiveR,reactiveS,reactiveT});

    if(dataReset){
        scalaMaxTableUp = maxActive;
        scalaMinTableUp = smallestActive;
        scalaMaxTableMidle = maxReactive;
        scalaMinTableMidle = smallestReactive;

        dataReset = false;
    }else{


        if(scalaMaxTableUp < maxActive){
            scalaMaxTableUp = maxActive;
        }\
        if(scalaMinTableUp > smallestActive){
            scalaMinTableUp = smallestActive;
        }
        if(scalaMaxTableMidle < maxReactive){
            scalaMaxTableMidle = maxReactive;
        }\
        if(scalaMinTableMidle > smallestReactive){
            scalaMinTableMidle = smallestReactive;
        }
    }
    realtimeDataValue(activeR,activeS,activeT,reactiveR,reactiveS,reactiveT);
}

//void MainWindow::setTitleTranding(QString judul)
//{

//}

void MainWindow::clearDataGraphTranding()
{
    ui->PlotIndexMid->clearGraphs();
    ui->PlotIndexTop->clearGraphs();

    TrendingTopRunning("ActiveR","ActiveS","ActiveT","Trending Daya Aktif");
    TrendingMidRunning("ReactiveR","ReactiveS","ReactiveT","Trending Daya Reaktif");
}

void MainWindow::clearDataRadiasi()
{
    ui->PlotIndexDown->clearGraphs();
    TrendingDownRunning("Radiasi");
}

void MainWindow::setPlotPltmh()
{
    ui->PlotIndexTop->plotLayout()->insertRow(0);
    ui->PlotIndexTop->plotLayout()->addElement(0, 0, new QCPTextElement(ui->PlotIndexTop, "Trending Daya Aktif PLTS", QFont("sans", 8, QFont::Bold)));
    /*ui->PlotIndexTop->addGraph(); // blue line
    ui->PlotIndexTop->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->PlotIndexTop->graph(0)->setName("ActiveR");

    ui->PlotIndexTop->addGraph(); // red line
    ui->PlotIndexTop->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->PlotIndexTop->graph(1)->setName("ActiveS");

    ui->PlotIndexTop->addGraph(); // green line
    ui->PlotIndexTop->graph(2)->setPen(QPen(QColor(46 ,255, 133)));
    ui->PlotIndexTop->graph(2)->setName("ActiveT");


    ui->PlotIndexTop->setStyleSheet("background:hsva(255,255,255,0%);");
    ui->PlotIndexTop->setBackground(QBrush(Qt::NoBrush));

    QSharedPointer<QCPAxisTickerDateTime> timeTicker2(new QCPAxisTickerDateTime);
    timeTicker2->setDateTimeFormat("hh:mm:ss\ndd.MM.yy");
    ui->PlotIndexTop->xAxis->setTicker(timeTicker2);
    ui->PlotIndexTop->axisRect()->setupFullAxesBox();
    ui->PlotIndexTop->yAxis->setRange(-10, 10);

    // make left and bottom axes transfer their ranges to right and top axes:
    //connect(ui->PlotIndexTop->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexTop->xAxis2, SLOT(setRange(QCPRange)));
    //connect(ui->PlotIndexTop->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexTop->yAxis2, SLOT(setRange(QCPRange)));
    ui->PlotIndexTop->legend->setVisible(true);
    ui->PlotIndexTop->legend->setBrush(QColor(255, 255, 255, 150));
    */
    ui->PlotIndexTop->replot();
    /*
    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
    timeTicker->setDateTimeFormat("hh:mm:ss\ndd.MM.yy");
    */
    ui->PlotIndexMid->plotLayout()->insertRow(0);
    ui->PlotIndexMid->plotLayout()->addElement(0, 0, new QCPTextElement(ui->PlotIndexMid, "Trending Daya Reaktif PLTMH", QFont("sans", 8, QFont::Bold)));
    /*ui->PlotIndexMid->addGraph(); // blue line
    ui->PlotIndexMid->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->PlotIndexMid->graph(0)->setName("ReactiveR");
    ui->PlotIndexMid->addGraph(); // red line
    ui->PlotIndexMid->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->PlotIndexMid->graph(1)->setName("ReactiveS");
    ui->PlotIndexMid->addGraph(); // green line
    ui->PlotIndexMid->graph(2)->setPen(QPen(QColor(46 ,255, 133)));
    ui->PlotIndexMid->graph(2)->setName("ReactiveT");

    ui->PlotIndexMid->xAxis->setTicker(timeTicker);
    ui->PlotIndexMid->axisRect()->setupFullAxesBox();
    ui->PlotIndexMid->yAxis->setRange(-10, 10);

    //connect(ui->PlotIndexMid->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexMid->xAxis2, SLOT(setRange(QCPRange)));
    //connect(ui->PlotIndexMid->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexMid->yAxis2, SLOT(setRange(QCPRange)));
    ui->PlotIndexMid->legend->setVisible(true);
    ui->PlotIndexMid->legend->setBrush(QColor(255, 255, 255, 150));*/
    ui->PlotIndexMid->replot();


}

void MainWindow::setPlotPlts()
{
    ui->PlotIndexTop->plotLayout()->insertRow(0);
    ui->PlotIndexTop->plotLayout()->addElement(0, 0, new QCPTextElement(ui->PlotIndexTop, "Trending Daya Aktif PLTS", QFont("sans", 8, QFont::Bold)));
    /*ui->PlotIndexTop->addGraph(); // blue line
    ui->PlotIndexTop->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->PlotIndexTop->graph(0)->setName("ActiveR");

    ui->PlotIndexTop->addGraph(); // red line
    ui->PlotIndexTop->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->PlotIndexTop->graph(1)->setName("ActiveS");

    ui->PlotIndexTop->addGraph(); // green line
    ui->PlotIndexTop->graph(2)->setPen(QPen(QColor(46 ,255, 133)));
    ui->PlotIndexTop->graph(2)->setName("ActiveT");


    ui->PlotIndexTop->setStyleSheet("background:hsva(255,255,255,0%);");
    ui->PlotIndexTop->setBackground(QBrush(Qt::NoBrush));

    QSharedPointer<QCPAxisTickerDateTime> timeTicker2(new QCPAxisTickerDateTime);
    timeTicker2->setDateTimeFormat("hh:mm:ss\ndd.MM.yy");
    ui->PlotIndexTop->xAxis->setTicker(timeTicker2);
    ui->PlotIndexTop->axisRect()->setupFullAxesBox();
    ui->PlotIndexTop->yAxis->setRange(-10, 10);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->PlotIndexTop->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexTop->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->PlotIndexTop->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexTop->yAxis2, SLOT(setRange(QCPRange)));
    ui->PlotIndexTop->legend->setVisible(true);
    ui->PlotIndexTop->legend->setBrush(QColor(255, 255, 255, 150));*/
    ui->PlotIndexTop->replot();
    /*
    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
    timeTicker->setDateTimeFormat("hh:mm:ss\ndd.MM.yy");

    ui->PlotIndexMid->plotLayout()->insertRow(0);
    ui->PlotIndexMid->plotLayout()->addElement(0, 0, new QCPTextElement(ui->PlotIndexMid, "Trending Daya Reaktif PLTS", QFont("sans", 8, QFont::Bold)));
    ui->PlotIndexMid->addGraph(); // blue line
    ui->PlotIndexMid->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->PlotIndexMid->graph(0)->setName("ReactiveR");
    ui->PlotIndexMid->addGraph(); // red line
    ui->PlotIndexMid->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->PlotIndexMid->graph(1)->setName("ReactiveS");
    ui->PlotIndexMid->addGraph(); // green line
    ui->PlotIndexMid->graph(2)->setPen(QPen(QColor(46 ,255, 133)));
    ui->PlotIndexMid->graph(2)->setName("ReactiveT");

    ui->PlotIndexMid->xAxis->setTicker(timeTicker);
    ui->PlotIndexMid->axisRect()->setupFullAxesBox();
    ui->PlotIndexMid->yAxis->setRange(-10, 10);

    connect(ui->PlotIndexMid->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexMid->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->PlotIndexMid->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->PlotIndexMid->yAxis2, SLOT(setRange(QCPRange)));
    ui->PlotIndexMid->legend->setVisible(true);
    ui->PlotIndexMid->legend->setBrush(QColor(255, 255, 255, 150));*/
    ui->PlotIndexMid->replot();


}

void MainWindow::realtimeDataValue(float aktifA, float aktifB, float aktifC, float reaktifA, float reaktifB, float reaktifC)
{
    ui->PlotIndexTop->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));
    //double data3 = ui->sb1->value(), data4 = ui->sb2->value();
    //realtime trending
    double time(QTime::currentTime().msec()),now = QDateTime::currentDateTime().toTime_t();
    //QMessageBox::information(this,"Information CB",QDateTime::currentDateTime().toString());
    //ui->PlotIndexTop->yAxis->setRange(data3, data4*(-1));
    // calculate two new dataout points:
    //QMessageBox::information(this,"data",)
    double key = now+(time/1000);
    //QMessageBox::information(this,"Information CB",QString::number(key));
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002)
    {
        // add dataout to lines:
        ui->PlotIndexTop->graph(0)->addData(key, aktifA);//pltsW
        ui->PlotIndexTop->graph(1)->addData(key, aktifB);//pltmhW
        ui->PlotIndexTop->graph(2)->addData(key, aktifC);//ovInvW
        lastPointKey = key;

        //            qDebug()<<QString::number(key,'g',10);
    }
    // make key axis range scroll with the dataout (at a constant range size of 8):
    ui->PlotIndexTop->xAxis->setRange(key, 2400, Qt::AlignRight);
    for(int loop=0;loop<2;loop++){
        ui->PlotIndexTop->graph(loop)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    }
    ui->PlotIndexTop->replot();

    ui->PlotIndexMid->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));
    //double data3 = ui->sb1->value(), data4 = ui->sb2->value();
    //realtime trending
    //double time(QTime::currentTime().msec()),now = QDateTime::currentDateTime().toTime_t();
    //ui->PlotIndexMid->yAxis->setRange(data3, data4*(-1));
    // calculate two new dataout points:
    //double key = now+(time/1000);
    lastPointKey = 0;
    if (key-lastPointKey > 0.002)
    {

        // add dataout to lines:
        ui->PlotIndexMid->graph(0)->addData(key, reaktifA);//pltsW
        ui->PlotIndexMid->graph(1)->addData(key, reaktifB);//pltmhW
        ui->PlotIndexMid->graph(2)->addData(key, reaktifC);//ovInvW
        lastPointKey = key;

        //            qDebug()<<QString::number(key,'g',10);
    }
    // make key axis range scroll with the dataout (at a constant range size of 8):
    ui->PlotIndexMid->xAxis->setRange(key, 2400, Qt::AlignRight);
    for(int loop=0;loop<2;loop++){
        ui->PlotIndexMid->graph(loop)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    }
    ui->PlotIndexMid->replot();
}

void MainWindow::ReadAllRST(bool Activated)
{
    if(Activated!=false)
    {
        GetRST_PLTS("select * from mon_plts order by id desc limit 1",1);
        GetRST_PLTMH("select * from mon_pltmh order by id desc limit 1",2);
        //GetRST_INVERTER("select * from mon_inverter order by id desc limit 1",3);
        GetRST_LOAD("select * from mon_load order by id desc limit 1",4);
        //GetRST_BATTERY("select * from mon_battery order by id desc limit 1",5);
    }


}

void MainWindow::GetRST_PLTS(QString Query,int Column)
{
    QSqlQuery qry;
    QString pltsVR,
            pltsVS,
            pltsVT,
            pltsAR,
            pltsAS,
            pltsAT,pltsRad;
    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        pltsVR= qry.value(3).toString();
        pltsVS= qry.value(4).toString();
        pltsVT= qry.value(5).toString();
        pltsAR= qry.value(6).toString();
        pltsAS= qry.value(7).toString();
        pltsAT= qry.value(8).toString();

        //pltsRad= qry.value(9).toString();
        //set data from db
        // format row - column

        ui->twStatusMetering->item(10,Column)->setText(pltsAR);
        ui->twStatusMetering->item(11,Column)->setText(pltsAS);
        ui->twStatusMetering->item(12,Column)->setText(pltsAT);

        ui->twStatusMetering->item(14,Column)->setText(pltsVR);
        ui->twStatusMetering->item(15,Column)->setText(pltsVS);
        ui->twStatusMetering->item(16,Column)->setText(pltsVT);

        //ui->twStatusMetering->item(20,Column)->setText(pltsRad);

        double dataVR,dataVS,dataVT,dataAR,dataAS,dataAT;
        dataVR = pltsVR.toDouble();
        dataVS = pltsVS.toDouble();
        dataVT = pltsVT.toDouble();
        dataAR = pltsAR.toDouble();
        dataAS = pltsAS.toDouble();
        dataAT = pltsAT.toDouble();

        dataVR = std::abs(dataVR);
        dataVS = std::abs(dataVS);
        dataVT = std::abs(dataVT);
        dataAR = std::abs(dataAR);
        dataAS = std::abs(dataAS);
        dataAT = std::abs(dataAT);

        pltsAR = QString::number(dataAR);
        pltsAS = QString::number(dataAS);
        pltsAT = QString::number(dataAT);
        pltsVR = QString::number(dataVR);
        pltsVS = QString::number(dataVS);
        pltsVT = QString::number(dataVT);

        ui->inpltsAR->display(pltsAR);
        ui->inpltsAS->display(pltsAS);
        ui->inpltsAT->display(pltsAT);

        ui->inpltsVR->display(pltsVR);
        ui->inpltsVS->display(pltsVS);
        ui->inpltsVT->display(pltsVT);

        //ui->lnPltsMatahari->display(0);

        ui->lnPltsOutR->display(pltsVR);
    }


}

void MainWindow::GetRST_PLTMH(QString Query,int Column)
{
    QSqlQuery qry;
    QString pltmhVR,
            pltmhVS,
            pltmhVT,
            pltmhAR,
            pltmhAS,
            pltmhAT;

    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        pltmhVR= qry.value(3).toString();
        pltmhVS= qry.value(4).toString();
        pltmhVT= qry.value(5).toString();
        pltmhAR= qry.value(6).toString();
        pltmhAS= qry.value(7).toString();
        pltmhAT= qry.value(8).toString();
        //set data from db
        // format row - column

        ui->twStatusMetering->item(10,Column)->setText(pltmhAR);
        ui->twStatusMetering->item(11,Column)->setText(pltmhAS);
        ui->twStatusMetering->item(12,Column)->setText(pltmhAT);

        ui->twStatusMetering->item(14,Column)->setText(pltmhVR);
        ui->twStatusMetering->item(15,Column)->setText(pltmhVS);
        ui->twStatusMetering->item(16,Column)->setText(pltmhVT);
        // load LED INDICATOR HMI PLTMH
        double dataVR,dataVS,dataVT,dataAR,dataAS,dataAT;
        dataVR = pltmhVR.toDouble();
        dataVS = pltmhVS.toDouble();
        dataVT = pltmhVT.toDouble();
        dataAR = pltmhAR.toDouble();
        dataAS = pltmhAS.toDouble();
        dataAT = pltmhAT.toDouble();

        dataVR = std::abs(dataVR);
        dataVS = std::abs(dataVS);
        dataVT = std::abs(dataVT);
        dataAR = std::abs(dataAR);
        dataAS = std::abs(dataAS);
        dataAT = std::abs(dataAT);

        pltmhAR = QString::number(dataAR);
        pltmhAS = QString::number(dataAS);
        pltmhAT = QString::number(dataAT);
        pltmhVR = QString::number(dataVR);
        pltmhVS = QString::number(dataVS);
        pltmhVT = QString::number(dataVT);

        ui->lnPltmhAR->display(pltmhAR);
        ui->lnPltmhAS->display(pltmhAS);
        ui->lnPltmhAT->display(pltmhAT);

        ui->lnPltmhVR->display(pltmhVR);
        ui->lnPltmhVS->display(pltmhVS);
        ui->lnPltmhVT->display(pltmhVT);
    }
}

void MainWindow::GetRST_INVERTER(QString Query,int Column)
{
    QSqlQuery qry;
    QString invVR,
            invVS,
            invVT,
            invAR,
            invAS,
            invAT;

    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        invVR= qry.value(3).toString();
        invVS= qry.value(4).toString();
        invVT= qry.value(5).toString();
        invAR= qry.value(6).toString();
        invAS= qry.value(7).toString();
        invAT= qry.value(8).toString();
        //set data from db
        // format row - column

        ui->twStatusMetering->item(10,Column)->setText(invAR);
        ui->twStatusMetering->item(11,Column)->setText(invAS);
        ui->twStatusMetering->item(12,Column)->setText(invAT);

        ui->twStatusMetering->item(14,Column)->setText(invVR);
        ui->twStatusMetering->item(15,Column)->setText(invVS);
        ui->twStatusMetering->item(16,Column)->setText(invVT);
    }
}

void MainWindow::GetRST_LOAD(QString Query, int Column)
{
    QSqlQuery qry;
    QString loadVR,
            loadVS,
            loadVT,
            loadAR,
            loadAS,
            loadAT;

    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        loadVR= qry.value(3).toString();
        loadVS= qry.value(4).toString();
        loadVT= qry.value(5).toString();
        loadAR= qry.value(6).toString();
        loadAS= qry.value(7).toString();
        loadAT= qry.value(8).toString();
        //set data from db
        // format row - column table widget
        ui->twStatusMetering->item(10,Column)->setText (loadAR);
        ui->twStatusMetering->item(11,Column)->setText(loadAS);
        ui->twStatusMetering->item(12,Column)->setText(loadAT);

        ui->twStatusMetering->item(14,Column)->setText (loadVR);
        ui->twStatusMetering->item(15,Column)->setText(loadVS);
        ui->twStatusMetering->item(16,Column)->setText(loadVT);

        // load LED INDICATOR HMI LOAD
        double dataVR,dataVS,dataVT,dataAR,dataAS,dataAT;
        dataVR = loadVR.toDouble();
        dataVS = loadVS.toDouble();
        dataVT = loadVT.toDouble();
        dataAR = loadAR.toDouble();
        dataAS = loadAS.toDouble();
        dataAT = loadAT.toDouble();

        dataVR = std::abs(dataVR);
        dataVS = std::abs(dataVS);
        dataVT = std::abs(dataVT);
        dataAR = std::abs(dataAR);
        dataAS = std::abs(dataAS);
        dataAT = std::abs(dataAT);

        loadAR = QString::number(dataAR);
        loadAS = QString::number(dataAS);
        loadAT = QString::number(dataAT);
        loadVR = QString::number(dataVR);
        loadVS = QString::number(dataVS);
        loadVT = QString::number(dataVT);

        ui->lnLoadTVR->display(loadVR);
        ui->lnLoadTVS->display(loadVS);
        ui->lnLoadTVT->display(loadVT);

        ui->lnLoadTAR->display(loadAR);
        ui->lnLoadTAS->display(loadAS);
        ui->lnLoadTAT->display(loadAT);


    }

}

void MainWindow::GetRST_BATTERY(QString Query, int Column)
{
    QSqlQuery qry;
    QString BatteryVR,
            BatteryVS,
            BatteryVT,
            BatteryAR,
            BatteryAS,
            BatteryAT;

    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        BatteryVR= qry.value(3).toString();
        BatteryVS= qry.value(4).toString();
        BatteryVT= qry.value(5).toString();
        BatteryAR= qry.value(6).toString();
        BatteryAS= qry.value(7).toString();
        BatteryAT= qry.value(8).toString();
        //set data from db
        // format row - column table widget
        ui->twStatusMetering->item(10,Column)->setText(BatteryAR);
        ui->twStatusMetering->item(11,Column)->setText(BatteryAS);
        ui->twStatusMetering->item(12,Column)->setText(BatteryAT);

        ui->twStatusMetering->item(14,Column)->setText(BatteryVR);
        ui->twStatusMetering->item(15,Column)->setText(BatteryVS);
        ui->twStatusMetering->item(16,Column)->setText(BatteryVT);

        // load LED INDICATOR HMI LOAD
        //        ui->lnLoadTVR->display(loadVR);
        //        ui->lnLoadTVS->display(loadVS);
        //        ui->lnLoadTVT->display(loadVT);

        //        ui->lnLoadTAR->display(loadAR);
        //        ui->lnLoadTAS->display(loadAS);
        //        ui->lnLoadTAT->display(loadAT);


    }
}

void MainWindow::ReadAllRST_ActiveReactive(bool Activated)
{
    if (Activated!=false)
    {
        GetRST_PLTS_Active("select * from mon_active_plts order by id desc limit 1",1);
        GetRST_PLTMH_Active("select * from mon_active_pltmh order by id desc limit 1",2);
        //GetRST_INVERTER_Active("select * from mon_active_inverter order by id desc limit 1",3);
        GetRST_LOAD_Active("select * from mon_active_load order by id desc limit 1",4);
        GetRST_BATTERY_Active("select * from mon_active_battery order by id desc limit 1",5);

        GetRST_PLTS_ReActive("select * from mon_reactive_plts order by id desc limit 1",1);
        GetRST_PLTMH_ReActive("select * from mon_reactive_pltmh order by id desc limit 1",2);
        GetRST_INVERTER_ReActive("select * from mon_reactive_inverter order by id desc limit 1",3);
        GetRST_LOAD_ReActive("select * from mon_reactive_load order by id desc limit 1",4);
        //  GetRST_BATTERY_ReActive("select * from mon_reactive_battery order by id desc limit 1",5);
    }
}

void MainWindow::GetRST_PLTS_Active(QString Query, int Column)
{
    QSqlQuery qry;
    QString pltsAR,
            pltsAS,
            pltsAT;
    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        pltsAR= qry.value(3).toString();
        pltsAS= qry.value(4).toString();
        pltsAT= qry.value(5).toString();

        ui->twStatusMetering->item(2,Column)->setText(pltsAR);
        ui->twStatusMetering->item(3,Column)->setText(pltsAS);
        ui->twStatusMetering->item(4,Column)->setText(pltsAT);
    }
}

void MainWindow::GetRST_PLTMH_Active(QString Query, int Column)
{
    QSqlQuery qry;
    QString pltmhAR,
            pltmhAS,
            pltmhAT;
    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        pltmhAR= qry.value(3).toString();
        pltmhAS= qry.value(4).toString();
        pltmhAT= qry.value(5).toString();

        ui->twStatusMetering->item(2,Column)->setText(pltmhAR);
        ui->twStatusMetering->item(3,Column)->setText(pltmhAS);
        ui->twStatusMetering->item(4,Column)->setText(pltmhAT);
    }
}

void MainWindow::GetRST_INVERTER_Active(QString Query, int Column)
{
    QSqlQuery qry;
    QString inverterAR,
            inverterAS,
            inverterAT;
    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        inverterAR= qry.value(3).toString();
        inverterAS= qry.value(4).toString();
        inverterAT= qry.value(5).toString();

        ui->twStatusMetering->item(2,Column)->setText(inverterAR);
        ui->twStatusMetering->item(3,Column)->setText(inverterAS);
        ui->twStatusMetering->item(4,Column)->setText(inverterAT);
    }
}

void MainWindow::GetRST_LOAD_Active(QString Query, int Column)
{
    QSqlQuery qry;
    QString LoadAR,
            LoadAS,
            LoadAT;
    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        LoadAR= qry.value(3).toString();
        LoadAS= qry.value(4).toString();
        LoadAT= qry.value(5).toString();

        ui->twStatusMetering->item(2,Column)->setText(LoadAR);
        ui->twStatusMetering->item(3,Column)->setText(LoadAS);
        ui->twStatusMetering->item(4,Column)->setText(LoadAT);
    }
}

void MainWindow::GetRST_BATTERY_Active(QString Query, int Column)
{
    QSqlQuery qry;
    QString BatteryAR,
            BatteryAS,
            BatteryAT;
    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        BatteryAR= qry.value(3).toString();
        BatteryAS= qry.value(4).toString();
        BatteryAT= qry.value(5).toString();

        ui->twStatusMetering->item(2,Column)->setText(BatteryAR);
        ui->twStatusMetering->item(3,Column)->setText(BatteryAS);
        ui->twStatusMetering->item(4,Column)->setText(BatteryAT);
    }
}

void MainWindow::GetRST_PLTS_ReActive(QString Query, int Column)
{
    QSqlQuery qry;
    QString PltsRR,
            PltsRS,
            PltsRT;
    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        PltsRR= qry.value(3).toString();
        PltsRS= qry.value(4).toString();
        PltsRT= qry.value(5).toString();

        ui->twStatusMetering->item(6,Column)->setText(PltsRR);
        ui->twStatusMetering->item(7,Column)->setText(PltsRS);
        ui->twStatusMetering->item(8,Column)->setText(PltsRT);
    }
}

void MainWindow::GetRST_PLTMH_ReActive(QString Query, int Column)
{
    QSqlQuery qry;
    QString PltmhRR,
            PltmhRS,
            PltmhRT;
    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        PltmhRR= qry.value(3).toString();
        PltmhRS= qry.value(4).toString();
        PltmhRT= qry.value(5).toString();

        ui->twStatusMetering->item(6,Column)->setText(PltmhRR);
        ui->twStatusMetering->item(7,Column)->setText(PltmhRS);
        ui->twStatusMetering->item(8,Column)->setText(PltmhRT);
    }
}

void MainWindow::GetRST_INVERTER_ReActive(QString Query, int Column)
{
    QSqlQuery qry;
    QString InverterRR,
            InverterRS,
            InverterRT;
    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        InverterRR= qry.value(3).toString();
        InverterRS= qry.value(4).toString();
        InverterRT= qry.value(5).toString();

        ui->twStatusMetering->item(6,Column)->setText(InverterRR);
        ui->twStatusMetering->item(7,Column)->setText(InverterRS);
        ui->twStatusMetering->item(8,Column)->setText(InverterRT);
    }
}

void MainWindow::GetRST_LOAD_ReActive(QString Query, int Column)
{
    QSqlQuery qry;
    QString LoadRR,
            LoadRS,
            LoadRT;
    qry.prepare(Query);
    qry.exec();
    if (qry.next())
    {
        LoadRR= qry.value(3).toString();
        LoadRS= qry.value(4).toString();
        LoadRT= qry.value(5).toString();

        ui->twStatusMetering->item(6,Column)->setText(LoadRR);
        ui->twStatusMetering->item(7,Column)->setText(LoadRS);
        ui->twStatusMetering->item(8,Column)->setText(LoadRT);
    }
}



void MainWindow::ReadAllWVAR(bool Activated)
{
    if (Activated!=false)
    {
        GetPLTSWVAR("select * from in_power_plts order by id desc limit 1",1);
        GetPLTMHWVAR("select * from in_power_pltmh order by id desc limit 1",2);
        GetINVERTERWVAR("select * from in_power_inverter order by id desc limit 1",3);
        GetLOADWVAR("select * from in_power_load order by id desc limit 1",4);
        //GetBATTERYWVAR("select * from in_power_battery order by id desc limit 1",5);
    }
}

void MainWindow::GetPLTSWVAR(QString Query,int Col)
{
    QSqlQuery qry;
    QString W,Var;
    //float dataW;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        W=qry.value(3).toString();
        Var=qry.value(4).toString();
        // METERING
        ui->twStatusMetering->item(1,Col)->setText(W);
        ui->twStatusMetering->item(5,Col)->setText(Var);
        // HMI
        double dataW,dataVar;
        dataW = W.toDouble();
        dataVar = Var.toDouble();
        dataW = std::abs(dataW);
        dataVar = std::abs(dataVar);

        W = QString::number(dataW);
        Var = QString::number(dataVar);

        ui->lnPltsOutW->display(W);
        ui->lnPltsOutVar->display(Var);

        dataW = W.toFloat();
        if(dataW >= 0){
            ui->OvPltsW->display(W);

            ui->OvArrowPlts->setStyleSheet("image: url(:/img/DirectNormal.png)");
            ui->ArrowPlts->setStyleSheet("image: url(:/img/DirectNormal.png)");

        }
        //arah ke kiri
        else if(dataW < 0){
            dataW = dataW + (dataW * (-2));
            ui->OvPltsW->display(QString::number(dataW));
            ui->OvArrowPlts->setStyleSheet("image: url(:/img/right_direct_.png)");
            ui->ArrowPlts->setStyleSheet("image: url(:/img/right_direct_.png)");


        }
    }

}

void MainWindow::GetPLTMHWVAR(QString Query,int Col)
{
    QSqlQuery qry;
    QString W,Var;
    //float dataW;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        W=qry.value(3).toString();
        Var=qry.value(4).toString();
        // METERING
        ui->twStatusMetering->item(1,Col)->setText(W);
        ui->twStatusMetering->item(5,Col)->setText(Var);
        // HMI
        double dataW,dataVar;
        dataW = W.toDouble();
        dataVar = Var.toDouble();

        dataW = std::abs(dataW);
        dataVar = std::abs(dataVar);
        QString dataWW,dataVarr;

        dataWW = QString::number(dataW);
        dataVarr = QString::number(dataVar);

        ui->OutPltmhW->display(dataWW);
        ui->OutPltmhVar->display(dataVarr);

        dataW = W.toFloat();
        if(dataW > 0){
            ui->OvPltmhW->display(W);
            ui->OvArrowPltmh->setStyleSheet("image: url(:/img/right_direct_.png)");
            ui->ArrowPLTMH->setStyleSheet("image: url(:/img/right_direct_.png)");
        }
        //arah ke kiri
        else if(dataW < 0){
            dataW = dataW + (dataW * (-2));
            ui->OvPltmhW->display(QString::number(dataW));

            ui->OvArrowPltmh->setStyleSheet("image: url(:/img/DirectNormal.png)");
            ui->ArrowPLTMH->setStyleSheet("image: url(:/img/DirectNormal.png)");

        }
    }
}

void MainWindow::GetINVERTERWVAR(QString Query, int Col)
{
    QSqlQuery qry;
    QString W,Var;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        W=qry.value(3).toString();
        Var=qry.value(4).toString();
        // METERING
        ui->twStatusMetering->item(1,Col)->setText(W);
        ui->twStatusMetering->item(5,Col)->setText(Var);
        SetDataCurva(W.toDouble(),Var.toDouble());
    }
}

void MainWindow::GetLOADWVAR(QString Query, int Col)
{
    QSqlQuery qry;
    QString W,Var;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        W=qry.value(3).toString();
        Var=qry.value(4).toString();
        //METERING
        ui->twStatusMetering->item(1,Col)->setText(W);
        ui->twStatusMetering->item(5,Col)->setText(Var);

        //HMI
        double dataW,dataVar;
        dataW = W.toDouble();
        dataVar = Var.toDouble();
        dataW = std::abs(dataW);
        dataVar = std::abs(dataVar);

        W = QString::number(dataW);
        Var = QString::number(dataVar);

        ui->OutLoadW->display(W);
        ui->OutLoadVar->display(Var);

        ui->OvLoadW->display(W);
    }
}

void MainWindow::GetBATTERYWVAR(QString Query, int Col)
{
    QSqlQuery qry;
    QString W,Var;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        W=qry.value(3).toString();
        Var=qry.value(4).toString();
        // METERING
        ui->twStatusMetering->item(1,Col)->setText(W);
        ui->twStatusMetering->item(5,Col)->setText(Var);
        //HMI
        //ui->OutBatteryW->display(W);
        ui->OutBatteryVar->display(Var);
    }
}

void MainWindow::ReadAllCOSPHI(bool Activated)
{
    if (Activated!=false)
    {
        GetPLTS_COSPHi("select * from mon_cosphi_plts order by id desc limit 1",1);
        GetPLTMH_COSPHi("select * from mon_cosphi_pltmh order by id desc limit 1",2);
        //GetINVERTER_COSPHi("select * from mon_cosphi_inverter order by id desc limit 1",3);
        GetLOAD_COSPHi("select * from mon_cosphi_load order by id desc limit 1",4);
    }
}

void MainWindow::GetPLTS_COSPHi(QString Query, int col)
{
    QSqlQuery qry;
    QString R,S,T;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        R=qry.value(3).toString();
        S=qry.value(4).toString();
        T=qry.value(5).toString();
        // METERING
        ui->twStatusMetering->item(17,col)->setText(R);
        ui->twStatusMetering->item(18,col)->setText(S);
        ui->twStatusMetering->item(19,col)->setText(T);
    }
}

void MainWindow::GetPLTMH_COSPHi(QString Query, int col)
{
    QSqlQuery qry;
    QString R,S,T;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        R=qry.value(3).toString();
        S=qry.value(4).toString();
        T=qry.value(5).toString();
        // METERING
        ui->twStatusMetering->item(17,col)->setText(R);
        ui->twStatusMetering->item(18,col)->setText(S);
        ui->twStatusMetering->item(19,col)->setText(T);
    }
}

void MainWindow::GetINVERTER_COSPHi(QString Query, int col)
{
    QSqlQuery qry;
    QString R,S,T;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        R=qry.value(3).toString();
        S=qry.value(4).toString();
        T=qry.value(5).toString();
        // METERING
        ui->twStatusMetering->item(17,col)->setText(R);
        ui->twStatusMetering->item(18,col)->setText(S);
        ui->twStatusMetering->item(19,col)->setText(T);
    }
}

void MainWindow::GetLOAD_COSPHi(QString Query, int col)
{
    QSqlQuery qry;
    QString R,S,T;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        R=qry.value(3).toString();
        S=qry.value(4).toString();
        T=qry.value(5).toString();
        // METERING
        ui->twStatusMetering->item(17,col)->setText(R);
        ui->twStatusMetering->item(18,col)->setText(S);
        ui->twStatusMetering->item(19,col)->setText(T);
    }
}

void MainWindow::Get_Frequency(QString Query)
{

    int a = 0;
    QSqlQuery qry,qry2;
    QString val;
    QDateTime tanggal1,tanggal2;
    double value1,value2;
    double time;
    qry.prepare(Query);
    qry.exec();
    while(qry.next())
    {
        if(a == 0){
            QString tgl = qry.value(2).toString();
            QString frmt = "yyyy-MM-dd hh:mm:ss.zzz";
            tanggal1 = QDateTime::fromString(tgl,frmt);

            value1 = qry.value(3).toDouble();
            val = QString::number(value1);
            ui->lnPltsOutFreq->display(val);
        }
        else if(a == 1){
            QString tgl = qry.value(2).toString();
            QString frmt = "yyyy-MM-dd hh:mm:ss.zzz";
            tanggal2 = QDateTime::fromString(tgl,frmt);
            time = tanggal1.msecsTo(tanggal2);
            time = time / 1000;
            value2 = qry.value(3).toDouble();
            val = QString::number(value2);
        }
    }
}

void MainWindow::Get_Frequency_inverter(QString Query)
{
    QSqlQuery qry;
    QString val;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        val=qry.value(0).toString();
        ui->lnInvBusbarAcHz->display(val);
    }

}


void MainWindow::ReadAllDataFrmDB()
{
    //pengecekan database connect
    if(databaseRealtime == 1){
        //pembacaan realtime setiap database

        GetAllStatusBatre();
        getDataInverter();
        ReadAllWVAR(true);

        //get all status RST to hmi,metering
        ReadAllRST(true);
        ReadAllRST_ActiveReactive(true);
        //get all status rtu to hmi,metering,network
        GetAllStatusRTU(true);
        //GetAllEventList(true);
        ReadAllCOSPHI(true);
        Get_Frequency("select * from mon_plts_freq order by id desc limit 2");
        Get_Frequency_inverter("select value from mon_inverter_freq order by id desc limit 1");

        //hys
        if(realChart == 1){
            //data belum ditemukan
            if(dataSuccesRequestHystory == 0){
                //pengecekan ke database
                QString dataR =  "select id,information from alloutcontrol where Status = 'STATUS_ARCHIVE' order by id desc limit 1";
                QSqlQuery qry1;
                qry1.prepare(dataR);
                qry1.exec();
                if(qry1.next()){
                    if(idHystory < qry1.value(0).toInt()){
                        idHystory = qry1.value(0).toInt();
                        dataHystory = qry1.value(1).toString();
                        dataSuccesRequestHystory = 1;
                    }
                    else{
                        countStatusWaiting++;
                    }
                }else{
                    countStatusWaiting++;
                }
                if(countStatusWaiting == 10)
                {
                    dataSuccesRequestHystory = 2;
                    QMessageBox::information(this,"TIMEOUT","Trending menunggu terlalu lama,Kembali ke Realtime");
                    ui->pbTrenReak->setEnabled(false);
                    ui->pbTrenHis->setEnabled(true);
                    ui->comboBox->setEnabled(true);
                    realChart = 0;
                }
            }
            //data ditemukan namun belum dicari
            else if(dataSuccesRequestHystory == 1){
                //cek ganti database lalu
                if(dataHystory == "UNARCHIVE OK !"){
                    dataSuccesRequestHystory = 2;
                    readDatabaseHystory();
                }
                else {
                    QMessageBox::information(this,"ARCHIVE NOT OK","ARCHIVE TIDAK ADA DISERVER");
                    ui->pbTrenReak->setEnabled(false);
                    ui->pbTrenHis->setEnabled(true);
                    ui->comboBox->setEnabled(true);
                    realChart = 0;
                }
                //mengganti nilai dataSuccesRequestHystory = 2
                dataSuccesRequestHystory = 2;
            }
            else if(dataSuccesRequestHystory == 2){

            }
        }
        //realtime
        else if(realChart == 0){
            //get data trending
            setDataTrending();
            //dataSuccesRequestHystory = false;
        }

        //getdataradiasi
        getDataRadiasi("select radiation from mon_radiation order by id desc limit 1");
        //DATA CONNECTION

        //setiap jam 06.00 dan 18.00 clear graph
        if(ui->txtDateTime->text().contains("06:00:00") || ui->txtDateTime->text().contains("18:00:00")){
            clearDataGraphTranding();
            clearDataRadiasi();
        }
    }

}



void MainWindow::RealtimeDB()
{
    if(!connectToDatabase2){
        QString ON="image: url(:/img/ok.png)";
        QString OFF="image: url(:/img/nok.png)";
        QSettings settings(QDir::currentPath() + "/dbconfig.ini",
                           QSettings::IniFormat );
        settings.beginGroup("SettingsDB");
        QString host = settings.value("hostserver").toString();
        QString dbase = settings.value("database").toString();
        QString user = settings.value("user").toString();
        QString pwd = settings.value("password").toString();
        // int port = settings.value("port");
        //  QString timeout = settings.value("timeout").toInt();
        settings.endGroup();

        //set database connecting
        QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
        QElapsedTimer timer;
        timer.start();

        db.setHostName(host);
        db.setPort(3306);
        db.setDatabaseName(dbase);
        db.setUserName(user);
        db.setPassword(pwd);
        db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=5");

        if(!db.open())
        {
            //clear db
            db.setConnectOptions();

            ui->lblindiserver->setStyleSheet(OFF);
            addRowAlarmTable("Not Available","-","Server Protocol","Status Network Server Condition Is Offline");
            if(NetServer){
                dataout[28] = "Status Network Server Condition Is Offline";
                InsertDataControl_CB(17);
            }
            NetServer = false;
            //ui->lblindiGPS->setStyleSheet(OFF);
            ui->lbl_server->setText("OFFLINE");
            ui->db_stat->setStyleSheet("color: rgb(200, 0, 0);");
            ui->db_stat->setText("SERVER:DISCONNECTED! Now Reconnecting...!!!");
            setPinkAll();
            databaseRealtime = 0;
            // Insert data to alloutcontrol
            //dataout[26]="SERVER DISCONNECTED!!";
            //InsertDataStatusLogServer(0);
        }
        else
        {
            //clear db
            db.setConnectOptions();

            ui->lblindiserver->setStyleSheet(ON);
            removeRowAlarmTable("Status Network Server Condition Is Offline");
            if(!NetServer){
                dataout[28] = "Status Network Server Condition Is Normal";
                InsertDataControl_CB(17);
            }
            NetServer = true;
            ui->lblindiswitch->setStyleSheet(ON);
            removeRowAlarmTable("Status Network SWITCH Condition Is Offline");
            if(!NetSwitch){
                dataout[28] = "Status Network SWITCH Condition Is Normal";
                InsertDataControl_CB(17);
            }
            NetSwitch = true;
            ui->label_246->setText("ONLINE");
            //ui->lblindiGPS->setStyleSheet(ON);
            ui->lbl_server->setText("ONLINE");
            ui->db_stat->setStyleSheet("color: rgb(0, 170, 0);");
            ui->db_stat->setText("SERVER:CONNECTED!!");
            databaseRealtime = 1;


            // Insert data to alloutcontrol
            // dataout[26]="SERVER CONNECTED!!";
            //InsertDataStatusLogServer(15);
        }
    }
}

void MainWindow::InsertDataControl_CB(int ParamControl)
{
    timeHMI = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString value="timestamps,status,outvalue,operatorname,information,IOA";
    QString Param=":p_1,:p_2,:p_3,:p_4,:p_5,:p_6";
    switch (ParamControl) {
    case 0:
        InsertControl_PLTS("insert into alloutcontrol("+value+")values("+Param+")","ASDU 3",timeHMI,dataout[0],ui->lb_admin->text());
        InsertControl_PLTS("insert into all_control_protocol("+value+")values("+Param+")","ASDU 3",timeHMI,dataout[0],ui->lb_admin->text());
        break;
    case 1:
        InsertControl_PLTS_1("insert into alloutcontrol("+value+")values("+Param+")","ASDU 3",timeHMI,dataout[1],ui->lb_admin->text());
        InsertControl_PLTS_1("insert into all_control_protocol("+value+")values("+Param+")","ASDU 3",timeHMI,dataout[1],ui->lb_admin->text());
        break;
    case 2:
        InsertControl_PLTS_2("insert into alloutcontrol("+value+")values("+Param+")","ASDU 3",timeHMI,dataout[2],ui->lb_admin->text());
        InsertControl_PLTS_2("insert into all_control_protocol("+value+")values("+Param+")","ASDU 3",timeHMI,dataout[2],ui->lb_admin->text());
        break;
    case 3:
        InsertControl_PLTS_3("insert into alloutcontrol("+value+")values("+Param+")","ASDU 3",timeHMI,dataout[3],ui->lb_admin->text());
        InsertControl_PLTS_3("insert into all_control_protocol("+value+")values("+Param+")","ASDU 3",timeHMI,dataout[3],ui->lb_admin->text());
        break;
    case 4:
        InsertControl_PLTMH("insert into alloutcontrol("+value+")values("+Param+")","ASDU 1",timeHMI,dataout[4],ui->lb_admin->text());
        InsertControl_PLTMH("insert into all_control_protocol("+value+")values("+Param+")","ASDU 1",timeHMI,dataout[4],ui->lb_admin->text());
        break;
    case 5:
        InsertControl_BATTERY("insert into alloutcontrol("+value+")values("+Param+")","ASDU 4",timeHMI,dataout[5],ui->lb_admin->text());
        InsertControl_BATTERY("insert into all_control_protocol("+value+")values("+Param+")","ASDU 4",timeHMI,dataout[5],ui->lb_admin->text());
        break;
    case 6:
        InsertControl_INVERTER("insert into alloutcontrol("+value+")values("+Param+")","ASDU 4",timeHMI,dataout[6],ui->lb_admin->text());
        InsertControl_INVERTER("insert into all_control_protocol("+value+")values("+Param+")","ASDU 4",timeHMI,dataout[6],ui->lb_admin->text());
        break;
        //    case 7:
        //        InsertControl_INVERTER_1("insert into alloutcontrol("+value+")values("+Param+")","ASDU 4",timeHMI,dataout[7],ui->lb_admin->text());
        //        break;
        //    case 8:
        //        InsertControl_INVERTER_2("insert into alloutcontrol("+value+")values("+Param+")","ASDU 4",timeHMI,dataout[8],ui->lb_admin->text());
        //        break;
        //    case 9:
        //        InsertControl_INVERTER_3("insert into alloutcontrol("+value+")values("+Param+")","ASDU 4",timeHMI,dataout[9],ui->lb_admin->text());
        //        break;
    case 10:
        InsertControl_LOAD("insert into alloutcontrol("+value+")values("+Param+")","ASDU 2",timeHMI,dataout[10],ui->lb_admin->text());
        InsertControl_LOAD("insert into all_control_protocol("+value+")values("+Param+")","ASDU 2",timeHMI,dataout[10],ui->lb_admin->text());
        break;
    case 11:
        InsertControl_PLTS_Status("insert into alloutcontrol("+value+")values("+Param+")","IOA 1 ASDU 3",timeHMI,dataout[28],ui->lb_admin->text());
        break;
    case 12:
        InsertControl_PLTMH_Status("insert into alloutcontrol("+value+")values("+Param+")","IOA 1 ASDU 1",timeHMI,dataout[28],ui->lb_admin->text());
        break;
    case 13:
        InsertControl_LOAD_Status("insert into alloutcontrol("+value+")values("+Param+")","IOA 1 ASDU 3",timeHMI,dataout[28],ui->lb_admin->text());
        break;
    case 14:
        InsertControl_Battery_Status("insert into alloutcontrol("+value+")values("+Param+")","IOA 1 ASDU 4",timeHMI,dataout[28],ui->lb_admin->text());
        break;
    case 15:
        InsertControl_INVERTER_Status("insert into alloutcontrol("+value+")values("+Param+")","IOA 1 ASDU 5",timeHMI,dataout[28],ui->lb_admin->text());
        break;
    case 16:
        InsertControl_Hystory("insert into alloutcontrol("+value+")values("+Param+")","IOA 1 ASDU 5",timeHMI,dataout[28],ui->lb_admin->text());
        InsertControl_Hystory("insert into all_control_protocol("+value+")values("+Param+")","IOA 1 ASDU 5",timeHMI,dataout[28],ui->lb_admin->text());
        break;
    case 17:
        InsertControl_Network("insert into alloutcontrol("+value+")values("+Param+")","IOA 1 ASDU 5",timeHMI,dataout[28],ui->lb_admin->text());
        break;
    case 18:
        // InsertControl_InverterCurve("insert into alloutcontrol("+value+")values("+Param+")","",timeHMI,dataout[28],ui->lb_admin->text());
        break;

    default:
        break;
    }

}

void MainWindow::InsertDataSetPoint(int Param)
{
    timeHMI = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString value="timestamps,status,outvalue,operatorname,information,IOA";
    QString Par=":p_1,:p_2,:p_3,:p_4,:p_5,:p_6";
    switch (Param) {
    case 0:
        InsertPltmhW("insert into alloutcontrol("+value+")values("+Par+")","IOA 1 ASDU 3",timeHMI,dataout[11],ui->lb_admin->text());
        InsertPltmhW("insert into all_control_protocol("+value+")values("+Par+")","IOA 1 ASDU 3",timeHMI,dataout[11],ui->lb_admin->text());
        break;
    case 1:
        InsertPltmhVAR("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[12],ui->lb_admin->text());
        InsertPltmhVAR("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[12],ui->lb_admin->text());
        break;
    case 2:
        InsertBatteryDODMIN("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[13],ui->lb_admin->text());
        InsertBatteryDODMIN("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[13],ui->lb_admin->text());
        break;
    case 3:
        InsertBatteryDODMAX("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[14],ui->lb_admin->text());
        InsertBatteryDODMAX("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[14],ui->lb_admin->text());
        break;
    case 4:
        InsertBatterySOCMIN("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[15],ui->lb_admin->text());
        InsertBatterySOCMIN("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[15],ui->lb_admin->text());
        break;
    case 5:
        InsertBatterySOCMAX("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[16],ui->lb_admin->text());
        InsertBatterySOCMAX("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[16],ui->lb_admin->text());
        break;
    case 6:
        InsertModeInverter("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[17],ui->lb_admin->text());
        InsertModeInverter("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[17],ui->lb_admin->text());
        break;
    case 7:
        InsertInverterW("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[18],ui->lb_admin->text());
        InsertInverterW("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[18],ui->lb_admin->text());
        break;
    case 8:
        InsertInverterVAR("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[19],ui->lb_admin->text());
        InsertInverterVAR("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[19],ui->lb_admin->text());
        break;
    case 9:
        InsertGradienM1("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[20],ui->lb_admin->text());
        InsertGradienM1("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[20],ui->lb_admin->text());
        break;
    case 10:
        InsertGradienC1("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[21],ui->lb_admin->text());
        InsertGradienC1("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[21],ui->lb_admin->text());
        break;
    case 11:
        InsertGradienM2("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[22],ui->lb_admin->text());
        InsertGradienM2("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[22],ui->lb_admin->text());
        break;
    case 12:
        InsertGradienC2("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[23],ui->lb_admin->text());
        InsertGradienC2("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[23],ui->lb_admin->text());
        break;
    case 13:
        InsertLoadW("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[24],ui->lb_admin->text());
        InsertLoadW("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[24],ui->lb_admin->text());
        break;
    case 14:
        InsertLoadVAR("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[25],ui->lb_admin->text());
        InsertLoadVAR("insert into all_control_protocol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[25],ui->lb_admin->text());
        break;
    }
}

void MainWindow::InsertDataStatusLogServer(int Params)
{
    timeHMI = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString value="timestamps,status,outvalue,operatorname,information,IOA";
    QString Par=":p_1,:p_2,:p_3,:p_4,:p_5,:p_6";
    switch (Params)
    {
    case 0:
        InsertStatusServer("insert into alloutcontrol("+value+")values("+Par+")","IOA 2 ASDU 3",timeHMI,dataout[26],ui->lb_admin->text());
        break;
    case 1:
        InsertLogUser("insert into alloutcontrol("+value+")values("+Par+")","NOT AVALIABLE!",timeHMI,dataout[27],ui->lb_admin->text());
        break;
    default:
        break;
    }
}


void MainWindow::InsertControl_PLTS(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;

    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "PLTS");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);

    QString Info;
    if (ParamValue=="1")
    {
        Info="Control CB PLTS Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Control CB PLTS Condition Is On";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_PLTS_Status(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;

    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "PLTS_STATUS");
    qry.bindValue(":p_3", "1");
    qry.bindValue(":p_4", Operator);

    /*QString Info;
    if (ParamValue=="1")
    {
        Info="Status CB PLTS Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Status CB PLTS Condition Is On";
    }*/

    qry.bindValue(":p_5", ParamValue);
    qry.bindValue(":p_6", "ASDU 3");
    qry.exec();
}



void MainWindow::InsertControl_PLTS_1(QString Query, QString IOA, QString DateHmi, QString ParamValue,QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "PLTS1");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);

    QString Info;
    if (ParamValue=="1")
    {
        Info="Control CB 1 PLTS Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Control CB 1 PLTS Condition Is On";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_PLTS_2(QString Query, QString IOA, QString DateHmi, QString ParamValue,QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "PLTS2");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);

    QString Info;
    if (ParamValue=="1")
    {
        Info="Control CB 2 PLTS Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Control CB 2 PLTS Condition Is On";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_PLTS_3(QString Query, QString IOA, QString DateHmi, QString ParamValue,QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "PLTS3");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);

    QString Info;
    if (ParamValue=="1")
    {
        Info="Control CB 3 PLTS Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Control CB 3 PLTS Condition Is On";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_PLTMH(QString Query, QString IOA, QString DateHmi, QString ParamValue,QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "PLTMH");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);

    QString Info;
    if (ParamValue=="1")
    {
        Info="Control CB PLTMH Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Control CB PLTMH Condition Is On";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_PLTMH_Status(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;

    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "PLTMH_STATUS");
    qry.bindValue(":p_3", "1");
    qry.bindValue(":p_4", Operator);

    /*QString Info;
    if (ParamValue=="1")
    {
        Info="Status CB PLTMH Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Status CB PLTMH Condition Is On";
    }else if (ParamValue=="3")
    {
        Info="Status CB PLTMH Condition Is Offline";
    }*/
    qry.bindValue(":p_5", ParamValue);
    qry.bindValue(":p_6", "ASDU 1");
    qry.exec();
}

void MainWindow::InsertControl_INVERTER(QString Query, QString IOA, QString DateHmi, QString ParamValue,QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTER");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);

    QString Info;
    if (ParamValue=="1")
    {
        Info="Control CB INVERTER Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Control CB INVERTER Condition Is On";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_INVERTER_Status(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTER_STATUS");
    qry.bindValue(":p_3", "1");
    qry.bindValue(":p_4", Operator);


    qry.bindValue(":p_5", ParamValue);
    qry.bindValue(":p_6", "ASDU 4");
    qry.exec();
}

void MainWindow::InsertControl_INVERTER_1(QString Query, QString IOA, QString DateHmi, QString ParamValue,QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTER1");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);

    QString Info;
    if (ParamValue=="1")
    {
        Info="Control CB 1 INVERTER Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Control CB 1 INVERTER Condition Is On";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_INVERTER_2(QString Query, QString IOA, QString DateHmi, QString ParamValue,QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTER2");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);

    QString Info;
    if (ParamValue=="1")
    {
        Info="Control CB 2 INVERTER Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Control CB 2 INVERTER Condition Is On";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_INVERTER_3(QString Query, QString IOA, QString DateHmi, QString ParamValue,QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTER3");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);

    QString Info;
    if (ParamValue=="1")
    {
        Info="Control CB 3 INVERTER Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Control CB 3 INVERTER Condition Is On";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_LOAD(QString Query, QString IOA, QString DateHmi, QString ParamValue,QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "LOAD");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);

    QString Info;
    if (ParamValue=="1")
    {
        Info="Control CB LOAD Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Control CB LOAD Condition Is On";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_LOAD_Status(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "LOAD_STATUS");
    qry.bindValue(":p_3", "1");
    qry.bindValue(":p_4", Operator);

    /*QString Info;
    if (ParamValue=="1")
    {
        Info="Status CB LOAD Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Status CB LOAD Condition Is On";
    }*/
    qry.bindValue(":p_5", ParamValue);
    qry.bindValue(":p_6", "ASDU 2");
    qry.exec();
}

void MainWindow::InsertControl_Hystory(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "Data_Archive");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    qry.bindValue(":p_5", "User meminta archive hystory tanggal "+ParamValue);
    qry.bindValue(":p_6", "Not Available");
    qry.exec();
}

void MainWindow::InsertControl_Network(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "NETWORK_STATUS");
    qry.bindValue(":p_3", "-");
    qry.bindValue(":p_4", Operator);
    qry.bindValue(":p_5", ParamValue);
    qry.bindValue(":p_6", "Not Available");
    qry.exec();
    GetAllEventList(true,"Network");
}

void MainWindow::InsertControl_InverterCurve(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "CURVE_STATUS");
    qry.bindValue(":p_3", "-");
    qry.bindValue(":p_4", Operator);
    qry.bindValue(":p_5", ParamValue);
    qry.bindValue(":p_6", "Not Available");
    qry.exec();
    GetAllEventList(true,"Network");
}

void MainWindow::InsertPltmhW(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "PLTMHW");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="PLTMHW Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertPltmhVAR(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "PLTMHVAR");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="PLTMH VAR Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertBatteryDODMIN(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "DODMIN");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="DOD MIN Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertBatteryDODMAX(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "DODMAX");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="DOD MAX Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertBatterySOCMIN(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "SOCMIN");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="SOC MIN Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertBatterySOCMAX(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "SOCMAX");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="SOC MAX Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}



void MainWindow::InsertModeInverter(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTERMODE");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info;
    if (ParamValue=="0")
    {
        Info="Mode INVERTER : Manual";
    }
    else if (ParamValue =="1")
    {
        Info="Mode INVERTER : Auto";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertInverterW(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTERW");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="INVERTER W Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertInverterVAR(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTERVAR");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="INVERTER VAR Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertGradienM1(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTERM1");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="INVERTER Gradien M1 VAR Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertGradienC1(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTERC1");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="INVERTER Gradien C1 VAR Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertGradienM2(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTERM2");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="INVERTER Gradien M2 VAR Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertGradienC2(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "INVERTERC2");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="INVERTER Gradien M2 VAR Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertLoadW(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "LOADW");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="LOAD W Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertLoadVAR(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "LOADVAR");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="LOAD VAR Value Is "+ParamValue+"";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertStatusServer(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "SERVER_STATUS");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info="SERVER IS : "+ParamValue+"!";
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertLogUser(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "LOG_USER");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);
    QString Info;
    if (ParamValue=="0")
    {
        Info="USER '"+Operator+"' Has Been LoggedOut!";
    }
    else if(ParamValue=="1")
    {
        Info="USER '"+Operator+"' Has Been LoggedIn!";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_BATTERY(QString Query, QString IOA, QString DateHmi, QString ParamValue,QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "BATTERY");
    qry.bindValue(":p_3", ParamValue);
    qry.bindValue(":p_4", Operator);

    QString Info;
    if (ParamValue=="1")
    {
        Info="Control CB BATTERY Condition Is Off";
    }
    else if (ParamValue=="2")
    {
        Info="Control CB BATTERY Condition Is On";
    }
    qry.bindValue(":p_5", Info);
    qry.bindValue(":p_6", IOA);
    qry.exec();
}

void MainWindow::InsertControl_Battery_Status(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1", DateHmi);
    qry.bindValue(":p_2", "BATTERY_STATUS");
    qry.bindValue(":p_3", "1");
    qry.bindValue(":p_4", Operator);
    /*
    QString Info;
    if (ParamValue=="1")
    {
        Info="Status CB BATTERY Condition is closed";
    }
    else if (ParamValue=="2")
    {
        Info="Status CB BATTERY Condition is opened";
    }*/
    qry.bindValue(":p_5", ParamValue);
    qry.bindValue(":p_6", "ASDU 4");
    qry.exec();
}


void MainWindow::GetAllEventList(bool Activated,QString List)
{
    if (Activated!=false)
    {
        if (List=="PLTS")
        {
            EventListPLTS("select timestamps,operatorname,outvalue,IOA,information from alloutcontrol "
                          "where status='PLTS' or status='PLTS_STATUS' or status='PLTS1' or status='PLTS2' or status='PLTS3' or status='CBPLTS' or status='RTUPLTS' order by timestamps desc limit 100");
        }
        if (List=="PLTMH")
        {
            EventListPLTMH("select timestamps,operatorname,outvalue,IOA,information from alloutcontrol "
                           "where status ='PLTMH' or status='PLTMH_STATUS' or status ='PLTMHW' or status ='PLTMHVAR' or status='CBPLTMH'  or status='RTUPLTMH' order by timestamps desc limit 100");
        }
        if (List=="LOAD")
        {
            EventListLoad("select timestamps,operatorname,outvalue,IOA,information from alloutcontrol "
                          "where status ='LOAD' or status='LOAD_STATUS' or status='CBLOAD' or status='RTULOAD' or status='LOADW'  or status='LOADVAR' order by timestamps desc limit 100 ");
        }
        if(List =="INVERTER")
        {
            EventListInverter("select timestamps,operatorname,outvalue,IOA,information from alloutcontrol "
                              "where status ='INVERTER' or status='INVERTER_STATUS' or status ='INVERTER1' or status ='INVERTER2' or status ='INVERTER3' or status='CBINVERTER' or status='RTUINVERTER' or status='INVERTERMODE'  or status='INVERTERW'  or status='INVERTERVAR'  or status='INVERTERM1' or status='INVERTERC1' or status='INVERTERM2' or status='INVERTERC2' or status='CURVE_STATUS' order by timestamps desc limit 100");
        }
        if(List == "BATTERY")
        {
            EventListBattery("select timestamps,operatorname,outvalue,IOA,information from alloutcontrol "
                             "where status ='BATTERY' or status='BATTERY_STATUS' or status ='DODMAX' or status ='DODMIN' or status ='SOCMAX' or status ='SOCMIN' order by timestamps desc limit 100");
        }
        if(List == "EventList")
        {
            EventListAll("select timestamps,operatorname,outvalue,IOA,information from alloutcontrol where not status='LOG_USER' or not status='SERVER_STATUS' order by timestamps desc limit 100");
        }
        if(List == "Network")
        {
            EventListNetwork("select timestamps,operatorname,outvalue,IOA,information from alloutcontrol "
                             "where status='RTUPLTS' or status='RTUPLTMH' or status='RTULOAD' or status='RTUINVERTER' or status='SERVER_STATUS' or status ='NETWORK_STATUS' order by timestamps desc limit 100");
        }
        if(List == "Alarm")
        {
            //EventAlarmList("select timestamps,operatorname,outvalue,IOA,information from alloutcontrol "
            //               "where status='CBPLTS' or status='CBPLTMH' or status='CBINVERTER' or status='CBLOAD' or status='RTUPLTS' or status='RTUPLTMH' or status='RTULOAD' or status='RTUINVERTER' order by timestamps desc limit 100");
        }
    }
}

void MainWindow::EventListPLTS(QString Query)
{
    QSqlQuery qry;
    QString date,op,value,info,ioa;
    qry.prepare(Query);
    qry.exec();
    int index=0;
    // ui->twEventPlts->clear();
    while (qry.next())
    {

        date=qry.value(0).toString();
        op=qry.value(1).toString();
        value=qry.value(2).toString();
        info=qry.value(4).toString();
        ioa=qry.value(3).toString();
        ui->twEventPlts->setItem(index,0,new QTableWidgetItem(date));
        ui->twEventPlts->setItem(index,1,new QTableWidgetItem(ioa));
        ui->twEventPlts->setItem(index,2,new QTableWidgetItem(value));
        ui->twEventPlts->setItem(index,3,new QTableWidgetItem(op));
        ui->twEventPlts->setItem(index,4,new QTableWidgetItem(info));
        index++;
    }
}

void MainWindow::EventListPLTMH(QString Query)
{
    QSqlQuery qry;
    QString date,op,value,info,ioa;
    qry.prepare(Query);
    qry.exec();
    int index=0;
    //  ui->twEventPltmh->clear();
    while (qry.next())
    {
        date=qry.value(0).toString();
        op=qry.value(1).toString();
        value=qry.value(2).toString();
        info=qry.value(4).toString();
        ioa=qry.value(3).toString();
        ui->twEventPltmh->setItem(index,0,new QTableWidgetItem(date));
        ui->twEventPltmh->setItem(index,1,new QTableWidgetItem(ioa));
        ui->twEventPltmh->setItem(index,2,new QTableWidgetItem(value));
        ui->twEventPltmh->setItem(index,3,new QTableWidgetItem(op));
        ui->twEventPltmh->setItem(index,4,new QTableWidgetItem(info));
        index++;
    }
}

void MainWindow::EventListBattery(QString Query)
{
    QSqlQuery qry;
    QString date,op,value,info,ioa;
    qry.prepare(Query);
    qry.exec();
    int index=0;
    //   ui->twEventBattery->clear();
    while (qry.next())
    {
        date=qry.value(0).toString();
        op=qry.value(1).toString();
        value=qry.value(2).toString();
        info=qry.value(4).toString();
        ioa=qry.value(3).toString();
        ui->twEventBattery->setItem(index,0,new QTableWidgetItem(date));
        ui->twEventBattery->setItem(index,1,new QTableWidgetItem(ioa));
        ui->twEventBattery->setItem(index,2,new QTableWidgetItem(value));
        ui->twEventBattery->setItem(index,3,new QTableWidgetItem(op));
        ui->twEventBattery->setItem(index,4,new QTableWidgetItem(info));
        index++;
    }
}

void MainWindow::EventListInverter(QString Query)
{

    QSqlQuery qry;
    QString date,op,value,info,ioa;
    qry.prepare(Query);
    qry.exec();
    int index=0;
    // ui->twEventInverter->clear();
    while (qry.next())
    {
        date=qry.value(0).toString();
        op=qry.value(1).toString();
        value=qry.value(2).toString();
        info=qry.value(4).toString();
        ioa=qry.value(3).toString();
        ui->twEventInverter->setItem(index,0,new QTableWidgetItem(date));
        ui->twEventInverter->setItem(index,1,new QTableWidgetItem(ioa));
        ui->twEventInverter->setItem(index,2,new QTableWidgetItem(value));
        ui->twEventInverter->setItem(index,3,new QTableWidgetItem(op));
        ui->twEventInverter->setItem(index,4,new QTableWidgetItem(info));
        index++;
    }
}

void MainWindow::EventListLoad(QString Query)
{
    QSqlQuery qry;
    QString date,op,value,info,ioa;
    qry.prepare(Query);
    qry.exec();
    int index=0;
    //  ui->twEventLoad->clear();
    while (qry.next())
    {
        date=qry.value(0).toString();
        op=qry.value(1).toString();
        value=qry.value(2).toString();
        info=qry.value(4).toString();
        ioa=qry.value(3).toString();
        ui->twEventLoad->setItem(index,0,new QTableWidgetItem(date));
        ui->twEventLoad->setItem(index,1,new QTableWidgetItem(ioa));
        ui->twEventLoad->setItem(index,2,new QTableWidgetItem(value));
        ui->twEventLoad->setItem(index,3,new QTableWidgetItem(op));
        ui->twEventLoad->setItem(index,4,new QTableWidgetItem(info));
        index++;
    }
}

void MainWindow::EventListNetwork(QString Query)
{
    QSqlQuery qry;
    QString date,op,value,info,ioa;
    qry.prepare(Query);
    qry.exec();
    int index=0;
    //  ui->twEventLoad->clear();
    while (qry.next())
    {
        date=qry.value(0).toString();
        op=qry.value(1).toString();
        value=qry.value(2).toString();
        info=qry.value(4).toString();
        ioa=qry.value(3).toString();
        ui->twEventNetwork->setItem(index,0,new QTableWidgetItem(date));
        ui->twEventNetwork->setItem(index,1,new QTableWidgetItem(ioa));
        ui->twEventNetwork->setItem(index,2,new QTableWidgetItem(value));
        ui->twEventNetwork->setItem(index,3,new QTableWidgetItem(op));
        ui->twEventNetwork->setItem(index,4,new QTableWidgetItem(info));
        index++;
    }
}

void MainWindow::EventAlarmList(QString Query)
{
    QSqlQuery qry;
    QString date,op,value,info,ioa;
    qry.prepare(Query);
    qry.exec();
    int index=0;

    //sd
    //   ui->twAlarmList->clear();
    while (qry.next())
    {
        date=qry.value(0).toString();
        op=qry.value(1).toString();
        value=qry.value(2).toString();
        info=qry.value(4).toString();
        ioa=qry.value(3).toString();
        //ui->twAlarmList->removeRow(1);
        ui->twAlarmList->setItem(index,0,new QTableWidgetItem(date));
        ui->twAlarmList->setItem(index,1,new QTableWidgetItem(ioa));
        ui->twAlarmList->setItem(index,2,new QTableWidgetItem(value));
        ui->twAlarmList->setItem(index,3,new QTableWidgetItem(op));
        ui->twAlarmList->setItem(index,4,new QTableWidgetItem(info));
        index++;
    }
}

void MainWindow::EventListAll(QString Query)
{
    QSqlQuery qry;
    QString date,op,value,info,ioa;
    qry.prepare(Query);
    qry.exec();
    int index=0;
    // ui->twEventList->clear();
    while (qry.next())
    {
        date=qry.value(0).toString();
        op=qry.value(1).toString();
        value=qry.value(2).toString();
        info=qry.value(4).toString();
        ioa=qry.value(3).toString();
        ui->twEventList->setItem(index,0,new QTableWidgetItem(date));
        ui->twEventList->setItem(index,1,new QTableWidgetItem(ioa));
        ui->twEventList->setItem(index,2,new QTableWidgetItem(value));
        ui->twEventList->setItem(index,3,new QTableWidgetItem(op));
        ui->twEventList->setItem(index,4,new QTableWidgetItem(info));

        ui->twEventOverview->setItem(index,0,new QTableWidgetItem(date));
        ui->twEventOverview->setItem(index,1,new QTableWidgetItem(ioa));
        ui->twEventOverview->setItem(index,2,new QTableWidgetItem(value));
        ui->twEventOverview->setItem(index,3,new QTableWidgetItem(op));
        ui->twEventOverview->setItem(index,4,new QTableWidgetItem(info));

        ui->twEventSimbol->setItem(index,0,new QTableWidgetItem(date));
        ui->twEventSimbol->setItem(index,1,new QTableWidgetItem(ioa));
        ui->twEventSimbol->setItem(index,2,new QTableWidgetItem(value));
        ui->twEventSimbol->setItem(index,3,new QTableWidgetItem(op));
        ui->twEventSimbol->setItem(index,4,new QTableWidgetItem(info));
        index++;
    }
}



void MainWindow::imageSymbolPage()
{
    //isi dari page simbol
    ui->twSimbol->setStyleSheet("color: rgb(0, 0, 0);");
    QPixmap pix1(":/img/closeCB.png");
    QLabel *img1 = new QLabel;
    img1->setPixmap(pix1);
    img1->setAlignment(Qt::AlignCenter);
    ui->twSimbol->setCellWidget(0, 0, img1);
    ui->twSimbol->item(0,1)->setText("CB Close");

    QPixmap pix2(":/img/openCB.png");
    QLabel *img2 = new QLabel;
    img2->setPixmap(pix2);
    img2->setAlignment(Qt::AlignCenter);
    ui->twSimbol->setCellWidget(1, 0, img2);
    ui->twSimbol->item(1,1)->setText("CB Open");

    QPixmap pix3(":/img/invalidCB.png");
    QLabel *img3 = new QLabel;
    img3->setPixmap(pix3);
    img3->setAlignment(Qt::AlignCenter);
    ui->twSimbol->setCellWidget(2, 0, img3);
    ui->twSimbol->item(2,1)->setText("CB Invalid State");

    QPixmap pix4(":/img/offlineCB.png");
    QLabel *img4 = new QLabel;
    img4->setPixmap(pix4);
    img4->setAlignment(Qt::AlignCenter);
    ui->twSimbol->setCellWidget(3, 0, img4);
    ui->twSimbol->item(3,1)->setText("CB offline karena RTU offline");

    QPixmap pix5(":/img/ok.png");
    QLabel *img5 = new QLabel;
    img5->setPixmap(pix5);
    img5->setAlignment(Qt::AlignCenter);
    ui->twSimbol->setCellWidget(4, 0, img5);
    ui->twSimbol->item(4,1)->setText("Peralatan OK / Alarm OFF");

    QPixmap pix6(":/img/nok.png");
    QLabel *img6 = new QLabel;
    img6->setPixmap(pix6);
    img6->setAlignment(Qt::AlignCenter);
    ui->twSimbol->setCellWidget(5, 0, img6);
    ui->twSimbol->item(5,1)->setText("Peralatan NOK / Alarm ON");

    QPixmap pix7(":/img/nokoff.png");
    QLabel *img7 = new QLabel;
    img7->setPixmap(pix7);
    img7->setAlignment(Qt::AlignCenter);
    ui->twSimbol->setCellWidget(6, 0, img7);
    ui->twSimbol->item(6,1)->setText("Alarm Unknown karena RTU offline");

    QLabel *img8 = new QLabel;
    img8->setAlignment(Qt::AlignCenter);
    img8->setStyleSheet("color: rgb(0, 0, 0);");
    img8->setText("XXX");
    ui->twSimbol->setCellWidget(7, 0, img8);
    ui->twSimbol->item(7,1)->setText("Pengukuran Normal");

    QLabel *img9 = new QLabel;
    img9->setAlignment(Qt::AlignCenter);
    img9->setStyleSheet("color: rgb(170, 0, 127);");
    img9->setText("XXX");
    ui->twSimbol->setCellWidget(8, 0, img9);
    ui->twSimbol->item(8,1)->setText("Pengukuran Unknown karena RTU offline");
}

void MainWindow::set_menu(bool abc,bool masuk)
{
    ui->actionDashboard->setEnabled(abc);
    ui->actionPLTS->setEnabled(abc);
    ui->actionPLTMH->setEnabled(abc);
    ui->actionInverter->setEnabled(abc);
    ui->actionLoad->setEnabled(abc);
    ui->actionEvent_List->setEnabled(abc);
    ui->actionNetworking->setEnabled(abc);
    ui->actionChart->setEnabled(abc);
    ui->actionBattery->setEnabled(abc);
    ui->actionAlarm_Event->setEnabled(abc);
    ui->actionStatus_Metering->setEnabled(abc);
    ui->actionSymbol->setEnabled(abc);
    ui->actionProtocol->setEnabled(abc);
    ui->actionLogin->setVisible(!abc);
    ui->pbClose->setVisible(1);
    if (masuk==true)
    {
        ui->pbClose->setVisible(0);
        ui->stackedWidget->setCurrentIndex(12);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

int MainWindow::pingMode(QString ip)
{
    QProcess pingProcess;
    QString exec = "ping";
    QStringList params;
    params << "-n" << "1" << ip;
    pingProcess.start(exec,params,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = pingProcess.readAllStandardOutput();
    QString p_stderr = pingProcess.readAllStandardError();

    if(p_stdout.contains("TTL="))
    {
        return 0;
    }else{
        return 1;
    }
    //int exitCode = QProcess::execute("ping", QStringList()
    //                                 << "-n" << "1"
    //                                 << ip);
    /*if (exitCode==0) {
        // it's alive
    } else {
        // it's dead
    }*/
    //return exitCode;
}

void MainWindow::readDatabaseHystory()
{
    QSettings settings(QDir::currentPath() + "/dbconfig.ini",
                       QSettings::IniFormat );
    settings.beginGroup("SettingsDB");
    connectToDatabase2 = true;
    QString host = settings.value("hostserver").toString();
    QString dbase = settings.value("database").toString();
    QString user = settings.value("user").toString();
    QString pwd = settings.value("password").toString();
    // int port = settings.value("port");
    QString timeout = settings.value("timeout").toString();
    settings.endGroup();

    //set database connecting
    QSqlDatabase db2=QSqlDatabase::addDatabase("QMYSQL");
    QElapsedTimer timer;
    timer.start();

    db2.setHostName(host);
    db2.setPort(3306);
    db2.setDatabaseName("db_scada_temp");
    db2.setUserName(user);
    db2.setPassword(pwd);
    db2.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=5");
    if(!db2.open())
    {
        //clear db
        db2.setConnectOptions();

        QMessageBox::information(this,"Database","HMI Tidak bisa terhubung ke Database");
        ui->pbTrenReak->setEnabled(false);
        ui->pbTrenHis->setEnabled(true);
        ui->comboBox->setEnabled(true);
        realChart = 0;
    }
    else{
        //clear db
        db2.setConnectOptions();


        //mengambil semua data databse
        //data seharian
        QDateTime dateRealHys = QDateTime::fromString("2017-04-17 00:00:00","yyyy-MM-dd hh:mm:ss");

        /*for(int i = 1 ; i < 1000; i++){

            QString data = QString::number(qrand() % ((1000 + 1) - (-1000)) + (-1000));
            QString data2 = QString::number(qrand() % ((1000 + 1) - (-1000)) + (-1000));
            QString data3 = QString::number(qrand() % ((1000 + 1) - (-1000)) + (-1000));
            QString dataqry = "update mon_active_pltmh set timestamps = '"+dateRealHys.toString("yyyy-MM-dd hh:mm:ss")+"' where id = "+QString::number(i)+";";

            QSqlQuery qry;
            qry.prepare(dataqry);
            qry.exec();

            data = QString::number(qrand() % ((1000 + 1) - (-1000)) + (-1000));
            data2 = QString::number(qrand() % ((1000 + 1) - (-1000)) + (-1000));
            data3 = QString::number(qrand() % ((1000 + 1) - (-1000)) + (-1000));
            dataqry = "update mon_active_plts set timestamps = '"+dateRealHys.toString("yyyy-MM-dd hh:mm:ss")+"' where id = "+QString::number(i)+";";

            QSqlQuery qry2;
            qry2.prepare(dataqry);
            qry2.exec();



            dateRealHys = dateRealHys.addSecs(60);
        }*/

        clearDataGraphTranding();
        if(hystorySeharian){
            QSqlQuery qryActive,qryReactive;
            dataActive.clear();
            dataReactive.clear();
            if(dataSelectedHystory == 0){
                //selected pltmh
                qryActive.prepare("select timestamps,ActiveR,ActiveS,ActiveT from mon_active_pltmh order by id asc");
                qryActive.exec();
                while(qryActive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryActive.value(0).toString());
                    dataMonActive.append(qryActive.value(1).toString());
                    dataMonActive.append(qryActive.value(2).toString());
                    dataMonActive.append(qryActive.value(3).toString());
                    dataActive.append(dataMonActive);
                    //QList<QString> dataTempMon = dataRPlts.at(0);
                }

                qryReactive.prepare("select timestamps,ReactiveR,ReactiveS,ReactiveT from mon_reactive_pltmh order by id asc");
                qryReactive.exec();
                while(qryReactive.next()){
                    //jumlah database
                    QList<QString> dataMonReactive;
                    dataMonReactive.append(qryReactive.value(0).toString());
                    dataMonReactive.append(qryReactive.value(1).toString());
                    dataMonReactive.append(qryReactive.value(2).toString());
                    dataMonReactive.append(qryReactive.value(3).toString());
                    dataReactive.append(dataMonReactive);
                }
            }
            else if(dataSelectedHystory == 1){
                //selected plts
                qryActive.prepare("select timestamps,ActiveR,ActiveS,ActiveT from mon_active_plts order by id asc");
                qryActive.exec();
                while(qryActive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryActive.value(0).toString());
                    dataMonActive.append(qryActive.value(1).toString());
                    dataMonActive.append(qryActive.value(2).toString());
                    dataMonActive.append(qryActive.value(3).toString());
                    dataActive.append(dataMonActive);
                    //QList<QString> dataTempMon = dataRPlts.at(0);
                }

                qryReactive.prepare("select timestamps,ReactiveR,ReactiveS,ReactiveT from mon_reactive_plts order by id asc");
                qryReactive.exec();
                while(qryReactive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryReactive.value(0).toString());
                    dataMonActive.append(qryReactive.value(1).toString());
                    dataMonActive.append(qryReactive.value(2).toString());
                    dataMonActive.append(qryReactive.value(3).toString());
                    dataReactive.append(dataMonActive);
                }
            }
            else if(dataSelectedHystory == 2){
                //selected plts
                qryActive.prepare("select timestamps,ActiveR,ActiveS,ActiveT from mon_active_load order by id asc");
                qryActive.exec();
                while(qryActive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryActive.value(0).toString());
                    dataMonActive.append(qryActive.value(1).toString());
                    dataMonActive.append(qryActive.value(2).toString());
                    dataMonActive.append(qryActive.value(3).toString());
                    dataActive.append(dataMonActive);
                    //QList<QString> dataTempMon = dataRPlts.at(0);
                }

                qryReactive.prepare("select timestamps,ReactiveR,ReactiveS,ReactiveT from mon_reactive_load order by id asc");
                qryReactive.exec();
                while(qryReactive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryReactive.value(0).toString());
                    dataMonActive.append(qryReactive.value(1).toString());
                    dataMonActive.append(qryReactive.value(2).toString());
                    dataMonActive.append(qryReactive.value(3).toString());
                    dataReactive.append(dataMonActive);
                }
            }
            else if(dataSelectedHystory == 3){
                //selected plts
                qryActive.prepare("select timestamps,ActiveR,ActiveS,ActiveT from mon_active_inverter order by id asc");
                qryActive.exec();
                while(qryActive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryActive.value(0).toString());
                    dataMonActive.append(qryActive.value(1).toString());
                    dataMonActive.append(qryActive.value(2).toString());
                    dataMonActive.append(qryActive.value(3).toString());
                    dataActive.append(dataMonActive);
                    //QList<QString> dataTempMon = dataRPlts.at(0);
                }

                qryReactive.prepare("select timestamps,ReactiveR,ReactiveS,ReactiveT from mon_reactive_inverter order by id asc");
                qryReactive.exec();
                while(qryReactive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryReactive.value(0).toString());
                    dataMonActive.append(qryReactive.value(1).toString());
                    dataMonActive.append(qryReactive.value(2).toString());
                    dataMonActive.append(qryReactive.value(3).toString());
                    dataReactive.append(dataMonActive);
                }
            }

            if(dataActive.size() >= 1 && dataReactive.size() >= 1){
                //masukan data active
                bool dataMaxA = false,dataMaxR = false;
                int jumlah = 0,jumlahActive = 0,jumlahReactive = 0;
                DOUBLE dataActiveR,dataActiveS,dataActiveT,dataReactiveR,dataReactiveS,dataReactiveT;
                //hys = 2017-04-10 00:00:01
                QString tanggalHys = tanggalHystory + " 00:00:01.000";
                QDateTime dateHys = QDateTime::fromString(tanggalHys,"yyyy-MM-dd hh:mm:ss.zzz");
                for(jumlah = 0;jumlah < 1440;jumlah++){

                    //QMessageBox::information(this,"ini"," tanggal tambahan : "+dateHys.toString()+" tanggal real Aktif : "+ dateRealHys.toString() +" DATA KE "+ QString::number(jumlahActive));

                    QList<QString> dataMonActive = dataActive.at(jumlahActive);
                    QDateTime dateRealHys = QDateTime::fromString(dataMonActive.at(0),"yyyy-MM-dd hh:mm:ss.zzz");
                    while(dateHys > dateRealHys && !dataMaxA){
                        if(dataActive.size() - 1 > jumlahActive){
                            jumlahActive++;
                            dataMonActive = dataActive.at(jumlahActive);
                            dateRealHys = QDateTime::fromString(dataMonActive.at(0),"yyyy-MM-dd hh:mm:ss.zzz");
                        }else{
                            dataMaxA = true;
                        }

                    }
                    dataActiveR = dataMonActive.at(1).toDouble();
                    dataActiveS = dataMonActive.at(2).toDouble();
                    dataActiveT = dataMonActive.at(3).toDouble();

                    QList<QString> dataMonReactive = dataReactive.at(jumlahReactive);
                    QDateTime dateRealHysR = QDateTime::fromString(dataMonReactive.at(0),"yyyy-MM-dd hh:mm:ss.zzz");
                    //QMessageBox::information(this,"ini"," tanggal tambahan : "+dateHys.toString()+" tanggal real Reaktif : "+ dateRealHysR.toString() +" DATA KE "+ QString::number(jumlahReactive));

                    while(dateHys > dateRealHysR && !dataMaxR){
                        if(dataReactive.size() - 1 > jumlahReactive){
                            jumlahReactive++;
                            dataMonReactive = dataReactive.at(jumlahReactive);
                            dateRealHysR = QDateTime::fromString(dataMonReactive.at(0),"yyyy-MM-dd hh:mm:ss.zzz");
                        }else{
                            dataMaxR = true;
                        }
                    }
                    dataReactiveR = dataMonReactive.at(1).toDouble();
                    dataReactiveS = dataMonReactive.at(2).toDouble();
                    dataReactiveT = dataMonReactive.at(3).toDouble();
                    //QMessageBox::information(this,"ini"," tanggal tambahan : "+dateHys.toString()+" tanggal real aktif : "+ dateRealHysR.toString() +" DATA KE "+ QString::number(jumlahReactive) +" DATA ActiveR "+QString::number(dataReactiveR));

                    double smallestActive = std::numeric_limits<double>::max();
                    smallestActive = std::min({dataActiveR,dataActiveS,dataActiveT});

                    double maxActive = std::numeric_limits<double>::max();
                    maxActive = std::max({dataActiveR,dataActiveS,dataActiveT});

                    double smallestReactive = std::numeric_limits<double>::max();
                    smallestReactive = std::min({dataReactiveR,dataReactiveS,dataReactiveT});

                    double maxReactive = std::numeric_limits<double>::max();
                    maxReactive = std::max({dataReactiveR,dataReactiveS,dataReactiveT});

                    if(jumlah == 0){
                        scalaMaxTableUp = maxActive;
                        scalaMinTableUp = smallestActive;
                        scalaMaxTableMidle = maxReactive;
                        scalaMinTableMidle = smallestReactive;
                    }else{


                        if(scalaMaxTableUp < maxActive){
                            scalaMaxTableUp = maxActive;
                        }
                        if(scalaMinTableUp > smallestActive){
                            scalaMinTableUp = smallestActive;
                        }
                        if(scalaMaxTableMidle < maxReactive){
                            scalaMaxTableMidle = maxReactive;
                        }
                        if(scalaMinTableMidle > smallestReactive){
                            scalaMinTableMidle = smallestReactive;
                        }
                    }
                    AddDatabaseHystory(86400,dateHys,dataActiveR,dataActiveS,dataActiveT,dataReactiveR,dataReactiveS,dataReactiveT);
                    dateHys = dateHys.addSecs(60);
                }

            }
            else{
                //datakosong
                QMessageBox::information(this,"Error","Database B Kosong");
            }

        }
        //data dari jam tertentu
        else{
            QSqlQuery qryActive,qryReactive;
            dataActive.clear();
            dataReactive.clear();
            if(dataSelectedHystory == 0){
                //selected pltmh
                QString strActive = "select timestamps,ActiveR,ActiveS,ActiveT from mon_active_pltmh where timestamp >= '"+tanggalHystory+" "+waktuHystory1+"' and timestamp < '"+tanggalHystory+" "+waktuHystory2+"' order by id asc";
                qryActive.prepare(strActive);
                qryActive.exec();
                while(qryActive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryActive.value(0).toString());
                    dataMonActive.append(qryActive.value(1).toString());
                    dataMonActive.append(qryActive.value(2).toString());
                    dataMonActive.append(qryActive.value(3).toString());
                    dataActive.append(dataMonActive);
                    //QList<QString> dataTempMon = dataRPlts.at(0);
                }

                QString strReactive = "select timestamps,ReactiveR,ReactiveS,ReactiveT from mon_reactive_pltmh where timestamp >= '"+tanggalHystory+" "+waktuHystory1+"' and timestamp < '"+tanggalHystory+" "+waktuHystory2+"' order by id asc";
                qryReactive.prepare(strReactive);
                qryReactive.exec();
                while(qryReactive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryReactive.value(0).toString());
                    dataMonActive.append(qryReactive.value(1).toString());
                    dataMonActive.append(qryReactive.value(2).toString());
                    dataMonActive.append(qryReactive.value(3).toString());
                    dataReactive.append(dataMonActive);
                }
            }
            else if(dataSelectedHystory ==1){
                //selected plts
                QString strActive = "select timestamps,ActiveR,ActiveS,ActiveT from mon_active_plts where timestamp >= '"+tanggalHystory+" "+waktuHystory1+"' and timestamp < '"+tanggalHystory+" "+waktuHystory2+"' order by id asc";
                qryActive.prepare(strActive);
                qryActive.exec();
                while(qryActive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryActive.value(0).toString());
                    dataMonActive.append(qryActive.value(1).toString());
                    dataMonActive.append(qryActive.value(2).toString());
                    dataMonActive.append(qryActive.value(3).toString());
                    dataActive.append(dataMonActive);
                    //QList<QString> dataTempMon = dataRPlts.at(0);
                }

                QString strReactive = "select timestamps,ReactiveR,ReactiveS,ReactiveT from mon_reactive_plts where timestamp >= '"+tanggalHystory+" "+waktuHystory1+"' and timestamp < '"+tanggalHystory+" "+waktuHystory2+"' order by id asc";
                qryReactive.prepare(strReactive);
                qryReactive.exec();
                while(qryReactive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryReactive.value(0).toString());
                    dataMonActive.append(qryReactive.value(1).toString());
                    dataMonActive.append(qryReactive.value(2).toString());
                    dataMonActive.append(qryReactive.value(3).toString());
                    dataReactive.append(dataMonActive);
                }
            }
            else if(dataSelectedHystory ==2){
                //selected plts
                QString strActive = "select timestamps,ActiveR,ActiveS,ActiveT from mon_active_load where timestamp >= '"+tanggalHystory+" "+waktuHystory1+"' and timestamp < '"+tanggalHystory+" "+waktuHystory2+"' order by id asc";
                qryActive.prepare(strActive);
                qryActive.exec();
                while(qryActive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryActive.value(0).toString());
                    dataMonActive.append(qryActive.value(1).toString());
                    dataMonActive.append(qryActive.value(2).toString());
                    dataMonActive.append(qryActive.value(3).toString());
                    dataActive.append(dataMonActive);
                    //QList<QString> dataTempMon = dataRPlts.at(0);
                }

                QString strReactive = "select timestamps,ReactiveR,ReactiveS,ReactiveT from mon_reactive_load where timestamp >= '"+tanggalHystory+" "+waktuHystory1+"' and timestamp < '"+tanggalHystory+" "+waktuHystory2+"' order by id asc";
                qryReactive.prepare(strReactive);
                qryReactive.exec();
                while(qryReactive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryReactive.value(0).toString());
                    dataMonActive.append(qryReactive.value(1).toString());
                    dataMonActive.append(qryReactive.value(2).toString());
                    dataMonActive.append(qryReactive.value(3).toString());
                    dataReactive.append(dataMonActive);
                }
            }
            else if(dataSelectedHystory ==3){
                //selected plts
                QString strActive = "select timestamps,ActiveR,ActiveS,ActiveT from mon_active_inverter where timestamp >= '"+tanggalHystory+" "+waktuHystory1+"' and timestamp < '"+tanggalHystory+" "+waktuHystory2+"' order by id asc";
                qryActive.prepare(strActive);
                qryActive.exec();
                while(qryActive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryActive.value(0).toString());
                    dataMonActive.append(qryActive.value(1).toString());
                    dataMonActive.append(qryActive.value(2).toString());
                    dataMonActive.append(qryActive.value(3).toString());
                    dataActive.append(dataMonActive);
                    //QList<QString> dataTempMon = dataRPlts.at(0);
                }

                QString strReactive = "select timestamps,ReactiveR,ReactiveS,ReactiveT from mon_reactive_inverter where timestamp >= '"+tanggalHystory+" "+waktuHystory1+"' and timestamp < '"+tanggalHystory+" "+waktuHystory2+"' order by id asc";
                qryReactive.prepare(strReactive);
                qryReactive.exec();
                while(qryReactive.next()){
                    //jumlah database
                    QList<QString> dataMonActive;
                    dataMonActive.append(qryReactive.value(0).toString());
                    dataMonActive.append(qryReactive.value(1).toString());
                    dataMonActive.append(qryReactive.value(2).toString());
                    dataMonActive.append(qryReactive.value(3).toString());
                    dataReactive.append(dataMonActive);
                }
            }

            //QMessageBox::information(this,"tr",QString::number(dataActive.size()) +"   "+QString::number(dataReactive.size()));
            if(dataActive.size() >= 1 && dataReactive.size() >= 1){
                //masukan data active
                bool dataMaxA = false,dataMaxR = false;
                int jumlahActive = 1,jumlahReactive = 1,jumlah,jumlahLoopInt;
                double jumlahlooping,addSecsM;
                DOUBLE dataActiveR,dataActiveS,dataActiveT,dataReactiveR,dataReactiveS,dataReactiveT;
                //hys = 2017-04-10 00:00:01
                QString tanggalHys = tanggalHystory+" "+waktuHystory1 +".000";
                QDateTime dateHys = QDateTime::fromString(tanggalHys,"yyyy-MM-dd hh:mm:ss.zzz");
                QString tanggalHys2 = tanggalHystory+" "+waktuHystory2 +".000";
                QDateTime dateHys2 = QDateTime::fromString(tanggalHys2,"yyyy-MM-dd hh:mm:ss.zzz");
                //QMessageBox::information(this,"tr",tanggalHys+" to "+tanggalHys2);
                double msDetik = dateHys.secsTo(dateHys2);
                if(msDetik >= 43200){
                    //24 - 12 jam dibagi 60
                    addSecsM = 60;
                    jumlahlooping = msDetik / 60;
                    jumlahLoopInt = qFloor(jumlahlooping);
                }
                else if(msDetik >= 21600 && msDetik < 43200){
                    //12 - 06 jam dibagi 30
                    addSecsM = 30;
                    jumlahlooping = msDetik / 30;
                    jumlahLoopInt = qFloor(jumlahlooping);
                }
                else if(msDetik >= 10800 && msDetik < 21600){
                    //6 - 3 jam dibagi 15
                    addSecsM = 15;
                    jumlahlooping = msDetik / 15;
                    jumlahLoopInt = qFloor(jumlahlooping);
                }
                else if(msDetik >= 5400 && msDetik < 10800){
                    //3 jam - 1.5 jam dibagi 7.5
                    addSecsM = 7;
                    jumlahlooping = msDetik / 7;
                    jumlahLoopInt = qFloor(jumlahlooping);
                }
                else if(msDetik >= 2700 && msDetik < 5400){
                    //1.5 jam - 45 menit dibagi 3,75
                    addSecsM = 3;
                    jumlahlooping = msDetik / 3;
                    jumlahLoopInt = qFloor(jumlahlooping);
                }
                else {
                    //45 - 22 menit dibagi 1.875
                    addSecsM = 1;
                    jumlahlooping = msDetik / 1;
                    jumlahLoopInt = qFloor(jumlahlooping);
                }



                for(jumlah = 0;jumlah < jumlahLoopInt;jumlah++){
                    //QMessageBox::information(this,"tr",QString::number(jumlah)+" to "+QString::number(jumlahLoopInt));
                    QList<QString> dataMonActive = dataActive.at(jumlahActive);
                    QDateTime dateRealHys = QDateTime::fromString(dataMonActive.at(0),"yyyy-MM-dd hh:mm:ss.zzz");
                    while(dateHys > dateRealHys && !dataMaxA){
                        if(dataActive.size() - 1 > jumlahActive){
                            jumlahActive++;
                            dataMonActive = dataActive.at(jumlahActive);
                            dateRealHys = QDateTime::fromString(dataMonActive.at(0),"yyyy-MM-dd hh:mm:ss.zzz");
                        }else{
                            dataMaxA = true;
                        }
                    }
                    dataActiveR = dataMonActive.at(1).toDouble();
                    dataActiveS = dataMonActive.at(2).toDouble();
                    dataActiveT = dataMonActive.at(3).toDouble();

                    QList<QString> dataMonReactive = dataReactive.at(jumlahReactive);
                    QDateTime dateRealHysR = QDateTime::fromString(dataMonReactive.at(0),"yyyy-MM-dd hh:mm:ss.zzz");
                    while(dateHys > dateRealHysR && !dataMaxR){
                        if(dataReactive.size() - 1  > jumlahReactive){
                            jumlahReactive++;
                            dataMonReactive = dataReactive.at(jumlahReactive);
                            dateRealHysR = QDateTime::fromString(dataMonReactive.at(0),"yyyy-MM-dd hh:mm:ss.zzz");
                        }else{
                            dataMaxR = true;
                        }
                    }
                    dataReactiveR = dataMonReactive.at(1).toDouble();
                    dataReactiveS = dataMonReactive.at(2).toDouble();
                    dataReactiveT = dataMonReactive.at(3).toDouble();

                    double smallestActive = std::numeric_limits<double>::max();
                    smallestActive = std::min({dataActiveR,dataActiveS,dataActiveT});

                    double maxActive = std::numeric_limits<double>::max();
                    maxActive = std::max({dataActiveR,dataActiveS,dataActiveT});

                    double smallestReactive = std::numeric_limits<double>::max();
                    smallestReactive = std::min({dataReactiveR,dataReactiveS,dataReactiveT});

                    double maxReactive = std::numeric_limits<double>::max();
                    maxReactive = std::max({dataReactiveR,dataReactiveS,dataReactiveT});

                    if(jumlah == 0){
                        scalaMaxTableUp = maxActive;
                        scalaMinTableUp = smallestActive;
                        scalaMaxTableMidle = maxReactive;
                        scalaMinTableMidle = smallestReactive;
                    }else{


                        if(scalaMaxTableUp < maxActive){
                            scalaMaxTableUp = maxActive;
                        }
                        if(scalaMinTableUp > smallestActive){
                            scalaMinTableUp = smallestActive;
                        }
                        if(scalaMaxTableMidle < maxReactive){
                            scalaMaxTableMidle = maxReactive;
                        }
                        if(scalaMinTableMidle > smallestReactive){
                            scalaMinTableMidle = smallestReactive;
                        }
                    }
                    AddDatabaseHystory(msDetik,dateHys,dataActiveR,dataActiveS,dataActiveT,dataReactiveR,dataReactiveS,dataReactiveT);
                    dateHys = dateHys.addSecs(addSecsM);
                }


            }
            else{
                //datakosong
                QMessageBox::information(this,"Error","Database B Kosong");
            }

        }

    }
    connectToDatabase2 = false;
}

void MainWindow::AddDatabaseHystory(int rangeData,QDateTime time,double ACTIVER,double ACTIVES,double ACTIVET,double REACTIVER,double REACTIVES,double REACTIVET)
{
    ui->PlotIndexTop->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));
    //double data3 = ui->sb1->value(), data4 = ui->sb2->value();
    //realtime trending
    double key = time.toTime_t();
    ui->PlotIndexTop->xAxis->setRange(key, rangeData, Qt::AlignRight);
    ui->PlotIndexTop->graph(0)->addData(key, ACTIVER);//pltsW
    ui->PlotIndexTop->graph(1)->addData(key, ACTIVES);//pltmhW
    ui->PlotIndexTop->graph(2)->addData(key, ACTIVET);//ovInvW

    for(int loop=0;loop<2;loop++){
        ui->PlotIndexTop->graph(loop)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    }
    ui->PlotIndexTop->replot();

    ui->PlotIndexMid->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));
    ui->PlotIndexMid->graph(0)->addData(key, REACTIVER);//pltsW
    ui->PlotIndexMid->graph(1)->addData(key, REACTIVES);//pltmhW
    ui->PlotIndexMid->graph(2)->addData(key, REACTIVET);//ovInvW
    ui->PlotIndexMid->xAxis->setRange(key, rangeData, Qt::AlignRight);
    for(int loop=0;loop<2;loop++){
        ui->PlotIndexMid->graph(loop)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    }
    ui->PlotIndexMid->replot();
}

void MainWindow::clearHystoryPlotActive()
{
    ui->PlotIndexTop->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));
    //double data3 = ui->sb1->value(), data4 = ui->sb2->value();
    //realtime trending
    double key ;
    for(key = 0 ; key < 2400; key++){
        ui->PlotIndexTop->graph(0)->addData(key, 100000);//pltsW
        ui->PlotIndexTop->graph(1)->addData(key, 100000);//pltmhW
        ui->PlotIndexTop->graph(2)->addData(key, 100000);//ovInvW
        ui->PlotIndexTop->xAxis->setRange(key, 2400, Qt::AlignRight);
    }

    for(int loop=0;loop<2;loop++){
        ui->PlotIndexTop->graph(loop)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    }
    ui->PlotIndexTop->replot();
}

void MainWindow::pingHost()
{
    //QMessageBox::information(this,"Information CB","ngeping");
    QString ON="image: url(:/img/ok.png)";
    QString OFF="image: url(:/img/nok.png)";
    //gps 99
    int gps = pingMode("192.168.2.99");
    //rtu 19-21
    //QMessageBox::information(this,"s",QString::number(gps));
    //gps nyala
    if(gps == 0){
        ui->lblindiGPS->setStyleSheet(ON);
        ui->label_123->setText("ONLINE");
        removeRowAlarmTable("Status Network GPS Condition Is Offline");
        if(!NetGps){
            dataout[28] = "Status Network GPS Condition Is Normal";
            InsertDataControl_CB(17);
        }
        NetGps = true;

        //QMessageBox::information(this,"Information CB","GPS NYALA");
    }
    //gps mati
    else{
        ui->lblindiGPS->setStyleSheet(OFF);
        ui->label_123->setText("OFFLINE");
        addRowAlarmTable("Not Available","-","Server Protocol","Status Network GPS Condition Is Offline");
        if(NetGps){
            dataout[28] = "Status Network GPS Condition Is Offline";
            InsertDataControl_CB(17);
        }
        NetGps = false;
        //QMessageBox::information(this,"Information CB","GPS MATI");
    }
    //swtich mati
    if(gps != 0 && databaseRealtime == 0){
        ui->lblindiswitch->setStyleSheet(OFF);
        addRowAlarmTable("Not Available","-","Server Protocol","Status Network SWITCH Condition Is Offline");
        if(NetSwitch){
            dataout[28] = "Status SWITCH Condition Is Offline";
            InsertDataControl_CB(17);
        }
        NetSwitch = false;
        ui->label_246->setText("OFFLINE");
        ui->lblindilWorkstation->setStyleSheet(OFF);
        ui->label_122->setText("OFFLINE");
        addRowAlarmTable("Not Available","-","Server Protocol","Status Network Workstation Condition Is Offline");
        if(NetWorkstation){
            dataout[28] = "Status Network Workstation Condition Is Offline";
            InsertDataControl_CB(17);
        }
        NetWorkstation = false;


    }
    //switch nyala
    else{
        ui->lblindiswitch->setStyleSheet(ON);
        ui->label_246->setText("ONLINE");
        ui->lblindilWorkstation->setStyleSheet(ON);
        ui->label_122->setText("ONLINE");
        removeRowAlarmTable("Status Network Workstation Condition Is Offline");
        if(!NetWorkstation){
            dataout[28] = "Status Network Workstation Condition Is Normal";
            InsertDataControl_CB(17);
        }
        NetWorkstation = true;
        removeRowAlarmTable("Status Network SWITCH Condition Is Offline");
        if(!NetSwitch){
            dataout[28] = "Status SWITCH Condition Is Normal";
            InsertDataControl_CB(17);
        }
        NetSwitch = true;
    }
}




void MainWindow::realtimeDataSlot()
{
    ui->PlotIndexTop->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));
    //double data3 = ui->sb1->value(), data4 = ui->sb2->value();
    //realtime trending
    double time(QTime::currentTime().msec()),now = QDateTime::currentDateTime().toTime_t();
    //ui->PlotIndexTop->yAxis->setRange(data3, data4*(-1));
    // calculate two new dataout points:
    double key = now+(time/1000);
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002)
    {
        try{
            int number= 100;
            int randomValue = qrand() % number;
            // add dataout to lines:
            ui->PlotIndexTop->graph(0)->addData(key, randomValue);//pltsW
            randomValue = qrand() % randomValue;
            ui->PlotIndexTop->graph(1)->addData(key, randomValue);//pltmhW
            randomValue = qrand() % randomValue;
            ui->PlotIndexTop->graph(2)->addData(key, randomValue);//ovInvW
            randomValue = qrand() % randomValue;
            ui->PlotIndexTop->graph(3)->addData(key, randomValue);//ovLoad
            lastPointKey = key;
        }catch(std::exception e){
            ui->PlotIndexTop->graph(0)->addData(key, 0);//pltsW
            ui->PlotIndexTop->graph(1)->addData(key, 0);//pltmhW
            ui->PlotIndexTop->graph(2)->addData(key, 0);//ovInvW
            ui->PlotIndexTop->graph(3)->addData(key, 0);//ovLoad
        }

        //            qDebug()<<QString::number(key,'g',10);
    }
    // make key axis range scroll with the dataout (at a constant range size of 8):
    ui->PlotIndexTop->xAxis->setRange(key, 20, Qt::AlignRight);
    for(int loop=0;loop<4;loop++){
        ui->PlotIndexTop->graph(loop)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    }
    ui->PlotIndexTop->replot();
}

void MainWindow::realtimeDataSlot1()
{
    //realtime trending
    //double data3 = ui->sb1->value(), data4 = ui->sb2->value();
    double time(QTime::currentTime().msec()),now = QDateTime::currentDateTime().toTime_t();
    ui->PlotIndexMid->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));
    //ui->PlotIndexMid->yAxis->setRange(data3, data4*(-1));
    // calculate two new dataout points:
    double key = now+(time/1000);
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002)
    {
        // add dataout to lines:
        ui->PlotIndexMid->graph(0)->addData(key, pltsVar);
        ui->PlotIndexMid->graph(1)->addData(key, pltmhVar);
        ui->PlotIndexMid->graph(2)->addData(key, ovInvVar);
        ui->PlotIndexMid->graph(3)->addData(key, lodVarT);
        lastPointKey = key;
    }
    // make key axis range scroll with the dataout (at a constant range size of 8):
    ui->PlotIndexMid->xAxis->setRange(key, 20, Qt::AlignRight);
    for(int loop=0;loop<4;loop++){
        ui->PlotIndexMid->graph(loop)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    }
    ui->PlotIndexMid->replot();
}

void MainWindow::realtimeDataSlot2()
{
    //realtime trending
    //double data3 = ui->sb1->value(), data4 = ui->sb2->value();
    double time(QTime::currentTime().msec()),now = QDateTime::currentDateTime().toTime_t();
    ui->PlotIndexDown->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));
    //ui->PlotIndexDown->yAxis->setRange(data3, data4*(-1));
    // calculate two new dataout points:
    double key = now+(time/1000);
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002)
    {
        // add dataout to lines:
        ui->PlotIndexDown->graph(0)->addData(key, sunRad);
        lastPointKey = key;
    }
    // make key axis range scroll with the dataout (at a constant range size of 8):
    ui->PlotIndexDown->xAxis->setRange(key, 20, Qt::AlignRight);
    ui->PlotIndexDown->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    ui->PlotIndexDown->replot();
}

void MainWindow::historycalDataSlot()
{
    //set localtime widget to Indonesia
    ui->PlotIndexTop->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));

    QSqlQuery qry;
    QDateTime waktu1[4];
    QString date[6];
    double waktu2, now[2];
    //date[0] = ui->deFirst->text();
    //date[3] = ui->deLast->text();

    qry.prepare("SELECT * FROM monitorin WHERE TimeStamp >= :data1 and TimeStamp <= :data2");
    qry.bindValue(":data1", date[0]);
    qry.bindValue(":data2", date[3]);
    qry.exec();

    //variable for graph
    QString dataValue[qry.size()];
    QVector<QCPGraphData> dataValue1(qry.size()),dataValue2(qry.size()),dataValue3(qry.size()),dataValue4(qry.size());

    //parsing datetime
    date[1] = date[0].section(' ',0,0);
    date[2] = date[0].section(' ',1,1);
    date[0] = date[1]+"T"+date[2];

    date[4] = date[3].section(' ',0,0);
    date[5] = date[3].section(' ',1,1);
    date[3] = date[4]+"T"+date[5];

    //change string format to datetime format
    waktu1[0] = QDateTime::fromString(date[0],"yyyy-MM-ddThh:mm:ss");
    waktu1[1] = QDateTime::fromString(date[3],"yyyy-MM-ddThh:mm:ss");

    //change datetime time_t to time_t format
    now[0] = waktu1[0].toTime_t();
    now[1] = waktu1[1].toTime_t();

    //read status database isActive, isSelect and next data
    if(qry.isActive()){
        if(qry.isSelect()){
            while(qry.next()){

                dataValue[counter] = qry.value(1).toString();
                //change string format to datetime
                waktu1[3] = QDateTime::fromString(dataValue[counter],"yyyy-MM-ddThh:mm:ss");
                //change datetime time_t to time_t format
                waktu2 = waktu1[3].toTime_t();

                //assign time_t format to variable
                dataValue1[counter].key = waktu2;
                dataValue2[counter].key = waktu2;
                dataValue3[counter].key = waktu2;
                dataValue4[counter].key = waktu2;

                //assign value datebase to variable
                dataValue1[counter].value = qry.value(3).toDouble();
                dataValue2[counter].value = qry.value(27).toDouble();
                dataValue3[counter].value = qry.value(37).toDouble();
                dataValue4[counter].value = qry.value(47).toDouble();

                counter++;
                if(counter>=qry.size()){

                    //create graph with variable
                    ui->PlotIndexTop->graph(0)->data()->set(dataValue1);
                    ui->PlotIndexTop->graph(1)->data()->set(dataValue2);
                    ui->PlotIndexTop->graph(2)->data()->set(dataValue3);
                    ui->PlotIndexTop->graph(3)->data()->set(dataValue4);

                    //create xAxis format
                    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                    dateTicker->setDateTimeFormat("hh:mm:ss\ndd.MM.yy");
                    ui->PlotIndexTop->xAxis->setTicker(dateTicker);

                    //create range xAxis
                    ui->PlotIndexTop->xAxis->setRange(now[0],now[1]);

                    //create range yAxis
                    //ui->PlotIndexTop->yAxis->setRange(data3,data4*(-1));

                    //setLine and ScatterStyle
                    for(int loop=0;loop<4;loop++){
                        ui->PlotIndexTop->graph(loop)->setLineStyle(QCPGraph::lsLine);
                        ui->PlotIndexTop->graph(loop)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, sizeDisk));
                    }

                    //replot graph
                    ui->PlotIndexTop->replot();
                    realChart = 2;
                    counter = 0;
                    break;
                }
            }
        }
    }
}

void MainWindow::historycalDataSlot1()
{
    //set localtime widget to Indonesia
    ui->PlotIndexMid->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));

    QSqlQuery qry;
    QDateTime waktu1[4];
    QString date[6];
    double waktu2, now[2];
    //date[0] = ui->deFirst->text();
    //date[3] = ui->deLast->text();

    qry.prepare("SELECT * FROM monitorin WHERE TimeStamp >= :data1 and TimeStamp <= :data2");
    qry.bindValue(":data1", date[0]);
    qry.bindValue(":data2", date[3]);
    qry.exec();

    //variable for graph
    QString dataValue[qry.size()];
    QVector<QCPGraphData> dataValue1(qry.size()),dataValue2(qry.size()),dataValue3(qry.size()),dataValue4(qry.size());

    //parsing datetime
    date[1] = date[0].section(' ',0,0);
    date[2] = date[0].section(' ',1,1);
    date[0] = date[1]+"T"+date[2];

    date[4] = date[3].section(' ',0,0);
    date[5] = date[3].section(' ',1,1);
    date[3] = date[4]+"T"+date[5];

    //change string format to datetime format
    waktu1[0] = QDateTime::fromString(date[0],"yyyy-MM-ddThh:mm:ss");
    waktu1[1] = QDateTime::fromString(date[3],"yyyy-MM-ddThh:mm:ss");

    //change datetime time_t to time_t format
    now[0] = waktu1[0].toTime_t();
    now[1] = waktu1[1].toTime_t();

    //read status database isActive, isSelect and next data
    if(qry.isActive()){
        if(qry.isSelect()){
            while(qry.next()){

                dataValue[counter] = qry.value(1).toString();
                //change string format to datetime
                waktu1[3] = QDateTime::fromString(dataValue[counter],"yyyy-MM-ddThh:mm:ss");
                //change datetime time_t to time_t format
                waktu2 = waktu1[3].toTime_t();

                //assign time_t format to variable
                dataValue1[counter].key = waktu2;
                dataValue2[counter].key = waktu2;
                dataValue3[counter].key = waktu2;
                dataValue4[counter].key = waktu2;

                //assign value datebase to variable
                dataValue1[counter].value = qry.value(4).toDouble();
                dataValue2[counter].value = qry.value(28).toDouble();
                dataValue3[counter].value = qry.value(38).toDouble();
                dataValue4[counter].value = qry.value(48).toDouble();

                counter++;
                if(counter>=qry.size()){

                    //create graph with variable
                    ui->PlotIndexMid->graph(0)->data()->set(dataValue1);
                    ui->PlotIndexMid->graph(1)->data()->set(dataValue2);
                    ui->PlotIndexMid->graph(2)->data()->set(dataValue3);
                    ui->PlotIndexMid->graph(3)->data()->set(dataValue4);

                    //create xAxis format
                    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                    dateTicker->setDateTimeFormat("hh:mm:ss\ndd.MM.yy");
                    ui->PlotIndexMid->xAxis->setTicker(dateTicker);

                    //create range xAxis
                    ui->PlotIndexMid->xAxis->setRange(now[0],now[1]);

                    //create range yAxis
                    //ui->PlotIndexMid->yAxis->setRange(data3,data4*(-1));

                    //setLine and ScatterStyle
                    for(int loop=0;loop<4;loop++){
                        ui->PlotIndexMid->graph(loop)->setLineStyle(QCPGraph::lsLine);
                        ui->PlotIndexMid->graph(loop)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, sizeDisk));
                    }

                    //replot graph
                    ui->PlotIndexMid->replot();
                    realChart = 2;
                    counter = 0;
                    break;
                }
            }
        }
    }
}

void MainWindow::historycalDataSlot2()
{
    //set localtime widget to Indonesia
    ui->PlotIndexDown->setLocale(QLocale(QLocale::Indonesian, QLocale::Indonesia));

    QSqlQuery qry;
    QDateTime waktu1[4];
    QString date[6];
    double waktu2, now[2];
    //date[0] = ui->deFirst->text();
    //date[3] = ui->deLast->text();

    qry.prepare("SELECT * FROM monitorin WHERE TimeStamp >= :data1 and TimeStamp <= :data2");
    qry.bindValue(":data1", date[0]);
    qry.bindValue(":data2", date[3]);
    qry.exec();

    //variable for graph
    QString dataValue[qry.size()];
    QVector<QCPGraphData> dataValue1(qry.size());

    //parsing datetime
    date[1] = date[0].section(' ',0,0);
    date[2] = date[0].section(' ',1,1);
    date[0] = date[1]+"T"+date[2];

    date[4] = date[3].section(' ',0,0);
    date[5] = date[3].section(' ',1,1);
    date[3] = date[4]+"T"+date[5];

    //change string format to datetime format
    waktu1[0] = QDateTime::fromString(date[0],"yyyy-MM-ddThh:mm:ss");
    waktu1[1] = QDateTime::fromString(date[3],"yyyy-MM-ddThh:mm:ss");

    //change datetime time_t to time_t format
    now[0] = waktu1[0].toTime_t();
    now[1] = waktu1[1].toTime_t();

    //read status database isActive, isSelect and next data
    if(qry.isActive()){
        if(qry.isSelect()){
            while(qry.next()){

                dataValue[counter] = qry.value(1).toString();
                //change string format to datetime
                waktu1[3] = QDateTime::fromString(dataValue[counter],"yyyy-MM-ddThh:mm:ss");
                //change datetime time_t to time_t format
                waktu2 = waktu1[3].toTime_t();

                //assign time_t format to variable
                dataValue1[counter].key = waktu2;

                dataValue1[counter].value = qry.value(7).toDouble();

                counter++;
                if(counter>=qry.size()){

                    //create graph with variable
                    ui->PlotIndexDown->graph()->data()->set(dataValue1);

                    //create xAxis format
                    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                    dateTicker->setDateTimeFormat("hh:mm:ss\ndd.MM.yy");
                    ui->PlotIndexDown->xAxis->setTicker(dateTicker);

                    //create range xAxis
                    ui->PlotIndexDown->xAxis->setRange(now[0],now[1]);

                    //create range yAxis
                    //ui->PlotIndexDown->yAxis->setRange(data3,data4*(-1));

                    //setLine and ScatterStyle
                    ui->PlotIndexDown->graph()->setLineStyle(QCPGraph::lsLine);
                    ui->PlotIndexDown->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, sizeDisk));

                    //replot graph
                    ui->PlotIndexDown->replot();
                    realChart = 2;
                    counter = 0;
                    break;
                }
            }
        }
    }
}


void MainWindow::readTren()
{
    if(realChart == 0){
        realtimeDataSlot();
        realtimeDataSlot1();
        realtimeDataSlot2();
    }
    else if(realChart == 1){
        historycalDataSlot();
        historycalDataSlot1();
        historycalDataSlot2();
    }
}

void MainWindow::on_actionDashboard_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->action->setText("Overview");
    GetAllEventList(true,"EventList");

}
void MainWindow::on_actionPLTS_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->action->setText("PLTS");
    GetAllEventList(true,"PLTS");
}
void MainWindow::on_actionPLTMH_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->action->setText("PLTMH");
    GetAllEventList(true,"PLTMH");
}
void MainWindow::on_actionBattery_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->action->setText("Battery");
    GetAllEventList(true,"BATTERY");
}
void MainWindow::on_actionInverter_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->action->setText("Inverter");
    GetAllEventList(true,"INVERTER");
}
void MainWindow::on_actionLoad_triggered()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->action->setText("Load");
    GetAllEventList(true,"LOAD");
}
void MainWindow::on_actionStatus_Metering_triggered()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->action->setText("Status");
}
void MainWindow::on_actionChart_triggered()
{
    ui->stackedWidget->setCurrentIndex(7);
    ui->action->setText("Trending");
}
void MainWindow::on_actionEvent_List_triggered()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->action->setText("Event List");
    GetAllEventList(true,"EventList");
}
void MainWindow::on_actionAlarm_Event_triggered()
{
    ui->stackedWidget->setCurrentIndex(9);
    ui->action->setText("Alarm List");
    GetAllEventList(true,"Alarm");
}
void MainWindow::on_actionSymbol_triggered()
{
    ui->stackedWidget->setCurrentIndex(10);
    ui->action->setText("Simbol");
    imageSymbolPage();
    GetAllEventList(true,"EventList");
}
void MainWindow::on_actionNetworking_triggered()
{
    ui->stackedWidget->setCurrentIndex(11);
    ui->action->setText("Network");
    GetAllEventList(true,"Network");
}
void MainWindow::on_pbOverview1_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->action->setText("Battery");
}
void MainWindow::on_pbOverview2_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->action->setText("Inverter");
}
void MainWindow::on_pbOverview3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->action->setText("PLTS");
}
void MainWindow::on_pbOverview4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->action->setText("PLTMH");
}
void MainWindow::on_pbOverview5_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->action->setText("Load");
}

void MainWindow::on_actionLogin_triggered()
{
    ui->stackedWidget->setCurrentIndex(12);
    ui->action->setText("Login");
}
void MainWindow::on_actionProtocol_triggered()
{
    ui->stackedWidget->setCurrentIndex(13);
    ui->action->setText("Protocol");
}

void MainWindow::on_pbPltsmon_clicked()
{
    static int value = 0;
    if(dataConnectionPLTS == 0){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB PLTS",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB PLTS ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[0] = "1";
                InsertDataControl_CB(value);
                GetAllEventList(true,"PLTS");
                QMessageBox::information(this,"Information CB","CB PLTS Has Been Opened!");

            }
        }
    }
    else if(dataConnectionPLTS == 1){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB PLTS",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB PLTS ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[0] = "2";
                InsertDataControl_CB(value);
                GetAllEventList(true,"PLTS");
                QMessageBox::information(this,"Information CB","CB PLTS Has Been Closed!");

            }
        }
    }



}

void MainWindow::on_pbPltmh_clicked()
{
    static int value=4;
    if(dataConnectionPLMH == 0){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","CB PLTMH Out ?",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB CB PLTMH Out ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[4] = "1";
                InsertDataControl_CB(value);
                GetAllEventList(true,"PLTMH");
                QMessageBox::information(this,"Information CB","CB PLTMH Has Been Opened!");
            }
        }
    }
    else if(dataConnectionPLMH == 1){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","CB PLTMH Out ?",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB CB PLTMH Out ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[4] = "2";
                InsertDataControl_CB(value);
                GetAllEventList(true,"PLTMH");
                QMessageBox::information(this,"Information CB","CB PLTMH Has Been Closed!");
            }
        }
    }
}

void MainWindow::on_pbBattery_clicked()
{
    static int value=5;
    //QMessageBox::information(this,"Information CB",QString::number(dataConnectionBATTERY));
    if(dataConnectionBATTERY == 0){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB Battery",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB Battery ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[5] = "1";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB BATTERY Has Been Opened!");
            }
        }
    }
    else if(dataConnectionBATTERY == 1){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB Battery ",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB Battery ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[5] = "2";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB BATTERY Has Been Closed!");
            }
        }
    }
    //GetAllEventList(true,"BATTERY");
}

void MainWindow::on_pbOvBdimon_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"Question","CB PLTS Out ?",QMessageBox::Open |QMessageBox::Close | QMessageBox::Cancel);
    if(reply == QMessageBox::Open)
    {
        reply2 = QMessageBox::question(this,"Question","Open CB PLTS Out ?",QMessageBox::Yes | QMessageBox::Cancel);
        if(reply2 == QMessageBox::Yes){
            dataout[6] = "1";
            InsertDataControl_CB(6);
            QMessageBox::information(this,"Information CB","CB PLTS Has Been Opened!");
        }
    }
    else if(reply == QMessageBox::Close){
        reply2 = QMessageBox::question(this,"Question","Close CB PLTS Out ?",QMessageBox::Yes | QMessageBox::Cancel);
        if(reply2 == QMessageBox::Yes){
            dataout[6] = "2";
            InsertDataControl_CB(6);
            QMessageBox::information(this,"Information CB","CB PLTS Has Been Closed!");
        }
    }
    GetAllEventList(true,"PLTS");

}

void MainWindow::on_pbInverter1_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    //QMessageBox::information(this,"Information CB",QString::number(dataControlInvAc1));
    if(dataControlInvAc1 == 0){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER R",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB INVERTER R ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[7] = "1";
                InsertDataControl_CB(7);
                QMessageBox::information(this,"Information CB","CB INVERTER R Has Been Opened!");
            }
        }
    }
    else if(dataControlInvAc1 == 1){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER R",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB INVERTER R ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[7] = "2";
                InsertDataControl_CB(7);
                QMessageBox::information(this,"Information CB","CB INVERTER R Has Been Closed!");
            }
        }
    }

    GetAllEventList(true,"INVERTER");

}

void MainWindow::on_pbInverter2_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    if(dataControlInvAc2 == 0){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER S",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB INVERTER S ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[8] = "1";
                InsertDataControl_CB(8);
                QMessageBox::information(this,"Information CB","CB INVERTER S Has Been Opened!");
            }
        }
    }
    else if(dataControlInvAc2 == 1){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER S",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB INVERTER S ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[8] = "2";
                InsertDataControl_CB(8);
                QMessageBox::information(this,"Information CB","CB INVERTER S Has Been Closed!");
            }
        }
    }
    GetAllEventList(true,"INVERTER");
}

void MainWindow::on_pbInverter3_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    //QMessageBox::information(this,"Information CB",QString::number(dataControlInvAc3));

    if(dataControlInvAc3 == 0){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER T ",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB INVERTER T ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[9] = "1";
                InsertDataControl_CB(9);
                QMessageBox::information(this,"Information CB","CB INVERTER T Has Been Opened!");
            }
        }
    }
    else if(dataControlInvAc3 == 1){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER T",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB INVERTER T ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[9] = "2";
                InsertDataControl_CB(9);
                QMessageBox::information(this,"Information CB","CB INVERTER T Has Been Opened!");
            }
        }
    }

    GetAllEventList(true,"INVERTER");
}

void MainWindow::on_pbLoad_clicked()
{
    static int value=10;
    if(dataConnectionLOAD == 0){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","CB LOAD Out ?",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB CB LOAD Out ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[10] = "1";
                InsertDataControl_CB(value);
                GetAllEventList(true,"LOAD");
                QMessageBox::information(this,"Information CB","CB LOAD Has Been Opened!");

            }
        }
    }
    else if(dataConnectionLOAD == 1){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","CB LOAD Out ?",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB CB LOAD Out ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[10] = "2";
                InsertDataControl_CB(value);
                GetAllEventList(true,"LOAD");
                QMessageBox::information(this,"Information CB","CB LOAD Has Been Closed!");
            }
        }
    }
}

void MainWindow::on_pbOkOverview_clicked()
{
    QList<QTableWidgetItem *> items = ui->twEventOverview->findItems(ui->leFilterOverview->text(), Qt::MatchContains);
    if(ui->leFilterOverview->text()!=""){
        for(char a=0;a<100;a++)
            ui->twEventOverview->hideRow(a);
        for(int i = 0; i < items.count(); i++)
            ui->twEventOverview->showRow(items.at(i)->row());
    }
}

void MainWindow::on_pbResetOverview_clicked()
{
    ui->leFilterOverview->setText("");
    for(char a=0;a<100;a++)
        ui->twEventOverview->showRow(a);
}

void MainWindow::on_pbOkPlts_clicked()
{
    QList<QTableWidgetItem *> items = ui->twEventPlts->findItems(ui->leFilterPlts->text(), Qt::MatchContains);
    if(ui->leFilterPlts->text()!=""){
        for(char a=0;a<100;a++)
            ui->twEventPlts->hideRow(a);
        for(int i = 0; i < items.count(); i++)
            ui->twEventPlts->showRow(items.at(i)->row());
    }
}

void MainWindow::on_pbResetPlts_clicked()
{
    ui->leFilterPlts->setText("");
    for(char a=0;a<100;a++)
        ui->twEventPlts->showRow(a);
}

void MainWindow::on_pbOkPltmh_clicked()
{
    QList<QTableWidgetItem *> items = ui->twEventPltmh->findItems(ui->leFilterPltmh->text(), Qt::MatchContains);
    if(ui->leFilterPltmh->text()!=""){
        for(char a=0;a<100;a++)
            ui->twEventPltmh->hideRow(a);
        for(int i = 0; i < items.count(); i++)
            ui->twEventPltmh->showRow(items.at(i)->row());
    }
}

void MainWindow::on_pbResetPltmh_clicked()
{
    ui->leFilterPltmh->setText("");
    for(char a=0;a<100;a++)
        ui->twEventPltmh->showRow(a);
}

void MainWindow::on_pbOkBattery_clicked()
{
    QList<QTableWidgetItem *> items = ui->twEventBattery->findItems(ui->leFilterBattery->text(), Qt::MatchContains);
    if(ui->leFilterBattery->text()!=""){
        for(char a=0;a<100;a++)
            ui->twEventBattery->hideRow(a);
        for(int i = 0; i < items.count(); i++)
            ui->twEventBattery->showRow(items.at(i)->row());
    }
}

void MainWindow::on_pbResetBattery_clicked()
{
    ui->leFilterBattery->setText("");
    for(char a=0;a<100;a++)
        ui->twEventBattery->showRow(a);
}

void MainWindow::on_pbOkInverter_clicked()
{
    QList<QTableWidgetItem *> items = ui->twEventInverter->findItems(ui->leFilterInverter->text(), Qt::MatchContains);
    if(ui->leFilterInverter->text()!=""){
        for(char a=0;a<100;a++)
            ui->twEventInverter->hideRow(a);
        for(int i = 0; i < items.count(); i++)
            ui->twEventInverter->showRow(items.at(i)->row());
    }
}

void MainWindow::on_pbResetInverter_clicked()
{
    ui->leFilterInverter->setText("");
    for(char a=0;a<100;a++)
        ui->twEventInverter->showRow(a);
}

void MainWindow::on_pbOkLoad_clicked()
{
    QList<QTableWidgetItem *> items = ui->twEventLoad->findItems(ui->leFilterLoad->text(), Qt::MatchContains);
    if(ui->leFilterLoad->text()!=""){
        for(char a=0;a<100;a++)
            ui->twEventLoad->hideRow(a);
        for(int i = 0; i < items.count(); i++)
            ui->twEventLoad->showRow(items.at(i)->row());
    }
}

void MainWindow::on_pbResetLoad_clicked()
{
    ui->leFilterLoad->setText("");
    for(char a=0;a<100;a++)
        ui->twEventLoad->showRow(a);
}

void MainWindow::on_pbOkEventList_clicked()
{
    QList<QTableWidgetItem *> items = ui->twEventList->findItems(ui->leFilterEventList->text(), Qt::MatchContains);
    if(ui->leFilterEventList->text()!=""){
        for(char a=0;a<100;a++)
            ui->twEventList->hideRow(a);
        for(int i = 0; i < items.count(); i++)
            ui->twEventList->showRow(items.at(i)->row());
    }
}

void MainWindow::on_pbResetEventList_clicked()
{
    /*ui->leFilterEventList->setText("");
    for(char a=0;a<100;a++)
        ui->twEventList->showRow(a);*/
}

void MainWindow::on_pbOkAlarmList_clicked()
{
    /*QList<QTableWidgetItem *> items = ui->twAlarmList->findItems(ui->leFilterAlarmList->text(), Qt::MatchContains);
    if(ui->leFilterAlarmList->text()!=""){
        for(char a=0;a<100;a++)
            ui->twAlarmList->hideRow(a);
        for(int i = 0; i < items.count(); i++)
            ui->twAlarmList->showRow(items.at(i)->row());
    }*/
}

void MainWindow::on_pbResetAlarmList_clicked()
{
    /*ui->leFilterAlarmList->setText("");
    for(char a=0;a<100;a++)
        ui->twAlarmList->showRow(a);*/
}

void MainWindow::on_pbOkSimbol_clicked()
{
    QList<QTableWidgetItem *> items = ui->twEventList->findItems(ui->leFilterSimbol->text(), Qt::MatchContains);
    if(ui->leFilterSimbol->text()!=""){
        for(char a=0;a<100;a++)
            ui->twEventSimbol->hideRow(a);
        for(int i = 0; i < items.count(); i++)
            ui->twEventSimbol->showRow(items.at(i)->row());
    }
}

void MainWindow::on_pbResetSimbol_clicked()
{
    ui->leFilterSimbol->setText("");
    for(char a=0;a<100;a++)
        ui->twEventSimbol->showRow(a);
}

void MainWindow::on_pbPltmhW_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"PLTMH","Hz SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        double data = ui->lePltmhW->text().toDouble();
        double hasil = 32767*(data/50);
        int var_hasil =static_cast<int>(hasil);
        QString hasilfloat = QString::number(var_hasil);
        reply2 = QMessageBox::question(this,"PLTMH","SetPoint "+ui->lePltmhW->text()+" Hz ("+hasilfloat+") ?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            dataout[11] = QString::number(var_hasil);
            InsertDataSetPoint(0);
            GetAllEventList(true,"PLTMH");
            QMessageBox::information(this,"Information SetPoint","SetPoint "+dataout[11]+" has Been Inserted!");
        }
    }
}

void MainWindow::on_pbPltmhVar_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"PLTMH","VAR SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        reply2 = QMessageBox::question(this,"PLTMH","SetPoint "+ui->lePltmhVar->text()+" VAR?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            dataout[12] = ui->lePltmhVar->text();
            InsertDataSetPoint(1);
            GetAllEventList(true,"PLTMH");
            QMessageBox::information(this,"Information SetPoint","SetPoint "+dataout[12]+" has Been Inserted!");
        }
    }
}


void MainWindow::on_pbDodMin_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"BATTERY","DOD MIN SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        reply2 = QMessageBox::question(this,"BATTERY","SetPoint "+ui->leDodMin->text()+" DOD MIN?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            dataout[13] = ui->leDodMin->text();
            InsertDataSetPoint(2);
            GetAllEventList(true,"BATTERY");
            QMessageBox::information(this,"Information SetPoint","SetPoint DOD MIN "+dataout[13]+" has Been Inserted!");
        }
    }
}


void MainWindow::on_pbDodMax_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"BATTERY","DOD MAX SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        reply2 = QMessageBox::question(this,"BATTERY","SetPoint "+ui->leDodMax->text()+" DOD MAX?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            dataout[14] = ui->leDodMax->text();
            InsertDataSetPoint(3);
            GetAllEventList(true,"BATTERY");
            QMessageBox::information(this,"Information SetPoint","SetPoint DOD MAX "+dataout[14]+" has Been Inserted!");
        }
    }
}


void MainWindow::on_pbSOCMin_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"BATTERY","SOC MIN SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        reply2 = QMessageBox::question(this,"BATTERY","SetPoint "+ui->leSocMin->text()+" SOC MIN?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            dataout[15] = ui->leSocMin->text();
            InsertDataSetPoint(4);
            GetAllEventList(true,"BATTERY");
            QMessageBox::information(this,"Information SetPoint","SetPoint SOC MIN "+dataout[15]+" has Been Inserted!");
        }
    }
}

void MainWindow::on_pbSOCMax_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"BATTERY","SOC MAX SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        reply2 = QMessageBox::question(this,"BATTERY","SetPoint "+ui->leSocMax->text()+" SOC MAX?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            dataout[16] = ui->leSocMax->text();
            InsertDataSetPoint(5);
            GetAllEventList(true,"BATTERY");
            QMessageBox::information(this,"Information SetPoint","SetPoint SOC MAX "+dataout[16]+" has Been Inserted!");
        }
    }
}
void MainWindow::on_pbInvMode_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"Inverter Mode","Ganti Mode ?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        if(ui->spModeInv->value()==0 || ui->spModeInv->value()==1 || ui->spModeInv->value()==2){
            QString Mode;
            if (ui->spModeInv->value()==0 || ui->spModeInv->value()==2)
            {
                Mode="Manual";
            }
            else if (ui->spModeInv->value()==1)
            {
                Mode="Auto";
            }\
            reply2 = QMessageBox::question(this,"Inverter Mode","Ganti Mode "+Mode+" ?",QMessageBox::Ok | QMessageBox::Cancel);
            if (reply2 == QMessageBox::Ok){
                if(ui->spModeInv->text()== "0"){
                    dataout[17] = "0";
                    ui->labelInvMode->setText("Manual");
                    InsertDataSetPoint(6);
                    GetAllEventList(true,"INVERTER");
                    manualInverter = true;
                    QMessageBox::information(this,"Information Mode","Mode Manual has been Activated!");
                }
                else if (ui->spModeInv->text()== "1"){
                    dataout[17] = "1";
                    ui->labelInvMode->setText("Auto");
                    InsertDataSetPoint(6);
                    GetAllEventList(true,"INVERTER");
                    manualInverter = false;
                    QMessageBox::information(this,"Information Mode","Mode Auto has been Activated!");
                }
                else if(ui->spModeInv->text()== "2"){
                    dataout[17] = "2";
                    ui->labelInvMode->setText("Semi Auto");
                    InsertDataSetPoint(6);
                    GetAllEventList(true,"INVERTER");
                    manualInverter = true;
                    QMessageBox::information(this,"Information Mode","Mode Semi Auto has been Activated!");
                }
            }

        }
        else{
            QMessageBox::information(this,"Mode Error","Pilih Mode 2 untuk SemiAuto, 1 untuk Manual dan 0 untuk auto!");
            ui->spModeInv->setValue(0);
        }
    }
}


void MainWindow::on_pbInvW_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"INVERTER","INVERTER W SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+ui->leInvW->text()+" INVERTER W?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            if(ui->spModeInv->text()== "0"){
                dataout[18] = ui->leInvW->text();
                InsertDataSetPoint(7);
                GetAllEventList(true,"INVERTER");
                QMessageBox::information(this,"Information SetPoint","INVERTER W "+dataout[18]+" With Mode 1 has Been Inserted!");
            }
            else if(ui->spModeInv->text()== "2"){
                double datasP =ui->lnPltsOutW->value();
                double tot = ui->leInvW->text().toDouble() - datasP;
                dataout[18] = QString::number(tot);
                InsertDataSetPoint(7);
                GetAllEventList(true,"INVERTER");
                QMessageBox::information(this,"Information SetPoint","INVERTER W "+dataout[18]+"("+ui->leInvW->text()+"-"+ui->leInvW->text()+") With Mode 3 has Been Inserted!");
            }
        }
    }
}

void MainWindow::on_pbInvVar_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"INVERTER","INVERTER VAR SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+ui->leInvVa->text()+" INVERTER VAR?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            dataout[19] = ui->leInvVa->text();
            InsertDataSetPoint(8);
            GetAllEventList(true,"INVERTER");
            QMessageBox::information(this,"Information SetPoint","INVERTER VAR "+dataout[19]+" has Been Inserted!");
        }
    }
}

void MainWindow::on_pbInvM1_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"INVERTER","INVERTER Gradien M1 ?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+ui->leInvM1->text()+" INVERTER Gradien M1?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            dataout[20] = ui->leInvM1->text();
            InsertDataSetPoint(9);
            GetAllEventList(true,"INVERTER");
            QMessageBox::information(this,"Information SetPoint","INVERTER Gradien M1 "+dataout[20]+" has Been Inserted!");
        }
    }
}

void MainWindow::on_pbInvC1_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"INVERTER","INVERTER Gradien C1 ?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+ui->leInvC1->text()+" INVERTER Gradien C1?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            dataout[21] = ui->leInvC1->text();
            InsertDataSetPoint(10);
            GetAllEventList(true,"INVERTER");
            QMessageBox::information(this,"Information SetPoint","INVERTER Gradien C1 "+dataout[21]+" has Been Inserted!");
        }
    }
}

void MainWindow::on_pbInvM2_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"INVERTER","INVERTER Gradien M2 ?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+ui->leInvM2->text()+" INVERTER Gradien M2?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            dataout[22] = ui->leInvM2->text();
            InsertDataSetPoint(11);
            GetAllEventList(true,"INVERTER");
            QMessageBox::information(this,"Information SetPoint","INVERTER Gradien M2 "+dataout[21]+" has Been Inserted!");
        }
    }
}

void MainWindow::on_pbInvC2_clicked()
{
    QMessageBox::StandardButton reply,reply2;
    reply = QMessageBox::question(this,"INVERTER","INVERTER Gradien C2 ?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok){
        reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+ui->leInvC2->text()+" INVERTER Gradien C2?",QMessageBox::Ok | QMessageBox::Cancel);
        if (reply2 == QMessageBox::Ok){
            dataout[23] = ui->leInvC2->text();
            InsertDataSetPoint(12);
            GetAllEventList(true,"INVERTER");
            QMessageBox::information(this,"Information SetPoint","INVERTER Gradien C2 "+dataout[21]+" has Been Inserted!");
        }
    }
}



void MainWindow::on_pbClose_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"LogOut??","Are You Sure ?",QMessageBox::Close | QMessageBox::Cancel);
    if(reply == QMessageBox::Close){
        dataout[27]="0";
        InsertDataStatusLogServer(1);
        emit triggerClose();
    }
}

void MainWindow::on_pbPltsmon_T_clicked()
{
    static int value=3;
    QMessageBox::StandardButton reply,reply2;
    //QMessageBox::information(this,"Information CB",QString::number(dataConnectionPLTS3));
    if(dataConnectionPLTS3 == 0){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER T",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB INVERTER T?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[3] = "1";
                InsertDataControl_CB(value);
                QMessageBox::information(this,"Information CB","Control CB INVERTER T Has Been Opened!");
            }
        }
    }
    else if(dataConnectionPLTS3 == 1){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER T",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB INVERTER T?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[3] = "2";
                InsertDataControl_CB(value);
                QMessageBox::information(this,"Information CB","Control CB INVERTER T Has Been Closed!");
            }
        }
    }
}
void MainWindow::on_pbPltsmon_S_clicked()
{
    static int value=2;
    QMessageBox::StandardButton reply,reply2;
    if(dataConnectionPLTS2 == 0){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER S",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB INVERTER S?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[2] = "1";
                InsertDataControl_CB(value);
                QMessageBox::information(this,"Information CB","Control CB INVERTER S Has Been Opened!");
            }
        }
    }
    else if(dataConnectionPLTS2 == 1){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER S",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB INVERTER S?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[2] = "2";
                InsertDataControl_CB(value);
                QMessageBox::information(this,"Information CB","Control CB INVERTER S Has Been Closed!");
            }
        }
    }
}

void MainWindow::on_pbPltsmon_R_clicked()
{
    static int value=1;
    QMessageBox::StandardButton reply,reply2;
    if(dataConnectionPLTS1 == 0){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER R",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB INVERTER R?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[1] = "1";
                InsertDataControl_CB(value);
                QMessageBox::information(this,"Information CB","Control CB INVERTER R Has Been Opened!");
            }
        }
    }
    else if(dataConnectionPLTS1 == 1){
        reply = QMessageBox::question(this,"Question","Control CB INVERTER R",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB INVERTER R?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[1] = "2";
                InsertDataControl_CB(value);
                QMessageBox::information(this,"Information CB","Control CB INVERTER R Has Been Closed!");
            }
        }
    }
}

void MainWindow::openNewWindow()
{
    //mMyNewWindow = new settings_network(); // Be sure to destroy you window somewhere
    //mMyNewWindow->showNormal();
}



void MainWindow::on_pbPltsControl_clicked()
{
    static int value=0;
    if(dataConnectionPLTS == 0){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB PLTS",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB PLTS ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[0] = "1";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB PLTS Has Been Opened!");

            }
        }
    }
    else if(dataConnectionPLTS == 1){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB PLTS ",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB PLTS ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[0] = "2";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB PLTS Has Been Closed!");

            }
        }
    }

}

void MainWindow::on_pbPltmhControl_clicked()
{
    //ui->Lbl_OvControlInverter->setStyleSheet("image: url(:/img/ControlClose.png)");
    static int value=4;
    if(dataConnectionPLMH == 0){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB PLTMH ",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB PLTMH ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[4] = "1";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB PLTMH Has Been Opened!");

            }
        }
    }
    else if(dataConnectionPLMH == 1){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB PLTMH",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB PLTMH ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[4] = "2";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB PLTMH Has Been Closed!");

            }
        }
    }
}

void MainWindow::on_pbLoadControl_clicked()
{
    static int value=10;
    if(dataConnectionLOAD == 0){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB LOAD",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB LOAD ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[10] = "1";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB LOAD Has Been Opened!");

            }
        }
    }
    else if(dataConnectionLOAD == 1){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB LOAD",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB LOAD ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[10] = "2";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB LOAD Has Been Closed!");
            }
        }
    }
}

void MainWindow::on_pbBatteryControl_clicked()
{
    static int value=5;
    if(dataConnectionBATTERY == 0){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB Battery",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB Battery ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[5] = "1";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB BATTERY Has Been Opened!");
            }
        }
    }
    else if(dataConnectionBATTERY == 1){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB Battery ",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB Battery ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[5] = "2";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB BATTERY Has Been Closed!");
            }
        }
    }

}

void MainWindow::on_pbInverter_clicked()
{
    static int value=6;

    if(dataConnectionINVERTER == 0){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB INVERTER ",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB INVERTER ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[6] = "1";
                InsertDataControl_CB(value);
                GetAllEventList(true,"INVERTER");
                QMessageBox::information(this,"Information CB","Control CB INVERTER Has Been Opened!");
            }
        }
    }
    else if(dataConnectionINVERTER == 1){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB INVERTER ",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB INVERTER ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[6] = "2";
                InsertDataControl_CB(value);
                GetAllEventList(true,"INVERTER");
                QMessageBox::information(this,"Information CB","Control CB INVERTER Has Been Closed!");
            }
        }
    }
}

void MainWindow::on_pbInverterControl_clicked()
{
    static int value=6;
    if(dataConnectionINVERTER == 0){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB INVERTER ",QMessageBox::Open | QMessageBox::Cancel);
        if(reply == QMessageBox::Open)
        {
            reply2 = QMessageBox::question(this,"Question","Open CB INVERTER ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[6] = "1";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB INVERTER Has Been Opened!");
            }
        }
    }
    else if(dataConnectionINVERTER == 1){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Question","Control CB INVERTER ",QMessageBox::Close | QMessageBox::Cancel);
        if(reply == QMessageBox::Close){
            reply2 = QMessageBox::question(this,"Question","Close CB INVERTER ?",QMessageBox::Yes | QMessageBox::Cancel);
            if(reply2 == QMessageBox::Yes){
                dataout[6] = "2";
                InsertDataControl_CB(value);
                GetAllEventList(true,"EventList");
                QMessageBox::information(this,"Information CB","Control CB INVERTER Has Been Closed!");
            }
        }
    }
}


void MainWindow::on_BtnLogin_clicked()
{
    RealtimeDB();
    //login ok
    AuthLogin(ui->TxtuserLog->text(),ui->TxtPassLog->text());

    QString dataConne = "select * from in_list_network";
    QSqlQuery qryrtu;
    QString name,ip,asdu,sts;
    qryrtu.prepare(dataConne);
    qryrtu.exec();
    int loop = 0;
    while(qryrtu.next()){
        //QMessageBox::information(this,"Information CB","CB PLTS Has Been Opened!");
        if(loop == 0){
            name = qryrtu.value(2).toString();
            ip = qryrtu.value(3).toString();
            asdu = qryrtu.value(4).toString();
            sts = qryrtu.value(5).toString();
            ui->name1SNC->setText(name);
            ui->ip1SNC->setText(ip);
            ui->asdu1SNC->setText(asdu);
            if(sts == "1"){
                ui->val1SNC->setText("Used");
                ui->val1SNC->setChecked(true);
            }else{
                ui->val1SNC->setText("Unused");
                ui->val1SNC->setChecked(false);
            }

        }
        else if(loop == 1){
            name = qryrtu.value(2).toString();
            ip = qryrtu.value(3).toString();
            asdu = qryrtu.value(4).toString();
            sts = qryrtu.value(5).toString();
            ui->name2SNC->setText(name);
            ui->ip2SNC->setText(ip);
            ui->asdu2SNC->setText(asdu);
            if(sts == "1"){
                ui->val2SNC->setText("Used");
                ui->val2SNC->setChecked(true);
            }else{
                ui->val2SNC->setText("Unused");
                ui->val2SNC->setChecked(false);
            }
        }
        else if(loop == 2){
            name = qryrtu.value(2).toString();
            ip = qryrtu.value(3).toString();
            asdu = qryrtu.value(4).toString();
            sts = qryrtu.value(5).toString();
            ui->name3SNC->setText(name);
            ui->ip3SNC->setText(ip);
            ui->asdu3SNC->setText(asdu);
            if(sts == "1"){
                ui->val3SNC->setText("Used");
                ui->val3SNC->setChecked(true);
            }else{
                ui->val3SNC->setText("Unused");
                ui->val3SNC->setChecked(false);
            }
        }
        else if(loop == 3){
            name = qryrtu.value(2).toString();
            ip = qryrtu.value(3).toString();
            asdu = qryrtu.value(4).toString();
            sts = qryrtu.value(5).toString();
            ui->name4SNC->setText(name);
            ui->ip4SNC->setText(ip);
            ui->asdu4SNC->setText(asdu);
            if(sts == "1"){
                ui->val4SNC->setText("Used");
                ui->val4SNC->setChecked(true);
            }else{
                ui->val4SNC->setText("Unused");
                ui->val4SNC->setChecked(false);
            }
        }
        loop++;
    }
}

void MainWindow::AuthLogin(QString user, QString Pass)
{
    QSqlQuery qry;
    QString auth;
    qry.exec("select username,password,fullname,authentication from in_login where username='"+user+"' and password='"+Pass+"'");
    if (qry.next())
    {
        auth=qry.value(2).toString();
        if (qry.value(0)!=user)
        {
            QMessageBox::information(this,"Failed","Failed please check your username"+qry.value(3).toString() +"");
        }
        else if (qry.value(1)!=Pass)
        {
            QMessageBox::information(this,"Failed","Failed please check your password"+qry.value(3).toString() +"");
        }
        else
        {

            if(qry.value(3).toString()=="Administrator")
            {
                QMessageBox::information(this,"Succesfully","Welcome "+qry.value(2).toString() +" You're logged in as "+qry.value(3).toString()+"");
                ui->lb_admin->setText(auth);
                set_menu(1,false);
                dataout[27]="1";
                InsertDataStatusLogServer(1);

            }
            else if (qry.value(3).toString()=="Operator")
            {
                QMessageBox::information(this,"Succesfully","Welcome "+qry.value(2).toString() +" You're logged in as "+qry.value(3).toString()+"");
                ui->lb_admin->setText(auth);
                set_menu(1,false);
                // new auth

                ui->wOverview->setEnabled(0);
                ui->wOverview->setEnabled(0);
                dataout[27]="1";
                InsertDataStatusLogServer(1);

            }
            GetAllEventList(true,"EventList");
        }

    }
}

void MainWindow::on_BtnSave_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Confirmation","Save Configuration ?",QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        QSettings settings(QDir::currentPath() + "/dbconfig.ini",
                           QSettings::IniFormat );
        //set setting
        settings.beginGroup("SettingsDB");
        settings.setValue("hostserver",ui->txthost->text());
        settings.setValue("port",ui->txtport->text());
        settings.setValue("database",ui->txtdatabase->text());
        settings.setValue("user",ui->txtusername->text());
        settings.setValue("password",ui->txtpassword->text());
        settings.setValue("timeout",ui->txttimeout->text());
        settings.setValue("Startup",ui->cb_Startup->checkState());
        settings.endGroup();
        settings.sync();
        //  ui->btnPush1->setText("Edit Configuration");
        //    Disabled(0);
        QMessageBox::information(this,"Succesfully","Configuration has been saved!");
    }

}
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    selectedTranding = index;
    clearDataGraphTranding();

    //ui->PlotIndexTop->graph()->data().clear();
    //ui->PlotIndexTop->replot();

    /*if(ui->PlotIndexTop->plotLayout()->elementCount() >= 1){
         ui->PlotIndexTop->plotLayout()->removeAt(0);
         //ui->PlotIndexTop->clearFocus();
         //ui->PlotIndexTop->clearGraphs();
         //ui->PlotIndexTop->clearItems();
         //ui->PlotIndexTop->clearMask();
         //ui->PlotIndexTop->clearPlottables();

     }
     if(ui->PlotIndexMid->plotLayout()->elementCount() >= 1){
         ui->PlotIndexMid->plotLayout()->removeAt(0);
         //ui->PlotIndexMid->plotLayout()->clear();
         //ui->PlotIndexMid->clearFocus();
         //ui->PlotIndexMid->clearGraphs();
         //ui->PlotIndexMid->clearItems();
         //ui->PlotIndexMid->clearMask();
         //ui->PlotIndexMid->clearPlottables();
     }
     if(index == 0){


         //setPlotPltmh();
         //ui->PlotIndexMid->plotLayout()->clear();
         //setTitleTranding("Trending Daya Aktif PLTMH");
         //TrendingMid("ReactiveR","ReactiveS","ReactiveT","Trending Daya Reaktif PLTMH");
     }
     else if(index == 1){

         //setPlotPlts();
         //ui->PlotIndexMid->plotLayout()->clear();
         //setTitleTranding("Trending Daya Aktif PLTS");
         //TrendingMid("ReactiveR","ReactiveS","ReactiveT","Trending Daya Reaktif PLTS");
     }*/
}

void MainWindow::on_leInvW_textChanged(const QString &arg1)
{
    //    if(arg1.toInt() > 29730){
    //        //ui->warning29750->setText(arg1);
    //    }
}

void MainWindow::on_lePltmhW_textChanged(const QString &arg1)
{
    if(arg1.toInt() >= 50){
        ui->w29750->setText("Inverter > 50 Hz");
        ui->w29750->setStyleSheet("QLabel {  color : red; }");
    }
    else{
        ui->w29750->setText("");
    }
}

void MainWindow::on_sliderSunMin_valueChanged(int value)
{
    int valueMax = ui->sliderSunMax->value();
    //normal
    if(value < valueMax){
        ui->PlotIndexDown->yAxis->setRange(value, valueMax);
        ui->PlotIndexDown->replot();
    }
    //error
    else{
        ui->sliderSunMin->setValue(valueMax - 10);
        ui->PlotIndexDown->yAxis->setRange(valueMax - 10 , valueMax);
        ui->PlotIndexDown->replot();
    }
}

void MainWindow::on_sliderSunMax_valueChanged(int value)
{
    int valueMin = ui->sliderSunMin->value();
    //normal
    if(value > valueMin){
        ui->PlotIndexDown->yAxis->setRange(valueMin, value);
        ui->PlotIndexDown->replot();
    }
    //error
    else{
        ui->sliderSunMax->setValue(valueMin + 10);
        ui->PlotIndexDown->yAxis->setRange(valueMin , valueMin + 10  );
        ui->PlotIndexDown->replot();
    }
    //ui->autoScaleRadiasi->setChecked(false);
}

void MainWindow::on_sliderActiveMin_valueChanged(int value)
{
    int valueMax = ui->sliderActiveMax->value();
    //normal
    if(value < valueMax){
        ui->PlotIndexTop->yAxis->setRange(value, valueMax);
        ui->PlotIndexTop->replot();
    }
    //error
    else{
        ui->sliderActiveMin->setValue(valueMax - 10);
        ui->PlotIndexTop->yAxis->setRange(valueMax - 10 , valueMax);
        ui->PlotIndexTop->replot();
    }
}

void MainWindow::on_sliderActiveMax_valueChanged(int value)
{
    int valueMin = ui->sliderActiveMin->value();
    //normal
    if(value > valueMin){
        ui->PlotIndexTop->yAxis->setRange(valueMin, value);
        ui->PlotIndexTop->replot();
    }
    //error
    else{
        ui->sliderActiveMax->setValue(valueMin + 10);
        ui->PlotIndexTop->yAxis->setRange(valueMin , valueMin + 10  );
        ui->PlotIndexTop->replot();
    }
}

void MainWindow::on_sliderReactiveMin_valueChanged(int value)
{
    int valueMax = ui->sliderReactiveMax->value();
    //normal
    if(value < valueMax){
        ui->PlotIndexMid->yAxis->setRange(value, valueMax);
        ui->PlotIndexMid->replot();
    }
    //error
    else{
        ui->sliderReactiveMin->setValue(valueMax - 10);
        ui->PlotIndexMid->yAxis->setRange(valueMax - 10 , valueMax);
        ui->PlotIndexMid->replot();
    }
}

void MainWindow::on_sliderReactiveMax_valueChanged(int value)
{
    int valueMin = ui->sliderReactiveMin->value();
    //normal
    if(value > valueMin){
        ui->PlotIndexMid->yAxis->setRange(valueMin, value);
        ui->PlotIndexMid->replot();
    }
    //error
    else{
        ui->sliderReactiveMax->setValue(valueMin + 10);
        ui->PlotIndexMid->yAxis->setRange(valueMin , valueMin + 10  );
        ui->PlotIndexMid->replot();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString val1,val2,val3,val4;
    if(ui->val1SNC->isChecked()){
        val1 = "1";
    }else{
        val1 = "0";
    }
    if(ui->val2SNC->isChecked()){
        val2 = "1";
    }else{
        val2 = "0";
    }
    if(ui->val3SNC->isChecked()){
        val3 = "1";
    }else{
        val3 = "0";
    }
    if(ui->val4SNC->isChecked()){
        val4 = "1";
    }else{
        val4 = "0";
    }
    QString dataConne1 = "UPDATE in_list_network SET time_hmi='"+ui->txtDateTime->text()+"',name='"+ui->name1SNC->text()+"',ipaddr='"+ui->ip1SNC->text()+"',asdu='"+ui->asdu1SNC->text()+"', Status='"+val1+"' WHERE id=1";
    QString dataConne2 = "UPDATE in_list_network SET time_hmi='"+ui->txtDateTime->text()+"',name='"+ui->name2SNC->text()+"',ipaddr='"+ui->ip2SNC->text()+"',asdu='"+ui->asdu2SNC->text()+"', Status='"+val2+"' WHERE id=2";
    QString dataConne3 = "UPDATE in_list_network SET time_hmi='"+ui->txtDateTime->text()+"',name='"+ui->name3SNC->text()+"',ipaddr='"+ui->ip3SNC->text()+"',asdu='"+ui->asdu3SNC->text()+"', Status='"+val3+"' WHERE id=3";
    QString dataConne4 = "UPDATE in_list_network SET time_hmi='"+ui->txtDateTime->text()+"',name='"+ui->name4SNC->text()+"',ipaddr='"+ui->ip4SNC->text()+"',asdu='"+ui->asdu4SNC->text()+"', Status='"+val4+"' WHERE id=4";
    QSqlQuery qry1,qry2,qry3,qry4;
    qry1.prepare(dataConne1);
    qry2.prepare(dataConne2);
    qry3.prepare(dataConne3);
    qry4.prepare(dataConne4);
    qry1.exec();
    qry2.exec();
    qry3.exec();
    qry4.exec();
    QMessageBox::information(this,"Done","Set Status Done");

}


void MainWindow::on_autoScaleActive_clicked()
{

    ui->sliderActiveMin->setValue(scalaMinTableUp - 50);
    ui->sliderActiveMax->setValue(scalaMaxTableUp + 50);
}

void MainWindow::on_autoScaleReactive_clicked()
{
    ui->sliderReactiveMin->setValue(scalaMinTableMidle - 50);
    ui->sliderReactiveMax->setValue(scalaMaxTableMidle + 50);
}

void MainWindow::on_autoScaleRadiation_clicked()
{
    ui->sliderSunMin->setValue(scalaMinTableDown - 50);
    ui->sliderSunMax->setValue(scalaMaxTableDown + 50);
}

void MainWindow::on_ResetScale_clicked()
{
    //clearHystoryPlotActive();

    //scalaMinTableUp = 0;
    //scalaMaxTableUp = 0;

    //scalaMinTableMidle = 0;
    //scalaMaxTableMidle = 0;

    //scalaMinTableDown = 0;
    //scalaMaxTableDown = 0;
    dataReset = true;
    dataResetF = true;
}

void MainWindow::on_pbExcelOverview_clicked()
{
    //overview
    //tabel alloutcontrol
    //overview
    if(databaseRealtime == 1){
        QDateTime time = QDateTime::currentDateTime();
        QString date = time.toString("ddMMyyyy_hh_mm_ss");
        QString qry = "SELECT 'TIMESTAMPS' , 'OPERATORNAME' , 'OUTVALUE' , 'IOA' , 'INFORMATION' UNION ALL SELECT * from ( select timestamps,operatorname,outvalue,IOA,information from alloutcontrol where not status='LOG_USER' or not status='SERVER_STATUS' order by timestamps desc) a INTO OUTFILE 'C://IEC_104//PRINT//OverView_"+date+".xls' FIELDS TERMINATED BY ',' ENCLOSED BY '\"' LINES TERMINATED BY '\\n';";
        QSqlQuery qry1;
        qry1.prepare(qry);
        qry1.exec();
        QMessageBox::information(this,"Koneksi","Data Request terkirim dan akan disimpan di C://IEC_104//PRINT//OverView_"+date+".XLS");
    }
    else{
        QMessageBox::information(this,"Koneksi","Koneksi Server Offline");
    }


}

void MainWindow::on_pbExcelPlts_clicked()
{
    //plts
    //tabel alloutcontrol filter plts
    if(databaseRealtime == 1){
        QDateTime time = QDateTime::currentDateTime();
        QString date = time.toString("ddMMyyyy_hh_mm_ss");
        QString qry = "SELECT 'TIMESTAMPS' , 'OPERATORNAME' , 'OUTVALUE' , 'IOA' , 'INFORMATION' UNION ALL SELECT * from (select timestamps,operatorname,outvalue,IOA,information from alloutcontrol where status='PLTS' or status='PLTS_STATUS' or status='PLTS1' or status='PLTS2' or status='PLTS3' or status='CBPLTS' or status='RTUPLTS' order by timestamps desc) a INTO OUTFILE 'C://IEC_104//PRINT//PLTS_"+date+".xls' FIELDS TERMINATED BY ',' ENCLOSED BY '\"' LINES TERMINATED BY '\\n';";
        QSqlQuery qry1;
        qry1.prepare(qry);
        qry1.exec();
        QMessageBox::information(this,"Koneksi","Data Request terkirim dan akan disimpan di C://IEC_104//PRINT//PLTS_"+date+".XLS");
    }
    else{
        QMessageBox::information(this,"Koneksi","Koneksi Server Offline");
    }

}

void MainWindow::on_pbExcelPltmh_clicked()
{
    //pltmh
    if(databaseRealtime == 1){
        QDateTime time = QDateTime::currentDateTime();
        QString date = time.toString("ddMMyyy_hh_mm_ss");
        QString qry = "SELECT 'TIMESTAMPS' , 'OPERATORNAME' , 'OUTVALUE' , 'IOA' , 'INFORMATION' UNION ALL SELECT * from (select timestamps,operatorname,outvalue,IOA,information from alloutcontrol where status ='PLTMH' or status='PLTMH_STATUS' or status ='PLTMHW' or status ='PLTMHVAR' or status='CBPLTMH'  or status='RTUPLTMH' order by timestamps desc) a INTO OUTFILE 'C://IEC_104//PRINT//PLTMH_"+date+".xls' FIELDS TERMINATED BY ',' ENCLOSED BY '\"' LINES TERMINATED BY '\\n';";
        QSqlQuery qry1;
        qry1.prepare(qry);
        qry1.exec();
        QMessageBox::information(this,"Koneksi","Data Request terkirim dan akan disimpan di C://IEC_104//PRINT//PLTMH_"+date+".XLS");
    }
    else{
        QMessageBox::information(this,"Koneksi","Koneksi Server Offline");
    }

}

void MainWindow::on_pbExcelBattery_clicked()
{
    //battery
    if(databaseRealtime == 1){
        QDateTime time = QDateTime::currentDateTime();
        QString date = time.toString("ddMMyyyy_hh_mm_ss");
        QString qry = "SELECT 'TIMESTAMPS' , 'OPERATORNAME' , 'OUTVALUE' , 'IOA' , 'INFORMATION' UNION ALL SELECT * from (select timestamps,operatorname,outvalue,IOA,information from alloutcontrol where status ='BATTERY' or status='BATTERY_STATUS' or status ='DODMAX' or status ='DODMIN' or status ='SOCMAX' or status ='SOCMIN' order by timestamps desc) a INTO OUTFILE 'C://IEC_104//PRINT//BATTERY_"+date+".xls' FIELDS TERMINATED BY ',' ENCLOSED BY '\"' LINES TERMINATED BY '\\n';";
        QSqlQuery qry1;
        qry1.prepare(qry);
        qry1.exec();
        QMessageBox::information(this,"Koneksi","Data Request terkirim dan akan disimpan di C://IEC_104//PRINT//BATTERY_"+date+".XLS");
    }
    else{
        QMessageBox::information(this,"Koneksi","Koneksi Server Offline");
    }

}

void MainWindow::on_pbExcelInverter_clicked()
{
    //inverter
    if(databaseRealtime == 1){
        QDateTime time = QDateTime::currentDateTime();
        QString date = time.toString("ddMMyyyy_hh_mm_ss");
        QString qry = "SELECT 'TIMESTAMPS' , 'OPERATORNAME' , 'OUTVALUE' , 'IOA' , 'INFORMATION' UNION ALL SELECT * from (select timestamps,operatorname,outvalue,IOA,information from alloutcontrol where status ='INVERTER' or status='INVERTER_STATUS' or status ='INVERTER1' or status ='INVERTER2' or status ='INVERTER3' or status='CBINVERTER' or status='RTUINVERTER' or status='INVERTERMODE' or status='INVERTERW'  or status='INVERTERVAR'  or status='INVERTERM1' or status='INVERTERC1' or status='INVERTERM2' or status='INVERTERC2' order by timestamps) a INTO OUTFILE 'C://IEC_104//PRINT//Inverter_"+date+".xls' FIELDS TERMINATED BY ',' ENCLOSED BY '\"' LINES TERMINATED BY '\\n';";
        QSqlQuery qry1;
        qry1.prepare(qry);
        qry1.exec();
        QMessageBox::information(this,"Koneksi","Data Request terkirim dan akan disimpan di C://IEC_104//PRINT//INVERTER_"+date+".XLS");
    }
    else{
        QMessageBox::information(this,"Koneksi","Koneksi Server Offline");
    }

}

void MainWindow::on_pbExcelLoad_clicked()
{
    //load
    if(databaseRealtime == 1){
        QDateTime time = QDateTime::currentDateTime();
        QString date = time.toString("ddMMyyyy_hh_mm_ss");
        QString qry = "SELECT 'TIMESTAMPS' , 'OPERATORNAME' , 'OUTVALUE' , 'IOA' , 'INFORMATION' UNION ALL SELECT * from (select timestamps,operatorname,outvalue,IOA,information from alloutcontrol where status ='LOAD' or status='LOAD_STATUS' or status='CBLOAD' or status='RTULOAD' or status='LOADW'  or status='LOADVAR' order by timestamps desc) a INTO OUTFILE 'C://IEC_104//PRINT//LOAD_"+date+".xls' FIELDS TERMINATED BY ',' ENCLOSED BY '\"' LINES TERMINATED BY '\\n';";
        QSqlQuery qry1;
        qry1.prepare(qry);
        qry1.exec();
        QMessageBox::information(this,"Koneksi","Data Request terkirim dan akan disimpan di C://IEC_104//PRINT//LOAD_"+date+".XLS");
    }
    else{
        QMessageBox::information(this,"Koneksi","Koneksi Server Offline");
    }

}

void MainWindow::on_pbExcelEventList_clicked()
{
    //eventlist
    if(databaseRealtime == 1){
        QDateTime time = QDateTime::currentDateTime();
        QString date = time.toString("ddMMyyyy_hh_mm_ss");
        QString qry = "SELECT 'TIMESTAMPS' , 'OPERATORNAME' , 'OUTVALUE' , 'IOA' , 'INFORMATION' UNION ALL SELECT * from (select timestamps,operatorname,outvalue,IOA,information from alloutcontrol where not status='LOG_USER' or not status='SERVER_STATUS' order by timestamps desc) a INTO OUTFILE 'C://IEC_104//PRINT//Event_"+date+".xls' FIELDS TERMINATED BY ',' ENCLOSED BY '\"' LINES TERMINATED BY '\\n';";
        QSqlQuery qry1;
        qry1.prepare(qry);
        qry1.exec();
        QMessageBox::information(this,"Koneksi","Data Request terkirim dan akan disimpan di C://IEC_104//PRINT//EVENT_"+date+".XLS");
    }
    else{
        QMessageBox::information(this,"Koneksi","Koneksi Server Offline");
    }

}

void MainWindow::openDateWindow()
{
    tanggal = new Date();
    //int dataExec = tanggal->exec();
    //QMessageBox::information(this,"Information CB",QString::number(dataExec));

    if(tanggal->exec() == 0){
        realChart = tanggal->valueReturn();
        //QMessageBox::information(this,"Information CB",QString::number(realChart));
        //jika memilih ok
        if(databaseRealtime == 1){
            if(realChart == 1){
                countStatusWaiting = 0;
                ui->pbTrenReak->setEnabled(true);
                ui->pbTrenHis->setEnabled(false);
                //memilih seharian waktuHystory1,waktuHystory2,tanggalHystory;
                tanggalHystory = tanggal->tanggal();
                //QMessageBox::information(this,"da",tanggalHystory);
                dataSelectedHystory = tanggal->pilihan();
                ui->comboBox->setCurrentIndex(dataSelectedHystory);
                ui->comboBox->setEnabled(false);
                dataout[28] = tanggalHystory;
                //memasukan data id dan informasi terakhir
                QString dataR =  "select id,information from alloutcontrol where Status = 'STATUS_ARCHIVE' order by id desc limit 1";
                QSqlQuery qry1;
                qry1.prepare(dataR);
                qry1.exec();
                if(qry1.next()){
                    idHystory = qry1.value(0).toInt();
                    dataHystory = qry1.value(1).toString();

                }
                else{
                    idHystory = 0;
                    dataHystory = "";
                }
                InsertDataControl_CB(16);
                //QMessageBox::information(this,"data",QString::(idHystory));
                if(tanggal->seharianReturn()){
                    hystorySeharian = true;
                    QMessageBox::information(this,"data","Request Data pada tanggal "+tanggalHystory+" selama seharian");
                }
                //memilih range data tertentu
                else{

                    hystorySeharian = false;
                    waktuHystory1 = tanggal->data1();
                    waktuHystory2 = tanggal->date2();
                    QMessageBox::information(this,"data","Request Data pada tanggal "+tanggalHystory+" dari jam "+waktuHystory1+" - "+waktuHystory2);
                }
                dataSuccesRequestHystory = 0;
                countDataHystory = 0;
            }
            //jika memilih cancel atau close
            else{
                countStatusWaiting = 0;
                ui->pbTrenReak->setEnabled(false);
                ui->pbTrenHis->setEnabled(true);
                //kondisi jika close lewat menu
                realChart = 0;
            }
        }else{
            countStatusWaiting = 0;
            QMessageBox::information(this,"Koneksi","Koneksi Server Offline");
        }
    }

}

void MainWindow::on_pbMeteringInvMode_clicked()
{
    //QString inputUser = searchRowTable(2,2);
    QString inputUser = ui->inputMMode->text();

    bool ok;
    double data = inputUser.toDouble(&ok);
    QMessageBox::information(this,"",QString::number(data));
    if(ok){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"Inverter Mode","Ganti Mode ?",QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok){
            if(data==0 || data==1){
                QString Mode;
                if (data==0)
                {
                    Mode="Manual";
                }
                else if (data==1)
                {
                    Mode="Auto";
                }\
                reply2 = QMessageBox::question(this,"Inverter Mode","Ganti Mode "+Mode+" ?",QMessageBox::Ok | QMessageBox::Cancel);
                if (reply2 == QMessageBox::Ok){
                    if(data == 0){
                        dataout[17] = "0";
                        ui->labelInvMode->setText("Manual");
                        InsertDataSetPoint(6);
                        GetAllEventList(true,"INVERTER");
                        manualInverter = true;
                        QMessageBox::information(this,"Information Mode","Mode Manual has been Activated!");
                    }
                    else if (data == 1){
                        dataout[17] = "1";
                        ui->labelInvMode->setText("Auto");
                        InsertDataSetPoint(6);
                        GetAllEventList(true,"INVERTER");
                        manualInverter = false;
                        QMessageBox::information(this,"Information Mode","Mode Auto has been Activated!");
                    }
                }

            }
            else{
                QMessageBox::information(this,"Mode Error","Pilih Mode 1 untuk Manual dan 0 untuk auto!");
                ui->spModeInv->setValue(0);
            }
        }
    }else{
        QMessageBox::information(this,"Information SetPoint","Data Harus Number!");
    }
}

void MainWindow::on_pbMeteringPltmhW_clicked()
{

    QMessageBox::StandardButton reply,reply2;
    //
    //QString inputUser = searchRowTable(2,2);//27,2
    QString inputUser = ui->inputMPLTMHFreq->text();
    bool ok;
    double data = inputUser.toDouble(&ok);
    QMessageBox::information(this,"",QString::number(data));
    if(ok){
        reply = QMessageBox::question(this,"PLTMH","Hz SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok){

            double hasil = 32767*(data/50);
            int var_hasil =static_cast<int>(hasil);
            QString hasilfloat = QString::number(var_hasil);
            reply2 = QMessageBox::question(this,"PLTMH","SetPoint "+ inputUser+" Hz ("+hasilfloat+") ?",QMessageBox::Ok | QMessageBox::Cancel);
            if (reply2 == QMessageBox::Ok){
                dataout[11] = QString::number(var_hasil);
                InsertDataSetPoint(0);
                GetAllEventList(true,"PLTMH");
                QMessageBox::information(this,"Information SetPoint","SetPoint "+dataout[11]+" has Been Inserted!");
            }
        }
    }else{
        QMessageBox::information(this,"Information SetPoint","Data Harus Number!");
    }
}

void MainWindow::on_pbMeteringPltmhVar_clicked()
{
    //
    //QString inputUser = searchRowTable(28,2);
    QString inputUser = ui->inputMPLTMHVar->text();
    bool ok;
    double data = inputUser.toDouble(&ok);
    QMessageBox::information(this,"",QString::number(data));
    if(ok){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"PLTMH","VAR SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok){
            reply2 = QMessageBox::question(this,"PLTMH","SetPoint "+inputUser+" VAR?",QMessageBox::Ok | QMessageBox::Cancel);
            if (reply2 == QMessageBox::Ok){
                dataout[12] = QString::number(data);
                InsertDataSetPoint(1);
                GetAllEventList(true,"PLTMH");
                QMessageBox::information(this,"Information SetPoint","SetPoint "+dataout[12]+" has Been Inserted!");
            }
        }
    }else{
        QMessageBox::information(this,"Information SetPoint","Data Harus Number!");
    }
}

void MainWindow::on_pbMeteringInvW_clicked()
{
    //QString inputUser = searchRowTable(27,2);
    QString inputUser = ui->inputMWatt->text();
    bool ok;
    double data = inputUser.toDouble(&ok);
    QMessageBox::information(this,"",QString::number(data));
    if(ok){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"INVERTER","INVERTER W SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok){
            reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+inputUser+" INVERTER W?",QMessageBox::Ok | QMessageBox::Cancel);
            if (reply2 == QMessageBox::Ok){
                dataout[18] = inputUser;
                InsertDataSetPoint(7);
                GetAllEventList(true,"INVERTER");
                QMessageBox::information(this,"Information SetPoint","INVERTER W "+inputUser+" has Been Inserted!");
            }
        }
    }else{
        QMessageBox::information(this,"Information SetPoint","Data Harus Number!");
    }
}

void MainWindow::on_pbMeteringInvVar_clicked()
{
    //QString inputUser = searchRowTable(28,2);
    QString inputUser = ui->inputMVar->text();
    bool ok;
    double data = inputUser.toDouble(&ok);
    QMessageBox::information(this,"",QString::number(data));
    if(ok){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"INVERTER","INVERTER VAR SetPoint?",QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok){
            reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+inputUser+" INVERTER VAR?",QMessageBox::Ok | QMessageBox::Cancel);
            if (reply2 == QMessageBox::Ok){
                dataout[19] = inputUser;
                InsertDataSetPoint(8);
                GetAllEventList(true,"INVERTER");
                QMessageBox::information(this,"Information SetPoint","INVERTER VAR "+dataout[19]+" has Been Inserted!");
            }
        }
    }else{
        QMessageBox::information(this,"Information SetPoint","Data Harus Number!");
    }
}

void MainWindow::on_pbMeteringInvM1_clicked()
{
    //QString inputUser = searchRowTable(29,2);
    QString inputUser = ui->inputMGDWatt->text();
    bool ok;
    double data = inputUser.toDouble(&ok);
    QMessageBox::information(this,"",QString::number(data));
    if(ok){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"INVERTER","INVERTER Gradien M1 ?",QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok){
            reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+inputUser+" INVERTER Gradien M1?",QMessageBox::Ok | QMessageBox::Cancel);
            if (reply2 == QMessageBox::Ok){
                dataout[20] = inputUser;
                InsertDataSetPoint(9);
                GetAllEventList(true,"INVERTER");
                QMessageBox::information(this,"Information SetPoint","INVERTER Gradien M1 "+inputUser+" has Been Inserted!");
            }
        }
    }else{
        QMessageBox::information(this,"Information SetPoint","Data Harus Number!");
    }
}

void MainWindow::on_pbMeteringInvC1_clicked()
{
    //QString inputUser = searchRowTable(30,2);
    QString inputUser = ui->inputMKDW->text();
    bool ok;
    double data = inputUser.toDouble(&ok);
    QMessageBox::information(this,"",QString::number(data));
    if(ok){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"INVERTER","INVERTER Gradien C1 ?",QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok){
            reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+inputUser+" INVERTER Gradien C1?",QMessageBox::Ok | QMessageBox::Cancel);
            if (reply2 == QMessageBox::Ok){
                dataout[21] = inputUser;
                InsertDataSetPoint(10);
                GetAllEventList(true,"INVERTER");
                QMessageBox::information(this,"Information SetPoint","INVERTER Gradien C1 "+dataout[21]+" has Been Inserted!");
            }
        }
    }else{
        QMessageBox::information(this,"Information SetPoint","Data Harus Number!");
    }
}

void MainWindow::on_pbMeteringInvM2_clicked()
{
    //QString inputUser = searchRowTable(31,2);
    QString inputUser = ui->inputMGDFrekuensi->text();
    bool ok;
    double data = inputUser.toDouble(&ok);
    QMessageBox::information(this,"",QString::number(data));
    if(ok){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"INVERTER","INVERTER Gradien M2 ?",QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok){
            reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+inputUser+" INVERTER Gradien M2?",QMessageBox::Ok | QMessageBox::Cancel);
            if (reply2 == QMessageBox::Ok){
                dataout[22] = inputUser;
                InsertDataSetPoint(11);
                GetAllEventList(true,"INVERTER");
                QMessageBox::information(this,"Information SetPoint","INVERTER Gradien M2 "+inputUser+" has Been Inserted!");
            }
        }
    }else{
        QMessageBox::information(this,"Information SetPoint","Data Harus Number!");
    }
}

void MainWindow::on_pbMeteringInvC2_clicked()
{
    //QString inputUser = searchRowTable(32,2);
    QString inputUser = ui->inputMKDFrekuensi->text();
    bool ok;
    double data = inputUser.toDouble(&ok);
    QMessageBox::information(this,"",QString::number(data));
    if(ok){
        QMessageBox::StandardButton reply,reply2;
        reply = QMessageBox::question(this,"INVERTER","INVERTER Gradien C2 ?",QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok){
            reply2 = QMessageBox::question(this,"INVERTER","SetPoint "+inputUser+" INVERTER Gradien C2?",QMessageBox::Ok | QMessageBox::Cancel);
            if (reply2 == QMessageBox::Ok){
                dataout[23] = inputUser;
                InsertDataSetPoint(12);
                GetAllEventList(true,"INVERTER");
                QMessageBox::information(this,"Information SetPoint","INVERTER Gradien C2 "+inputUser+" has Been Inserted!");
            }
        }
    }else{
        QMessageBox::information(this,"Information SetPoint","Data Harus Number!");
    }
}

void MainWindow::AutoStartupOnWindows()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                       QSettings::NativeFormat);
    if (ui->cb_Startup->checkState())
    {

        settings.setValue("HMI_IEC_104",
                          QCoreApplication::applicationFilePath().replace('/', '\\'));
    }
    else
    {
        settings.remove("HMI_IEC_104");
    }

}

void MainWindow::value(int k)
{
    if (ui->pb_sw->text()=="+")
    {
        ui->leInvW->setText(QString::number(k));
    }
    else if (ui->pb_sw->text()=="-")
    {
        ui->leInvW->setText(QString::number(-k));
    }
}

void MainWindow::value2(int k)
{
    if (ui->pbsw2->text()=="+")
    {
        ui->leInvVa->setText(QString::number(k));
    }
    else if (ui->pbsw2->text()=="-")
    {
        ui->leInvVa->setText(QString::number(-k));
    }
}

void MainWindow::on_pb_sw_clicked()
{
    if (ui->pb_sw->text()=="+")
    {
        ui->pb_sw->setText("-");
    }
    else if (ui->pb_sw->text()=="-")
    {
        ui->pb_sw->setText("+");
    }
}

void MainWindow::on_pbsw2_clicked()
{
    if (ui->pbsw2->text()=="+")
    {
        ui->pbsw2->setText("-");
    }
    else if (ui->pbsw2->text()=="-")
    {
        ui->pbsw2->setText("+");
    }
}
