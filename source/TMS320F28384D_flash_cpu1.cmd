CLA_SCRATCHPAD_SIZE = 0x400;
--undef_sym=__cla_scratchpad_end
--undef_sym=__cla_scratchpad_start

MEMORY
{
   	BOOT_RSVD		: origin = 0x000002, length = 0x0001AE     /* Part of M0, BOOT rom will use this for stack */
   	RAMM			: origin = 0x0001B0, length = 0x000650
   	RAMD			: origin = 0x00C000, length = 0x001000

   	RAMLS0			: origin = 0x008000, length = 0x002000
   	RAMLS4			: origin = 0x00A000, length = 0x001000
   	RAMLS6			: origin = 0x00B000, length = 0x001000

   	RAMGS0			: origin = 0x00D000, length = 0x006000
	RAMGS1			: origin = 0x013000, length = 0x004000
	RAMGS_SHARE		: origin = 0x017000, length = 0x006000

#ifdef __TI_COMPILER_VERSION__
   	#if __TI_COMPILER_VERSION__ >= 20012000
	GROUP {      /* GROUP memory ranges for crc/checksum of entire flash */
   	#endif
#endif
   	/* BEGIN is used for the "boot to Flash" bootloader mode   */
   	BEGIN				: origin = 0x080000, length = 0x000002
   	/* Flash sectors */
   	FLASH				: origin = 0x080002, length = 0x037FFE  /* on-chip Flash */
	FLASH_CLA          	: origin = 0x0B8000, length = 0x006000  /* on-chip Flash */
	FLASH13				: origin = 0x0BE000, length = 0x001FF0  /* FLASH13 Not USE */
   	FLASH13_DO_NOT_USE	: origin = 0x0BFFF0, length = 0x000010	/* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */
   
#ifdef __TI_COMPILER_VERSION__
	#if __TI_COMPILER_VERSION__ >= 20012000
	}  crc(_ccs_flash_checksum, algorithm=C28_CHECKSUM_16)
	#endif
#endif

	CPU1TOCPU2	: origin = 0x03A000, length = 0x000800
	CPU2TOCPU1	: origin = 0x03B000, length = 0x000800

	CPUTOCMRAM		: origin = 0x039000, length = 0x000800
	CMTOCPURAM		: origin = 0x038000, length = 0x000800

	CANA_MSG_RAM	: origin = 0x049000, length = 0x000800
	CANB_MSG_RAM	: origin = 0x04B000, length = 0x000800
	RESET			: origin = 0x3FFFC0, length = 0x000002
}

SECTIONS
{
	codestart		: > BEGIN, ALIGN(8)
	.text			: > FLASH, ALIGN(8)
	.cinit			: > FLASH, ALIGN(8)
	.switch			: > FLASH, ALIGN(8)
	.reset			: > RESET, TYPE = DSECT /* not used, */
	.stack			: > RAMM

#if defined(__TI_EABI__)
	.init_array		: > FLASH, ALIGN(8)
	.bss			: > RAMLS0
	.bss:output		: > RAMLS0
	.bss:cio		: > RAMLS0
	.data			: > RAMLS0//RAMLS0
	.sysmem			: > RAMLS0//RAMLS0
	/* Initalized sections go in Flash */
	.const			: > FLASH, ALIGN(8)
#else
	.pinit			: > FLASH, ALIGN(8)
	.ebss			: > RAMLS0
	.esysmem		: > RAMLS0
	.cio			: > RAMLS0
	/* Initalized sections go in Flash */
	.econst			: > FLASH, ALIGN(8)
#endif

	ramgs0 : > RAMGS0, type=NOINIT
	ramgs1 : > RAMGS0, type=NOINIT

	//	병렬 처리시 공유 메모리
	shareMem : > RAMGS_SHARE, type=NOINIT
   
	MSGRAM_CPU1TOCPU2 > CPU1TOCPU2, type=NOINIT
	MSGRAM_CPU2TOCPU1 > CPU2TOCPU1, type=NOINIT

	MSGRAM_CPU_TO_CM   > CPUTOCMRAM, type=NOINIT
	MSGRAM_CM_TO_CPU   > CMTOCPURAM, type=NOINIT

   	/* The following section definition are for SDFM examples */
	Filter_RegsFile  	: > RAMGS0
	Filter1_RegsFile 	: > RAMGS0, fill=0x1111
	Filter2_RegsFile 	: > RAMGS0, fill=0x2222
	Filter3_RegsFile 	: > RAMGS0, fill=0x3333
	Filter4_RegsFile 	: > RAMGS0, fill=0x4444
	Difference_RegsFile	: > RAMGS0, fill=0x3333

	/* crc/checksum section configured as COPY section to avoid including in executable */
	.TI.memcrc          	: type = COPY

#if defined(__TI_EABI__)
	Cla1Prog				:   LOAD = FLASH_CLA,
								RUN = RAMLS4,
								LOAD_START(Cla1funcsLoadStart),
								LOAD_END(Cla1funcsLoadEnd),
								RUN_START(Cla1funcsRunStart),
								LOAD_SIZE(Cla1funcsLoadSize),
								PAGE = 0, ALIGN(8)
#else
	Cla1Prog				:   LOAD = FLASH_CLA,
								RUN = RAMLS4,
								LOAD_START(_Cla1funcsLoadStart),
								LOAD_END(_Cla1funcsLoadEnd),
								RUN_START(_Cla1funcsRunStart),
								LOAD_SIZE(_Cla1funcsLoadSize),
								PAGE = 0, ALIGN(8)
#endif

	Cla1DataRam				: > RAMLS6, PAGE = 0

	Cla1ToCpuMsgRAM			: > RAMLS6, PAGE = 0
	CpuToCla1MsgRAM			: > RAMLS6, PAGE = 0

	/* CLA C compiler sections */
	CLAscratch				:
							{ *.obj(CLAscratch)
							. += CLA_SCRATCHPAD_SIZE;
							*.obj(CLAscratch_end) } > RAMLS6, PAGE = 0

	.scratchpad				: > RAMLS6, PAGE = 0
	.bss_cla				: > RAMLS6, PAGE = 0
	//.const_cla			: > RAMLS6, PAGE = 0
#if defined(__TI_EABI__)
	.const_cla				:	LOAD = FLASH_CLA,
								RUN = RAMLS6,
								RUN_START(Cla1ConstRunStart),
								LOAD_START(Cla1ConstLoadStart),
								LOAD_SIZE(Cla1ConstLoadSize),
								ALIGN(8)
#else
	.const_cla				:	LOAD = FLASH_CLA,
								RUN = RAMLS6,
								RUN_START(_Cla1ConstRunStart),
								LOAD_START(_Cla1ConstLoadStart),
								LOAD_SIZE(_Cla1ConstLoadSize),
								ALIGN(8)
#endif

#if defined(__TI_EABI__)
		.TI.ramfunc : {} 	LOAD = FLASH,
							RUN = RAMGS0,
							LOAD_START(RamfuncsLoadStart),
							LOAD_SIZE(RamfuncsLoadSize),
							LOAD_END(RamfuncsLoadEnd),
							RUN_START(RamfuncsRunStart),
							RUN_SIZE(RamfuncsRunSize),
							RUN_END(RamfuncsRunEnd),
							ALIGN(8)
#else
		.TI.ramfunc : {}	LOAD = FLASH,
							RUN = RAMGS0,
							LOAD_START(_RamfuncsLoadStart),
							LOAD_SIZE(_RamfuncsLoadSize),
							LOAD_END(_RamfuncsLoadEnd),
							RUN_START(_RamfuncsRunStart),
							RUN_SIZE(_RamfuncsRunSize),
							RUN_END(_RamfuncsRunEnd),
							ALIGN(8)
#endif

}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
