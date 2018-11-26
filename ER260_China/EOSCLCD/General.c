#define GENERAL_C 4
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "Message.h"

void CheckBreakKey()
{
	BYTE key;


	if (KbHit())
    {
		key = Getch();
		if (key > 65)       /* central or clerk lock */
		{
		   	if (key < NOCLERK)      /* central Lock */
		   	{
			    ApplVar.CentralLock = key;
				while(ApplVar.CentralLock == OFF)       /* lock off then wait */
				{
				    key = Getch();
				    if (key > 65 && key < NOCLERK)
					    ApplVar.CentralLock = key;
				}
		     }
		     else
		     {
			     if (key == NOCLERK)
				 {
					 if (ApplVar.ClerkLock)
				     {
						 ApplVar.ClerkLock = 0;
						 ApplVar.SaveClerk = 0;
				     }
				 }
				else if (key != ApplVar.ClerkLock)
				{
				     ApplVar.ClerkLock = key;
				     ApplVar.SaveClerk = key - NOCLERK;
				     if (ApplVar.SaveClerk > ApplVar.AP.Clerk.Number)
				     {
						 ApplVar.SaveClerk = 0;
						 ApplVar.ClerkLock = 0;
				     }
				 }
		    }
		 }
		 else if (key == ApplVar.AP.FirmKeys[ID_CLEAR] && ApplVar.FReport != Z && !ApplVar.RepComputer)  /* clear is break */
		 {
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI27);   /* user break*/
		    return;
		 }
    }
}

BYTE ReadTotal()
{
#if NETVATVERSION //jdb2018-08-11
	if (ApplVar.BufCC && !ApplVar.FNetAmt) //jdb2018-03-29
#else
    if (ApplVar.BufCC)     /* reset Customer count added ?*/
#endif
    {//Z报表数据清0
		ReadRam(&ApplVar.Total.Qty.Sign, 1);
		//return;
		return 0x01;//jdb2018-05-22
    }
    memset(&ApplVar.Total, 0, sizeof(ApplVar.Total));
    ReadRam((BYTE*)&ApplVar.Total.Qty, ApplVar.Size.Qty + 1);
    if (!TESTBIT(ApplVar.Total.Qty.Sign, BIT2))    /* already used then clear by not read */
    {//BIT2=0,各项统计数据为0
		ApplVar.Total.Qty = ZERO;
		//return;
		return 0x02;//jdb2018-05-22 为零返回0x02
    }
    if (ApplVar.Size.Cc)
		ReadRam(ApplVar.Total.Cc.Value, ApplVar.Size.Cc);
    if (ApplVar.Size.Amt)
    {
		ReadRam(ApplVar.Total.Amt.Value, ApplVar.Size.Amt);
		if (TESTBIT(ApplVar.Total.Qty.Sign, BIT6))     /* qty negative ? */
		    SETBIT(ApplVar.Total.Amt.Sign, BIT7);
    }
    if (ApplVar.Size.RetQty)
    {
		ReadRam(ApplVar.Total.RetQty.Value, ApplVar.Size.RetQty);
		ApplVar.Total.RetQty.Sign = ApplVar.Total.Qty.Sign & 0x03;    /* same decimal */
    }
    if (ApplVar.Size.Disc)
    {
		ReadRam(ApplVar.Total.Disc.Value, ApplVar.Size.Disc);
		if (TESTBIT(ApplVar.Total.Qty.Sign, BIT4))     /* disc negative ? */
		    SETBIT(ApplVar.Total.Disc.Sign, BIT7);
    }
    if (ApplVar.Size.Cost)
    {
		ReadRam(ApplVar.Total.Cost.Value, ApplVar.Size.Cost);
		if (TESTBIT(ApplVar.Total.Qty.Sign, BIT5))     /* cost  negative ? */
		    SETBIT(ApplVar.Total.Cost.Sign, BIT7);
    }
		
		return 0x0;
}

#if 0
void WriteTotal()
{
    //if (ApplVar.BufCC)     /* reset Customer count added ?*/
    if (ApplVar.BufCC && !ApplVar.BufCG) //jdb2018-03-28 GRAND 不清零
    {
		RESETBIT(ApplVar.Total.Qty.Sign, BIT3);
		WriteRam(&ApplVar.Total.Qty.Sign, 1);    /* write back sign */
		return;
    }
    if (TESTBIT(ApplVar.Total.Amt.Sign, BIT7))
		SETBIT(ApplVar.Total.Qty.Sign, BIT6);
    else
		RESETBIT(ApplVar.Total.Qty.Sign, BIT6);
    if (TESTBIT(ApplVar.Total.Cost.Sign, BIT7))
		SETBIT(ApplVar.Total.Qty.Sign, BIT5);
    else
		RESETBIT(ApplVar.Total.Qty.Sign, BIT5);
    if (TESTBIT(ApplVar.Total.Disc.Sign, BIT7))
		SETBIT(ApplVar.Total.Qty.Sign, BIT4);
    else
		RESETBIT(ApplVar.Total.Qty.Sign, BIT4);
    SETBIT(ApplVar.Total.Qty.Sign, BIT2 + BIT3);   /* set bit 4 & 5 to indicate used */
    WriteRam((BYTE*)&ApplVar.Total.Qty, ApplVar.Size.Qty + 1);
    if (ApplVar.Size.Cc)
		WriteRam(ApplVar.Total.Cc.Value, ApplVar.Size.Cc);
    if (ApplVar.Size.Amt){
		if(ApplVar.BufCG){ //jdb2018-03-28 NEW GRAND = ZERO
			ApplVar.Total.Amt = ZERO;
			WriteRam(ApplVar.Total.Amt.Value, ApplVar.Size.Amt);
		}else{
			WriteRam(ApplVar.Total.Amt.Value, ApplVar.Size.Amt);
		}
    }
    if (ApplVar.Size.RetQty){
		if(ApplVar.BufCG){ //jdb2018-03-28 OLD GRAND + NEW GRAND
			Add(&ApplVar.Total.RetQty, &ApplVar.Total.Amt);
			WriteRam(ApplVar.Total.RetQty.Value, ApplVar.Size.RetQty);
		}else{
			WriteRam(ApplVar.Total.RetQty.Value, ApplVar.Size.RetQty);
		}
    }
    if (ApplVar.Size.Disc)
		WriteRam(ApplVar.Total.Disc.Value, ApplVar.Size.Disc);
    if (ApplVar.Size.Cost){
		if(ApplVar.BufCG){ //jdb2018-03-28 NET AMOUNT = ZERO
			ApplVar.Total.Cost = ZERO;
			WriteRam(ApplVar.Total.Cost.Value, ApplVar.Size.Cost);
		}else{
			WriteRam(ApplVar.Total.Cost.Value, ApplVar.Size.Cost);
		}
    }
}
#else
void WriteTotal()
{
#if NETVATVERSION //jdb2018-08-11
	if (ApplVar.BufCC && !ApplVar.FNetAmt) //jdb2018-03-29
#else
    if (ApplVar.BufCC)     /* reset Customer count added ?*/
#endif
    {
		RESETBIT(ApplVar.Total.Qty.Sign, BIT3);
		WriteRam(&ApplVar.Total.Qty.Sign, 1);    /* write back sign */
		return;
    }
    if (TESTBIT(ApplVar.Total.Amt.Sign, BIT7))
		SETBIT(ApplVar.Total.Qty.Sign, BIT6);
    else
		RESETBIT(ApplVar.Total.Qty.Sign, BIT6);
    if (TESTBIT(ApplVar.Total.Cost.Sign, BIT7))
		SETBIT(ApplVar.Total.Qty.Sign, BIT5);
    else
		RESETBIT(ApplVar.Total.Qty.Sign, BIT5);
    if (TESTBIT(ApplVar.Total.Disc.Sign, BIT7))
		SETBIT(ApplVar.Total.Qty.Sign, BIT4);
    else
		RESETBIT(ApplVar.Total.Qty.Sign, BIT4);
    SETBIT(ApplVar.Total.Qty.Sign, BIT2 + BIT3);   /* set bit 4 & 5 to indicate used */
    WriteRam((BYTE*)&ApplVar.Total.Qty, ApplVar.Size.Qty + 1);
    if (ApplVar.Size.Cc)
		WriteRam(ApplVar.Total.Cc.Value, ApplVar.Size.Cc);
    if (ApplVar.Size.Amt){
		WriteRam(ApplVar.Total.Amt.Value, ApplVar.Size.Amt);
    }
    if (ApplVar.Size.RetQty){
		WriteRam(ApplVar.Total.RetQty.Value, ApplVar.Size.RetQty);
    }
    if (ApplVar.Size.Disc)
		WriteRam(ApplVar.Total.Disc.Value, ApplVar.Size.Disc);
    if (ApplVar.Size.Cost){
		WriteRam(ApplVar.Total.Cost.Value, ApplVar.Size.Cost);
    }
}

#endif

void PrintMultiply()
{
    BYTE saveprint;

    if (!ApplVar.MultiplyCount)     /* if not used no print */
		return;
    if (ApplVar.MultiplyCount == 1 && !CompareBCD(&ApplVar.Qty, (BCD*)&ONE))
		return;     /* don't print qty of one */
    saveprint = ApplVar.PrintLayOut;
    if (!TESTBIT(SLIPINFO, BIT0))
		ApplVar.PrintLayOut &= 0xe7;
    MemSet(SysBuf, sizeof(SysBuf), ' ');
    SysBuf[sizeof(SysBuf) - 1] = 0;
    if (ApplVar.MultiplyCount > 2)
    {
#if PRTLEN == 18
		FormatQty(SysBuf + sizeof(SysBuf) - 12, &ApplVar.Qty1);
		SysBuf[sizeof(SysBuf) - 10] = 'X';
		PrintStr(SysBuf + sizeof(SysBuf) - 1 - PRTLEN);
		MemSet(SysBuf, sizeof(SysBuf), ' ');
		SysBuf[sizeof(SysBuf) - 1] = 0;
#endif
		FormatQty(SysBuf + sizeof(SysBuf) - 4, &ApplVar.Qty3);
		SysBuf[sizeof(SysBuf) - 10] = 'X';
    }
	if (ApplVar.MultiplyCount > 1)
	{
		FormatQty(SysBuf + sizeof(SysBuf) - 12, &ApplVar.Qty2);
#if PRTLEN == 18
		if (ApplVar.MultiplyCount == 2)
#endif
		{
		    SysBuf[sizeof(SysBuf) - 18] = 'X';
		    FormatQty(SysBuf + sizeof(SysBuf) - 20, &ApplVar.Qty1);
		}
		if (ApplVar.MultiplyCount > 2)
		    SysBuf[sizeof(SysBuf) - 2] = '=';
		else
		    SysBuf[sizeof(SysBuf) - 10] = '=';

#if PRTLEN == 18
		if (ApplVar.MultiplyCount ==2)
		    PrintStr(SysBuf + sizeof(SysBuf) - 1 - PRTLEN - 8);
		else
#endif
			PrintStr(SysBuf + sizeof(SysBuf) - 1 - PRTLEN);
		MemSet(SysBuf, sizeof(SysBuf), ' ');
		SysBuf[sizeof(SysBuf) - 1] = 0;
    }
#if PRTLEN == 18
    FormatQty(SysBuf + sizeof(SysBuf) - 14, &ApplVar.Qty);
    FormatAmt(SysBuf + sizeof(SysBuf) - 3, &ApplVar.Price);
    SysBuf[sizeof(SysBuf) - 12] = 'X';
#else
    FormatQty(SysBuf + sizeof(SysBuf) - 16, &ApplVar.Qty);
    FormatAmt(SysBuf + sizeof(SysBuf) - 4, &ApplVar.Price);
    SysBuf[sizeof(SysBuf) - 14] = 'X';
#endif
	if (SysBuf[sizeof(SysBuf) - 1 - PRTLEN + 4] == ' ')
	    PrintStr(SysBuf + sizeof(SysBuf) - 1 - PRTLEN + 5);
	else
	    PrintStr(SysBuf + sizeof(SysBuf) - 1 - PRTLEN);
   	 ApplVar.PrintLayOut = saveprint;
}


void PrintSaleQty()
{
    if (!PRN_QTY_ITEMS || !ApplVar.FReceipt)
		return;
    if (CheckNotZero(&ApplVar.SaleQty))    /* print item counter */
    {
		if (ApplVar.FPb)
		    ApplVar.PrintLayOut &= 0x02;
		else
		    ApplVar.PrintLayOut = 0x02;     /* set receipt */
 //		FormatQtyStr(ApplVar.TXT.Caption[25], &ApplVar.SaleQty, 14);
		PrintStr(FormatQtyStr(Msg[XIANGSHU].str, &ApplVar.SaleQty, PRTLEN));
//ccr2016-03-03		DispStrLF(FormatQtyStr(Msg[XIANGSHU].str, &ApplVar.SaleQty, SCREENWD));
    }
}

void PrintMessage(WORD number)
{
    if (Msg[number].str[0])
		PrintStr(Msg[number].str);
}
// add by lqw ,to print GPRS Messages...
#if(defined(CASE_GPRS))
void PrintGprsMessage(WORD number)
{
	if(GPRSMess[number].str[0])
		PrintStr(GPRSMess[number].str);
}
#endif
void PrintPbTrailer()
{
    BYTE saveprint;
    short i;

    if (ApplVar.PbNumber && TESTBIT(ApplVar.AP.Pb.Random, BIT7)
		    && TESTBIT(ApplVar.PrintLayOut, BIT1)
			&& TESTBIT(ApplVar.FNoPb, BIT2))
    {
		saveprint = ApplVar.PrintLayOut;
		ApplVar.PrintLayOut = 0x02;
	  	RamOffSet = (ApplVar.AP.Pb.Random & 0x0f) + ApplVar.AP.Pb.Text + ApplVar.AP.Pb.AmtSize + 6;
	    if (TESTBIT(PBINFO, BIT7))  /* Discount item stored ? */
			RamOffSet += ((ApplVar.AP.Pb.AmtSize + 1) * 2);
	    RamOffSet = RamOffSet * ApplVar.AP.Pb.NumberOfPb + ApplVar.AP.StartAddress[AddrPBt];      /*Start Pb Trailer */
		SysBuf[24] = 0;
		for (i = 0; i < 50; i++)
		{
		    ReadRam(SysBuf, 24);     /* read Line */
		    if (!SysBuf[0])     /* end line ? */
				break;
		    PrintRJ(SysBuf);
		}
		ApplVar.PrintLayOut = saveprint;
    }
}

void GetWordEntry()
{
	UnLong TempL ;
//ccr2014-09-09    ApplVar.NumberEntry = 0;
	if(TESTBIT(ApplVar.INPUTFUN, BIT1)){//jdb2018-07-27 输入无符号整型
		BCDValueToULong(ApplVar.Entry.Value, &TempL);
		ApplVar.LNumberEntry = TempL;
	}
	else if(TESTBIT(ApplVar.INPUTFUN, BIT2)){//jdb2018-08-02 输入浮点数，无最大值限制
	}
    else if (CompareBCD(&ApplVar.Entry, (BCD*)&MAXWORD) < 1)   /* entry < 65535 */
		ApplVar.NumberEntry = (WORD)BCDValueToULong(ApplVar.Entry.Value, &TempL);
    else
        ApplVar.NumberEntry = 65535;//ccr2014-09-09
}

/* add corresponding totals */

void AddTotal()
{
	UnLong    save ;             /* unsigned long save;  */
	BCD Val1;

    if (ApplVar.FTrain && ApplVar.PointerType != 1)
    {
   		RamOffSet += ApplVar.Size.Length; /* next period or total */
		return;         /* only add in clerk when training */
    }
    save = RamOffSet;      /* save OffSet for Writing */
    ReadTotal();
    RamOffSet = save;
#if NETVATVERSION //jdb2018-08-11
	if (ApplVar.BufCC && !ApplVar.FNetAmt) //jdb2018-03-28
#else
    if (ApplVar.BufCC)             /* reset customer added ? customer count ? */
#endif
    {
		WriteTotal();
	    RamOffSet = save + ApplVar.Size.Length;  /* next period or total */
    }
    else
    {
#if NETVATVERSION //jdb2018-08-11
    	if(ApplVar.BufCG){ //jdb2018-03-28 计算GRAND
    		#if 0
    		Add(&ApplVar.Total.RetQty, &ApplVar.Total.Amt);
			Subtract(&ApplVar.Total.RetQty, &ApplVar.Total.Disc);
			#else //jdb2018-03-29
			if(TESTBIT(ApplVar.Total.RetQty.Sign, BIT7)){
				ApplVar.Total.RetQty.Sign = ApplVar.Total.Cost.Sign; //jdb2018-04-04 小数点一样
				SETBIT(ApplVar.Total.RetQty.Sign, BIT7);
			}else{
				ApplVar.Total.RetQty.Sign = ApplVar.Total.Cost.Sign; //jdb2018-04-04 小数点一样
				RESETBIT(ApplVar.Total.RetQty.Sign, BIT7);
			}
			//ApplVar.Total.RetQty.Sign = ApplVar.Total.Cost.Sign; //jdb2018-04-04 小数点一样
			Add(&ApplVar.Total.RetQty, &ApplVar.Total.Cost);
			#endif
			
			ApplVar.Total.Amt = ZERO;
			ApplVar.Total.Disc = ZERO;
			ApplVar.Total.Cost = ZERO;
    	}else if(ApplVar.FNetAmt){ //jdb2018-03-28
    		if (ApplVar.Size.Cost)
		    	Add(&ApplVar.Total.Cost, &ApplVar.Cost);
			//if (ApplVar.Size.Cc) //jdb2018-03-29
			//{
				//if (!ApplVar.FTrvoid && !ApplVar.FCanc) /* already added ?*/
					//Add(&ApplVar.Total.Cc, (BCD*)&ONE);
			//}
		}else
#endif
        {
		if (ApplVar.Size.Cc)
		{
	    	if (!TESTBIT(ApplVar.Total.Qty.Sign, BIT3))
			{
				if (!ApplVar.FTrvoid && !ApplVar.FCanc) /* already added ?*/
					Add(&ApplVar.Total.Cc, (BCD*)&ONE);
			}
		}
		if (ApplVar.Size.Qty)
		{
		    ApplVar.Total.Qty.Sign &= 0x83;         /* skip bits */
		    Add(&ApplVar.Total.Qty, &ApplVar.Qty);
		}
		if (ApplVar.Size.Amt)
		{
	    	Add(&ApplVar.Total.Amt, &ApplVar.Amt);
//			PrintAmt(ApplVar.TXT.Caption[47], &ApplVar.Total.Amt);
		}
		if (ApplVar.Size.RetQty)
		{
#if NETVATVERSION //jdb2018-08-11
			if(ApplVar.Report.Type == REPORT_GRANDTOTAL && ApplVar.PointerType == 0 && ApplVar.Size.Length == 26){ //jdb2018-04-13 OLD GRAND TOTAL  NOT ADD
				#if 0
				Val1 = ApplVar.Amt;
				Multiply(&Val1, &ApplVar.RetQty);
				RoundBcd(&Val1, 0);
			    AmtRound(1, &Val1);
				Val1.Sign = ApplVar.Total.Cost.Sign;
				if (ApplVar.Size.Cost)
		    		Subtract(&ApplVar.Total.Cost, &Val1); //SUBTRACT NEW GRAND
		    	#endif
			}else
#endif
            {
		    	Subtract(&ApplVar.Total.RetQty, &ApplVar.RetQty);   /* no Sign in memory so always subtract !!*/
		    	if (TESTBIT(ApplVar.Total.RetQty.Sign, BIT7))       /* Result negative? ? */
					ApplVar.Total.RetQty = ZERO;
			}
		}
		if (ApplVar.Size.Disc)
		    Add(&ApplVar.Total.Disc, &ApplVar.DiscAmt);
		if (ApplVar.Size.Cost)
		    Add(&ApplVar.Total.Cost, &ApplVar.Cost);
    	}
		WriteTotal();
    }
}

void AddPointerTotal()
{
    WORD skip, next;

    if (ApplVar.FProforma)
		return;
    if (!ApplVar.Size.Periods)
		return;
    if (!ApplVar.Size.Length)     /* active ? */
		return;
    if (ApplVar.FSplit)
		return;             /* don't add when split bill */
    skip = 0;
    if (TESTBIT(ApplVar.Size.Periods, BIT0))      /* calculate size of 1 pointer */
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT1))
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT2))
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT3))
		skip += ApplVar.Size.Length;
    next = skip;
    switch (ApplVar.PointerType)
    {
	case 0://统计总销售
	    skip = 0;
	    next = 0;
	    break;
	case 1:         /* Clerks */
	    if (!ApplVar.AP.Clerk.Number)
			return;
	    next *= (ApplVar.AP.Clerk.Number - ApplVar.ClerkNumber);
	    skip *= (ApplVar.ClerkNumber - 1);
	    break;
	case 2:         /* Hour */
	    if (!ApplVar.AP.Zone.Number)
			return;
	    next *= (ApplVar.AP.Zone.Number - ApplVar.Zone - 1);
	    skip *= ApplVar.Zone;
	    break;
	case 3:         /* day of week */
	    if (!ApplVar.AP.Day.Number)
			return;
	    next *= (ApplVar.AP.Day.Number - ApplVar.Day - 1);
	    skip *= ApplVar.Day;
	    break;
	case 4:         /* ApplVar.Month of Year */
	    if (!ApplVar.AP.Month.Number)
			return;
	    next *= (ApplVar.AP.Month.Number - ApplVar.Month - 1);
	    skip *= ApplVar.Month;
	    break;
	case REPDEFMAX-1:
#if (salNumber)
	    if (ApplVar.AP.SalPer.Number)
		{
			next *= (ApplVar.AP.SalPer.Number - ApplVar.SalPerNumber);
		    skip *= (ApplVar.SalPerNumber - 1);
            break;
		}
        else
            return;
#else
        return;
#endif
	default:
	    break;
    }
   	RamOffSet += skip;
#if NETVATVERSION //jdb2018-08-11
	if(ApplVar.PointerType == 0) //jdb2018-04-09
	{
		if(!TESTBIT(ApplVar.FREP, BIT5)){ //jdb2018-07-03 非折扣报表
			if(CheckNotZero(&ApplVar.NetVatAmt))
				Subtract(&ApplVar.Amt, &ApplVar.NetVatAmt); //由于免税后按取消键减去了NetVatAmt CORREC
		
			if(ApplVar.FVoid && CheckNotZero(&ApplVar.LastNetVatAmt))
				Subtract(&ApplVar.Amt, &ApplVar.LastNetVatAmt); //jdb2018-04-10 VOID
		}
	}
#endif
    if (TESTBIT(ApplVar.Size.Periods, BIT0))
		AddTotal();
    if (TESTBIT(ApplVar.Size.Periods, BIT1)){
#if NETVATVERSION //jdb2018-08-11
		if(ApplVar.BufCG == 1){ //jdb2018-04-26 PERIOD REPORT 不清GRAND
		}else
#endif
        {
		AddTotal();
		}
    	}
    if (TESTBIT(ApplVar.Size.Periods, BIT2))
		AddTotal();
    if (TESTBIT(ApplVar.Size.Periods, BIT3))
		AddTotal();
	RamOffSet += next;     /* start next pointer totals */

#if NETVATVERSION //jdb2018-08-11
	if(ApplVar.PointerType == 0){ //jdb2018-04-03
		if(ApplVar.Size.Length == 26){ //jdb2018-04-13 为totalRecordSize ---0
			ApplVar.FNetAmt = 0;
		}
		if(!TESTBIT(ApplVar.FREP, BIT5)){ //jdb2018-07-03 非折扣报表
			if(CheckNotZero(&ApplVar.NetVatAmt))
				Add(&ApplVar.Amt, &ApplVar.NetVatAmt); //jdb2018-04-09
			if(ApplVar.FVoid && CheckNotZero(&ApplVar.LastNetVatAmt))
				Add(&ApplVar.Amt, &ApplVar.LastNetVatAmt); //jdb2018-04-10
		}
	}
#endif
}

void ClearEntry(BYTE disp)
{
    MemSet(EntryBuffer, ENTRYSIZE, ' ');
    if (!TESTBIT(ApplVar.MyFlags,HANZIMODE))
	{
      if (ApplVar.CentralLock==RG ||  ApplVar.CentralLock==MG)
      {
        AtEntryBuffer(4) = '0';
        AtEntryBuffer(3) = '.';
        AtEntryBuffer(2) = '0';
      }
      AtEntryBuffer(1) = '0';
	}

	AtEntryBuffer(0) = 0;
    Appl_EntryCounter = 0;
	Editor.EditCount = 0;
	Editor.EditOffs = 0;
    ApplVar.DecimalPoint = 0;
    if (disp)
    {
        LCD10_SetFlag(FLAG_NULL);
    	PutsO(EntryBuffer+ENTRYSIZE-DISLEN-1);//C-DISPLAY
#if (SCRSTATDBL)
        if (ApplVar.CentralLock==RG ||ApplVar.CentralLock==MG)
            DispOnStatDBL(EntryBuffer+ENTRYSIZE-SCRDBLWD-1,0);
        else
#endif
            DispSt2(EntryBuffer+ENTRYSIZE-INPUTWIDTH-1);//RG-DISPLAY

    }
}

/**********************************************************
@Para: (max / 10) is digit and (max % 10) is number of zeroes:
    89:表示最大数据为8000000000
@return: 0-Data in the range;1-data out of range
********************************************************/
short CheckMaxEntry(BCD *amt, BYTE max)
{
    BCD mx;
    BYTE i;

    if (!max)       /* not active */
		return 0;

    mx = ZERO;
    i = (max % 10) / 2;
    mx.Value[i] = max / 10;
    if (max % 2)
		mx.Value[i] *= 0x10;
    if (CompareBCD(amt, &mx) < 1)        /* amt <= max */
		return 0;
    else
		return 1;
}


/* When "err" is non zero only display "err" else */
/* display ApplVar.ErrorNumber */
void CheckError(BYTE err)
{
    static BYTE errLast=0;
    char dbuf[SCREENWD+1];
    short  erX,erY;


	if (!err)
    {
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
        if (!ApplVar.ErrorNumber)
        {
            if (TESTBIT(ApplVar.EJVar.ContFlag,EJWILLFULL))
            {
                if (!ApplVar.FRegi)//没有打开收据时
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI96);              // 40 KB left, only warn once
                    RESETBIT(ApplVar.EJVar.ContFlag,EJWILLFULL);
                }
            }
            else if (TESTBIT(ApplVar.EJVar.ContFlag,EJISFULL))
            {
                //启用此行会导致无法继续操作收款机 ApplVar.ErrorNumber=ERROR_ID(CWXXI97);
                if (ApplVar.FRegi)//允许完成结账
                    RESETBIT(ApplVar.EJVar.ContFlag,EJISFULL);
            }
        }
#endif
		err = ApplVar.ErrorNumber;
    }

	if (err==255)
	{//报警
		Bell(0);
		if (ApplVar.ErrorNumber ==255)
			ApplVar.ErrorNumber=0;
	}
	else
    {
        if (errLast==err)
        {
            if (err && GetTimerDelay1()==0)
            {//相同的错误号,只报警
                SetTimerDelay1(500);
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
                //EJ的错误信息被菜单信息覆盖,因此在状态行显示一次
                if (ECRMode()!='R')
                {
                    ClsState12();
                    DispSt2((char*)Msg[CWXXI01+err-1].str);
                }
#endif
                Bell(1);
            }
        }
        else
        {
            erY = (SCREENLN-1-3)/2;
            if (err==0)
            {
                if (errLast)
                {//恢复屏幕内容
                    mDrawABox(0,0);//mRefreshLine(erY++); mRefreshLine(erY++); mRefreshLine(erY);
                }
            }
            else
            {//显示出错信息,占用屏幕三行
                mDrawABox(erY,erY+2);

                mDispStringXY((char*)Msg[CHUCUO].str,CenterForDisplay(SCREENWD-2,strlen(Msg[CHUCUO].str))+1,erY);
                if (err>0 && err<=ERROR_ID(CWXXIMAX))
                {
                    mDispStringXY((char*)Msg[CWXXI01+err-1].str,CenterForDisplay(SCREENWD-2,strlen(Msg[CWXXI01+err-1].str))+1,erY+1);
                }
                else
                {//没有对应的错误代码,显示错误号
                    memset(dbuf,0,SCREENWD);
                    CopyFrStr(dbuf,Msg[CHUCUO].str);
                    WORDtoASCL(dbuf, err);
                    dbuf[SCREENWD-2]=0;
                    mDispStringXY(dbuf,SCREENWD/2-1,erY+1);
                }
                SetTimerDelay1(500);
                Bell(1);
            }
            errLast = err;
        }
    }

}
/**
 * 显示提示信息
 *
 * @author EutronSoftware (2016-03-11)
 *
 * @param msg
 */
void DisplayPrompt(char *msg)
{
    short  erY;

    erY = (SCREENLN-1-3)/2;
    mDrawABox(erY,erY+2);

    mDispStringXY((char*)Msg[CHUCUO].str,CenterForDisplay(SCREENWD-2,strlen(Msg[CHUCUO].str))+1,erY);
    mDispStringXY(msg,CenterForDisplay(SCREENWD-2,strlen(msg))+1,erY+1);
    Bell(1);
    WAIT_INPUT(erY);
    // mDrawABox(0,0);
}

WORD RegiInit()
{
    if (ApplVar.FCanc)
		ApplVar.PrintLayOut &= ApplVar.CancPrint;
    if (!ApplVar.FRegi)   /* already in registration ? */
    {
		if (!ApplVar.ClerkNumber)
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI33);       /* select clerk */
		    return 1;
		}
#if (salNumber)
		if (ApplVar.AP.SalPer.Number && !ApplVar.SalPerNumber)
		{
			if (!TESTBIT(KEYTONE, BIT6))
			{
				ApplVar.SalPerNumber = 1;
			}
			else
			{
			    ApplVar.ErrorNumber=ERROR_ID(CWXXI34);	    /* select salesperson */
			    return 1;
			}
		}
#endif
		ApplVar.SlipPage = 0;
		if (CheckSlip())
		    return 1;
		Now.sec = 0x61;     /* force read of new time */
		CheckTime(FALSE);        /* read correct time */
		GetTimeZones();      /* set up correct time, day, month */
		if (ApplVar.FPb && TESTBIT(PBINFO, BIT4))
		{
		}
		else
		    AddReceiptNumber();
		ApplVar.SaleAmt = ZERO;
		ApplVar.SaleQty = ZERO;
		MemSet(ApplVar.TaxItem, sizeof(ApplVar.TaxItem), 0);
		MemSet(ApplVar.DiscItem, sizeof(ApplVar.DiscItem), 0);
		ApplVar.CopyReceipt = ApplVar.RGItems.RGNumber = 0;    /* reset trans buffer */
		ApplVar.NoVatRGItems.RGNumber = 0; //jdb2018-04-08
		//jdb2018-04-12 初始化免税相关参数
		ApplVar.LastNetVatAmt = ZERO;
		ApplVar.NetVatAmt = ZERO;
		ApplVar.LastVatExempt = ZERO; //jdb2018-06-14
		//jdb2018-04-24 初始化零税，免税标志
		ApplVar.FZV = 0x00;
		ApplVar.ZEROAMOUNT = ZERO;
		ApplVar.ZEROLessVatAmt = ZERO;
		ApplVar.LessVatAmt = ZERO;
		//jdb2018-07-03 初始化ApplVar.MixQty
		ApplVar.MixQty = ZERO;
#if (salNumber)
		if (ApplVar.AP.SalPer.Number)
		{//1.将营业员存入RGBuf中
		    ApplVar.RGRec.Key.Code = SALPER + ApplVar.SalPerNumber;  /* store in buffer for correct CC update */
			StoreInBuffer();
		}
#endif
		ApplVar.RGRec.Key.Code = CLERK + ApplVar.ClerkNumber;
		ApplVar.FTend = 0;  /* reset incase post-tendering */
		ApplVar.FRegi = 1;
		ApplVar.FNoTax = 0;
		ApplVar.SubDiscFlag = 0;			//cc 2006-10-11 for SubDisc
		SETBIT(ARROWS, BIT1);
		Collect_Data(REGISLOG);
    }
    else if (ApplVar.FTend)  /* in tender ? */
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI17);
		return 1;
    }
    else if (!ApplVar.SlipLines && CheckSlip())
		return 1;
    else if (ApplVar.FBuffer)       /* regi buffer full */
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI15);
		return 1;
    }
    if (!ApplVar.FReceipt && !ApplVar.FInv)   /* receipt already used ? */
    {
		if (TESTBIT(ARROWS, BIT0) &&     /* receipt on ? */
		    TESTBIT(ApplVar.PrintLayOut, BIT1))
		{
			PrintHead1(PREHEADER);
			//ccr2016-11-15 RFeed(1);
		}
    }
    return 0;
}

WORD RegiStart()           /* registration start */
{
    BYTE saveprint;
    int l;
	//jdb2018-03-09
	char tmpbuf[32] = {0};

    saveprint = ApplVar.PrintLayOut;
    ApplVar.PrintLayOut &= 0xfb;        /* skip double height */
    if (RegiInit())
		return 1;
    if (!ApplVar.FReceipt)
    {
		if (ApplVar.FCanc == 1)
	    	PrintMessage(QUXIAOP);
    }
#if (salNumber)
    if (ApplVar.AP.SalPer.Number && ApplVar.SalPerNumber != ApplVar.OldSalPer)    /* print salp when changed ? */
    {
//		if (TESTBIT(ApplVar.SalPer.Options, BIT0))      /* double heigth ? */
//			SETBIT(ApplVar.PrintLayOut, BIT2);
//		PrintStr(ApplVar.SalPer.Name);
	    ApplVar.PrintLayOut &= 0xfb;        /* reset double height */
		ApplVar.OldSalPer = ApplVar.SalPerNumber;
    }
#endif
    if (TESTBIT(CLERKFIX, BIT4) && ApplVar.ClerkNumber != ApplVar.OldClerk)    /* print clerk when changed ? */
    {
		if(TESTBIT(ApplVar.MyFlags, ENSLIPPB))
		{
			RESETBIT(ApplVar.MyFlags, ENSLIPPB);	 //lyq added for slip 20040324
			RESETBIT(ApplVar.PrintLayOut,BIT3);
		}
		if((ApplVar.FPb==0 && (ApplVar.PbF.Options & 0x0f) != 3) ||(ApplVar.PbF.Options & 0x0f) == 0 || ApplVar.PbNumber==0)
		{
#if !defined(DEBUGBYPC)//  打印票头图象  //
#if !defined(CASE_INNERDOTPRN)
			if ((ApplVar.Graph[0].PictNo>0 && ApplVar.Graph[0].PictNo<=GRAPHMAX) && !TESTBIT(ApplVar.MyFlags, PRNONPB+ENPRINTER))
			{
				if (!TESTBIT(ApplVar.MyFlags, ENPRINTER))
					Bios(BiosCmd_PrintGraph, (void*)(ApplVar.Graph[0].PictNo), 1 , 0); //Stampa msg ram
		    	PrintLine('=');
			}
#endif
#endif
//ccr2014-03-31>>>>>>>>>>>>
            if (PRNINDEX)
            {//打印取菜号
                memset(ProgLineMes,' ',PRTLEN);
                CopyFrStr(ProgLineMes,Msg[mPRNINDEX].str);
                l = strlen(Msg[mPRNINDEX].str)+1;
//ccr2016-04-20                l+=WORDtoASCL(ProgLineMes+l,REGISTER);
//ccr2016-04-20                ProgLineMes[l]='-';
//ccr2016-04-20                l++;
                HEXtoASC(ProgLineMes+l, ApplVar.ReceiptNumber, 3);
                ProgLineMes[l+6]=0;
                SETBIT(ApplVar.PrintLayOut, BIT2);//倍高打印
                PrintStr_Center(ProgLineMes,true);
            }
//<<<<<<<<<<<<<<<<<<<<<<<<<
#if defined(CASE_PERU)//ccr2016-10-24>>>>>>>>>>>>>>>
            PrintCustomerMessage();
#endif//ccr2016-10-24<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
            if (TestSaveEJ())
                RefreshEJStart(true);
#endif
			PrintStr(Msg[TITLE].str);
#if NETVATVERSION //jdb2018-08-11
			if(TESTBIT(ApplVar.ECRRGFLAG, BIT0)){//jdb2018-07-26 打印要退货的票据号
				memset(SysBuf,' ', PRTLEN);
				strcpy(SysBuf, REFNUM);
				SysBuf[strlen(SysBuf)] = ' ';
				HEXtoASC(SysBuf +  PRTLEN -8, ApplVar.REFECRNUM.Value, RECNUM);
				PrintStr(SysBuf);
			}
			//jdb2018-03-09
			if(TESTBIT(ECRVIP.ECRVIPState, BIT4)){
				l = HexToLong64(ECRVIP.VipFix.VipTEL, strlen((char *)ECRVIP.VipFix.VipTEL));
				sprintf(tmpbuf, "%s%d", ECRVIP.VipFix.VipName, l);
				PrintStr(tmpbuf);
			}
#endif
//20130709分割线没有多大的意义，客户反馈也浪费纸	    	PrintLine('=');
		}
		ApplVar.OldClerk = ApplVar.ClerkNumber;
    }
    ApplVar.PrintLayOut = saveprint;    /* restore print construction */

    return 0;
}

void PromotionBeep()
{
	short i;
	for (i=0;i<5;i++)
	{
		memset(ProgLineMes,'8',DISLEN+1);
		PutsO(ProgLineMes);//C-DISPLAY
		Bell(1);
		Delay(ASECOND/2);
		memset(ProgLineMes,' ',DISLEN+1);
		PutsO(ProgLineMes);//C-DISPLAY
		Delay(ASECOND/2);
	}
	memset(ProgLineMes,'8',DISLEN+1);
	PutsO(ProgLineMes);//C-DISPLAY
}

void PromtionCheck()
{
	BCD	sVal1,sVal2;

	if (!TESTBIT(PROMOTION,BIT0))
	{
		sVal1 = ZERO;
		memcpy(sVal1.Value,ApplVar.AP.Promotion.JollyMin,sizeof(ApplVar.AP.Promotion.JollyMin));
	    if (CompareBCD(&ApplVar.SubTotal,&sVal1) >= 0)
		    ApplVar.PromCounter++;
	    if (ApplVar.PromCounter==ApplVar.AP.Promotion.Freq)
	    {
#if !defined(CASE_INNERDOTPRN)
	    	if (ApplVar.AP.Promotion.GrapIdx>0 && !TESTBIT(ApplVar.MyFlags, ENPRINTER))
				Bios(BiosCmd_PrintGraph, (void*)(ApplVar.AP.Promotion.GrapIdx), 1 , 0); //Stampa msg ram
#endif
			RJPrint(0,ApplVar.AP.Promotion.Memo);
	    	ApplVar.PromCounter = 0;
	    	PromotionBeep();
	    }
    }
	if (!TESTBIT(PROMOTION,BIT1))
	{
		sVal1 = ZERO;
		memcpy(sVal1.Value,ApplVar.AP.Promotion.PointMin,sizeof(ApplVar.AP.Promotion.PointMin));
	    if (CompareBCD(&ApplVar.SubTotal,&sVal1) >= 0)
	    {
			sVal1 = ZERO;
			memcpy(sVal1.Value,ApplVar.AP.Promotion.PointVal,sizeof(ApplVar.AP.Promotion.PointVal));
			sVal2 = ApplVar.SubTotal;
			if (CheckNotZero(&sVal1))
			{
				Divide(&sVal2,&sVal1);
				SetBCDPoint(&sVal2,0);
				PrintRJ(FormatQtyStr(Msg[BCXFEI].str, &sVal2, PRTLEN));

/*				if (CompareBCD(&sVal2,&TEN) >= 0)
				{
			    	if (ApplVar.AP.Promotion.GrapIdx>0 && !TESTBIT(ApplVar.MyFlags, ENPRINTER))
						Bios(BiosCmd_PrintGraph, ApplVar.AP.Promotion.GrapIdx-1, 0 , 0); //Stampa msg ram
					RJPrint(0,ApplVar.AP.Promotion.Memo);
			    	PromotionBeep();
		    	}*/
		    }
	    }
    }
}

void RegiEnd()
{
	BCD sVal2, sVal3;//jdb2018-03-09

    if (PRICE_LEVEL & 0x02)       /* to zero at end of trans action ? */
		ApplVar.PluPriceLevel = 0;
#if defined(ARTLEVEL1) //ccr2016-01-27>>>>
    if ((ART_LEVEL & 0x03) == 2)         /* to zero at end of transaction ? */
		ApplVar.PluArtLevel = 0;
#endif
    ApplVar.TaxPerm = 0;            /* reset perm tax shift */
	ApplVar.FNFolio = 0;
    if (!ApplVar.FSale)   /* no sale then don't print total */
	{
		if (!ApplVar.PbNumber || (ApplVar.PbNumber && !TESTBIT(PBINFO, BIT3)))
		{
			return;
		}
	}
    CalculateTax(2);        /* only print and calculate tax */

#if NETVATVERSION //jdb2018-08-11
	//jdb2018-03-28
	ApplVar.NetVatAmtTotal = ApplVar.SubTotal;
#endif
	#if 0
	if(TESTBIT(ECRVIP.ECRVIPState, BIT4)){ //jdb2018-03-29
		Subtract(&ApplVar.NetVatAmtTotal, &ApplVar.VatAmtTotal);
	}
	//PrintAmt("NET VAT AMT", &ApplVar.NetVatAmtTotal);
	ApplVar.VatAmtTotal = ZERO;
	#endif
	
    //jdb2018-03-09
    #if 0
    if(TESTBIT(ECRVIP.ECRVIPState, BIT4)){
        memset(ProgLineMes, ' ', sizeof(ProgLineMes));
        strcpy(ProgLineMes,"LESS 12% VAT");
        PrintAmt(ProgLineMes, &ApplVar.LessVatTotalAmt);
        PrintLine('.');
        sVal2 = ApplVar.SaleAmt;
        sVal3 = ApplVar.LessVatTotalAmt;
        Add(&sVal2, &sVal3);
        memset(ProgLineMes, ' ', sizeof(ProgLineMes));
        if(strcmp(ECRVIP.VipFix.VipName, "SC#") == 0){
            strcpy(ProgLineMes,"SC VAT EXEMPT SALES");
        }else{
            strcpy(ProgLineMes,"PWD VAT EXEMPT SALES");
        }
        PrintAmt(ProgLineMes, &sVal2);
        sVal3 = ZERO;
        WORDtoBCD(sVal3.Value, 2000);
        sVal3.Sign = 0x04;
        Multiply(&sVal2, &sVal3);
        RoundBcd(&sVal2, 0);
        AmtRound(1, &sVal2);
        sVal2.Sign |= 0x80;
        ApplVar.LessDiscAmt = sVal2;
        memset(ProgLineMes, ' ', sizeof(ProgLineMes));
        if(strcmp(ECRVIP.VipFix.VipName, "SC#") == 0){
            strcpy(ProgLineMes,"SC DISCOUNT (20%)");
        }else{
            strcpy(ProgLineMes,"PWD DISCOUNT (20%)");
        }
        PrintAmt(ProgLineMes, &sVal2);
    }
	#endif

    PrintLine('.');//20130709分割线没有多大的意义，客户反馈也浪费纸
	if (!ApplVar.FPb) /* also when no check out function */
	{
		SETBIT(ApplVar.PrintLayOut, BIT2);  /* double size */
		ApplVar.SlipDouble = 1;
		
		//jdb2018-03-09
		#if 0
		if(TESTBIT(ECRVIP.ECRVIPState, BIT4)){
			sVal2 = ApplVar.SubTotal;
			Add(&sVal2, &ApplVar.LessVatTotalAmt);
			Add(&sVal2, &ApplVar.LessDiscAmt);
			PrintAmt(Msg[YINGFU].str, &sVal2);
			ApplVar.SubTotal = sVal2;
			if(!Appl_EntryCounter){
				ApplVar.Entry = sVal2;
			}
		}
		else
		#endif
		{
			PrintAmt(Msg[YINGFU].str, &ApplVar.SubTotal);	//ApplVar.Total
		}
#if (SCREENLN<5)
#if (SCRSTATDBL)
        DispStrXY(DispAmtStr(Msg[YINGFU].str, &ApplVar.SubTotal,SCRDBLWD),0,STATELINE);//ccr2016-03-02
#else
        DispStrXY(DispAmtStr(Msg[YINGFU].str, &ApplVar.SubTotal,SCREENWD),0,STATELINE);//ccr2016-03-02
#endif
#endif
		RESETBIT(ApplVar.PrintLayOut, BIT2);  /* double size */
	}
	ApplVar.SubDiscFlag = 0;	/* cc 2006-10-11 for SubDisc */
}


void PrintTotal()
{
	BCD Val1, Val2, Val3;

	Val1 = ZERO;
	Val2 = ZERO;
	Val3 = ZERO;

	//if (ApplVar.Report.Type == REPORT_DEPARTMENT) //jdb2018-04-8 不打印部类
		//return;

#if (HOR_REPORT)
    //>>>>>>>> for new report(ApplVar.Dept & PLU only)
	if (!ApplVar.RepComputer)//ccr100702>>>>>>>>>
	{
	    if(ApplVar.Report.Type==2)  // dept report
	    {
	    	if (ApplVar.Size.Qty)
	           FormatQty(SysBuf+19+pSHIFTx,&ApplVar.Total.Qty);
	        else if (ApplVar.Size.Cc)
	           FormatQty(SysBuf+19+pSHIFTx,&ApplVar.Total.Cc);
	        if (ApplVar.Size.Amt)
	        {
	            ApplVar.Amt = ApplVar.Total.Amt;
				if (ApplVar.Size.Disc && CheckNotZero(&ApplVar.Total.Disc))
				{
				    Subtract(&ApplVar.Amt, &ApplVar.Total.Disc);       /* calculate gross */
					if(((ApplVar.Total.Amt.Sign & 0x80)==0) && ((ApplVar.Total.Disc.Sign & 0x80)!=0))
						ApplVar.Amt.Sign = ApplVar.Total.Amt.Sign;
				}
				FormatAmt(SysBuf+PRTLEN-1, &ApplVar.Amt);
	        }
	        RJPrint(0,SysBuf);
			if (ApplVar.Size.Disc && CheckNotZero(&ApplVar.Total.Disc))
			{
		    	memset(SysBuf,' ',PRTLEN);
				CopyFrStr(SysBuf+12,Msg[ZHEKOUF].str);FormatAmt(SysBuf+PRTLEN-1, &ApplVar.Total.Disc);
		        RJPrint(0,SysBuf);
			}
	        return;
	    }
	    else if(ApplVar.Report.Type==3)       // plu report
	    {
	    	if (ApplVar.Size.Qty)
		        FormatQty(SysBuf+19+pSHIFTx,&ApplVar.Total.Qty);
	        else if (ApplVar.Size.Cc)
	           FormatQty(SysBuf+19+pSHIFTx,&ApplVar.Total.Cc);

	        if (ApplVar.Size.Amt)
	        {
	            ApplVar.Amt = ApplVar.Total.Amt;
				if (ApplVar.Size.Disc && CheckNotZero(&ApplVar.Total.Disc))
				{
				    Subtract(&ApplVar.Amt, &ApplVar.Total.Disc);
					if(((ApplVar.Total.Amt.Sign & 0x80)==0) && ((ApplVar.Total.Disc.Sign & 0x80)!=0))
						ApplVar.Amt.Sign = ApplVar.Total.Amt.Sign;
				}
				FormatAmt(SysBuf+PRTLEN-1, &ApplVar.Amt);
	        }
	        RJPrint(0,SysBuf);

		    if (ApplVar.Size.Cost && ApplVar.Size.Amt && ApplVar.AP.Plu.Cost && CheckNotZero(&ApplVar.Total.Cost))//
		    {
		    	memset(SysBuf,' ',PRTLEN);
				if (ApplVar.Size.Disc && CheckNotZero(&ApplVar.Total.Disc))
				{
					CopyFrStr(SysBuf+2,Msg[ZHEKOUF].str);
                    FormatAmt(SysBuf+14+(pSHIFTx>>1), &ApplVar.Total.Disc);
				}
				else
				{
					CopyFrStr(SysBuf+2,Msg[CHENBEN].str);//SysBuf[2+4]=':';
                    FormatAmt(SysBuf+14+(pSHIFTx>>1), &ApplVar.Total.Cost);
				}
				ApplVar.Amt = ApplVar.Total.Amt;
				Subtract(&ApplVar.Amt, &ApplVar.Total.Cost);
				CopyFrStr(SysBuf+16+(pSHIFTx>>1),Msg[MAOLI].str);//SysBuf[16+4]=':';
                FormatAmt(SysBuf+PRTLEN-1, &ApplVar.Amt);
		        RJPrint(0,SysBuf);
		    }
			else if (ApplVar.Size.Disc && CheckNotZero(&ApplVar.Total.Disc))
			{
		    	memset(SysBuf,' ',PRTLEN);
				CopyFrStr(SysBuf+12,Msg[ZHEKOUF].str);FormatAmt(SysBuf+PRTLEN-1, &ApplVar.Total.Disc);
		        RJPrint(0,SysBuf);
			}
	        return;
	    }
    }//<<<<<<<<<<       end
#endif

#if NETVATVERSION //jdb2018-08-11
	if(ApplVar.Report.Type == REPORT_GRANDTOTAL || ApplVar.Report.Type == REPORT_DISCOUNT){ //jdb2018-03-29 不打印 CC QTY
	}else
#endif
    {
    if (ApplVar.Size.Cc)
    {
		if (!SendComp(2, &ApplVar.Total.Cc))
		    PrintQty(Msg[JIAOYISHU].str, &ApplVar.Total.Cc);
    }
    if (ApplVar.Size.Qty)
    {
		if (!SendComp(3, &ApplVar.Total.Qty))
		    PrintQty(Msg[SHULIANG].str, &ApplVar.Total.Qty);
    }
	}
    if (ApplVar.Size.RetQty)
	//if (ApplVar.Size.RetQty || ApplVar.PointerType == 0) //jdb2018-04-03
    {
		if (!SendComp(4, &ApplVar.Total.RetQty)){
#if NETVATVERSION //jdb2018-08-11
			if(ApplVar.Report.Type == REPORT_GRANDTOTAL){ //jdb2018-03-28 OLD GRAND TOTAL
                RESETBIT(ApplVar.Total.RetQty.Sign, BIT7);
				if(ApplVar.FONP == 0x01){ //jdb2018-04-24 period report 不打印
				}else{
				PrintAmt(OLDGRANDTOTAL, &ApplVar.Total.RetQty);
				}
				Val3 = ApplVar.Total.RetQty; //jdb2018-04-02
			}else
#endif
            {
		    	PrintQty(Msg[TUIHUO].str, &ApplVar.Total.RetQty);
			}
		}
    }
    if (ApplVar.Size.Amt)
    {
		if (ApplVar.Report.Type == REPORT_TAX)       /* tax report ? */
		{
#if NETVATVERSION //jdb2018-08-11
			//jdb2018-03-21
			if(ApplVar.TaxNumber + 2 == ApplVar.AP.Tax.Number){ //jdb2018-04-27 打印零税率净销售额
				PrintAmt(ZERORATEDSALES, &ApplVar.Total.Amt);
			}
			else if(ApplVar.TaxNumber + 1 == ApplVar.AP.Tax.Number){ //最后一个税为SC PWD减税额
				if (!SendComp(9, &ApplVar.Total.Amt)){       /* taxable */
					//PrintAmt("VAT EXEMPT", &ApplVar.Total.Amt);
					PrintAmt(VATEXEMPTSALES, &ApplVar.Total.Amt); //jdb2018-04-27 打印免税净销售额
				}
			}else
#endif
            {
		    	if (!SendComp(9, &ApplVar.Total.Amt)){       /* taxable */
#if NETVATVERSION //jdb2018-08-11
					PrintAmt(GROSSSALES, &ApplVar.Total.Amt);
#else
					PrintAmt(Msg[HSHXSHOU].str, &ApplVar.Total.Amt);
#endif
		    	}
			}
		}
		else if (ApplVar.Report.Type == REPORT_PBFUNC && (ApplVar.PbF.Options & 0x0f) == 10)       /* PB info report */
		{
		    if (!SendComp(3, &ApplVar.Total.Amt))
				PrintQty(Msg[SHULIANG].str, &ApplVar.Total.Amt);   /* and cover function ?*/
		}
		//else if(ApplVar.Report.Type == REPORT_GRANDTOTAL){ //jdb2018-03-28 不打印
		//}
		else if (!ApplVar.Size.Disc)
		//else if (!ApplVar.Size.Disc && ApplVar.PointerType != 0) //jdb2018-04-03
		{
		    if (ApplVar.Report.Type == REPORT_DRAWER)    /* drawer report */
		    {
//				if (TESTBIT(ApplVar.Draw.Options, BIT1))
//				{//打开钱箱次数统计
//				    if (!SendComp(3, &ApplVar.Total.Amt))
//						PrintQty(Msg[SHULIANG].str, &ApplVar.Total.Amt);
//				}
//				else
                if (!SendComp(7, &ApplVar.Total.Amt))
				{
#if (0) //ccr2016-11-07>>>>>>>>小数点与系统设置一致
					if (TESTBIT(ApplVar.Draw.Options, BIT2))
						ApplVar.AmtDecimal = 2;
					else
						ApplVar.AmtDecimal = 0;
					ApplVar.AmtDecimal += ApplVar.Draw.Options & BIT0;
				    Prefix1 = 0;
				    Prefix2 = 0;    /* no prefix */
#endif //ccr2016-11-07<<<<<<<<<<
				    PrintAmt(Msg[JINE].str, &ApplVar.Total.Amt);
				    ApplVar.AmtDecimal = NO_DECIMAL;
				    Prefix1 = PREFIX_1;
				    Prefix2 = PREFIX_2;
				}
		    }
		    else if (!SendComp(7, &ApplVar.Total.Amt)){
#if NETVATVERSION //jdb2018-08-11
				if (ApplVar.Report.Type == REPORT_DISCOUNT){ //jdb2018-03-29
					PrintAmt(ApplVar.Disc.Name, &ApplVar.Total.Amt);
					Add(&ApplVar.DiscAmtTotal, &ApplVar.Total.Amt);
				}else
#endif
                {
					PrintAmt(Msg[JINE].str, &ApplVar.Total.Amt);
				}
		    }
		}
		else
		{
		    ApplVar.Amt = ApplVar.Total.Amt;
		    Subtract(&ApplVar.Amt, &ApplVar.Total.Disc);       /* calculate gross */
//lyq added 2003\10\27 start
			if(((ApplVar.Total.Amt.Sign & 0x80)==0) && ((ApplVar.Total.Disc.Sign & 0x80)!=0))
				ApplVar.Amt.Sign = ApplVar.Total.Amt.Sign;
		    if (!SendComp(5, &ApplVar.Amt)){
#if NETVATVERSION //jdb2018-08-11
				if(ApplVar.Report.Type == REPORT_GRANDTOTAL){ //jdb2018-03-29
					Val1 = ApplVar.Amt;
					//PrintLine('.');
					//PrintAmt("TOTAL GROSS SALES", &ApplVar.Amt);
				}else
#endif
                {
					PrintAmt(Msg[JINE].str, &ApplVar.Amt);
				}
		    }
		}
		if (ApplVar.Report.Type == REPORT_CURRENCY && TESTBIT(ApplVar.Curr.Options, BIT2))   /* currency */
		{
		    GetCurrency(0, &ApplVar.Total.Amt);
		    if (!SendComp(7, &ApplVar.Total.Amt))
		    {
				ApplVar.AmtDecimal = ApplVar.Curr.Options & BIT0;
				Prefix1 = ApplVar.Curr.Prefix1;
				Prefix2 = ApplVar.Curr.Prefix2;
				PrintAmt(Msg[JINE].str, &ApplVar.Total.Amt);
				ApplVar.AmtDecimal = NO_DECIMAL;
				Prefix1 = PREFIX_1;
				Prefix2 = PREFIX_2;
		    }
		}
    }
    if (ApplVar.Size.Disc)
    {
#if NETVATVERSION //jdb2018-08-11
    	if(ApplVar.Report.Type == REPORT_GRANDTOTAL){ //jdb2018-03-28 不打印
		}else
#endif
        {
			if (!SendComp(6, &ApplVar.Total.Disc))
		    	PrintAmt(Msg[SETDISC].str, &ApplVar.Total.Disc);
			if (!SendComp(7, &ApplVar.Total.Amt))
		    	PrintAmt(Msg[JINGE].str, &ApplVar.Total.Amt);
		}
    }
    if (ApplVar.Size.Cost)
    {
		if (ApplVar.Report.Type == REPORT_TAX)
		{
#if NETVATVERSION //jdb2018-08-11
			//jdb2018-03-21
			if(ApplVar.TaxNumber + 2 == ApplVar.AP.Tax.Number){ //jdb2018-04-27 零税率不打印
			}
			else if(ApplVar.TaxNumber + 1 == ApplVar.AP.Tax.Number){ //jdb2018-04-27 打印免税额
				PrintAmt(VATEXEMPTAMOUNT, &ApplVar.Total.Disc);
			}
			else if(ApplVar.TaxNumber + 1 < ApplVar.AP.Tax.Number)//最后一个税为SC PWD 减税额不打印
#endif
            { 
				if (!SendComp(7, &ApplVar.Total.Disc)){       /* NET tax report ? */
#if NETVATVERSION //jdb2018-08-11
					PrintAmt(VATABLESALES, &ApplVar.Total.Disc);
#else
			    	PrintAmt(Msg[XSHOUE].str, &ApplVar.Total.Disc);
#endif
				}
		    	if (!SendComp(10, &ApplVar.Total.Cost)){       /* tax report ? */
#if NETVATVERSION //jdb2018-08-11
					PrintAmt(VATAMOUNT, &ApplVar.Total.Cost);
#else
					PrintAmt(Msg[SHUIE].str, &ApplVar.Total.Cost);
#endif
		    	}
			}
		}
#if NETVATVERSION //jdb2018-08-11
		else if(ApplVar.Report.Type == REPORT_GRANDTOTAL){ //jdb2018-03-28为当前Z报表净销售额
			Val2 = ApplVar.Total.Cost;
			//PrintAmt("NEW GRAND TOTAL", &ApplVar.Total.Cost);
		}
#endif
		else
		{
			if (!SendComp(8, &ApplVar.Total.Cost))
			    PrintAmt(Msg[CHENBEN].str, &ApplVar.Total.Cost);
			ApplVar.Amt = ApplVar.Total.Amt;
			Subtract(&ApplVar.Amt, &ApplVar.Total.Cost);
			if (!SendComp(12, &ApplVar.Amt))
				PrintAmt(Msg[MAOLI].str, &ApplVar.Amt);
		}
    }

#if NETVATVERSION //jdb2018-08-11
	if(ApplVar.Report.Type == REPORT_GRANDTOTAL && ApplVar.Size.RetQty != 0){ //jdb2018-04-03
		#if 0
		PrintAmt("NEW GRAND TOTAL", &Val2);
		#else //jdb2018-04-02
		if(TESTBIT(Val3.Sign, BIT7)){
			Val3.Sign = Val2.Sign; //jdb2018-04-04 小数点一样
			SETBIT(Val3.Sign, BIT7);
		}else{
			Val3.Sign = Val2.Sign; //jdb2018-04-04 小数点一样
			RESETBIT(Val3.Sign, BIT7);
		}
		//Val3.Sign = Val2.Sign; //jdb2018-04-04 小数点一样
		Add(&Val3, &Val2);
		if(ApplVar.FONP == 0x01){ //jdb2018-04-24 period report 不打印
		}else{
			ApplVar.ENDBALANCE = Val3; //jdb2018-07-26 保存 ending balance value
			SETBIT(ApplVar.ECRRGFLAG, BIT1); //jdb2018-07-26 打印GRAND 标志
		PrintAmt(NEWGRANDTOTAL, &Val3);
		PrintLine('.');
		}
		#endif
		
		PrintAmt(TOTALGROSSSALES, &Val1);
	}
#endif
}


//ccr 050301 显示汇总数据>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //

void DisplayTotal()
{
	UnLong    save ;             /* unsigned long save;  */

    if (ApplVar.FTrain && ApplVar.PointerType != 1)
    {
   		RamOffSet += ApplVar.Size.Length; /* next period or total */
		return;         /* only add in clerk when training */
    }
    save = RamOffSet;      /* save OffSet for Writing */
    ReadTotal();
    RamOffSet = save;


//    SETBIT(ApplVar.Total.Qty.Sign, BIT2 + BIT3);   /* set bit 4 & 5 to indicate used */
//    WriteRam((BYTE*)&ApplVar.Total.Qty, ApplVar.Size.Qty + 1);
//    if (ApplVar.Size.Cc)
//   		DispSt2C(DispQtyStr(Msg[JIAOYISHU].str, &ApplVar.Total.Cc));
    if (ApplVar.Size.Amt)
   		DispSt2C(DispAmtStr(Msg[JINE].str, &ApplVar.Total.Amt,DISLEN));
//    if (ApplVar.Size.RetQty)
//   		DispSt2C(DispQtyStr(Msg[SHULIANG].str, &ApplVar.Total.RetQty));
//    if (ApplVar.Size.Disc)
//   		DispSt2C(DispAmtStr(Msg[SETDISC].str, &ApplVar.Total.Disc,DISLEN));
//    if (ApplVar.Size.Cost)
//   		DispSt2C(DispAmtStr(Msg[CHENBEN].str, &ApplVar.Total.Cost,DISLEN));
}


void DisplayPointerTotal(char *sFile)
{
    WORD skip, next;

    if (ApplVar.FProforma)
		return;
    if (!ApplVar.Size.Periods)
		return;
    if (!ApplVar.Size.Length)     /* active ? */
		return;
    if (ApplVar.FSplit)
		return;             /* don't add when split bill */
    skip = 0;
    if (TESTBIT(ApplVar.Size.Periods, BIT0))      /* calculate size of 1 pointer */
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT1))
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT2))
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT3))
		skip += ApplVar.Size.Length;
    next = skip;
    switch (ApplVar.PointerType)
    {
	case 0:
	    skip = 0;
	    next = 0;
	    break;
	case 1:         /* Clerks */
	    if (!ApplVar.AP.Clerk.Number)
			return;
	    next *= (ApplVar.AP.Clerk.Number - ApplVar.ClerkNumber);
	    skip *= (ApplVar.ClerkNumber - 1);
	    break;
	case 2:         /* Hour */
	    if (!ApplVar.AP.Zone.Number)
			return;
	    next *= (ApplVar.AP.Zone.Number - ApplVar.Zone - 1);
	    skip *= ApplVar.Zone;
	    break;
	case 3:         /* day of week */
	    if (!ApplVar.AP.Day.Number)
			return;
	    next *= (ApplVar.AP.Day.Number - ApplVar.Day - 1);
	    skip *= ApplVar.Day;
	    break;
	case 4:         /* ApplVar.Month of Year */
	    if (!ApplVar.AP.Month.Number)
			return;
	    next *= (ApplVar.AP.Month.Number - ApplVar.Month - 1);
	    skip *= ApplVar.Month;
	    break;
	case REPDEFMAX-1:
#if (salNumber)
	    if (ApplVar.AP.SalPer.Number)
		{
			next *= (ApplVar.AP.SalPer.Number - ApplVar.SalPerNumber);
		    skip *= (ApplVar.SalPerNumber - 1);
            break;
		}
        else
            return;
#else
        return;
#endif
	default:
	    break;
    }
   	RamOffSet += skip;
	DispSt1C(sFile);
    if (TESTBIT(ApplVar.Size.Periods, BIT0))
		DisplayTotal();
	else if (TESTBIT(ApplVar.Size.Periods, BIT1))
		DisplayTotal();
	else if (TESTBIT(ApplVar.Size.Periods, BIT2))
		DisplayTotal();
    else if (TESTBIT(ApplVar.Size.Periods, BIT3))
		DisplayTotal();
	RamOffSet += next;     /* start next pointer totals */
}
//ccr 050301 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< //

#if defined(CASE_PERU)//ccr2016-10-24>>>>>>>>>>>>>>>
//打印顾客信息
void PrintCustomerMessage()
{
    int i,sP;

    if (ApplVar.CustomerMessage.Messages[0][0])//ApplVar.CustomerMessage.CopyOfMess)
    {
		PrintLine('.');
        for (i=0;i<CUSTLINES && ApplVar.CustomerMessage.Messages[i][0];i++)
        {
            RJPrint(0,ApplVar.CustomerMessage.Messages[i]);
        }
    }
    ApplVar.CustomerMessage.CopyOfMess = ApplVar.CustomerMessage.Messages[0][0];
    ApplVar.CustomerMessage.Messages[0][0]=0;
}
#endif//ccr2016-10-24<<<<<<<<<<<<<
