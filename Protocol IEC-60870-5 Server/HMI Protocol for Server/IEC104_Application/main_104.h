#ifndef MAIN_104_H
#define MAIN_104_H

#include <QMainWindow>
#include <QTimer>
namespace Ui {
class main_104;
}

class main_104 : public QMainWindow
{
    Q_OBJECT

public:
    explicit main_104(QWidget *parent = 0);
    ~main_104();

private slots:
    void get_cb_db();
    // get from checkbox using timer
    void get_time_archive();
    // get from checkbox
    void get_cb_plts();
    void get_cb_pltmh();
    void get_cb_inverter();
    void get_cb_load();
    //button
    void on_pb_db_clicked();
    void on_pb_plts_clicked();
    void on_pb_pltmh_clicked();
    void on_pb_inverter_clicked();
    void on_pb_load_clicked();
    // insert rtu offline after terminate exe
    void rtu_plts_off(QString Query,QString Timestamp,QString Value);
    void rtu_pltmh_off(QString Query,QString Timestamp,QString Value);
    void rtu_inverter_off(QString Query,QString Timestamp,QString Value);
    void rtu_load_off(QString Query,QString Timestamp,QString Value);
    // filter RTU
    void Set_RTU_OFF(int values);
    void Get_settings();
    void Get_settingsdb();
    void Set_settings();
    void Clear_settings();
    // setting archive hmi
    void get_hmi_archive();
    void on_pb_config_clicked();


    void UsageCPU();


    void RealtimeDigitalInput();

    void insertLogArchive(QString Query,QString Timestamp,QString Value);
    void ConfigStartup(QString Mode);
    void AutoStartupOnWindows();
    void AutoSyncWindows();

    void ArchiveIntegration();
    void get_status_running();
    // backup
    void set_backup_List(QString Mode);

    void set_ActivePLTS(QString Query);
    void set_ReactivePLTS(QString Query);
    void set_InPowerPLTS(QString Query);
    void set_MonPLTS(QString Query);
    void set_CosphiPLTS(QString Query);
    void set_RadiationPLTS(QString Query);

    void set_ActivePLTMH(QString Query);
    void set_ReactivePLTMH(QString Query);
    void set_InPowerPLTMH(QString Query);
    void set_MonPLTMH(QString Query);
    void set_CosphiPLTMH(QString Query);

    void set_ActiveINVERTER(QString Query);
    void set_ReactiveINVERTER(QString Query);
    void set_InPowerINVERTER(QString Query);
    void set_MonINVERTER(QString Query);
    void set_CosphiINVERTER(QString Query);

    void set_ActiveLOAD(QString Query);
    void set_ReactiveLOAD(QString Query);
    void set_InPowerLOAD(QString Query);
    void set_MonLOAD(QString Query);
    void set_CosphiLOAD(QString Query);

    void insert_status_alloutcontrol(QString Query ,QString Timestamp,QString Operator, QString Status, QString value, QString IOA, QString Info);

    void set_TruncateTable();
    void set_PLTS();
    void set_PLTMH();
    void set_INVERTER();
    void set_LOAD();
    void set_Control();
    //==============================================
    void set_TruncatePLTSActive(QString Query);
    void set_TruncatePLTSReactive(QString Query);
    void set_TruncatePLTSinPower(QString Query);
    void set_TruncatePLTSCosphi(QString Query);
    void set_TruncatePLTSMon(QString Query);
    void set_TruncatePLTSRadiation(QString Query);
    //==============================================
    void set_TruncatePLTMHActive(QString Query);
    void set_TruncatePLTMHReactive(QString Query);
    void set_TruncatePLTMHinPower(QString Query);
    void set_TruncatePLTMHCosphi(QString Query);
    void set_TruncatePLTMHMon(QString Query);
    //==============================================
    void set_TruncateINVERTERActive(QString Query);
    void set_TruncateINVERTERReactive(QString Query);
    void set_TruncateINVERTERinPower(QString Query);
    void set_TruncateINVERTERCosphi(QString Query);
    void set_TruncateINVERTERMon(QString Query);
    //==============================================
    void set_TruncateLOADActive(QString Query);
    void set_TruncateLOADReactive(QString Query);
    void set_TruncateLOADinPower(QString Query);
    void set_TruncateLOADCosphi(QString Query);
    void set_TruncateLOADMon(QString Query);
    //==============================================
    void set_TruncateControl(QString Query);
    //==============================================
    void Log_db(QString Query,QString Timestamp,QString Status,QString Information);
    void SetLoggerNow(QString Status,QString Information);
    void on_pb_db_2_clicked();

    void RepairOnSrartup();



    void on_cb_delete_clicked();

private:
    Ui::main_104 *ui;
    QTimer *timer;
    QTimer *timerwindows;
    QString timestamp,timehmi;
    //status
    int a=0,b=0,c=0;
    // execute exe
    QString PLTS_EXE="\"C:\\IEC_104\\PLTS\\PLTS_104.exe\"";
    QString PLTMH_EXE="\"C:\\IEC_104\\PLTMH\\PLTMH_104.exe\"";
    QString INVERTER_EXE="\"C:\\IEC_104\\INVERTER\\INVERTER_104.exe\"";
    QString LOAD_EXE="\"C:\\IEC_104\\LOAD\\LOAD_104.exe\"";

    //terminate exe
    QString KILL_PLTS_EXE="Taskkill /IM PLTS_104.exe /F";
    QString KILL_PLTMH_EXE="Taskkill /IM PLTMH_104.exe /F";
    QString KILL_INVERTER_EXE="Taskkill /IM INVERTER_104.exe /F";
    QString KILL_LOAD_EXE="Taskkill /IM LOAD_104.exe /F";

    QString PLTS="mon_active_plts mon_reactive_plts in_power_plts mon_plts mon_cosphi_plts mon_radiation";
    QString PLTMH="mon_active_pltmh mon_reactive_pltmh in_power_pltmh mon_pltmh mon_cosphi_pltmh";
    QString INVERTER="mon_active_inverter mon_reactive_inverter in_power_inverter mon_inverter mon_cosphi_inverter";
    QString LOAD="mon_active_load mon_reactive_load in_power_load mon_load mon_cosphi_load";

    QString GlobalTime,yy,mm,dd,hh;

    QString Temp2,temp,tempku,temps_log;
};

#endif // MAIN_104_H
