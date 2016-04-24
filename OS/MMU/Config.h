#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#ifndef configUSE_MALLOC_FAILED_HOOK
	#define configUSE_MALLOC_FAILED_HOOK 0
#endif


/*-----------------------------------------*/
/*
#define configTOTAL_HEAP_SIZE   10240 
*/
/*
typedef struct
{
	bool type; //TivaC=0 , Atmega32=1
	uint16_t configTOTAL_HEAP_SIZE;
} Heap_Init;
void heap_init(Heap_Init *size);
*/
/*-----------------------------------------*/

#if KitType == 0 // atmega32
	#define configTOTAL_HEAP_SIZE 1500
#else if KitType == 1 //tivaC
	#define configTOTAL_HEAP_SIZE 30000
#endif

/*portBYTE_ALIGNMENT defined by user but for default value portBYTE_ALIGNMENT=8  need to implement this as structure*/
#if portBYTE_ALIGNMENT == 8
	#define portBYTE_ALIGNMENT_MASK ( 0x0007 )
#endif

#if portBYTE_ALIGNMENT == 4
	#define portBYTE_ALIGNMENT_MASK	( 0x0003 )
#endif

#if portBYTE_ALIGNMENT == 2
	#define portBYTE_ALIGNMENT_MASK	( 0x0001 )
#endif

#if portBYTE_ALIGNMENT == 1
	#define portBYTE_ALIGNMENT_MASK	( 0x0000 )
#endif

#ifndef portBYTE_ALIGNMENT_MASK
	#error "Invalid portBYTE_ALIGNMENT definition"
#endif