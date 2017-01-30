__attribute__ ((unused))        static char *SVNid = "DTPLSVN$Id: EOS_Processing.c 20586 2010-10-20 12:54:45Z bine $";

/*----------------------------------------------------------------------
 *  Copyright by Wincor Nixdorf International GmbH, 33094 Paderborn
 *  Germany, 2005 - 2010
 *  All rights reserved.
----------------------------------------------------------------------*/

/*
 *  Holds sUser_EOS_Processing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "calypso/db_def.h"
#include "calypso/db_empl.h"
#include "calypso/db_plu.h"
#include "calypso/db_move.h"
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

#include "calypso/events.h"
#include "calypso/posdef.h"
#include "calypso/pospi.h"
#include "calypso/structs.h"
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
    Description  : sUser_EOS_Processing() - This function is where a user
                   would link into the system with their own functions if
                   the user wanted to gain control at a End Of Sale.
                   User Exits are supplied at the following positions
                   in the processing of End Of Sale.

                   13. Upon entry into EOS. i.e. Set_Up_For_EOS.
                       13 is NOT a spelling mistake !!

                   2. Prior to displaying Change.

                   3. Prior to Final EOS Processing.

                   4. Immediately before going back to the event handler.

                   8.  When leaving EOS.

    Comments     : This is called from the event handler. EVENT 72

    Arguments    : None

    Returns      : USER_EXIT_RETURN The User must exit to the event handler with
                   a return(USER_EXIT_RETURN) after calling the function
                   Restore_Stack_For_User_Exit. 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
short sUser_EOS_Processing(void)
{
  calprint("V1",CLP_POS_USEX, "userexit", "sUser_EOS_Processing US=<%d>",
    gstUserSave.uchUserPosition);
  /*
	//clear keyboard display before showing new message
	vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_ONE, "","");
	vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_TWO, "","");
	vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_THREE, "","");
	vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_FOUR, "","");

	//display message on keyboard display
	vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_ONE, "COD ITEM:","");
	vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_TWO, gstItemStruct.stItemRecord.szNmbr,"");
	vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_THREE, 1,"");
	vFour_Line_Oper_Disp(NO_SAVE, DISP_LINE_FOUR, HIT_CLEAR,"");

	//copy message to be displayed on customer display too
	strcpy(gstDispStr.szErrorString,1);
	strcpy(gstDispStr.szErrorString2,HIT_CLEAR);

	//wait until operator press clear
	while( EXT_CLEAR != sWait_Ent_Clear())
	{
		//since below method resets some displays, we need to write it again
		strcpy(gstDispStr.szErrorString,gstUserSave.uchUserPosition);
		strcpy(gstDispStr.szErrorString2,HIT_CLEAR);
	}
*/
  (void)sprintf(gstDispStr.szErrorString, "Position: <%d>", gstUserSave.uchUserPosition);
	sDo_Error(EDIT_CLEAR_ONLY, STANDARD_BEEP);
  printf("\n*****Position: <%d>", gstUserSave.uchUserPosition);
  vRestore_Stack_For_User_Return();
  return (USER_EXIT_RETURN);
}
