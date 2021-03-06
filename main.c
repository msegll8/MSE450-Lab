/*
 * Author: Tom Liang
 * Date: Jan 23, 2019
 * Objective: at the very beginning the program should keep all the LEDs off and wait until SW1 is pressed,
 * after which it starts to switch on and off indefinitely the three LEDs according to the sequence of "RED->
 * BLUE->GREEN->RED". You need to add a time delay between the switchings to make the
 * flashing visible. Under this mode, if the user presses SW2 the program should goes back to the
 * initial state (all LEDs off and waiting for SW1 to be pressed).
 */

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>


//Delay function --> Takes any integer and store it as itime
void MSDealy(unsigned int itime) {
    unsigned int i;
    unsigned int j;
    for (i=0; i<itime; i++) {
        for (j=0; j<331; j++);
    }
}

//Provided with lab instruction
void PortF_Init(void) {
    SYSCTL_RCGC2_R |= 0x00000020; // activate clock for PortF
    while ((SYSCTL_PRGPIO_R & 0x00000020) == 0) {}; // wait until PortF is ready
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
    GPIO_PORTF_CR_R = 0x1F; // allow changes to PF4-0
    GPIO_PORTF_AMSEL_R = 0x00; // disable analog function on PF
    GPIO_PORTF_PCTL_R = 0x00000000; // GPIO clear bit PCTL
    GPIO_PORTF_DIR_R = 0x0E; // Set PF4,PF0 input, PF3,PF2,PF1 output
    GPIO_PORTF_AFSEL_R = 0x00; // disable all alternate function
    GPIO_PORTF_PUR_R = 0x11; // enable pullup resistors on PF4 and PF0
    GPIO_PORTF_DEN_R = 0x1F; // enable digital pins PF4-PF0
}

//For part I
uint8_t SW1_State = 0;
uint32_t SW1_ON;

//For part II
uint32_t SW1; // SW1 is PF4, located at bottom left corner, SW1 is HIGH when switch is NOT pressed
uint32_t Out; // outputs to PF3,PF2,PF1 (multicolor LED)
uint32_t SW2; // SW2 is PF0, located at bottom right corner
uint8_t state = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  PART I
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){

    PortF_Init();

    while(1){
        SW1_ON = GPIO_PORTF_DATA_R & 0x10; // read PF4 into SW1 and keep the values updated

        //SW_ON = Switch is closed
        if (SW1_ON){
            SW1_State = 0;
        }

        //else = !SW_ON = Switch is opened
        else {
            SW1_State = 1;
        }
        if (state == 0) {
            GPIO_PORTF_DATA_R = 0x00; // keep all the LEDs off and wait until SW1 is pressed
        }
            //When switch is closed, Turn on Blue LED
            if (SW1_State == 1){
                GPIO_PORTF_DATA_R |= 0x04;
            }

            else if (SW1_State == 0){
                GPIO_PORTF_DATA_R &= 0x11;
                //SW1_ON = GPIO_PORTF_DATA_R & 0x10;
            }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  PART II
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*int main(void)
{
    //initializes port F
    PortF_Init();
    while (1) {
        SW1 = GPIO_PORTF_DATA_R & 0x10; // read PF4 into SW1 and keep the values updated
        SW2 = GPIO_PORTF_DATA_R & 0x01; // read PF0 into SW2 and keep the values updated
        if (!SW1) state = 1;
        else if (!SW2) state = 0;

        if (state == 0) {
            GPIO_PORTF_DATA_R = 0x00; // keep all the LEDs off and wait until SW1 is pressed
        }
        else if (state == 1) {
            GPIO_PORTF_DATA_R &= 0x11; // clear color
            GPIO_PORTF_DATA_R |= 0x02; // red
            MSDealy(800);
            SW2 = GPIO_PORTF_DATA_R & 0x01; // update SW2 value
            if (!SW2) state = 0;
            GPIO_PORTF_DATA_R &= 0x11; // clear color
            GPIO_PORTF_DATA_R |= 0x04; // blue
            MSDealy(800);
            SW2 = GPIO_PORTF_DATA_R & 0x01; // update SW2 value
            if (!SW2) state = 0;
            GPIO_PORTF_DATA_R &= 0x11; // clear color
            GPIO_PORTF_DATA_R |= 0x08; // green
            MSDealy(800);
        }
    }
    //return 0;
}*/

