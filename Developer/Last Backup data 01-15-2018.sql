-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.7.14-log - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL Version:             9.4.0.5125
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- Dumping database structure for db_scada
CREATE DATABASE IF NOT EXISTS `db_scada` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `db_scada`;

-- Dumping structure for table db_scada.alloutcontrol
CREATE TABLE IF NOT EXISTS `alloutcontrol` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `operatorname` varchar(50) NOT NULL,
  `Status` varchar(50) NOT NULL,
  `outvalue` varchar(50) NOT NULL,
  `IOA` varchar(50) NOT NULL,
  `information` varchar(250) NOT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`Status`,`outvalue`)
) ENGINE=MyISAM AUTO_INCREMENT=849 DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED COMMENT='control kontaktor dan status eventlist pada hmi';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.all_control_protocol
CREATE TABLE IF NOT EXISTS `all_control_protocol` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `operatorname` varchar(50) NOT NULL,
  `Status` varchar(50) NOT NULL,
  `outvalue` varchar(50) NOT NULL,
  `IOA` varchar(50) NOT NULL,
  `information` varchar(250) NOT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`Status`,`outvalue`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED COMMENT='control kontaktor dan status eventlist pada hmi';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_battery
CREATE TABLE IF NOT EXISTS `in_battery` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `battery` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=480 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT COMMENT='status kontaktor battery';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_battery_bank
CREATE TABLE IF NOT EXISTS `in_battery_bank` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamps` varchar(50) DEFAULT NULL,
  `Volt` varchar(50) DEFAULT '0',
  `Temp` varchar(50) DEFAULT '0',
  `FaultVolt` varchar(50) DEFAULT '0',
  `FaultTemp` varchar(50) DEFAULT '0',
  `Status` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`Volt`,`Temp`,`FaultVolt`,`FaultTemp`,`Status`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='list informasi data dari battery';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_battery_dod
CREATE TABLE IF NOT EXISTS `in_battery_dod` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `MinDod` varchar(50) DEFAULT '0',
  `MaxDod` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`MinDod`,`MaxDod`)
) ENGINE=MyISAM AUTO_INCREMENT=16 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='status dod setpoint';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_battery_soc
CREATE TABLE IF NOT EXISTS `in_battery_soc` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `MinSoc` varchar(50) DEFAULT '0',
  `MaxSoc` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`MaxSoc`,`MinSoc`)
) ENGINE=MyISAM AUTO_INCREMENT=16 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='status soc setpoint';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_battery_status
CREATE TABLE IF NOT EXISTS `in_battery_status` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamps` varchar(50) DEFAULT NULL,
  `FaultVSystem` varchar(50) DEFAULT NULL,
  `FaultCurSystem` varchar(50) DEFAULT NULL,
  `SOC` varchar(50) DEFAULT '0',
  `DOD` varchar(50) DEFAULT '0',
  `V_TOT` varchar(50) DEFAULT '0',
  `VHVDC` varchar(50) DEFAULT '0',
  `ITOT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`SOC`,`DOD`,`V_TOT`,`VHVDC`,`ITOT`,`FaultVSystem`,`FaultCurSystem`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='monitoring data battery';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_battery
CREATE TABLE IF NOT EXISTS `in_cb_battery` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `aBattery` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=459 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='cb alarm battery';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_battery_com
CREATE TABLE IF NOT EXISTS `in_cb_battery_com` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5212 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='cb comunikasi battery';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_battery_spv
CREATE TABLE IF NOT EXISTS `in_cb_battery_spv` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=254 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='cb komunikasi spv battery';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter
CREATE TABLE IF NOT EXISTS `in_cb_inverter` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `aInverter` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='status inverter dari kontaktor';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_ac1
CREATE TABLE IF NOT EXISTS `in_cb_inverter_ac1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='status inverter ac dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_ac2
CREATE TABLE IF NOT EXISTS `in_cb_inverter_ac2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='status inverter ac dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_ac3
CREATE TABLE IF NOT EXISTS `in_cb_inverter_ac3` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='status inverter ac dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_fault1
CREATE TABLE IF NOT EXISTS `in_cb_inverter_fault1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=17 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_fault2
CREATE TABLE IF NOT EXISTS `in_cb_inverter_fault2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_fault3
CREATE TABLE IF NOT EXISTS `in_cb_inverter_fault3` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_fault4
CREATE TABLE IF NOT EXISTS `in_cb_inverter_fault4` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_fault5
CREATE TABLE IF NOT EXISTS `in_cb_inverter_fault5` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_fault6
CREATE TABLE IF NOT EXISTS `in_cb_inverter_fault6` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_spv1
CREATE TABLE IF NOT EXISTS `in_cb_inverter_spv1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1931 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_spv2
CREATE TABLE IF NOT EXISTS `in_cb_inverter_spv2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=341 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_spv3
CREATE TABLE IF NOT EXISTS `in_cb_inverter_spv3` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=235 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_spv4
CREATE TABLE IF NOT EXISTS `in_cb_inverter_spv4` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=279 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_spv5
CREATE TABLE IF NOT EXISTS `in_cb_inverter_spv5` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=219 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_spv6
CREATE TABLE IF NOT EXISTS `in_cb_inverter_spv6` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=235 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_state_r1
CREATE TABLE IF NOT EXISTS `in_cb_inverter_state_r1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=63 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_state_r2
CREATE TABLE IF NOT EXISTS `in_cb_inverter_state_r2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=63 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_state_s1
CREATE TABLE IF NOT EXISTS `in_cb_inverter_state_s1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=67 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_state_s2
CREATE TABLE IF NOT EXISTS `in_cb_inverter_state_s2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=58 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_state_t1
CREATE TABLE IF NOT EXISTS `in_cb_inverter_state_t1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=73 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_inverter_state_t2
CREATE TABLE IF NOT EXISTS `in_cb_inverter_state_t2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=54 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='spv dari rabbit';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_load
CREATE TABLE IF NOT EXISTS `in_cb_load` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `aLoadA` varchar(255) DEFAULT '0',
  `aLoadB` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_pltmh
CREATE TABLE IF NOT EXISTS `in_cb_pltmh` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `aPltmh` varchar(255) DEFAULT '0',
  `aPltmh_b` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=46 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_plts
CREATE TABLE IF NOT EXISTS `in_cb_plts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `aPlts` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=35 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_plts_fault1
CREATE TABLE IF NOT EXISTS `in_cb_plts_fault1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=17 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_plts_fault2
CREATE TABLE IF NOT EXISTS `in_cb_plts_fault2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_plts_fault3
CREATE TABLE IF NOT EXISTS `in_cb_plts_fault3` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_plts_spv1
CREATE TABLE IF NOT EXISTS `in_cb_plts_spv1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=461 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_plts_spv2
CREATE TABLE IF NOT EXISTS `in_cb_plts_spv2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=266 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_plts_spv3
CREATE TABLE IF NOT EXISTS `in_cb_plts_spv3` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=297 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_rtu_inverter
CREATE TABLE IF NOT EXISTS `in_cb_rtu_inverter` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `value` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=134 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_rtu_load
CREATE TABLE IF NOT EXISTS `in_cb_rtu_load` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `value` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=512 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_rtu_pltmh
CREATE TABLE IF NOT EXISTS `in_cb_rtu_pltmh` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `value` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=33145 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_cb_rtu_plts
CREATE TABLE IF NOT EXISTS `in_cb_rtu_plts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `value` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=289 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter
CREATE TABLE IF NOT EXISTS `in_inverter` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdimon` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=76 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_1
CREATE TABLE IF NOT EXISTS `in_inverter_1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdi1mon` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_2
CREATE TABLE IF NOT EXISTS `in_inverter_2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdi2mon` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_3
CREATE TABLE IF NOT EXISTS `in_inverter_3` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdi3mon` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_cas
CREATE TABLE IF NOT EXISTS `in_inverter_cas` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdimoncas` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_cas_1
CREATE TABLE IF NOT EXISTS `in_inverter_cas_1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdi1cas` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_cas_2
CREATE TABLE IF NOT EXISTS `in_inverter_cas_2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdi2cas` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_cas_3
CREATE TABLE IF NOT EXISTS `in_inverter_cas_3` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdi3cas` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_curve_p
CREATE TABLE IF NOT EXISTS `in_inverter_curve_p` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_curve_q
CREATE TABLE IF NOT EXISTS `in_inverter_curve_q` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_setpoint_c1
CREATE TABLE IF NOT EXISTS `in_inverter_setpoint_c1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_setpoint_c2
CREATE TABLE IF NOT EXISTS `in_inverter_setpoint_c2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_setpoint_m1
CREATE TABLE IF NOT EXISTS `in_inverter_setpoint_m1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_setpoint_m2
CREATE TABLE IF NOT EXISTS `in_inverter_setpoint_m2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_setpoint_mode
CREATE TABLE IF NOT EXISTS `in_inverter_setpoint_mode` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` double DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1558 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_setpoint_var
CREATE TABLE IF NOT EXISTS `in_inverter_setpoint_var` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` double DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=177 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_setpoint_w
CREATE TABLE IF NOT EXISTS `in_inverter_setpoint_w` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` double DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=542 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch
CREATE TABLE IF NOT EXISTS `in_inverter_switch` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdimonsw` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=15 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_1
CREATE TABLE IF NOT EXISTS `in_inverter_switch_1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdi1sw` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=71 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_10
CREATE TABLE IF NOT EXISTS `in_inverter_switch_10` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=34 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_11
CREATE TABLE IF NOT EXISTS `in_inverter_switch_11` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=54 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_12
CREATE TABLE IF NOT EXISTS `in_inverter_switch_12` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=38 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_2
CREATE TABLE IF NOT EXISTS `in_inverter_switch_2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdi2sw` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=90 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_3
CREATE TABLE IF NOT EXISTS `in_inverter_switch_3` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `bdi3sw` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=73 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_4
CREATE TABLE IF NOT EXISTS `in_inverter_switch_4` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=88 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_5
CREATE TABLE IF NOT EXISTS `in_inverter_switch_5` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=111 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_6
CREATE TABLE IF NOT EXISTS `in_inverter_switch_6` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=104 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_7
CREATE TABLE IF NOT EXISTS `in_inverter_switch_7` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=66 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_8
CREATE TABLE IF NOT EXISTS `in_inverter_switch_8` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=40 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_inverter_switch_9
CREATE TABLE IF NOT EXISTS `in_inverter_switch_9` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=58 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_load
CREATE TABLE IF NOT EXISTS `in_load` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `loadpage` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_login
CREATE TABLE IF NOT EXISTS `in_login` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(50) DEFAULT NULL,
  `password` varchar(50) DEFAULT NULL,
  `fullname` varchar(200) DEFAULT NULL,
  `authentication` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`username`,`password`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_log_protocol
CREATE TABLE IF NOT EXISTS `in_log_protocol` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `Status` varchar(50) NOT NULL,
  `information` varchar(6553) NOT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`Status`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC COMMENT='log error protocol';

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_pltmh
CREATE TABLE IF NOT EXISTS `in_pltmh` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `pltmh` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=280 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_plts
CREATE TABLE IF NOT EXISTS `in_plts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `gtimon` varchar(255) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=81 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_plts_control1
CREATE TABLE IF NOT EXISTS `in_plts_control1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `gti1` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_plts_control2
CREATE TABLE IF NOT EXISTS `in_plts_control2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `gti2` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_plts_control3
CREATE TABLE IF NOT EXISTS `in_plts_control3` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(255) DEFAULT NULL,
  `gti3` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_battery
CREATE TABLE IF NOT EXISTS `in_power_battery` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `BatteryW` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`BatteryW`)
) ENGINE=MyISAM AUTO_INCREMENT=119647 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_inverter
CREATE TABLE IF NOT EXISTS `in_power_inverter` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `InvW` varchar(50) NOT NULL DEFAULT '0',
  `InvVar` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`InvVar`,`InvW`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_inverter_curve
CREATE TABLE IF NOT EXISTS `in_power_inverter_curve` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `InvW` varchar(50) NOT NULL DEFAULT '0',
  `InvVar` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`InvVar`,`InvW`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_inverter_r
CREATE TABLE IF NOT EXISTS `in_power_inverter_r` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `invW` varchar(50) DEFAULT NULL,
  `invVar` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`invVar`,`invW`)
) ENGINE=MyISAM AUTO_INCREMENT=2893 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_inverter_r1
CREATE TABLE IF NOT EXISTS `in_power_inverter_r1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `invW` varchar(50) DEFAULT NULL,
  `invVar` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`invVar`,`invW`)
) ENGINE=MyISAM AUTO_INCREMENT=2740 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_inverter_s
CREATE TABLE IF NOT EXISTS `in_power_inverter_s` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `invW` varchar(50) DEFAULT NULL,
  `invVar` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`invW`,`invVar`)
) ENGINE=MyISAM AUTO_INCREMENT=2571 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_inverter_s1
CREATE TABLE IF NOT EXISTS `in_power_inverter_s1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `invW` varchar(50) DEFAULT NULL,
  `invVar` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`invVar`,`invW`)
) ENGINE=MyISAM AUTO_INCREMENT=2538 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_inverter_t
CREATE TABLE IF NOT EXISTS `in_power_inverter_t` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `invW` varchar(50) DEFAULT NULL,
  `invVar` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`invW`,`invVar`)
) ENGINE=MyISAM AUTO_INCREMENT=2336 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_inverter_t1
CREATE TABLE IF NOT EXISTS `in_power_inverter_t1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `invW` varchar(50) DEFAULT NULL,
  `invVar` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`invW`,`invVar`)
) ENGINE=MyISAM AUTO_INCREMENT=1729 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_load
CREATE TABLE IF NOT EXISTS `in_power_load` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `LoadW` varchar(50) DEFAULT '0',
  `LoadVar` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`LoadW`,`LoadVar`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_pltmh
CREATE TABLE IF NOT EXISTS `in_power_pltmh` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `PltmhW` varchar(50) DEFAULT '0',
  `PltmhVar` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`PltmhW`,`PltmhVar`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_plts
CREATE TABLE IF NOT EXISTS `in_power_plts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `PltsW` varchar(50) DEFAULT '0',
  `PltsVar` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`PltsW`,`PltsVar`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_plts_block_r
CREATE TABLE IF NOT EXISTS `in_power_plts_block_r` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `W` varchar(50) DEFAULT NULL,
  `V` varchar(50) DEFAULT NULL,
  `A` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`V`,`W`,`A`)
) ENGINE=MyISAM AUTO_INCREMENT=7120 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_plts_block_s
CREATE TABLE IF NOT EXISTS `in_power_plts_block_s` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `W` varchar(50) DEFAULT NULL,
  `V` varchar(50) DEFAULT NULL,
  `A` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`V`,`W`,`A`)
) ENGINE=MyISAM AUTO_INCREMENT=7454 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.in_power_plts_block_t
CREATE TABLE IF NOT EXISTS `in_power_plts_block_t` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `timeStamps` varchar(50) DEFAULT NULL,
  `W` varchar(50) DEFAULT NULL,
  `V` varchar(50) DEFAULT NULL,
  `A` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`V`,`W`,`A`)
) ENGINE=MyISAM AUTO_INCREMENT=6710 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_active_inverter
CREATE TABLE IF NOT EXISTS `mon_active_inverter` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `ActiveR` varchar(50) DEFAULT NULL,
  `ActiveS` varchar(50) DEFAULT NULL,
  `ActiveT` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`ActiveR`,`ActiveS`,`ActiveT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_active_load
CREATE TABLE IF NOT EXISTS `mon_active_load` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `ActiveR` varchar(50) DEFAULT NULL,
  `ActiveS` varchar(50) DEFAULT NULL,
  `ActiveT` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`ActiveR`,`ActiveS`,`ActiveT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_active_pltmh
CREATE TABLE IF NOT EXISTS `mon_active_pltmh` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `ActiveR` varchar(50) DEFAULT NULL,
  `ActiveS` varchar(50) DEFAULT NULL,
  `ActiveT` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`ActiveR`,`ActiveS`,`ActiveT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_active_plts
CREATE TABLE IF NOT EXISTS `mon_active_plts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `ActiveR` varchar(50) DEFAULT NULL,
  `ActiveS` varchar(50) DEFAULT NULL,
  `ActiveT` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`ActiveR`,`ActiveS`,`ActiveT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_battery
CREATE TABLE IF NOT EXISTS `mon_battery` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `Timestamps` varchar(50) DEFAULT NULL,
  `BatteryV` varchar(50) DEFAULT '0',
  `BatteryA` varchar(50) DEFAULT '0',
  `Radiation` varchar(50) DEFAULT '0',
  `Temp` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_cosphi_inverter
CREATE TABLE IF NOT EXISTS `mon_cosphi_inverter` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `CosPhiR` varchar(50) DEFAULT '0',
  `CosPhiS` varchar(50) DEFAULT '0',
  `CosPhiT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`CosPhiR`,`CosPhiS`,`CosPhiT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_cosphi_load
CREATE TABLE IF NOT EXISTS `mon_cosphi_load` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `CosPhiR` varchar(50) DEFAULT '0',
  `CosPhiS` varchar(50) DEFAULT '0',
  `CosPhiT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`CosPhiR`,`CosPhiS`,`CosPhiT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_cosphi_pltmh
CREATE TABLE IF NOT EXISTS `mon_cosphi_pltmh` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `CosPhiR` varchar(50) DEFAULT '0',
  `CosPhiS` varchar(50) DEFAULT '0',
  `CosPhiT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`CosPhiT`,`CosPhiR`,`CosPhiS`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_cosphi_plts
CREATE TABLE IF NOT EXISTS `mon_cosphi_plts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `CosPhiR` varchar(50) DEFAULT '0',
  `CosPhiS` varchar(50) DEFAULT '0',
  `CosPhiT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`CosPhiR`,`CosPhiS`,`CosPhiT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_inverter
CREATE TABLE IF NOT EXISTS `mon_inverter` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `invVR` varchar(50) DEFAULT '0',
  `invVS` varchar(50) DEFAULT '0',
  `invVT` varchar(50) DEFAULT '0',
  `invAR` varchar(50) DEFAULT '0',
  `invAS` varchar(50) DEFAULT '0',
  `invAT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`invVS`,`invVR`,`invVT`,`invAR`,`invAS`,`invAT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_inverter_freq
CREATE TABLE IF NOT EXISTS `mon_inverter_freq` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `value` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`value`)
) ENGINE=MyISAM AUTO_INCREMENT=22476 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_load
CREATE TABLE IF NOT EXISTS `mon_load` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `loadVR` varchar(50) DEFAULT '0',
  `loadVS` varchar(50) DEFAULT '0',
  `loadVT` varchar(50) DEFAULT '0',
  `loadAR` varchar(50) DEFAULT '0',
  `loadAS` varchar(50) DEFAULT '0',
  `loadAT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`loadVR`,`loadVS`,`loadVT`,`loadAR`,`loadAS`,`loadAT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_pltmh
CREATE TABLE IF NOT EXISTS `mon_pltmh` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `pltmhVR` varchar(50) DEFAULT '0',
  `pltmhVS` varchar(50) DEFAULT '0',
  `pltmhVT` varchar(50) DEFAULT '0',
  `pltmhAR` varchar(50) DEFAULT '0',
  `pltmhAS` varchar(50) DEFAULT '0',
  `pltmhAT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`pltmhVR`,`pltmhVS`,`pltmhVT`,`pltmhAR`,`pltmhAS`,`pltmhAT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_plts
CREATE TABLE IF NOT EXISTS `mon_plts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `pltsVR` varchar(50) DEFAULT '0',
  `pltsVS` varchar(50) DEFAULT '0',
  `pltsVT` varchar(50) DEFAULT '0',
  `pltsAR` varchar(50) DEFAULT '0',
  `pltsAS` varchar(50) DEFAULT '0',
  `pltsAT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`pltsVR`,`pltsVS`,`pltsVT`,`pltsAR`,`pltsAS`,`pltsAT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_plts_freq
CREATE TABLE IF NOT EXISTS `mon_plts_freq` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `pltsfreq` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`pltsfreq`)
) ENGINE=MyISAM AUTO_INCREMENT=78987 DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_radiation
CREATE TABLE IF NOT EXISTS `mon_radiation` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `radiation` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`radiation`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_reactive_inverter
CREATE TABLE IF NOT EXISTS `mon_reactive_inverter` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `ReactiveR` varchar(50) DEFAULT '0',
  `ReactiveS` varchar(50) DEFAULT '0',
  `ReactiveT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`ReactiveR`,`ReactiveS`,`ReactiveT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_reactive_load
CREATE TABLE IF NOT EXISTS `mon_reactive_load` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `ReactiveR` varchar(50) DEFAULT '0',
  `ReactiveS` varchar(50) DEFAULT '0',
  `ReactiveT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`ReactiveR`,`ReactiveS`,`ReactiveT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_reactive_pltmh
CREATE TABLE IF NOT EXISTS `mon_reactive_pltmh` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `ReactiveR` varchar(50) DEFAULT '0',
  `ReactiveS` varchar(50) DEFAULT '0',
  `ReactiveT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`ReactiveR`,`ReactiveS`,`ReactiveT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table db_scada.mon_reactive_plts
CREATE TABLE IF NOT EXISTS `mon_reactive_plts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `timestamps` varchar(50) DEFAULT NULL,
  `ReactiveR` varchar(50) DEFAULT '0',
  `ReactiveS` varchar(50) DEFAULT '0',
  `ReactiveT` varchar(50) DEFAULT '0',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `Index Search` (`ReactiveR`,`ReactiveS`,`ReactiveT`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
