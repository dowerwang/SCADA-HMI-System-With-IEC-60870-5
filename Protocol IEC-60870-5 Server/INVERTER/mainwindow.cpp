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

    this->setWindowTitle( tr("60870-5-104 BDI") );

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
    int x=18;
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
    }
    trig = true;
    //    for(x=0;x<10;x++)
    //    {
    //        qDebug()<<"ke -"<<x<<dataku[x];
    //    }
    DataIn_Status(); // IN STATUS
    DataIn_Status_RTU(); // IN RTU
    DataIn_Status_CB(); // IN CB
    //CB DAN STATUS
    INSERTALLDATA();
    DataIn_DigitalInput_LogRegistery();
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
    //INVERTER
    //=====================================================================================================================
    SendValueToIEC_INVERTER ("SELECT id,outvalue FROM all_control_protocol where status='INVERTER'  order by id desc limit 1",1);
    SendValueToIEC_BATTERY  ("SELECT id,outvalue FROM all_control_protocol where status='BATTERY' order by id desc limit 1",5);
}

void MainWindow::SendAllValueToIEC_SetPoint()
{
    SendValueToIEC_INVERTERW(  "SELECT id,outvalue FROM all_control_protocol where status='INVERTERW' order by id desc limit 1",7);
    SendValueToIEC_INVERTERVAR("SELECT id,outvalue FROM all_control_protocol where status='INVERTERVAR' order by id desc limit 1",8);

    SendValueToIEC_INVERTERMODE("SELECT id,outvalue FROM all_control_protocol where status='INVERTERMODE' order by id desc limit 1",6);

    SendValueToIEC_BATTERYDODMIN("SELECT id,outvalue FROM all_control_protocol where status='DODMIN' order by id desc limit 1",15);
    SendValueToIEC_BATTERYDODMAX("SELECT id,outvalue FROM all_control_protocol where status='DODMAX' order by id desc limit 1",16);
    SendValueToIEC_BATTERYSOCMIN("SELECT id,outvalue FROM all_control_protocol where status='SOCMIN' order by id desc limit 1",13);
    SendValueToIEC_BATTERYSOCMAX("SELECT id,outvalue FROM all_control_protocol where status='SOCMAX' order by id desc limit 1",14);

    SendValueToIEC_BATTERYSPARE ("SELECT id,outvalue FROM all_control_protocol where status='SPARE' order by id desc limit 1",17);
}

void MainWindow::SendValueToIEC_INVERTER(QString Query,int IOA)
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
        no=qry.value(0).toInt();
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

void MainWindow::SendValueToIEC_BATTERY(QString Query,int IOA)
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
        no=qry.value(0).toInt();
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
void MainWindow::SendValueToIEC_INVERTERW(QString Query, int IOA)
{
    //setpoint command kirim ke iec
    iec_obj obj;
    QSqlQuery qry;
    static int val;
    static int no,temp;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        no=qry.value(0).toInt();
        val = qry.value(1).toInt();
        //========================================
        obj.ca = ui->leAsduAddr->text().toInt(); //ASDU addr
        i104.setSecondaryAddress(obj.ca);
        obj.address = IOA; //IOA addr
        //========================================
        if(ui->sb->value()==0)//protocol simulator =0 | plc =1
            obj.type = 50;//float command C_SE_NC_1
        else if (ui->sb->value()==1)
            obj.type = 63;//float command with cp56timea2 C_SE_TC_1
        //filter value
        //====================================================================
        obj.value = val; //value from last insert db
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
void MainWindow::SendValueToIEC_INVERTERVAR(QString Query, int IOA)
{
    //setpoint command kirim ke iec
    iec_obj obj;
    QSqlQuery qry;
    static int val;
    static int no,temp;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        no=qry.value(0).toInt();
        val = qry.value(1).toInt();
        //========================================
        obj.ca = ui->leAsduAddr->text().toInt(); //ASDU addr
        i104.setSecondaryAddress(obj.ca);
        obj.address = IOA; //IOA addr
        //========================================
        if(ui->sb->value()==0)//protocol simulator =0 | plc =1
            obj.type = 50;//float command C_SE_NC_1
        else if (ui->sb->value()==1)
            obj.type = 63;//float command with cp56timea2 C_SE_TC_1
        //filter value
        //====================================================================
        obj.value = val; //value from last insert db
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
void MainWindow::SendValueToIEC_INVERTERMODE(QString Query, int IOA)
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
        no=qry.value(0).toInt();
        val = qry.value(1).toInt();
        //========================================
        obj.ca = ui->leAsduAddr->text().toInt(); //ASDU addr
        i104.setSecondaryAddress(obj.ca);
        obj.address = IOA; //IOA addr
        //========================================
        if(ui->sb->value()==0)//protocol simulator =0 | plc =1
            obj.type = 45;//single command
        else if (ui->sb->value()==1)
            obj.type = 58;//single command with cp56timea2
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

void MainWindow::SendValueToIEC_BATTERYDODMIN(QString Query, int IOA)
{
    //setpoint command kirim ke iec
    iec_obj obj;
    QSqlQuery qry;
    static int val;
    static int no,temp;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        no=qry.value(0).toInt();
        val = qry.value(1).toInt();
        //========================================
        obj.ca = ui->leAsduAddr->text().toInt(); //ASDU addr
        i104.setSecondaryAddress(obj.ca);
        obj.address = IOA; //IOA addr
        //========================================
        if(ui->sb->value()==0)//protocol simulator =0 | plc =1
            obj.type = 50;//float command C_SE_NC_1
        else if (ui->sb->value()==1)
            obj.type = 63;//float command with cp56timea2 C_SE_TC_1
        //filter value
        //====================================================================
        obj.value = val; //value from last insert db
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
void MainWindow::SendValueToIEC_BATTERYDODMAX(QString Query, int IOA)
{
    //setpoint command kirim ke iec
    iec_obj obj;
    QSqlQuery qry;
    static int val;
    static int no,temp;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        no=qry.value(0).toInt();
        val = qry.value(1).toInt();
        //========================================
        obj.ca = ui->leAsduAddr->text().toInt(); //ASDU addr
        i104.setSecondaryAddress(obj.ca);
        obj.address = IOA; //IOA addr
        //========================================
        if(ui->sb->value()==0)//protocol simulator =0 | plc =1
            obj.type = 50;//float command C_SE_NC_1
        else if (ui->sb->value()==1)
            obj.type = 63;//float command with cp56timea2 C_SE_TC_1
        //filter value
        //====================================================================
        obj.value = val; //value from last insert db
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
void MainWindow::SendValueToIEC_BATTERYSOCMIN(QString Query, int IOA)
{
    //setpoint command kirim ke iec
    iec_obj obj;
    QSqlQuery qry;
    static int val;
    static int no,temp;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        no=qry.value(0).toInt();
        val = qry.value(1).toInt();
        //========================================
        obj.ca = ui->leAsduAddr->text().toInt(); //ASDU addr
        i104.setSecondaryAddress(obj.ca);
        obj.address = IOA; //IOA addr
        //========================================
        if(ui->sb->value()==0)//protocol simulator =0 | plc =1
            obj.type = 50;//float command C_SE_NC_1
        else if (ui->sb->value()==1)
            obj.type = 63;//float command with cp56timea2 C_SE_TC_1
        //filter value
        //====================================================================
        obj.value = val; //value from last insert db
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
void MainWindow::SendValueToIEC_BATTERYSOCMAX(QString Query, int IOA)
{
    //setpoint command kirim ke iec
    iec_obj obj;
    QSqlQuery qry;
    static int val;
    static int no,temp;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        no=qry.value(0).toInt();
        val = qry.value(1).toInt();
        //========================================
        obj.ca = ui->leAsduAddr->text().toInt(); //ASDU addr
        i104.setSecondaryAddress(obj.ca);
        obj.address = IOA; //IOA addr
        //========================================
        if(ui->sb->value()==0)//protocol simulator =0 | plc =1
            obj.type = 50;//float command C_SE_NC_1
        else if (ui->sb->value()==1)
            obj.type = 63;//float command with cp56timea2 C_SE_TC_1
        //filter value
        //====================================================================
        obj.value = val; //value from last insert db
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
void MainWindow::SendValueToIEC_BATTERYSPARE(QString Query, int IOA)
{
    //setpoint command kirim ke iec
    iec_obj obj;
    QSqlQuery qry;
    static int val;
    static int no,temp;
    qry.prepare(Query);
    qry.exec();
    if(qry.next())
    {
        no=qry.value(0).toInt();
        val = qry.value(1).toInt();
        //========================================
        obj.ca = ui->leAsduAddr->text().toInt(); //ASDU addr
        i104.setSecondaryAddress(obj.ca);
        obj.address = IOA; //IOA addr
        //========================================
        if(ui->sb->value()==0)//protocol simulator =0 | plc =1
            obj.type = 50;//float command C_SE_NC_1
        else if (ui->sb->value()==1)
            obj.type = 63;//float command with cp56timea2 C_SE_TC_1
        //filter value
        //====================================================================
        obj.value = val; //value from last insert db
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
        SendAllValueToIEC_SetPoint();
    }
}

void MainWindow::GetValue()
{
    ControllerDB();

    QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                  QSettings::NativeFormat );
    set.beginGroup("DATABASE");
    //ui->leIPRemoto->setText(ipaddr);
    ui->leDbHost->setText(set.value("hostserver").toString());
    ui->leDbName->setText(set.value("database").toString());
    ui->leDbPass_2->setText(set.value("user").toString());
    ui->leDbPass->setText(set.value("password").toString());
    set.endGroup();

    QSettings s("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                QSettings::NativeFormat );
    s.beginGroup("ProtocolConfiguration");
    ui->leIPRemoto->setText(s.value("IP INVERTER").toString());
    ui->leAsduAddr->setText(s.value("ASDU INVERTER").toString());
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
    DataIn_Status_IN_POWER();//ok
    InsertAllRST(); // ok
    InsertAllRST_ActiveReactive();//ok
    InsertAllRST_COSPHI(); //ok
    DataIn_Status_BAT_FAULT();
    DataIn_Status_AC_SPV_BDI(); // ok
    DataIn_Status_AC_STATE_BDI(); //ok
    DataIn_Status_CURVE_P_AND_Q(); //ok
    DataIn_Status_SW_DC(); //ok
    DataIn_Status_SETPOINT(); // get status setpoint from hadware
    DataIn_Frequency(); // get status freq from hadware
    DataIn_Status_UpdateBank(); //ok get status bank from battery
    DataIn_Status_Battery(); //ok get status from battery
}

void MainWindow::DataIn_Status()
{
    DataIn_INVERTER("select count(id) from in_INVERTER limit 1",
                    "insert into in_INVERTER (timestamps,bdimon) values(:p_1,:p_2) ",
                    "select bdimon from in_INVERTER order by id desc limit 1",timestamp,dataku[0]);
    DataIn_BATTERY("select count(id) from in_battery limit 1",
                   "insert into in_battery (timestamps,battery) values(:p_1,:p_2)",
                   "select battery from in_battery order by id desc limit 1",timestamp,dataku[4]);
}
void MainWindow::DataIn_Status_RTU()
{
    Datain_RTU_INVERTER("select count(id) from in_cb_rtu_INVERTER limit 1",
                        "insert into in_cb_rtu_INVERTER (timestamps,value) values(:p_1,:p_2) ",
                        "select value from in_cb_rtu_INVERTER order by id desc limit 1",timestamp,dataku[5]);

}
void MainWindow::DataIn_Status_CB()
{
    //=========================IN STATUS CB INVERTER======================================================

    Datain_CB_INVERTER("select count(id) from in_cb_INVERTER limit 1",
                       "insert into in_cb_INVERTER (timestamps,aINVERTER) values(:p_1,:p_2) ",
                       "select aINVERTER from in_cb_INVERTER order by id desc limit 1",timestamp,dataku[6]);

    Datain_CB_BATTERY("select count(id) from in_cb_battery limit 1",
                      "insert into in_cb_battery (timestamps,aBattery) values(:p_1,:p_2) ",
                      "select aBattery from in_cb_battery order by id desc limit 1",timestamp,dataku[7]);

}
void MainWindow::DataIn_Status_SETPOINT()
{
//    Set_Point_Mode("select count(id) from in_inverter_setpoint_mode limit 1",
//                   "insert into in_inverter_setpoint_mode (timestamps,value) values(:p_1,:p_2) ",
//                   "select value from in_inverter_setpoint_mode order by id desc limit 1",timestamp,dataku[86]);

    Set_Point_W("select count(id) from in_inverter_setpoint_w limit 1",
                "insert into in_inverter_setpoint_w (timestamps,value) values(:p_1,:p_2) ",
                "select value from in_inverter_setpoint_w order by id desc limit 1",timestamp,dataku[87]);

    Set_Point_Var("select count(id) from in_inverter_setpoint_var limit 1",
                  "insert into in_inverter_setpoint_var (timestamps,value) values(:p_1,:p_2) ",
                  "select value from in_inverter_setpoint_var order by id desc limit 1",timestamp,dataku[88]);

}
void MainWindow::DataIn_Status_IN_POWER()
{
    //================================= INVERTER MON W/VAR =================================================

    Datain_INVERTER_W_VAR("select count(id) from in_power_INVERTER limit 1",
                          "insert into in_power_INVERTER (timestamps,InvW,InvVar) values(:p_1,:p_2,:p_3) ",
                          "select InvW,InvVar from in_power_INVERTER order by id desc limit 1",timestamp,dataku[16],dataku[17]);

    Datain_INVERTER_W_VAR_R_1("select count(id) from in_power_inverter_r limit 1",
                              "insert into in_power_inverter_r (timestamps,InvW,InvVar) values(:p_1,:p_2,:p_3) ",
                              "select InvW,InvVar from in_power_inverter_r order by id desc limit 1",timestamp,dataku[34],dataku[35]);
    Datain_INVERTER_W_VAR_S_1("select count(id) from in_power_inverter_s limit 1",
                              "insert into in_power_inverter_s (timestamps,InvW,InvVar) values(:p_1,:p_2,:p_3) ",
                              "select InvW,InvVar from in_power_inverter_s order by id desc limit 1",timestamp,dataku[36],dataku[37]);
    Datain_INVERTER_W_VAR_T_1("select count(id) from in_power_inverter_t limit 1",
                              "insert into in_power_inverter_t (timestamps,InvW,InvVar) values(:p_1,:p_2,:p_3) ",
                              "select InvW,InvVar from in_power_inverter_t order by id desc limit 1",timestamp,dataku[38],dataku[39]);

    Datain_INVERTER_W_VAR_R_2("select count(id) from in_power_inverter_r1 limit 1",
                              "insert into in_power_inverter_r1 (timestamps,InvW,InvVar) values(:p_1,:p_2,:p_3) ",
                              "select InvW,InvVar from in_power_inverter_r1 order by id desc limit 1",timestamp,dataku[97],dataku[98]);
    Datain_INVERTER_W_VAR_S_2("select count(id) from in_power_inverter_s1 limit 1",
                              "insert into in_power_inverter_s1 (timestamps,InvW,InvVar) values(:p_1,:p_2,:p_3) ",
                              "select InvW,InvVar from in_power_inverter_s1 order by id desc limit 1",timestamp,dataku[99],dataku[100]);
    Datain_INVERTER_W_VAR_T_2("select count(id) from in_power_inverter_t1 limit 1",
                              "insert into in_power_inverter_t1 (timestamps,InvW,InvVar) values(:p_1,:p_2,:p_3) ",
                              "select InvW,InvVar from in_power_inverter_t1 order by id desc limit 1",timestamp,dataku[101],dataku[102]);

    Datain_BATTERY_W("select count(id) from in_power_battery limit 1",
                     "insert into in_power_battery (timestamps,batteryw) values(:p_1,:p_2) ",
                     "select batteryw from in_power_battery order by id desc limit 1",timestamp,dataku[18]);

}
void MainWindow::DataIn_Status_BAT_FAULT()
{
    DataIn_BATTERY_SPV   ("select count(id) from in_cb_battery_spv limit 1",
                          "insert into in_cb_battery_spv (timestamps,value) values(:p_1,:p_2)",
                          "select value from in_cb_battery_spv order by id desc limit 1",timestamp,dataku[8]);
    DataIn_BATTERY_COM("select count(id) from in_cb_battery_com limit 1",
                       "insert into in_cb_battery_com (timestamps,value) values(:p_1,:p_2)",
                       "select value from in_cb_battery_com order by id desc limit 1",timestamp,dataku[85]);

    DataIn_INVERTER_FAULTR_1("select count(id) from in_cb_inverter_fault1 limit 1",
                             "insert into in_cb_inverter_fault1 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_fault1 order by id desc limit 1",timestamp,dataku[9]);
    DataIn_INVERTER_FAULTS_1("select count(id) from in_cb_inverter_fault2 limit 1",
                             "insert into in_cb_inverter_fault2 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_fault2 order by id desc limit 1",timestamp,dataku[10]);
    DataIn_INVERTER_FAULTT_1("select count(id) from in_cb_inverter_fault3 limit 1",
                             "insert into in_cb_inverter_fault3 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_fault3 order by id desc limit 1",timestamp,dataku[11]);

    DataIn_INVERTER_FAULTR_2("select count(id) from in_cb_inverter_fault4 limit 1",
                             "insert into in_cb_inverter_fault4 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_fault4 order by id desc limit 1",timestamp,dataku[94]);
    DataIn_INVERTER_FAULTS_2("select count(id) from in_cb_inverter_fault5 limit 1",
                             "insert into in_cb_inverter_fault5 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_fault5 order by id desc limit 1",timestamp,dataku[95]);
    DataIn_INVERTER_FAULTT_2("select count(id) from in_cb_inverter_fault6 limit 1",
                             "insert into in_cb_inverter_fault6 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_fault6 order by id desc limit 1",timestamp,dataku[96]);

}
void MainWindow::DataIn_Status_SW_DC()
{
    DataIn_INVERTER_SWITCH("select count(id) from in_inverter_switch limit 1",
                           "insert into in_inverter_switch (timestamps,bdimonsw) values(:p_1,:p_2)",
                           "select bdimonsw from in_inverter_switch order by id desc limit 1",timestamp,dataku[12]);

    DataIn_INVERTER_SWITCH1("select count(id) from in_inverter_switch_1 limit 1",
                            "insert into in_inverter_switch_1 (timestamps,bdi1sw) values(:p_1,:p_2)",
                            "select bdi1sw from in_inverter_switch_1 order by id desc limit 1",timestamp,dataku[13]);

    DataIn_INVERTER_SWITCH2("select count(id) from in_inverter_switch_2 limit 1",
                            "insert into in_inverter_switch_2 (timestamps,bdi2sw) values(:p_1,:p_2)",
                            "select bdi2sw from in_inverter_switch_2 order by id desc limit 1",timestamp,dataku[14]);

    DataIn_INVERTER_SWITCH3("select count(id) from in_inverter_switch_3 limit 1",
                            "insert into in_inverter_switch_3 (timestamps,bdi3sw) values(:p_1,:p_2)",
                            "select bdi3sw from in_inverter_switch_3 order by id desc limit 1",timestamp,dataku[15]);

    DataIn_INVERTER_SWITCH4("select count(id) from in_inverter_switch_4 limit 1",
                            "insert into in_inverter_switch_4 (timestamps,value) values(:p_1,:p_2)",
                            "select value from in_inverter_switch_4 order by id desc limit 1",timestamp,dataku[103]);

    DataIn_INVERTER_SWITCH5("select count(id) from in_inverter_switch_5 limit 1",
                            "insert into in_inverter_switch_5 (timestamps,value) values(:p_1,:p_2)",
                            "select value from in_inverter_switch_5 order by id desc limit 1",timestamp,dataku[104]);

    DataIn_INVERTER_SWITCH6("select count(id) from in_inverter_switch_6 limit 1",
                            "insert into in_inverter_switch_6 (timestamps,value) values(:p_1,:p_2)",
                            "select value from in_inverter_switch_6 order by id desc limit 1",timestamp,dataku[105]);

    DataIn_INVERTER_DC1("select count(id) from in_inverter_switch_7 limit 1",
                        "insert into in_inverter_switch_7 (timestamps,value) values(:p_1,:p_2)",
                        "select value from in_inverter_switch_7 order by id desc limit 1",timestamp,dataku[106]);

    DataIn_INVERTER_DC2("select count(id) from in_inverter_switch_8 limit 1",
                        "insert into in_inverter_switch_8 (timestamps,value) values(:p_1,:p_2)",
                        "select value from in_inverter_switch_8 order by id desc limit 1",timestamp,dataku[107]);

    DataIn_INVERTER_DC3("select count(id) from in_inverter_switch_9 limit 1",
                        "insert into in_inverter_switch_9 (timestamps,value) values(:p_1,:p_2)",
                        "select value from in_inverter_switch_9 order by id desc limit 1",timestamp,dataku[108]);

    DataIn_INVERTER_DC4("select count(id) from in_inverter_switch_10 limit 1",
                        "insert into in_inverter_switch_10 (timestamps,value) values(:p_1,:p_2)",
                        "select value from in_inverter_switch_10 order by id desc limit 1",timestamp,dataku[109]);

    DataIn_INVERTER_DC5("select count(id) from in_inverter_switch_11 limit 1",
                        "insert into in_inverter_switch_11 (timestamps,value) values(:p_1,:p_2)",
                        "select value from in_inverter_switch_11 order by id desc limit 1",timestamp,dataku[110]);

    DataIn_INVERTER_DC6("select count(id) from in_inverter_switch_12 limit 1",
                        "insert into in_inverter_switch_12 (timestamps,value) values(:p_1,:p_2)",
                        "select value from in_inverter_switch_12 order by id desc limit 1",timestamp,dataku[111]);

}
void MainWindow::DataIn_Status_AC_SPV_BDI()
{
    DataIn_INVERTER_SPV_R1("select count(id) from in_cb_inverter_spv1 limit 1",
                           "insert into in_cb_inverter_spv1 (timestamps,value) values(:p_1,:p_2)",
                           "select value from in_cb_inverter_spv1 order by id desc limit 1",timestamp,dataku[79]);

    DataIn_INVERTER_SPV_R2("select count(id) from in_cb_inverter_spv2 limit 1",
                           "insert into in_cb_inverter_spv2 (timestamps,value) values(:p_1,:p_2)",
                           "select value from in_cb_inverter_spv2 order by id desc limit 1",timestamp,dataku[80]);

    DataIn_INVERTER_SPV_S1("select count(id) from in_cb_inverter_spv3 limit 1",
                           "insert into in_cb_inverter_spv3 (timestamps,value) values(:p_1,:p_2)",
                           "select value from in_cb_inverter_spv3 order by id desc limit 1",timestamp,dataku[81]);

    DataIn_INVERTER_SPV_S2("select count(id) from in_cb_inverter_spv4 limit 1",
                           "insert into in_cb_inverter_spv4 (timestamps,value) values(:p_1,:p_2)",
                           "select value from in_cb_inverter_spv4 order by id desc limit 1",timestamp,dataku[82]);

    DataIn_INVERTER_SPV_T1("select count(id) from in_cb_inverter_spv5 limit 1",
                           "insert into in_cb_inverter_spv5 (timestamps,value) values(:p_1,:p_2)",
                           "select value from in_cb_inverter_spv5 order by id desc limit 1",timestamp,dataku[83]);

    DataIn_INVERTER_SPV_T2("select count(id) from in_cb_inverter_spv6 limit 1",
                           "insert into in_cb_inverter_spv6 (timestamps,value) values(:p_1,:p_2)",
                           "select value from in_cb_inverter_spv6 order by id desc limit 1",timestamp,dataku[84]);

    //    DataIn_INVERTER_AC_R("select count(id) from in_cb_inverter_ac1 limit 1",
    //                         "insert into in_cb_inverter_ac1 (timestamps,value) values(:p_1,:p_2)",
    //                         "select value from in_cb_inverter_ac1 order by id desc limit 1",timestamp,dataku[82]);

    //    DataIn_INVERTER_AC_S("select count(id) from in_cb_inverter_ac2 limit 1",
    //                         "insert into in_cb_inverter_ac2 (timestamps,value) values(:p_1,:p_2)",
    //                         "select value from in_cb_inverter_ac2 order by id desc limit 1",timestamp,dataku[83]);

    //    DataIn_INVERTER_AC_T("select count(id) from in_cb_inverter_ac3 limit 1",
    //                         "insert into in_cb_inverter_ac3 (timestamps,value) values(:p_1,:p_2)",
    //                         "select value from in_cb_inverter_ac3 order by id desc limit 1",timestamp,dataku[84]);
}

void MainWindow::DataIn_Status_AC_STATE_BDI()
{
    DataIn_INVERTER_STATE_R1("select count(id) from in_cb_inverter_state_r1 limit 1",
                             "insert into in_cb_inverter_state_r1 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_state_r1 order by id desc limit 1",timestamp,dataku[114]);

    DataIn_INVERTER_STATE_R2("select count(id) from in_cb_inverter_state_r2 limit 1",
                             "insert into in_cb_inverter_state_r2 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_state_r2 order by id desc limit 1",timestamp,dataku[115]);

    DataIn_INVERTER_STATE_S1("select count(id) from in_cb_inverter_state_s1 limit 1",
                             "insert into in_cb_inverter_state_s1 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_state_s1 order by id desc limit 1",timestamp,dataku[116]);

    DataIn_INVERTER_STATE_S2("select count(id) from in_cb_inverter_state_s2 limit 1",
                             "insert into in_cb_inverter_state_s2 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_state_s2 order by id desc limit 1",timestamp,dataku[117]);

    DataIn_INVERTER_STATE_T1("select count(id) from in_cb_inverter_state_t1 limit 1",
                             "insert into in_cb_inverter_state_t1 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_state_t1 order by id desc limit 1",timestamp,dataku[118]);

    DataIn_INVERTER_STATE_T2("select count(id) from in_cb_inverter_state_t2 limit 1",
                             "insert into in_cb_inverter_state_t2 (timestamps,value) values(:p_1,:p_2)",
                             "select value from in_cb_inverter_state_t2 order by id desc limit 1",timestamp,dataku[119]);

}

void MainWindow::DataIn_Status_CURVE_P_AND_Q()
{
    DataIn_INVERTER_CURVE_P("select count(id) from in_inverter_curve_p limit 1",
                            "insert into in_inverter_curve_p (timestamps,value) values(:p_1,:p_2)",
                            "select value from in_inverter_curve_p order by id desc limit 1",timestamp,dataku[112]);

    DataIn_INVERTER_CURVE_Q("select count(id) from in_inverter_curve_q limit 1",
                            "insert into in_inverter_curve_q (timestamps,value) values(:p_1,:p_2)",
                            "select value from in_inverter_curve_q order by id desc limit 1",timestamp,dataku[113]);
}
void MainWindow::DataIn_Status_UpdateBank()
{
    QString table="in_battery_bank";
    DataIn_Update_Bank_1("update "+table+" set timestamps=:p_2,volt=:p_3,temp=:p_4,faultvolt=:p_5,faulttemp=:p_6 where id=:p_1",
                         timestamp,dataku[40],dataku[47],dataku[54],dataku[61]);
    DataIn_Update_Bank_2("update "+table+" set timestamps=:p_2,volt=:p_3,temp=:p_4,faultvolt=:p_5,faulttemp=:p_6 where id=:p_1",
                         timestamp,dataku[41],dataku[48],dataku[55],dataku[62]);
    DataIn_Update_Bank_3("update "+table+" set timestamps=:p_2,volt=:p_3,temp=:p_4,faultvolt=:p_5,faulttemp=:p_6 where id=:p_1",
                         timestamp,dataku[42],dataku[49],dataku[56],dataku[63]);
    DataIn_Update_Bank_4("update "+table+" set timestamps=:p_2,volt=:p_3,temp=:p_4,faultvolt=:p_5,faulttemp=:p_6 where id=:p_1",
                         timestamp,dataku[43],dataku[50],dataku[57],dataku[64]);
    DataIn_Update_Bank_5("update "+table+" set timestamps=:p_2,volt=:p_3,temp=:p_4,faultvolt=:p_5,faulttemp=:p_6 where id=:p_1",
                         timestamp,dataku[44],dataku[51],dataku[58],dataku[65]);
    DataIn_Update_Bank_6("update "+table+" set timestamps=:p_2,volt=:p_3,temp=:p_4,faultvolt=:p_5,faulttemp=:p_6 where id=:p_1",
                         timestamp,dataku[45],dataku[52],dataku[59],dataku[66]);
    DataIn_Update_Bank_7("update "+table+" set timestamps=:p_2,volt=:p_3,temp=:p_4,faultvolt=:p_5,faulttemp=:p_6 where id=:p_1",
                         timestamp,dataku[46],dataku[53],dataku[60],dataku[67]);
}
void MainWindow::DataIn_Status_Battery()
{
    QString val="update in_battery_Status set timestamps=:p_2,FaultVSystem=:p_3,FaultCurSystem=:p_4,SOC=:p_5,DOD=:p_6,V_TOT=:p_7,VHVDC=:p_8,ITOT=:p_9 where id=:p_1";
    DataIn_Battery_Status(val,timestamp,dataku[68],dataku[69],dataku[70],dataku[71],dataku[72],dataku[73],dataku[74]);

    DataIn_Battery_SOC("select count(id) from in_battery_soc limit 1",
                       "insert into in_battery_soc (timestamps,minsoc,maxsoc) values(:p_1,:p_2,:p_3) ",
                       "select minsoc,maxsoc from in_battery_soc order by id desc limit 1",timestamp,dataku[75],dataku[76]);

    DataIn_Battery_DOD("select count(id) from in_battery_dod limit 1",
                       "insert into in_battery_dod (timestamps,mindod,maxdod) values(:p_1,:p_2,:p_3) ",
                       "select mindod,maxdod from in_battery_dod order by id desc limit 1",timestamp,dataku[77],dataku[78]);
}
void MainWindow::DataIn_Frequency()
{
    DataIn_Freq("select count(id) from mon_inverter_freq limit 1",
                "insert into mon_inverter_freq (timestamps,value) values(:p_1,:p_2)",
                "select value from mon_inverter_freq order by id desc limit 1",timestamp,dataku[86]);

}
void MainWindow::DataIn_DigitalInput_LogRegistery()
{
    QSettings set("HKEY_CURRENT_USER\\IEC-Protocol-60870-5-104",
                  QSettings::NativeFormat );
    set.beginGroup("Realtime Digital Input");

    set.setValue("DI_INVERTER",dataku[0]);

    set.setValue("DI_DC_INVERTER0",dataku[12]);

    set.setValue("DI_DC_INVERTER1",dataku[13]);
    set.setValue("DI_DC_INVERTER2",dataku[14]);
    set.setValue("DI_DC_INVERTER3",dataku[15]);
    set.setValue("DI_DC_INVERTER4",dataku[103]);
    set.setValue("DI_DC_INVERTER5",dataku[104]);
    set.setValue("DI_DC_INVERTER6",dataku[105]);

    set.setValue("DI_SW_INVERTER0",dataku[106]);
    set.setValue("DI_SW_INVERTER1",dataku[107]);
    set.setValue("DI_SW_INVERTER2",dataku[108]);
    set.setValue("DI_SW_INVERTER3",dataku[109]);
    set.setValue("DI_SW_INVERTER4",dataku[110]);
    set.setValue("DI_SW_INVERTER5",dataku[111]);

    set.setValue("DI_BATTERY",dataku[4]);
    set.endGroup();
}
void MainWindow::InsertAllRST()
{
    QString formatparam;
    formatparam=":time,:vr,:vs,:vt,:ar,:as,:at";
    //===================================================================================================================
    //MONITOR INVERTER RST
    //===================================================================================================================
    InsertINVERTER_Monitor("select count(id) from mon_INVERTER limit 1",
                           "INSERT INTO mon_INVERTER (timestamps, invVR, invVS,invVT,invAR,invAS,invAT) VALUES ("+formatparam+")",
                           "select invVR, invVS,invVT,invAR,invAS,invAT from mon_INVERTER order by id desc limit 1 ",
                           timestamp,
                           dataku[28],dataku[29],
            dataku[30],dataku[31],
            dataku[32],dataku[33]);
}
void MainWindow::InsertAllRST_ActiveReactive()
{
    QString Paramvalue=":time,:r,:s,:t",
            ParamDB="timestamps,ActiveR,ActiveS,ActiveT",
            ParamDBR="timestamps,ReactiveR,ReactiveS,ReactiveT";

    InsertINVERTER_Active("select count(id) from mon_active_INVERTER limit 1",
                          "insert into mon_active_INVERTER ("+ParamDB+") values ("+Paramvalue+")",
                          "select ActiveR,ActiveS,ActiveT from mon_active_INVERTER order by id desc limit 1",
                          timestamp,
                          dataku[19],dataku[20],dataku[21]);
    InsertINVERTER_ReActive("select count(id) from mon_reactive_INVERTER limit 1",
                            "insert into mon_reactive_INVERTER ("+ParamDBR+") values ("+Paramvalue+")",
                            "select ReactiveR,ReactiveS,ReactiveT from mon_reactive_INVERTER order by id desc limit 1",
                            timestamp,
                            dataku[22],dataku[23],dataku[24]);

}
void MainWindow::InsertAllRST_COSPHI()
{
    QString formatparam;
    formatparam=":time,:r,:s,:t";
    InsertINVERTER_COSPHI("select count(id) from mon_cosphi_inverter limit 1",
                          "insert into mon_cosphi_inverter(timestamps,CosPhiR,CosPhiS,CosPhiT) values("+formatparam+")",
                          "select CosPhiR,CosPhiS,CosPhiT from mon_cosphi_inverter order by id desc limit 1 ",
                          timestamp,dataku[25],dataku[26],dataku[27]);
}
//=========================IN STATUS MONITOR====================================================
void MainWindow::DataIn_INVERTER(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN)
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
void MainWindow::DataIn_BATTERY(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN)
{
    //BATTERY
    if(Value_IN!=NULL  )
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
void MainWindow::Datain_RTU_INVERTER(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN)
{
    // INVERTER
    if(Value_IN!=NULL  )
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
    }}
//=========================IN STATUS CB ========================================================
void MainWindow::Datain_CB_INVERTER(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN_1)
{
    //INVERTER
    if(Value_IN_1!=NULL )
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
                qry.exec();
            }
            else
            {
                qry_d.exec(Validation_IN);
                if (qry_d.next())
                {
                    val=qry_d.value(0).toString();
                    val2=qry_d.value(1).toString();
                    if(Value_IN_1!=val )
                    {
                        qry.prepare(QueryInsert);
                        qry.bindValue(":p_1",Timestamp);
                        qry.bindValue(":p_2",Value_IN_1);
                        qry.exec();
                    }

                }
            }
        }
    }
}
void MainWindow::Datain_CB_BATTERY(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
{
    //BATTERY
    if(Value_IN!=NULL  )
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
//=========================IN W VAR MONITORING =================================================
void MainWindow::Datain_INVERTER_W_VAR(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2)
{
    //================================= INVERTER MON W/VAR ================================================
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
void MainWindow::Datain_INVERTER_W_VAR_R_1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2)
{
    //================================= INVERTER MON W/VAR ================================================
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
void MainWindow::Datain_INVERTER_W_VAR_S_1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2)
{
    //================================= INVERTER MON W/VAR ================================================
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
void MainWindow::Datain_INVERTER_W_VAR_T_1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2)
{
    //================================= INVERTER MON W/VAR ================================================
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
void MainWindow::Datain_INVERTER_W_VAR_R_2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2)
{
    //================================= INVERTER MON W/VAR ================================================
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
void MainWindow::Datain_INVERTER_W_VAR_S_2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2)
{
    //================================= INVERTER MON W/VAR ================================================
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
void MainWindow::Datain_INVERTER_W_VAR_T_2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2)
{
    //================================= INVERTER MON W/VAR ================================================
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

void MainWindow::Datain_BATTERY_W(QString QueryCount,QString QueryInsert,QString Validation_IN,QString Timestamp,QString Value_IN)
{
    // INVERTER
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

void MainWindow::DataIn_BATTERY_SPV(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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

void MainWindow::DataIn_BATTERY_COM(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_FAULTR_1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_FAULTS_1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_FAULTT_1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_FAULTR_2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_FAULTS_2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_FAULTT_2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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

void MainWindow::DataIn_INVERTER_SWITCH(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_SWITCH1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_SWITCH2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_SWITCH3(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_SWITCH4(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_SWITCH5(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_SWITCH6(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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

void MainWindow::DataIn_INVERTER_DC1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_DC2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_DC3(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_DC4(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_DC5(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_DC6(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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


void MainWindow::DataIn_INVERTER_SPV_R1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_SPV_R2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_SPV_S1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_SPV_S2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_SPV_T1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_SPV_T2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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

void MainWindow::DataIn_INVERTER_STATE_R1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_STATE_R2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_STATE_S1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_STATE_S2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_STATE_T1(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_STATE_T2(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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

void MainWindow::DataIn_INVERTER_CURVE_P(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_INVERTER_CURVE_Q(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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

//=========================ALL BATTERY BANK =================================================
void MainWindow::DataIn_Update_Bank_1(QString QueryUpdate, QString Timestamp, QString Volt, QString Temp, QString FaultV, QString FaultTmp)
{
    QSqlQuery qry;
    qry.prepare(QueryUpdate);
    qry.bindValue(":p_1","1");
    qry.bindValue(":p_2",Timestamp);
    qry.bindValue(":p_3",Volt);
    qry.bindValue(":p_4",Temp);
    qry.bindValue(":p_5",FaultV);
    qry.bindValue(":p_6",FaultTmp);
    qry.exec();
}
void MainWindow::DataIn_Update_Bank_2(QString QueryUpdate, QString Timestamp, QString Volt, QString Temp, QString FaultV, QString FaultTmp)
{
    QSqlQuery qry;
    qry.prepare(QueryUpdate);
    qry.bindValue(":p_1","2");
    qry.bindValue(":p_2",Timestamp);
    qry.bindValue(":p_3",Volt);
    qry.bindValue(":p_4",Temp);
    qry.bindValue(":p_5",FaultV);
    qry.bindValue(":p_6",FaultTmp);
    qry.exec();
}
void MainWindow::DataIn_Update_Bank_3(QString QueryUpdate, QString Timestamp, QString Volt, QString Temp, QString FaultV, QString FaultTmp)
{
    QSqlQuery qry;
    qry.prepare(QueryUpdate);
    qry.bindValue(":p_1","3");
    qry.bindValue(":p_2",Timestamp);
    qry.bindValue(":p_3",Volt);
    qry.bindValue(":p_4",Temp);
    qry.bindValue(":p_5",FaultV);
    qry.bindValue(":p_6",FaultTmp);
    qry.exec();
}
void MainWindow::DataIn_Update_Bank_4(QString QueryUpdate, QString Timestamp, QString Volt, QString Temp, QString FaultV, QString FaultTmp)
{
    QSqlQuery qry;
    qry.prepare(QueryUpdate);
    qry.bindValue(":p_1","4");
    qry.bindValue(":p_2",Timestamp);
    qry.bindValue(":p_3",Volt);
    qry.bindValue(":p_4",Temp);
    qry.bindValue(":p_5",FaultV);
    qry.bindValue(":p_6",FaultTmp);
    qry.exec();
}
void MainWindow::DataIn_Update_Bank_5(QString QueryUpdate, QString Timestamp, QString Volt, QString Temp, QString FaultV, QString FaultTmp)
{
    QSqlQuery qry;
    qry.prepare(QueryUpdate);
    qry.bindValue(":p_1","5");
    qry.bindValue(":p_2",Timestamp);
    qry.bindValue(":p_3",Volt);
    qry.bindValue(":p_4",Temp);
    qry.bindValue(":p_5",FaultV);
    qry.bindValue(":p_6",FaultTmp);
    qry.exec();
}
void MainWindow::DataIn_Update_Bank_6(QString QueryUpdate, QString Timestamp, QString Volt, QString Temp, QString FaultV, QString FaultTmp)
{
    QSqlQuery qry;
    qry.prepare(QueryUpdate);
    qry.bindValue(":p_1","6");
    qry.bindValue(":p_2",Timestamp);
    qry.bindValue(":p_3",Volt);
    qry.bindValue(":p_4",Temp);
    qry.bindValue(":p_5",FaultV);
    qry.bindValue(":p_6",FaultTmp);
    qry.exec();
}
void MainWindow::DataIn_Update_Bank_7(QString QueryUpdate, QString Timestamp, QString Volt, QString Temp, QString FaultV, QString FaultTmp)
{
    QSqlQuery qry;
    qry.prepare(QueryUpdate);
    qry.bindValue(":p_1","7");
    qry.bindValue(":p_2",Timestamp);
    qry.bindValue(":p_3",Volt);
    qry.bindValue(":p_4",Temp);
    qry.bindValue(":p_5",FaultV);
    qry.bindValue(":p_6",FaultTmp);
    qry.exec();
}
void MainWindow::DataIn_Battery_Status(QString QueryUpdate, QString Timestamp, QString FaultVolt, QString FaultCurrent, QString SOC, QString DOD, QString VTOT, QString VHVDC, QString ITOT)
{
    QSqlQuery qry;
    qry.prepare(QueryUpdate);
    qry.bindValue(":p_1","1");
    qry.bindValue(":p_2",Timestamp);
    qry.bindValue(":p_3",FaultVolt);
    qry.bindValue(":p_4",FaultCurrent);
    qry.bindValue(":p_5",SOC);
    qry.bindValue(":p_6",DOD);
    qry.bindValue(":p_7",VTOT);
    qry.bindValue(":p_8",VHVDC);
    qry.bindValue(":p_9",ITOT);
    qry.exec();
}
void MainWindow::DataIn_Battery_SOC(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2)
{
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
void MainWindow::DataIn_Battery_DOD(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN_1, QString Value_IN_2)
{
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
void MainWindow::InsertINVERTER_Monitor(QString QueryCount,QString QueryInsert, QString Validation_IN,QString Timestamp,QString VR,QString VS,QString VT,QString AR,QString AS,QString AT)
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
void MainWindow::InsertINVERTER_Active(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString VR, QString VS, QString VT)
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
void MainWindow::InsertINVERTER_ReActive(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString VR, QString VS, QString VT)
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
void MainWindow::InsertINVERTER_COSPHI(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString PHIR, QString PHIS, QString PHIT)
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
void MainWindow::Set_RTU_INVERTER(QString QueryInsert, QString Timestamp, QString Value_IN)
{
    QSqlQuery query;
    query.prepare(QueryInsert);
    query.bindValue(":p_1",Timestamp);
    query.bindValue(":p_2","Server Protocol");
    query.bindValue(":p_3","RTUINVERTER");
    query.bindValue(":p_4",Value_IN);
    query.bindValue(":p_5","10");
    query.bindValue(":p_6","RTU INVERTER Condition is OFF !");
    query.exec();
}
void MainWindow::Set_CB_INVERTER(QString QueryInsert, QString Timestamp, QString Value_IN)
{
    QSqlQuery query;
    QString info;
    query.prepare(QueryInsert);
    query.bindValue(":p_1",Timestamp);
    query.bindValue(":p_2","Server Protocol");
    if (Value_IN=="1" )
    {
        info="CB INVERTER Condition is ON ";
        query.bindValue(":p_3","CBINVERTER");
        query.bindValue(":p_4","1");
        query.bindValue(":p_5","11");
        query.bindValue(":p_6",info);
        query.exec();
    }
    if(Value_IN=="0")
    {
        info="CB INVERTER Condition is OFF ";
        query.bindValue(":p_3","CBINVERTER");
        query.bindValue(":p_4","0");
        query.bindValue(":p_5","11");
        query.bindValue(":p_6",info);
        query.exec();
    }

}
void MainWindow::Set_CB_BATTERY(QString QueryInsert, QString Timestamp, QString Value_IN )
{
    QSqlQuery query;
    QString info;
    query.prepare(QueryInsert);
    query.bindValue(":p_1",Timestamp);
    query.bindValue(":p_2","Server Protocol");
    if (Value_IN=="1" )
    {
        info="CB BATTERY Condition is ON ";
        query.bindValue(":p_3","CBBATTERY");
        query.bindValue(":p_4","1");
        query.bindValue(":p_5","12");
        query.bindValue(":p_6",info);
        query.exec();
    }
    if(Value_IN=="0")
    {
        info="CB BATTERY Condition is OFF ";
        query.bindValue(":p_3","CBBATTERY");
        query.bindValue(":p_4","0");
        query.bindValue(":p_5","12");
        query.bindValue(":p_6",info);
        query.exec();
    }
}
void MainWindow::Set_Point_Mode(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::Set_Point_W(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::Set_Point_Var(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::DataIn_Freq(QString QueryCount, QString QueryInsert, QString Validation_IN, QString Timestamp, QString Value_IN)
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
void MainWindow::ClockSync_clicked()
{
    i104.clockSync();
}

void MainWindow::on_pbConnect_2_clicked()
{
    QSettings set(QDir::currentPath() + "/dbconfig.ini",
                  QSettings::IniFormat );
    //set setting
    set.beginGroup("Settings_DB_Protocol");
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

void MainWindow::load_Db(QString Host,QString Database,QString Username,QString Password)
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
    load_Db(Server,Database,Username,Password);
    set.endGroup();
}

void MainWindow::ConnectionRTU()
{
    QString time;
    time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QSqlQuery qry_2;
    QString value="0";
    qry_2.prepare("insert into in_cb_rtu_INVERTER (timestamps,value) values (:p_1,:p_2)");
    qry_2.bindValue(":p_1",time);
    qry_2.bindValue(":p_2",value);
    qry_2.exec();

    Set_RTU_INVERTER("insert into all_control_protocol_protocol(timestamps,operatorname,status,outvalue,ioa,information) values(:p_1,:p_2,:p_3,:p_4,:p_5,:p_6)",
                     time,value);

}
