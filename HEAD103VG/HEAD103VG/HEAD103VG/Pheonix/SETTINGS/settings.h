
#include "configure.h"
#include "FLASH/flash.h"
#ifdef USE_FLASH
#elif defined(USE_QSPI)
#include "EEPROM/eeprom.h"
#endif

#define MARK_SIZE 					9

#define REVISION 					1.0000
#define REVISIONDATE 				"5/13/2022"

#define SOAP_STRING_BUFFERSIZE	256 +3
#define SOAP_STRING_LINE_DELIMETER '\n'
#define SOAP_STRING_START_MARK		0x0a02

#define SOAP_STRING_BLOCK_SIZE	1024
#define SOAP_STRING_FLASH_SIZE (1 * SOAP_STRING_BLOCK_SIZE)
#define SOAP_STRING_BASE_ADDRESS FLASH_LAST_ADDRESS



extern uint32_t soapstring_address;
extern uint32_t current_read_address;
extern uint32_t current_write_address;
extern uint32_t init_gcode_address;
extern uint32_t init_accelerationtable_address;

// Initialize the configuration subsystem (load settings from EEPROM)


void EraseSoapString();
void AppendToSoapString(uint8_t* StringPointer, uint8_t size);
uint16_t ReadSoapString(uint32_t address, uint8_t* data);

uint32_t FindStartAddressOfSoapString();
uint32_t FindNextValuePairStartAddress();
uint32_t FindAddressOfFirstSoapMcodeToExecute();
uint32_t FindFirst0xffInSoapString();
uint32_t FindCurrentSoapStringStartAddress();
uint32_t FindAddressOfFirstAccelerationTableMcode();

void EcecuteEmbeddedMcodeInSoapString();