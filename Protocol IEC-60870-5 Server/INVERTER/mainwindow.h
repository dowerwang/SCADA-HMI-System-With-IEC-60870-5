/*
 * This software implements an IEC 60870-5-104 protocol tester.
 * Copyright � 2010,2011,2012 Ricardo L. Olsen
 *
 * Disclaimer
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QPushButton>
#include <QTimer>
#include <QSettings>
#include <QtWidgets/QTableWidgetItem>
#include <map>
#include "bdtr.h"
#include "iec104_class.h"
#include "qiec104.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent( QCloseEvent *event );

public:
    MainWindow( QWidget *parent = 0 );
    ~MainWindow();

private slots:
    //void on_pbConnect_clicked(); // connect button pressed
    void slot_timer_BDTR_kamsg(); // timer for sending keepalive BDTR messages
    void slot_BDTR_pronto_para_ler();  // BDTR: sinal para leitura de dados no tcp do BDTR
    void slot_dataIndication( iec_obj *obj, int numpoints );
    void slot_interrogationActConfIndication();
    void slot_interrogationActTermIndication();
    void slot_tcpconnect();         // tcp connect for iec104
    void slot_tcpdisconnect();      // tcp disconnect for iec104
    void slot_commandActConfIndication( iec_obj *obj );
    //send value iec from db
    void Trigger_SendValueToIEC();
    //get value from hmi
    void GetValue();
    //==============================================================================
    // SET TO DB STATUS DATA IN  AND STATUS CB FROM IEC 104
    //==============================================================================
    void INSERTALLDATA();
    //==============================================================================
    // GET STATUS DATA IN  AND STATUS CB FROM IEC 104
    //==============================================================================
    void DataIn_INVERTER(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_BATTERY(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void Datain_RTU_INVERTER(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN);
    void Datain_CB_INVERTER(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1);
    void Datain_CB_BATTERY(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN);
    //==============================================================================
    // GET STATUS DATA  W VAR R S T FROM IEC 104
    //==============================================================================
    void Datain_INVERTER_W_VAR  (QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2);
    void Datain_INVERTER_W_VAR_R_1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2);
    void Datain_INVERTER_W_VAR_S_1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2);
    void Datain_INVERTER_W_VAR_T_1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2);

    void Datain_INVERTER_W_VAR_R_2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2);
    void Datain_INVERTER_W_VAR_S_2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2);
    void Datain_INVERTER_W_VAR_T_2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2);

    void Datain_BATTERY_W       (QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN);
    //==============================================================================
    // GET STATUS DATA IN FAULT AND SWITCH FROM IEC 104
    //==============================================================================
    void DataIn_BATTERY_SPV(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_BATTERY_COM(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_FAULTR_1(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_FAULTS_1(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_FAULTT_1(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_FAULTR_2(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_FAULTS_2(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_FAULTT_2(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);

    void DataIn_INVERTER_SWITCH(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_SWITCH1(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_SWITCH2(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_SWITCH3(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_SWITCH4(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_SWITCH5(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_SWITCH6(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);

    void DataIn_INVERTER_DC1(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_DC2(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_DC3(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_DC4(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_DC5(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_DC6(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    //==============================================================================
    // GET STATUS FROM RABBIT INVERTER SPV
    //==============================================================================
    void DataIn_INVERTER_SPV_R1(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_SPV_R2(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_SPV_S1(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_SPV_S2(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_SPV_T1(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_SPV_T2(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    //==============================================================================
    // GET STATUS FROM RABBIT INVERTER CONTROL CB ALARM
    //==============================================================================
    void DataIn_INVERTER_STATE_R1(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_STATE_R2(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_STATE_S1(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_STATE_S2(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_STATE_T1(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_STATE_T2(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    //==============================================================================
    // GET STATUS FROM INVERTER CURVE P AND Q
    //==============================================================================
    void DataIn_INVERTER_CURVE_P(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void DataIn_INVERTER_CURVE_Q(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    //==============================================================================
    // GET STATUS DATA IN BATTERY RACK FROM IEC 104
    //==============================================================================
    void DataIn_Update_Bank_1(QString QueryUpdate, QString Timestamp, QString Volt,QString Temp,QString FaultV,QString FaultTmp);
    void DataIn_Update_Bank_2(QString QueryUpdate, QString Timestamp, QString Volt,QString Temp,QString FaultV,QString FaultTmp);
    void DataIn_Update_Bank_3(QString QueryUpdate, QString Timestamp, QString Volt,QString Temp,QString FaultV,QString FaultTmp);
    void DataIn_Update_Bank_4(QString QueryUpdate, QString Timestamp, QString Volt,QString Temp,QString FaultV,QString FaultTmp);
    void DataIn_Update_Bank_5(QString QueryUpdate, QString Timestamp, QString Volt,QString Temp,QString FaultV,QString FaultTmp);
    void DataIn_Update_Bank_6(QString QueryUpdate, QString Timestamp, QString Volt,QString Temp,QString FaultV,QString FaultTmp);
    void DataIn_Update_Bank_7(QString QueryUpdate, QString Timestamp, QString Volt,QString Temp,QString FaultV,QString FaultTmp);
    //==============================================================================
    // SET DATA BATTERY MONITORING
    //==============================================================================
    void DataIn_Battery_Status(QString QueryUpdate, QString Timestamp, QString FaultVolt, QString FaultCurrent, QString SOC,QString DOD,QString VTOT,QString VHVDC,QString ITOT);
    void DataIn_Battery_SOC(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN_1, QString Value_IN_2);
    void DataIn_Battery_DOD(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN_1, QString Value_IN_2);
    //==============================================================================
    // SET DATA INPUT FROM PLC
    //==============================================================================
    void DataIn_Status();
    void DataIn_Status_RTU();
    void DataIn_Status_CB();
    void DataIn_Status_SETPOINT();
    void DataIn_Status_IN_POWER();
    void DataIn_Status_BAT_FAULT();
    void DataIn_Status_SW_DC();
    void DataIn_Status_AC_SPV_BDI();
    void DataIn_Status_AC_STATE_BDI();
     void DataIn_Status_CURVE_P_AND_Q();
    void DataIn_Status_UpdateBank();
    void DataIn_Status_Battery();
    void DataIn_Frequency();
    void DataIn_DigitalInput_LogRegistery();
    //==============================================================================
    // GET CLOCK AND DB
    //==============================================================================
    void ClockSync_clicked(); // event get clock from PLC/IEC 104
    void on_pbConnect_2_clicked(); // save default configuration
    void load_Db(QString Host,QString Database,QString Username,QString Password);
    void ControllerDB();
    //==============================================================================
    //INSERT ALL DATA MONITORING RST
    //==============================================================================
    void InsertAllRST();
    void InsertINVERTER_Monitor(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString VR, QString VS, QString VT, QString AR, QString AS, QString AT);
    //==============================================================================
    //INSERT ALL DATA MONITORING RST ACTIVE REACTIVE
    //==============================================================================
    void InsertAllRST_ActiveReactive();
    void InsertINVERTER_Active(QString QueryCount,QString QueryInsert, QString Validation_IN,QString Timestamp,QString VR,QString VS,QString VT);
    void InsertINVERTER_ReActive(QString QueryCount,QString QueryInsert, QString Validation_IN,QString Timestamp,QString VR,QString VS,QString VT);
    //==============================================================================
    //INSERT ALL DATA MONITORING RST COS PHI
    //==============================================================================
    void InsertAllRST_COSPHI();
    void InsertINVERTER_COSPHI(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp,QString PHIR,QString PHIS,QString PHIT);
    //==============================================================================
    //LOG TO ALARMLIST
    //==============================================================================
    void Set_RTU_INVERTER(QString QueryInsert,QString Timestamp,QString Value_IN);
    void Set_CB_INVERTER(QString QueryInsert, QString Timestamp, QString Value_IN);
    void Set_CB_BATTERY(QString QueryInsert, QString Timestamp, QString Value_IN);
    //==============================================================================
    // DATA MONITORING INVERTER SETPOINT
    //==============================================================================
    void Set_Point_Mode(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void Set_Point_W(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void Set_Point_Var(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    //==============================================================================
    // DATA MONITORING FREQUENCY
    //==============================================================================
    void DataIn_Freq(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
private:
    Ui::MainWindow *ui;
    QIec104 i104;
    //==============================================================================
    // CHECK AFTER RTU DISCONNECTED
    //==============================================================================
    void ConnectionRTU();
    //=====================================================
    // CONTROL VALUE TO IEC 104 DIGITAL OUT
    //=====================================================
    // control all cb
    void SendAllValueToIEC_Control();
    // control all INVERTER
    void SendValueToIEC_INVERTER(QString Query, int IOA);
    void SendValueToIEC_INVERTER1(QString Query, int IOA);
    void SendValueToIEC_INVERTER2(QString Query, int IOA);
    void SendValueToIEC_INVERTER3(QString Query, int IOA);
    void SendValueToIEC_BATTERY(QString Query, int IOA);
    //=====================================================
    // CONTROL VALUE TO IEC 104 SET POINT
    //=====================================================
    // control all setpoint
    void SendAllValueToIEC_SetPoint();
    // control setpoint INVERTER
    void SendValueToIEC_INVERTERW(QString Query, int IOA);
    void SendValueToIEC_INVERTERVAR(QString Query, int IOA);
    void SendValueToIEC_INVERTERMODE(QString Query, int IOA);
    // control setpoint BATTERY
    void SendValueToIEC_BATTERYDODMIN(QString Query, int IOA);
    void SendValueToIEC_BATTERYDODMAX(QString Query, int IOA);
    void SendValueToIEC_BATTERYSOCMIN(QString Query, int IOA);
    void SendValueToIEC_BATTERYSOCMAX(QString Query, int IOA);
    void SendValueToIEC_BATTERYSPARE(QString Query, int IOA);
    unsigned LastCommandAddress;
    int SendCommands;             // 1 = allow sending commands, 0 = don't send commands
    int no[4],Hide,dB;
    bool trig,trig2;

    double dataValue[120]; //addr default iec
    QString dataku[120],timestamp;
    QTimer *waktu;
    //QTimer *jam;
    QTimer *clockSync;
    // BDTR Related
    QTimer *tmBDTR_kamsg; // timer to send keep alive messages to the dual host
    void BDTR_processPoints( iec_obj *obj, int numpoints ); // BDTR: process points
    inline bool BDTR_HaveDualHost() { return ( BDTR_host_dual != (QHostAddress)"0.0.0.0"); }
    bool isPrimary; // define se modo prim�rio ou secund�rio (o secund�rio permanece desconectado pelo IEC104)
    static const int BDTR_CntToBePrimary = 3; // counts necessary to be primary when not receiving keepalive messages
    int BDTR_CntDnToBePrimary; // countdown to be primary when not receiving keepalive messages
    int BDTR_Logar; // controla log das mensagens BDTR
    static const int BDTR_seconds_kamsg = 7;
    QUdpSocket *udps; // BDTR: udp socket
    QHostAddress BDTR_host; // endere�o IP do cliente BDTR (normalmente a pr�pria m�quina)
    QHostAddress BDTR_host_dual; // endere�o IP do cliente BDTR dual
    static const int BDTR_porta = 65280; // porta UDP para envio de dados aos clientes BDTR
    static const int BDTR_porta_escuta = 65281; // porta UDP para envio de dados aos clientes BDTR
    static const unsigned char BDTR_orig = 0; // endere�o de origem BDTR

};


#endif // MAINWINDOW_H
