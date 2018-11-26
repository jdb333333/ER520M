#define DISCOUNT_C 7
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "Message.h"

#define JIANGBO 0


/***************************************************************/


void GetDiscOffSet()
{
  	RamOffSet = ApplVar.DiscNumber * ApplVar.AP.Disc.RecordSize + ApplVar.AP.StartAddress[AddrDisc];
}

void AddDiscTotal()
{
	SETBIT(ApplVar.FREP, BIT5); //jdb2018-07-03 增加折扣报表标志
    GetDiscOffSet();
    RamOffSet += ApplVar.AP.Disc.TotalOffSet;
    for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
    {
		ApplVar.Size = ApplVar.AP.Disc.Size[ApplVar.PointerType];
		AddPointerTotal();
    }
	RESETBIT(ApplVar.FREP, BIT5); //jdb2018-07-03 复位折扣报表标志
}

void WriteDisc()
{
    if (ApplVar.DiscNumber < ApplVar.AP.Disc.Number)
    {
		GetDiscOffSet();
	    WriteRam((BYTE*)&ApplVar.Disc, ApplVar.AP.Disc.TotalOffSet);      /* write function options */
    }
}

void ReadDisc()
{
    GetDiscOffSet();

    ReadRam((BYTE*)&ApplVar.Disc, ApplVar.AP.Disc.TotalOffSet);      /* write function options */
    ApplVar.Disc.Name[ApplVar.AP.Disc.CapSize]=0 ;
}

void PrintPercentage(BCD *perc)
{
    if (!TESTBIT(ApplVar.Disc.Options, BIT2) && /* percentage ? */
		TESTBIT(ApplVar.Disc.Options, BIT0 + BIT1))  /* and open, open/fixed */
    {
		MemSet(SysBuf, sizeof(SysBuf), ' ');
		perc->Sign = 0x02;
		FormatQty(SysBuf + 10, perc);
		SysBuf[12] = '%';
		SysBuf[13] = 0;
		PrintStr(SysBuf);
    }
}

//jdb2018-08-09 SC PWD小计后折扣计算
//sType = 0 SC PWD 小计百分比折扣
//sType = 1 折扣后取消交易
void CalculateScPwdDisc(int sType)
{
	BYTE i;
	int ret;
	BCD sAmt,saveAmt,saveDiscAmt,saveQty,sDiscVal;//保存全局变量
	BCD matchqty, soldqty, freeqty, offamt;//计算特殊出价
	BCD offvat;//计算免税额
	WORD savePlu,saveDept;
	BYTE ScPwdDisc_Flag;/* BIT0:	PLU Sales*/
	struct TRANSRECORD  MyRGRec;
	BYTE deptnum[256] = {0}; //SI版本没有免税情况下，同一种税率只计算一次SC PWD折扣
	BYTE scpwdtaxflag;
	
	saveAmt = ApplVar.Amt;
	saveDiscAmt = ApplVar.DiscAmt;
	saveQty = ApplVar.Qty;
	savePlu = ApplVar.PluNumber;
	saveDept = ApplVar.DeptNumber;
	ScPwdDisc_Flag = 0;

	if(sType == 0)
	{
		sAmt = sDiscVal = ZERO;
		sDiscVal = ApplVar.Entry;
		Multiply(&sDiscVal,(BCD*)&THOUSAND100);

		sDiscVal.Sign ^= 0x80;
		if(ApplVar.FVoid)
			sDiscVal.Sign ^= 0x80;
	}

	for(i=0;i<ApplVar.RGItems.RGNumber;i++)
	{
		matchqty = ZERO;
		soldqty = ZERO;
		freeqty = ZERO;
		offamt = ZERO;
		offvat = ZERO;
		if(ApplVar.RGItems.RGBuf[i].Key.Code > DEPT)
		{
			if(ApplVar.RGItems.RGBuf[i].Key.Code > PLU1)
			{
				SETBIT(ScPwdDisc_Flag, BIT0);
				ApplVar.PluNumber = ApplVar.RGItems.RGBuf[i].Key.Code - PLU1 - 1;
				ReadPlu();
				ApplVar.DeptNumber = ApplVar.PluDept;
				if(sType == 0){
				if (TESTBIT(ApplVar.MyFlags,PLUPACK))//jdb2018-08-02 特殊打包出价情况
					{
						matchqty = ZERO;
						memcpy(matchqty.Value, ApplVar.Plu.Cost, ApplVar.AP.Plu.PriceSize);
						soldqty = ZERO;
						soldqty = ApplVar.RGItems.RGBuf[i].Qty;
						matchqty.Sign = soldqty.Sign;
						freeqty = ZERO;
            			while (CompareBCD(&soldqty, &matchqty) >= 0)
            			{
                			Subtract(&soldqty, &matchqty);
                			Add(&freeqty, (BCD*)&ONE);
            			}
						if (CheckNotZero(&freeqty))    /* not zero */
            			{
                			offamt = ZERO;
                			memcpy(offamt.Value, ApplVar.Plu.Price[1], ApplVar.AP.Plu.PriceSize);
                			Multiply(&offamt, &freeqty);
            			}
					}
				else if(TESTBIT(ApplVar.MyFlags,PLUDISC))//jdb2018-08-02 特殊出价折扣出价情况
					{
						offamt = ZERO;
            			memcpy(offamt.Value, ApplVar.Plu.Price[1], ApplVar.AP.Plu.PriceSize);
            			offamt.Sign = 4;
						soldqty = ZERO;
						soldqty = ApplVar.RGItems.RGBuf[i].DiscAmt;
						Multiply(&offamt, &soldqty);
            			RoundBcd(&offamt,0);
					}
				}
			}
			else
				ApplVar.DeptNumber = ApplVar.RGItems.RGBuf[i].Key.Code - DEPT - 1;
			ReadDept();
			MyRGRec = ApplVar.RGItems.RGBuf[i];
			ret = StoreInBuffer2(5, MyRGRec);//查询是否有免税记录
			if(sType == 0)
			{
				if(ret >= 0){
					MyRGRec = ApplVar.NoVatRGItems.RGBuf[ret];
					offvat = CalculateNetTax(MyRGRec);
					sAmt = ApplVar.NoVatRGItems.RGBuf[ret].Amt;
					Multiply(&sAmt, &ApplVar.NoVatRGItems.RGBuf[ret].Qty);
					offvat.Sign = sAmt.Sign;
					Subtract(&sAmt, &offvat);//减去免税额
				}else{
					sAmt = ApplVar.RGItems.RGBuf[i].Amt;
					Multiply(&sAmt, &ApplVar.RGItems.RGBuf[i].Qty);
				}

				offamt.Sign = sAmt.Sign;
				Subtract(&sAmt, &offamt);//减去特殊出价的优惠
				Multiply(&sAmt, &sDiscVal);
				Divide(&sAmt, (BCD*)&MILLION10);
				RoundBcd(&sAmt, 0);
				AmtRound(1, &sAmt);

				ApplVar.DiscAmt = ApplVar.Amt = sAmt;

				if(ret >= 0){
					Add(&ApplVar.NoVatRGItems.RGBuf[ret].DiscAmt, &sAmt);//折扣后金额
				}
				else{
					Add(&ApplVar.RGItems.RGBuf[i].DiscAmt, &sAmt);//折扣后金额

					#if ORRECEIPT
					#else
					if(deptnum[ApplVar.Dept.Tax] != 0x01) //0x0 表示没有计算过 0x01 表示计算过
					{
						scpwdtaxflag = 0x0; //相同税率小计折扣只计算一次
						deptnum[ApplVar.Dept.Tax] = 0x01; //计算过后，相同税率下次不再计算
					}
				
					ApplVar.Amt = saveAmt;

					if(scpwdtaxflag == 0x0) //小计折扣减税只计算一次
					{
						CalculateTax2();
						scpwdtaxflag = 0x1;
					}
					#endif
				}
			}
			else if(sType == 1){
				if(ret >= 0){
					ApplVar.DiscAmt = ApplVar.NoVatRGItems.RGBuf[ret].Amt;
					Multiply(&ApplVar.DiscAmt, &ApplVar.NoVatRGItems.RGBuf[ret].Qty);
					Subtract(&ApplVar.DiscAmt, &ApplVar.NoVatRGItems.RGBuf[ret].DiscAmt);
					ApplVar.Amt = ApplVar.DiscAmt;
				}else{
					ApplVar.DiscAmt = ApplVar.RGItems.RGBuf[i].Amt;
					Multiply(&ApplVar.DiscAmt, &ApplVar.RGItems.RGBuf[i].Qty);
					Subtract(&ApplVar.DiscAmt, &ApplVar.RGItems.RGBuf[i].DiscAmt);
					ApplVar.Amt = ApplVar.DiscAmt;
				}
			}

			if(TESTBIT(ScPwdDisc_Flag, BIT0))
				AddPluTotal();
			AddDeptTotal();
		}
	}
	
	if(ApplVar.FVoid)
		sDiscVal = ZERO;
	ApplVar.Amt = saveAmt;
	ApplVar.DiscAmt = saveDiscAmt;
	ApplVar.Qty = saveQty;
	ApplVar.PluNumber = savePlu;
	ApplVar.DeptNumber = saveDept;
}

//cc 2006-10-10 for SubDisc>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//sType 	= 0:百分比小计折扣	//
//		= 1:小计折扣后取消交易	//
//		= 2:金额小计折扣		//
//		= 4:小计折扣计税		//
//////////////////////////////////////////////////////////////
void CalculateSubDisc(int sType)
{
	BYTE i;
	BYTE SubDisc_Flag;/* BIT0:	PLU Sales	BIT1:	ApplVar.FRefund		BIT2:	ApplVar.FVoid*/

	BCD sAmt,saveAmt,saveDiscAmt,saveQty,sDiscVal;
	BCD matchqty, soldqty, freeqty, offamt;//jdb2018-08-02 计算特殊出价

	WORD savePlu,saveDept;
    ////////////////////////

	BYTE deptnum[256] = {0}; //jdb2018-06-15 同一种税率只计算一次小计折扣
	BYTE subtaxflag;

	saveAmt = ApplVar.Amt;
	saveDiscAmt = ApplVar.DiscAmt;
	saveQty = ApplVar.Qty;
	savePlu = ApplVar.PluNumber;
	saveDept = ApplVar.DeptNumber;

	SubDisc_Flag = 0;
	if(sType == 0)
	{
		sAmt = sDiscVal = ZERO;
		sDiscVal = ApplVar.Entry;
		Multiply(&sDiscVal,(BCD*)&THOUSAND100);

		if(!TESTBIT(ApplVar.Disc.Options,BIT3))//为折扣
			sDiscVal.Sign ^= 0x80;
		if(ApplVar.FVoid)
			sDiscVal.Sign ^= 0x80;
	}
//	else if(sType == 1)
//		sDiscVal.Sign ^= 0x80;
	else if(sType == 2)
	{
		sAmt = sDiscVal = ZERO;
		sDiscVal = ApplVar.Entry;//存储折扣金额
		for(i=0;i<ApplVar.RGItems.RGNumber;i++)
		{
			//jdb2018-08-02 特殊出价相关参数清零
			matchqty = ZERO;
			soldqty = ZERO;
			freeqty = ZERO;
			offamt = ZERO;
			if(ApplVar.RGItems.RGBuf[i].Key.Code > DEPT)
			{
				if(ApplVar.RGItems.RGBuf[i].Key.Code > PLU1)
				{
					ApplVar.PluNumber = ApplVar.RGItems.RGBuf[i].Key.Code - PLU1 - 1;
					ReadPlu();
					ApplVar.DeptNumber = ApplVar.PluDept;
					if (TESTBIT(ApplVar.MyFlags,PLUPACK))//jdb2018-08-02 特殊打包出价情况
					{
						matchqty = ZERO;
						memcpy(matchqty.Value, ApplVar.Plu.Cost, ApplVar.AP.Plu.PriceSize);
						soldqty = ZERO;
						soldqty = ApplVar.RGItems.RGBuf[i].Qty;
						matchqty.Sign = soldqty.Sign;
						freeqty = ZERO;
            			while (CompareBCD(&soldqty, &matchqty) >= 0)
            			{
                			Subtract(&soldqty, &matchqty);
                			Add(&freeqty, (BCD*)&ONE);
            			}
						if (CheckNotZero(&freeqty))    /* not zero */
            			{
                			offamt = ZERO;
                			memcpy(offamt.Value, ApplVar.Plu.Price[1], ApplVar.AP.Plu.PriceSize);
                			Multiply(&offamt, &freeqty);
                			offamt.Sign ^= 0x80;
            			}
					}
					else if(TESTBIT(ApplVar.MyFlags,PLUDISC))//jdb2018-08-02 特殊出价折扣出价情况
					{
						offamt = ZERO;
            			memcpy(offamt.Value, ApplVar.Plu.Price[1], ApplVar.AP.Plu.PriceSize);
            			offamt.Sign = 4;
						soldqty = ZERO;
						soldqty = ApplVar.RGItems.RGBuf[i].DiscAmt;
						Multiply(&offamt, &soldqty);
            			RoundBcd(&offamt,0);
						offamt.Sign ^= 0x80;
					}
				}
				else
					ApplVar.DeptNumber = ApplVar.RGItems.RGBuf[i].Key.Code - DEPT - 1;
				ReadDept();

				if(TESTBIT(ApplVar.Disc.Options,BIT6) || TESTBIT(ApplVar.Disc.Options,BIT7))
				{
					if(!(TESTBIT(ApplVar.Disc.Options,BIT6) && TESTBIT(ApplVar.Dept.Options,BIT6)) &&
						!(TESTBIT(ApplVar.Disc.Options,BIT7) && TESTBIT(ApplVar.Dept.Options,BIT7)))
						continue;
				}
				sAmt = ApplVar.RGItems.RGBuf[i].DiscAmt;
				if(TESTBIT(SubDisc_Flag,BIT1))
				{
					sAmt.Sign ^= 0x80;
					RESETBIT(SubDisc_Flag,BIT1);
				}
				Add(&ApplVar.SUB_DiscTot,&sAmt);
				Add(&ApplVar.SUB_DiscTot, &offamt);//jdb2018-08-02 减去特殊出价优惠的价格
			}
			else if(ApplVar.RGItems.RGBuf[i].Key.Code == CORREC+3 || ApplVar.RGItems.RGBuf[i].Key.Code == CORREC+2)
			{
//				SETBIT(SubDisc_Flag,BIT1);
				continue;
			}
		}

		Multiply(&sDiscVal,(BCD*)&MILLION10);
		Divide(&sDiscVal,&ApplVar.SUB_DiscTot);
		//Divide(&sDiscVal,&ApplVar.SubTotal);//jdb2018-08-02 计算百分比

		if (!TESTBIT(ApplVar.Disc.Options, BIT3))    /* Positive or negative */
			sDiscVal.Sign ^= 0x80;
		if(ApplVar.FVoid)
			sDiscVal.Sign ^= 0x80;
	}

	for(i=0;i<ApplVar.RGItems.RGNumber;i++)
	{
			//jdb2018-08-02 特殊出价相关参数清零
			matchqty = ZERO;
			soldqty = ZERO;
			freeqty = ZERO;
			offamt = ZERO;
		if(ApplVar.RGItems.RGBuf[i].Key.Code > DEPT)
		{
			RESETBIT(SubDisc_Flag,BIT0);
			if(ApplVar.RGItems.RGBuf[i].Key.Code > PLU1)
			{
				SETBIT(SubDisc_Flag,BIT0);
				ApplVar.PluNumber = ApplVar.RGItems.RGBuf[i].Key.Code - PLU1 - 1;
				ReadPlu();
				ApplVar.DeptNumber = ApplVar.PluDept;
				if (TESTBIT(ApplVar.MyFlags,PLUPACK))//jdb2018-08-02 特殊打包出价情况
					{
						matchqty = ZERO;
						memcpy(matchqty.Value, ApplVar.Plu.Cost, ApplVar.AP.Plu.PriceSize);
						soldqty = ZERO;
						soldqty = ApplVar.RGItems.RGBuf[i].Qty;
						matchqty.Sign = soldqty.Sign;
						freeqty = ZERO;
            			while (CompareBCD(&soldqty, &matchqty) >= 0)
            			{
                			Subtract(&soldqty, &matchqty);
                			Add(&freeqty, (BCD*)&ONE);
            			}
						if (CheckNotZero(&freeqty))    /* not zero */
            			{
                			offamt = ZERO;
                			memcpy(offamt.Value, ApplVar.Plu.Price[1], ApplVar.AP.Plu.PriceSize);
                			Multiply(&offamt, &freeqty);
                			offamt.Sign ^= 0x80;
            			}
					}
				else if(TESTBIT(ApplVar.MyFlags,PLUDISC))//jdb2018-08-02 特殊出价折扣出价情况
					{
						offamt = ZERO;
            			memcpy(offamt.Value, ApplVar.Plu.Price[1], ApplVar.AP.Plu.PriceSize);
            			offamt.Sign = 4;
						soldqty = ZERO;
						soldqty = ApplVar.RGItems.RGBuf[i].DiscAmt;
						Multiply(&offamt, &soldqty);
            			RoundBcd(&offamt,0);
            			offamt.Sign ^= 0x80;
					}
			}
			else
				ApplVar.DeptNumber = ApplVar.RGItems.RGBuf[i].Key.Code - DEPT - 1;
			ReadDept();

			if(sType == 4)
			{
				if(deptnum[ApplVar.Dept.Tax] != 0x01) //jdb2018-06-15 0x0 表示没有计算过 0x01 表示计算过
				{
				subtaxflag = 0x0; //jdb2018-06-15 相同税率小计折扣只计算一次
				deptnum[ApplVar.Dept.Tax] = 0x01; //计算过后，相同税率下次不再计算
				}
				
				ApplVar.Amt = saveAmt;
				if(!TESTBIT(ApplVar.Disc.Options,BIT6) && !TESTBIT(ApplVar.Disc.Options,BIT7))
				{
					if(subtaxflag == 0x0) //jdb2018-06-08 小计折扣减税只计算一次
					{
						CalculateTax2();
						subtaxflag = 0x1;
					}
				}
				else
				{
					if((TESTBIT(ApplVar.Disc.Options,BIT6) && TESTBIT(ApplVar.Dept.Options,BIT6)) ||
						(TESTBIT(ApplVar.Disc.Options,BIT7) && TESTBIT(ApplVar.Dept.Options,BIT7)))
					{
						if(subtaxflag == 0x0) //jdb2018-06-08 小计折扣减税只计算一次
						{
							CalculateTax2();
							subtaxflag = 0x1;
						}
					}
				}
			}

			else//if(sType != 4)
			{
				if(TESTBIT(ApplVar.Disc.Options,BIT6) || TESTBIT(ApplVar.Disc.Options,BIT7))
				{
					if(!(TESTBIT(ApplVar.Disc.Options,BIT6) && TESTBIT(ApplVar.Dept.Options,BIT6)) &&
						!(TESTBIT(ApplVar.Disc.Options,BIT7) && TESTBIT(ApplVar.Dept.Options,BIT7)))
						continue;
				}
//				PrintAmt(Prompt.Caption[16],&RGBuf[i].DiscAmt);
				if(sType == 1)
				{
					ApplVar.DiscAmt = ApplVar.RGItems.RGBuf[i].Amt;
					if(TESTBIT(SubDisc_Flag,BIT1))
					{
						sAmt.Sign ^= 0x80;
						RESETBIT(SubDisc_Flag,BIT1);
					}

					Multiply(&ApplVar.DiscAmt, &ApplVar.RGItems.RGBuf[i].Qty);	//20070118
					Subtract(&ApplVar.DiscAmt, &ApplVar.RGItems.RGBuf[i].DiscAmt);
					ApplVar.Amt = ApplVar.DiscAmt;
					ApplVar.Qty = ApplVar.RGItems.RGBuf[i].Qty;
					ApplVar.Qty.Sign ^= 0x80;
				}
				else
				{
					sAmt = ApplVar.RGItems.RGBuf[i].DiscAmt;

					if(TESTBIT(SubDisc_Flag,BIT1))
					{
						sAmt.Sign ^= 0x80;
						RESETBIT(SubDisc_Flag,BIT1);
					}

					Add(&sAmt, &offamt);//jdb2018-08-02 减去特殊出价的优惠
					Multiply(&sAmt,&sDiscVal);
					Divide(&sAmt,(BCD*)&MILLION10);
					RoundBcd(&sAmt,0);
					AmtRound(1,&sAmt);

					ApplVar.DiscAmt = ApplVar.Amt = sAmt;
					Add(&ApplVar.RGItems.RGBuf[i].DiscAmt,&sAmt);
				}
			}
		}
		else if(ApplVar.RGItems.RGBuf[i].Key.Code >= DISC_PERINC &&
                ApplVar.RGItems.RGBuf[i].Key.Code <= DISC_NETDEC && sType == 1)
		{
			ApplVar.Amt = ApplVar.DiscAmt = ApplVar.RGItems.RGBuf[i].Amt;
/*			sAmt = ApplVar.RGItems.RGBuf[i].Amt;

			if(TESTBIT(ApplVar.Disc.Options,BIT6) || TESTBIT(ApplVar.Disc.Options,BIT7))		//cc 20070518
			{
				if(!(TESTBIT(ApplVar.Disc.Options,BIT6) && TESTBIT(ApplVar.Dept.Options,BIT6)) &&
					!(TESTBIT(ApplVar.Disc.Options,BIT7) && TESTBIT(ApplVar.Dept.Options,BIT7)))
					continue;
			}

			Multiply(&sAmt,&sDiscVal);
			Divide(&sAmt,&MILLION10);
			RoundBcd(&sAmt,0);
			AmtRound(1,&sAmt);

			ApplVar.DiscAmt = ApplVar.Amt = sAmt;
*/
		}
		else if(ApplVar.RGItems.RGBuf[i].Key.Code >= DISC_SUB_PERINC &&
                ApplVar.RGItems.RGBuf[i].Key.Code <= DISC_SUB_NETDEC && sType == 1)
			continue;	//break;
		//jdb2018-03-27
		else if(ApplVar.RGItems.RGBuf[i].Key.Code >= DISC_SCSUB_PERDEC && 
				ApplVar.RGItems.RGBuf[i].Key.Code <= DISC_PWDSUB_PERDEC && sType == 1)
			continue;	//break;
		else if(ApplVar.RGItems.RGBuf[i].Key.Code == CORREC+2 && sType != 4)
		{
//			SETBIT(SubDisc_Flag,BIT1);
			continue;
		}
		else
			ApplVar.DiscAmt = ApplVar.Amt = ZERO;

		if(sType || ApplVar.FVoid)
			ApplVar.Qty = ZERO;

		if(sType != 4)
		{
			if (ApplVar.Dept.Group < ApplVar.AP.Group.Number)
				ApplVar.GroupNumber = ApplVar.Dept.Group;
			else
				ApplVar.GroupNumber = 0;

			//jdb2018-03-21
			if(ApplVar.DiscNumber == 8 || ApplVar.DiscNumber == 9){
				//SC PWD 折扣
				AddGroupTotal();

				if(TESTBIT(SubDisc_Flag,BIT0))
					AddPluTotal();
				AddDeptTotal();
			}else{
				AddGroupTotal();

				if(TESTBIT(SubDisc_Flag,BIT0))
					AddPluTotal();
				AddDeptTotal();
			}
		}
	}

	if(ApplVar.FVoid)
		sDiscVal = ZERO;
	ApplVar.Amt = saveAmt;
	ApplVar.DiscAmt = saveDiscAmt;
	ApplVar.Qty = saveQty;
	ApplVar.PluNumber = savePlu;
	ApplVar.DeptNumber = saveDept;
}
//cc 2006-10-10 for SubDisc<<<<<<<<<<<<<<<<<<<<

void Discount()
{
    BCD d1, d2;
    BYTE i, sign,sSign;
	WORD SKC;
	BCD Val1, Val2;

	Val1 = ZERO;
	Val2 = ZERO;

    ApplVar.FCurr = 0;
	sign = 0;
    if (!ApplVar.FVoid)
    {
		ApplVar.DiscNumber = ApplVar.Key.Code - DISC - 1;

//cc 2006-10-10 for SubDisc>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		if(ApplVar.DiscNumber > 1 && ApplVar.DiscNumber < 4 && !ApplVar.FRegi)
		{
			ApplVar.ErrorNumber = ERROR_ID(CWXXI01);
			return;
		}

		//jdb2018-03-20
		if(ApplVar.DiscNumber + 4 > ApplVar.AP.Disc.Number){ //jdb2018-03-20
		#if ORRECEIPT //jdb2018-08-09 SI版本不需要免税也可以折扣
			if(!TESTBIT(ECRVIP.ECRVIPState, BIT4)){ //SC PWD 没有经过减税操作
				ApplVar.ErrorNumber =  ERROR_ID(CWXXI04);
				return;
			}
		#endif
			
			if(!(ApplVar.FSub || ApplVar.RGRec.Key.Code==SUB)){ //jdb2018-04-03  没有经过小计
				ApplVar.ErrorNumber =  ERROR_ID(CWXXI04);
				return;
			}
		}

		if(ApplVar.FSub ||
           ApplVar.RGRec.Key.Code==SUB)//ccr20130815,记录的最后一个有效操作为小计时,仍然当作小计折扣
		{
            ApplVar.FSub = 1;//ccr20130815
			if(TESTBIT(ApplVar.SubDiscFlag,BIT0))
			{//已经进行过小计折扣后,禁止在进行小计折扣
				ApplVar.ErrorNumber =  ERROR_ID(CWXXI04);
				return;
			}
			if(ApplVar.DiscNumber + 4 < ApplVar.AP.Disc.Number){ //jdb2018-03-20
				//转换为小计折扣功能码
				ApplVar.Key.Code += 4;
				ApplVar.DiscNumber += 4;
			}
            //ccr20130809 SETBIT(ApplVar.SubDiscFlag,BIT0);
		}
//cc 2006-10-10 for SubDisc<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		if (ApplVar.DiscNumber < ApplVar.AP.Disc.Number)
		    ReadDisc();           /* read function */
		else
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI42);
		    return;
		}

		if ((!TESTBIT(ApplVar.Clerk.Options,BIT6) && TESTBIT(ApplVar.Disc.Options, BIT2) && !CheckPWD(Msg[SETMGPWD].str,ApplVar.AP.ModePwd.PwdMG)) ||
			(!TESTBIT(ApplVar.Clerk.Options,BIT4) && !TESTBIT(ApplVar.Disc.Options, BIT3) && !CheckPWD(Msg[SETMGPWD].str,ApplVar.AP.ModePwd.PwdMG)) ||
			(!TESTBIT(ApplVar.Clerk.Options,BIT5) && TESTBIT(ApplVar.Disc.Options, BIT3) && !CheckPWD(Msg[SETMGPWD].str,ApplVar.AP.ModePwd.PwdMG)))
		{
			ApplVar.ErrorNumber=ERROR_ID(CWXXI36);
			return;
		}

		if (ApplVar.FSub)
		{//为小计折扣
			ApplVar.RGRec.Group = 1;		//cc 2006-06-05 for SubDisc
//	    		ApplVar.FSub = 0;       /* reset subtotal  */
			if (TESTBIT(KEYTONE, BIT5))
		    	RESETBIT(ApplVar.Disc.Options, BIT4);    /* reset item */
		}
		else
			ApplVar.RGRec.Group = 0;		//cc 2006-06-05 for SubDisc
//		if ((ApplVar.Disc.Options & (BIT2+BIT4)) != BIT2) /* not sub amt disc? */       //cc050906
		{
		    if (!ApplVar.FRegi || ApplVar.FRefund || ApplVar.FCorr)//ccr090812!!!!!!!!
		    {
				ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
				return;
		    }
		    if (ApplVar.FSplit)
		    {
				ApplVar.ErrorNumber=ERROR_ID(CWXXI29);
				return;
		    }
		}
		if (ApplVar.FPb)
		    ApplVar.Disc.Print = ApplVar.PbF.Print;

		if (Appl_EntryCounter && (TESTBIT(ApplVar.Disc.Options, BIT0 + BIT1) || FRegSuspend))     /* open or open/fixed? */
		{
            //ccr2016-11-07>>>>>>>>
		    ApplVar.Qty1 = ZERO;
		    memcpy(ApplVar.Qty1.Value, ApplVar.Disc.Max, sizeof(ApplVar.Disc.Max));
		    if (CompareBCD(&ApplVar.Entry, &ApplVar.Qty1) > 0)
		    {
				ApplVar.ErrorNumber=ERROR_ID(CWXXI18);
				return;
		    }
            //ccr2016-11-07<<<<<<<<
		    if (!TESTBIT(ApplVar.Disc.Options, BIT2))   /* percentage ? */
				ChangePoint2();
			else
				ChangePoint();

/*
		    if (ApplVar.DecimalPoint || AmtInputMask())
		    {
				ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
				return;
		    }
*/
		}
		else if (!Appl_EntryCounter && !TESTBIT(ApplVar.Disc.Options, BIT0))     /* fixed? */
		{
		    ApplVar.Entry = ZERO;
		    memcpy(ApplVar.Entry.Value, ApplVar.Disc.Fixed, sizeof(ApplVar.Disc.Fixed));
				//ChangePoint2();
				//ChangePoint();

		}
#if (DD_CHIPC==1)
#if (BARCUSTOMER==1)
		else if (!TESTBIT(ApplVar.IC.ICState,IC_DISCOUNT) //ccr chipcard
			&& !(ApplVar.BarCustomer.OFFIndex>0 && ApplVar.BarCustomer.DeptHi==0 && TESTBIT(ApplVar.BarCustomer.Dept,BIT0)))
#else
		else if (!TESTBIT(ApplVar.IC.ICState,IC_DISCOUNT))//ccr chipcard
#endif
#else
        else
#endif
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
		    return;
		}
		if (TESTBIT(ApplVar.Disc.Options, BIT4))    /* item or subtotal */
		{
		    if (ApplVar.RGRec.Key.Code < DEPT || !ApplVar.RepeatCount || ApplVar.FMixMatch)  /* no item */
		    {
				ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
				return;
		    }
			sign = ApplVar.Qty.Sign & 0x80; /* when negative then corr or refund used !!*/
		}
		else
		{
			if (!ApplVar.FRegi)
				ApplVar.Amt = ZERO;
		    else if (!TESTBIT(ApplVar.Disc.Options, BIT6 + BIT7))    /* items 1 */
		    {
				ApplVar.Amt = ApplVar.SaleAmt;
	/* JB Add */
				if ((!TESTBIT(PBINFO, BIT3) || ApplVar.FPb) && ApplVar.PbNumber && !ApplVar.FSplit)    /* fast ApplVar.PB ? */
				{
					PbTotal(ApplVar.PbNumber, 0);       /* read total */
					Add(&ApplVar.Amt, &ApplVar.PB.Amt);
				}
		    }
		    else
		    {
				ApplVar.Amt = ZERO;
				if (TESTBIT(ApplVar.Disc.Options, BIT6))    /* items 1 */
				{
				    d1 = ApplVar.DiscItem[0];
				    if (/* TESTBIT(PBINFO, BIT3) && !ApplVar.FPb && */ ApplVar.PbNumber && !ApplVar.FSplit)    /* fast ApplVar.PB ? */
					Add(&d1, &ApplVar.PB.Disc1);
				    Add(&ApplVar.Amt, &d1);
				}
				if (TESTBIT(ApplVar.Disc.Options, BIT7))    /* items 2 */
				{
				    d2 = ApplVar.DiscItem[1];
				    if (/* TESTBIT(PBINFO, BIT3) && !ApplVar.FPb && */ ApplVar.PbNumber && !ApplVar.FSplit)    /* fast ApplVar.PB ? */
						Add(&d2, &ApplVar.PB.Disc2);
				    Add(&ApplVar.Amt, &d2);
				}
		    }
		}
		if (TESTBIT(ApplVar.Disc.Options, BIT2))    /* amount or percentage ? */
		{ /* amount */
            //ccr20130809折扣金额大于销售金额)>>>>>>>
            if (!TESTBIT(ApplVar.Disc.Options, BIT3)
                && CompareBCD(&ApplVar.Entry,&ApplVar.Amt)>0)
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                return;
            }
            //<<<<<<<<<<<<<<<<<<<<
		    ApplVar.Amt = ApplVar.Entry;
		    if (TESTBIT(ApplVar.Disc.Options, BIT4))    /* item */
				ApplVar.Amt.Sign = sign;
		}
		else
		{    /* Percentage */
		    ApplVar.Entry.Sign = 0x04;    /* 4 decimals */
		    Multiply(&ApplVar.Amt, &ApplVar.Entry);
		    RoundBcd(&ApplVar.Amt, 0);
		    AmtRound(1, &ApplVar.Amt);
		}

		if (!TESTBIT(ApplVar.Disc.Options, BIT3))    /* Positive or negative */
		    ApplVar.Amt.Sign ^= 0x80;

//cc 2006-10-12 for SubDisc>>>>>>>>>>>>>>>>>>>>>>>>
		if(TESTBIT(ApplVar.Disc.Options,BIT4))
		{
			if (TESTBIT(ApplVar.Dept.Options, BIT6))    /* items 1 */
				Add(&ApplVar.DiscItem[0],&ApplVar.Amt);
			if (TESTBIT(ApplVar.Dept.Options, BIT7))    /* items 2 */
				Add(&ApplVar.DiscItem[1],&ApplVar.Amt);
		}
//cc 2006-10-12 for SubDisc<<<<<<<<<<<<<<<<<<<<<<<<
    }
    ApplVar.PrintLayOut = ApplVar.Disc.Print;
    if (ApplVar.FVoid || ApplVar.FCorr || ApplVar.FRefund)
    {
        ApplVar.Amt.Sign ^= 0x80;

    	SKC = ApplVar.RGRec.Key.Code;
    	if(ApplVar.DiscNumber == 2)
    	{
    		ApplVar.RGRec.Amt.Sign ^= 0x80;
    		ApplVar.RGRec.Key.Code = DISC+3;
    		StoreInBuffer();
    		ApplVar.RGRec.Amt = ZERO;
    	}
    	if(ApplVar.DiscNumber == 3)
    	{
    		ApplVar.RGRec.Amt.Sign ^= 0x80;
    		ApplVar.RGRec.Key.Code = DISC+4;
    		StoreInBuffer();
    		ApplVar.RGRec.Amt = ZERO;
    	}
    	ApplVar.RGRec.Key.Code = SKC;

    }
    if (ApplVar.FCanc && TESTBIT(ApplVar.Disc.Options, BIT2))    /* amount ? */
		ApplVar.Amt.Sign ^= 0x80;
    if (RegiStart())
		return;  /* check if start of registration */

    if (TESTBIT(ApplVar.Disc.Options, BIT4))    /* item then no consolidation */
		ApplVar.BufCmd = 0;
    StoreInBuffer();

    ApplVar.RGRec.Key.Code = ApplVar.Key.Code;
    if (TESTBIT(ApplVar.Disc.Options, BIT2))    /* amount or percentage ? */
		ApplVar.RGRec.Qty = ZERO;
    else
		ApplVar.RGRec.Qty = ApplVar.Entry;
    ApplVar.RGRec.Amt = ApplVar.Amt;

//cc 2006-10-20 for SubDisc>>>>>>>>>>>>>>>>
	if(TESTBIT(ApplVar.Disc.Options,BIT4) && !ApplVar.FVoid)
	{
		sSign = ApplVar.Amt.Sign;
//		if(TESTBIT(ApplVar.SubDiscFlag,BIT2))
//			ApplVar.Amt.Sign ^= 0x80;
		Add(&ApplVar.RGItems.RGBuf[ApplVar.RGItems.RGNumber-1].DiscAmt,&ApplVar.Amt);
		ApplVar.Amt.Sign = sSign;
	}
//cc 2006-10-20 for SubDisc<<<<<<<<<<<<<<<<<

    if (ApplVar.FCorr || (sign && (ApplVar.Correc.Options & 0x07) == 1)) /* correction ? */
    {
		ApplVar.BufCmd = 2;         /* only check if used */
		StoreInBuffer();    /* check if discount used */
		if (ApplVar.ErrorNumber)
		{
			ApplVar.RGRec.Key.Code = 0;
	    	return;
		}
    }
    if (StorePbBuffer())     /* store in ApplVar.PB buffer */
    {
		ApplVar.RGRec.Key.Code = 0;
		return;
    }
    if (ApplVar.FCorr || ApplVar.FRefund)
    {
		ApplVar.RGRec.Key.Code = CORREC + 1 + ApplVar.CorrecNumber;
		StoreInBuffer();      /* store correction or refund function */
		ApplVar.RGRec.Key.Code = ApplVar.Key.Code;
    }
#if defined(LCD10_DISP)
    LCD10_SetFlag(FLAG_NULL);
    PutsO(DispAmtStr(0, &ApplVar.Amt,DISLEN));//C-DISPLAY
#else
    PutsO(DispAmtStr(Msg[DISCOUNT].str, &ApplVar.Amt,DISLEN));//C-DISPLAY
#endif
    if (ApplVar.FVoid)
    {
		Add(&ApplVar.RGItems.RGBuf[ApplVar.RGItems.RGNumber-3].DiscAmt,&ApplVar.Amt);	/* cc 2006-10-28 for SubDisc */
        ApplVar.TaxTemp = ApplVar.TaxTemp1;     /* restore temp tax shift */
    }
    if (ApplVar.FVoid || ApplVar.FCorr || ApplVar.FRefund) /* void */
    {
        ApplVar.PrintLayOut = ApplVar.Correc.Print;
        PrintStr(ApplVar.Correc.Name);
        ApplVar.PrintLayOut = ApplVar.Disc.Print;
    }
    else if ((ApplVar.Disc.Options & (BIT4 + BIT5)) == BIT5)
    {           /* subtotal  and print Subtotal ?*/
        if (!TESTBIT(ApplVar.Disc.Options, BIT6 + BIT7)){    /* items 1 */
			if(ApplVar.DiscNumber == 8 || ApplVar.DiscNumber == 9){ //jdb2018-04-28 SC PWD 不打印TOTAL
			}else{
			PrintLine('-');
            PrintAmt(Msg[YINGFU].str, &ApplVar.SaleAmt);
			}
        }
        else
        {	
        	PrintLine('-');
            if (TESTBIT(ApplVar.Disc.Options, BIT6))    /* items 1 */
                PrintAmt(Msg[ZHKOU1].str, &d1);
            if (TESTBIT(ApplVar.Disc.Options, BIT7))    /* items 2 */
                PrintAmt(Msg[ZHKOU2].str, &d2);
        }
    }

	MemSet(ProgLineMes, sizeof(ProgLineMes), ' ');
	/*
	if(ApplVar.DiscNumber == 8){ //jdb2018-03-30 sc pwd 不限制长度
		PrintLine('.'); //jdb2018-04-27 增加隔行
		strcpy(ProgLineMes,SCDISCNAME);
		i = strlen(SCDISCNAME); //jdb2018-07-31
		//i = 11;
	}else if(ApplVar.DiscNumber == 9){
		PrintLine('.'); //jdb2018-04-27 增加隔行
		strcpy(ProgLineMes,PWDDISCNAME);
		i = strlen(PWDDISCNAME);//jdb2018-07-31
		//i = 12;
	}*/
	//jdb2018-08-09 打印SC PWD名字
	if(ApplVar.DiscNumber == 8 || ApplVar.DiscNumber == 9){
		PrintLine('.');
		i = strlen(ApplVar.Disc.Name);
		if(i > 15)//限制名字长度为15
			i = 15;
		strncpy(ProgLineMes,ApplVar.Disc.Name,i);
	}
	else{
	i = strlen(ApplVar.Disc.Name);
	if (i>10)/*限制折扣名称长度为10*/
		i = 10;
	strncpy(ProgLineMes,ApplVar.Disc.Name,i);
	}
	ProgLineMes[i]=0;
    DispStrLF(FormatAmtStr(ProgLineMes,&ApplVar.Amt,SCREENWD));/*ccr20130425 显示折扣金额 */

    if (!TESTBIT(ApplVar.Disc.Options, BIT2) && /* percentage ? */
		(TESTBIT(ApplVar.Disc.Options, BIT0 + BIT1) || FRegSuspend)  /* and open, open/fixed */
		|| (ApplVar.DiscNumber == 8 || ApplVar.DiscNumber == 9)) //jdb2018-07-31 SC PWD显示折扣率
    {//为挂起销售是,总认为是输入折扣
    	ProgLineMes[i]=' ';
		if(ApplVar.FVoid)
		{
		    BcdDiv10(&ApplVar.Entry);
            BcdDiv10(&ApplVar.Entry);
		}
		ApplVar.Entry.Sign = 0x02;
		FormatQty(ProgLineMes + 14, &ApplVar.Entry);	 //折扣率
		ProgLineMes[15] = '%';
		i = 21;
    }
	ProgLineMes[i] = 0;
    PrintRJ(FormatAmtStr(ProgLineMes, &ApplVar.Amt, PRTLEN));//ccr20130425
//ccr20130425    PrintAmt(ProgLineMes, &ApplVar.Amt);

    ApplVar.Qty = ZERO;
	if(ApplVar.DiscNumber == 8 || ApplVar.DiscNumber == 9){ //jdb2018-04-03
		//SETBIT(ApplVar.SubDiscFlag,BIT0);
		ApplVar.RetQty = ZERO;
		ApplVar.Cost = ZERO;
		ApplVar.DiscAmt = ApplVar.Amt;
		//jdb2018-04-03
		#if 0
		Val1 = ApplVar.Amt;
		Val2 = ApplVar.DiscAmt;
		ApplVar.VatAmtTotal.Sign |= 0x80;
		Add(&ApplVar.Amt, &ApplVar.VatAmtTotal);
		Add(&ApplVar.DiscAmt, &ApplVar.VatAmtTotal);
		#endif
		
		AddGroupTotal();
		CalculateScPwdDisc(0);//jdb2018-08-09 计算单品，部类折扣
		ApplVar.DiscAmt = ZERO;
		//ApplVar.Amt = Val1;
		Add(&ApplVar.SaleAmt, &ApplVar.Amt);
		//Add(&ApplVar.SubTotal, &ApplVar.Amt);
	}
    else if (TESTBIT(ApplVar.Disc.Options, BIT4))    /* subtotal or item */
    {//only to 1 item
		ApplVar.BufCmd = 0;             /* don't consolidate item discounts */
	    if (!TESTBIT(KEYTONE, BIT4))    /* only add in function ? */
		{
			ApplVar.RetQty = ZERO;
			ApplVar.Cost = ZERO;
			ApplVar.DiscAmt = ApplVar.Amt;
			//jdb2018-03-21
			if(ApplVar.DiscNumber == 8 || ApplVar.DiscNumber == 9){
				//为SC PWD折扣
			}else{
				AddGroupTotal();
		    	AddDeptTotal();
				if (ApplVar.FPlu)   /* PLU ? */
					AddPluTotal();
			}
			ApplVar.DiscAmt = ZERO;
		}
		AddTaxItem(ApplVar.Dept.Tax & (ApplVar.Disc.Tax ^ 0xff));       /* add to tax itemizers */
    }
    else
/* cc 2006-10-28 for SubDisc>>>>>>>>>>>>>>>>>>>>>>*/
#if (1)
	{//discount for subtotal
        SETBIT(ApplVar.SubDiscFlag,BIT0);//ccr20130809
		if (!TESTBIT(ApplVar.Disc.Options, BIT2))    /* percentage ? */
			CalculateSubDisc(0);	/* cc 2006-10-10 for subDisc */
		else
		{
			ApplVar.SUB_DiscTot = ZERO;
			CalculateSubDisc(2);
		}

		if (ApplVar.Disc.Tax)           /* calculate discount into ApplVar.Tax itemizers */
		{
			CalculateSubDisc(4);
			Add(&ApplVar.SaleAmt, &ApplVar.Amt);        /* also add to Sales ApplVar.Total */
		}
	}
#else /* cc 2006-10-28 for SubDisc<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    {
		if (ApplVar.Disc.Tax)           /* calculate discount into ApplVar.Tax itemizers */
			if (!TESTBIT(ApplVar.Disc.Options, BIT2))    /* percentage ? */
			{
			    ApplVar.Entry.Sign = 0x04;    /* 4 decimals */
			    for (i = 0; i < 8; i++)
			    {
						if (ApplVar.Disc.Tax & (0x01 << i))
						{
							d1 = ApplVar.TaxItem[i];
						    Multiply(&d1, &ApplVar.Entry);
							RoundBcd(&d1, 0);
						    AmtRound(1, &d1);
							if (!TESTBIT(ApplVar.Disc.Options, BIT3))    /* Positive or negative */
							    d1.Sign ^= 0x80;
						    if (ApplVar.FVoid)
							    d1.Sign ^= 0x80;
						    Add(&ApplVar.TaxItem[i], &d1);
						}
			    }
			}
			else
			{
			    d1 = ZERO;
			    for (i = 0; i < 8; i++)
					{
						if (ApplVar.Disc.Tax & (0x01 << i))
							Add(&d1, &ApplVar.TaxItem[i]);
					}
			    if (CheckNotZero(&d1))     /* active ? */
			    {
					for (i = 0; i < 8; i++)
					{
						if ((ApplVar.Disc.Tax & (0x01 << i)) && CheckNotZero(&ApplVar.TaxItem[i]))
						    if (CheckNotZero(&ApplVar.TaxItem[i]))
						    {
								d2 = ApplVar.TaxItem[i];
								Divide(&d2, &d1);
								Multiply(&d2, &ApplVar.Amt);
								RoundBcd(&d2, 0);
								d2.Sign &= 0xfc;
								AmtRound(1, &d2);
								Add(&ApplVar.TaxItem[i], &d2);  /* sign is already oke */
						    }
					}
			    }
			}
			Add(&ApplVar.SaleAmt, &ApplVar.Amt);        /* also add to Sales ApplVar.Total */
    }
	if (sign)
	/* when sign 0x80 then Item refund or correction !!*/
	{
		ApplVar.Qty = ZERO;
		AddCorrecTotal();
	}
#endif

    ApplVar.Qty = ONE;
	ApplVar.Qty.Sign = sign;                /* Set sign incase Item Corr or Refund */
	if (ApplVar.FCanc)
		ApplVar.Qty.Sign ^= 0x80;
	if (ApplVar.FVoid || ApplVar.FCorr || ApplVar.FRefund)
    {
		ApplVar.Qty.Sign ^= 0x80;
		AddCorrecTotal();
    }
	if(ApplVar.FPlu == 0)
		memset(ApplVar.Plu.Random, 0, sizeof(ApplVar.Plu.Random));

	if(ApplVar.FSub)								   //lyq2003
	{//小计后折扣
		if(!TESTBIT(ApplVar.Disc.Options,BIT2))//ccr20130905
		 {//%折扣
			Collect_Data(SUBDISCADDLOG);
		 }
		 else
		 {//直接折扣
			Collect_Data(SUBDIRECTLOG);
		 }
	}
	else
	{
		if(!TESTBIT(ApplVar.Disc.Options,BIT2))//ccr20130905
		 {//%折扣
			Collect_Data(DISCADDLOG);
		 }
		 else
		 {//直接折扣
			Collect_Data(DIRECTLOG);
		 }
	}

    AddDiscTotal();

    //lyq2004 added for don't print the message of void discount start
    if(ApplVar.FRegi && ApplVar.FVoid && (TESTBIT(ApplVar.Disc.Options, BIT4)))
    	ApplVar.RGItems.RGNumber -=2;
    //lyq2004 added for don't print the message of void discount end

    ApplVar.FSale = 1;
    ApplVar.FVoid = 0;
    ApplVar.FCorr = 0;
	ApplVar.FSub = 0;       /* reset subtotal  */	   //lyq2003
    ApplVar.FRefund = 0;
    ApplVar.RepeatCount = 0;
    if ((ApplVar.Disc.Options & (BIT2+BIT4)) != BIT2) /* not sub amt disc? */
		ApplVar.BufCmd = 0;     /* don't consolidate discounts */

	DispSubTotal();
}
