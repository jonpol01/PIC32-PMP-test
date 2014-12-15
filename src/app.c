/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
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

//#include "app.h"
#include "app.h"
static int count;
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;
//DRV_HANDLE pmphandle;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback funtions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/
void HD_Wr(uint8_t addr, uint8_t data){
	/***/
	//uint8_t data = 0x0F;
	// Set the source address
	PLIB_PMP_AddressSet(PMP_ID_0, addr);
	// Check to see the PMP is not busy, and then read the data
	if(!PLIB_PMP_PortIsBusy(PMP_ID_0)){
		PLIB_PMP_MasterSend(PMP_ID_0, data);
		PMDOUT = 0x4C;
		SYS_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_D,PORTS_BIT_POS_11);
	}
}

void HD_Rd(uint8_t addr, uint8_t data){
	if(!PLIB_PMP_PortIsBusy(PMP_ID_0)){
	}
}
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

void APP_TimerCallback ( void )
{
	appData.timerExpired = true;
}
/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    //PMCONbits.CSF1 = 1;
    //PMCONbits.CS1P = 1;



    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
		appData.state = APP_STATE_REGISTER_TMR;
		break;

	case APP_STATE_REGISTER_TMR:
		appData.timer = SYS_TMR_CallbackPeriodic(5, NULL, APP_TimerCallback);
		if (SYS_TMR_HANDLE_INVALID != appData.timer){
			appData.state = APP_STATE_WAIT;
		}
		break;

	case APP_STATE_WAIT:
		if(appData.timerExpired){
			SYS_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B,PORTS_BIT_POS_10);
			appData.timerExpired = false;
			count++;
			if(count == 10){
				count = 0;
				HD_Wr(0x02, 0x0C);
			}
			// receive the data

		}

		//DRV_PMP_Write(&pmphandle, 0x0000, &myWriteBuffer, 2, 0);
		break;

        /* TODO: implement your application state machine.*/

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }

    }
}
/*******************************************************************************
 End of File
 */
