/*******************************************************************************
  MPLAB Harmony Project Main Source File

  Company:
    Microchip Technology Inc.
  
  File Name:
    main.c

  Summary:
    This file contains the "main" function for an MPLAB Harmony project.

  Description:
    This file contains the "main" function for an MPLAB Harmony project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state 
    machines of all MPLAB Harmony modules in the system and it calls the 
    "SYS_Tasks" function from within a system-wide "super" loop to maintain 
    their correct operation. These two functions are implemented in 
    configuration-specific files (usually "system_init.c" and "system_tasks.c")
    in a configuration-specific folder under the "src/system_config" folder 
    within this project's top-level folder.  An MPLAB Harmony project may have
    more than one configuration, each contained within it's own folder under
    the "system_config" folder.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

//Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <math.h>
#include "system/common/sys_module.h"   // SYS function prototypes
#include "GenericTypeDefs.h"

#define MAX_RS232	274		/* MAX=1+2*8*17+1 */
#define	HEX_02		0x02
#define	HEX_03		0x03

int		Byte2ToByte1(BYTE*, BYTE*);	/* 2Byte To 1Byte */
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all MPLAB Harmony modules, including application(s). */
    SYS_Initialize ( NULL );

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

/************************************************
*  2Byte To 1Byte Data Function
************************************************/
int Byte2ToByte1(BYTE bin_data[], BYTE bout_data[])
{
	int i, n, j, flg;
	BYTE bdata;
	BYTE bdata1;
	BYTE bflg;

	i = n = flg = bflg = 0;

	while(1)
	{
		/* Loop End */
		if(i >= MAX_RS232) break;
		/* 0x03 Search */
		if(bin_data[i] == HEX_03) break;

		/* 0x02 Search */
		if(bin_data[i] == HEX_02) {
			flg=1;
		/* Data Set */
		} else if(flg) {
			bdata = 0;
			/* 0-9 */
			if(bin_data[i] >= 0x30 && bin_data[i] <= 0x39) {
				for(j=0; j<10; j++) if(bin_data[i] == 0x30+j) bdata=0x00+j;
			/* A-F a-f */
			} else if((bin_data[i] >= 0x41 && bin_data[i] <= 0x46) ||
					  (bin_data[i] >= 0x61 && bin_data[i] <= 0x66)) {
				for(j=1; j<7; j++) if(bin_data[i] == 0x40+j || bin_data[i] == 0x60+j) bdata=0x09+j;
			/* DATA ?? PASS */
			} else {
				bflg = 0;
				goto LOOP_END;
				/* END */
				return(1);
			}

			/* 4-7Bit */
			if(bflg==0) {
				bdata1 = bdata << 4;
				bdata1 &= 0xf0;
				bflg = 1;
			/* 0-3Bit */
			} else {
				bdata1 |= bdata;
				bout_data[n] = bdata1;		/* DATA SET */
				n++;
				bflg = 0;
			}
		}
LOOP_END:
		/* Point Up */
		i++;
	}


/*  TEST  */
//	unsigned char uc_Work[5];
	BYTE uc_Work[5];
	memset(uc_Work, 0x00, 5);
	uc_Work[1] = 0x66;
	uc_Work[0] = 0x66; j=0;
	for(i=0;i<strlen(uc_Work);i++){
		/* 0-9 */
		if(0x30 <= uc_Work[i] && 0x39 >= uc_Work[i])
			j+=(uc_Work[i] - 0x30) * (unsigned int)pow(16,i);
		/* a-f */
		if(0x61 <= uc_Work[i] && 0x66 >= uc_Work[i])
			j+=(uc_Work[i] - 0x61 + 0x0A) * (unsigned int)pow(16,i);
	}
/*  TEST  */
	
	/* END */
	return(0);
}