#ifndef _H_MAVPARAMS_H_
#define _H_MAVPARAMS_H_


#include <stdint.h>
#include "TM24Cxxx.h"
#include "STMSTRING.h"
#include "TM24Cxxx.h"

typedef enum MAV_PARAM_TYPE
{
   MAV_PARAM_TYPE_UINT8=1, /* 8-bit unsigned integer | */
   MAV_PARAM_TYPE_INT8=2, /* 8-bit signed integer | */
   MAV_PARAM_TYPE_UINT16=3, /* 16-bit unsigned integer | */
   MAV_PARAM_TYPE_INT16=4, /* 16-bit signed integer | */
   MAV_PARAM_TYPE_UINT32=5, /* 32-bit unsigned integer | */
   MAV_PARAM_TYPE_INT32=6, /* 32-bit signed integer | */
   MAV_PARAM_TYPE_UINT64=7, /* 64-bit unsigned integer | */
   MAV_PARAM_TYPE_INT64=8, /* 64-bit signed integer | */
   MAV_PARAM_TYPE_REAL32=9, /* 32-bit floating-point | */
   MAV_PARAM_TYPE_REAL64=10, /* 64-bit floating-point | */
   MAV_PARAM_TYPE_ENUM_END=11, /*  | */
} MAV_PARAM_TYPE;

// список внутрених параметров

#define C_EXTMVPR EINTMVPR_ENDENUM  // 


#pragma pack (push, 1)

typedef struct {
	char param_id[16];
} S_MAVPARAMNAME_T;

typedef struct {
	char param_id[16];
	MAV_PARAM_TYPE type;
} S_MVPARAM_HDR_T;



typedef struct {
	S_MVPARAM_HDR_T hdr;
	float value;
	float min;
	float max;
	float def_value;
	float step_val;
} S_MVPARAM_FLOAT_T;



typedef struct {
	S_MVPARAM_HDR_T hdr;
	uint32_t value;
	uint32_t min;
	uint32_t max;
	uint32_t def_value;
	uint32_t step_val;
} S_MVPARAM_U32_T;



typedef struct {
	union {
			uint32_t u32;
			float f;
	} u;
} S_MDAT_T;



typedef struct {
	S_MVPARAM_HDR_T hdr;
	S_MDAT_T value;
	S_MDAT_T min;
	S_MDAT_T max;
	S_MDAT_T def_value;
} S_BASETAGPARAM_T;



typedef struct {
	uint32_t params_cnt;
	S_MDAT_T data;
	uint32_t crc32;
} S_SAVE_PARAMS_T;



#pragma pack (pop)


class TPARAMCONTRL {
	protected:
		bool f_need_save;
		S_MVPARAM_HDR_T **param_internal;
		uint32_t param_int_count;
	
		S_BASETAGPARAM_T *param_external;
		uint32_t param_ext_max_alocate;
		uint32_t param_ext_count;
	
		TEEPROMIF *flashmem;
		const uint32_t c_startflash_adr;
		const uint32_t c_sizeflash;	

		bool load_internal_params ();
		bool save_internal_params ();
		void sets_internal_params_to_default ();
		
		S_MVPARAM_HDR_T *find_param_to_name (char *name, long *dst_ix);
		
		void set_param_to_default (long ix);
	
	public:
		TPARAMCONTRL (TEEPROMIF *m, uint32_t start_m, uint32_t size_m, S_MVPARAM_HDR_T **lst, uint32_t cp);
		
		bool set_value (long ix, S_MDAT_T v);
		bool set_value (char *p, S_MDAT_T v);
		S_MDAT_T *get_value (char *name);
		S_MDAT_T *get_value (long ix);
		S_MVPARAM_HDR_T *get_param_tag (long ix);
	
		uint32_t param_internal_cnt ();
		uint32_t param_external_cnt ();
		uint32_t param_full_cnt ();
		bool is_changed ();
};



#endif
