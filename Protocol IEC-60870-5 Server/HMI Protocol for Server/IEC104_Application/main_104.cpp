#include "main_104.h"
#include "ui_main_104.h"
#include <QProcess>
#include <QDebug>
#include <QDateTime>
#include <dirent.h>
#include <regex>
#include <QtSql/qsqldatabase.h>
#include <QMessageBox>
#include <QSettings>
#include <QSqlQuery>
#include <QDir>

main_104::main_104(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::main_104)
{
    ui->setupUi(this);
    Get_settings();
    ui->gb->setEnabled(0);
    //ui->groupBox_3->setEnabled(0);

    //RepairOnSrartup();
    timerwindows = new QTimer(this);
    connect(timerwindows, SIGNAL(timeout()), this, SLOT(RepairOnSrartup()));
    timerwindows->start(3600000);


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(get_time_archive()));
    connect(timer, SIGNAL(timeout()), this, SLOT(get_hmi_archive()));
    connect(timer, SIGNAL(timeout()), this, SLOT(get_status_running()));
    connect(timer, SIGNAL(timeout()), this, SLOT(RealtimeDigitalInput()));
    timer->start();

    connect(ui->cb_plts, SIGNAL(stateChanged(int)), this, SLOT(get_cb_plts()));
    connect(ui->cb_pltmh, SIGNAL(stateChanged(int)), this, SLOT(get_cb_pltmh()));
    connect(ui->cb_inverter, SIGNAL(stateChanged(int)), this, SLOT(get_cb_inverter()));
    connect(ui->cb_load, SIGNAL(stateChanged(int)), this, SLOT(get_cb_load()));
    connect(ui->cb_archive, SIGNAL(stateChanged(int)), this, SLOT(get_time_archive()));
    connect(ui->cb_archive_hmi, SIGNAL(stateChanged(int)), this, SLOT(get_hmi_archive()));

    connect(ui->cb_db, SIGNAL(stateChanged(int)), this, SLOT(get_cb_db()));
    connect(ui->cbSync,SIGNAL(stateChanged(int)),this,SLOT(AutoSyncWindows()));

    connect(ui->cb_Startup, SIGNAL(stateChanged(int)), this, SLOT(AutoStartupOnWindows()));
    ConfigStartup("Cons");
    ConfigStartup("Load");

}

main_104::~main_104()
{
    delete ui;
    delete timer;
}

void main_104::get_cb_db()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ui->l_server->text());
    db.setDatabaseName(ui->l_database->text());
    db.setUserName(ui->l_username->text());
    db.setPassword(ui->l_password->text());
    if(ui->cb_db->checkState())
    {
        if(ui->cb_db->text()=="Manual")
        {
            if (db.open())
            {
                qDebug()<<"db1 open";
                ui->status_db->setStyleSheet("color: rgb(0, 255, 0)");
                ui->status_db->setText("Connected !");
                ui->l_server->setEnabled(0);
                ui->l_database->setEnabled(0);
                ui->l_username->setEnabled(0);
                ui->l_password->setEnabled(0);
                ui->l_database_2->setEnabled(0);
                ui->gb->setEnabled(1);
                ui->pb_db->setText("Stop");
                ui->cb_db->setText("AutoConnect");
                ui->pb_db->setEnabled(0);
                Set_settings();
                Get_settings();
                //Get_settingsdb();
                ConfigStartup("Load");
                ui->History->appendPlainText(timehmi+": Database Connected With AutoConnected !");
            }
            else
            {
                ui->status_db->setText("Connection error !");
                ui->History->appendPlainText(timehmi+": Unable Connect to database please check your connection !");
            }
        }
        else
        {
            db.close();
            ui->pb_db->setText("Start");
            ui->status_db->setStyleSheet("color: rgb(255, 0, 0)");
            ui->status_db->setText("Disconnected !");
            ui->l_server->setEnabled(1);
            ui->l_database->setEnabled(1);
            ui->l_username->setEnabled(1);
            ui->l_password->setEnabled(1);
            ui->gb->setEnabled(0);
            ui->History->appendPlainText(timehmi+": Database Disconnected !");
        }
    }
    else if(ui->cb_db->checkState()==false)
    {
        ui->cb_db->setText("Manual");
        db.close();
        ui->pb_db->setText("Start");
        ui->status_db->setStyleSheet("color: rgb(255, 0, 0)");
        ui->status_db->setText("Disconnected !");
        ui->l_server->setEnabled(1);
        ui->l_database->setEnabled(1);
        ui->l_database_2->setEnabled(1);
        ui->l_username->setEnabled(1);
        ui->l_password->setEnabled(1);
        ui->gb->setEnabled(0);
        ui->pb_db->setEnabled(1);
        Clear_settings();
        ui->History->appendPlainText(timehmi+": Database manual mode activated !");
    }

}


void main_104::get_time_archive()
{
    QDateTime dtime = QDateTime::currentDateTime();
    QString dtxt = dtime.toString("hh:mm:ss");
    QString hmi = dtime.toString("yyyy-MM-dd");
    timestamp=hmi;
    QString HmiProt=dtime.toString("yyyy-MM-dd hh:mm:ss.zzz");
    timehmi=HmiProt;
    QString y=dtime.toString("yyyy"); // years
    yy=y;
    QString m=dtime.toString("MM"); // month
    mm=m;
    QString d=dtime.toString("dd"); // day
    dd=d;
    QString h=dtime.toString("hh"); // hours
    hh=h;
    QString min=dtime.toString("mm"); // minutes

    QString tt=dtime.toString("AP"); // AM/PM

    ui->label_24->setText(dtxt);

    if(ui->cb_archive->checkState())
    {
        QDir().mkdir("C:\\IEC_104\\");
        QDir().mkdir("C:\\IEC_104\\Backup\\");
        QDir().mkdir("C:\\IEC_104\\Backup\\SQL\\");
        QDir().mkdir("C:\\IEC_104\\Backup\\SQL\\ALLDATA\\");
        QDir().mkdir("C:\\IEC_104\\Backup\\SQL\\TRENDING\\");

        QString Tempslog=""+y+"-"+m+"-"+d+"_"+h+"-"+min+"-"+tt+"";
        temps_log=Tempslog;
        QString Temps2=""+y+"-"+m+"-"+d+"";
        GlobalTime=Temps2;
        ui->timeEdit->setEnabled(0);
        ui->cb_archive->setStyleSheet("color: rgb(0, 255, 0)");
        ui->cb_archive->setText("Auto Archive Now Activated On "+ui->timeEdit->text()+"");
        //========================================================================================//
        // create directory
        //========================================================================================
        if(ui->label_24->text()==ui->timeEdit->text())
        {
            QProcess *p= new QProcess(this);
            QString Alldb="mysqldump "+ui->l_database->text()+" -u "+ui->l_username->text()+" -p"+ui->l_password->text()+" | gzip > ""C:\\IEC_104\\Backup\\SQL\\ALLDATA\\Archive-"+Temps2+".sql.gz";

            QString trending="mysqldump "+ui->l_database->text()+" -u "+ui->l_username->text()+" -p"+ui->l_password->text()+" "+PLTS+" "+PLTMH+" "+INVERTER+" "+LOAD+" | gzip > ""C:\\IEC_104\\Backup\\SQL\\TRENDING\\Archive-"+Temps2+".sql.gz";

            QString Repair="mysqlcheck --auto-repair -o --databases "+ui->l_database->text()+" "+ui->l_database_2->text()+" -u "+ui->l_username->text()+" -p"+ui->l_password->text()+"";
            p->start("cmd.exe", QStringList() << "/c" << Alldb);
            if (p->waitForStarted())
            {
                //========================================================================================//
                // Excel
                //========================================================================================
                if (ui->cb_archive_xls->checkState())
                {
                    set_backup_List("XLS");
                    ui->History->appendPlainText(timehmi+": Archieve Excel Succesfully");
                }
                else if(ui->cb_archive_xls->checkState()==false)
                {
                    ui->cb_archive_xls->setStyleSheet("color: rgb(255, 0, 0)");
                    ui->cb_archive_xls->setText("Archive To EXCEL (Xls)");
                }
                //========================================================================================//
                // CSV
                //========================================================================================
                if (ui->cb_archive_csv->checkState())
                {
                    set_backup_List("CSV");
                    ui->History->appendPlainText(timehmi+": Archieve Csv Succesfully");
                }
                else if(ui->cb_archive_csv->checkState()==false)
                {
                    ui->cb_archive_csv->setStyleSheet("color: rgb(255, 0, 0)");
                    ui->cb_archive_csv->setText("Archive To CSV (csv)");
                }
                p->waitForFinished();
                qDebug() << p->readAllStandardOutput();
                // alldb
                p->start("cmd.exe", QStringList() << "/c" << trending);
                if (p->waitForStarted())
                {
                    p->waitForFinished();
                    // backup data
                    qDebug() << p->readAllStandardOutput();
                    qDebug() << trending;
                }
                //====================================================================
                //repair and optimize after delete
                //====================================================================
                p->start("cmd.exe", QStringList() << "/c" << Repair);
                if (p->waitForStarted())
                {
                    p->waitForFinished();
                    // repair table
                    qDebug() << p->readAllStandardOutput();
                    AutoSyncWindows();
                }
                qDebug() << Alldb;
                //===================================================================
                // delete archive / truncate after archive
                //===================================================================
             if (ui->cb_delete->checkState())
             {
                 set_TruncateTable();
                 ui->History->appendPlainText(timehmi+": Data Has Been deleted !");
             }
                ui->label_28->setStyleSheet("color: rgb(0, 255, 0)");
                ui->label_28->setText("Archive "+timestamp+ " Succesfully");
            }
        }

    }
    else if(ui->cb_archive->checkState()==false)
    {
        ui->timeEdit->setEnabled(1);
        ui->cb_archive->setStyleSheet("color: rgb(255, 0, 0)");
        ui->cb_archive->setText("Auto Archive Not Activated");
    }
}

void main_104::get_cb_plts()
{
    if(ui->cb_plts->checkState())
    {
        QString executable = PLTS_EXE;
        QProcess *process = new QProcess(this);
        process->start(executable, QStringList());
        if ( process->state() == QProcess::Running ) {
            ui->cb_plts->setText("AutoConnect");
            ui->pb_plts->setText("Stop");
            ui->status_plts->setStyleSheet("color: rgb(0, 255, 0)");
            ui->status_plts->setText("Running !");
            ui->History->appendPlainText("Service PLTS Started With Auto Action");
            ui->ip_plts->setEnabled(0);
            ui->asdu_plts->setEnabled(0);
            ui->port_plts->setEnabled(0);
            ui->pb_plts->setEnabled(0);
        }
    }
    else if (ui->cb_plts->checkState()==false)
    {
        QProcess *process = new QProcess(this);
        process->execute(KILL_PLTS_EXE);
        if (process->state() == QProcess::NotRunning ) {
            ui->cb_plts->setText("Manual");
            ui->pb_plts->setText("Start");
            ui->status_plts->setStyleSheet("color: rgb(255, 0, 0)");
            ui->status_plts->setText("Stopped !");
            ui->History->appendPlainText("Service PLTS Stopped With Auto Action");
            ui->ip_plts->setEnabled(1);
            ui->asdu_plts->setEnabled(1);
            ui->port_plts->setEnabled(1);
            ui->pb_plts->setEnabled(1);
            Set_RTU_OFF(0);
        };

    }

}

void main_104::get_cb_pltmh()
{
    if(ui->cb_pltmh->checkState())
    {
        QString executable = PLTMH_EXE;
        QProcess *process = new QProcess(this);
        process->start(executable, QStringList());
        if ( process->state() == QProcess::Running ) {
            ui->cb_pltmh->setText("AutoConnect");
            ui->pb_pltmh->setText("Stop");
            ui->status_pltmh->setStyleSheet("color: rgb(0, 255, 0)");
            ui->status_pltmh->setText("Running !");
            ui->History->appendPlainText("Service PLTMH Started With Auto Action");
            ui->ip_pltmh->setEnabled(0);
            ui->asdu_pltmh->setEnabled(0);
            ui->port_pltmh->setEnabled(0);
            ui->pb_pltmh->setEnabled(0);
        };
    }
    else if (ui->cb_pltmh->checkState()==false)
    {
        QProcess *process = new QProcess(this);
        process->execute(KILL_PLTMH_EXE);
        if (process->state() == QProcess::NotRunning ) {
            ui->cb_pltmh->setText("Manual");
            ui->pb_pltmh->setText("Start");
            ui->status_pltmh->setStyleSheet("color: rgb(255, 0, 0)");
            ui->status_pltmh->setText("Stopped !");
            ui->History->appendPlainText("Service PLTMH Stopped With Auto Action");
            ui->ip_pltmh->setEnabled(1);
            ui->asdu_pltmh->setEnabled(1);
            ui->port_pltmh->setEnabled(1);
            ui->pb_pltmh->setEnabled(1);
            Set_RTU_OFF(1);
        };
    }
}

void main_104::get_cb_inverter()
{
    if(ui->cb_inverter->checkState())
    {
        QString executable = INVERTER_EXE;
        QProcess *process = new QProcess(this);
        process->start(executable, QStringList());
        if ( process->state() == QProcess::Running ) {
            ui->cb_inverter->setText("AutoConnect");
            ui->pb_inverter->setText("Stop");
            ui->status_bdi->setStyleSheet("color: rgb(0, 255, 0)");
            ui->status_bdi->setText("Running !");
            ui->History->appendPlainText("Service Inverter Started With Auto Action");
            ui->ip_bdi->setEnabled(0);
            ui->asdu_bdi->setEnabled(0);
            ui->port_inverter->setEnabled(0);
            ui->pb_inverter->setEnabled(0);
        };
    }
    else if (ui->cb_inverter->checkState()==false)
    {
        QProcess *process = new QProcess(this);
        process->execute(KILL_INVERTER_EXE);
        if (process->state() == QProcess::NotRunning ) {
            ui->cb_inverter->setText("Manual");
            ui->pb_inverter->setText("Start");
            ui->status_bdi->setStyleSheet("color: rgb(255, 0, 0)");
            ui->status_bdi->setText("Stopped !");
            ui->History->appendPlainText("Service Inverter Stopped With Auto Action");
            ui->ip_bdi->setEnabled(1);
            ui->asdu_bdi->setEnabled(1);
            ui->port_inverter->setEnabled(1);
            ui->pb_inverter->setEnabled(1);
            Set_RTU_OFF(2);
        };
    }

}

void main_104::get_cb_load()
{
    if(ui->cb_load->checkState())
    {
        QString executable = LOAD_EXE;
        QProcess *process = new QProcess(this);
        process->start(executable, QStringList());
        if ( process->state() == QProcess::Running ) {
            ui->cb_load->setText("AutoConnect");
            ui->pb_load->setText("Stop");
            ui->status_load->setStyleSheet("color: rgb(0, 255, 0)");
            ui->status_load->setText("Running !");
            ui->History->appendPlainText("Service Load Started With Auto Action");
            ui->ip_load->setEnabled(0);
            ui->asdu_load->setEnabled(0);
            ui->port_load->setEnabled(0);
            ui->pb_load->setEnabled(0);
        };
    }
    else if (ui->cb_load->checkState()==false)
    {
        QProcess *process = new QProcess(this);
        process->execute(KILL_LOAD_EXE);
        if (process->state() == QProcess::NotRunning ) {
            ui->cb_load->setText("Manual");
            ui->pb_load->setText("Start");
            ui->status_load->setStyleSheet("color: rgb(255, 0, 0)");
            ui->status_load->setText("Stopped !");
            ui->History->appendPlainText("Service Load Stopped With Auto Action");
            ui->ip_load->setEnabled(1);
            ui->asdu_load->setEnabled(1);
            ui->port_load->setEnabled(1);
            ui->pb_load->setEnabled(1);
            Set_RTU_OFF(3);
        };
    }

}

void main_104::on_pb_db_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ui->l_server->text());
    db.setDatabaseName(ui->l_database->text());
    db.setUserName(ui->l_username->text());
    db.setPassword(ui->l_password->text());
    if(ui->pb_db->text()=="Start")
    {
        if (db.open())
        {
            qDebug()<<"db1 open";
            ui->status_db->setStyleSheet("color: rgb(0, 255, 0)");
            ui->status_db->setText("Connected !");
            ui->l_server->setEnabled(0);
            ui->l_database->setEnabled(0);
            ui->l_database_2->setEnabled(0);
            ui->l_username->setEnabled(0);
            ui->l_password->setEnabled(0);
            ui->gb->setEnabled(1);
            ui->cb_db->setEnabled(0);
            ui->pb_db->setText("Stop");
            Set_settings();
            Get_settings();
            // Get_settingsdb();
            ConfigStartup("Load");
            //ui->groupBox_3->setEnabled(1);
        }
        else
        {
            ui->status_db->setText("Unable Connect to database !");
        }

    }
    else
    {
        db.close();
        ui->pb_db->setText("Start");
        ui->cb_db->setEnabled(1);
        ui->status_db->setStyleSheet("color: rgb(255, 0, 0)");
        ui->status_db->setText("Disconnected !");
        ui->l_server->setEnabled(1);
        ui->l_database->setEnabled(1);
        ui->l_database_2->setEnabled(1);
        ui->l_username->setEnabled(1);
        ui->l_password->setEnabled(1);
        ui->gb->setEnabled(0);
        //ui->groupBox_3->setEnabled(0);
        Clear_settings();

    }
}

void main_104::on_pb_plts_clicked()
{
    if(ui->pb_plts->text()=="Start")
    {
        QString executable = PLTS_EXE;
        QProcess *process = new QProcess(this);
        process->start(executable, QStringList());
        // task priority
        //choosetask();
        if ( process->state() == QProcess::Running ) {
            ui->status_plts->setStyleSheet("color: rgb(0, 255, 0)");
            ui->status_plts->setText("Running !");
            ui->ip_plts->setEnabled(0);
            ui->asdu_plts->setEnabled(0);
            ui->port_plts->setEnabled(0);
            ui->cb_plts->setEnabled(0);
            ui->pb_plts->setText("Stop");
            ui->History->appendPlainText("Service PLTS Started On "+timehmi+" with Manual Action");
        };
    }
    else
    {
        QProcess *process = new QProcess(this);
        process->execute(KILL_PLTS_EXE);
        if (process->state() == QProcess::NotRunning ) {
            ui->pb_plts->setText("Start");
            ui->status_plts->setStyleSheet("color: rgb(255, 0, 0)");
            ui->status_plts->setText("Stopped !");
            ui->History->appendPlainText("Service PLTS Stopped On "+timehmi+" With Manual Action");
            ui->ip_plts->setEnabled(1);
            ui->asdu_plts->setEnabled(1);
            ui->port_plts->setEnabled(1);
            ui->cb_plts->setEnabled(1);
            Set_RTU_OFF(0);
        };
    }

}


void main_104::on_pb_pltmh_clicked()
{
    if(ui->pb_pltmh->text()=="Start")
    {
        QString executable = PLTMH_EXE;
        QProcess *process = new QProcess(this);
        process->start(executable, QStringList());
        if ( process->state() == QProcess::Running ) {
            ui->status_pltmh->setStyleSheet("color: rgb(0, 255, 0)");
            ui->status_pltmh->setText("Running !");
            ui->History->appendPlainText("Service PLTMH Started On "+timehmi+" With Manual Action");
            ui->ip_pltmh->setEnabled(0);
            ui->asdu_pltmh->setEnabled(0);
            ui->port_pltmh->setEnabled(0);
            ui->cb_pltmh->setEnabled(0);
            ui->pb_pltmh->setText("Stop");
        };
    }
    else
    {
        QProcess *process = new QProcess(this);
        process->execute(KILL_PLTMH_EXE);
        if (process->state() == QProcess::NotRunning ) {
            ui->pb_pltmh->setText("Start");
            ui->status_pltmh->setStyleSheet("color: rgb(255, 0, 0)");
            ui->status_pltmh->setText("Stopped !");
            ui->History->appendPlainText("Service PLTMH Stopped On "+timehmi+" With Manual Action");
            ui->ip_pltmh->setEnabled(1);
            ui->asdu_pltmh->setEnabled(1);
            ui->port_pltmh->setEnabled(1);
            ui->cb_pltmh->setEnabled(1);
            Set_RTU_OFF(1);
        };
    }
}

void main_104::on_pb_inverter_clicked()
{
    if(ui->pb_inverter->text()=="Start")
    {
        QString executable = INVERTER_EXE;
        QProcess *process = new QProcess(this);
        process->start(executable, QStringList());
        if ( process->state() == QProcess::Running ) {
            ui->status_bdi->setStyleSheet("color: rgb(0, 255, 0)");
            ui->status_bdi->setText("Running !");
            ui->ip_bdi->setEnabled(0);
            ui->asdu_bdi->setEnabled(0);
            ui->port_inverter->setEnabled(0);
            ui->cb_inverter->setEnabled(0);
            ui->pb_inverter->setText("Stop");
            ui->History->appendPlainText("Service Inverter Started On "+timehmi+" With Manual Action");
        };
    }
    else
    {
        QProcess *process = new QProcess(this);
        process->execute(KILL_INVERTER_EXE);
        if (process->state() == QProcess::NotRunning ) {
            ui->pb_inverter->setText("Start");
            ui->status_bdi->setStyleSheet("color: rgb(255, 0, 0)");
            ui->status_bdi->setText("Stopped !");
            ui->History->appendPlainText("Service Inverter Stopped On "+timehmi+" With Manual Action");
            ui->ip_bdi->setEnabled(1);
            ui->asdu_bdi->setEnabled(1);
            ui->port_inverter->setEnabled(1);
            ui->cb_inverter->setEnabled(1);
            Set_RTU_OFF(2);
        };
    }
}

void main_104::on_pb_load_clicked()
{
    if(ui->pb_load->text()=="Start")
    {
        QString executable = LOAD_EXE;
        QProcess *process = new QProcess(this);
        process->start(executable, QStringList());
        if ( process->state() == QProcess::Running ) {
            ui->status_load->setStyleSheet("color: rgb(0, 255, 0)");
            ui->status_load->setText("Running !");
            ui->History->appendPlainText("Service Load Started On "+timehmi+" With Manual Action");
            ui->ip_load->setEnabled(0);
            ui->asdu_load->setEnabled(0);
            ui->port_load->setEnabled(0);
            ui->cb_load->setEnabled(0);
            ui->pb_load->setText("Stop");
        };
    }
    else
    {
        QProcess *process = new QProcess(this);
        process->execute(KILL_LOAD_EXE);
        if (process->state() == QProcess::NotRunning ) {
            ui->pb_load->setText("Start");
            ui->status_load->setStyleSheet("color: rgb(255, 0, 0)");
            ui->status_load->setText("Stopped !");
            ui->History->appendPlainText("Service Load Stopped On "+timehmi+" With Manual Action");
            ui->ip_load->setEnabled(1);
            ui->asdu_load->setEnabled(1);
            ui->port_load->setEnabled(1);
            ui->cb_load->setEnabled(1);
            Set_RTU_OFF(3);
        };
    }
}

void main_104::rtu_plts_off(QString Query, QString Timestamp, QString Value)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1",Timestamp);
    qry.bindValue(":p_2",Value);
    qry.exec();
}

void main_104::rtu_pltmh_off(QString Query, QString Timestamp, QString Value)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1",Timestamp);
    qry.bindValue(":p_2",Value);
    qry.exec();
}

void main_104::rtu_inverter_off(QString Query, QString Timestamp, QString Value)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1",Timestamp);
    qry.bindValue(":p_2",Value);
    qry.exec();
}

void main_104::rtu_load_off(QString Query, QString Timestamp, QString Value)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1",Timestamp);
    qry.bindValue(":p_2",Value);
    qry.exec();
}

void main_104::Set_RTU_OFF(int values)
{
    switch(values)
    {
    case 0:
        rtu_plts_off("insert into in_cb_rtu_plts(timestamps,value) values (:p_1,:p_2)",timehmi,"0");
        break;
    case 1:
        rtu_pltmh_off("insert into in_cb_rtu_pltmh(timestamps,value) values (:p_1,:p_2)",timehmi,"0");
        break;
    case 2:
        rtu_inverter_off("insert into in_cb_rtu_inverter(timestamps,value) values (:p_1,:p_2)",timehmi,"0");
        break;
    case 3:
        rtu_load_off("insert into in_cb_rtu_load(timestamps,value) values (:p_1,:p_2)",timehmi,"0");
        break;
    }
}

void main_104::Get_settingsdb()
{
    QSqlQuery qry;
    qry.prepare("select ipaddr,asdu from in_list_network where name='PLTS'");
    qry.exec();
    if (qry.next())
    {
        ui->ip_plts->setText(qry.value(0).toString());
        ui->asdu_plts->setText(qry.value(1).toString());
    }

    qry.prepare("select ipaddr,asdu from in_list_network where name='PLTMH'");
    qry.exec();
    if (qry.next())
    {
        ui->ip_pltmh->setText(qry.value(0).toString());
        ui->asdu_pltmh->setText(qry.value(1).toString());
    }
    qry.prepare("select ipaddr,asdu from in_list_network where name='INVERTER'");
    qry.exec();
    if (qry.next())
    {
        ui->ip_bdi->setText(qry.value(0).toString());
        ui->asdu_bdi->setText(qry.value(1).toString());
    }
    qry.prepare("select ipaddr,asdu from in_list_network where name='LOAD'");
    qry.exec();
    if (qry.next())
    {
        ui->ip_load->setText(qry.value(0).toString());
        ui->asdu_load->setText(qry.value(1).toString());
    }

}

void main_104::Get_settings()
{
    QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                  QSettings::NativeFormat );
    set.beginGroup("DATABASE");
    ui->l_server->setText(set.value("hostserver").toString());
    ui->l_database->setText(set.value("database").toString());
    ui->l_database_2->setText(set.value("database2").toString());
    ui->l_username->setText(set.value("user").toString());
    ui->l_password->setText(set.value("password").toString());
    set.endGroup();
}

void main_104::Set_settings()
{
    QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                  QSettings::NativeFormat );
    set.beginGroup("DATABASE");
    set.setValue("hostserver",ui->l_server->text());
    set.setValue("database",ui->l_database->text());
    set.setValue("database2",ui->l_database_2->text());
    set.setValue("user",ui->l_username->text());
    set.setValue("password",ui->l_password->text());
    set.endGroup();
    qDebug()<<"Succesfully";
}

void main_104::Clear_settings()
{
    ui->ip_plts->clear();
    ui->ip_pltmh->clear();
    ui->ip_bdi->clear();
    ui->ip_load->clear();

    ui->asdu_plts->clear();
    ui->asdu_pltmh->clear();
    ui->asdu_bdi->clear();
    ui->asdu_load->clear();

}

void main_104::get_hmi_archive()
{
    if (ui->cb_archive_hmi->checkState())
    {
        ui->cb_archive_hmi->setStyleSheet("color: rgb(0, 255, 0)");
        ui->cb_archive_hmi->setText("Unarchive From HMI Activated");
        //archive
        ArchiveIntegration();
    }
    else if(ui->cb_archive_hmi->checkState()==false)
    {
        ui->cb_archive_hmi->setStyleSheet("color: rgb(255, 0, 0)");
        ui->cb_archive_hmi->setText("Unarchive From HMI Not Activated");
    }
}




void main_104::on_pb_config_clicked()
{
    //UpdateAll();
    ConfigStartup("Save");
    ui->History->appendPlainText("Configuration has been saved! on "+timehmi);
    //QMessageBox::information(this,"Succesfully","Configuration has been saved!");
}
void main_104::UsageCPU()
{

    QProcess *p= new QProcess(this);
    p->start("cmd.exe", QStringList() << "/c" << "wmic path Win32_PerfFormattedData_PerfProc_Process get Name,PercentProcessorTime | findstr /i /c:PLTS_104");
    if (p->waitForStarted())
    {
        p->waitForFinished();
        // qDebug() << p->readAllStandardOutput();
        //ui->usage_plts->text()=p->readAllStandardOutput();
    }

}

void main_104::RealtimeDigitalInput()
{
    QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                  QSettings::NativeFormat );
    set.beginGroup("Realtime Digital Input");
    QString Red="#ff0000",Green="#aaff7f";


    //PLTS
    if (set.value("DI_PLTS")==1)
    {
        ui->widDigital->item(1,0)->setText("PLTS ON OPEN");
        ui->widDigital->item(1,0)->setBackgroundColor(QColor(Green));
    }
    else if (set.value("DI_PLTS")==2 )
    {
        ui->widDigital->item(1,0)->setText("PLTS ON CLOSE");
        ui->widDigital->item(1,0)->setBackgroundColor(QColor(Green));
    }
    else
    {
        ui->widDigital->item(1,0)->setText("PLTS OFF/INVALID");
        ui->widDigital->item(1,0)->setBackgroundColor(QColor(Red));
    }
    //==============================================================================
    //PLTS PV 1                                                                     |
    //==============================================================================
    if (set.value("DI_PV_PLTS_1")==1)
    {
        ui->widDigital->item(2,0)->setText("PV GTI 1 ON OPEN");
        ui->widDigital->item(2,0)->setBackgroundColor(QColor(Green));
    }
    else if (set.value("DI_PV_PLTS_1")==2 )
    {
        ui->widDigital->item(2,0)->setText("PV GTI 1 ON CLOSE");
        ui->widDigital->item(2,0)->setBackgroundColor(QColor(Green));
    }
    else
    {
        ui->widDigital->item(2,0)->setText("PV GTI 1 OFF/INVALID");
        ui->widDigital->item(2,0)->setBackgroundColor(QColor(Red));
    }
    //==============================================================================
    //PLTS PV 2                                                                     |
    //==============================================================================
    if (set.value("DI_PV_PLTS_2")==1)
    {
        ui->widDigital->item(3,0)->setText("PV GTI 2 ON OPEN");
        ui->widDigital->item(3,0)->setBackgroundColor(QColor(Green));
    }
    else if (set.value("DI_PV_PLTS_2")==2)
    {
        ui->widDigital->item(3,0)->setText("PV GTI 2 ON CLOSE");
        ui->widDigital->item(3,0)->setBackgroundColor(QColor(Green));
    }
    else
    {
        ui->widDigital->item(3,0)->setText("PV GTI 2 OFF/INVALID");
        ui->widDigital->item(3,0)->setBackgroundColor(QColor(Red));
    }
    //==============================================================================
    //PLTS PV 3                                                                     |
    //==============================================================================
    if (set.value("DI_PV_PLTS_3")==1)
    {
        ui->widDigital->item(4,0)->setText("PV GTI 3 ON OPEN");
        ui->widDigital->item(4,0)->setBackgroundColor(QColor(Green));
    }
    else if (set.value("DI_PV_PLTS_3")==2)
    {
        ui->widDigital->item(4,0)->setText("PV GTI 3 ON CLOSE");
        ui->widDigital->item(4,0)->setBackgroundColor(QColor(Green));
    }
    else
    {
        ui->widDigital->item(4,0)->setText("PV GTI 3 OFF/INVALID");
        ui->widDigital->item(4,0)->setBackgroundColor(QColor(Red));
    }

    //PLTMH
    if (set.value("DI_PLTMH")==1)
    {
        ui->widDigital->item(1,1)->setText("PLTMH ON OPEN");
        ui->widDigital->item(1,1)->setBackgroundColor(QColor(Green));
    }
    else if (set.value("DI_PLTMH")==2)
    {
        ui->widDigital->item(1,1)->setText("PLTMH ON CLOSE");
        ui->widDigital->item(1,1)->setBackgroundColor(QColor(Green));
    }
    else
    {
        ui->widDigital->item(1,1)->setText("PLTMH OFF/INVALID");
        ui->widDigital->item(1,1)->setBackgroundColor(QColor(Red));
    }
    //INVERTER
    if (set.value("DI_INVERTER")==1)
    {
        ui->widDigital->item(1,2)->setText("INVERTER ON OPEN");
        ui->widDigital->item(1,2)->setBackgroundColor(QColor(Green));
    }
    else if ( set.value("DI_INVERTER")==2)
    {
        ui->widDigital->item(1,2)->setText("INVERTER ON CLOSE");
        ui->widDigital->item(1,2)->setBackgroundColor(QColor(Green));
    }
    else
    {
        ui->widDigital->item(1,2)->setText("INVERTER OFF/INVALID");
        ui->widDigital->item(1,2)->setBackgroundColor(QColor(Red));
    }
    //==============================================================================
    // INVERTER DC 1                                                                |
    //==============================================================================
    if (set.value("DI_DC_INVERTER0")==0)
    {
        ui->widDigital->item(2,2)->setText("DC INVERTER 1 OFFLINE");
        ui->widDigital->item(2,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(2,2)->setText("DC INVERTER 1 ONLINE");
        ui->widDigital->item(2,2)->setBackgroundColor(QColor(Green));
    }
    //==============================================================================
    // INVERTER DC 2                                                                |
    //==============================================================================
    if (set.value("DI_DC_INVERTER1")==0)
    {
        ui->widDigital->item(3,2)->setText("DC INVERTER 2 OFFLINE");
        ui->widDigital->item(3,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(3,2)->setText("DC INVERTER 2 ONLINE");
        ui->widDigital->item(3,2)->setBackgroundColor(QColor(Green));
    }

    //==============================================================================
    // INVERTER DC 3                                                                |
    //==============================================================================
    if (set.value("DI_DC_INVERTER2")==0)
    {
        ui->widDigital->item(4,2)->setText("DC INVERTER 3 OFFLINE");
        ui->widDigital->item(4,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(4,2)->setText("DC INVERTER 3 ONLINE");
        ui->widDigital->item(4,2)->setBackgroundColor(QColor(Green));
    }

    //==============================================================================
    // INVERTER DC 4                                                                |
    //==============================================================================
    if (set.value("DI_DC_INVERTER3")==0)
    {
        ui->widDigital->item(5,2)->setText("DC INVERTER 4 OFFLINE");
        ui->widDigital->item(5,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(5,2)->setText("DC INVERTER 4 ONLINE");
        ui->widDigital->item(5,2)->setBackgroundColor(QColor(Green));
    }

    //==============================================================================
    // INVERTER DC 5                                                                |
    //==============================================================================
    if (set.value("DI_DC_INVERTER4")==0)
    {
        ui->widDigital->item(6,2)->setText("DC INVERTER 5 OFFLINE");
        ui->widDigital->item(6,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(6,2)->setText("DC INVERTER 5 ONLINE");
        ui->widDigital->item(6,2)->setBackgroundColor(QColor(Green));
    }

    //==============================================================================
    // INVERTER DC 6                                                                |
    //==============================================================================
    if (set.value("DI_DC_INVERTER5")==0)
    {
        ui->widDigital->item(7,2)->setText("DC INVERTER 6 OFFLINE");
        ui->widDigital->item(7,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(7,2)->setText("DC INVERTER 6 ONLINE");
        ui->widDigital->item(7,2)->setBackgroundColor(QColor(Green));
    }
    //==============================================================================
    // INVERTER DC 7                                                                |
    //==============================================================================
    if (set.value("DI_DC_INVERTER6")==0)
    {
        ui->widDigital->item(8,2)->setText("DC INVERTER 7 OFFLINE");
        ui->widDigital->item(8,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(8,2)->setText("DC INVERTER 7 ONLINE");
        ui->widDigital->item(8,2)->setBackgroundColor(QColor(Green));
    }

    //==============================================================================
    // INVERTER SW 1                                                                |
    //==============================================================================
    if (set.value("DI_SW_INVERTER0")==0)
    {
        ui->widDigital->item(9,2)->setText("SW INVERTER 1 OFFLINE");
        ui->widDigital->item(9,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(9,2)->setText("SW INVERTER 1 ONLINE");
        ui->widDigital->item(9,2)->setBackgroundColor(QColor(Green));
    }
    //==============================================================================
    // INVERTER SW 2                                                                |
    //==============================================================================
    if (set.value("DI_SW_INVERTER1")==0)
    {
        ui->widDigital->item(10,2)->setText("SW INVERTER 2 OFFLINE");
        ui->widDigital->item(10,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(10,2)->setText("SW INVERTER 2 ONLINE");
        ui->widDigital->item(10,2)->setBackgroundColor(QColor(Green));
    }

    //==============================================================================
    // INVERTER SW 3                                                                |
    //==============================================================================
    if (set.value("DI_SW_INVERTER2")==0)
    {
        ui->widDigital->item(11,2)->setText("SW INVERTER 3 OFFLINE");
        ui->widDigital->item(11,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(11,2)->setText("SW INVERTER 3 ONLINE");
        ui->widDigital->item(11,2)->setBackgroundColor(QColor(Green));
    }

    //==============================================================================
    // INVERTER SW 4                                                                |
    //==============================================================================
    if (set.value("DI_SW_INVERTER3")==0)
    {
        ui->widDigital->item(12,2)->setText("SW INVERTER 4 OFFLINE");
        ui->widDigital->item(12,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(12,2)->setText("SW INVERTER 4 ONLINE");
        ui->widDigital->item(12,2)->setBackgroundColor(QColor(Green));
    }

    //==============================================================================
    // INVERTER SW 5                                                                |
    //==============================================================================
    if (set.value("DI_SW_INVERTER4")==0)
    {
        ui->widDigital->item(13,2)->setText("SW INVERTER 5 OFFLINE");
        ui->widDigital->item(13,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(13,2)->setText("SW INVERTER 5 ONLINE");
        ui->widDigital->item(13,2)->setBackgroundColor(QColor(Green));
    }

    //==============================================================================
    // INVERTER SW 6                                                                |
    //==============================================================================
    if (set.value("DI_SW_INVERTER5")==0)
    {
        ui->widDigital->item(14,2)->setText("SW INVERTER 6 OFFLINE");
        ui->widDigital->item(14,2)->setBackgroundColor(QColor(Red));
    }
    else
    {
        ui->widDigital->item(14,2)->setText("SW INVERTER 6 ONLINE");
        ui->widDigital->item(14,2)->setBackgroundColor(QColor(Green));
    }
    //LOAD
    if (set.value("DI_LOAD")==1)
    {
        ui->widDigital->item(1,3)->setText("LOAD ON OPEN");
        ui->widDigital->item(1,3)->setBackgroundColor(QColor(Green));
    }
    else if (set.value("DI_LOAD")==2)
    {
        ui->widDigital->item(1,3)->setText("LOAD ON CLOSE");
        ui->widDigital->item(1,3)->setBackgroundColor(QColor(Green));
    }
    else
    {
        ui->widDigital->item(1,3)->setText("LOAD OFF/INVALID");
        ui->widDigital->item(1,3)->setBackgroundColor(QColor(Red));
    }
    //BATTERY
    if (set.value("DI_BATTERY")==1)
    {
        ui->widDigital->item(1,4)->setText("BATTERY ON OPEN");
        ui->widDigital->item(1,4)->setBackgroundColor(QColor(Green));
    }
    else if (set.value("DI_BATTERY")==2)
    {
        ui->widDigital->item(1,4)->setText("BATTERY ON CLOSE");
        ui->widDigital->item(1,4)->setBackgroundColor(QColor(Green));
    }
    else
    {
        ui->widDigital->item(1,4)->setText("BATTERY OFF/INVALID");
        ui->widDigital->item(1,4)->setBackgroundColor(QColor(Red));
    }
    set.endGroup();

    //==============================================================================
    // contoh client server registry    pltmh                                      |
    //==============================================================================
    //    QSettings tes("HKEY_USERS\\.DEFAULT\\HMI\\PLTMH",
    //                  QSettings::NativeFormat );
    //    tes.beginGroup("Status DI");

    //    if (tes.value("DI").toInt()==0 || tes.value("DI").toInt()==3)
    //    {
    //        ui->History->appendPlainText("STATUS DI PLTMH IS INVALID");
    //    }
    //    else if (tes.value("DI").toInt()==1)
    //    {
    //        ui->History->appendPlainText("STATUS DI PLTMH IS OPEN");

    //    }
    //    else if (tes.value("DI").toInt()==2)
    //    {
    //        ui->History->appendPlainText("STATUS DI PLTMH IS CLOSE");
    //    }
    //    else
    //    {
    //        ui->History->appendPlainText("STATUS DI PLTMH IS UNKNOWN");
    //    }
    //    tes.endGroup();
}

void main_104::insertLogArchive(QString Query, QString Timestamp, QString Value)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1",Timestamp);
    qry.bindValue(":p_2",Value);
    qry.exec();

}

void main_104::ConfigStartup(QString Mode)
{
    if (Mode=="Save")
    {
        QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                      QSettings::NativeFormat );
        set.beginGroup("Startup");
        set.setValue("AutoPLTS",ui->cb_plts->checkState());
        set.setValue("AutoPLTMH",ui->cb_pltmh->checkState());
        set.setValue("AutoINVERTER",ui->cb_inverter->checkState());
        set.setValue("AutoLoad",ui->cb_load->checkState());

        set.setValue("AutoArchive",ui->cb_archive->checkState());
        set.setValue("AutoArchiveTime",ui->timeEdit->time());
        set.setValue("AutoHMI",ui->cb_archive_hmi->checkState());
        set.setValue("AutoExel",ui->cb_archive_xls->checkState());
        set.setValue("AutoCsv",ui->cb_archive_csv->checkState());

        set.setValue("AutoStartup",ui->cb_Startup->checkState());
        set.setValue("AutoStartupDatabase",ui->cb_db->checkState());
        set.setValue("ClockSync",ui->cbSync->checkState());

        set.endGroup();
        //================================================================
        QSettings setku("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                        QSettings::NativeFormat );
        setku.beginGroup("ProtocolConfiguration");

        setku.setValue("IP PLTS",ui->ip_plts->text());
        setku.setValue("IP PLTMH",ui->ip_pltmh->text());
        setku.setValue("IP INVERTER",ui->ip_bdi->text());
        setku.setValue("IP LOAD",ui->ip_load->text());

        setku.setValue("ASDU PLTS",ui->asdu_plts->text());
        setku.setValue("ASDU PLTMH",ui->asdu_pltmh->text());
        setku.setValue("ASDU INVERTER",ui->asdu_bdi->text());
        setku.setValue("ASDU LOAD",ui->asdu_load->text());

        setku.setValue("PORT PLTS",ui->port_plts->text());
        setku.setValue("PORT PLTMH",ui->port_pltmh->text());
        setku.setValue("PORT INVERTER",ui->port_inverter->text());
        setku.setValue("PORT LOAD",ui->port_load->text());

        setku.setValue("TIMESYNC",ui->txttime->text());

        setku.endGroup();
        //================================================================
        QSettings k("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                    QSettings::NativeFormat );
        k.beginGroup("DATABASE");
        k.setValue("database",ui->l_database->text());
        k.setValue("database2",ui->l_database_2->text());
        k.setValue("hostserver",ui->l_server->text());
        k.setValue("password",ui->l_password->text());
        k.setValue("user",ui->l_username->text());
        k.setValue("AutoDeleteBackup",ui->cb_delete->checkState());
        k.endGroup();
    }
    if (Mode=="Load")
    {
        QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                      QSettings::NativeFormat );
        set.beginGroup("Startup");
        ui->cb_plts->setChecked(set.value("AutoPLTS").toBool());
        ui->cb_pltmh->setChecked(set.value("AutoPLTMH").toBool());
        ui->cb_inverter->setChecked(set.value("AutoINVERTER").toBool());
        ui->cb_load->setChecked(set.value("AutoLoad").toBool());

        ui->cb_archive->setChecked(set.value("AutoArchive").toBool());
        ui->timeEdit->setTime(set.value("AutoArchiveTime").toTime());
        ui->cb_archive_hmi->setChecked(set.value("AutoHMI").toBool());
        ui->cb_archive_xls->setChecked(set.value("AutoExel").toBool());
        ui->cb_archive_csv->setChecked(set.value("AutoCsv").toBool());

        ui->cb_Startup->setChecked(set.value("AutoStartup").toBool());
        // ui->cbSync->setChecked(set.value("ClockSync").toBool());

        set.endGroup();
        //================================================================
        QSettings setku("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                        QSettings::NativeFormat );
        setku.beginGroup("ProtocolConfiguration");

        ui->ip_plts->setText(setku.value("IP PLTS").toString());
        ui->ip_pltmh->setText(setku.value("IP PLTMH").toString());
        ui->ip_bdi->setText(setku.value("IP INVERTER").toString());
        ui->ip_load->setText(setku.value("IP LOAD").toString());

        ui->asdu_plts->setText(setku.value("ASDU PLTS").toString());
        ui->asdu_pltmh->setText(setku.value("ASDU PLTMH").toString());
        ui->asdu_bdi->setText(setku.value("ASDU INVERTER").toString());
        ui->asdu_load->setText(setku.value("ASDU LOAD").toString());

        ui->port_plts->setText(setku.value("PORT PLTS").toString());
        ui->port_pltmh->setText(setku.value("PORT PLTMH").toString());
        ui->port_inverter->setText(setku.value("PORT INVERTER").toString());
        ui->port_load->setText(setku.value("PORT LOAD").toString());

        ui->txttime->setText(setku.value("TIMESYNC").toString());


        setku.endGroup();
        //================================================================
        QSettings a("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                    QSettings::NativeFormat );
        a.beginGroup("DATABASE");
        ui->l_server->setText(a.value("hostserver").toString());
        ui->l_database->setText(a.value("database").toString());
        ui->l_database_2->setText(a.value("database2").toString());
        ui->l_password->setText(a.value("password").toString());
        ui->l_username->setText(a.value("user").toString());

        ui->cb_delete->setChecked(set.value("AutoDeleteBackup").toBool());
        a.endGroup();

        //================================================================
        // set default digital input
        //================================================================
        QSettings k1("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                     QSettings::NativeFormat );
        k1.beginGroup("Realtime Digital Input");
        // plts
        k1.setValue("DI_PLTS","0");
        k1.setValue("DI_PV_PLTS_1","0");
        k1.setValue("DI_PV_PLTS_2","0");
        k1.setValue("DI_PV_PLTS_3","0");
        // pltmh
        k1.setValue("DI_PLTMH","0");
        // inverter
        k1.setValue("DI_INVERTER","0");
        k1.setValue("DI_DC_INVERTER0","0");
        k1.setValue("DI_DC_INVERTER1","0");
        k1.setValue("DI_DC_INVERTER2","0");
        k1.setValue("DI_DC_INVERTER3","0");
        k1.setValue("DI_DC_INVERTER4","0");
        k1.setValue("DI_DC_INVERTER5","0");
        k1.setValue("DI_DC_INVERTER6","0");

        k1.setValue("DI_SW_INVERTER0","0");
        k1.setValue("DI_SW_INVERTER1","0");
        k1.setValue("DI_SW_INVERTER2","0");
        k1.setValue("DI_SW_INVERTER3","0");
        k1.setValue("DI_SW_INVERTER4","0");
        k1.setValue("DI_SW_INVERTER5","0");

        k1.setValue("DI_BATTERY","0");
        //load
        k1.setValue("DI_LOAD","0");

        k1.endGroup();

    }
    if (Mode=="Cons")
    {
        QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                      QSettings::NativeFormat );
        set.beginGroup("Startup");
        ui->cb_db->setChecked(set.value("AutoStartupDatabase").toBool());
        ui->cbSync->setChecked(set.value("ClockSync").toBool());
        set.endGroup();

        QSettings set2("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                       QSettings::NativeFormat );
        set2.beginGroup("ProtocolConfiguration");
        ui->txttime->setText(set2.value("TIMESYNC").toString());

        set2.endGroup();
    }

}

void main_104::AutoStartupOnWindows()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                       QSettings::NativeFormat);
    if (ui->cb_Startup->checkState())
    {

        settings.setValue("IEC104_Application",
                          QCoreApplication::applicationFilePath().replace('/', '\\'));
        ui->History->appendPlainText(timehmi+": Startup on Windows On");
    }
    else
    {
        settings.remove("IEC104_Application");
        ui->History->appendPlainText(timehmi+": Startup on Windows Off");
    }

}

void main_104::AutoSyncWindows()
{
    if (ui->cbSync->checkState())
    {
        ui->txttime->setEnabled(0);
        QStringList newzone;
        QStringList update;
        QStringList Start;
        QStringList Stop;
        QStringList Resync;
        newzone <<"w32tm /config /manualpeerlist:"+ui->txttime->text()+" /syncfromflags:manual /reliable:yes /update";         // add new timezone
        update <<"w32tm /config /update";      // update new timezone

        Start <<"net start w32time";           // stop services time
        Stop <<"net stop w32time";             // open services time
        Resync <<"w32tm /resync /nowait /force";      // sync new timezone

        QProcess p;
        p.start("cmd.exe", QStringList() << "/c" << newzone);
        if (p.waitForStarted())
        {
            p.waitForFinished();
            qDebug() <<"Command Sync Time Windows Succesfully"<< p.readAllStandardOutput();
            QProcess p1;
            p1.start("cmd.exe", QStringList() << "/c" << update);
            if (p1.waitForStarted())
            {
                p1.waitForFinished();
                qDebug() <<"Update Time Windows Succesfully"<< p1.readAllStandardOutput();

                QProcess p2;
                p2.start("cmd.exe", QStringList() << "/c" << Stop);
                if (p2.waitForStarted())
                {
                    p2.waitForFinished();
                    qDebug() <<"Stop Services W32tm"<< p2.readAllStandardOutput();
                    QProcess p3;
                    p3.start("cmd.exe", QStringList() << "/c" << Start);
                    if (p3.waitForStarted())
                    {
                        p3.waitForFinished();
                        qDebug() <<"Start Services W32tm"<< p3.readAllStandardOutput();
                        QProcess p4;
                        p4.start("cmd.exe", QStringList() << "/c" << Resync);
                        if (p4.waitForStarted())
                        {
                            p4.waitForFinished();
                            qDebug() <<"Resync W32tm Succesfully"<< p4.readAllStandardOutput();
                            ui->History->appendPlainText(timehmi+": Sync to NTP Server Succesfully with IP : "+ui->txttime->text());
                        }
                        else
                        {
                            qDebug() <<"Error Resync W32tm"<< p4.readAllStandardError();
                        }
                    }
                    else
                    {
                        qDebug() <<"Error Start Services W32tm"<< p3.readAllStandardError();
                    }
                }

            }
        }
    }
    else
    {
        ui->txttime->setEnabled(1);
    }
}

void main_104::ArchiveIntegration()
{
    QSqlQuery qry;
    QString val,path,time,id;
    qry.prepare("select id,outvalue from all_control_protocol where status='Data_Archive' order by id desc limit 1");
    qry.exec();
    if (qry.next())
    {
        id=qry.value(0).toString();
        val=qry.value(1).toString();
        if (id!=temp)
        {
            QFile file( "C:\\IEC_104\\Backup\\SQL\\TRENDING\\Archive-"+val+".sql.gz" );
            if( !file.exists() )
            {
                qDebug() << "The file" << file.fileName() << "does not exist.";
                insert_status_alloutcontrol("insert into alloutcontrol (timestamps,operatorname,status,outvalue,ioa,information) values(:p_1,:p_2,:p_3,:p_4,:p_5,:p_6)",timehmi,"System Protocol","STATUS_ARCHIVE",val,"Not Avaliable","UNARCHIVE "+time+" NOK !");
            }
            else
            {
                qDebug() << "The file" << file.fileName() << "exist.";
                QProcess p;
                QString dir="gzip --uncompress < ""C:\\IEC_104\\Backup\\SQL\\TRENDING\\Archive-"+val+".sql.gz | mysql -u "+ui->l_username->text()+" -p"+ui->l_password->text()+" "+ui->l_database_2->text()+"";
                p.start("cmd.exe", QStringList() << "/c" << dir);
                if (p.waitForStarted())
                {
                    p.waitForFinished();
                    insert_status_alloutcontrol("insert into alloutcontrol (timestamps,operatorname,status,outvalue,ioa,information) values(:p_1,:p_2,:p_3,:p_4,:p_5,:p_6)",timehmi,"System Protocol","STATUS_ARCHIVE",val,"Not Avaliable","UNARCHIVE "+time+"OK !");
                }

            }
        }
        temp=id;
    }

}


void main_104::get_status_running()
{

}

void main_104::set_backup_List(QString Mode)
{
    QDir().mkdir("C:\\IEC_104\\Backup\\");
    QDir().mkdir("C:\\IEC_104\\Backup\\EXCEL\\");
    QDir().mkdir("C:\\IEC_104\\Backup\\CSV\\");

    QDir().mkdir("C:\\IEC_104\\Backup\\EXCEL\\"""+GlobalTime+"");
    QDir().mkdir("C:\\IEC_104\\Backup\\EXCEL\\"""+GlobalTime+"\\""PLTS\\");
    QDir().mkdir("C:\\IEC_104\\Backup\\EXCEL\\"""+GlobalTime+"\\""PLTMH\\");
    QDir().mkdir("C:\\IEC_104\\Backup\\EXCEL\\"""+GlobalTime+"\\""INVERTER\\");
    QDir().mkdir("C:\\IEC_104\\Backup\\EXCEL\\"""+GlobalTime+"\\""LOAD\\");

    QDir().mkdir("C:\\IEC_104\\Backup\\CSV\\"""+GlobalTime+"");
    QDir().mkdir("C:\\IEC_104\\Backup\\CSV\\"""+GlobalTime+"\\""PLTS\\");
    QDir().mkdir("C:\\IEC_104\\Backup\\CSV\\"""+GlobalTime+"\\""PLTMH\\");
    QDir().mkdir("C:\\IEC_104\\Backup\\CSV\\"""+GlobalTime+"\\""INVERTER\\");
    QDir().mkdir("C:\\IEC_104\\Backup\\CSV\\"""+GlobalTime+"\\""LOAD\\");

    if (Mode=="XLS")
    {
        QString ActivePlts="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//PLTS//ACTIVE PLTS "+GlobalTime+".xls";
        QString ReactivePlts="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//PLTS//REACTIVE PLTS "+GlobalTime+".xls";
        QString InPlts="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//PLTS//IN POWER PLTS "+GlobalTime+".xls";
        QString MonPlts="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//PLTS//MON PLTS "+GlobalTime+".xls";
        QString CosphiPlts="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//PLTS//COSPHI PLTS "+GlobalTime+".xls";
        QString RadPlts="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//PLTS//RADIATION PLTS "+GlobalTime+".xls";

        QString ActivePltmh="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//PLTMH//ACTIVE PLTMH "+GlobalTime+".xls";
        QString ReactivePltmh="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//PLTMH//REACTIVE PLTMH "+GlobalTime+".xls";
        QString InPltmh="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//PLTMH//IN POWER PLTMH "+GlobalTime+".xls";
        QString MonPltmh="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//PLTMH//MON PLTMH "+GlobalTime+".xls";
        QString CosphiPltmh="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//PLTMH//COSPHI PLTMH "+GlobalTime+".xls";

        QString ActiveInverter="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//INVERTER//ACTIVE INVERTER "+GlobalTime+".xls";
        QString ReactiveInverter="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//INVERTER//REACTIVE INVERTER "+GlobalTime+".xls";
        QString InInverter="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//INVERTER//IN POWER INVERTER "+GlobalTime+".xls";
        QString MonInverter="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//INVERTER//MON INVERTER "+GlobalTime+".xls";
        QString CosphiInverter="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//INVERTER//COSPHI INVERTER "+GlobalTime+".xls";

        QString ActiveLoad="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//LOAD//ACTIVE LOAD "+GlobalTime+".xls";
        QString ReactiveLoad="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//LOAD//REACTIVE LOAD "+GlobalTime+".xls";
        QString InLoad="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//LOAD//IN POWER LOAD "+GlobalTime+".xls";
        QString MonLoad="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//LOAD//MON LOAD "+GlobalTime+".xls";
        QString CosphiLoad="C://IEC_104/Backup//EXCEL//"+GlobalTime+"//LOAD//COSPHI LOAD "+GlobalTime+".xls";


        // backup exel plts
        set_ActivePLTS("SELECT 'TimeProtocol','TimePLC','ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ActiveR, ActiveS, ActiveT FROM mon_active_plts order by id desc) a INTO OUTFILE '"+ActivePlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_ReactivePLTS("SELECT 'TimeProtocol','TimePLC','REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_plts order by id desc) a INTO OUTFILE '"+ReactivePlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_InPowerPLTS("SELECT 'TimeProtocol','TimePLC','PLTS W', 'PLTS VAR' UNION ALL SELECT * from (SELECT timestamp,timestamps,pltsw, pltsvar FROM in_power_plts order by id desc) a INTO OUTFILE '"+InPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_MonPLTS("SELECT 'TimeProtocol','TimePLC','PLTS VR','PLTS VS','PLTS VT','PLTS AR','PLTS AS','PLTS AT' UNION ALL SELECT * from (SELECT timestamp,timestamps,pltsVR, pltsVS,pltsVT,pltsAR,pltsAS,pltsAT FROM mon_plts order by id desc) a INTO OUTFILE '"+MonPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_CosphiPLTS("SELECT 'TimeProtocol','TimePLC','PLTS Cosphi R','PLTS Cosphi S','PLTS Cosphi T' UNION ALL SELECT * from (SELECT timestamp,timestamps,CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_plts order by id desc) a INTO OUTFILE '"+CosphiPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_RadiationPLTS("SELECT 'TimeProtocol','TimePLC','Radiation' UNION ALL SELECT * from (SELECT timestamp,timestamps,radiation FROM mon_radiation order by id desc) a INTO OUTFILE '"+RadPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");

        // backup exel pltmh
        set_ActivePLTMH("SELECT 'TimeProtocol','TimePLC','ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ActiveR, ActiveS, ActiveT FROM mon_active_pltmh order by id desc) a INTO OUTFILE '"+ActivePltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_ReactivePLTMH("SELECT 'TimeProtocol','TimePLC','REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_pltmh order by id desc) a INTO OUTFILE '"+ReactivePltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_InPowerPLTMH("SELECT 'TimeProtocol','TimePLC','PLTMH W', 'PLTMH VAR' UNION ALL SELECT * from (SELECT timestamp,timestamps,pltmhw, pltmhvar FROM in_power_pltmh order by id desc) a INTO OUTFILE '"+InPltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_MonPLTMH("SELECT 'TimeProtocol','TimePLC','PLTMH VR','PLTMH VS','PLTMH VT','PLTMH AR','PLTMH AS','PLTMH AT' UNION ALL SELECT * from (SELECT timestamp,timestamps,pltmhVR, pltmhVS,pltmhVT,pltmhAR,pltmhAS,pltmhAT FROM mon_pltmh order by id desc) a INTO OUTFILE '"+MonPltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_CosphiPLTMH("SELECT 'TimeProtocol','TimePLC','PLTMH Cosphi R','PLTMH Cosphi S','PLTMH Cosphi T' UNION ALL SELECT * from (SELECT timestamp,timestamps,CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_pltmh order by id desc) a INTO OUTFILE '"+CosphiPltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");

        // backup exel inverter
        set_ActiveINVERTER("SELECT 'TimeProtocol','TimePLC','ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ActiveR, ActiveS, ActiveT FROM mon_active_inverter order by id desc) a INTO OUTFILE '"+ActiveInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_ReactiveINVERTER("SELECT 'TimeProtocol','TimePLC','REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_inverter order by id desc) a INTO OUTFILE '"+ReactiveInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_InPowerINVERTER("SELECT 'TimeProtocol','TimePLC','INVERTER W', 'INVERTER VAR' UNION ALL SELECT * from (SELECT timestamp,timestamps,invw, invvar FROM in_power_inverter order by id desc) a INTO OUTFILE '"+InInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_MonINVERTER("SELECT 'TimeProtocol','TimePLC','INVERTER VR','INVERTER VS','INVERTER VT','INVERTER AR','INVERTER AS','INVERTER AT' UNION ALL SELECT * from (SELECT timestamp,timestamps,invVR, invVS,invVT,invAR,invAS,invAT FROM mon_inverter order by id desc) a INTO OUTFILE '"+MonInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_CosphiINVERTER("SELECT 'TimeProtocol','TimePLC','INVERTER Cosphi R','INVERTER Cosphi S','INVERTER Cosphi T' UNION ALL SELECT * from (SELECT timestamp,timestamps,CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_inverter order by id desc) a INTO OUTFILE '"+CosphiInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");

        // backup exel load
        set_ActiveLOAD("SELECT 'TimeProtocol','TimePLC','ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ActiveR, ActiveS, ActiveT FROM mon_active_load order by id desc) a INTO OUTFILE '"+ActiveLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_ReactiveLOAD("SELECT 'TimeProtocol','TimePLC','REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_load order by id desc) a INTO OUTFILE '"+ReactiveLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_InPowerLOAD("SELECT 'TimeProtocol','TimePLC','LOAD W', 'LOAD VAR' UNION ALL SELECT * from (SELECT timestamp,timestamps,loadw, loadvar FROM in_power_load order by id desc) a INTO OUTFILE '"+InLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_MonLOAD("SELECT 'TimeProtocol','TimePLC','LOAD VR','LOAD VS','LOAD VT','LOAD AR','LOAD AS','LOAD AT' UNION ALL SELECT * from (SELECT timestamp,timestamps,loadVR, loadVS,loadVT,loadAR,loadAS,loadAT FROM mon_load order by id desc) a INTO OUTFILE '"+MonLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_CosphiLOAD("SELECT 'TimeProtocol','TimePLC','LOAD Cosphi R','LOAD Cosphi S','LOAD Cosphi T' UNION ALL SELECT * from (SELECT timestamp,timestamps,CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_load order by id desc) a INTO OUTFILE '"+CosphiLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
    }

    if (Mode=="CSV")
    {
        QString ActivePlts="C://IEC_104/Backup//CSV//"+GlobalTime+"//PLTS//ACTIVE PLTS "+GlobalTime+".csv";
        QString ReactivePlts="C://IEC_104/Backup//CSV//"+GlobalTime+"//PLTS//REACTIVE PLTS "+GlobalTime+".csv";
        QString InPlts="C://IEC_104/Backup//CSV//"+GlobalTime+"//PLTS//IN POWER PLTS "+GlobalTime+".csv";
        QString MonPlts="C://IEC_104/Backup//CSV//"+GlobalTime+"//PLTS//MON PLTS "+GlobalTime+".csv";
        QString CosphiPlts="C://IEC_104/Backup//CSV//"+GlobalTime+"//PLTS//COSPHI PLTS "+GlobalTime+".csv";
        QString RadPlts="C://IEC_104/Backup//CSV//"+GlobalTime+"//PLTS//RADIATION PLTS "+GlobalTime+".csv";

        QString ActivePltmh="C://IEC_104/Backup//CSV//"+GlobalTime+"//PLTMH//ACTIVE PLTMH "+GlobalTime+".csv";
        QString ReactivePltmh="C://IEC_104/Backup//CSV//"+GlobalTime+"//PLTMH//REACTIVE PLTMH "+GlobalTime+".csv";
        QString InPltmh="C://IEC_104/Backup//CSV//"+GlobalTime+"//PLTMH//IN POWER PLTMH "+GlobalTime+".csv";
        QString MonPltmh="C://IEC_104/Backup//CSV//"+GlobalTime+"//PLTMH//MON PLTMH "+GlobalTime+".csv";
        QString CosphiPltmh="C://IEC_104/Backup//CSV//"+GlobalTime+"//PLTMH//COSPHI PLTMH "+GlobalTime+".csv";

        QString ActiveInverter="C://IEC_104/Backup//CSV//"+GlobalTime+"//INVERTER//ACTIVE INVERTER "+GlobalTime+".csv";
        QString ReactiveInverter="C://IEC_104/Backup//CSV//"+GlobalTime+"//INVERTER//REACTIVE INVERTER "+GlobalTime+".csv";
        QString InInverter="C://IEC_104/Backup//CSV//"+GlobalTime+"//INVERTER//IN POWER INVERTER "+GlobalTime+".csv";
        QString MonInverter="C://IEC_104/Backup//CSV//"+GlobalTime+"//INVERTER//MON INVERTER "+GlobalTime+".csv";
        QString CosphiInverter="C://IEC_104/Backup//CSV//"+GlobalTime+"//INVERTER//COSPHI INVERTER "+GlobalTime+".csv";

        QString ActiveLoad="C://IEC_104/Backup//CSV//"+GlobalTime+"//LOAD//ACTIVE LOAD "+GlobalTime+".csv";
        QString ReactiveLoad="C://IEC_104/Backup//CSV//"+GlobalTime+"//LOAD//REACTIVE LOAD "+GlobalTime+".csv";
        QString InLoad="C://IEC_104/Backup//CSV//"+GlobalTime+"//LOAD//IN POWER LOAD "+GlobalTime+".csv";
        QString MonLoad="C://IEC_104/Backup//CSV//"+GlobalTime+"//LOAD//MON LOAD "+GlobalTime+".csv";
        QString CosphiLoad="C://IEC_104/Backup//CSV//"+GlobalTime+"//LOAD//COSPHI LOAD "+GlobalTime+".csv";

        // backup exel plts
        set_ActivePLTS("SELECT 'TimeProtocol','TimePLC','ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ActiveR, ActiveS, ActiveT FROM mon_active_plts order by id desc) a INTO OUTFILE '"+ActivePlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_ReactivePLTS("SELECT 'TimeProtocol','TimePLC','REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_plts order by id desc) a INTO OUTFILE '"+ReactivePlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_InPowerPLTS("SELECT 'TimeProtocol','TimePLC','PLTS W', 'PLTS VAR' UNION ALL SELECT * from (SELECT timestamp,timestamps,pltsw, pltsvar FROM in_power_plts order by id desc) a INTO OUTFILE '"+InPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_MonPLTS("SELECT 'TimeProtocol','TimePLC','PLTS VR','PLTS VS','PLTS VT','PLTS AR','PLTS AS','PLTS AT' UNION ALL SELECT * from (SELECT timestamp,timestamps,pltsVR, pltsVS,pltsVT,pltsAR,pltsAS,pltsAT FROM mon_plts order by id desc) a INTO OUTFILE '"+MonPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_CosphiPLTS("SELECT 'TimeProtocol','TimePLC','PLTS Cosphi R','PLTS Cosphi S','PLTS Cosphi T' UNION ALL SELECT * from (SELECT timestamp,timestamps,CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_plts order by id desc) a INTO OUTFILE '"+CosphiPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_RadiationPLTS("SELECT 'TimeProtocol','TimePLC','Radiation' UNION ALL SELECT * from (SELECT timestamp,timestamps,radiation FROM mon_radiation order by id desc) a INTO OUTFILE '"+RadPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");

        // backup exel pltmh
        set_ActivePLTMH("SELECT 'TimeProtocol','TimePLC','ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ActiveR, ActiveS, ActiveT FROM mon_active_pltmh order by id desc) a INTO OUTFILE '"+ActivePltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_ReactivePLTMH("SELECT 'TimeProtocol','TimePLC','REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_pltmh order by id desc) a INTO OUTFILE '"+ReactivePltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_InPowerPLTMH("SELECT 'TimeProtocol','TimePLC','PLTMH W', 'PLTMH VAR' UNION ALL SELECT * from (SELECT timestamp,timestamps,pltmhw, pltmhvar FROM in_power_pltmh order by id desc) a INTO OUTFILE '"+InPltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_MonPLTMH("SELECT 'TimeProtocol','TimePLC','PLTMH VR','PLTMH VS','PLTMH VT','PLTMH AR','PLTMH AS','PLTMH AT' UNION ALL SELECT * from (SELECT timestamp,timestamps,pltmhVR, pltmhVS,pltmhVT,pltmhAR,pltmhAS,pltmhAT FROM mon_pltmh order by id desc) a INTO OUTFILE '"+MonPltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_CosphiPLTMH("SELECT 'TimeProtocol','TimePLC','PLTMH Cosphi R','PLTMH Cosphi S','PLTMH Cosphi T' UNION ALL SELECT * from (SELECT timestamp,timestamps,CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_pltmh order by id desc) a INTO OUTFILE '"+CosphiPltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");

        // backup exel inverter
        set_ActiveINVERTER("SELECT 'TimeProtocol','TimePLC','ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ActiveR, ActiveS, ActiveT FROM mon_active_inverter order by id desc) a INTO OUTFILE '"+ActiveInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_ReactiveINVERTER("SELECT 'TimeProtocol','TimePLC','REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_inverter order by id desc) a INTO OUTFILE '"+ReactiveInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_InPowerINVERTER("SELECT 'TimeProtocol','TimePLC','INVERTER W', 'INVERTER VAR' UNION ALL SELECT * from (SELECT timestamp,timestamps,invw, invvar FROM in_power_inverter order by id desc) a INTO OUTFILE '"+InInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_MonINVERTER("SELECT 'TimeProtocol','TimePLC','INVERTER VR','INVERTER VS','INVERTER VT','INVERTER AR','INVERTER AS','INVERTER AT' UNION ALL SELECT * from (SELECT timestamp,timestamps,invVR, invVS,invVT,invAR,invAS,invAT FROM mon_inverter order by id desc) a INTO OUTFILE '"+MonInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_CosphiINVERTER("SELECT 'TimeProtocol','TimePLC','INVERTER Cosphi R','INVERTER Cosphi S','INVERTER Cosphi T' UNION ALL SELECT * from (SELECT timestamp,timestamps,CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_inverter order by id desc) a INTO OUTFILE '"+CosphiInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");

        // backup exel load
        set_ActiveLOAD("SELECT 'TimeProtocol','TimePLC','ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ActiveR, ActiveS, ActiveT FROM mon_active_load order by id desc) a INTO OUTFILE '"+ActiveLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_ReactiveLOAD("SELECT 'TimeProtocol','TimePLC','REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT timestamp,timestamps,ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_load order by id desc) a INTO OUTFILE '"+ReactiveLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_InPowerLOAD("SELECT 'TimeProtocol','TimePLC','LOAD W', 'LOAD VAR' UNION ALL SELECT * from (SELECT timestamp,timestamps,loadw, loadvar FROM in_power_load order by id desc) a INTO OUTFILE '"+InLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_MonLOAD("SELECT 'TimeProtocol','TimePLC','LOAD VR','LOAD VS','LOAD VT','LOAD AR','LOAD AS','LOAD AT' UNION ALL SELECT * from (SELECT timestamp,timestamps,loadVR, loadVS,loadVT,loadAR,loadAS,loadAT FROM mon_load order by id desc) a INTO OUTFILE '"+MonLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        set_CosphiLOAD("SELECT 'TimeProtocol','TimePLC','LOAD Cosphi R','LOAD Cosphi S','LOAD Cosphi T' UNION ALL SELECT * from (SELECT timestamp,timestamps,CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_load order by id desc) a INTO OUTFILE '"+CosphiLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");



        //        // backup csv plts
        //        set_ActivePLTS("SELECT 'ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT ActiveR, ActiveS, ActiveT FROM mon_active_plts order by id desc) a INTO OUTFILE '"+ActivePlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_ReactivePLTS("SELECT 'REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_plts order by id desc) a INTO OUTFILE '"+ReactivePlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_InPowerPLTS("SELECT 'PLTS W', 'PLTS VAR' UNION ALL SELECT * from (SELECT pltsw, pltsvar FROM in_power_plts order by id desc) a INTO OUTFILE '"+InPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_MonPLTS("SELECT 'PLTS VR','PLTS VS','PLTS VT','PLTS AR','PLTS AS','PLTS AT' UNION ALL SELECT * from (SELECT pltsVR, pltsVS,pltsVT,pltsAR,pltsAS,pltsAT FROM mon_plts order by id desc) a INTO OUTFILE '"+MonPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_CosphiPLTS("SELECT 'PLTS Cosphi R','PLTS Cosphi S','PLTS Cosphi T' UNION ALL SELECT * from (SELECT CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_plts order by id desc) a INTO OUTFILE '"+CosphiPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_RadiationPLTS("SELECT 'Radiation' UNION ALL SELECT * from (SELECT radiation FROM mon_radiation order by id desc) a INTO OUTFILE '"+RadPlts+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");

        //        // backup csv pltmh
        //        set_ActivePLTMH("SELECT 'ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT ActiveR, ActiveS, ActiveT FROM mon_active_pltmh order by id desc) a INTO OUTFILE '"+ActivePltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_ReactivePLTMH("SELECT 'REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_pltmh order by id desc) a INTO OUTFILE '"+ReactivePltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_InPowerPLTMH("SELECT 'PLTMH W', 'PLTMH VAR' UNION ALL SELECT * from (SELECT pltmhw, pltmhvar FROM in_power_pltmh order by id desc) a INTO OUTFILE '"+InPltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_MonPLTMH("SELECT 'PLTMH VR','PLTMH VS','PLTMH VT','PLTMH AR','PLTMH AS','PLTMH AT' UNION ALL SELECT * from (SELECT pltmhVR, pltmhVS,pltmhVT,pltmhAR,pltmhAS,pltmhAT FROM mon_pltmh order by id desc) a INTO OUTFILE '"+MonPltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_CosphiPLTMH("SELECT 'PLTMH Cosphi R','PLTMH Cosphi S','PLTMH Cosphi T' UNION ALL SELECT * from (SELECT CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_pltmh order by id desc) a INTO OUTFILE '"+CosphiPltmh+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");

        //        // backup csv inverter
        //        set_ActiveINVERTER("SELECT 'ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT ActiveR, ActiveS, ActiveT FROM mon_active_inverter order by id desc) a INTO OUTFILE '"+ActiveInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_ReactiveINVERTER("SELECT 'REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_inverter order by id desc) a INTO OUTFILE '"+ReactiveInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_InPowerINVERTER("SELECT 'INVERTER W', 'INVERTER VAR' UNION ALL SELECT * from (SELECT invw, invvar FROM in_power_inverter order by id desc) a INTO OUTFILE '"+InInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_MonINVERTER("SELECT 'INVERTER VR','INVERTER VS','INVERTER VT','INVERTER AR','INVERTER AS','INVERTER AT' UNION ALL SELECT * from (SELECT invVR, invVS,invVT,invAR,invAS,invAT FROM mon_inverter order by id desc) a INTO OUTFILE '"+MonInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_CosphiINVERTER("SELECT 'INVERTER Cosphi R','INVERTER Cosphi S','INVERTER Cosphi T' UNION ALL SELECT * from (SELECT CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_inverter order by id desc) a INTO OUTFILE '"+CosphiInverter+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");

        //        // backup csv load
        //        set_ActiveLOAD("SELECT 'ACTIVE R', 'ACTIVE S', 'ACTIVE T' UNION ALL SELECT * from (SELECT ActiveR, ActiveS, ActiveT FROM mon_active_load order by id desc) a INTO OUTFILE '"+ActiveLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_ReactiveLOAD("SELECT 'REACTIVE R', 'REACTIVE S', 'REACTIVE T' UNION ALL SELECT * from (SELECT ReactiveR, ReactiveS, ReactiveT FROM mon_reactive_load order by id desc) a INTO OUTFILE '"+ReactiveLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_InPowerLOAD("SELECT 'LOAD W', 'LOAD VAR' UNION ALL SELECT * from (SELECT loadw, loadvar FROM in_power_load order by id desc) a INTO OUTFILE '"+InLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_MonLOAD("SELECT 'LOAD VR','LOAD VS','LOAD VT','LOAD AR','LOAD AS','LOAD AT' UNION ALL SELECT * from (SELECT loadVR, loadVS,loadVT,loadAR,loadAS,loadAT FROM mon_load order by id desc) a INTO OUTFILE '"+MonLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");
        //        set_CosphiLOAD("SELECT 'LOAD Cosphi R','LOAD Cosphi S','LOAD Cosphi T' UNION ALL SELECT * from (SELECT CosPhiR, CosPhiS,CosPhiT FROM mon_cosphi_load order by id desc) a INTO OUTFILE '"+CosphiLoad+"' FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\\n';");

        //  }
    }
}

void main_104::set_ActivePLTS(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_ReactivePLTS(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_InPowerPLTS(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_MonPLTS(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_CosphiPLTS(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_RadiationPLTS(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_ActivePLTMH(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_ReactivePLTMH(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_InPowerPLTMH(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_MonPLTMH(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_CosphiPLTMH(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_ActiveINVERTER(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_ReactiveINVERTER(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_InPowerINVERTER(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_MonINVERTER(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_CosphiINVERTER(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_ActiveLOAD(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_ReactiveLOAD(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_InPowerLOAD(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_MonLOAD(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::set_CosphiLOAD(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.exec();
}

void main_104::insert_status_alloutcontrol(QString Query,QString Timestamp,QString Operator,QString Status,QString value,QString IOA,QString Info)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1",Timestamp);
    qry.bindValue(":p_2",Operator);
    qry.bindValue(":p_3",Status);
    qry.bindValue(":p_4",value);
    qry.bindValue(":p_5",IOA);
    qry.bindValue(":p_6",Info);
    qry.exec();
}

void main_104::set_TruncateTable()
{
    set_PLTS();
    ui->History->appendPlainText(timehmi+": Clear PLTS Table After Backup Sucessfully !");
    set_PLTMH();
    ui->History->appendPlainText(timehmi+": Clear PLTMH Table After Backup Sucessfully !");
    set_INVERTER();
    ui->History->appendPlainText(timehmi+": Clear INVERTER Table After Backup Sucessfully !");
    set_LOAD();
    ui->History->appendPlainText(timehmi+": Clear LOAD Table After Backup Sucessfully !");
    set_Control();
    ui->History->appendPlainText(timehmi+": Clear Control Table After Backup Sucessfully !");
}

void main_104::set_PLTS()
{
    set_TruncatePLTSActive("truncate mon_active_plts");
    set_TruncatePLTSReactive("truncate mon_reactive_plts");
    set_TruncatePLTSinPower("truncate in_power_plts");
    set_TruncatePLTSCosphi("truncate mon_cosphi_plts");
    set_TruncatePLTSMon("truncate mon_plts");
    set_TruncatePLTSRadiation("truncate mon_radiation");
}

void main_104::set_PLTMH()
{
    set_TruncatePLTMHActive("truncate mon_active_pltmh");
    set_TruncatePLTMHReactive("truncate mon_reactive_pltmh");
    set_TruncatePLTMHinPower("truncate in_power_pltmh");
    set_TruncatePLTMHCosphi("truncate mon_cosphi_pltmh");
    set_TruncatePLTMHMon("truncate mon_pltmh");
}

void main_104::set_INVERTER()
{
    set_TruncateINVERTERActive("truncate mon_active_inverter");
    set_TruncateINVERTERReactive("truncate mon_reactive_inverter");
    set_TruncateINVERTERinPower("truncate in_power_inverter");
    set_TruncateINVERTERCosphi("truncate mon_cosphi_inverter");
    set_TruncateINVERTERMon("truncate mon_inverter");
}

void main_104::set_LOAD()
{
    set_TruncateLOADActive("truncate mon_active_load");
    set_TruncateLOADReactive("truncate mon_reactive_load");
    set_TruncateLOADinPower("truncate in_power_load");
    set_TruncateLOADCosphi("truncate mon_cosphi_load");
    set_TruncateLOADMon("truncate mon_load");
}

void main_104::set_Control()
{
    set_TruncateControl("truncate all_control_protocol");
}

void main_104::set_TruncatePLTSActive(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        //  QString a=qDebug()<<qry.lastError();
        qDebug()<<"Truncate In Power PLTS Active Fail";
        // SetLoggerNow("Truncate In Power PLTS Active Fail",a);

    }
}

void main_104::set_TruncatePLTSReactive(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power PLTS Reactive Fail";
        // SetLoggerNow("Truncate In Power PLTS Reactive Fail",qry.lastError());
    }
}

void main_104::set_TruncatePLTSinPower(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power PLTS In Power Fail";
        //   SetLoggerNow("Truncate In Power PLTS In Power Fail",""+qry.lastError()+"");
    }
}

void main_104::set_TruncatePLTSCosphi(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power PLTS Cosphi Fail";
        //   SetLoggerNow("Truncate In Power PLTS Cosphi Fail",qry.lastError());
    }
}

void main_104::set_TruncatePLTSMon(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power PLTS Monitor Fail";
        //   SetLoggerNow("Truncate In Power PLTS Monitor Fail",qry.lastError());
    }
}

void main_104::set_TruncatePLTSRadiation(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power PLTS Radiation Fail";
        //    SetLoggerNow("Truncate In Power PLTS Radiation Fail",qry.lastError());
    }
}

void main_104::set_TruncatePLTMHActive(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power PLTMH Active Fail";
        //   SetLoggerNow("Truncate In Power PLTMH Active Fail",qry.lastError());
    }

}

void main_104::set_TruncatePLTMHReactive(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power PLTMH Rective Fail";
        //  SetLoggerNow("Truncate In Power PLTMH Rective Fail",qry.lastError());
    }
}

void main_104::set_TruncatePLTMHinPower(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power PLTMH In Power Fail";
        //  SetLoggerNow("Truncate In Power PLTMH In Power Fail",qry.lastError());
    }
}

void main_104::set_TruncatePLTMHCosphi(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power PLTMH Cosphi Fail";
        // SetLoggerNow("Truncate In Power PLTMH Cosphi Fail",qry.lastError());
    }
}

void main_104::set_TruncatePLTMHMon(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power PLTMH Monitor Fail";
        //   SetLoggerNow("Truncate In Power PLTMH Monitor Fail",qry.lastError());
    }
}

void main_104::set_TruncateINVERTERActive(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power Inverter Active Fail";
        //  SetLoggerNow("Truncate In Power Inverter Active Fail",qry.lastError());
    }
}

void main_104::set_TruncateINVERTERReactive(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power Inverter Rective Fail";
        //   SetLoggerNow("Truncate In Power Inverter Rective Fail",qry.lastError());
    }
}

void main_104::set_TruncateINVERTERinPower(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power Inverter In Power Fail";
        //   SetLoggerNow("Truncate In Power Inverter In Power Fail",qry.lastError());
    }
}

void main_104::set_TruncateINVERTERCosphi(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power Inverter Cosphi Fail";
        //  SetLoggerNow("Truncate In Power Inverter Cosphi Fail",qry.lastError());
    }
}

void main_104::set_TruncateINVERTERMon(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power Inverter Monitor Fail";
        //  SetLoggerNow("Truncate In Power Inverter Monitor Fail",qry.lastError());
    }
}

void main_104::set_TruncateLOADActive(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power Load Active Fail";
        //   SetLoggerNow("Truncate In Power Load Active Fail",qry.lastError());
    }
}

void main_104::set_TruncateLOADReactive(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power Load Rective Fail";
        //  SetLoggerNow("Truncate In Power Load Rective Fail",qry.lastError());
    }
}

void main_104::set_TruncateLOADinPower(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power Load In Power Fail";
        //     SetLoggerNow("Truncate In Power Load In Power Fail",qry.lastError());
    }
}

void main_104::set_TruncateLOADCosphi(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power Load Cosphi Fail";
        // SetLoggerNow("Truncate In Power Load Cosphi Fail",qry.lastError());
    }
}

void main_104::set_TruncateLOADMon(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate In Power Load Monitor Fail";
        //  SetLoggerNow("Truncate In Power Load Monitor Fail",qry.lastError());
    }
}

void main_104::set_TruncateControl(QString Query)
{
    QSqlQuery qry;
    qry.prepare(Query) ;
    qry.exec();
    if (!qry.exec())
    {
        qDebug()<<"Truncate Control Fail";
    }
}

void main_104::Log_db(QString Query, QString Timestamp, QString Status, QString Information)
{
    QSqlQuery qry;
    qry.prepare(Query);
    qry.bindValue(":p_1",Timestamp);
    qry.bindValue(":p_2",Status);
    qry.bindValue(":p_3",Information);
    qry.exec();
}

void main_104::SetLoggerNow(QString Status, QString Information)
{
    Log_db("insert into in_log_protocol (timestamps,status,information) values (:p_1,:p_2,:p_3)",
           timestamp,
           Status,
           Information);
}

void main_104::on_pb_db_2_clicked()
{
    ui->History-> clear();
}

void main_104::RepairOnSrartup()
{
    QProcess *p= new QProcess(this);
    QString Repair="mysqlcheck --auto-repair -o --databases "+ui->l_database->text()+" "+ui->l_database_2->text()+" -u "+ui->l_username->text()+" -p"+ui->l_password->text()+"";
    //====================================================================
    //repair on startup
    //====================================================================
    p->start("cmd.exe", QStringList() << "/c" << Repair);
    if (p->waitForStarted())
    {
        p->waitForFinished();
        // repair table
        qDebug() << p->readAllStandardOutput();
        AutoSyncWindows();
    }
}



void main_104::on_cb_delete_clicked()
{
   //   ConfigStartup("Save");
}
