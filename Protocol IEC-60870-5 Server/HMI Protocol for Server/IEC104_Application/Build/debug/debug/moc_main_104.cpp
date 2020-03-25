/****************************************************************************
** Meta object code from reading C++ file 'main_104.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../main_104.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_104.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_main_104_t {
    QByteArrayData data[98];
    char stringdata0[1607];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_main_104_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_main_104_t qt_meta_stringdata_main_104 = {
    {
QT_MOC_LITERAL(0, 0, 8), // "main_104"
QT_MOC_LITERAL(1, 9, 9), // "get_cb_db"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 16), // "get_time_archive"
QT_MOC_LITERAL(4, 37, 11), // "get_cb_plts"
QT_MOC_LITERAL(5, 49, 12), // "get_cb_pltmh"
QT_MOC_LITERAL(6, 62, 15), // "get_cb_inverter"
QT_MOC_LITERAL(7, 78, 11), // "get_cb_load"
QT_MOC_LITERAL(8, 90, 16), // "on_pb_db_clicked"
QT_MOC_LITERAL(9, 107, 18), // "on_pb_plts_clicked"
QT_MOC_LITERAL(10, 126, 19), // "on_pb_pltmh_clicked"
QT_MOC_LITERAL(11, 146, 22), // "on_pb_inverter_clicked"
QT_MOC_LITERAL(12, 169, 18), // "on_pb_load_clicked"
QT_MOC_LITERAL(13, 188, 12), // "rtu_plts_off"
QT_MOC_LITERAL(14, 201, 5), // "Query"
QT_MOC_LITERAL(15, 207, 9), // "Timestamp"
QT_MOC_LITERAL(16, 217, 5), // "Value"
QT_MOC_LITERAL(17, 223, 13), // "rtu_pltmh_off"
QT_MOC_LITERAL(18, 237, 16), // "rtu_inverter_off"
QT_MOC_LITERAL(19, 254, 12), // "rtu_load_off"
QT_MOC_LITERAL(20, 267, 11), // "Set_RTU_OFF"
QT_MOC_LITERAL(21, 279, 6), // "values"
QT_MOC_LITERAL(22, 286, 12), // "Get_settings"
QT_MOC_LITERAL(23, 299, 14), // "Get_settingsdb"
QT_MOC_LITERAL(24, 314, 12), // "Set_settings"
QT_MOC_LITERAL(25, 327, 14), // "Clear_settings"
QT_MOC_LITERAL(26, 342, 15), // "get_hmi_archive"
QT_MOC_LITERAL(27, 358, 20), // "on_pb_config_clicked"
QT_MOC_LITERAL(28, 379, 8), // "UsageCPU"
QT_MOC_LITERAL(29, 388, 20), // "RealtimeDigitalInput"
QT_MOC_LITERAL(30, 409, 16), // "insertLogArchive"
QT_MOC_LITERAL(31, 426, 13), // "ConfigStartup"
QT_MOC_LITERAL(32, 440, 4), // "Mode"
QT_MOC_LITERAL(33, 445, 20), // "AutoStartupOnWindows"
QT_MOC_LITERAL(34, 466, 15), // "AutoSyncWindows"
QT_MOC_LITERAL(35, 482, 18), // "ArchiveIntegration"
QT_MOC_LITERAL(36, 501, 18), // "get_status_running"
QT_MOC_LITERAL(37, 520, 15), // "set_backup_List"
QT_MOC_LITERAL(38, 536, 14), // "set_ActivePLTS"
QT_MOC_LITERAL(39, 551, 16), // "set_ReactivePLTS"
QT_MOC_LITERAL(40, 568, 15), // "set_InPowerPLTS"
QT_MOC_LITERAL(41, 584, 11), // "set_MonPLTS"
QT_MOC_LITERAL(42, 596, 14), // "set_CosphiPLTS"
QT_MOC_LITERAL(43, 611, 17), // "set_RadiationPLTS"
QT_MOC_LITERAL(44, 629, 15), // "set_ActivePLTMH"
QT_MOC_LITERAL(45, 645, 17), // "set_ReactivePLTMH"
QT_MOC_LITERAL(46, 663, 16), // "set_InPowerPLTMH"
QT_MOC_LITERAL(47, 680, 12), // "set_MonPLTMH"
QT_MOC_LITERAL(48, 693, 15), // "set_CosphiPLTMH"
QT_MOC_LITERAL(49, 709, 18), // "set_ActiveINVERTER"
QT_MOC_LITERAL(50, 728, 20), // "set_ReactiveINVERTER"
QT_MOC_LITERAL(51, 749, 19), // "set_InPowerINVERTER"
QT_MOC_LITERAL(52, 769, 15), // "set_MonINVERTER"
QT_MOC_LITERAL(53, 785, 18), // "set_CosphiINVERTER"
QT_MOC_LITERAL(54, 804, 14), // "set_ActiveLOAD"
QT_MOC_LITERAL(55, 819, 16), // "set_ReactiveLOAD"
QT_MOC_LITERAL(56, 836, 15), // "set_InPowerLOAD"
QT_MOC_LITERAL(57, 852, 11), // "set_MonLOAD"
QT_MOC_LITERAL(58, 864, 14), // "set_CosphiLOAD"
QT_MOC_LITERAL(59, 879, 27), // "insert_status_alloutcontrol"
QT_MOC_LITERAL(60, 907, 8), // "Operator"
QT_MOC_LITERAL(61, 916, 6), // "Status"
QT_MOC_LITERAL(62, 923, 5), // "value"
QT_MOC_LITERAL(63, 929, 3), // "IOA"
QT_MOC_LITERAL(64, 933, 4), // "Info"
QT_MOC_LITERAL(65, 938, 17), // "set_TruncateTable"
QT_MOC_LITERAL(66, 956, 8), // "set_PLTS"
QT_MOC_LITERAL(67, 965, 9), // "set_PLTMH"
QT_MOC_LITERAL(68, 975, 12), // "set_INVERTER"
QT_MOC_LITERAL(69, 988, 8), // "set_LOAD"
QT_MOC_LITERAL(70, 997, 11), // "set_Control"
QT_MOC_LITERAL(71, 1009, 22), // "set_TruncatePLTSActive"
QT_MOC_LITERAL(72, 1032, 24), // "set_TruncatePLTSReactive"
QT_MOC_LITERAL(73, 1057, 23), // "set_TruncatePLTSinPower"
QT_MOC_LITERAL(74, 1081, 22), // "set_TruncatePLTSCosphi"
QT_MOC_LITERAL(75, 1104, 19), // "set_TruncatePLTSMon"
QT_MOC_LITERAL(76, 1124, 25), // "set_TruncatePLTSRadiation"
QT_MOC_LITERAL(77, 1150, 23), // "set_TruncatePLTMHActive"
QT_MOC_LITERAL(78, 1174, 25), // "set_TruncatePLTMHReactive"
QT_MOC_LITERAL(79, 1200, 24), // "set_TruncatePLTMHinPower"
QT_MOC_LITERAL(80, 1225, 23), // "set_TruncatePLTMHCosphi"
QT_MOC_LITERAL(81, 1249, 20), // "set_TruncatePLTMHMon"
QT_MOC_LITERAL(82, 1270, 26), // "set_TruncateINVERTERActive"
QT_MOC_LITERAL(83, 1297, 28), // "set_TruncateINVERTERReactive"
QT_MOC_LITERAL(84, 1326, 27), // "set_TruncateINVERTERinPower"
QT_MOC_LITERAL(85, 1354, 26), // "set_TruncateINVERTERCosphi"
QT_MOC_LITERAL(86, 1381, 23), // "set_TruncateINVERTERMon"
QT_MOC_LITERAL(87, 1405, 22), // "set_TruncateLOADActive"
QT_MOC_LITERAL(88, 1428, 24), // "set_TruncateLOADReactive"
QT_MOC_LITERAL(89, 1453, 23), // "set_TruncateLOADinPower"
QT_MOC_LITERAL(90, 1477, 22), // "set_TruncateLOADCosphi"
QT_MOC_LITERAL(91, 1500, 19), // "set_TruncateLOADMon"
QT_MOC_LITERAL(92, 1520, 19), // "set_TruncateControl"
QT_MOC_LITERAL(93, 1540, 6), // "Log_db"
QT_MOC_LITERAL(94, 1547, 11), // "Information"
QT_MOC_LITERAL(95, 1559, 12), // "SetLoggerNow"
QT_MOC_LITERAL(96, 1572, 18), // "on_pb_db_2_clicked"
QT_MOC_LITERAL(97, 1591, 15) // "RepairOnSrartup"

    },
    "main_104\0get_cb_db\0\0get_time_archive\0"
    "get_cb_plts\0get_cb_pltmh\0get_cb_inverter\0"
    "get_cb_load\0on_pb_db_clicked\0"
    "on_pb_plts_clicked\0on_pb_pltmh_clicked\0"
    "on_pb_inverter_clicked\0on_pb_load_clicked\0"
    "rtu_plts_off\0Query\0Timestamp\0Value\0"
    "rtu_pltmh_off\0rtu_inverter_off\0"
    "rtu_load_off\0Set_RTU_OFF\0values\0"
    "Get_settings\0Get_settingsdb\0Set_settings\0"
    "Clear_settings\0get_hmi_archive\0"
    "on_pb_config_clicked\0UsageCPU\0"
    "RealtimeDigitalInput\0insertLogArchive\0"
    "ConfigStartup\0Mode\0AutoStartupOnWindows\0"
    "AutoSyncWindows\0ArchiveIntegration\0"
    "get_status_running\0set_backup_List\0"
    "set_ActivePLTS\0set_ReactivePLTS\0"
    "set_InPowerPLTS\0set_MonPLTS\0set_CosphiPLTS\0"
    "set_RadiationPLTS\0set_ActivePLTMH\0"
    "set_ReactivePLTMH\0set_InPowerPLTMH\0"
    "set_MonPLTMH\0set_CosphiPLTMH\0"
    "set_ActiveINVERTER\0set_ReactiveINVERTER\0"
    "set_InPowerINVERTER\0set_MonINVERTER\0"
    "set_CosphiINVERTER\0set_ActiveLOAD\0"
    "set_ReactiveLOAD\0set_InPowerLOAD\0"
    "set_MonLOAD\0set_CosphiLOAD\0"
    "insert_status_alloutcontrol\0Operator\0"
    "Status\0value\0IOA\0Info\0set_TruncateTable\0"
    "set_PLTS\0set_PLTMH\0set_INVERTER\0"
    "set_LOAD\0set_Control\0set_TruncatePLTSActive\0"
    "set_TruncatePLTSReactive\0"
    "set_TruncatePLTSinPower\0set_TruncatePLTSCosphi\0"
    "set_TruncatePLTSMon\0set_TruncatePLTSRadiation\0"
    "set_TruncatePLTMHActive\0"
    "set_TruncatePLTMHReactive\0"
    "set_TruncatePLTMHinPower\0"
    "set_TruncatePLTMHCosphi\0set_TruncatePLTMHMon\0"
    "set_TruncateINVERTERActive\0"
    "set_TruncateINVERTERReactive\0"
    "set_TruncateINVERTERinPower\0"
    "set_TruncateINVERTERCosphi\0"
    "set_TruncateINVERTERMon\0set_TruncateLOADActive\0"
    "set_TruncateLOADReactive\0"
    "set_TruncateLOADinPower\0set_TruncateLOADCosphi\0"
    "set_TruncateLOADMon\0set_TruncateControl\0"
    "Log_db\0Information\0SetLoggerNow\0"
    "on_pb_db_2_clicked\0RepairOnSrartup"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_main_104[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      85,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  439,    2, 0x08 /* Private */,
       3,    0,  440,    2, 0x08 /* Private */,
       4,    0,  441,    2, 0x08 /* Private */,
       5,    0,  442,    2, 0x08 /* Private */,
       6,    0,  443,    2, 0x08 /* Private */,
       7,    0,  444,    2, 0x08 /* Private */,
       8,    0,  445,    2, 0x08 /* Private */,
       9,    0,  446,    2, 0x08 /* Private */,
      10,    0,  447,    2, 0x08 /* Private */,
      11,    0,  448,    2, 0x08 /* Private */,
      12,    0,  449,    2, 0x08 /* Private */,
      13,    3,  450,    2, 0x08 /* Private */,
      17,    3,  457,    2, 0x08 /* Private */,
      18,    3,  464,    2, 0x08 /* Private */,
      19,    3,  471,    2, 0x08 /* Private */,
      20,    1,  478,    2, 0x08 /* Private */,
      22,    0,  481,    2, 0x08 /* Private */,
      23,    0,  482,    2, 0x08 /* Private */,
      24,    0,  483,    2, 0x08 /* Private */,
      25,    0,  484,    2, 0x08 /* Private */,
      26,    0,  485,    2, 0x08 /* Private */,
      27,    0,  486,    2, 0x08 /* Private */,
      28,    0,  487,    2, 0x08 /* Private */,
      29,    0,  488,    2, 0x08 /* Private */,
      30,    3,  489,    2, 0x08 /* Private */,
      31,    1,  496,    2, 0x08 /* Private */,
      33,    0,  499,    2, 0x08 /* Private */,
      34,    0,  500,    2, 0x08 /* Private */,
      35,    0,  501,    2, 0x08 /* Private */,
      36,    0,  502,    2, 0x08 /* Private */,
      37,    1,  503,    2, 0x08 /* Private */,
      38,    1,  506,    2, 0x08 /* Private */,
      39,    1,  509,    2, 0x08 /* Private */,
      40,    1,  512,    2, 0x08 /* Private */,
      41,    1,  515,    2, 0x08 /* Private */,
      42,    1,  518,    2, 0x08 /* Private */,
      43,    1,  521,    2, 0x08 /* Private */,
      44,    1,  524,    2, 0x08 /* Private */,
      45,    1,  527,    2, 0x08 /* Private */,
      46,    1,  530,    2, 0x08 /* Private */,
      47,    1,  533,    2, 0x08 /* Private */,
      48,    1,  536,    2, 0x08 /* Private */,
      49,    1,  539,    2, 0x08 /* Private */,
      50,    1,  542,    2, 0x08 /* Private */,
      51,    1,  545,    2, 0x08 /* Private */,
      52,    1,  548,    2, 0x08 /* Private */,
      53,    1,  551,    2, 0x08 /* Private */,
      54,    1,  554,    2, 0x08 /* Private */,
      55,    1,  557,    2, 0x08 /* Private */,
      56,    1,  560,    2, 0x08 /* Private */,
      57,    1,  563,    2, 0x08 /* Private */,
      58,    1,  566,    2, 0x08 /* Private */,
      59,    7,  569,    2, 0x08 /* Private */,
      65,    0,  584,    2, 0x08 /* Private */,
      66,    0,  585,    2, 0x08 /* Private */,
      67,    0,  586,    2, 0x08 /* Private */,
      68,    0,  587,    2, 0x08 /* Private */,
      69,    0,  588,    2, 0x08 /* Private */,
      70,    0,  589,    2, 0x08 /* Private */,
      71,    1,  590,    2, 0x08 /* Private */,
      72,    1,  593,    2, 0x08 /* Private */,
      73,    1,  596,    2, 0x08 /* Private */,
      74,    1,  599,    2, 0x08 /* Private */,
      75,    1,  602,    2, 0x08 /* Private */,
      76,    1,  605,    2, 0x08 /* Private */,
      77,    1,  608,    2, 0x08 /* Private */,
      78,    1,  611,    2, 0x08 /* Private */,
      79,    1,  614,    2, 0x08 /* Private */,
      80,    1,  617,    2, 0x08 /* Private */,
      81,    1,  620,    2, 0x08 /* Private */,
      82,    1,  623,    2, 0x08 /* Private */,
      83,    1,  626,    2, 0x08 /* Private */,
      84,    1,  629,    2, 0x08 /* Private */,
      85,    1,  632,    2, 0x08 /* Private */,
      86,    1,  635,    2, 0x08 /* Private */,
      87,    1,  638,    2, 0x08 /* Private */,
      88,    1,  641,    2, 0x08 /* Private */,
      89,    1,  644,    2, 0x08 /* Private */,
      90,    1,  647,    2, 0x08 /* Private */,
      91,    1,  650,    2, 0x08 /* Private */,
      92,    1,  653,    2, 0x08 /* Private */,
      93,    4,  656,    2, 0x08 /* Private */,
      95,    2,  665,    2, 0x08 /* Private */,
      96,    0,  670,    2, 0x08 /* Private */,
      97,    0,  671,    2, 0x08 /* Private */,

 // slots: parameters
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
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   14,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   14,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   14,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   14,   15,   16,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   14,   15,   16,
    QMetaType::Void, QMetaType::QString,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   32,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   14,   15,   60,   61,   62,   63,   64,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   14,   15,   61,   94,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   61,   94,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void main_104::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        main_104 *_t = static_cast<main_104 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->get_cb_db(); break;
        case 1: _t->get_time_archive(); break;
        case 2: _t->get_cb_plts(); break;
        case 3: _t->get_cb_pltmh(); break;
        case 4: _t->get_cb_inverter(); break;
        case 5: _t->get_cb_load(); break;
        case 6: _t->on_pb_db_clicked(); break;
        case 7: _t->on_pb_plts_clicked(); break;
        case 8: _t->on_pb_pltmh_clicked(); break;
        case 9: _t->on_pb_inverter_clicked(); break;
        case 10: _t->on_pb_load_clicked(); break;
        case 11: _t->rtu_plts_off((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 12: _t->rtu_pltmh_off((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 13: _t->rtu_inverter_off((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 14: _t->rtu_load_off((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 15: _t->Set_RTU_OFF((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->Get_settings(); break;
        case 17: _t->Get_settingsdb(); break;
        case 18: _t->Set_settings(); break;
        case 19: _t->Clear_settings(); break;
        case 20: _t->get_hmi_archive(); break;
        case 21: _t->on_pb_config_clicked(); break;
        case 22: _t->UsageCPU(); break;
        case 23: _t->RealtimeDigitalInput(); break;
        case 24: _t->insertLogArchive((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 25: _t->ConfigStartup((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->AutoStartupOnWindows(); break;
        case 27: _t->AutoSyncWindows(); break;
        case 28: _t->ArchiveIntegration(); break;
        case 29: _t->get_status_running(); break;
        case 30: _t->set_backup_List((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 31: _t->set_ActivePLTS((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 32: _t->set_ReactivePLTS((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 33: _t->set_InPowerPLTS((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 34: _t->set_MonPLTS((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 35: _t->set_CosphiPLTS((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 36: _t->set_RadiationPLTS((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 37: _t->set_ActivePLTMH((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 38: _t->set_ReactivePLTMH((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 39: _t->set_InPowerPLTMH((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 40: _t->set_MonPLTMH((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 41: _t->set_CosphiPLTMH((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 42: _t->set_ActiveINVERTER((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 43: _t->set_ReactiveINVERTER((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 44: _t->set_InPowerINVERTER((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 45: _t->set_MonINVERTER((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 46: _t->set_CosphiINVERTER((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 47: _t->set_ActiveLOAD((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 48: _t->set_ReactiveLOAD((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 49: _t->set_InPowerLOAD((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 50: _t->set_MonLOAD((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 51: _t->set_CosphiLOAD((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 52: _t->insert_status_alloutcontrol((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 53: _t->set_TruncateTable(); break;
        case 54: _t->set_PLTS(); break;
        case 55: _t->set_PLTMH(); break;
        case 56: _t->set_INVERTER(); break;
        case 57: _t->set_LOAD(); break;
        case 58: _t->set_Control(); break;
        case 59: _t->set_TruncatePLTSActive((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 60: _t->set_TruncatePLTSReactive((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 61: _t->set_TruncatePLTSinPower((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 62: _t->set_TruncatePLTSCosphi((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 63: _t->set_TruncatePLTSMon((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 64: _t->set_TruncatePLTSRadiation((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 65: _t->set_TruncatePLTMHActive((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 66: _t->set_TruncatePLTMHReactive((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 67: _t->set_TruncatePLTMHinPower((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 68: _t->set_TruncatePLTMHCosphi((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 69: _t->set_TruncatePLTMHMon((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 70: _t->set_TruncateINVERTERActive((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 71: _t->set_TruncateINVERTERReactive((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 72: _t->set_TruncateINVERTERinPower((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 73: _t->set_TruncateINVERTERCosphi((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 74: _t->set_TruncateINVERTERMon((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 75: _t->set_TruncateLOADActive((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 76: _t->set_TruncateLOADReactive((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 77: _t->set_TruncateLOADinPower((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 78: _t->set_TruncateLOADCosphi((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 79: _t->set_TruncateLOADMon((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 80: _t->set_TruncateControl((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 81: _t->Log_db((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 82: _t->SetLoggerNow((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 83: _t->on_pb_db_2_clicked(); break;
        case 84: _t->RepairOnSrartup(); break;
        default: ;
        }
    }
}

const QMetaObject main_104::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_main_104.data,
      qt_meta_data_main_104,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *main_104::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *main_104::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_main_104.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int main_104::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 85)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 85;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 85)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 85;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
