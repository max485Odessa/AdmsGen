#ifndef _H_STM32F4_HALLLIB_M24128_H_
#define _H_STM32F4_HALLLIB_M24128_H_



#include "hard_rut.h"
#include "I2CSOFTWARE.H"
#include "SYSBIOS.H"



typedef struct {
	uint8_t pagesize;
	uint8_t wrtime_ms;
} TM24MEMARTIB_T;


enum E24MEM {E24MEM_01 = 0, E24MEM_02, E24MEM_04, E24MEM_08, E24MEM_16, E24MEM_32, E24MEM_64, E24MEM_128, E24MEM_256, E24MEM_512, E24MEM_ENDENUM};

class TEEPROMIF {
	public:
		virtual bool write (uint32_t adr, uint8_t *src, uint32_t sz) = 0;
		virtual bool read (uint32_t adr, uint8_t *dst, uint32_t sz) = 0;
};



class TM24CIF: public TEEPROMIF {
	protected:
		TI2CIFACE *i2c;
		const E24MEM memtype;
		const uint8_t chip_sel_adr;		// external sel pins (A2-A0)
		uint16_t page_contrl_mask;
		uint16_t c_mem_size;
	
		//virtual uint8_t genchipsel () = 0;
		virtual bool adress_tx (uint32_t adr, bool f_read_bit) = 0;
		bool write_page (uint16_t adr, uint8_t *src, uint16_t sz_wr, uint16_t &rslt_wr);
		TM24CIF (TI2CIFACE *i2, uint8_t csa, E24MEM m);
	
	public:
		
		virtual bool write (uint32_t adr, uint8_t *src, uint32_t sz) override;
		virtual bool read (uint32_t adr, uint8_t *dst, uint32_t sz) override;
};



class TM24C16: public TM24CIF {
		
		virtual bool adress_tx (uint32_t adr, bool f_read_bit) override;
		//virtual uint8_t genchipsel () override;

	public:
		TM24C16 (TI2CIFACE *i2, uint8_t extadrpin);
};


class TM24C128: public TM24CIF {
		
		virtual bool adress_tx ( uint32_t adr, bool f_read_bit) override;
	
	public:
		TM24C128 (TI2CIFACE *i2, uint8_t extadrpin);
};



#endif

