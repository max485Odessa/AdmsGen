#include "TMAVPARAMS.h"



TPARAMCONTRL::TPARAMCONTRL (TEEPROMIF *m, uint32_t start_m, uint32_t size_m, S_MVPARAM_HDR_T **lst, uint32_t cp) : c_startflash_adr (start_m), c_sizeflash (size_m)
{
	param_internal = lst;
	param_int_count = cp;
	flashmem = m;
}



uint32_t TPARAMCONTRL::param_internal_cnt ()
{
	return param_int_count;
}



uint32_t TPARAMCONTRL::param_external_cnt ()
{
	return param_ext_count;
}



uint32_t TPARAMCONTRL::param_full_cnt ()
{
	return param_ext_count + param_int_count;
}



bool TPARAMCONTRL::load_internal_params ()
{
	bool rv = false;
	uint32_t load_cnt;
	uint32_t load_crc32;
	uint32_t read_adr = c_startflash_adr;
	
	if (flashmem->read ( read_adr, (uint8_t*)&load_cnt, sizeof(load_cnt)))
		{
		if (load_cnt == param_int_count)
			{
			read_adr += sizeof(load_cnt);
			S_MDAT_T *savearr = new S_MDAT_T[param_int_count];
			uint32_t loadbytesize =  sizeof(S_MDAT_T) * param_int_count;
			if (flashmem->read (read_adr, (uint8_t*)savearr, loadbytesize))
				{
				read_adr += loadbytesize;
				if (flashmem->read (read_adr, (uint8_t*)&load_crc32, sizeof(load_crc32)))
					{
					if (load_crc32 == GN_CRC32 (savearr, loadbytesize))
						{
						uint32_t ix = 0;
						S_BASETAGPARAM_T *dst;
						while (ix < param_int_count)
							{
							dst = (S_BASETAGPARAM_T*)param_internal[ix];
							CopyMemorySDC ((char*)&savearr[ix], (char*)&dst->value, sizeof(S_MDAT_T));
							//dst->value = savearr[ix];
							ix++;
							}
						rv = true;
						}
					}
				}
			delete [] savearr;
			}
		}
	return rv;
}



bool TPARAMCONTRL::save_internal_params ()
{
bool rv = false;
uint32_t write_cnt = param_int_count;
uint32_t write_adr = c_startflash_adr;
	

if (flashmem->write ( write_adr, (uint8_t*)&write_cnt, sizeof(write_cnt)))
	{
	write_adr += sizeof(write_cnt);
	S_MDAT_T *savearr = new S_MDAT_T[param_int_count];
	uint32_t ix = 0;
	S_BASETAGPARAM_T *dst;	
	while (ix < param_int_count)
		{
		CopyMemorySDC ((char*)&((S_BASETAGPARAM_T*)param_internal[ix])->value, (char*)&savearr[ix], sizeof(S_MDAT_T));
		//savearr[ix] = ((S_BASETAGPARAM_T*)param_internal[ix])->value;
		ix++;
		}
	uint32_t writebytesize =  sizeof(S_MDAT_T) * param_int_count;
	if (flashmem->write ( write_adr, (uint8_t*)savearr, writebytesize))
		{
		write_adr += writebytesize;
		uint32_t write_crc32 = GN_CRC32 (savearr, writebytesize);
		if (flashmem->write ( write_adr, (uint8_t*)&write_crc32, sizeof(write_crc32))) 
			{
			f_need_save = false;
			rv = true;
			}
		}
	delete [] savearr;
	}
return rv;
}



void TPARAMCONTRL::sets_internal_params_to_default ()
{
	uint32_t ix = 0;
	while (ix < param_int_count)
		{
		set_param_to_default (ix);
		ix++;
		}
}




S_MVPARAM_HDR_T *TPARAMCONTRL::find_param_to_name (char *name, long *dst_ix)
{
	S_MVPARAM_HDR_T *rv = 0;
	long ix = 0;
	TSTMSTRING str;
	uint32_t maxparams_cnt = param_full_cnt ();
	while (ix < maxparams_cnt)
		{
		S_MVPARAM_HDR_T *paramtag = get_param_tag (ix);
		if (!paramtag) break;
		str.set_context (paramtag->param_id, sizeof(paramtag->param_id)-1);
		if (str == name)
			{
			if (dst_ix) *dst_ix = ix;
			rv = paramtag;
			break;
			}
		ix++;
		}
	return rv;
}




S_MVPARAM_HDR_T *TPARAMCONTRL::get_param_tag (long ix)
{
	S_MVPARAM_HDR_T *rv = 0;
	if (ix < param_full_cnt ()) 
		{
		if (ix < param_int_count)
			{
			// internal params
			rv = const_cast<S_MVPARAM_HDR_T*>(param_internal[ix]);
			}
		else
			{
			ix -= param_int_count;
			// external params
			if (param_external) rv = (S_MVPARAM_HDR_T*)&param_external[ix];
			}
		}
	return rv;
}






S_MDAT_T *TPARAMCONTRL::get_value (char *name)
{
	S_MDAT_T *rv = 0;
	S_MVPARAM_HDR_T *s = find_param_to_name (name, 0);
	if (s)  {
		S_BASETAGPARAM_T *slot = (S_BASETAGPARAM_T*)s;
		rv = &slot->value;
		}
	return rv;
}



S_MDAT_T *TPARAMCONTRL::get_value (long ix)
{
	S_MDAT_T *rv = 0;
	S_MVPARAM_HDR_T *s = get_param_tag (ix);
	if (s) {
		S_BASETAGPARAM_T *slot = (S_BASETAGPARAM_T*)s;
		rv = &slot->value;
		}
	return rv;
}



bool TPARAMCONTRL::set_value (char *name, S_MDAT_T v)
{
	bool rv = false;
	S_MVPARAM_HDR_T *s = find_param_to_name (name, 0);
	if (s) {
		S_BASETAGPARAM_T *slot = (S_BASETAGPARAM_T*)s;
		slot->value = v;
		f_need_save = true;
		}
	return rv;
}


bool TPARAMCONTRL::set_value (long ix, S_MDAT_T v)
{
	bool rv = false;
	S_MVPARAM_HDR_T *s = get_param_tag (ix);
	if (s) {
		S_BASETAGPARAM_T *slot = (S_BASETAGPARAM_T*)s;
		slot->value = v;
		f_need_save = true;
		}
	return rv;
}



void TPARAMCONTRL::set_param_to_default (long ix)
{
	S_MVPARAM_HDR_T *s = get_param_tag (ix);
	if (s) {
		S_BASETAGPARAM_T *slot = (S_BASETAGPARAM_T*)s;
		switch (slot->hdr.type)
			{
			case MAV_PARAM_TYPE_UINT32:
			case MAV_PARAM_TYPE_INT32:
				{
				slot->value.u.u32 = slot->def_value.u.u32;
				break;
				}
			case MAV_PARAM_TYPE_REAL32:
				{
				slot->value.u.f = slot->def_value.u.f;
				break;
				}
			default: break;
			}
		}
}


