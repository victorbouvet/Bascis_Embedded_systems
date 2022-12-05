//-----------------------------------------------------------------------------
// TP1_BSE.c
//-----------------------------------------------------------------------------
// AUTH: 
// DATE: 
//
// Target: C8051F02x
// Tool chain: KEIL Microvision5
//
//-----------------------------------------------------------------------------
// Déclarations Registres et Bits de l'espace SFR
#include "intrins.h"
#include<c8051F020.h>
#include<c8051F020_SFR16.h>
#include<TP1_BSE_Lib_Config_Globale.h>
#include<TP1_BSE_Lib_Divers.h>

#define LED_ON 1
#define LED_OFF 0

//on définit les états du bouton poussoir 
#define BP_ON 1
#define BP_OFF 0

//on définit éteint ou clignotement 
#define CLIGNOTEMENT 1
#define ETEINT 0


sbit LED = P1^6;  // LED
sbit BP = P3^7;  //Bouton poussoir 
bit ACK_BP; //variable de type booléenne
bit ETAT_LED;

// signaux témoins pour l'oscilloscope
sbit VISU_INT7_START = P2^0;
bit VISU_INT7_END;
bit VISU_INT7_WIDTH;





//------------------------------------------------------------------------------------
// Function Prototypes

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void ISR_INT7 (void) interrupt 19 {
		VISU_INT7_START = 1;
		VISU_INT7_START = 0;

	
		VISU_INT7_WIDTH = P6 | (1<<1);
	
		ETAT_LED = ~ETAT_LED;
		P3IF &= ~(1<<7); // Pending Flag IE7 remis à zéro
	
		VISU_INT7_WIDTH = P6 | (0<<1);
	
		VISU_INT7_END = P6 | (1<<0);
		VISU_INT7_END = P6 | (0<<0);
}
	
	
	
void config_INT7(void){
	P3IF &= ~(1<<7); // Pending Flag IE7 remis à zéro
	P3IF |= (0<<3); //IE7CF = 0, déclenchement sur front descendant
	EIE2 |= (1<<5); //EX7 = 1, autorisation de l'interruption INT7
	EIP2 |= (1<<5); // Priorité haute pour INT7
}

void main (void) {
	
	      Init_Device();
				config_INT7();
	
				P3MDOUT = P3MDOUT & 0x7F ;
				BP = BP_ON;
				ACK_BP = 0;
				ETAT_LED = LED_ON;
				EA = 1;
			
			while(1)
        {  	
					/*
						if((ACK_BP == BP_OFF) && (BP == BP_OFF))
							{
								ETAT_LED = ~ETAT_LED;
								ACK_BP = BP_ON; //mémoire du dernier état du bouton poussoir
						  }
							
						else if(BP == BP_ON)
							{
								ACK_BP = BP_OFF; //mémoire du dernier état du bouton poussoir
							}*/
						
						
						if(ETAT_LED == CLIGNOTEMENT)
							{
								LED = LED_ON;
								Software_Delay(2); // Allumaghe 20ms
								LED = LED_OFF;
								Software_Delay(10); // Extinction 100ms
							}
							
					  else
							{
								LED = LED_OFF;
							}
					
					
        }						
			}


//*****************************************************************************	 
