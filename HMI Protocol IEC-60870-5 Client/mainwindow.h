#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QList>
#include "drawing.h"
#include "date.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void triggerClose();

private slots:
    void Index();
    void Indexplts();
    void waktu();
    void readTren();
    void ReadAllDataFrmDB();
    void on_actionDashboard_triggered();
    void on_actionAlarm_Event_triggered();
    void on_actionChart_triggered();
    void on_actionLoad_triggered();
    void on_actionBattery_triggered();
    void on_actionPLTS_triggered();
    void on_actionPLTMH_triggered();
    void on_actionStatus_Metering_triggered();
    void on_actionSymbol_triggered();
    void on_actionInverter_triggered();
    void on_actionEvent_List_triggered();
    void on_pbOverview1_clicked();
    void on_pbOverview2_clicked();
    void on_pbOverview4_clicked();
    void on_pbBattery_clicked();

    void on_pbPltmh_clicked();
    void on_pbInverter3_clicked();
    void on_pbInverter2_clicked();
    void on_pbInverter1_clicked();
    void on_pbLoad_clicked();
    void on_pbOverview3_clicked();
    void on_pbOverview5_clicked();
    void on_pbOkOverview_clicked();
    void on_pbResetOverview_clicked();
    void on_pbOkPlts_clicked();
    void on_pbResetPlts_clicked();
    void on_pbOkPltmh_clicked();
    void on_pbResetPltmh_clicked();
    void on_pbOkBattery_clicked();
    void on_pbResetBattery_clicked();
    void on_pbOkInverter_clicked();
    void on_pbResetInverter_clicked();
    void on_pbOkLoad_clicked();
    void on_pbResetLoad_clicked();
    void on_pbOkEventList_clicked();
    void on_pbResetEventList_clicked();
    void on_pbOkAlarmList_clicked();
    void on_pbResetAlarmList_clicked();
    void on_pbOkSimbol_clicked();
    void on_pbResetSimbol_clicked();
    void on_pbPltmhW_clicked();
    void on_pbPltmhVar_clicked();
    void on_pbInvW_clicked();
    void on_pbInvVar_clicked();
    void on_pbInvM1_clicked();
    void on_pbInvC1_clicked();
    void on_pbInvM2_clicked();
    void on_pbInvC2_clicked();
    void on_pbInvMode_clicked();
    void on_pbTrenHis_clicked();
    void on_pbTrenReak_clicked();
    void on_pbClose_clicked();
    void on_actionNetworking_triggered();
    void on_pbTrenOk_clicked();
    void on_pbTrenReset_clicked();
    void on_pbPltsmon_clicked();
    void on_pbOvBdimon_clicked();
    void on_pbPltsmon_T_clicked();
    void on_pbPltsmon_S_clicked();
    void on_pbPltsmon_R_clicked();
    void on_actionLogin_triggered();
    void openNewWindow();

    void RealtimeDB();

    void on_pbPltsControl_clicked();

    void on_pbPltmhControl_clicked();

    void on_pbLoadControl_clicked();

    void on_pbBatteryControl_clicked();

    void on_pbInverter_clicked();

    void on_pbInverterControl_clicked();

    void on_BtnLogin_clicked();
    void AuthLogin(QString user,QString Pass);

    void on_BtnSave_clicked();

    void on_pbDodMax_clicked();

    void on_pbDodMin_clicked();

    void on_pbSOCMax_clicked();

    void on_pbSOCMin_clicked();

    void on_actionProtocol_triggered();

    void on_comboBox_currentIndexChanged(int index);

    void on_leInvW_textChanged(const QString &arg1);

    void on_lePltmhW_textChanged(const QString &arg1);

    void on_sliderSunMin_valueChanged(int value);

    void on_sliderSunMax_valueChanged(int value);

    void on_sliderActiveMin_valueChanged(int value);

    void on_sliderActiveMax_valueChanged(int value);

    void on_sliderReactiveMin_valueChanged(int value);

    void on_sliderReactiveMax_valueChanged(int value);

    void on_pushButton_clicked();

    void on_autoScaleActive_clicked();

    void on_autoScaleReactive_clicked();

    void on_autoScaleRadiation_clicked();

    void on_ResetScale_clicked();

    void on_pbExcelOverview_clicked();

    void on_pbExcelPlts_clicked();

    void on_pbExcelPltmh_clicked();

    void on_pbExcelBattery_clicked();

    void on_pbExcelInverter_clicked();

    void on_pbExcelLoad_clicked();

    void on_pbExcelEventList_clicked();

    void openDateWindow();
    void pingHost();

    void on_pbMeteringInvMode_clicked();

    void on_pbMeteringPltmhW_clicked();

    void on_pbMeteringPltmhVar_clicked();

    void on_pbMeteringInvW_clicked();

    void on_pbMeteringInvVar_clicked();

    void on_pbMeteringInvM1_clicked();

    void on_pbMeteringInvC1_clicked();

    void on_pbMeteringInvM2_clicked();

    void on_pbMeteringInvC2_clicked();

    void AutoStartupOnWindows();
    void value(int k);
    void value2(int k);
    void on_pb_sw_clicked();

    void on_pbsw2_clicked();

private:
    Ui::MainWindow *ui;
    Date *tanggal;

    //settings_network *mMyNewWindow;
    bool dataReset = false,dataResetF = false;
    QTimer *refresh,*timer,*timerPing;
    QList<QList<QString>> dataReactive;
    QList<QList<QString>> dataActive;
    int selectedTranding,countStatusWaiting;
    double nilaiVarAuto,dataarrow;
    double scalaMaxTableUp,scalaMaxTableMidle,scalaMaxTableDown,scalaMinTableUp,scalaMinTableMidle,scalaMinTableDown;
    double InvAT,lodVarT,lodAT,ovInvVar,ovInvW,ovLoad,pltsW,pltsVar,pltmhW,pltmhVar,sunRad;
    int databaseRealtime,dataSelectedHystory;
    //true = manual
    //false = auto
    bool manualInverter;
    bool NetSwitch,NetGps,NetPltmh,NetPlts,NetWorkstation,NetServer,NetLoad,NetInverter;
    bool dataCurveLast;
    //dataConnection = 0 = close
    //dataConnection = 1 = open
    //dataConnection = 2 = offline
    int dataConnectionPLTS,dataConnectionPLTS1,dataConnectionPLTS2,dataConnectionPLTS3,dataPLTSStatus;
    int dataConnectionPLMH;
    int dataConnectionLOAD;
    int dataConnectionINVERTER;
    int dataConnectionBATTERY;
    bool hystorySeharian,connectToDatabase2;
    int dataSuccesRequestHystory;
    QString waktuHystory1,waktuHystory2,tanggalHystory;
    //data alarm
    // dataAlarm = 1 = On
    // dataAlarm = 2 = Off
    int dataAlarmPLTS,dataAlarmPLTS1,dataAlarmPLTS2,dataAlarmPLTS3,dataFaultAlarmPLTS1,dataFaultAlarmPLTS2,dataFaultAlarmPLTS3;
    int dataAlarmPLTMH1;
    int dataAlarmPLTMH2;
    int dataAlarmLoad1;
    int dataAlarmLoad2;
    int countManual = 0;
    int dataAlarmInverter,dataAlarmInverterRtu,dataAlarmInvAc1,dataAlarmInvAc2,dataAlarmInvAc3,dataAlarmInvDc1,dataAlarmInvDc2,dataAlarmInvDc3,dataAlarmInvDc4,dataAlarmInvDc4p,dataAlarmInvDc5,dataAlarmInvDc6,dataAlarmInvDc7,dataAlarmInvDc8,dataAlarmInvDc9,dataAlarmInvDc10,dataAlarmInvDc11,dataAlarmInvDc12,dataAlarmInvDc13,dataControlInvAc1,dataControlInvAc2,dataControlInvAc3,dataControlInvAc4,dataAlarmInvFaultAC1,dataAlarmInvFaultAC2,dataAlarmInvFaultAC3,dataAlarmInvFaultAC4,dataAlarmInvFaultAC5,dataAlarmInvFaultAC6,dataKomInvSPV1,dataKomInvSPV2,dataKomInvSPV3,dataKomInvSPV4,dataKomInvSPV5,dataKomInvSPV6;
    int dataAlarmBattery,dataAlarmBatterySPV,dataKomunikasiRabbitBattery,rtuConditionInverter;
    int faultVoltage1,faultVoltage2,faultVoltage3,faultVoltage4,faultVoltage5,faultVoltage6,faultVoltage7;
    int overTemp1,overTemp2,overTemp3,overTemp4,overTemp5,overTemp6,overTemp7;
    int idHystory;
    QString dataHystory;
    int statuscb;

    QString dataout[30],timeHMI,no[16],dataGti[10];
    QString valueData[26],valueDataAni[20],valueDataBtr[112],valueDataMon[54];
    char a=0,b=0,c=0,item1=0,item2=0,item3=0,item4=0,item5=0;
    int realChart, counter = 0, sizeDisk = 4,countDataHystory;
    void init();
    void InsertDataControl_CB(int ParamControl);
    void InsertDataSetPoint(int Param);
    void InsertDataStatusLogServer(int Params);
    void imageSymbolPage();
    void set_menu(bool abc, bool masuk);
    int pingMode(QString ip);

    //=========================================================================================
    // READ HYSTORY
    //=========================================================================================
    void readDatabaseHystory();
    //=========================================================================================
    // ADD HYSTORY
    //=========================================================================================
    void AddDatabaseHystory(int rangeData, QDateTime time, double ACTIVER, double ACTIVES, double ACTIVET, double REACTIVER, double REACTIVES, double REACTIVET);
    void clearHystoryPlotActive();
    void clearHystoryPlotReactive();
    void clearHystoryPlotRadiasi();
    //=========================================================================================
    // INSERT CONTROL
    //=========================================================================================
    void InsertControl_PLTS(QString Query, QString IOA,QString DateHmi, QString ParamValue, QString Operator);
    void InsertControl_PLTS_Status(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator);
    void InsertControl_PLTS_1(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertControl_PLTS_2(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertControl_PLTS_3(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertControl_PLTMH(QString Query, QString IOA, QString DateHmi, QString ParamValue, QString Operator);
    void InsertControl_PLTMH_Status(QString Query,QString IOA, QString DateHmi, QString ParamValue, QString Operator);
    void InsertControl_BATTERY(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertControl_Battery_Status(QString Query, QString IOA,QString DateHmi, QString ParamValue, QString Operator);
    void InsertControl_INVERTER(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertControl_INVERTER_Status(QString Query, QString IOA,QString DateHmi, QString ParamValue, QString Operator);
    void InsertControl_INVERTER_1(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertControl_INVERTER_2(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertControl_INVERTER_3(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertControl_LOAD(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertControl_LOAD_Status(QString Query,QString IOA, QString DateHmi, QString ParamValue, QString Operator);
    void InsertControl_Hystory(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertControl_Network(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertControl_InverterCurve(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    //=========================================================================================
    // INSERT SETPOINT
    //=========================================================================================
    void InsertPltmhW(QString Query, QString IOA,QString DateHmi, QString ParamValue, QString Operator);
    void InsertPltmhVAR(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);

    void InsertBatteryDODMIN(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertBatteryDODMAX(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);

    void InsertBatterySOCMIN(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertBatterySOCMAX(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);

    void InsertModeInverter(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertInverterW(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertInverterVAR(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);

    void InsertGradienM1(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertGradienC1(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);

    void InsertGradienM2(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertGradienC2(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);

    void InsertLoadW(QString Query, QString IOA,QString DateHmi, QString ParamValue, QString Operator);
    void InsertLoadVAR(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    //=========================================================================================
    // INSERT STATUS NETWORK LOGIN
    //=========================================================================================
    void InsertStatusServer(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    void InsertLogUser(QString Query, QString IOA,QString DateHmi, QString ParamValue,QString Operator);
    //=========================================================================================
    // GET ALL BATTERY
    //=========================================================================================
    void GetAllBattery(bool Activated);
    void GetStatusRTU_ALARMBATTERY(QString query,QString query2);
    void batery1();
    void batery2();
    void batery3();
    void batery4();
    void view_curve();
    void GetAllStatusBatre();
    void setPinkBattery();
    void setNormalBattery();
    void setPinkBatteryPack();
    //=========================================================================================
    // GET ALL TRENDING
    //=========================================================================================
    void TrendingTop(QString BlueLine,QString RedLine,QString GreenLine, QString YellowLine);
    void TrendingTopRunning(QString BlueLine,QString RedLine,QString GreenLine, QString YellowLine);
    void TrendingMid(QString BlueLine,QString RedLine,QString GreenLine, QString YellowLine);
    void TrendingMidRunning(QString BlueLine,QString RedLine,QString GreenLine, QString YellowLine);
    void TrendingDown(QString BlueLine);
    void TrendingDownRunning(QString BlueLine);
    void View_TrendingUI();
    void setDataTrending();
    void setTitleTranding(QString title);
    void clearDataGraphTranding();
    void clearDataRadiasi();
    void setPlotPltmh();
    void setPlotPlts();

    void realtimeDataValue(float aktifA,float aktifB,float aktifC,float reaktifA,float reaktifB,float reaktifC);
    void realtimeDataSlot();
    void realtimeDataSlot1();
    void realtimeDataSlot2();
    void historycalDataSlot();
    void historycalDataSlot1();
    void historycalDataSlot2();


    //=========================================================================================
    // GET ALL EVENTLIST
    //=========================================================================================
    void GetAllEventList(bool Activated, QString List);
    void EventListPLTS(QString Query);
    void EventListPLTMH(QString Query);
    void EventListBattery(QString Query);
    void EventListInverter(QString Query);
    void EventListLoad(QString Query);
    void EventListNetwork(QString Query);
    void EventAlarmList(QString Query);
    void EventListAll(QString Query);

    //=========================================================================================
    // GET ALL RST
    //=========================================================================================
    void ReadAllRST(bool Activated);
    // RST PLTS
    void GetRST_PLTS(QString Query,int Column);
    //RST PLTMH
    void GetRST_PLTMH(QString Query,int Column);
    //RST INVERTER
    void GetRST_INVERTER(QString Query,int Column);
    //RST LOAD
    void GetRST_LOAD(QString Query,int Column);
    //RST LOAD
    void GetRST_BATTERY(QString Query,int Column);
    //=========================================================================================
    // GET ALL RST ACTIVE REACTIVE POWER
    //=========================================================================================
    void ReadAllRST_ActiveReactive(bool Activated);
    //read all rst active
    void GetRST_PLTS_Active(QString Query,int Column);
    void GetRST_PLTMH_Active(QString Query,int Column);
    void GetRST_INVERTER_Active(QString Query,int Column);
    void GetRST_LOAD_Active(QString Query,int Column);
    void GetRST_BATTERY_Active(QString Query,int Column);
    // read all rst reactive
    void GetRST_PLTS_ReActive(QString Query,int Column);
    void GetRST_PLTMH_ReActive(QString Query,int Column);
    void GetRST_INVERTER_ReActive(QString Query,int Column);
    void GetRST_LOAD_ReActive(QString Query,int Column);
    // void GetRST_BATTERY_ReActive(QString Query,int Column);
    //=========================================================================================
    // GET ALL WATT VAR
    //=========================================================================================
    void ReadAllWVAR(bool Activated);
    // GET PLTS W VAR
    void GetPLTSWVAR(QString Query, int Col);
    // GET PLTMH W VAR
    void GetPLTMHWVAR(QString Query, int Col);
    // GET INVERTER W VAR
    void GetINVERTERWVAR(QString Query, int Col);
    // GET LOAD W VAR
    void GetLOADWVAR(QString Query, int Col);
    // GET BATTERY W VAR
    void GetBATTERYWVAR(QString Query, int Col);

    //=========================================================================================
    // GET ALL COS PHI
    //=========================================================================================
    void ReadAllCOSPHI(bool Activated);
    void GetPLTS_COSPHi(QString Query,int col);
    void GetPLTMH_COSPHi(QString Query,int col);
    void GetINVERTER_COSPHi(QString Query,int col);
    void GetLOAD_COSPHi(QString Query,int col);
    //=========================================================================================
    // GET FREQUENCY
    //=========================================================================================
    void Get_Frequency(QString Query);
    void Get_Frequency_inverter(QString Query);
    //=========================================================================================
    // GET ALL STATUS RTU HMI,METERING,COLOR PINK/BLACK
    //=========================================================================================
    void GetAllStatusRTU(bool Activated);
    void Get_StatusRTU_PLTS(QString Query_RTU, QString Query_ALARM, QString QueryCB, int Column);
    void Get_StatusRTU_PLTMH(QString Query_RTU, QString Query_ALARM, QString QueryCB, int Column);
    void Get_StatusRTU_INVERTER(QString Query_RTU, QString Query_ALARM, QString QueryCB,int Column);
    void Get_StatusRTU_LOAD(QString Query_RTU, QString Query_ALARM, QString QueryCB,int Column);
    void Get_StatusRTU_BATTERY(QString Query_RTU, QString Query_ALARM);
    //=========================================================================================
    // GET ALL COLOR PINK
    //=========================================================================================
    void Get_ColorWidget_PLTS(int Activated, int Event, QString CodePaint);
    void Get_ColorWidget_PLTMH(int Activated, int Event,QString CodePaint);
    void Get_ColorWidget_INVERTER(int Activated, int Event, QString CodePaint);
    void Get_ColorWidget_LOAD(int Activated, int Event,QString CodePaint);
    //=========================================================================================
    // GET RADIASI
    //=========================================================================================
    void getDataRadiasi(QString query);

    //=========================================================================================
    // PERHITUNGAN INVERTER
    //=========================================================================================
    void setDataGenerator(double dfdt);
    void SetDataCurva(double x,double y);
    //=========================================================================================
    // CONTROL COMPONENT
    //=========================================================================================
    void setPinkAll();
    //=========================================================================================
    // CONTROL TABEL ALARM
    //=========================================================================================

    void removeRowAlarmTable(QString text);
    void addRowAlarmTable(QString ioa, QString value, QString op, QString info);
    QString searchRowTable(int row,int column);
    //=========================================================================================
    // PLTS DATA GTI
    //=========================================================================================
    void getAllDataGTI(int i);
    //=========================================================================================
    // MODUL Komunikasi Inverter
    //=========================================================================================
    //set komunikasi
    //1  = normal
    //2  = pink
    void setKomunikasiBDI1(int i);
    void setKomunikasiBDI2(int i);
    void setKomunikasiBDI3(int i);
    void setKomunikasiBDI4(int i);
    void setKomunikasiBDI5(int i);
    void setKomunikasiBDI6(int i);
    void getDataInverter();

    void setDataFaultBDI1(QString Fault);
    void setDataFaultBDI2();
    void setDataFaultBDI3();
    void setDataFaultBDI4();
    void setDataFaultBDI5();
    void setDataFaultBDI6();
    void setDataAlarmFault();
};

#endif // MAINWINDOW_H
