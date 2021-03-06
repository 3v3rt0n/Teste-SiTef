__attribute__ ((unused))        static char *SVNid = "DTPLSVN$Id: Plu_Processing.c 20586 2010-10-20 12:54:45Z bine $";

/*----------------------------------------------------------------------
 *  Copyright by Wincor Nixdorf International GmbH, 33094 Paderborn
 *  Germany, 2005 - 2010
 *  All rights reserved.
----------------------------------------------------------------------*/

/*
 *  Holds sUser_Plu_Processing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "calypso/db_def.h"
#include "calypso/db_empl.h"
#include "calypso/db_plu.h"
#include "calypso/db_move.h"
#include "calypso/db_msgue.h"
#include "calypso/db_dept.h"
#include "calypso/db_cmmedia.h"
#include "calypso/db_medlst.h"
#include "calypso/db_ejrecs.h"
#include "calypso/db_acc.h"
#include "calypso/db_bwlist.h"
#include "calypso/db_barcode.h"
#include "calypso/db_command.h"
#include "calypso/db_cust.h"
#include "calypso/db_datent.h"
#include "calypso/db_emprof.h"
#include "calypso/db_halo.h"
#include "calypso/db_itmz.h"
#include "calypso/db_keyb.h"
#include "calypso/db_medgrp.h"
#include "calypso/db_msr.h"
#include "calypso/db_pluext.h"
#include "calypso/db_poslit.h"
#include "calypso/db_postxt.h"
#include "calypso/db_restr.h"
#include "calypso/db_scf.h"
#include "calypso/db_tax.h"
#include "calypso/db_tcf.h"
#include "calypso/db_termtot.h"
#include  "calypso/db_cust_account.h"

#include "calypso/events.h"
#include "calypso/posdef.h"
#include "calypso/pospi.h"
#include "calypso/structs.h"
#include  "calypso/cust_account.h"
#include "calypso/poslits.h"
#include "calypso/environ.h"
#include "calypso/osys.h"
#include "calypso/devices.h"
#include "calypso/errserv.h"
#include "calypso/protos.h"
#include "calypso/config.h"
//#include "calypso/sysapi.h"
#include "calypso/discount.h"
#include "calypso/ueprotos.h"
#include "calypso/fiscal.h"

#include "syscom/nm_error.h" /* Containing errorcodes for SYSCOM */
#include "syscom/nm_ta.h"    /* Containing return and errorcodes for lSend_EJ */
#include "syscom/calprint.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Description	: sGetICMSTaxRate() -

NOTE 		:


Comments    :

Arguments   : None

Returns     : None.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
short sGetICMSTaxRate(char * szFiguraFiscal, short * psICMSTaxRate)
{
	printf("%s",gstCustomerAccount.szAccountNmbr);
	if (SUCCESS == sGet_Customer_Account_Info(gstItemStruct.stItemRecord.szCouponFmly))
	{
		//now search for the ICMS tax rate
		if (atoi(gstCustomerAccount.szTaxId) > 0 && atoi(gstCustomerAccount.szTaxId) < 10000)
		{
			*psICMSTaxRate = atoi(gstCustomerAccount.szTaxId);
		}
		else
		{
			return (ERROR);
		}

	}
	else
	{
		return (ERROR);
	}

	return (SUCCESS);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Description	: cCheckForFiscalItems() -

NOTE 		:


Comments    :

Arguments   : None

Returns     : None.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
char cCheckForFiscalItems()
{
	char cRes = 1;

	//need to find all validations rules for ICMS, by now we'll check if it exists and is >0 and <100
	printf("\nszCouponFmly %s", gstItemStruct.stItemRecord.szCouponFmly);
	//check for ICMS category, considering we'll get it from szCouponFmly
	if (strcmp(gstItemStruct.stItemRecord.szCouponFmly,"T0"	)==0 	||
		strcmp(gstItemStruct.stItemRecord.szCouponFmly,"T1"	)==0 	||
		strcmp(gstItemStruct.stItemRecord.szCouponFmly,"T2"	)==0 	||
		strcmp(gstItemStruct.stItemRecord.szCouponFmly,"T3"	)==0 	||
		strcmp(gstItemStruct.stItemRecord.szCouponFmly,"T4"	)==0 	||
		strcmp(gstItemStruct.stItemRecord.szCouponFmly,"TN"	)==0 	||
		strcmp(gstItemStruct.stItemRecord.szCouponFmly,"F"	)==0 	||
		strcmp(gstItemStruct.stItemRecord.szCouponFmly,"I"	)==0	)
	{
		//if valid "figura fiscal" try to find corresponding CustomerAccount, where we can find the tax rates for ICMS or ISS
		if (ERROR == sGetICMSTaxRate(gstItemStruct.stItemRecord.szCouponFmly, (short*)&gstItemStruct.stItemRecord.flItmz2))
		{
			return cRes = 0;
		}
	}
	else
	{
		cRes=0;
	}

	//get PIS tax rate
	if (gstItemStruct.unSpecific.stPlu.usP_short1 > 9999)
	{
		cRes = 0;
	}
	else
	{
		gstItemStruct.stItemRecord.ulMrpPrice1 = gstItemStruct.unSpecific.stPlu.usP_short1;
	}
	//get COFINS tax rate
	if (gstItemStruct.unSpecific.stPlu.usP_short2 > 9999)
	{
		cRes = 0;
	}
	else
	{
		gstItemStruct.stItemRecord.ulMrpPrice2 = gstItemStruct.unSpecific.stPlu.usP_short2;
	}

	return cRes=1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Description	: cCheckForFiscalItems() -

NOTE 		:


Comments    :

Arguments   : None

Returns     : None.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
char cCheckForSATItems()
{
	char cRes = 1;
	/*printf("\ncCheckForSATItems \n");
	printf("\nRecType %d\n", gstItemStruct.uchRecType);
	printf("\nDesc %s\n",gstItemStruct.stItemRecord.szDesc);
	printf("\Qty %d\n",gstProcItem.ulCurrentQty);*/

	if (strcmp(gstItemStruct.stItemRecord.szDesc,"") 	== 0	||
			gstProcItem.ulCurrentQty 					== 0	||
			gstItemStruct.unSpecific.stPlu.ulP_long3 	== 0	||
			gstItemStruct.unSpecific.stPlu.ulP_long4 	== 0	||
			/*gstItemStruct.unSpecific.stPlu.usP_short3	== 0 	||*/
			gstItemStruct.unSpecific.stPlu.uchP_char1	>  0	||
			gstItemStruct.unSpecific.stPlu.uchP_char2	>  0	)
	{
		printf("SAT cRes %d", cRes);
		cRes = 0;
	}

/*	long NCM = gstItemStruct.unSpecific.stPlu.ulP_long3;
	long TotalTaxRates = gstItemStruct.unSpecific.stPlu.ulP_long4;
	short AdditionalNCMCode = gstItemStruct.unSpecific.stPlu.usP_short3;
	char CSTReturn = gstItemStruct.unSpecific.stPlu.uchP_char1;
	char CSTSale = gstItemStruct.unSpecific.stPlu.uchP_char2;*/

//fie
/*	strcpy(cEAN, ""); // pegar da aplicacao																			const char *cEAN,
	strcpy(unidadeMedida, "KG"); // pegar da aplicacao																const char *unidadeMedida,
	strcpy(cEANTrib, ""); // pegar da aplicacao																		const char *cEANTrib,
	strcpy(valorDesconto, ""); // pegar da aplicacao																const char *valorDesconto,
	strcpy(arredondaTrunca, "A"); // passar o caractere "A"															const char *arredondaTrunca,
	strcpy(ncm, "19041000"); // pegar da aplicacao																	const char *ncm,
	strcpy(cfop, "5102"); // pegar da aplicacao																		const char *cfop,
	strcpy(informacaoAdicional, "informacaoAdicional001"); // pegar da aplicacao									const char *informacaoAdicional,
	strcpy(cstIcms, "40"); // pegar da aplicacao																	const char *cstIcms,
	strcpy(origemProduto, "0"); // pegar da aplicacao																const char *origemProduto,
	strcpy(csosn, ""); // vazio																						const char *csosn,
	strcpy(percentualReducBCICMS, ""); // vazio																		const char *percentualReducBCICMS,
	strcpy(aliquotaICMS, ""); // pegar da aplicacao																	const char *aliquotaICMS,
	strcpy(aliquotaCreditoICMSSN, ""); // pegar da aplicacao														const char *aliquotaCreditoICMSSN,
	strcpy(CEST, "1702401"); // pegar da aplicacao																	const char *CEST,
*/
	printf("SAT returning %d", cRes);
	return cRes=1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Description  : sUser_Plu_Processing() - This function is where a user
                   would link into the system with their own functions if
                   the user wanted to gain control in the processing of
                   a Plu.

                   NOTE : If the Enter key is defined as a Plu key, e.g. item
                   entries are done using the enter key, you have to enable
                   user exits on the enter key in keyprof.cfg, too.
                   User Exits are supplied at the following positions in the
                   processing of a Plu:

                   1. Immediately after a key is depressed but prior to
                      any processing or error checking.

                   2. After data records are read in but before testing
                      fields or flags in these records.

                   3. After data records are read and checked for errors
                      but before the price option.

                   4. Before Displaying the Item.

                   5. Before Updating Totals for the Item.

                   6. Before Printing the Item.

                   7. At the start of setting up post processing.

                   8. After all processing is done but prior to going
                      back to the event handler.     

    Comments     : This is called from the event handler.
                 : EVENT 24, EVENT_ROW 61 -> EXT_PLU.

    Arguments    : None

    Returns      : USER_EXIT_RETURN The User must exit to the event handler with
                   a return(USER_EXIT_RETURN) after calling the function
                   Restore_Stack_For_User_Exit.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
short sUser_Plu_Processing(void)
{
	calprint("V1",CLP_POS_USEX, "userexit", "sUser_Plu_Processing US=<%d>",
			gstUserSave.uchUserPosition);

	switch(gstUserSave.uchUserPosition){
	case 1:
		break;
	case 2:
		printf("**** CASE 2 ulPrice1 %ld", gstItemStruct.stItemRecord.ulPrice1);
		if (gstItemStruct.stItemRecord.ulPrice1 == 0)//check for item price == 0
		{
			//clear keyboard display before showing new message
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_ONE, "","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_TWO, "","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_THREE, "","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_FOUR, "","");

			//display message on keyboard display
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_ONE, "COD ITEM:","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_TWO, gstItemStruct.stItemRecord.szNmbr,"");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_THREE, "PRECO INVALIDO","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_FOUR, HIT_CLEAR,"");

			//copy message to be displayed on customer display too
			strcpy(gstDispStr.szErrorString,"PRECO INVALIDO");
			strcpy(gstDispStr.szErrorString2,HIT_CLEAR);

			//wait until operator press clear
			while( EXT_CLEAR != sWait_Ent_Clear())
			{
				//since below method resets some displays, we need to write it again
				strcpy(gstDispStr.szErrorString,"PRECO INVALIDO");
				strcpy(gstDispStr.szErrorString2,HIT_CLEAR);
			}

			gstUserSave.uchUserReturnCode = 1;
			//gstUserSave.unUserFlags.stFlags.fbUserError = TRUE;

		}
		//check for Fiscal items
		else if (!cCheckForFiscalItems())
		{
			//clear keyboard display before showing new message
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_ONE, "","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_TWO, "","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_THREE, "","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_FOUR, "","");

			//display message on keyboard display
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_ONE, "COD ITEM:","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_TWO, gstItemStruct.stItemRecord.szNmbr,"");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_THREE, "TRIB INVALIDA","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_FOUR, HIT_CLEAR,"");

			//copy message to be displayed on customer display too
			strcpy(gstDispStr.szErrorString,"TRIB INVALIDA");
			strcpy(gstDispStr.szErrorString2,HIT_CLEAR);

			while( EXT_CLEAR != sWait_Ent_Clear())
			{
				//since below method resets some displays, we need to write it again
				strcpy(gstDispStr.szErrorString,"TRIB INVALIDA");
				strcpy(gstDispStr.szErrorString2,HIT_CLEAR);
			}

			gstUserSave.uchUserReturnCode = 1;
		}
		//check for all info on Item that is needed to send a request for SAT-NFCe
		else if (!cCheckForSATItems())
		{
			//clear keyboard display before showing new message
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_ONE, "","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_TWO, "","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_THREE, "","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_FOUR, "","");

			//display message on keyboard display
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_ONE, "COD ITEM:","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_TWO, gstItemStruct.stItemRecord.szNmbr,"");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_THREE, "MSGRIA INVALIDA","");
			vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_FOUR, HIT_CLEAR,"");

			//copy message to be displayed on customer display too
			strcpy(gstDispStr.szErrorString,"MSGRIA INVALIDA");
			strcpy(gstDispStr.szErrorString2,HIT_CLEAR);

			while( EXT_CLEAR != sWait_Ent_Clear())
			{
				//since below method resets some displays, we need to write it again
				strcpy(gstDispStr.szErrorString,"MSGRIA INVALIDA");
				strcpy(gstDispStr.szErrorString2,HIT_CLEAR);
			}

			gstUserSave.uchUserReturnCode = 1;
		}
		//if we have no issue, proceed with taxes calculation and store them
		else
		{
			//using tax rates on following struct fields
			//ICMS rate on gstItemStruct.stItemRecord.flItmz2
			//PIS rate on gstItemStruct.stItemRecord.ulMrpPrice1
			//COFINS rate on gstItemStruct.stItemRecord.ulMrpPrice2

			gstItemStruct.stItemRecord.flItmz2 = 1800;
			gstItemStruct.stItemRecord.ulMrpPrice1 = 750;
			gstItemStruct.stItemRecord.ulMrpPrice2 = 350;

			//ICMS calculations / maybe this will have to be done after item is processed

			//later will have to add discounts to calculation
//(price*qty)*tax
			//storing ICMS amount on ulMrpPrice3
			//gstItemStruct.stItemRecord.ulMrpPrice3 =((gstItemStruct.stItemRecord.ulPrice1 * gstProcItem.ulCurrentQty)/1000)*gstItemStruct.stItemRecord.flItmz2/100;
			long lModule=0;

			//gstItemStruct.stItemRecord.ulMrpPrice4 =((gstItemStruct.stItemRecord.ulPrice1 * gstProcItem.ulCurrentQty)/1000)*gstItemStruct.stItemRecord.flItmz2/100;
			gstItemStruct.stItemRecord.ulMrpPrice3 = lRounding(((gstItemStruct.stItemRecord.ulPrice1 * gstProcItem.ulCurrentQty)/1000),
																 gstItemStruct.stItemRecord.flItmz2, MULTIPLY_OPRT, 1, ARITHMETIC_ROUND);
			gstItemStruct.stItemRecord.ulMrpPrice3 = gstItemStruct.stItemRecord.ulMrpPrice3%100 < 50 ? gstItemStruct.stItemRecord.ulMrpPrice3/100 :
													gstItemStruct.stItemRecord.ulMrpPrice3/100 + 1;


			//storing PIS amount on ulMrpPrice4
			//gstItemStruct.stItemRecord.ulMrpPrice4 =((gstItemStruct.stItemRecord.ulPrice1 * gstProcItem.ulCurrentQty)/1000)*gstItemStruct.stItemRecord.ulMrpPrice1/100;
			gstItemStruct.stItemRecord.ulMrpPrice4 = lRounding(((gstItemStruct.stItemRecord.ulPrice1 * gstProcItem.ulCurrentQty)/1000),
																 gstItemStruct.stItemRecord.ulMrpPrice1, MULTIPLY_OPRT, 1, ARITHMETIC_ROUND);
			gstItemStruct.stItemRecord.ulMrpPrice4 = gstItemStruct.stItemRecord.ulMrpPrice4%100 < 50 ? gstItemStruct.stItemRecord.ulMrpPrice4/100 :
																gstItemStruct.stItemRecord.ulMrpPrice4/100 + 1;

			//storing COFINS amount on ulMrpPrice5
			//gstItemStruct.stItemRecord.ulMrpPrice5 =((gstItemStruct.stItemRecord.ulPrice1 * gstProcItem.ulCurrentQty)/1000)*gstItemStruct.stItemRecord.ulMrpPrice2/100;
			gstItemStruct.stItemRecord.ulMrpPrice5 = lRounding(((gstItemStruct.stItemRecord.ulPrice1 * gstProcItem.ulCurrentQty)/1000),
																 gstItemStruct.stItemRecord.ulMrpPrice2, MULTIPLY_OPRT, 1, ARITHMETIC_ROUND);
			gstItemStruct.stItemRecord.ulMrpPrice5 = gstItemStruct.stItemRecord.ulMrpPrice5%100 < 50 ? gstItemStruct.stItemRecord.ulMrpPrice4/100 :
																gstItemStruct.stItemRecord.ulMrpPrice5/100 + 1;

			printf("\nPreco %ld\n", gstItemStruct.stItemRecord.ulPrice1);
			printf("\nTaxa ICMS %ld\n", gstItemStruct.stItemRecord.flItmz2);
			printf("\nTaxa PIS %ld\n", gstItemStruct.stItemRecord.ulMrpPrice1);
			printf("\nTaxa COFINS %ld\n", gstItemStruct.stItemRecord.ulMrpPrice2);

			printf("\nvalor ICMS %ld\n", gstItemStruct.stItemRecord.ulMrpPrice3);
			printf("\nvalor PIS %ld\n", gstItemStruct.stItemRecord.ulMrpPrice4);
			printf("\nvalor COFINS %ld\n", gstItemStruct.stItemRecord.ulMrpPrice5);
/*
			printf("\nItem Struct\n");
			printf("uchQtyUnit %d\n", gstItemStruct.stItemRecord.uchQtyUnit);
			printf("ulPrice1 %ld\n", gstItemStruct.stItemRecord.ulPrice1);
			printf("ulQtySold %ld\n", gstItemStruct.ulQtySold);
			printf("ulExtendAmt %ld\n", gstItemStruct.ulExtendAmt);
			printf("ulQtyMkdn %ld\n", gstItemStruct.ulQtyMkdn);
			printf("lAmtMkdn %ld\n", gstItemStruct.lAmtMkdn);

			printf("\nProc Item\n");
			printf("lCurrentPrice %d\n", gstProcItem.lCurrentPrice);
			printf("lQtyMkdn %d\n", gstProcItem.lQtyMkdn);
			printf("lRegularPrice %d\n", gstProcItem.lRegularPrice);
			printf("ulCurrentQty %d\n", gstProcItem.ulCurrentQty);
			printf("ulForQty %d\n", gstProcItem.ulForQty);*/
		}

		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		/*printf("\nCASE 5");
		printf("\nItem Struct\n");
		printf("uchQtyUnit %d\n", gstItemStruct.stItemRecord.uchQtyUnit);
		printf("ulPrice1 %ld\n", gstItemStruct.stItemRecord.ulPrice1);
		printf("ulQtySold %ld\n", gstItemStruct.ulQtySold);
		printf("ulExtendAmt %ld\n", gstItemStruct.ulExtendAmt);
		printf("ulQtyMkdn %ld\n", gstItemStruct.ulQtyMkdn);
		printf("lAmtMkdn %ld\n", gstItemStruct.lAmtMkdn);

		printf("\nProc Item\n");
		printf("lCurrentPrice %d\n", gstProcItem.lCurrentPrice);
		printf("lQtyMkdn %d\n", gstProcItem.lQtyMkdn);
		printf("lRegularPrice %d\n", gstProcItem.lRegularPrice);
		printf("ulCurrentQty %d\n", gstProcItem.ulCurrentQty);
		printf("ulForQty %d\n", gstProcItem.ulForQty);*/
		break;
	case 6:
		/*printf("\nCASE 6");
		printf("\nItem Struct\n");
		printf("uchQtyUnit %d\n", gstItemStruct.stItemRecord.uchQtyUnit);
		printf("ulPrice1 %ld\n", gstItemStruct.stItemRecord.ulPrice1);
		printf("ulQtySold %ld\n", gstItemStruct.ulQtySold);
		printf("ulExtendAmt %ld\n", gstItemStruct.ulExtendAmt);
		printf("ulQtyMkdn %ld\n", gstItemStruct.ulQtyMkdn);
		printf("lAmtMkdn %ld\n", gstItemStruct.lAmtMkdn);

		printf("\nProc Item\n");
		printf("lCurrentPrice %d\n", gstProcItem.lCurrentPrice);
		printf("lQtyMkdn %d\n", gstProcItem.lQtyMkdn);
		printf("lRegularPrice %d\n", gstProcItem.lRegularPrice);
		printf("ulCurrentQty %d\n", gstProcItem.ulCurrentQty);
		printf("ulForQty %d\n", gstProcItem.ulForQty);*/
		break;
	case 7:
		/*printf("\nCASE 7");
		printf("\nItem Struct\n");
		printf("uchQtyUnit %d\n", gstItemStruct.stItemRecord.uchQtyUnit);
		printf("ulPrice1 %ld\n", gstItemStruct.stItemRecord.ulPrice1);
		printf("ulQtySold %ld\n", gstItemStruct.ulQtySold);
		printf("ulExtendAmt %ld\n", gstItemStruct.ulExtendAmt);
		printf("ulQtyMkdn %ld\n", gstItemStruct.ulQtyMkdn);
		printf("lAmtMkdn %ld\n", gstItemStruct.lAmtMkdn);

		printf("\nProc Item\n");
		printf("lCurrentPrice %d\n", gstProcItem.lCurrentPrice);
		printf("lQtyMkdn %d\n", gstProcItem.lQtyMkdn);
		printf("lRegularPrice %d\n", gstProcItem.lRegularPrice);
		printf("ulCurrentQty %d\n", gstProcItem.ulCurrentQty);
		printf("ulForQty %d\n", gstProcItem.ulForQty);*/
		break;
	case 8:
		/*printf("\nCASE 8");
		printf("\nItem Struct\n");
		printf("uchQtyUnit %d\n", gstItemStruct.stItemRecord.uchQtyUnit);
		printf("ulPrice1 %ld\n", gstItemStruct.stItemRecord.ulPrice1);
		printf("ulQtySold %ld\n", gstItemStruct.ulQtySold);
		printf("ulExtendAmt %ld\n", gstItemStruct.ulExtendAmt);
		printf("ulQtyMkdn %ld\n", gstItemStruct.ulQtyMkdn);
		printf("lAmtMkdn %ld\n", gstItemStruct.lAmtMkdn);

		printf("\nProc Item\n");
		printf("lCurrentPrice %d\n", gstProcItem.lCurrentPrice);
		printf("lQtyMkdn %d\n", gstProcItem.lQtyMkdn);
		printf("lRegularPrice %d\n", gstProcItem.lRegularPrice);
		printf("ulCurrentQty %d\n", gstProcItem.ulCurrentQty);
		printf("ulForQty %d\n", gstProcItem.ulForQty);*/
		break;
	default:
		break;
	}


	vRestore_Stack_For_User_Return();
	return (USER_EXIT_RETURN);
}
