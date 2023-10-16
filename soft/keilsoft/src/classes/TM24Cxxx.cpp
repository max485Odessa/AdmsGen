#include "TM24Cxxx.h"



static const TM24MEMARTIB_T mematrib[E24MEM_ENDENUM] = {{8, 5}/*24C01*/, {8, 5}, {16, 5}, {16, 5}, {16, 5}/*24C16*/, {32, 10}, {32, 10}/*24C64*/, {64, 5}/*24C128*/, {64, 5}, {128, 5}/*24C512*/};



TM24CIF::TM24CIF (TI2CIFACE *i2, uint8_t csa, E24MEM m) : chip_sel_adr (0xA0 | (csa << 1)), memtype (m)
{
	c_mem_size =  128UL << memtype;
	page_contrl_mask = mematrib[m].pagesize;
	i2c = i2;
}



bool TM24CIF::write_page (uint16_t adr, uint8_t *src, uint16_t sz_wr, uint16_t &rslt_wr)
{
	bool rv = false;
	uint16_t wr_cnt = 0, adr_chng = adr & page_contrl_mask;
	bool rslt = false;
	if (sz_wr)
		{
		i2c->Start_I2C ();
		do	{
				if (!adress_tx (adr, false)) break;
				rv = true;
				while (sz_wr)
					{
					if (i2c->DataOut_I2C (*src++)) 
						{
						rv = false;
						break;
						}
					adr++;
					if (adr >= c_mem_size)
						{
						rv = false;
						break;
						}
					wr_cnt++;
					if ((adr & page_contrl_mask) != adr_chng) break;
					sz_wr--;
					}
			if (rv) rslt_wr = wr_cnt;
			} while (false);
		i2c->Stop_I2C ();
		}
	return rv;
}



bool TM24CIF::write (uint32_t adr, uint8_t *src, uint32_t sz)
{
	bool rv = false;
	uint16_t szwr_rslt;
	while (sz)
		{
		if (!write_page (adr, src, sz, szwr_rslt)) break;
		SYSBIOS::Wait (mematrib[memtype].wrtime_ms);
		if (szwr_rslt == sz)
			{
			rv = true;
			break;
			}
		adr += szwr_rslt;
		src += szwr_rslt;
		sz -= szwr_rslt;
		}
	return rv;
}



bool TM24CIF::read (uint32_t adr, uint8_t *dst, uint32_t sz_rd)
{
	bool rv = false;
	uint16_t wr_cnt = 0, adr_chng = adr & page_contrl_mask;
	bool rslt = false;
	if (sz_rd)
		{
		i2c->Start_I2C ();
		do	{
				if (!adress_tx (adr, false)) break;		// WRITE BIT
			
				bool ack_level = false;
				i2c->Start_I2C ();
				if (i2c->DataOut_I2C (chip_sel_adr | 1)) break;;		// READ BIT
				while (sz_rd)
					{
					if (sz_rd == 1) ack_level = true;
					dst[0] = i2c->DataIn_I2C (ack_level);
					dst++;
					sz_rd--;
					}
				rv = true;
				} while (false);
		i2c->Stop_I2C ();
		}
	return rv;
}






TM24C16::TM24C16 (TI2CIFACE *i2, uint8_t csa) : TM24CIF(i2, 0, E24MEM_16)
{
	
}




bool TM24C16::adress_tx ( uint32_t adr, bool f_read_bit)
{
	bool rv = false;
	uint8_t ah = (adr >> 7) & 0x0E;
	do	{
			if (i2c->DataOut_I2C (chip_sel_adr | ah | f_read_bit)) break;
			if (i2c->DataOut_I2C (adr)) break;
			rv = true;
			} while (false);
	
	return rv;
}



TM24C128::TM24C128 (TI2CIFACE *i2, uint8_t extadrpin) : TM24CIF(i2, extadrpin & 3, E24MEM_128)
{
}





bool TM24C128::adress_tx (uint32_t adr, bool f_read_bit)
{
	bool rv = false;

	do	{
			if (i2c->DataOut_I2C (chip_sel_adr | f_read_bit)) break;
			if (i2c->DataOut_I2C (adr >> 8)) break;
			if (i2c->DataOut_I2C (adr)) break;
			rv = true;
			} while (false);
	
	return rv;
}
