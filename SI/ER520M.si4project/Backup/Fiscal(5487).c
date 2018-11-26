#define FISCAL_C
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "Message.h"

/***************************************************************/

void AddReceiptNumber()
{
	BCD temp;
	short i;

    if (!ApplVar.FInv && ApplVar.FCanc != 2 && !ApplVar.FProforma)
    {
		if(ApplVar.FTrain)
			i = RECNUM;
		else
			i = 0;
		temp = ZERO;
		if(TESTBIT(ApplVar.ECRRGFLAG, BIT0)){//jdb2018-07-26 为退货票据
			memcpy(temp.Value, ApplVar.ReturnNumber, RETNUM);
			Add(&temp, (BCD*)&ONE);
			memcpy(ApplVar.ReturnNumber, temp.Value, RETNUM);
			//RESETBIT(ApplVar.ECRRGFLAG, BIT0);
		}else{
			memcpy(temp.Value, ApplVar.ReceiptNumber+i, RECNUM);
			Add(&temp, (BCD*)&ONE);
			memcpy(ApplVar.ReceiptNumber+i, temp.Value, RECNUM);
		}
    }
}
//Get the index of clerk and convert it into ascii string for print.
void GetReceiptNumber(char *sp)
{
	BYTE l;
	char *to = sp;//jdb2018-07-26

	{
	    if ((CHARSET & 0xf0) == 0x10 || (SLIP != 4 && ((CHARSET & 0x0f) == 3 || (CHARSET & 0x0f) == 7)))  /* number sign not available */
			*to = 'N';
	    else
	    {//jdb2018-07-26 增加RECNUMPRE
	    	if(!TESTBIT(ApplVar.ECRRGFLAG, BIT0)){
	    		strcpy(to, RECNUMPRE);
				to += strlen(to);
	    	}else{//jdb2018-07-31 退货加入票据号前缀
				strcpy(to, RETNUMPRE);
				to += strlen(to);
			}
			
	    	*to = '#';
	    }
		if (TESTBIT(KPPRICE, BIT7))
			l = 1;
		else
			l = RECNUM; //jdb2018-07-26
			//l = 3;
		if (ApplVar.FTrain)
		    HEXtoASC(to + 1, ApplVar.ReceiptNumber+RECNUM, l);
		else if(TESTBIT(ApplVar.ECRRGFLAG, BIT0))
			HEXtoASC(to + 1, ApplVar.ReturnNumber, l);//jdb2018-07-26 退货票据号
		else if(TESTBIT(ApplVar.ECRRGFLAG, BIT2))//jdb2018-07-27 上一次Z报表记录的票据号
			HEXtoASC(to + 1, ApplVar.ZReceiptNumber, l);
		else
		    HEXtoASC(to + 1, ApplVar.ReceiptNumber, l);
	}
}

