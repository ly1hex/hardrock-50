/*
    Copyright 2012 HobbyPCB LLC

    This file is part of HARDROCK-50 Control Firmware

    HARDROCK-50 Control Firmware is free software: you can redistribute it and/or modify it under the terms of the
    GNU General Public License as published by the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    HARDROCK-50 Control Firmware is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along with HARDROCK-50 Control Firmware. If not, see http://www.gnu.org/licenses/.
*/

// Interrupt function for HARDROCK-50

#include "defs.h"

unsigned short snapshotB;

void interrupt() {
     if (INTCON.RBIF)
     {  // RBIF set by change on PORTB b4-b7 (see Data Sheet P101)
        snapshotB = PORTB;      // end any mismatch before clear, take snapshot
        lastB = lastB ^ snapshotB; // xor previous value with current value to detect which pin changed

        INTCON.RBIF = 0;        // clear interrupt flag
        INTCON.RBIE = 0;        // Disable interrupt

        if (lastB.key == 1)
        { // Key Line changed - check value and set flags
          if (snapshotB.key == active) {
           setTX_ON();
          } else {
           setTX_OFF();
          }
        }

        if (lastB.cor == 1)
        { // COR Line changed - check value and set flags
          if (snapshotB.cor == 0)
          { // Carrier Detect when COR Line is LOW
           setTX_ON();
          } else {
           setTX_OFF();
          }
        }
        rbDelayFlag = 1;
        lastB = PORTB;
      }
      else if (INTCON.INT0IF)
      { // BAND-DOWN - INT0 interrupt
        INTCON.INT0IF = 0;
        INTCON.INT0IE = 0;
      }
      else if (INTCON3.INT1IF)
      { // BAND-UP - INT1 interrupt
        INTCON3.INT1IF = 0;
        INTCON3.INT1IE = 0;
      }
      else if (INTCON3.INT2IF)
      { // KEY MODE - INT2 interrupt
        INTCON3.INT2IF = 0;
        INTCON3.INT2IE = 0;
      }
      else if (INTCON.TMR0IF)
      {
        //  Reset for 1ms next interrupt
        TMR0H = 0xC1;    // preset for Timer0 MSB register
        TMR0L = 0x80;    // preset for Timer0 LSB register
        timer0Flag = 1;
        INTCON.TMR0IF = 0;
      }
      
      

}