/*
 * This software implements an IEC 60870-5-104 protocol tester.
 * Copyright ? 2010,2011,2012 Ricardo L. Olsen
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

#include <QClipboard>
#include <QDir>
#include <QCloseEvent>
#include <string>
#include <time.h>
#include <QtSql>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mainwindow.h"
#include <QDebug>

//-------------------------------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    // busca configuracoes no arquivo ini
    QSettings settings( "./qtester104.ini", QSettings::IniFormat );

    i104.setPrimaryAddress( settings.value( "IEC104/PRIMARY_ADDRESS", 2 ).toInt() );
    i104.BDTRForcePrimary = settings.value( "BDTR/FORCE_PRIMARY", 0 ).toInt();
    i104.setSecondaryAddress( settings.value( "RTU1/SECONDARY_ADDRESS", 2 ).toInt() );
    i104.SendCommands = settings.value( "RTU1/ALLOW_COMMANDS", 0 ).toInt();

    QString IPEscravo;
    IPEscravo = settings.value( "RTU1/IP_ADDRESS_BACKUP", "" ).toString();
    i104.setSecondaryIP_backup( (char *)IPEscravo.toStdString().c_str() );
    IPEscravo = settings.value( "RTU1/IP_ADDRESS", "" ).toString();
    i104.setSecondaryIP( (char *)IPEscravo.toStdString().c_str() );
    i104.setPortTCP( settings.value( "RTU1/TCP_PORT", i104.getPortTCP() ).toInt() );
    i104.setGIPeriod( settings.value( "RTU1/GI_PERIOD", 330 ).toInt() );

    // this is for using with the OSHMI HMI in a dual architecture
    QSettings settings_bdtr( "./hmi.ini", QSettings::IniFormat );
    BDTR_host_dual = settings_bdtr.value( "REDUNDANCY/OTHER_HMI_IP", "0.0.0.0" ).toString();
    BDTR_host = "127.0.0.1";
    BDTR_CntDnToBePrimary = BDTR_CntToBePrimary;

    ui->setupUi( this );

    // this is for hiding the window when runnig
    Hide = settings_bdtr.value( "RUN/HIDE", "" ).toInt();

    this->setWindowTitle( tr("60870-5-104 Load") );

    QIntValidator * validator = new QIntValidator( 0, 65535, this );
    ui->leAsduAddr->setValidator( validator );
    QIntValidator * validator2 = new QIntValidator( 0, 255, this );
    ui->leMasterAddr->setValidator( validator2 );

    QRegExp rx( "\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b" );
    QValidator *valip = new QRegExpValidator( rx, this );
    ui->leIPRemoto->setValidator( valip );

    udps = new QUdpSocket();
    udps->bind( BDTR_porta_escuta );
    udps->open( QIODevice::ReadWrite );

    QString qs;
    ui->leIPRemoto->setText( IPEscravo );
    QTextStream( &qs ) << i104.getSecondaryAddress();
    qs = "";
    QTextStream( &qs ) << i104.getPrimaryAddress();
    ui->leMasterAddr->setText( qs );

    ui->leIPRemoto->setText( IPEscravo );

    tmBDTR_kamsg = new QTimer();
    waktu = new QTimer();
    clockSync = new QTimer ();
    // jam = new QTimer ();

    connect( udps, SIGNAL(readyRead()), this, SLOT(slot_BDTR_pronto_para_ler()) );
    connect( tmBDTR_kamsg, SIGNAL(timeout()), this, SLOT(slot_timer_BDTR_kamsg()) );

    connect( &i104, SIGNAL(signal_dataIndication(iec_obj *, int)), this, SLOT(slot_dataIndication(iec_obj *, int)) );

    connect( &i104, SIGNAL(signal_interrogationActConfIndication()), this, SLOT(slot_interrogationActConfIndication()) );
    connect( &i104, SIGNAL(signal_interrogationActTermIndication()), this, SLOT(slot_interrogationActTermIndication()) );
    connect( &i104, SIGNAL(signal_tcp_connect()), this, SLOT(slot_tcpconnect()) );
    connect( &i104, SIGNAL(signal_tcp_disconnect()), this, SLOT(slot_tcpdisconnect()) );
    connect( &i104, SIGNAL(signal_commandActConfIndication(iec_obj *)), this, SLOT(slot_commandActConfIndication(iec_obj *)) );
    connect( waktu, SIGNAL(timeout()), this, SLOT(Trigger_SendValueToIEC()));
    waktu->start(1);
    connect( clockSync, SIGNAL(timeout()), this, SLOT(ClockSync_clicked()));
    //call db automatic
    //connect( pbConnect, SIGNAL(timeout()), this, SLOT(ControllerDB()));
    // jam->start();
    GetValue();


    if ( BDTR_HaveDualHost() )
    {
        tmBDTR_kamsg->start( BDTR_seconds_kamsg * 1000 );
        isPrimary = false;
        i104.disable_connect();
    }
    else
    {
        isPrimary = true;
        i104.enable_connect();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tmBDTR_kamsg;
    delete waktu;
    delete clockSync;
}


// Receipt of information by the BDTR
void MainWindow::slot_BDTR_pronto_para_ler()
{
    char buf[5000];
    char bufOut[1600];  // buffer para mensagem bdtr
    buf[0]=0;

    unsigned char br[2000]; // buffer de recepcao
    int bytesrec;

    while ( udps->hasPendingDatagrams() )
    {

        QHostAddress address;
        quint16 port;
        bytesrec = udps->readDatagram ( (char *)br, sizeof(br), &address, &port );

        sprintf( buf+strlen(buf), "%3d: ", bytesrec );
        for ( int i=0; i< bytesrec; i++ )
            sprintf (buf+strlen(buf), "%02x ", br[i]);

        int Tipo = br[0] & T_MASC;

        switch ( Tipo )
        {
        case T_REQ:
        {
            msg_req * msg;
            msg = (msg_req*)br;

            if ( msg->TIPO == REQ_GRUPO && msg->ID == 0 ) // GI
            {
                i104.solicitGI();
            }
            if ( msg->TIPO == REQ_GRUPO && msg->ID == 255 ) // request group 255: show form
            {
                Hide = ! Hide;
                if ( ! Hide )
                    this->setVisible( true );
            }
        }
            break;
        case T_HORA:
            // if BDTRForcePrimary==1 don't become secondary when received keep alive messages from other machine
            // if BDTRForcePrimary==0 become secondary when received keep alive messages from other machine
            if ( address == BDTR_host_dual && i104.BDTRForcePrimary == 0 )
            {
                isPrimary = false;
                i104.disable_connect();
                BDTR_CntDnToBePrimary = BDTR_CntToBePrimary; // restart count to be primary
            }
            break;
        case T_COM: // COMANDO
        {
            msg_com * msg;
            msg = (msg_com*)br;
            bool enviar = false;

            if ( msg->TVAL == T_DIG ) // DIGITAL
            {
                msg_ack *ms;
                ms = (msg_ack*)bufOut;
                // status bits 11 and 00 are used for command blocking
                // forward only commands for status = 10 (ON) or = 01 (OFF)
                if ( (msg->PONTO.STATUS & ESTADO) != 3 && (msg->PONTO.STATUS & ESTADO) != 0 )
                {
                    iec_obj obj;
                    obj.cause = iec104_class::ACTIVATION;
                    obj.address = msg->PONTO.ID;
                    obj.ca = msg->PONTO.VALOR.COM_SEMBANCO.UTR;
                    obj.qu = msg->PONTO.VALOR.COM_SEMBANCO.COMIEC.qu;
                    obj.se = msg->PONTO.VALOR.COM_SEMBANCO.COMIEC.se;

                    switch ( msg->PONTO.VALOR.COM_SEMBANCO.ASDU )
                    {
                    case 0: // if ASDU not defined, use single command
                        msg->PONTO.VALOR.COM_SEMBANCO.ASDU = iec104_class::C_SC_NA_1;
                    case iec104_class::C_SC_NA_1:
                    case iec104_class::C_SC_TA_1: // single
                        obj.type = msg->PONTO.VALOR.COM_SEMBANCO.ASDU;
                        obj.scs = !(msg->PONTO.VALOR.COM_SEMBANCO.COMIEC.dcs & 0x01);
                        enviar=true;
                        break;

                    case iec104_class::C_DC_NA_1:
                    case iec104_class::C_DC_TA_1: // double
                        obj.type = msg->PONTO.VALOR.COM_SEMBANCO.ASDU;
                        obj.dcs = msg->PONTO.VALOR.COM_SEMBANCO.COMIEC.dcs;
                        enviar=true;
                        break;

                    case iec104_class::C_RC_NA_1:
                    case iec104_class::C_RC_TA_1: // reg. step
                        obj.type = msg->PONTO.VALOR.COM_SEMBANCO.ASDU;
                        obj.rcs = msg->PONTO.VALOR.COM_SEMBANCO.COMIEC.dcs;
                        enviar=true;
                        break;

                    default:
                        enviar=false;
                        break;
                    }

                    if (enviar)
                    {
                        // forward command to IEC104
                        i104.sendCommand( &obj );
                        LastCommandAddress = obj.address;
                        // You will send ack by the BDTR when you receive the activation at level of 104
                    }
                    else
                    { // REJECT COMMAND (ASDU not supported)
                        ms->COD = T_ACK;
                        ms->TIPO = T_COM;
                        ms->ORIG = BDTR_orig;
                        ms->ID = 0x80 | msg->PONTO.VALOR.COM_SEMBANCO.COMIEC.dcs;
                        ms->COMP = msg->PONTO.ID;
                        udps->writeDatagram ( (const char *) bufOut, sizeof( msg_ack ), BDTR_host, BDTR_porta );
                        if ( BDTR_HaveDualHost() )
                            udps->writeDatagram ( (const char *) bufOut, sizeof( msg_ack ), BDTR_host_dual, BDTR_porta );
                    }
                }
            }
            else
                if ( msg->TVAL == T_FLT ) // Analogic scaling float
                {
                    msg_ack *ms;
                    ms = (msg_ack*)bufOut;
                    iec_obj obj;
                    obj.cause = iec104_class::ACTIVATION;
                    obj.address = msg->PONTO.ID;
                    obj.ca = msg->PONTO.VALOR.COM_SEMBANCOANA.UTR;
                    obj.qu = 0;
                    obj.se = msg->PONTO.VALOR.COM_SEMBANCOANA.COMIEC.se;

                    switch ( msg->PONTO.VALOR.COM_SEMBANCOANA.ASDU )
                    {
                    case 0: // if ASDU not defined, use float
                        msg->PONTO.VALOR.COM_SEMBANCO.ASDU = iec104_class::C_SE_NC_1;
                    case iec104_class::C_SE_NA_1: // Standardized
                    case iec104_class::C_SE_TA_1: // Standardized with time tag
                    case iec104_class::C_SE_NB_1: // Analogic scaling
                    case iec104_class::C_SE_TB_1: // Analogic scaling with time tag
                    case iec104_class::C_SE_NC_1: // Analogic scaling float
                    case iec104_class::C_SE_TC_1: // Analogic scaling float with time tag
                        obj.type = msg->PONTO.VALOR.COM_SEMBANCOANA.ASDU;
                        obj.value = msg->PONTO.VALOR.COM_SEMBANCOANA.FLT;
                        enviar=true;
                        break;

                    default:
                        enviar=false;
                        break;
                    }

                    if (enviar)
                    {
                        // forward command to IEC104
                        i104.sendCommand( &obj );
                        LastCommandAddress = obj.address;
                        // You will send ack by the BDTR when you receive the activation at level of 104
                    }
                    else
                    { // REJECT COMMAND (ASDU not supported)
                        ms->COD = T_ACK;
                        ms->TIPO = T_COM;
                        ms->ORIG = BDTR_orig;
                        ms->ID = 0x80 | msg->PONTO.VALOR.COM_SEMBANCOANA.COMIEC.dcs;
                        ms->COMP = msg->PONTO.ID;
                        udps->writeDatagram ( (const char *) bufOut, sizeof( msg_ack ), BDTR_host, BDTR_porta );
                        if ( BDTR_HaveDualHost() )
                            udps->writeDatagram ( (const char *) bufOut, sizeof( msg_ack ), BDTR_host_dual, BDTR_porta );
                    }
                }
        }
            break;
        default:
            break;
        }
    }
}

void MainWindow::BDTR_processPoints( iec_obj *obj, int numpoints )
{

    TFA_Qual qfa;
    int tam_msg;

    // Attention, I will leave the T_CONV bit of the message code to signal scanner without bank

    switch ( obj->type )
    {
    case iec104_class::M_DP_TB_1: // Double point information with time tag CP56Time2a
    case iec104_class::M_SP_TB_1: // Single point information with time tag CP56Time2a
    {
        msg_dig_tag *msgdigtag;

        tam_msg = sizeof( A_dig_tag ) * numpoints + sizeof( msg_dig_tag ) - sizeof( A_dig_tag );
        msgdigtag = (msg_dig_tag*)malloc( tam_msg );
        msgdigtag->COD = T_DIG_TAG;
        if ( obj->cause == iec104_class::CYCLIC )
            msgdigtag->COD |= T_CIC;
        if ( obj->cause == iec104_class::SPONTANEOUS )
            msgdigtag->COD |= T_SPONT;
        msgdigtag->NRPT = numpoints;
        msgdigtag->ORIG = BDTR_orig;

        for ( int cntpnt=0; cntpnt < numpoints; cntpnt++, obj++ )
        {
            // converte o qualificador do IEC para formato A do PABD
            qfa.Byte = 0;
            qfa.Subst = obj->bl || obj->sb;
            qfa.Tipo = TFA_TIPODIG;
            qfa.Falha = obj->iv || obj->nt;
            qfa.FalhaTag = obj->timetag.iv;

            if ( obj->type == iec104_class::M_DP_TB_1 || obj->type == iec104_class::M_DP_NA_1 )
            {
                qfa.Duplo = obj->dp;
            }
            else
            { // single to double
                qfa.Estado = !obj->sp;
                qfa.EstadoH = obj->sp;
            }

            msgdigtag->PONTO[cntpnt].ID = obj->address;
            msgdigtag->PONTO[cntpnt].UTR = obj->ca;
            msgdigtag->PONTO[cntpnt].STAT = qfa.Byte;
            msgdigtag->PONTO[cntpnt].TAG.ANO = 2000+obj->timetag.year;
            msgdigtag->PONTO[cntpnt].TAG.MES = obj->timetag.month;
            msgdigtag->PONTO[cntpnt].TAG.DIA = obj->timetag.mday;
            msgdigtag->PONTO[cntpnt].TAG.HORA = obj->timetag.hour;
            msgdigtag->PONTO[cntpnt].TAG.MINUTO = obj->timetag.min;
            msgdigtag->PONTO[cntpnt].TAG.MSEGS = obj->timetag.msec;
        }

        udps->writeDatagram ( (const char *) msgdigtag, tam_msg, BDTR_host, BDTR_porta );
        if ( BDTR_HaveDualHost() )
            udps->writeDatagram ( (const char *) msgdigtag, tam_msg, BDTR_host_dual, BDTR_porta );

        free( msgdigtag );
    }
        break;

    case iec104_class::M_DP_NA_1: // Double untagged
    case iec104_class::M_SP_NA_1: // Single untagged
    {
        msg_dig *msgdig;
        tam_msg = sizeof(A_dig) * numpoints + sizeof( msg_dig ) - sizeof( A_dig );
        msgdig = (msg_dig*)malloc( tam_msg );
        msgdig->COD = T_DIG;
        if ( obj->cause == iec104_class::CYCLIC )
            msgdig->COD |= T_CIC;
        if ( obj->cause == iec104_class::SPONTANEOUS )
            msgdig->COD |= T_SPONT;
        msgdig->NRPT = numpoints;
        msgdig->ORIG = BDTR_orig;

        for (int cntpnt=0; cntpnt < numpoints; cntpnt++, obj++)
        {
            // converte o qualificador do IEC para formato A do PABD
            qfa.Byte = 0;
            qfa.Subst = obj->bl || obj->sb;
            qfa.Tipo = TFA_TIPODIG;
            qfa.Falha = obj->iv || obj->nt;
            if ( obj->type == iec104_class::M_DP_TB_1 || obj->type == iec104_class::M_DP_NA_1 )
            {
                qfa.Duplo = obj->dp;
            }
            else
            { // simples para duplo
                qfa.Estado = !obj->sp;
                qfa.EstadoH = obj->sp;
            }

            msgdig->PONTO[cntpnt].ID = obj->address;
            msgdig->PONTO[cntpnt].STAT = qfa.Byte;
        }

        udps->writeDatagram ( (const char *) msgdig, tam_msg, BDTR_host, BDTR_porta );
        if ( BDTR_HaveDualHost() )
            udps->writeDatagram ( (const char *) msgdig, tam_msg, BDTR_host_dual, BDTR_porta );
        free(msgdig);
    }
        break;

    case iec104_class::M_ST_NA_1: // 5 = step
    case iec104_class::M_ME_NA_1: // 9 = normalized
    case iec104_class::M_ME_NB_1: // 11 = scaled
    case iec104_class::M_ST_TB_1: // 32 = step with time tag
    case iec104_class::M_ME_TD_1: // 34 = normalized with time tag
    case iec104_class::M_ME_TE_1: // 35 = scaled with time tag
    {
        msg_ana *msgana;

        tam_msg = sizeof( A_ana ) * numpoints + sizeof( msg_ana ) - sizeof( A_ana );
        msgana = (msg_ana*) malloc( tam_msg );
        if ( obj->type == iec104_class::M_ME_NA_1 || obj->type == iec104_class::M_ME_TD_1 )
            msgana->COD = T_NORM;
        else
            msgana->COD = T_ANA;
        if ( obj->cause == iec104_class::CYCLIC )
            msgana->COD |= T_CIC;
        if ( obj->cause == iec104_class::SPONTANEOUS )
            msgana->COD |= T_SPONT;
        msgana->NRPT = numpoints;
        msgana->ORIG = BDTR_orig;

        for ( int cntpnt=0; cntpnt < numpoints; cntpnt++, obj++ )
        {
            // converte o qualificador do IEC para formato A do PABD
            qfa.Byte = 0;
            qfa.Subst = obj->bl || obj->sb;
            qfa.Tipo = TFA_TIPOANA;
            qfa.Falha = obj->iv || obj->nt || obj->ov;
            if ( obj->type == iec104_class::M_ST_NA_1 || obj->type == iec104_class::M_ST_TB_1 ) // tap
                qfa.Falha =  qfa.Falha || obj->t; // transient = falha

            msgana->PONTO[cntpnt].ID = obj->address;
            msgana->PONTO[cntpnt].STAT = qfa.Byte;
            msgana->PONTO[cntpnt].VALOR = obj->value;
        }

        udps->writeDatagram ( (const char *) msgana, tam_msg, BDTR_host, BDTR_porta );
        if ( BDTR_HaveDualHost() )
            udps->writeDatagram ( (const char *) msgana, tam_msg, BDTR_host_dual, BDTR_porta );
        free(msgana);
    }
        break;

    case iec104_class::M_ME_NC_1: // 13
    case iec104_class::M_ME_TF_1: // 36
    {
        msg_float *msgflt;

        tam_msg = sizeof(A_float) * numpoints + sizeof(msg_float) - sizeof(A_float);
        msgflt = (msg_float*)malloc( tam_msg );
        msgflt->COD = T_FLT;
        if ( obj->cause == iec104_class::CYCLIC )
            msgflt->COD |= T_CIC;
        if ( obj->cause == iec104_class::SPONTANEOUS )
            msgflt->COD |= T_SPONT;

        msgflt->NRPT = numpoints;
        msgflt->ORIG = BDTR_orig;

        for ( int cntpnt=0; cntpnt < numpoints; cntpnt++, obj++ )
        {
            // qualifier converte o qualificador do IEC para formato A do PABD/BDTR
            qfa.Byte = 0;
            qfa.Subst = obj->bl || obj->sb;
            qfa.Tipo = TFA_TIPOANA;
            qfa.Falha = obj->iv || obj->nt || obj->ov;

            msgflt->PONTO[cntpnt].ID = obj->address;
            msgflt->PONTO[cntpnt].STAT = qfa.Byte;
            msgflt->PONTO[cntpnt].VALOR = obj->value;
        }

        udps->writeDatagram ( (const char *) msgflt, tam_msg, BDTR_host, BDTR_porta );
        if ( BDTR_HaveDualHost() )
            udps->writeDatagram ( (const char *) msgflt, tam_msg, BDTR_host_dual, BDTR_porta );
        free( msgflt );
    }
        break;
    default:
        break;
    }
}

//function untuk penerimaan data oleh protocol yang kemudian di kirim langsung ke database
void MainWindow::slot_dataIndication( iec_obj *obj, int numpoints )
{
    trig = false;
    char buftt[1000];
    int x=2;
    BDTR_processPoints( obj, numpoints );
    int i;
    for ( i=0; i< numpoints; i++, obj++)
    {
        //obj->value is value from iec 104 set to variable datavalue
        dataValue[(obj->address)-x] = obj->value;

        QDateTime current = QDateTime::currentDateTime();
        sprintf(buftt, "Local: %s", current.toString("yyyy/MM/dd hh:mm:ss.zzz").toStdString().c_str());

        switch (obj->type)
        {
        case iec104_class::M_SP_TB_1: // 30 Single point information with time tag CP56Time2a
            // format yyyy/MM/dd hh:mm:ss.zzz
            sprintf( buftt, "%04d-%02d-%02d %02d:%02d:%02d.%03d", obj->timetag.year+2000, obj->timetag.month, obj->timetag.mday, obj->timetag.hour, obj->timetag.min, obj->timetag.msec/1000,obj->timetag.msec%1000);
            timestamp = buftt;
            timestamp = timestamp.left(24);
            break;

        case iec104_class::M_DP_TB_1: // 31 Double point information with time tag CP56Time2a
            // format yyyy/MM/dd hh:mm:ss.zzz
            sprintf( buftt, "%04d-%02d-%02d %02d:%02d:%02d.%03d", obj->timetag.year+2000, obj->timetag.month, obj->timetag.mday, obj->timetag.hour, obj->timetag.min, obj->timetag.msec/1000,obj->timetag.msec%1000);
            timestamp = buftt;
            timestamp = timestamp.left(24);
            break;
        case iec104_class::M_ME_TD_1: // 34 Measured value, normalized value with time tag CP56Time2a
        case iec104_class::M_ME_TE_1: // 35 Measured value, scaled value with time tag CP56Time2a
        case iec104_class::M_ME_TF_1: // 36 Measured value, short floating point value with time tag CP56Time2a
            // format yyyy/MM/dd hh:mm:ss.zzz
            sprintf( buftt, "%04d-%02d-%02d %02d:%02d:%02d.%03d", obj->timetag.year+2000, obj->timetag.month, obj->timetag.mday, obj->timetag.hour, obj->timetag.min, obj->timetag.msec/1000,obj->timetag.msec%1000);
            timestamp = buftt;
            timestamp = timestamp.left(24);
            break;
        }
        dataku[(obj->address)-x] = QString::number(dataValue[(obj->address)-x]);
        qDebug()<<dataku[0];
    }

    //  Trigger_SendValueToIEC();
    DataIn_Status(); // IN STATUS
    DataIn_Status_RTU(); // IN RTU
    DataIn_Status_CB(); // IN CB
    //CB DAN STATUS
    INSERTALLDATA();
    DataIn_Status_Registry();
    trig = true;
}

void  MainWindow::slot_interrogationActConfIndication()
{
    msg_req m;
    m.COD = T_INICIO;
    m.TIPO = REQ_GRUPO;
    m.ORIG = BDTR_orig;
    m.ID = 0;
    m.NPTS = 0;
    m.PONTOS[0] = 0;
    udps->writeDatagram ( (const char *) &m, sizeof( msg_req ), BDTR_host, BDTR_porta );
    if ( BDTR_HaveDualHost() )
        udps->writeDatagram ( (const char *) &m, sizeof( msg_req ), BDTR_host_dual, BDTR_porta );
}

void  MainWindow::slot_interrogationActTermIndication()
{
    msg_req m;
    m.COD = T_FIM;
    m.TIPO = REQ_GRUPO;
    m.ORIG = BDTR_orig;
    m.ID = 0;
    m.NPTS = 0;
    m.PONTOS[0] = 0;
    udps->writeDatagram ( (const char *) &m, sizeof( msg_req ), BDTR_host, BDTR_porta );
    if ( BDTR_HaveDualHost() )
        udps->writeDatagram ( (const char *) &m, sizeof( msg_req ), BDTR_host_dual, BDTR_porta );
}

void MainWindow::slot_tcpconnect()
{
    ui->leAsduAddr->setEnabled(false);
    ui->leIPRemoto->setText( i104.tcps->peerAddress().toString() );
    ui->statusBar->setStyleSheet("color: rgb(0, 170, 0);");
    ui->statusBar->showMessage( "CONNECTED!" );
    //ui->pbConnect->setText( "Disconnected" );

    i104.clockSync();
    clockSync->start(1000*3600);
}

void MainWindow::slot_tcpdisconnect()
{
    if ( BDTR_HaveDualHost() && isPrimary == true )
    {
        BDTR_CntDnToBePrimary = BDTR_CntToBePrimary + 1; // wait a little more time to be primary again to allow for the secondary to assume
        isPrimary = false;
        i104.disable_connect();
        // ConnectionRTU();// save status rtu offline when disconnected
    }
    ConnectionRTU();// save status rtu offline when disconnected

    ui->statusBar->setStyleSheet("color: rgb(200, 0, 0);");
    ui->statusBar->showMessage( "DISCONNECTED!" );


    if ( i104.tmKeepAlive->isActive() )
    {
        //  ui->pbConnect->setText( "Give up" );
        ui->leIPRemoto->setEnabled( false );
    }
    else
    {
        //ui->pbConnect->setText( "Connect" );
        ui->leIPRemoto->setEnabled( true );
    }
}

void MainWindow::slot_commandActConfIndication( iec_obj *obj )
{
    bool is_select = false;

    if ( LastCommandAddress == obj->address )
    {
        is_select = ( obj->se == iec104_class::SELECT );

        // if confirmed select, execute
        if ( obj->se == iec104_class::SELECT && obj->pn == iec104_class::POSITIVE )
        {
            obj->se = iec104_class::EXECUTE;
            i104.sendCommand( obj );
        }

        // respond to BDTR only if it's not a select or if its a negative response
        if ( is_select == false || obj->pn == iec104_class::NEGATIVE )
        {
            char bufOut[1600];  // buffer for bdtr response
            msg_ack *ms;
            ms=(msg_ack*)bufOut;
            // ack msg for BDTR
            ms->ID = 0;
            ms->COD = T_ACK;
            ms->TIPO = T_COM;
            ms->ORIG = BDTR_orig;
            ms->COMP = obj->address;
            switch ( obj->type )
            {
            case iec104_class::C_SC_NA_1: //45 	Single command
            case iec104_class::C_SC_TA_1: //58  Single command with time tag CP56Time2a
                ms->ID = ( obj->scs == 1 ) ? 2 : 1;
                break;
            case iec104_class::C_DC_NA_1: //46 	Double command
            case iec104_class::C_DC_TA_1: //59 	Double command with time tag CP56Time2a
                ms->ID = obj->dcs;
                break;
            case iec104_class::C_RC_NA_1: //47 	Regulating step command
            case iec104_class::C_RC_TA_1: //60 	Regulating step command with time tag CP56Time2a
                ms->ID = obj->rcs;
                break;
            }
            if ( obj->pn == iec104_class::NEGATIVE )
            {
                ms->ID |= 0x80;
            }
            udps->writeDatagram ( (const char *) bufOut, sizeof(msg_ack), BDTR_host, BDTR_porta );
            if ( BDTR_HaveDualHost() )
                udps->writeDatagram ( (const char *) bufOut, sizeof(msg_ack), BDTR_host_dual, BDTR_porta );
        }
    }
}

void MainWindow::closeEvent( QCloseEvent *event )
{
    i104.terminate();
    event->accept();
}

void MainWindow::slot_timer_BDTR_kamsg()
{
    if ( ! isPrimary )
    {
        if ( BDTR_CntDnToBePrimary <= 0 )
        {
            isPrimary = true;
            i104.enable_connect();
            i104.tmKeepAlive->start();
            BDTR_CntDnToBePrimary = BDTR_CntToBePrimary;
        }
        else
            BDTR_CntDnToBePrimary--;
    }

    if ( isPrimary )
    {   // send keepalive message to the dual host

        msg_sinc m;
        m.COD = T_HORA;
        m.VAGO = 0;
        m.ORIG = BDTR_orig;

        time_t timer;
        struct tm *tblock;
        timer = time(NULL); // gets time of day
        tblock = localtime(&timer); // converts date/time to a structure

        m.TAG.ANO = tblock->tm_year+1900;
        m.TAG.MES = tblock->tm_mon+1;
        m.TAG.DIA = tblock->tm_mday;
        m.TAG.HORA = tblock->tm_hour;
        m.TAG.MINUTO = tblock->tm_min;
        m.TAG.MSEGS = 1000*tblock->tm_sec+0;

        udps->writeDatagram ( (const char *) &m, sizeof(m), BDTR_host_dual, BDTR_porta_escuta );
    }
}


void MainWindow::SendAllValueToIEC_Control()
{
    //Load
    //=====================================================================================================================
    SendValueToIEC_Load("SELECT id,outvalue FROM all_control_protocol where status='LOAD' order by id desc limit 1",1);
}

void MainWindow::SendValueToIEC_Load(QString Query,int IOA)
{
    //double command kirim ke iec
    iec_obj obj;
    QSqlQuery qry;
    static int val;
    static int no,temp;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        no = qry.value(0).toInt();
        val = qry.value(1).toInt();
        //========================================
        obj.ca = ui->leAsduAddr->text().toInt(); //ASDU addr
        i104.setSecondaryAddress(obj.ca);
        obj.address = IOA; //IOA addr
        //========================================
        if(ui->sb->value()==0)//protocol simulator =0 | plc =1
            obj.type = 46;//double command
        else if (ui->sb->value()==1)
            obj.type = 59;//double command with cp56timea2
        //filter value
        //====================================================================
        obj.value = val; //value from last insert db
        obj.dcs =val; //value from last insert db convert to double command
        obj.qu = 3; //persistence output send data anytime
        //====================================================================
        if (temp!=no)
        {
            i104.sendCommand( &obj );
            LastCommandAddress = obj.address;
        }
        temp=no;
    }
}

void MainWindow::Trigger_SendValueToIEC()
{
    if(trig == true){
        //send all data digitalout
        SendAllValueToIEC_Control();
    }
}

void MainWindow::GetValue()
{
    ControllerDB();

    QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                  QSettings::NativeFormat );
    set.beginGroup("DATABASE");
  //  ui->leIPRemoto->setText(ipaddr);
    ui->leDbHost->setText(set.value("hostserver").toString());
    ui->leDbName->setText(set.value("database").toString());
    ui->leDbPass_2->setText(set.value("user").toString());
    ui->leDbPass->setText(set.value("password").toString());
    set.endGroup();

    QSettings s("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                QSettings::NativeFormat );
    s.beginGroup("ProtocolConfiguration");
    ui->leIPRemoto->setText(s.value("IP LOAD").toString());
    ui->leAsduAddr->setText(s.value("ASDU LOAD").toString());
    s.endGroup();

    if ( i104.tmKeepAlive->isActive() )
    {
        i104.tmKeepAlive->stop();
        i104.tcps->close();
        i104.slot_tcpdisconnect();

        ui->leAsduAddr->setEnabled(true);
        ui->leMasterAddr->setEnabled(true);
        ui->leIPRemoto->setEnabled(true);
        ui->sb->setEnabled(true);
        ui->pbConnect_2->setEnabled(1);
        ui->leDbHost->setEnabled(true);
        ui->leDbName->setEnabled(true);
        ui->leDbPass->setEnabled(true);
        ui->leDbPass_2->setEnabled(true);
    }
    else
    {
        i104.solicitGI();
        i104.setSecondaryIP( (char*)ui->leIPRemoto->text().toStdString().c_str() );
        i104.setSecondaryAddress( ui->leAsduAddr->text().toInt() );
        i104.setPrimaryAddress( ui->leMasterAddr->text().toInt() );

        ui->leAsduAddr->setEnabled(false);
        ui->leMasterAddr->setEnabled(false);
        ui->leIPRemoto->setText( i104.getSecondaryIP() );
        ui->leIPRemoto->setEnabled( false );
        ui->sb->setEnabled(false);
        ui->pbConnect_2->setEnabled(0);
        ui->leDbHost->setEnabled(false);
        ui->leDbName->setEnabled(false);
        ui->leDbPass->setEnabled(false);
        ui->leDbPass_2->setEnabled(false);
        ui->statusBar->setStyleSheet("color: rgb(200, 170, 0);");
        ui->statusBar->showMessage( "TRYING TO CONNECT!" );

        i104.tmKeepAlive->start(1000);
    }
}



void MainWindow::INSERTALLDATA()
{
    DataIn_Status_IN_POWER();
    InsertAllRST();
    InsertAllRST_ActiveReactive();
    InsertAllRST_COSPHI();
}

void MainWindow::DataIn_Status()
{
    DataIn_Load("select count(id) from in_Load limit 1",
                "insert into in_Load (timestamps,Loadpage) values(:p_1,:p_2) ",
                "select Loadpage from in_Load order by id desc limit 1",timestamp,dataku[0]);
}

void MainWindow::DataIn_Status_RTU()
{
    Datain_RTU_Load("select count(id) from in_cb_rtu_Load limit 1",
                    "insert into in_cb_rtu_Load (timestamps,value) values(:p_1,:p_2) ",
                    "select value from in_cb_rtu_Load order by id desc limit 1",timestamp,dataku[1]);

}

void MainWindow::DataIn_Status_CB()
{
    //    //=========================IN STATUS CB Load======================================================

    Datain_CB_Load("select count(id) from in_cb_Load limit 1",
                   "insert into in_cb_Load (timestamps,aLoadA,aLoadB) values(:p_1,:p_2,:p_3) ",
                   "select aLoadA,aLoadB from in_cb_Load order by id desc limit 1",timestamp,dataku[2],dataku[3]);
}

void MainWindow::DataIn_Status_IN_POWER()
{
    //================================= Load MON W/VAR =================================================

    Datain_Load_W_VAR("select count(id) from in_power_Load limit 1",
                      "insert into in_power_Load (timestamps,LoadW,LoadVar) values(:p_1,:p_2,:p_3) ",
                      "select LoadW,LoadVar from in_power_Load order by id desc limit 1",timestamp,dataku[4],dataku[5]);
}

void MainWindow::DataIn_Status_Registry()
{
    QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                  QSettings::NativeFormat );
    set.beginGroup("Realtime Digital Input");
    set.setValue("DI_LOAD",dataku[0]);

    set.endGroup();
}



void MainWindow::InsertAllRST()
{
    QString formatparam,formatparam_plts;
    formatparam=":time,:vr,:vs,:vt,:ar,:as,:at";
    formatparam_plts=":time,:vr,:vs,:vt,:ar,:as,:at,:rad";
    //===================================================================================================================
    //MONITOR Load RST
    //===================================================================================================================
    //    int z;
    //    for (z=15; z<20;z++)
    //    {
    //        if (dataku[z]!=NULL)
    //        {
    InsertLoad_Monitor("select count(id) from mon_Load limit 1",
                       "INSERT INTO mon_Load (timestamps, LoadVR, LoadVS,LoadVT,LoadAR,LoadAS,LoadAT) VALUES ("+formatparam+")",
                       "select LoadVR, LoadVS,LoadVT,LoadAR,LoadAS,LoadAT from mon_Load order by id desc limit 1 ",
                       timestamp,
                       dataku[15],dataku[16],
            dataku[17],dataku[18],
            dataku[19],dataku[20]);
    //        }
    //    }
}
void MainWindow::InsertAllRST_ActiveReactive()
{
    QString Paramvalue=":time,:r,:s,:t",
            ParamDB="timestamps,ActiveR,ActiveS,ActiveT",
            ParamDBR="timestamps,ReactiveR,ReactiveS,ReactiveT";

    //    for (int a=6; a<8;a++)
    //    {
    //        if (dataku[a]!=NULL)
    //        {
    InsertLoad_Active("select count(id) from mon_active_Load limit 1",
                      "insert into mon_active_Load ("+ParamDB+") values ("+Paramvalue+")",
                      "select ActiveR,ActiveS,ActiveT from mon_active_Load order by id desc limit 1",
                      timestamp,
                      dataku[6],dataku[7],dataku[8]);
    //        }
    //    }


    //    for (int a=9; a<11;a++)
    //    {
    //        if (dataku[a]!=NULL)
    //        {
    InsertLoad_ReActive("select count(id) from mon_reactive_Load limit 1",
                        "insert into mon_reactive_Load ("+ParamDBR+") values ("+Paramvalue+")",
                        "select ReactiveR,ReactiveS,ReactiveT from mon_reactive_Load order by id desc limit 1",
                        timestamp,
                        dataku[9],dataku[10],dataku[11]);
    //        }
    //    }
}
void MainWindow::InsertAllRST_COSPHI()
{
    QString formatparam;
    formatparam=":time,:r,:s,:t";

    //    for (int d=12;d<14;d++)
    //    {
    //        if(dataku[d]!=NULL)
    //        {
    InsertLoad_COSPHI("select count(id) from mon_cosphi_Load limit 1",
                      "insert into mon_cosphi_Load(timestamps,CosPhiR,CosPhiS,CosPhiT) values("+formatparam+")",
                      "select CosPhiR,CosPhiS,CosPhiT from mon_cosphi_Load order by id desc limit 1 ",
                      timestamp,dataku[12],dataku[13],dataku[14]);
    //        }
    //    }
}
//=========================IN STATUS MONITOR====================================================


void MainWindow::DataIn_Load(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN)
{
    if(Value_IN!=NULL)
    {
        QSqlQuery qry,qry_d,qry_count;
        QString val,count;
        qry_count.prepare(QueryCount);
        qry_count.exec();
        if (qry_count.next())
        {
            count=qry_count.value(0).toString();
            if  (count=="0")
            {
                qry.prepare(QueryInsert);
                qry.bindValue(":p_1",Timestamp);
                qry.bindValue(":p_2",Value_IN);
                qry.exec();
            }
            else
            {
                qry_d.exec(Validation_IN);
                if (qry_d.next())
                {
                    val=qry_d.value(0).toString();
                    if(Value_IN!=val )
                    {
                        qry.prepare(QueryInsert);
                        qry.bindValue(":p_1",Timestamp);
                        qry.bindValue(":p_2",Value_IN);
                        qry.exec();
                    }

                }
            }
        }
    }
}

//=========================IN STATUS RTU========================================================

void MainWindow::Datain_RTU_Load(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN)
{
    // Load
    if(Value_IN!=NULL )
    {
        QSqlQuery qry,qry_d,qry_count;
        QString val,count;
        qry_count.prepare(QueryCount);
        qry_count.exec();
        if (qry_count.next())
        {
            count=qry_count.value(0).toString();
            if  (count=="0")
            {
                qry.prepare(QueryInsert);
                qry.bindValue(":p_1",Timestamp);
                qry.bindValue(":p_2",Value_IN);
                qry.exec();
            }
            else
            {
                qry_d.exec(Validation_IN);
                if (qry_d.next())
                {
                    val=qry_d.value(0).toString();
                    if(Value_IN!=val )
                    {
                        qry.prepare(QueryInsert);
                        qry.bindValue(":p_1",Timestamp);
                        qry.bindValue(":p_2",Value_IN);
                        qry.exec();

                    }

                }
            }
        }

    }
}

//=========================IN STATUS CB ========================================================

void MainWindow::Datain_CB_Load(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN_1,QString Value_IN_2)
{
    //Load
    if(Value_IN_1!=NULL && Value_IN_2!=NULL )
    {
        QSqlQuery qry,qry_d,qry_count;
        QString val,val2,count;
        qry_count.prepare(QueryCount);
        qry_count.exec();
        if (qry_count.next())
        {
            count=qry_count.value(0).toString();
            if  (count=="0")
            {
                qry.prepare(QueryInsert);
                qry.bindValue(":p_1",Timestamp);
                qry.bindValue(":p_2",Value_IN_1);
                qry.bindValue(":p_3",Value_IN_2);
                qry.exec();
            }
            else
            {
                qry_d.exec(Validation_IN);
                if (qry_d.next())
                {
                    val=qry_d.value(0).toString();
                    val2=qry_d.value(1).toString();
                    if(Value_IN_1!=val || Value_IN_2!=val2 )
                    {
                        qry.prepare(QueryInsert);
                        qry.bindValue(":p_1",Timestamp);
                        qry.bindValue(":p_2",Value_IN_1);
                        qry.bindValue(":p_3",Value_IN_2);
                        qry.exec();
                        //                        Set_CB_Load("insert into AllOutControl_protocol(timestamps,operatorname,status,outvalue,ioa,information) values(:p_1,:p_2,:p_3,:p_4,:p_5,:p_6)",
                        //                                    Timestamp,Value_IN_1,Value_IN_2);

                    }

                }
            }
        }
    }
}

//=========================IN W VAR MONITORING =================================================

void MainWindow::Datain_Load_W_VAR(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2)
{
    //================================= Load MON W/VAR ================================================
    if(Value_IN_1!=NULL && Value_IN_2!=NULL)
    {
        QSqlQuery qry,qry_d,qry_count;
        QString val,val2,count;
        qry_count.prepare(QueryCount);
        qry_count.exec();
        if (qry_count.next())
        {
            count=qry_count.value(0).toString();
            if  (count=="0")
            {
                qry.prepare(QueryInsert);
                qry.bindValue(":p_1",Timestamp);
                qry.bindValue(":p_2",Value_IN_1);
                qry.bindValue(":p_3",Value_IN_2);
                qry.exec();
            }
            else
            {
                qry_d.exec(Validation_IN);
                if (qry_d.next())
                {
                    val=qry_d.value(0).toString();
                    val2=qry_d.value(1).toString();
                    if(Value_IN_1!=val || Value_IN_2!=val2 )
                    {
                        qry.prepare(QueryInsert);
                        qry.bindValue(":p_1",Timestamp);
                        qry.bindValue(":p_2",Value_IN_1);
                        qry.bindValue(":p_3",Value_IN_2);
                        qry.exec();
                    }

                }
            }
        }
    }
}

//=========================ALL RST MONITOR =================================================

void MainWindow::InsertLoad_Monitor(QString QueryCount,QString QueryInsert, QString Validation_IN,QString Timestamp,QString VR,QString VS,QString VT,QString AR,QString AS,QString AT)
{

    QSqlQuery query_get,q_data,qry_count;
    QString vr,vs,vt,ar,as,at,count;

    qry_count.prepare(QueryCount);
    qry_count.exec();
    if (qry_count.next())
    {
        count=qry_count.value(0).toString();
        if  (count=="0")
        {
            q_data.prepare(QueryInsert);
            q_data.bindValue(":time",Timestamp);
            q_data.bindValue(":vr",VR);
            q_data.bindValue(":vs",VS);
            q_data.bindValue(":vt",VT);
            q_data.bindValue(":ar",AR);
            q_data.bindValue(":as",AS);
            q_data.bindValue(":at",AT);
            q_data.exec();
        }
        else
        {
            query_get.prepare(Validation_IN);
            query_get.exec();
            if (query_get.next())
            {
                vr=query_get.value(0).toString();
                vs=query_get.value(1).toString();
                vt=query_get.value(2).toString();
                ar=query_get.value(3).toString();
                as=query_get.value(4).toString();
                at=query_get.value(5).toString();

                if (vr!=VR || vs!=VS || vt!=VT || ar!=AR || as!=AS || at!=AT )
                {
                    q_data.prepare(QueryInsert);
                    q_data.bindValue(":time",Timestamp);
                    q_data.bindValue(":vr",VR);
                    q_data.bindValue(":vs",VS);
                    q_data.bindValue(":vt",VT);
                    q_data.bindValue(":ar",AR);
                    q_data.bindValue(":as",AS);
                    q_data.bindValue(":at",AT);
                    q_data.exec();
                }
            }
        }
    }
}

//=========================ALL RST ACTIVE REACTIVE =================================================

void MainWindow::InsertLoad_Active(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString VR, QString VS, QString VT)
{
    QSqlQuery query_get,q_data,qry_count;
    QString r,s,t,count;

    qry_count.prepare(QueryCount);
    qry_count.exec();
    if (qry_count.next())
    {
        count=qry_count.value(0).toString();
        if  (count=="0")
        {
            q_data.prepare(QueryInsert);
            q_data.bindValue(":time",Timestamp);
            q_data.bindValue(":r",VR);
            q_data.bindValue(":s",VS);
            q_data.bindValue(":t",VT);
            q_data.exec();
        }
        else
        {
            query_get.prepare(Validation_IN);
            query_get.exec();
            if (query_get.next())
            {
                r=query_get.value(0).toString();
                s=query_get.value(1).toString();
                t=query_get.value(2).toString();
                if (r!=VR || s!=VS || t!=VT )
                {
                    q_data.prepare(QueryInsert);
                    q_data.bindValue(":time",Timestamp);
                    q_data.bindValue(":r",VR);
                    q_data.bindValue(":s",VS);
                    q_data.bindValue(":t",VT);
                    q_data.exec();
                }
            }
        }
    }
}

void MainWindow::InsertLoad_ReActive(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString VR, QString VS, QString VT)
{
    QSqlQuery query_get,q_data,qry_count;
    QString r,s,t,count;

    qry_count.prepare(QueryCount);
    qry_count.exec();
    if (qry_count.next())
    {
        count=qry_count.value(0).toString();
        if  (count=="0")
        {
            q_data.prepare(QueryInsert);
            q_data.bindValue(":time",Timestamp);
            q_data.bindValue(":r",VR);
            q_data.bindValue(":s",VS);
            q_data.bindValue(":t",VT);
            q_data.exec();
        }
        else
        {
            query_get.prepare(Validation_IN);
            query_get.exec();
            if (query_get.next())
            {
                r=query_get.value(0).toString();
                s=query_get.value(1).toString();
                t=query_get.value(2).toString();
                if (r!=VR || s!=VS || t!=VT )
                {
                    q_data.prepare(QueryInsert);
                    q_data.bindValue(":time",Timestamp);
                    q_data.bindValue(":r",VR);
                    q_data.bindValue(":s",VS);
                    q_data.bindValue(":t",VT);
                    q_data.exec();
                }
            }
        }
    }
}

//=========================ALL RST COSPHI ==============================================

void MainWindow::InsertLoad_COSPHI(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString PHIR, QString PHIS, QString PHIT)
{
    QSqlQuery query,q_data,qry_count;
    QString r,s,t,count;
    qry_count.prepare(QueryCount);
    qry_count.exec();
    if (qry_count.next())
    {
        count=qry_count.value(0).toString();
        if  (count=="0")
        {
            q_data.prepare(QueryInsert);
            q_data.bindValue(":time",Timestamp);
            q_data.bindValue(":r",PHIR);
            q_data.bindValue(":s",PHIS);
            q_data.bindValue(":t",PHIT);
            q_data.exec();
        }
        else
        {
            query.prepare(Validation_IN);
            query.exec();
            if (query.next())
            {
                r=query.value(0).toString();
                s=query.value(1).toString();
                t=query.value(2).toString();

                if (r!=PHIR|| s!=PHIS || t!=PHIT )
                {
                    q_data.prepare(QueryInsert);
                    q_data.bindValue(":time",Timestamp);
                    q_data.bindValue(":r",PHIR);
                    q_data.bindValue(":s",PHIS);
                    q_data.bindValue(":t",PHIT);
                    q_data.exec();
                }

            }
        }
    }
}

void MainWindow::Set_RTU_Load(QString QueryInsert, QString Timestamp, QString Value_IN)
{
    QSqlQuery query;
    query.prepare(QueryInsert);
    query.bindValue(":p_1",Timestamp);
    query.bindValue(":p_2","Server Protocol");
    query.bindValue(":p_3","RTULOAD");
    query.bindValue(":p_4",Value_IN);
    query.bindValue(":p_5","3");
    query.bindValue(":p_6","RTU LOAD Condition is OFF !");
    query.exec();
}

void MainWindow::Set_CB_Load(QString QueryInsert, QString Timestamp, QString Value_IN ,  QString Value_IN2)
{
    QSqlQuery query;
    QString info;
    query.prepare(QueryInsert);
    query.bindValue(":p_1",Timestamp);
    query.bindValue(":p_2","Server Protocol");
    if (Value_IN=="0" || Value_IN2=="0")
    {
        info="CB LOAD 1 and CB LOAD 2 Condition is OFF ";
        query.bindValue(":p_3","CBLOAD");
        query.bindValue(":p_4","0,0");
        query.bindValue(":p_5","4,5");
        query.bindValue(":p_6",info);
        query.exec();
    }
    if(Value_IN=="1" || Value_IN2=="0")
    {
        info="CB LOAD 1 Condition is ON and CB LOAD 2 Condition is OFF ";
        query.bindValue(":p_3","CBLOAD");
        query.bindValue(":p_4","1,0");
        query.bindValue(":p_5","4,5");
        query.bindValue(":p_6",info);
        query.exec();
    }
    if(Value_IN=="0" || Value_IN2=="1")
    {
        info="CB LOAD 1 Condition is OFF and CB LOAD 2 Condition is ON ";
        query.bindValue(":p_3","CBLOAD");
        query.bindValue(":p_4","0,1");
        query.bindValue(":p_5","4,5");
        query.bindValue(":p_6",info);
        query.exec();
    }
}

void MainWindow::ClockSync_clicked()
{
    i104.clockSync();
}

void MainWindow::on_pbConnect_2_clicked()
{
    QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                  QSettings::NativeFormat );
    //set setting
    set.beginGroup("DATABASE");
    set.setValue("RTUserver",ui->leIPRemoto->text());
    set.setValue("hostserver",ui->leDbHost->text());
    set.setValue("database",ui->leDbName->text());
    set.setValue("user",ui->leDbPass_2->text());
    set.setValue("password",ui->leDbPass->text());
    set.setValue("MODE",ui->sb->text());
    set.setValue("ASDU ADDR",ui->leAsduAddr->text().toInt());
    //  set.setValue("IOA",ui->dataIOA->text());
    set.endGroup();
    set.sync();
    QMessageBox::information(this,"Succesfully","Configuration has been saved!");
}

void MainWindow::Load_Db(QString Host,QString Database,QString Username,QString Password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(Host);
    db.setDatabaseName(Database);
    db.setUserName(Username);
    db.setPassword(Password);

    if(!db.open())
    {
        ui->label_status->setStyleSheet("color: rgb(200, 0, 0);");
        ui->label_status->setText("DISCONNECTED!");
    }
    else
    {
        ui->label_status->setStyleSheet("color: rgb(0, 170, 0);");
        ui->label_status->setText("CONNECTED!");
        qDebug()<<"DB OK";
    }
}

void MainWindow::ControllerDB()
{
    QString  Server,Database,Username,Password;
    QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                  QSettings::NativeFormat );
    set.beginGroup("DATABASE");
    Server=set.value("hostserver").toString();
    Database=set.value("database").toString();
    Username=set.value("user").toString();
    Password=set.value("password").toString();
    Load_Db(Server,Database,Username,Password);
    set.endGroup();
}

void MainWindow::ConnectionRTU()
{
    QString time;
    time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QSqlQuery qry_2;
    QString value="0";
    qry_2.prepare("insert into in_cb_rtu_Load (timestamps,value) values (:p_1,:p_2)");
    qry_2.bindValue(":p_1",time);
    qry_2.bindValue(":p_2",value);
    qry_2.exec();

    //    Set_RTU_Load("insert into AllOutControl_protocol(timestamps,operatorname,status,outvalue,ioa,information) values(:p_1,:p_2,:p_3,:p_4,:p_5,:p_6)",
    //                 time,value);

}
