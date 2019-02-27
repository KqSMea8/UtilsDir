#pragma once

#define _ODBC_							_T("MySQL")
#define _DB_							_T("zet7xxx_storage")
#define _USER_							_T("zet7xxx_store")
#define _PASS_							_T("Zet12345")

#define _CREATE_DEVICE_TABLE_			_T("CREATE TABLE IF NOT EXISTS `%s`")\
										_T("(")\
										_T("`#` INT UNSIGNED NOT NULL AUTO_INCREMENT,")\
										_T("`type` INT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`serial` BIGINT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`name` VARCHAR(128) NOT NULL,")\
										_T("`add_date_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,")\
										_T("`remove_date_time` TIMESTAMP,")\
										_T("`master_type` INT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`master_serial` BIGINT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`master_name` VARCHAR(128) NOT NULL,")\
										_T("`node` INT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`baud_rate` INT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`parity` INT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`max_level` FLOAT DEFAULT '0',")\
										_T("`interface` INT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`status` INT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`hardware` INT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`firmware` INT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`pc_ip` VARCHAR(16) NOT NULL,")\
										_T("`pc_name` VARCHAR(128) NOT NULL,")\
										_T("PRIMARY KEY (`#`, `serial`),")\
										_T("UNIQUE INDEX `#` (`#`),")\
										_T("UNIQUE INDEX `serial` (`serial`)")\
										_T(")")\
										_T("COLLATE='utf8_general_ci'")\
										_T("ENGINE = InnoDB")

#define _GET_DEVICE_INFO_				_T("SELECT * FROM `%s` WHERE `type` = %u AND `serial` = %u")
#define _GET_DEVICE_INFO_EX_			_T("SELECT * FROM `%s` WHERE `serial` = %u")

#define _GET_ACTIVE_MASTERS_			_T("SELECT * FROM `%s` WHERE (`type` = 6 OR `type` = 8 OR `type` = 25 OR `type` = 46) ")\
										_T("AND `pc_ip` = \"%s\" AND `status` = 1")
#define _GET_INACTIVE_MASTERS_			_T("SELECT * FROM `%s` WHERE (`type` = 6 OR `type` = 8 OR `type` = 25 OR `type` = 46) ")\
										_T("AND `pc_ip` = \"%s\" AND `status` = 0")

#define _GET_ACTIVE_DEVICES_			_T("SELECT * FROM `%s` WHERE `master_type` = %u AND `master_serial` = %u ")\
										_T("AND `status` = 1")

#define _CREATE_DEVICE_					_T("INSERT INTO `%s` ")\
										_T("(")\
										_T("`type`, `serial`, `name`, `master_type`, `master_serial`, ")\
										_T("`master_name`, `node`, `baud_rate`, `parity`, `max_level`, `interface`, ")\
										_T("`status`, `hardware`, `firmware`, `pc_ip`, `pc_name`")\
										_T(") ")\
										_T("VALUES(")\
										_T("%d, %u, \"%s\", %d, %u, ")\
										_T("\"%s\", %u, %u, %u, %f, %u, ")\
										_T("1, %u, %u, \"%s\", \"%s\"")\
										_T(")")

#define _REMOVE_DEVICE_					_T("UPDATE `%s` SET `status`=0, `remove_date_time`=CURRENT_TIMESTAMP ")\
										_T("WHERE `type`=%u AND `serial`=%u")

#define _UPDATE_DEVICE_TYPE_			_T("UPDATE `%s` SET ")\
										_T("`type`=%u ")\
										_T("WHERE `serial`=%u")

#define _UPDATE_DEVICE_MASTER_			_T("UPDATE `%s` SET ")\
										_T("`master_type`=%u, `master_serial`=%u, `master_name`=\"%s\" ")\
										_T("WHERE `type`=%u AND `serial`=%u")

#define _UPDATE_DEVICE_NODE_			_T("UPDATE `%s` SET ")\
										_T("`node`=%u ")\
										_T("WHERE `type`=%u AND `serial`=%u")

#define _UPDATE_DEVICE_IO_PARAMETERS_	_T("UPDATE `%s` SET ")\
										_T("`baud_rate`=%u, `parity`=%u ")\
										_T("WHERE `type`=%u AND `serial`=%u")

#define _UPDATE_DEVICE_PARAMETERS_		_T("UPDATE `%s` SET ")\
										_T("`max_level`=%f ")\
										_T("WHERE `type`=%u AND `serial`=%u")

#define _UPDATE_DEVICE_FIRMWARE_		_T("UPDATE `%s` SET ")\
										_T("`hardware`=%u, `firmware`=%u ")\
										_T("WHERE `type`=%u AND `serial`=%u")

#define _UPDATE_DEVICE_PC_				_T("UPDATE `%s` SET ")\
										_T("`pc_ip`=\"%s\", `pc_name`=\"%s\" ")\
										_T("WHERE `type`=%u AND `serial`=%u")

#define _UPDATE_DEVICE_STATUS_			_T("UPDATE `%s` SET ")\
										_T("`status`=%d ")\
										_T("WHERE `type`=%u AND `serial`=%u")

#define _CREATE_TYPE_TABLE_				_T("CREATE TABLE IF NOT EXISTS `%s`")\
										_T("(")\
										_T("`type` INT UNSIGNED NOT NULL DEFAULT '0',")\
										_T("`type_name` VARCHAR(16) NOT NULL,")\
										_T("PRIMARY KEY (`type`)")\
										_T(")")\
										_T("COLLATE='utf8_general_ci'")\
										_T("ENGINE = InnoDB")

#define _CREATE_TYPE_					_T("INSERT INTO `%s` ")\
										_T("(")\
										_T("`type`, `type_name`")\
										_T(") ")\
										_T("VALUES(")\
										_T("%d, \"%s\"")\
										_T(") ")\
										_T("ON DUPLICATE KEY UPDATE `type_name` = `type_name`")


#define _GET_LAST_ID_					_T("SELECT MAX(`id`) ")\
										_T("FROM %s")

//////////////////////////////////////////////////////////////////////////////
//							SensorTracking									//
//////////////////////////////////////////////////////////////////////////////
#define _GET_COUNT_DEVICES_WITH_TYPE_	_T("SELECT COUNT(type) ")\
										_T("FROM `zet7xxx_common` ")\
										_T("WHERE `type` = %s and status = 1 and (pc_ip = \"10.0.170.2\" or pc_ip = \"10.0.170.10\" or pc_ip = \"192.168.0.207\" or pc_ip = \"192.168.1.1\") ")\
										_T("ORDER BY type")

#define _GET_COUNT_DEVICES_WITH_TYPE_AND_MAX_LEVEL_	_T("SELECT COUNT(max_level) ")\
										_T("FROM `zet7xxx_common` ")\
										_T("WHERE `type` = %s and max_level >= %f and max_level <= %f and status = 1 and (pc_ip = \"10.0.170.2\" or pc_ip = \"10.0.170.10\" or pc_ip = \"192.168.0.207\" or pc_ip = \"192.168.1.1\") ")\
										_T("ORDER BY type")

#define _GET_MAX_LEVEL_WITH_TYPE_		_T("SELECT max_level, COUNT(max_level) ")\
										_T("FROM `zet7xxx_common` ")\
										_T("WHERE `type` = %s and hardware = %s and status = 1 and (pc_ip = \"10.0.170.2\" or pc_ip = \"10.0.170.10\" or pc_ip = \"192.168.0.207\" or pc_ip = \"192.168.1.1\") ")\
										_T("GROUP BY max_level ")\
										_T("ORDER BY max_level")

#define _GET_DEVICE_HARDWARE_			_T("SELECT hardware, COUNT(hardware) ")\
										_T("FROM `zet7xxx_common` ")\
										_T("where `type` = %s and status = 1 and (pc_ip = \"10.0.170.2\" or pc_ip = \"10.0.170.10\" or pc_ip = \"192.168.0.207\" or pc_ip = \"192.168.1.1\") ")\
										_T("GROUP BY hardware ")\
										_T("ORDER BY hardware")

#define _GET_DEVICE_HARDWARE_WITH_MAX_LEVEL_		_T("SELECT hardware, COUNT(hardware) ")\
										_T("FROM `zet7xxx_common` ")\
										_T("where `type` = %s and max_level >= %f and max_level <= %f and status = 1 and (pc_ip = \"10.0.170.2\" or pc_ip = \"10.0.170.10\" or pc_ip = \"192.168.0.207\" or pc_ip = \"192.168.1.1\") ")\
										_T("GROUP BY hardware ")\
										_T("ORDER BY hardware")

#define _SET_TYPE_PRIORITY_				_T("UPDATE `device_types` ")\
										_T("SET `sorting_priority` = %d ")\
										_T("WHERE `type` = %d")

#define _SET_TYPE_MIN_BALANCE_			_T("UPDATE `device_types` ")\
										_T("SET `minimum_balance` = %s ")\
										_T("WHERE `type` = %d")

#define _SET_TYPE_MIN_BALANCE_PRESSURE_ _T("UPDATE `%s_types` ")\
										_T("SET `minimum_balance` = %s ")\
										_T("WHERE `max_level` >= %f and `max_level` <= %f")

#define _SET_TYPE_INTERNAL_USE_			_T("UPDATE `device_types` ")\
										_T("SET `internal_use` = %s ")\
										_T("WHERE `type` = %d")

#define _SET_TYPE_INTERNAL_USE_PRESSURE_ _T("UPDATE `%s_types` ")\
										 _T("SET `internal_use` = %s ")\
										 _T("WHERE `max_level` >= %f and `max_level` <= %f")

#define _GET_TYPE_NAME_					_T("SELECT type_name ")\
										_T("FROM `device_types` ")\
										_T("WHERE `type` = %d")

#define _GET_DEVICE_TYPES_TABLE			_T("SELECT * ")\
										_T("FROM `%s_types` ")\
										_T("ORDER BY freshness")

#define _GET_REPRESENTATIVE 			_T("SELECT * ")\
										_T("FROM `employee` ")\
										_T("WHERE `id` = \"%s\"")
#define _GET_ALL_REPRESENTATIVE			_T("SELECT * ")\
										_T("FROM `employee` ")\
										_T("WHERE `client` = 0")