/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[82];
    char stringdata0[1159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 21), // "slot_timer_BDTR_kamsg"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 25), // "slot_BDTR_pronto_para_ler"
QT_MOC_LITERAL(4, 60, 19), // "slot_dataIndication"
QT_MOC_LITERAL(5, 80, 8), // "iec_obj*"
QT_MOC_LITERAL(6, 89, 3), // "obj"
QT_MOC_LITERAL(7, 93, 9), // "numpoints"
QT_MOC_LITERAL(8, 103, 35), // "slot_interrogationActConfIndi..."
QT_MOC_LITERAL(9, 139, 35), // "slot_interrogationActTermIndi..."
QT_MOC_LITERAL(10, 175, 15), // "slot_tcpconnect"
QT_MOC_LITERAL(11, 191, 18), // "slot_tcpdisconnect"
QT_MOC_LITERAL(12, 210, 29), // "slot_commandActConfIndication"
QT_MOC_LITERAL(13, 240, 22), // "Trigger_SendValueToIEC"
QT_MOC_LITERAL(14, 263, 8), // "GetValue"
QT_MOC_LITERAL(15, 272, 13), // "INSERTALLDATA"
QT_MOC_LITERAL(16, 286, 11), // "DataIn_Plts"
QT_MOC_LITERAL(17, 298, 10), // "QueryCount"
QT_MOC_LITERAL(18, 309, 11), // "QueryInsert"
QT_MOC_LITERAL(19, 321, 13), // "Validation_IN"
QT_MOC_LITERAL(20, 335, 9), // "Timestamp"
QT_MOC_LITERAL(21, 345, 8), // "Value_IN"
QT_MOC_LITERAL(22, 354, 12), // "DataIn_Plts1"
QT_MOC_LITERAL(23, 367, 12), // "DataIn_Plts2"
QT_MOC_LITERAL(24, 380, 12), // "DataIn_Plts3"
QT_MOC_LITERAL(25, 393, 15), // "Datain_RTU_Plts"
QT_MOC_LITERAL(26, 409, 14), // "Datain_CB_Plts"
QT_MOC_LITERAL(27, 424, 10), // "Value_IN_1"
QT_MOC_LITERAL(28, 435, 17), // "Datain_Plts_W_VAR"
QT_MOC_LITERAL(29, 453, 10), // "Value_IN_2"
QT_MOC_LITERAL(30, 464, 13), // "DataIn_Status"
QT_MOC_LITERAL(31, 478, 17), // "DataIn_Status_RTU"
QT_MOC_LITERAL(32, 496, 16), // "DataIn_Status_CB"
QT_MOC_LITERAL(33, 513, 22), // "DataIn_Status_IN_POWER"
QT_MOC_LITERAL(34, 536, 21), // "DataIn_Status_IN_FREQ"
QT_MOC_LITERAL(35, 558, 12), // "InsertAllRST"
QT_MOC_LITERAL(36, 571, 19), // "InsertAllRST_COSPHI"
QT_MOC_LITERAL(37, 591, 21), // "Insert_Inverter_Block"
QT_MOC_LITERAL(38, 613, 26), // "Insert_Inverter_PLTS_FAULT"
QT_MOC_LITERAL(39, 640, 22), // "Datain_Status_Registry"
QT_MOC_LITERAL(40, 663, 17), // "ClockSync_clicked"
QT_MOC_LITERAL(41, 681, 22), // "on_pbConnect_2_clicked"
QT_MOC_LITERAL(42, 704, 7), // "Plts_Db"
QT_MOC_LITERAL(43, 712, 4), // "Host"
QT_MOC_LITERAL(44, 717, 8), // "Database"
QT_MOC_LITERAL(45, 726, 8), // "Username"
QT_MOC_LITERAL(46, 735, 8), // "Password"
QT_MOC_LITERAL(47, 744, 12), // "ControllerDB"
QT_MOC_LITERAL(48, 757, 18), // "InsertPlts_Monitor"
QT_MOC_LITERAL(49, 776, 2), // "VR"
QT_MOC_LITERAL(50, 779, 2), // "VS"
QT_MOC_LITERAL(51, 782, 2), // "VT"
QT_MOC_LITERAL(52, 785, 2), // "AR"
QT_MOC_LITERAL(53, 788, 2), // "AS"
QT_MOC_LITERAL(54, 791, 2), // "AT"
QT_MOC_LITERAL(55, 794, 27), // "InsertAllRST_ActiveReactive"
QT_MOC_LITERAL(56, 822, 17), // "InsertPlts_Active"
QT_MOC_LITERAL(57, 840, 19), // "InsertPlts_ReActive"
QT_MOC_LITERAL(58, 860, 17), // "InsertPlts_COSPHI"
QT_MOC_LITERAL(59, 878, 4), // "PHIR"
QT_MOC_LITERAL(60, 883, 4), // "PHIS"
QT_MOC_LITERAL(61, 888, 4), // "PHIT"
QT_MOC_LITERAL(62, 893, 12), // "PLTS_BLOCK_R"
QT_MOC_LITERAL(63, 906, 4), // "Watt"
QT_MOC_LITERAL(64, 911, 4), // "Volt"
QT_MOC_LITERAL(65, 916, 6), // "Ampere"
QT_MOC_LITERAL(66, 923, 12), // "PLTS_BLOCK_S"
QT_MOC_LITERAL(67, 936, 12), // "PLTS_BLOCK_T"
QT_MOC_LITERAL(68, 949, 10), // "PLTS_INV_R"
QT_MOC_LITERAL(69, 960, 10), // "PLTS_INV_S"
QT_MOC_LITERAL(70, 971, 10), // "PLTS_INV_T"
QT_MOC_LITERAL(71, 982, 16), // "PLTS_INV_R_FAULT"
QT_MOC_LITERAL(72, 999, 16), // "PLTS_INV_S_FAULT"
QT_MOC_LITERAL(73, 1016, 16), // "PLTS_INV_T_FAULT"
QT_MOC_LITERAL(74, 1033, 16), // "PLTS_SPV_R_FAULT"
QT_MOC_LITERAL(75, 1050, 16), // "PLTS_SPV_S_FAULT"
QT_MOC_LITERAL(76, 1067, 16), // "PLTS_SPV_T_FAULT"
QT_MOC_LITERAL(77, 1084, 12), // "Set_RTU_Plts"
QT_MOC_LITERAL(78, 1097, 11), // "Set_CB_Plts"
QT_MOC_LITERAL(79, 1109, 20), // "Set_RTU_Plts_Offline"
QT_MOC_LITERAL(80, 1130, 13), // "Set_Plts_Freq"
QT_MOC_LITERAL(81, 1144, 14) // "set_OfflineRTU"

    },
    "MainWindow\0slot_timer_BDTR_kamsg\0\0"
    "slot_BDTR_pronto_para_ler\0slot_dataIndication\0"
    "iec_obj*\0obj\0numpoints\0"
    "slot_interrogationActConfIndication\0"
    "slot_interrogationActTermIndication\0"
    "slot_tcpconnect\0slot_tcpdisconnect\0"
    "slot_commandActConfIndication\0"
    "Trigger_SendValueToIEC\0GetValue\0"
    "INSERTALLDATA\0DataIn_Plts\0QueryCount\0"
    "QueryInsert\0Validation_IN\0Timestamp\0"
    "Value_IN\0DataIn_Plts1\0DataIn_Plts2\0"
    "DataIn_Plts3\0Datain_RTU_Plts\0"
    "Datain_CB_Plts\0Value_IN_1\0Datain_Plts_W_VAR\0"
    "Value_IN_2\0DataIn_Status\0DataIn_Status_RTU\0"
    "DataIn_Status_CB\0DataIn_Status_IN_POWER\0"
    "DataIn_Status_IN_FREQ\0InsertAllRST\0"
    "InsertAllRST_COSPHI\0Insert_Inverter_Block\0"
    "Insert_Inverter_PLTS_FAULT\0"
    "Datain_Status_Registry\0ClockSync_clicked\0"
    "on_pbConnect_2_clicked\0Plts_Db\0Host\0"
    "Database\0Username\0Password\0ControllerDB\0"
    "InsertPlts_Monitor\0VR\0VS\0VT\0AR\0AS\0AT\0"
    "InsertAllRST_ActiveReactive\0"
    "InsertPlts_Active\0InsertPlts_ReActive\0"
    "InsertPlts_COSPHI\0PHIR\0PHIS\0PHIT\0"
    "PLTS_BLOCK_R\0Watt\0Volt\0Ampere\0"
    "PLTS_BLOCK_S\0PLTS_BLOCK_T\0PLTS_INV_R\0"
    "PLTS_INV_S\0PLTS_INV_T\0PLTS_INV_R_FAULT\0"
    "PLTS_INV_S_FAULT\0PLTS_INV_T_FAULT\0"
    "PLTS_SPV_R_FAULT\0PLTS_SPV_S_FAULT\0"
    "PLTS_SPV_T_FAULT\0Set_RTU_Plts\0Set_CB_Plts\0"
    "Set_RTU_Plts_Offline\0Set_Plts_Freq\0"
    "set_OfflineRTU"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      54,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  284,    2, 0x08 /* Private */,
       3,    0,  285,    2, 0x08 /* Private */,
       4,    2,  286,    2, 0x08 /* Private */,
       8,    0,  291,    2, 0x08 /* Private */,
       9,    0,  292,    2, 0x08 /* Private */,
      10,    0,  293,    2, 0x08 /* Private */,
      11,    0,  294,    2, 0x08 /* Private */,
      12,    1,  295,    2, 0x08 /* Private */,
      13,    0,  298,    2, 0x08 /* Private */,
      14,    0,  299,    2, 0x08 /* Private */,
      15,    0,  300,    2, 0x08 /* Private */,
      16,    5,  301,    2, 0x08 /* Private */,
      22,    5,  312,    2, 0x08 /* Private */,
      23,    5,  323,    2, 0x08 /* Private */,
      24,    5,  334,    2, 0x08 /* Private */,
      25,    5,  345,    2, 0x08 /* Private */,
      26,    5,  356,    2, 0x08 /* Private */,
      28,    6,  367,    2, 0x08 /* Private */,
      30,    0,  380,    2, 0x08 /* Private */,
      31,    0,  381,    2, 0x08 /* Private */,
      32,    0,  382,    2, 0x08 /* Private */,
      33,    0,  383,    2, 0x08 /* Private */,
      34,    0,  384,    2, 0x08 /* Private */,
      35,    0,  385,    2, 0x08 /* Private */,
      36,    0,  386,    2, 0x08 /* Private */,
      37,    0,  387,    2, 0x08 /* Private */,
      38,    0,  388,    2, 0x08 /* Private */,
      39,    0,  389,    2, 0x08 /* Private */,
      40,    0,  390,    2, 0x08 /* Private */,
      41,    0,  391,    2, 0x08 /* Private */,
      42,    4,  392,    2, 0x08 /* Private */,
      47,    0,  401,    2, 0x08 /* Private */,
      48,   10,  402,    2, 0x08 /* Private */,
      55,    0,  423,    2, 0x08 /* Private */,
      56,    7,  424,    2, 0x08 /* Private */,
      57,    7,  439,    2, 0x08 /* Private */,
      58,    7,  454,    2, 0x08 /* Private */,
      62,    7,  469,    2, 0x08 /* Private */,
      66,    7,  484,    2, 0x08 /* Private */,
      67,    7,  499,    2, 0x08 /* Private */,
      68,    7,  514,    2, 0x08 /* Private */,
      69,    7,  529,    2, 0x08 /* Private */,
      70,    7,  544,    2, 0x08 /* Private */,
      71,    5,  559,    2, 0x08 /* Private */,
      72,    5,  570,    2, 0x08 /* Private */,
      73,    5,  581,    2, 0x08 /* Private */,
      74,    5,  592,    2, 0x08 /* Private */,
      75,    5,  603,    2, 0x08 /* Private */,
      76,    5,  614,    2, 0x08 /* Private */,
      77,    3,  625,    2, 0x08 /* Private */,
      78,    2,  632,    2, 0x08 /* Private */,
      79,    3,  637,    2, 0x08 /* Private */,
      80,    5,  644,    2, 0x08 /* Private */,
      81,    0,  655,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   27,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   27,   29,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   43,   44,   45,   46,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   49,   50,   51,   52,   53,   54,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   49,   50,   51,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   49,   50,   51,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   59,   60,   61,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   63,   64,   65,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   63,   64,   65,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   63,   64,   65,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   63,   64,   65,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   63,   64,   65,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   63,   64,   65,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   18,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   18,   20,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   18,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,   21,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slot_timer_BDTR_kamsg(); break;
        case 1: _t->slot_BDTR_pronto_para_ler(); break;
        case 2: _t->slot_dataIndication((*reinterpret_cast< iec_obj*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->slot_interrogationActConfIndication(); break;
        case 4: _t->slot_interrogationActTermIndication(); break;
        case 5: _t->slot_tcpconnect(); break;
        case 6: _t->slot_tcpdisconnect(); break;
        case 7: _t->slot_commandActConfIndication((*reinterpret_cast< iec_obj*(*)>(_a[1]))); break;
        case 8: _t->Trigger_SendValueToIEC(); break;
        case 9: _t->GetValue(); break;
        case 10: _t->INSERTALLDATA(); break;
        case 11: _t->DataIn_Plts((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 12: _t->DataIn_Plts1((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 13: _t->DataIn_Plts2((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 14: _t->DataIn_Plts3((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 15: _t->Datain_RTU_Plts((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 16: _t->Datain_CB_Plts((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 17: _t->Datain_Plts_W_VAR((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6]))); break;
        case 18: _t->DataIn_Status(); break;
        case 19: _t->DataIn_Status_RTU(); break;
        case 20: _t->DataIn_Status_CB(); break;
        case 21: _t->DataIn_Status_IN_POWER(); break;
        case 22: _t->DataIn_Status_IN_FREQ(); break;
        case 23: _t->InsertAllRST(); break;
        case 24: _t->InsertAllRST_COSPHI(); break;
        case 25: _t->Insert_Inverter_Block(); break;
        case 26: _t->Insert_Inverter_PLTS_FAULT(); break;
        case 27: _t->Datain_Status_Registry(); break;
        case 28: _t->ClockSync_clicked(); break;
        case 29: _t->on_pbConnect_2_clicked(); break;
        case 30: _t->Plts_Db((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 31: _t->ControllerDB(); break;
        case 32: _t->InsertPlts_Monitor((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8])),(*reinterpret_cast< QString(*)>(_a[9])),(*reinterpret_cast< QString(*)>(_a[10]))); break;
        case 33: _t->InsertAllRST_ActiveReactive(); break;
        case 34: _t->InsertPlts_Active((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 35: _t->InsertPlts_ReActive((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 36: _t->InsertPlts_COSPHI((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 37: _t->PLTS_BLOCK_R((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 38: _t->PLTS_BLOCK_S((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 39: _t->PLTS_BLOCK_T((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 40: _t->PLTS_INV_R((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 41: _t->PLTS_INV_S((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 42: _t->PLTS_INV_T((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 43: _t->PLTS_INV_R_FAULT((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 44: _t->PLTS_INV_S_FAULT((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 45: _t->PLTS_INV_T_FAULT((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 46: _t->PLTS_SPV_R_FAULT((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 47: _t->PLTS_SPV_S_FAULT((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 48: _t->PLTS_SPV_T_FAULT((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 49: _t->Set_RTU_Plts((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 50: _t->Set_CB_Plts((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 51: _t->Set_RTU_Plts_Offline((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 52: _t->Set_Plts_Freq((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 53: _t->set_OfflineRTU(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 54)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 54;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 54)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 54;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
