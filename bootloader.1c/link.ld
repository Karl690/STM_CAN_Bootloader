OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
SEARCH_DIR(.)
INCLUDE "memory.ld"

/* Section Definitions */ 
SECTIONS 
{ 
    .text : 
    { 
        KEEP(*(.isr_vector .isr_vector.*)) 
        *(.text .text.* .gnu.linkonce.t.*) 	      
        *(.glue_7t) *(.glue_7)		                
        *(.rodata .rodata* .gnu.linkonce.r.*)		    	                  
    } > rom
    
    .ARM.extab : 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > rom
    
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > rom
    
    . = ALIGN(4); 
    _etext = .;
    _sidata = .; 
    		
    .data : AT (_etext) 
    { 
        _sdata = .; 
        *(.data .data.*) 
        . = ALIGN(4); 
        _edata = . ;
    } > ram

    /* .bss section which is used for uninitialized data */ 
    .bss (NOLOAD) : 
    { 
        _sbss = . ; 
        *(.bss .bss.*) 
        *(COMMON) 
        . = ALIGN(4); 
        _ebss = . ; 
    } > ram
    
    /* stack section */
    .co_stack (NOLOAD):
    {
        . = ALIGN(8);
        *(.co_stack .co_stack.*)
    } > ram

    .POSITION_TABLE_ADDR : AT (0x08007000)
    {
        _sPOSITION_TABLE_ADDR = .;
        *(.POSITION_TABLE_ADDR .POSITION_TABLE_ADDR.*)
        . = ALIGN(4);
        _ePOSITION_TABLE_ADDR = . ;
    } > rom

    .RTD1_TABLE_ADDR : AT (0x08007080)
    {
        _sRTD1_TABLE_ADDR = .;
        *(.RTD1_TABLE_ADDR .RTD1_TABLE_ADDR.*)
        . = ALIGN(4);
        _eRTD1_TABLE_ADDR = . ;
    } > rom

    .RTD2_TABLE_ADDR : AT (0x08007100)
    {
        _sRTD2_TABLE_ADDR = .;
        *(.RTD2_TABLE_ADDR .RTD2_TABLE_ADDR.*)
        . = ALIGN(4);
        _eRTD2_TABLE_ADDR = . ;
    } > rom

    .RTD3_TABLE_ADDR : AT (0x08007180)
    {
        _sRTD3_TABLE_ADDR = .;
        *(.RTD3_TABLE_ADDR .RTD3_TABLE_ADDR.*)
        . = ALIGN(4);
        _eRTD3_TABLE_ADDR = . ;
    } > rom

    .TEMP_SENSOR_TABLE_ADDR : AT (0x08007200)
    {
        _sTEMP_SENSOR_TABLE_ADDR = .;
        *(.TEMP_SENSOR_TABLE_ADDR .TEMP_SENSOR_TABLE_ADDR.*)
        . = ALIGN(4);
        _eTEMP_SENSOR_TABLE_ADDR = . ;
    } > rom
    
    .TEMP_2_LED_ADDR : AT (0x08007228)
    {
        _sLED_TEMP_ADDR = .;
        *(.LED_TEMP_ADDR .LED_TEMP_ADDR.*)
        . = ALIGN(4);
        _eLED_TEMP_ADDR = . ;
    } > rom
    
    .PULSE_TRAIN_ADDR : AT (0x08007268)
    {
        _sPULSE_TRAIN_ADDR = .;
        *(.PULSE_TRAIN_ADDR .PULSE_TRAIN_ADDR.*)
        . = ALIGN(4);
        _ePULSE_TRAIN_ADDR = . ;
    } > rom
    
    . = ALIGN(4); 
    _end = . ; 
} 
