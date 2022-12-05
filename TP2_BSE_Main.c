//-----------------------------------------------------------------------------
// TP2_BSE.c
//-----------------------------------------------------------------------------
// AUTH: Victor Bouvet, Paul Moreau-Neidhardt
// DATE: 
//
// Target: C8051F02x
// Tool chain: KEIL Microvision5
//
//-----------------------------------------------------------------------------
// Fichiers d'entête
#include "intrins.h"
#include<c8051F020.h>
#include<c8051F020_SFR16.h>
#include<TP2_BSE_Lib_Config_Globale.h>
#include<TP2_BSE_Lib_Divers.h>
#include<TP2_BSE_Main.h>
//-----------------------------------------------------------------------------
// Déclaration des MACROS
#define LED_ON 1
#define LED_OFF 0
#define LED_BLINK 0
#define BP_ON 0
#define BP_OFF 1
#define TO_BE_PROCESSED 1
#define PROCESSED 0
#define SET_VISU_DECL_EXTN  P6 |= (1<<4)
#define RESET_VISU_DECL_EXTN P6 &= ~(1<<4)

//-----------------------------------------------------------------------------
// Déclarations Registres et Bits de l'espace SFR
sbit LED = P1^6;  // LED
sbit BP =P3^7;    // Bouton Poussoir 
sbit DECL_EXTRN = P3^6; // Interruption externe
//-----------------------------------------------------------------------------
// Signaux témoins de l'oscilloscope
sbit VISU_BP = P2^4;
bit VISU_DECL_EXTN;
sbit Visu_INT_Timer2 = P3^5;
//-----------------------------------------------------------------------------
// Variable globale
Event = PROCESSED;


//******************************************************************************
//Configuration des interruptions
void Config_INT7(void)
{
	P3IF &= ~(1<<7); // IE7 mis à 0 pending flag de INT7 effacé
	P3IF |= (0<<3); // IE7CF mis à 0 - sensibilité int7 front descendant	
	
	EIP2 &= ~(1<<5);  // PX7 mis à 0 - INT7 priorité basse
	EIE2 |= (1<<5);  // EX7 mis à 1 - INT7 autorisée
}
//******************************************************************************
void ISR_INT7 (void) interrupt 19
{
	VISU_BP = 1;
	P3IF &= ~(1<<7); // IE7 mis à 0 - remise à zéro du pending flag de INT7 effacé
	Event = TO_BE_PROCESSED;
	VISU_BP = 0;
}	

//*****************************************************************************	 
void Config_INT6(void)
{
	P3IF &= ~(1<<6); // DECL_EXTRN mis à 0 pending flag de INT6 effacé
	P3IF |= (1<<2); // IE6CF mis à 1 - sensibilité int6 front montant
	
	EIP2 &= ~(1<<4); // PX6 mis à 0 - INT6 priorité basse
	EIE2 |= (1<<4); // EX6 mis à 1 - INT6 autorisée
}
//*****************************************************************************	
void ISR_INT6 (void) interrupt 18
{
	SET_VISU_DECL_EXTN;
	
	if((P3IF&(1<<2)) >0){
		P3IF &= ~(1<<2); // IE6CF mis à 0 - sensibilité int6 front descendant
	}
	
	else{
	P3IF |= (1<<2); // IE6CF mis à 1 - sensibilité int6 front montant
	}
	
	P3IF &= ~(1<<6); // IE6 mis à 0 - remise à zéro du pending flag de INT6 effacé
	Event = TO_BE_PROCESSED;	
	RESET_VISU_DECL_EXTN;
}
//*****************************************************************************	
void Config_TIMER2_BT(void)
{
	TR2 = 0; // désactiver le timer
	
	//CKCON &= 0x6f;
	
	TF2 = 0; // reset flag d'overflow
	EXF2 = 0; // reset flag externe
	CT2 = 0; // incrémentation par la clock T2M
	CPRL2 = 0; // auto-reload
	RCLK0 = 0; // auto-reload
	TCLK0 = 0; // auto-reload
	EXEN2 = 0; // dévalider entrée T2EX
	
	RCAP2L = 0x7E;
	RCAP2H = 0xF9;
	TL2 = RCAP2L;
	TH2  = RCAP2H;
	
	PT2 = 1; // priorité haute
	ET2 = 1; // activer interruption
	TR2 = 1; // activer le timer
}
//-----------------------------------------------------------------------------
void ISR_Timer2(void) interrupt 5
{
	Visu_INT_Timer2 = 1;
	Visu_INT_Timer2 = 0;
	// code
	TF2 =0;
	
}
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {
	
bit STATE_LED = LED_BLINK;

 	   // Configurations globales
	      Init_Device();
	   // Configurations  spécifiques  
	      Config_INT7(); // Configuration de INT7
				Config_INT6(); // Configuration de DECL_EXTRN
				Config_TIMER2_BT(); // Configuration du Timer2
	   // Fin des configurations
	      
	      EA = 1;  // Validation globale des interruptions
	
				P6 &= ~(1<<4);
				P74OUT |= (1<<5); // push-pull sur le bit 5 de P74OUT (push pull sur P6.4)
				//P2MDOUT |= (1<<4); // psuh-pull sur le bit 4 de P2
	
				P2MDOUT = 0x10;
	
				P3MDOUT |= (1<<5); // push-pull
// Boucle infinie	
        while(1)
        {  
					   if (Event == TO_BE_PROCESSED)
						 {
							 Event = PROCESSED;
							 STATE_LED =  !STATE_LED;	
						 }
   
            if (STATE_LED == LED_BLINK)
						{
							LED = LED_ON;
							Software_Delay(2);
					    LED = LED_OFF;
					    Software_Delay(10);
						}
						else LED = LED_OFF;						
        }						               	
			}
