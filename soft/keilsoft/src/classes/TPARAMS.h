#ifndef _h_rf_params_data_h_
#define _h_rf_params_data_h_



#include <stdint.h>
#include "TM24Cxxx.h"


#define C_SERIALNUMBER_SIZE 5
#define C_PARAMCAPTION_SIZE 32
#define C_MEAS_TXT_VAL_SIZE 8
enum EPARAMTYPE {EPARAMTYPE_NONE = 0, EPARAMTYPE_BOOL = 1, EPARAMTYPE_I32 = 2, EPARAMTYPE_U32 = 3, EPARAMTYPE_I64 = 4, EPARAMTYPE_U64 = 5, EPARAMTYPE_RAW8 = 6, EPARAMTYPE_FLOAT = 7, EPARAMTYPE_STR32 = 8, EPARAMTYPE_ENDENUM = 9};


#pragma pack (push,4)

typedef struct {
	const EPARAMTYPE type;
	const char *name;
} S_HDRPARAM_T;



typedef struct {
	S_HDRPARAM_T hdr;
	const float min;
	const float max;
	const float def;
} S_CONTROL_FLOAT_T;


typedef struct {
	S_HDRPARAM_T hdr;
	const int64_t min;
	const int64_t max;
	const int64_t def;
} S_CONTROL_INT64_T;


typedef struct {
	S_HDRPARAM_T hdr;
	const uint32_t min;
	const uint32_t max;
	const uint32_t def;
} S_CONTROL_UINT32_T;


typedef struct {
	union {
		bool v_b;
		uint8_t raw[4];
		int64_t v_i64;
		uint64_t v_u64;
		int32_t v_i32;
		uint32_t v_u32;
		float v_f;
	} u;
} S_PARAMVALUE_T;


#ifdef RFPARAM_SUPPORT
typedef struct {
	union {
		struct {
			S_PARAMVALUE_T min;
			S_PARAMVALUE_T max;
			S_PARAMVALUE_T def;
			S_PARAMVALUE_T val;
			} dig;
		char txt[sizeof(S_PARAMVALUE_T) * 4];	// max size only 32 charsets
		} u;
} S_FULLPARAM_T;


typedef struct {
	uint8_t type;				// EPARAMTYPE
	S_FULLPARAM_T param;
} S_RFPARAMVALUE_T;
#endif



typedef struct {
	char txt[C_PARAMCAPTION_SIZE];
} S_PARAM_CAPTION_T;


typedef struct {
	uint8_t type;			// EPARAMTYPE
	S_PARAMVALUE_T data;
} S_DATAFLASH_T;

#pragma pack (pop)


class IRFPARAMS {
	protected:
		S_DATAFLASH_T *dloc;//[EPRMIX_ENDENUM];
		TEEPROMIF *mem;
		void clear_data_todef ();
		void correct_all ();
		const S_HDRPARAM_T *list;//[EPRMIX_ENDENUM];		// [EPRMIX_ENDENUM]
		const uint16_t c_list_cnt;
		//static IRFPARAMS *singlobj;
		long find_param_to_name (const char *name);


	
	public:
		IRFPARAMS (TEEPROMIF *m, S_HDRPARAM_T *l, uint16_t pcnt);
		bool get_papam_i32 (uint32_t ix, long &dst);
		long get_papam_i32 (uint32_t ix);
	
		bool get_papam_u32 (uint32_t ix, uint32_t &dst);
		uint32_t get_papam_u32 (uint32_t ix);
	
		bool get_papam_f (uint32_t ix, float &dst);
		float get_papam_f (uint32_t ix);
	
		bool get_papam_b (uint32_t ix, bool &dst);
		bool get_papam_b (uint32_t ix);
	
		void set_papam_f (uint32_t ix, float prm);
		void set_papam_b (uint32_t ix, bool prm);
		void set_papam_u32 (uint32_t ix, uint32_t prm);
		void set_papam_i32 (uint32_t ix, long prm);
		 
		 virtual void load ();
		 virtual void save ();
		 void correct_param (uint32_t ix);
		 void param_todef (uint32_t ix);
		 EPARAMTYPE gettype (uint32_t ix);
		 
		#ifdef RFPARAM_SUPPORT
		bool get_param (EPRMIX ix, const S_PARAM_CAPTION_T *name, S_RFPARAMVALUE_T &dst);
		bool set_param (EPRMIX ix, const S_PARAM_CAPTION_T *name, S_RFPARAMVALUE_T &src);
		#endif

};





#endif
