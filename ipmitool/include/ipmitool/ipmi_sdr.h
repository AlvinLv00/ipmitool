/*
 * Copyright (c) 2003 Sun Microsystems, Inc.  All Rights Reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * Redistribution of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * Redistribution in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of Sun Microsystems, Inc. or the names of
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * 
 * This software is provided "AS IS," without a warranty of any kind.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED.
 * SUN MICROSYSTEMS, INC. ("SUN") AND ITS LICENSORS SHALL NOT BE LIABLE
 * FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.  IN NO EVENT WILL
 * SUN OR ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA,
 * OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR
 * PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF
 * LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE,
 * EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * 
 * You acknowledge that this software is not designed or intended for use
 * in the design, construction, operation or maintenance of any nuclear
 * facility.
 */

#ifndef IPMI_SDR_H
#define IPMI_SDR_H

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <inttypes.h>
#include <math.h>
#include <ipmitool/bswap.h>
#include <ipmitool/ipmi.h>
#include <ipmitool/ipmi_entity.h>

int ipmi_sdr_main(struct ipmi_intf *, int, char **);
int32_t utos(uint32_t val, int bits);

#if WORDS_BIGENDIAN
# define __TO_TOL(mtol)     (uint16_t)(mtol & 0x3f)
# define __TO_M(mtol)       (int16_t)(utos((((mtol & 0xff00) >> 8) | ((mtol & 0xc0) << 2)), 10))
# define __TO_B(bacc)       (int32_t)(utos((((bacc & 0xff000000) >> 24) | ((bacc & 0xc00000) >> 14)), 10))
# define __TO_ACC(bacc)     (uint32_t)(((bacc & 0x3f0000) >> 16) | ((bacc & 0xf000) >> 6))
# define __TO_ACC_EXP(bacc) (uint32_t)((bacc & 0xc00) >> 10)
# define __TO_R_EXP(bacc)   (int32_t)(utos(((bacc & 0xf0) >> 4), 4))
# define __TO_B_EXP(bacc)   (int32_t)(utos((bacc & 0xf), 4))
#else
# define __TO_TOL(mtol)     (uint16_t)(BSWAP_16(mtol) & 0x3f)
# define __TO_M(mtol)       (int16_t)(utos((((BSWAP_16(mtol) & 0xff00) >> 8) | ((BSWAP_16(mtol) & 0xc0) << 2)), 10))
# define __TO_B(bacc)       (int32_t)(utos((((BSWAP_32(bacc) & 0xff000000) >> 24) | \
                            ((BSWAP_32(bacc) & 0xc00000) >> 14)), 10))
# define __TO_ACC(bacc)     (uint32_t)(((BSWAP_32(bacc) & 0x3f0000) >> 16) | ((BSWAP_32(bacc) & 0xf000) >> 6))
# define __TO_ACC_EXP(bacc) (uint32_t)((BSWAP_32(bacc) & 0xc00) >> 10)
# define __TO_R_EXP(bacc)   (int32_t)(utos(((BSWAP_32(bacc) & 0xf0) >> 4), 4))
# define __TO_B_EXP(bacc)   (int32_t)(utos((BSWAP_32(bacc) & 0xf), 4))
#endif

#define GET_SDR_REPO_INFO	0x20
#define GET_SDR_ALLOC_INFO	0x21

#define SDR_SENSOR_STAT_LO_NC	(1<<0)
#define SDR_SENSOR_STAT_LO_CR	(1<<1)
#define SDR_SENSOR_STAT_LO_NR	(1<<2)
#define SDR_SENSOR_STAT_HI_NC	(1<<3)
#define SDR_SENSOR_STAT_HI_CR	(1<<4)
#define SDR_SENSOR_STAT_HI_NR	(1<<5)

struct sdr_repo_info_rs {
	uint8_t	version;	/* SDR version (51h) */
	uint16_t	count;		/* number of records */
	uint16_t	free;		/* free space in SDR */
	uint32_t	add_stamp;	/* last add timestamp */
	uint32_t	erase_stamp;	/* last del timestamp */
	uint8_t	op_support;	/* supported operations */
} __attribute__ ((packed));

#define GET_SDR_RESERVE_REPO	0x22
struct sdr_reserve_repo_rs {
	uint16_t	reserve_id;	/* reservation ID */
} __attribute__ ((packed));

#define GET_SDR		0x23
struct sdr_get_rq {
	uint16_t	reserve_id;	/* reservation ID */
	uint16_t	id;		/* record ID */
	uint8_t	offset;		/* offset into SDR */
#define GET_SDR_ENTIRE_RECORD	0xff
	uint8_t	length;		/* length to read */
} __attribute__ ((packed));

struct sdr_get_rs {
	uint16_t	next;		/* next record id */
	uint16_t	id;		/* record ID */
	uint8_t	version;	/* SDR version (51h) */
#define SDR_RECORD_TYPE_FULL_SENSOR		0x01
#define SDR_RECORD_TYPE_COMPACT_SENSOR		0x02
#define SDR_RECORD_TYPE_EVENTONLY_SENSOR	0x03
#define SDR_RECORD_TYPE_ENTITY_ASSOC		0x08
#define SDR_RECORD_TYPE_DEVICE_ENTITY_ASSOC	0x09
#define SDR_RECORD_TYPE_GENERIC_DEVICE_LOCATOR	0x10
#define SDR_RECORD_TYPE_FRU_DEVICE_LOCATOR	0x11
#define SDR_RECORD_TYPE_MC_DEVICE_LOCATOR	0x12
#define SDR_RECORD_TYPE_MC_CONFIRMATION		0x13
#define SDR_RECORD_TYPE_BMC_MSG_CHANNEL_INFO	0x14
#define SDR_RECORD_TYPE_OEM			0xc0
	uint8_t	type;		/* record type */
	uint8_t	length;		/* remaining record bytes */
} __attribute__ ((packed));

struct sdr_record_compact_sensor {
	struct {
		uint8_t	owner_id;
#if WORDS_BIGENDIAN
		uint8_t	channel     : 4;	/* channel number */
		uint8_t	__reserved  : 2;
		uint8_t	lun         : 2;	/* sensor owner lun */
#else
		uint8_t	lun         : 2;	/* sensor owner lun */
		uint8_t	__reserved  : 2;
		uint8_t	channel     : 4;	/* channel number */
#endif
		uint8_t	sensor_num;		/* unique sensor number */
	} keys;

	struct entity_id entity;

	struct {
		struct {
#if WORDS_BIGENDIAN
			uint8_t	__reserved   : 1;
			uint8_t	scanning     : 1;
			uint8_t	events       : 1;
			uint8_t	thresholds   : 1;
			uint8_t	hysteresis   : 1;
			uint8_t	type         : 1;
			uint8_t	event_gen    : 1;
			uint8_t	sensor_scan  : 1;
#else
			uint8_t	sensor_scan  : 1;
			uint8_t	event_gen    : 1;
			uint8_t	type         : 1;
			uint8_t	hysteresis   : 1;
			uint8_t	thresholds   : 1;
			uint8_t	events       : 1;
			uint8_t	scanning     : 1;
			uint8_t	__reserved   : 1;
#endif
		} init;
		struct {
#if WORDS_BIGENDIAN
			uint8_t	ignore       : 1;
			uint8_t	rearm        : 1;
			uint8_t	hysteresis   : 2;
			uint8_t	threshold    : 2;
			uint8_t	event_msg    : 2;
#else
			uint8_t	event_msg    : 2;
			uint8_t	threshold    : 2;
			uint8_t	hysteresis   : 2;
			uint8_t	rearm        : 1;
			uint8_t	ignore       : 1;
#endif
		} capabilities;
		uint8_t	type; /* sensor type */
	} sensor;

	uint8_t	event_type; /* event/reading type code */

	union {
		struct {
			uint16_t	assert_event;	/* assertion event mask */
			uint16_t	deassert_event;	/* de-assertion event mask */
			uint16_t	read;		/* discrete reaading mask */
		} discrete;
		struct {
			uint16_t	lower;		/* lower threshold reading mask */
			uint16_t	upper;		/* upper threshold reading mask */
			uint8_t	set;		/* settable threshold mask */
			uint8_t	read;		/* readable threshold mask */
		} threshold;
	} mask;

	struct {
#if WORDS_BIGENDIAN
		uint8_t	analog        : 2;
		uint8_t	rate          : 3;
		uint8_t	modifier      : 2;
		uint8_t	pct           : 1;
#else
		uint8_t	pct           : 1;
		uint8_t	modifier      : 2;
		uint8_t	rate          : 3;
		uint8_t	analog        : 2;
#endif
		struct {
			uint8_t	base;
			uint8_t	modifier;
		} type;
	} unit;

	struct {
#if WORDS_BIGENDIAN
		uint8_t	__reserved  : 2;
		uint8_t	mod_type    : 2;
		uint8_t	count       : 4;
#else
		uint8_t	count       : 4;
		uint8_t	mod_type    : 2;
		uint8_t	__reserved  : 2;
#endif
#if WORDS_BIGENDIAN
		uint8_t	entity_inst : 1;
		uint8_t	mod_offset  : 7;
#else
		uint8_t	mod_offset  : 7;
		uint8_t	entity_inst : 1;
#endif
	} share;

	struct {
		struct {
			uint8_t	positive;
			uint8_t	negative;
		} hysteresis;
	} threshold;

	uint8_t	__reserved[3];
	uint8_t	oem;		/* reserved for OEM use */
	uint8_t	id_code;	/* sensor ID string type/length code */
	uint8_t	id_string[16];	/* sensor ID string bytes, only if id_code != 0 */
} __attribute__ ((packed));

struct sdr_record_eventonly_sensor {
	struct {
		uint8_t	owner_id;
#if WORDS_BIGENDIAN
		uint8_t	channel     : 4;	/* channel number */
		uint8_t	fru_owner   : 2;        /* fru device owner lun */
		uint8_t	lun         : 2;	/* sensor owner lun */
#else
		uint8_t	lun         : 2;	/* sensor owner lun */
		uint8_t	fru_owner   : 2;        /* fru device owner lun */
		uint8_t	channel     : 4;	/* channel number */
#endif
		uint8_t	sensor_num;		/* unique sensor number */
	} keys;

	struct entity_id entity;

	uint8_t	sensor_type;			/* sensor type */
	uint8_t	event_type;			/* event/reading type code */

	struct {
#if WORDS_BIGENDIAN
		uint8_t	__reserved  : 2;
		uint8_t	mod_type    : 2;
		uint8_t	count       : 4;
#else
		uint8_t	count       : 4;
		uint8_t	mod_type    : 2;
		uint8_t	__reserved  : 2;
#endif
#if WORDS_BIGENDIAN
		uint8_t	entity_inst : 1;
		uint8_t	mod_offset  : 7;
#else
		uint8_t	mod_offset  : 7;
		uint8_t	entity_inst : 1;
#endif
	} share;

	uint8_t	__reserved;
	uint8_t	oem;		/* reserved for OEM use */
	uint8_t	id_code;	/* sensor ID string type/length code */
	uint8_t	id_string[16];	/* sensor ID string bytes, only if id_code != 0 */

} __attribute__ ((packed));

struct sdr_record_full_sensor {
	struct {
		uint8_t	owner_id;
#if WORDS_BIGENDIAN
		uint8_t	channel     : 4;	/* channel number */
		uint8_t	__reserved  : 2;
		uint8_t	lun         : 2;	/* sensor owner lun */
#else
		uint8_t	lun         : 2;	/* sensor owner lun */
		uint8_t	__reserved  : 2;
		uint8_t	channel     : 4;	/* channel number */
#endif
		uint8_t	sensor_num;		/* unique sensor number */
	} keys;

	struct entity_id entity;

	struct {
		struct {
#if WORDS_BIGENDIAN
			uint8_t	 __reserved  : 1;
			uint8_t	scanning     : 1;
			uint8_t	events       : 1;
			uint8_t	thresholds   : 1;
			uint8_t	hysteresis   : 1;
			uint8_t	type         : 1;
			uint8_t	event_gen    : 1;
			uint8_t	sensor_scan  : 1;
#else
			uint8_t	sensor_scan  : 1;
			uint8_t	event_gen    : 1;
			uint8_t	type         : 1;
			uint8_t	hysteresis   : 1;
			uint8_t	thresholds   : 1;
			uint8_t	events       : 1;
			uint8_t	scanning     : 1;
			uint8_t	 __reserved  : 1;
#endif
		} init;
		struct {
#if WORDS_BIGENDIAN
			uint8_t	ignore       : 1;
			uint8_t	rearm        : 1;
			uint8_t	hysteresis   : 2;
			uint8_t	threshold    : 2;
			uint8_t	event_msg    : 2;
#else
			uint8_t	event_msg    : 2;
			uint8_t	threshold    : 2;
			uint8_t	hysteresis   : 2;
			uint8_t	rearm        : 1;
			uint8_t	ignore       : 1;
#endif
		} capabilities;
		uint8_t	type;
	} sensor;

	uint8_t	event_type;			/* event/reading type code */

	union {
		struct {
			uint16_t	assert_event;	/* assertion event mask */
			uint16_t	deassert_event;	/* de-assertion event mask */
			uint16_t	read;		/* discrete reaading mask */
		} discrete;
		struct {
			uint16_t	lower;		/* lower threshold reading mask */
			uint16_t	upper;		/* upper threshold reading mask */
			uint8_t	set;		/* settable threshold mask */
			uint8_t	read;		/* readable threshold mask */
		} threshold;
	} mask;

	struct {
#if WORDS_BIGENDIAN
		uint8_t	analog        : 2;
		uint8_t	rate          : 3;
		uint8_t	modifier      : 2;
		uint8_t	pct           : 1;
#else
		uint8_t	pct           : 1;
		uint8_t	modifier      : 2;
		uint8_t	rate          : 3;
		uint8_t	analog        : 2;
#endif
		struct {
			uint8_t	base;
			uint8_t	modifier;
		} type;
	} unit;

#define SDR_SENSOR_L_LINEAR     0x00
#define SDR_SENSOR_L_LN         0x01
#define SDR_SENSOR_L_LOG10      0x02
#define SDR_SENSOR_L_LOG2       0x03
#define SDR_SENSOR_L_E          0x04
#define SDR_SENSOR_L_EXP10      0x05
#define SDR_SENSOR_L_EXP2       0x06
#define SDR_SENSOR_L_1_X        0x07
#define SDR_SENSOR_L_SQR        0x08
#define SDR_SENSOR_L_CUBE       0x09
#define SDR_SENSOR_L_SQRT       0x0a
#define SDR_SENSOR_L_CUBERT     0x0b
#define SDR_SENSOR_L_NONLINEAR  0x70

	uint8_t	linearization;	/* 70h=non linear, 71h-7Fh=non linear, OEM */
	uint16_t	mtol;		/* M, tolerance */
	uint32_t	bacc;		/* accuracy, B, Bexp, Rexp */

	struct {
#if WORDS_BIGENDIAN
		uint8_t	__reserved    : 5;
		uint8_t	normal_min    : 1;	/* normal min field specified */
		uint8_t	normal_max    : 1;	/* normal max field specified */
		uint8_t	nominal_read  : 1;	/* nominal reading field specified */
#else
		uint8_t	nominal_read  : 1;	/* nominal reading field specified */
		uint8_t	normal_max    : 1;	/* normal max field specified */
		uint8_t	normal_min    : 1;	/* normal min field specified */
		uint8_t	__reserved    : 5;
#endif
	} analog_flag;

	uint8_t	nominal_read;	/* nominal reading, raw value */
	uint8_t	normal_max;	/* normal maximum, raw value */
	uint8_t	normal_min;	/* normal minimum, raw value */
	uint8_t	sensor_max;	/* sensor maximum, raw value */
	uint8_t	sensor_min;	/* sensor minimum, raw value */

	struct {
		struct {
			uint8_t	non_recover;
			uint8_t	critical;
			uint8_t	non_critical;
		} upper;
		struct {
			uint8_t	non_recover;
			uint8_t	critical;
			uint8_t	non_critical;
		} lower;
		struct {
			uint8_t	positive;
			uint8_t	negative;
		} hysteresis;
	} threshold;
	uint8_t	__reserved[2];
	uint8_t	oem;		/* reserved for OEM use */
	uint8_t	id_code;	/* sensor ID string type/length code */
	uint8_t	id_string[16];	/* sensor ID string bytes, only if id_code != 0 */
} __attribute__ ((packed));

struct sdr_record_mc_locator {
	uint8_t dev_slave_addr;
#if WORDS_BIGENDIAN
	uint8_t __reserved2	: 4;
	uint8_t channel_num 	: 4;
#else
	uint8_t channel_num 	: 4;
	uint8_t __reserved2	: 4;
#endif
#if WORDS_BIGENDIAN
	uint8_t pwr_state_notif	: 3;
	uint8_t __reserved3	: 1;
	uint8_t global_init	: 4;
#else
	uint8_t global_init	: 4;
	uint8_t __reserved3	: 1;
	uint8_t pwr_state_notif	: 3;
#endif
	uint8_t dev_support;
	uint8_t __reserved4[3];
	struct entity_id entity;
	uint8_t oem;
	uint8_t id_code;
	uint8_t id_string[16];
} __attribute__ ((packed));

struct sdr_record_fru_locator {
	uint8_t dev_slave_addr;
	uint8_t device_id;
#if WORDS_BIGENDIAN
	uint8_t logical		: 1;
	uint8_t __reserved2	: 2;
	uint8_t lun		: 2;
	uint8_t bus		: 3;
#else
	uint8_t bus		: 3;
	uint8_t lun		: 2;
	uint8_t __reserved2	: 2;
	uint8_t logical		: 1;
#endif
#if WORDS_BIGENDIAN
	uint8_t channel_num	: 4;
	uint8_t __reserved3	: 4;
#else
	uint8_t __reserved3	: 4;
	uint8_t channel_num	: 4;
#endif
	uint8_t __reserved4;
	uint8_t dev_type;
	uint8_t dev_type_modifier;
	struct entity_id entity;
	uint8_t oem;
	uint8_t id_code;
	uint8_t id_string[16];
} __attribute__ ((packed));

struct sdr_record_oem {
	uint8_t * data;
	int data_len;
};

/*
 * The Get SDR Repository Info response structure
 * From table 33-3 of the IPMI v2.0 spec
 */
struct get_sdr_repository_info_rsp {
	uint8_t sdr_version;
	uint8_t record_count_lsb;
	uint8_t record_count_msb;
	uint8_t free_space[2];
	uint8_t most_recent_addition_timestamp[4];
	uint8_t most_recent_erase_timestamp[4];
#if WORDS_BIGENDIAN	
	uint8_t overflow_flag                          : 1;
	uint8_t modal_update_support                   : 2;
	uint8_t __reserved1                            : 1;
	uint8_t delete_sdr_supported                   : 1;
	uint8_t partial_add_sdr_supported              : 1;
	uint8_t reserve_sdr_repository_supported       : 1;
	uint8_t get_sdr_repository_allo_info_supported : 1;
#else
	uint8_t get_sdr_repository_allo_info_supported : 1;
	uint8_t reserve_sdr_repository_supported       : 1;
	uint8_t partial_add_sdr_supported              : 1;
	uint8_t delete_sdr_supported                   : 1;
	uint8_t __reserved1                            : 1;
	uint8_t modal_update_support                   : 2;
	uint8_t overflow_flag                          : 1;
#endif
} __attribute__ ((packed));




struct ipmi_sdr_iterator
{
	uint16_t reservation;
	int total;
	int next;
};

struct sdr_record_list {
	uint16_t id;
	uint8_t type;
	struct sdr_record_list * next;
	union {
		struct sdr_record_full_sensor * full;
		struct sdr_record_compact_sensor * compact;
		struct sdr_record_eventonly_sensor * eventonly;
		struct sdr_record_fru_locator * fruloc;
		struct sdr_record_mc_locator * mcloc;
		struct sdr_record_oem * oem;
	} record;
};

/* unit description codes (IPMI v1.5 section 37.16) */
#define UNIT_MAX	0x90
static const char * unit_desc[] __attribute__((unused)) = {
	"unspecified",
	"degrees C", "degrees F", "degrees K",
	"Volts", "Amps", "Watts", "Joules",
	"Coulombs", "VA", "Nits",
	"lumen", "lux", "Candela",
	"kPa", "PSI", "Newton",
	"CFM", "RPM", "Hz",
	"microsecond", "millisecond", "second", "minute", "hour", "day", "week",
	"mil", "inches", "feet", "cu in", "cu feet", "mm", "cm", "m", "cu cm", "cu m",
	"liters", "fluid ounce",
	"radians", "steradians", "revolutions", "cycles", "gravities",
	"ounce", "pound", "ft-lb", "oz-in",
	"gauss", "gilberts", "henry", "millihenry",
	"farad", "microfarad", "ohms", "siemens", "mole", "becquerel",
	"PPM", "reserved",
	"Decibels", "DbA", "DbC",
	"gray", "sievert", "color temp deg K",
	"bit", "kilobit", "megabit", "gigabit",
	"byte", "kilobyte", "megabyte", "gigabyte",
	"word", "dword", "qword", "line",
	"hit", "miss", "retry", "reset",
	"overflow", "underrun",
	"collision", "packets",
	"messages", "characters",
	"error", "correctable error", "uncorrectable error",
};

/* sensor type codes (IPMI v1.5 table 36.3) */
#define SENSOR_TYPE_MAX 0x29
static const char * sensor_type_desc[] __attribute__((unused)) = {
	"reserved",
	"Temperature", "Voltage", "Current", "Fan", "Physical Security", "Platform Security Violation Attempt",
	"Processor", "Power Supply", "Power Unit", "Cooling Device", "Other", "Memory", "Drive Slot / Bay",
	"POST Memory Resize", "System Firmware Progress", "Event Logging Disabled", "Watchdog", "System Event",
	"Critical Interrupt", "Button", "Module / Board", "Microcontroller / Coprocessor", "Add-in Card",
	"Chassis", "Chip Set", "Other FRU", "Cable / Interconnect", "Terminator", "System Boot Initiated",
	"Boot Error", "OS Boot", "OS Critical Stop", "Slot / Connector", "System ACPI Power State", "Watchdog",
	"Platform Alert", "Entity Presence", "Monitor ASIC / IC", "LAN", "Management Subsystem Health", "Battery"
};

struct ipmi_sdr_iterator * ipmi_sdr_start(struct ipmi_intf * intf);
struct sdr_get_rs * ipmi_sdr_get_next_header(struct ipmi_intf * intf, struct ipmi_sdr_iterator * i);
uint8_t * ipmi_sdr_get_record(struct ipmi_intf * intf, struct sdr_get_rs * header, struct ipmi_sdr_iterator * i);
void ipmi_sdr_end(struct ipmi_intf * intf, struct ipmi_sdr_iterator * i);
int ipmi_sdr_print_sdr(struct ipmi_intf * intf, uint8_t type);
int ipmi_sdr_print_rawentry(struct ipmi_intf * intf, uint8_t type, uint8_t * raw, int len);
int ipmi_sdr_print_listentry(struct ipmi_intf * intf, struct sdr_record_list * entry);
const char * ipmi_sdr_get_status(uint8_t stat);
double sdr_convert_sensor_reading(struct sdr_record_full_sensor * sensor, uint8_t val);
uint8_t sdr_convert_sensor_value_to_raw(struct sdr_record_full_sensor * sensor, double val);
struct ipmi_rs * ipmi_sdr_get_sensor_reading(struct ipmi_intf * intf, uint8_t sensor);
const char * ipmi_sdr_get_sensor_type_desc(const uint8_t type);

int ipmi_sdr_print_sensor_full(struct ipmi_intf * intf, struct sdr_record_full_sensor * sensor);
int ipmi_sdr_print_sensor_compact(struct ipmi_intf * intf, struct sdr_record_compact_sensor * sensor);
int ipmi_sdr_print_sensor_eventonly(struct ipmi_intf * intf, struct sdr_record_eventonly_sensor * sensor);
int ipmi_sdr_print_sensor_fru_locator(struct ipmi_intf * intf, struct sdr_record_fru_locator * fru);
int ipmi_sdr_print_sensor_mc_locator(struct ipmi_intf * intf, struct sdr_record_mc_locator * mc);

struct sdr_record_list * ipmi_sdr_find_sdr_byentity(struct ipmi_intf * intf, struct entity_id * entity);
struct sdr_record_list * ipmi_sdr_find_sdr_bynumtype(struct ipmi_intf * intf, uint8_t num, uint8_t type);
struct sdr_record_list * ipmi_sdr_find_sdr_byid(struct ipmi_intf * intf, char * id);
void ipmi_sdr_list_empty(struct ipmi_intf * intf);
int ipmi_sdr_print_info(struct ipmi_intf * intf);
void ipmi_sdr_print_discrete_state(uint8_t sensor_type, uint8_t event_type, uint8_t state);

#endif  /* IPMI_SDR_H */
