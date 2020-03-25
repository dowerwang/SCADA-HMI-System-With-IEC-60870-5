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
    void DataIn_Load(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN);
    void Datain_RTU_Load(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN);
    void Datain_CB_Load(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN_1,QString Value_IN_2);
    void Datain_Load_W_VAR(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2);
    //==============================================================================
    // SET DATA INPUT FROM PLC
    //==============================================================================
    void DataIn_Status();
    void DataIn_Status_RTU();
    void DataIn_Status_CB();
    void DataIn_Status_IN_POWER();
    void DataIn_Status_Registry();
    //==============================================================================
    // GET CLOCK AND DB
    //==============================================================================
    void ClockSync_clicked(); // event get clock from PLC/IEC 104
    void on_pbConnect_2_clicked(); // save default configuration
    void Load_Db(QString Host,QString Database,QString Username,QString Password);
    void ControllerDB();
    //==============================================================================
    //INSERT ALL DATA MONITORING RST
    //==============================================================================
    void InsertAllRST();
    void InsertLoad_Monitor(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString VR, QString VS, QString VT, QString AR, QString AS, QString AT);
    //==============================================================================
    //INSERT ALL DATA MONITORING RST ACTIVE REACTIVE
    //==============================================================================
    void InsertAllRST_ActiveReactive();
    void InsertLoad_Active(QString QueryCount,QString QueryInsert, QString Validation_IN,QString Timestamp,QString VR,QString VS,QString VT);
    void InsertLoad_ReActive(QString QueryCount,QString QueryInsert, QString Validation_IN,QString Timestamp,QString VR,QString VS,QString VT);
    //==============================================================================
    //INSERT ALL DATA MONITORING RST COS PHI
    //==============================================================================
    void InsertAllRST_COSPHI();
    void InsertLoad_COSPHI(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp,QString PHIR,QString PHIS,QString PHIT);
    //==============================================================================
    //LOG TO ALARMLIST
    //==============================================================================
    void Set_RTU_Load(QString QueryInsert,QString Timestamp,QString Value_IN);
    void Set_CB_Load(QString QueryInsert, QString Timestamp, QString Value_IN, QString Value_IN2);

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
    // control all Load
    void SendValueToIEC_Load(QString Query, int IOA);

    unsigned LastCommandAddress;
    int SendCommands;             // 1 = allow sending commands, 0 = don't send commands
    int no[4],Hide,dB;
    bool trig,trig2;

    double dataValue[24]; //addr default iec
    QString dataku[24],timestamp;
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
