#define TAX_C 5
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "Message.h"

#define FOR_CANADA  0   //ccr2014-08-25 当销售金额<=4.00时,不计税.

void GetTaxOffSet()
{
	RamOffSet = ApplVar.TaxNumber * ApplVar.AP.Tax.RecordSize + ApplVar.AP.StartAddress[AddrTax];
}

void AddTaxTotal()
{

	GetTaxOffSet();
	RamOffSet += ApplVar.AP.Tax.TotalOffSet;
	for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
	{
		ApplVar.Size = ApplVar.AP.Tax.Size[ApplVar.PointerType];
		AddPointerTotal();
	}
}

void WriteTax()
{
	if (ApplVar.TaxNumber < ApplVar.AP.Tax.Number)
	{
		GetTaxOffSet();
		WriteRam(ApplVar.Tax.Name, ApplVar.AP.Tax.CapSize);
		WriteRam(ApplVar.Tax.Rate, sizeof(ApplVar.Tax.Rate));
		WriteRam(&ApplVar.Tax.Options, sizeof(ApplVar.Tax.Options));
	}
}
//Read ApplVar.Tax from area of TAX_C
void ReadTax()
{
	GetTaxOffSet();

	ReadRam(ApplVar.Tax.Name, ApplVar.AP.Tax.CapSize);
	ApplVar.Tax.Name[ApplVar.AP.Tax.CapSize]=0;
	ReadRam(ApplVar.Tax.Rate, sizeof(ApplVar.Tax.Rate));
	ReadRam(&ApplVar.Tax.Options, sizeof(ApplVar.Tax.Options));
}

void AddTaxItem(BYTE tax)
{
	BYTE t, i;

	t = 1;
	tax ^= ApplVar.TaxPerm;	/* shift tax 1 & 2 if needed */
	tax ^= ApplVar.TaxTemp;

	for (i = 0; i < 8; i++)
	{
		if (tax & t)
		{
			Add(&ApplVar.TaxItem[i], &ApplVar.Amt);
		}
		t <<= 1;
	}
	
#if NETVATVERSION //jdb2018-08-11
	if(tax == 0x0){ //jdb2018-05-03 记录零税销售额
		Add(&ApplVar.ZEROAMOUNT, &ApplVar.Amt);
		if (TESTBIT(ApplVar.FZV, BIT3) && (ApplVar.FVoid || ApplVar.FCorr)) //jdb2018-05-03 为按了免税或零税键后取消更正交易
		{
			//Add(&ApplVar.LessVatAmt, &ApplVar.Amt); //jdb2018-05-03 免税或零税毛销售额减去Amt;
			Add(&ApplVar.ZEROLessVatAmt, &ApplVar.Amt); //jdb2018-05-03 免税或零税毛销售额减去Amt;
		}
	}
#endif
	
	Add(&ApplVar.SaleAmt, &ApplVar.Amt);
	Add(&ApplVar.SaleQty, &ApplVar.Qty);
	ApplVar.TaxTemp1 = ApplVar.TaxTemp;     /* save article tax shift */
	ApplVar.TaxTemp = 0;	    /* reset article tax shift */
}
//type=0:Total sales rounding;
//type=1:Rounding on MULT, TAX & Percentage discount

void AmtRound(BYTE type ,BCD *amt)
{
	BYTE b, sign;

	if ((!type && TESTBIT(ROUNDMASK, BIT0+BIT1+BIT2+BIT3)) ||
			(type && TESTBIT(ROUNDMASK, BIT4+BIT5+BIT6+BIT7)))   /* Detail rounding?*/
	{

		sign = amt->Sign;	/* save sign */
		amt->Sign = 0;
		b = ROUNDMASK & 0x0f;
		if (!b)
			b = (ROUNDMASK>>4) & 0x0f;
		switch (b){
			case 1:
				if ((amt->Value[0] & 0x0f)>=0x05)
					Add(amt,(BCD*)&TEN);
				amt->Value[0] &= 0xf0;
				break;
			case 2:
				b = amt->Value[0];
				amt->Value[0] = 0;		/* 00 - 24 -->0 */
				if (b > 0x74)			/* 75 - 99  --> 100 */
					Add(amt, (BCD*)&HUNDRED);
				else if (b > 0x24)		/* 25 - 74 --> 50 */
					amt->Value[0] = 0x50;
				break;
			case 3:
				b = amt->Value[0] & 0xf0;
				amt->Value[0] = 0;		/* 00 - 49 -->0 */
				if (b >= 0x50)
					Add(amt, (BCD*)&HUNDRED);/* 50 - 99  --> 100 */
				break;
			case 4:
				b = amt->Value[0] & 0xf0;
				amt->Value[0] = 0;		/* 00 - 49 -->0 */
				if (b > 0x00)
					Add(amt, (BCD*)&HUNDRED);/* 50 - 99  --> 100 */
				break;
		}
		amt->Sign = sign;	    /* restore sign */
	}
}

void RoundTaxable(BCD *amt)
{
	RoundBcd(amt, 0);
	AmtRound(1, amt);
}

/* if type = 0 then add in report and calculate add on tax */
/* if type = 1 then not add in report and calculate add on tax */
/* if type = 2 then calculate and print add on tax not report */
/* if type = 3 then print inclusive tax */
/* GST is a special tax which is always ADD ON and calculated as */
/* a percentage on (SalesAmt + TAX1 + TAX2 ..)	*/

#if FOR_CANADA
BCD BCD_4={0,{0,0x04,0,0,0,0,0,0}};
#endif

void CalculateTax(BYTE type )
{
	int len;
	BYTE printsub;
	BYTE calcgst, SaveSign;
	BCD gst,sVal1;//ccr20130922
#if NETVATVERSION //jdb2018-08-11
	BCD LessVat, sVal2, sVal3, sVal4;//jdb2018-03-09
	BYTE FVAMOUNT = 0; //jdb-05-02 关联了税打印标志
#endif
#if FOR_CANADA
	BYTE sCAxTAX,sCAPrint;//加拿大<4.00不计税,但是打印
#endif
#if (defined(CASE_PERU)) //ccr2016-10-24>>>>>>>>>>>>>>
	BYTE   printline=0;
#endif
	printsub = 0;
	calcgst = 0;    /* checkif tax used for GST (tax on total sales amount)*/
	if (!type)
		AmtRound(0, &ApplVar.SaleAmt);	/* sales amount rounding */

#if NETVATVERSION //jdb2018-08-11
	if(type == 6){ //jdb2018-04-09
		Add(&ApplVar.SaleAmt, &ApplVar.Amt);
		Add(&ApplVar.SaleQty, &ApplVar.Qty);
		//CORREC
		if(CheckNotZero(&ApplVar.NetVatAmt)){
			Add(&ApplVar.LessVatAmt, &ApplVar.Amt); //jdb2018-04-27 免税或零税毛销售额减去Amt;
			Subtract(&ApplVar.LessVatAmt, &ApplVar.NetVatAmt);
		
			
			if(TESTBIT(ApplVar.FZV, BIT0)){ //jdb2018-07-03 免税
				ApplVar.NetVatAmt.Sign = ApplVar.TaxItem[ApplVar.AP.Tax.Number - 1].Sign;
				Subtract(&ApplVar.TaxItem[ApplVar.AP.Tax.Number - 1], &ApplVar.NetVatAmt);
			}
			else if(TESTBIT(ApplVar.FZV, BIT1)){ //jdb2018-07-03 零税
				ApplVar.NetVatAmt.Sign = ApplVar.TaxItem[ApplVar.AP.Tax.Number - 2].Sign;
				Subtract(&ApplVar.TaxItem[ApplVar.AP.Tax.Number - 2], &ApplVar.NetVatAmt);
			}
			ApplVar.NetVatAmt = ZERO;
		}
		//VOID jdb2018-04-10
		if(ApplVar.FVoid && CheckNotZero(&ApplVar.LastNetVatAmt)){
			Add(&ApplVar.LessVatAmt, &ApplVar.Amt); //jdb2018-04-27 免税或零税毛销售额减去Amt;
			Subtract(&ApplVar.LessVatAmt, &ApplVar.LastNetVatAmt);
		
			SaveSign = ApplVar.LastNetVatAmt.Sign;
			
			if(TESTBIT(ApplVar.FZV, BIT0)){ //jdb2018-07-03 免税
				ApplVar.LastNetVatAmt.Sign = ApplVar.TaxItem[ApplVar.AP.Tax.Number - 1].Sign;
				Subtract(&ApplVar.TaxItem[ApplVar.AP.Tax.Number - 1], &ApplVar.LastNetVatAmt);
			}
			else if(TESTBIT(ApplVar.FZV, BIT1)){ //jdb2018-07-03 零税
				ApplVar.LastNetVatAmt.Sign = ApplVar.TaxItem[ApplVar.AP.Tax.Number - 2].Sign;
				Subtract(&ApplVar.TaxItem[ApplVar.AP.Tax.Number - 2], &ApplVar.LastNetVatAmt);
			}
			ApplVar.LastNetVatAmt.Sign = SaveSign;
		}
		return;
	}
#endif

#if NETVATVERSION //jdb2018-08-11
	//jdb2018-03-19
	if(type == 5){
		if(TESTBIT(ApplVar.FZV, BIT0)){
			sVal1 = ApplVar.TaxItem[ApplVar.AP.Tax.Number - 2]; //jdb2018-04-18 保存零税率销售额
			sVal2 = ApplVar.TaxItem[ApplVar.AP.Tax.Number - 1]; //jdb2018-04-03
			memset(ApplVar.TaxItem, 0, sizeof(ApplVar.TaxItem));
			ApplVar.TaxItem[ApplVar.AP.Tax.Number - 2] = sVal1; //jdb2018-04-18
		}else if(TESTBIT(ApplVar.FZV, BIT1)){ //零税销售
			sVal1 = ApplVar.TaxItem[ApplVar.AP.Tax.Number - 1]; //jdb2018-04-24 保存免税率销售额
			sVal2 = ApplVar.TaxItem[ApplVar.AP.Tax.Number - 2];
			memset(ApplVar.TaxItem, 0, sizeof(ApplVar.TaxItem));
			ApplVar.TaxItem[ApplVar.AP.Tax.Number - 1] = sVal1; //jdb2018-04-24 恢复免税率销售额
		}
		sVal1 = ZERO;

		if(ApplVar.FReVat){ //jdb2018-04-12 退货操作
			ApplVar.LessVatTotalAmt.Sign ^= 0x80;
		}

		if(TESTBIT(ApplVar.FZV, BIT0)){ //免税销售
			ApplVar.TaxItem[ApplVar.AP.Tax.Number - 1] = ApplVar.LessVatTotalAmt;
			Add(&ApplVar.TaxItem[ApplVar.AP.Tax.Number - 1], &sVal2); //jdb2018-04-03
		}else if(TESTBIT(ApplVar.FZV, BIT1)){ //零税销售
			ApplVar.TaxItem[ApplVar.AP.Tax.Number - 2] = ApplVar.LessVatTotalAmt;
			Add(&ApplVar.TaxItem[ApplVar.AP.Tax.Number - 2], &sVal2);
		}


		//if(TESTBIT(ECRVIP.ECRVIPState, BIT4)){
		if(TESTBIT(ECRVIP.ECRVIPState, BIT4)){
			//memset(ProgLineMes, ' ', sizeof(ProgLineMes));
			//strcpy(ProgLineMes,"VAT EXEMPT AMOUNT");
			//PrintAmt(ProgLineMes, &ApplVar.LessVatTotalAmt); //打印总减税额
			//PrintLine('.');
			sVal2 = ApplVar.SaleAmt;
			sVal3 = ApplVar.LessVatTotalAmt;
			Add(&sVal2, &sVal3);

			if(!TESTBIT(ApplVar.FZV, BIT4)) //jdb2018-05-02 没有打印 VATABLE VATAMOUNT 打印零
			{
				PrintLine('.');
				sVal4 = ZERO;
				
				memset(ProgLineMes, ' ', sizeof(ProgLineMes));
				strcpy(ProgLineMes,VATABLESALES);
				PrintAmt(ProgLineMes, &sVal4);

				memset(ProgLineMes, ' ', sizeof(ProgLineMes));
				strcpy(ProgLineMes,VATAMOUNT);
				PrintAmt(ProgLineMes, &sVal4);
			}

			if(TESTBIT(ApplVar.FZV, BIT0)){
				memset(ProgLineMes, ' ', sizeof(ProgLineMes));
				strcpy(ProgLineMes,ZERORATEDSALES);
				PrintAmt(ProgLineMes, &sVal1);

				memset(ProgLineMes, ' ', sizeof(ProgLineMes));
				strcpy(ProgLineMes,VATEXEMPTSALES);
				#if 0
				PrintAmt(ProgLineMes, &sVal2);
				#else
				sVal3 = ApplVar.LastVatExempt;
				ApplVar.LastVatExempt = sVal2; //jdb2018-06-14 保存这次的值
				
				Subtract(&sVal2, &sVal3);
				PrintAmt(ProgLineMes, &sVal2);
				#endif

				memset(ProgLineMes, ' ', sizeof(ProgLineMes));
				strcpy(ProgLineMes,VATEXEMPTAMOUNT);
				//jdb2018-07-26 取消打印
				//PrintAmt(ProgLineMes, &ApplVar.LessVatTotalAmt); //打印总减税额

				if(CheckNotZero(&ApplVar.ZEROAMOUNT)) //jdb2018-05-03 没有关联税也加入报表
				{
					Add(&ApplVar.LessVatAmt, &ApplVar.ZEROAMOUNT);
				}

				//jdb2018-07-19 写入免税流水
				sVal1 = ApplVar.Amt;
				sVal4 = ApplVar.Cost;
				ApplVar.Amt = sVal2;
				ApplVar.Cost = ApplVar.LessVatTotalAmt;
				Collect_Data(VATLOG);
				ApplVar.Amt = sVal1;
				ApplVar.Cost = sVal4;
			}else if(TESTBIT(ApplVar.FZV, BIT1)){ //零税销售
				memset(ProgLineMes, ' ', sizeof(ProgLineMes));
				strcpy(ProgLineMes,ZERORATEDSALES);
				PrintAmt(ProgLineMes, &sVal2);

				memset(ProgLineMes, ' ', sizeof(ProgLineMes));
				strcpy(ProgLineMes,VATEXEMPTSALES);
				PrintAmt(ProgLineMes, &sVal1);

				if(CheckNotZero(&ApplVar.ZEROAMOUNT)) //jdb2018-05-03 没有关联税也加入报表
				{
					//Add(&ApplVar.LessVatAmt, &ApplVar.ZEROAMOUNT);
					ApplVar.ZEROLessVatAmt = ApplVar.ZEROAMOUNT;
				}

				//jdb2018-07-19 写入零税流水
				sVal3 = ApplVar.Amt;
				sVal4 = ApplVar.Cost;
				ApplVar.Amt = sVal2;
				ApplVar.Cost = ZERO;
				Collect_Data(ZEROLOG);
				ApplVar.Amt = sVal3;
				ApplVar.Cost = sVal4;
			}
		}

		Add(&ApplVar.SaleAmt, &ApplVar.LessVatTotalAmt);
		Add(&ApplVar.SubTotal, &ApplVar.LessVatTotalAmt);
		Add(&ApplVar.VatAmtTotal, &ApplVar.LessVatTotalAmt);
		ApplVar.LessVatTotalAmt = ZERO;

#if 1 //jdb2018-04-03
		sVal1 = ApplVar.Amt;
		sVal2 = ApplVar.DiscAmt;
		sVal3 = ApplVar.RetQty;
		sVal4 = ApplVar.Cost;

		ApplVar.RetQty = ZERO;
		ApplVar.Cost = ZERO;
		//ApplVar.VatAmtTotal.Sign |= 0x80;
		ApplVar.Amt = ApplVar.VatAmtTotal;
		ApplVar.DiscAmt = ApplVar.VatAmtTotal;

		AddGroupTotal();

		ApplVar.Amt = sVal1;
		ApplVar.DiscAmt = sVal2;
		ApplVar.RetQty = sVal3;
		ApplVar.Cost = sVal4;

		ApplVar.VatAmtTotal = ZERO;
#endif

		return;
	}
#endif

	ApplVar.SubTotal = ApplVar.SaleAmt;
#if FOR_CANADA
	if (CompareBCD(&ApplVar.SubTotal, &BCD_4) < 0)//为加拿大,销售金额<4.00时不计税
		sCAxTAX = 1;//		MemSet(ApplVar.TaxItem, sizeof(ApplVar.TaxItem), 0);//去掉计税统计
	else
		sCAxTAX = 0;
#endif

	if (ApplVar.AP.Tax.Number && !ApplVar.FNoTax)   /* if ApplVar.FNoTax == 1 then no tax */
	{
		for(;;)
		{
			for (ApplVar.TaxNumber = 0; ApplVar.TaxNumber < ApplVar.AP.Tax.Number; ApplVar.TaxNumber++)
			{
				ReadTax();
				if (TESTBIT(ApplVar.Tax.Options, BIT3))        /* GST tax ? */
				{
					if (calcgst == 2)
					{
						ApplVar.Amt = gst;		 /* Taxable is Saleamt + ApplVar.Tax */
						ApplVar.TaxItem[ApplVar.TaxNumber] = ApplVar.Amt;   /* used for external invoice */
					}
					else
					{
						calcgst = 1;		/* set GST active */
						continue;
					}
				}//if
				else if (calcgst == 2)
					continue;
				else
					ApplVar.Amt = ApplVar.TaxItem[ApplVar.TaxNumber];/* 读取税种销售金额累计 */
				ApplVar.Cost = ZERO;	    /* clear calculated tax */
				ApplVar.DiscAmt = ZERO; //jdb2018-04-18
#if NETVATVERSION //jdb2018-08-11
				//jdb2018-03-12
				LessVat = ZERO;
#endif
				if (CheckNotZero(&ApplVar.Amt))
				{
#if FOR_CANADA
					if (sCAxTAX)//销售金额<4,不计税
						ApplVar.Amt = ZERO;
					sCAPrint = 1;//但是打印
#endif
					ApplVar.Qty = ZERO;

					memcpy(ApplVar.Qty.Value, ApplVar.Tax.Rate, sizeof(ApplVar.Tax.Rate));
					if (!TESTBIT(ApplVar.Tax.Options, BIT0)) /* add on ? */
						//if (!TESTBIT(ApplVar.Tax.Options, BIT0) && ApplVar.TaxNumber != 6) //jdb2018-04-18 第7种税为价内税零税率
					{
						ApplVar.Cost = ApplVar.Amt;
						ApplVar.Qty.Sign = 0x04;
						Multiply(&ApplVar.Cost, &ApplVar.Qty);
						RoundBcd(&ApplVar.Cost, 0);
						AmtRound(1, &ApplVar.Cost);
						Add(&ApplVar.SubTotal, &ApplVar.Cost);
						//if(type == 2) //jdb2018-03-28
						//Add(&ApplVar.VatAmtTotal, &ApplVar.Cost); //jdb2018-03-28

#if NETVATVERSION //jdb2018-08-11
						if(type == 2) //jdb2018-03-27
						{
							if(TESTBIT(ECRVIP.ECRVIPState, BIT4)){
								Add(&ApplVar.SaleAmt, &ApplVar.Cost);
								LessVat = ApplVar.Cost;
								LessVat.Sign |= 0x80;
								ApplVar.LessVatAmt = LessVat;
								Add(&ApplVar.LessVatTotalAmt, &LessVat);
							}else{
								LessVat = ZERO;
								sVal2 = ZERO;
								sVal3 = ZERO;
							}
						}
#endif
					}
					else
					{
						//			            if (type == 3 )//ccr2015-08-19|| type == 2)//ccr20130922  if (type == 3)
						if (type == 3 || type == 2)//ccr20130922  if (type == 3)
						{
							ApplVar.Cost = ApplVar.Amt; /* contains TAX */
							if (ApplVar.Qty.Value[0] == 0x99 &&
									ApplVar.Qty.Value[1] == 0x99 &&
									ApplVar.Qty.Value[2] == 0x99)
								ApplVar.DiscAmt = ZERO; 	/* NET is zero */
							else
							{
								ApplVar.DiscAmt = ApplVar.Amt;	/* contains NET */
								Add(&ApplVar.Qty, (BCD*)&THOUSAND10);
								ApplVar.Qty.Sign = 0x04;
								Divide(&ApplVar.DiscAmt, &ApplVar.Qty);	/* calculate net */
								RoundTaxable(&ApplVar.DiscAmt);       //税额
								Subtract(&ApplVar.Cost, &ApplVar.DiscAmt);	/* taxable - net =净额 */
								Subtract(&gst, &ApplVar.Cost);	/* calculate net for */

								//Add(&ApplVar.VatAmtTotal, &ApplVar.Cost); //jdb2018-03-28

#if NETVATVERSION //jdb2018-08-11
								//jdb2018-03-09
								if(TESTBIT(ECRVIP.ECRVIPState, BIT4)){
									LessVat = ApplVar.Amt;
									sVal3 = ApplVar.Amt;
									sVal2 = ApplVar.Qty;
									if(CompareBCD(&sVal2, &ApplVar.Qty) <= 0){
										Divide(&sVal3, &sVal2);
										RoundTaxable(&sVal3);
										Subtract(&LessVat, &sVal3);
										LessVat.Sign |= 0x80;
										//ApplVar.LessVatAmt = LessVat;
										Add(&ApplVar.LessVatAmt, &ApplVar.Amt); //jdb2018-04-27 报表加入免税毛销售额
										Add(&ApplVar.LessVatTotalAmt, &LessVat);
									}
								}else{
									LessVat = ZERO;
									sVal2 = ZERO;
									sVal3 = ZERO;
								}
#endif
								/* add on tax for Argentina */
							}
						}
					}
					if (!type || type == 4){		/* add report */
						//if (!TESTBIT(ECRVIP.ECRVIPState, BIT4) && (!type || type == 4)) 	//jdb2018-03-19
						//if(ApplVar.TaxNumber != 6) //jdb2018-04-24 第七种税率为零税率销售 不写入报表s
						{
#if NETVATVERSION //jdb2018-08-11
							sVal4 = ApplVar.Cost;
							if(ApplVar.TaxNumber == 6 || ApplVar.TaxNumber == 7){ //jdb2018-04-27 加入免税、零税总毛销售
								ApplVar.Cost = ApplVar.LessVatAmt;
							}
#endif
							AddTaxTotal();
#if NETVATVERSION //jdb2018-08-11
							SETBIT(ApplVar.FZV, BIT5); //jdb2018-05-03 增加税率报表标志
							ApplVar.Cost = sVal4;
#endif
						}
					}
				}//if
#if FOR_CANADA
				else
					sCAPrint = 0;
#endif
				//if (type == 2)
#if NETVATVERSION //jdb2018-08-11
				if ((CheckNotZero(&ApplVar.Amt) || ApplVar.TaxNumber == 6) && type == 2) //jdb2018-04-18 第7种税率为零税率
#else
				if (CheckNotZero(&ApplVar.Amt) && type == 2) //jdb20180320
#endif
				{
					if (TESTBIT(ApplVar.Tax.Options,BIT1))//ccr20130922 if (!TESTBIT(ApplVar.Tax.Options, BIT0) && TESTBIT(ApplVar.Tax.Options,BIT1))
						//if (TESTBIT(ApplVar.Tax.Options,BIT1)  || ApplVar.TaxNumber == 6) //jdb2018-04-18
					{			      /* ADD ON and print */
						if (CheckNotZero(&ApplVar.Cost)
#if FOR_CANADA
								|| sCAPrint
#endif
								//|| ApplVar.TaxNumber == 6 //jdb2018-04-18
								|| TESTBIT(ApplVar.Tax.Options, BIT2))/* print also zero */
						{
#if NETVATVERSION //jdb2018-08-11
							SETBIT(ApplVar.FZV, BIT4); //jdb2018-05-02 打印标志
#endif
							PrintLine('.'); //jdb2018-04-27 打印隔行
#if 0
							if (!printsub)
							{
								if(ApplVar.FZV == 0x00){ //jdb2018-04-24 没有零税销售或免税销售也打印
									sVal4 = ZERO;
									memset(ProgLineMes, ' ', sizeof(ProgLineMes));
									strcpy(ProgLineMes,"ZERO RATED SALES");
									PrintAmt(ProgLineMes, &sVal4);

									memset(ProgLineMes, ' ', sizeof(ProgLineMes));
									strcpy(ProgLineMes,"VAT EXEMPT SALES");
									PrintAmt(ProgLineMes, &sVal4);
								}

#if !defined(CASE_PERU)//ccr2017-01-12秘鲁客户要求不打印小计
#if 0 //jdb2018-04-27 不打印小计
								if(TESTBIT(ECRVIP.ECRVIPState, BIT4)){ //jdb2018-03-30 不打印小计
								}else{
									if (calcgst == 2)   /* GST subtotal ?*/
										PrintAmt(Msg[XIAOJI].str, &gst);
									else{
										PrintAmt(Msg[XIAOJI].str, &ApplVar.SaleAmt);
									}
								}
#endif
#endif
								printsub = 1;
							}
#endif
							//ccr20130922>>>>>>>>>>>>
							Collect_Data(TAXLOG);//ccr2014-12-30,计税非0时才记录流水
#if (defined(CASE_PERU)) //ccr2016-10-24>>>>>>>>>>>>>>
							if (!printline)
							{
								printline=1;
								PrintLine('.');
							}
#endif //ccr2016-10-24<<<<<<<<<<<<
							if (TESTBIT(ApplVar.Tax.Options, BIT4))	   /* print taxable? */
								//if (TESTBIT(ApplVar.Tax.Options, BIT4) || ApplVar.TaxNumber == 6) //jdb2018-04-18
							{
#if NETVATVERSION //jdb2018-04-25 不打印税种名称
								strcpy(ProgLineMes,VATABLESALES);
#else
								strcpy(ProgLineMes,ApplVar.Tax.Name);
								len = strlen(ProgLineMes);
								ProgLineMes[len++] = ' ';
								strcpy(ProgLineMes+len,Msg[XSHOUE].str);
#endif
#if NETVATVERSION //jdb2018-08-11
								if(TESTBIT(ApplVar.FZV, BIT3)){ //jdb2018-04-27 免税或零税销售
									sVal4 = ZERO;
									PrintAmt(ProgLineMes, &sVal4);
								}
                                else
#endif
                                {
#if NETVATVERSION //jdb2018-08-11
									if(ApplVar.TaxNumber == 6){ //jdb2018-04-18
										PrintAmt(ProgLineMes, &ApplVar.DiscAmt);/* 打印税种的销售净额 */
									}
                                    else
#endif
                                    {
										if (!TESTBIT(ApplVar.Tax.Options, BIT0)) /*ccr20130927 add on ? */
											PrintAmt(ProgLineMes, &ApplVar.Amt);/* 打印税种的销售金额 */
										else{
											PrintAmt(ProgLineMes, &ApplVar.DiscAmt);/* 打印税种的销售净额 */
#if NETVATVERSION //jdb2018-08-11
											FVAMOUNT = 0x01;
#endif
										}
									}
								}
							}						
							memset(ProgLineMes, ' ', sizeof(ProgLineMes));
							sVal1 = ZERO;
							CWORD(sVal1.Value[0]) = CWORD(ApplVar.Tax.Rate[0]);
#if NETVATVERSION //jdb2018-04-25 不打印税种名称
							CopyFrStr(ProgLineMes,VATAMOUNT);
#else
							CopyFrStr(ProgLineMes,ApplVar.Tax.Name);
							sVal1.Sign = 2;
							FormatQty(ProgLineMes+16,&sVal1);//cr070424
							ProgLineMes[17] = '%';      //ProgLineMes[13] = '%';
#endif
							ProgLineMes[18] = 0;        //ccr090113
#if NETVATVERSION //jdb2018-08-11
							if(TESTBIT(ApplVar.FZV, BIT3)){ //jdb2018-04-27 免税或零税销售
								sVal4 = ZERO;
								PrintAmt(ProgLineMes, &sVal4);
							}
                            else
#endif
                            {
#if NETVATVERSION //jdb2018-08-11
								if(ApplVar.TaxNumber == 6){ //jdb2018-04-18 不打印
								}
                                else
#endif
                                {
									PrintAmt(ProgLineMes,&ApplVar.Cost);/* 打印计税金额 */
								}
							}
							//ccr20130922			                PrintAmt(ApplVar.Tax.Name, &ApplVar.Cost);/* 打印计税金额 */

#if NETVATVERSION //jdb2018-08-11
							if (!printsub) //jdb2018-04-27 放在VATABLE VAT后打印
							{
								if(!(TESTBIT(ApplVar.FZV, BIT0) || TESTBIT(ApplVar.FZV, BIT1))){ //jdb2018-04-24 没有零税销售或免税销售也打印
									sVal4 = ZERO;
									memset(ProgLineMes, ' ', sizeof(ProgLineMes));
									strcpy(ProgLineMes,ZERORATEDSALES);
									PrintAmt(ProgLineMes, &sVal4);

									memset(ProgLineMes, ' ', sizeof(ProgLineMes));
									strcpy(ProgLineMes,VATEXEMPTSALES);
									PrintAmt(ProgLineMes, &sVal4);
								}

#if !defined(CASE_PERU)//ccr2017-01-12秘鲁客户要求不打印小计
#if 0 //jdb2018-04-27 不打印小计
								if(TESTBIT(ECRVIP.ECRVIPState, BIT4)){ //jdb2018-03-30 不打印小计
								}else{
									if (calcgst == 2)	/* GST subtotal ?*/
										PrintAmt(Msg[XIAOJI].str, &gst);
									else{
										PrintAmt(Msg[XIAOJI].str, &ApplVar.SaleAmt);
									}
								}
#endif
#endif
								printsub = 1;
							}
#endif


#if NETVATVERSION //jdb2018-08-11
							//jdb2018-03-09
							if(TESTBIT(ECRVIP.ECRVIPState, BIT4)){
								memset(ProgLineMes, ' ', sizeof(ProgLineMes));
#if 0 //jdb2018-04-25 不打印税种名称
								strcpy(ProgLineMes,ApplVar.Tax.Name);
								len = strlen(ProgLineMes);
								ProgLineMes[len++] = ' ';
								strcpy(ProgLineMes+len,"LESS VAT");
#else
								strcpy(ProgLineMes,LESSVAT);
#endif
								//PrintAmt(ProgLineMes, &LessVat); //jdb2018-04-03 不打印
							}							
#endif
							//ccr20130922<<<<<<<<<<<<
						}
					}
					/* 非ADD ON 税种不打印 ?*/
				}
				else if (type == 3)
				{
					if (TESTBIT(ApplVar.Tax.Options, BIT0) && TESTBIT(ApplVar.Tax.Options, BIT1))		/* print incl tax ? */
					{
						if (CheckNotZero(&ApplVar.Amt)
#if FOR_CANADA
								|| sCAPrint
#endif
								|| TESTBIT(ApplVar.Tax.Options, BIT2))/* print also zero */
						{
							PrintAmt(ApplVar.Tax.Name, &ApplVar.Cost);
							if (TESTBIT(ApplVar.Tax.Options, BIT4)) 	/* print taxable ? */
								PrintAmt(Msg[XSHOUE].str, &ApplVar.DiscAmt);
						}
					}
					ApplVar.Cost = ZERO;
				}
			}//for

#if NETVATVERSION //jdb2018-08-11
			if(type == 2 && !TESTBIT(ApplVar.FZV, BIT4) && !(TESTBIT(ApplVar.FZV, BIT0) || TESTBIT(ApplVar.FZV, BIT1))) //jdb2018-05-02 测试是否有打印VATABLE VAT ZERO VAT
			{
				PrintLine('.');
				sVal4 = ZERO;
				
				memset(ProgLineMes, ' ', sizeof(ProgLineMes));
				strcpy(ProgLineMes,VATABLESALES);
				PrintAmt(ProgLineMes, &sVal4);

				memset(ProgLineMes, ' ', sizeof(ProgLineMes));
				strcpy(ProgLineMes,VATAMOUNT);
				PrintAmt(ProgLineMes, &sVal4);

				memset(ProgLineMes, ' ', sizeof(ProgLineMes));
				strcpy(ProgLineMes,ZERORATEDSALES);
				PrintAmt(ProgLineMes, &sVal4);

				memset(ProgLineMes, ' ', sizeof(ProgLineMes));
				strcpy(ProgLineMes,VATEXEMPTSALES);
				PrintAmt(ProgLineMes, &sVal4);
			}
#endif

			if (calcgst == 1)	    /* gst active ? */
			{
				AmtRound(0, &ApplVar.SubTotal);     /* sales amount rounding */
				gst = ApplVar.SubTotal;
				calcgst = 2;
				printsub = 0;
			}
			else
				break;
		}//for /* also calculate gst */
	}//if

	//if (!TESTBIT(ApplVar.FZV, BIT5) && CheckNotZero(&ApplVar.LessVatAmt) && (!type || type == 4)){	 //jdb2018-05-03 零税销售加入报表
#if NETVATVERSION //jdb2018-08-11
	if (CheckNotZero(&ApplVar.ZEROLessVatAmt) && (!type || type == 4)){	 //jdb2018-05-03 零税销售加入报表
		sVal4 = ApplVar.Cost;
		if(TESTBIT(ApplVar.FZV, BIT0)){
			ApplVar.TaxNumber = 7;
			//ApplVar.Cost = ApplVar.LessVatAmt;
			ApplVar.Cost = ApplVar.ZEROLessVatAmt;
			AddTaxTotal();
		}else if(TESTBIT(ApplVar.FZV, BIT1)){
			ApplVar.TaxNumber = 6;
			//ApplVar.Cost = ApplVar.LessVatAmt;
			ApplVar.Cost = ApplVar.ZEROLessVatAmt;
			AddTaxTotal();
		}
		
		ApplVar.Cost = sVal4;
	}
#endif
	
	if (!ApplVar.FPb)
		AmtRound(0, &ApplVar.SubTotal);	/* sales amount rounding */
}


//cc 2006-10-28 for SubDisc>>>>>>>>>>>
void CalculateTax2()
{
	BCD d1,d2,EntryVal;
	BYTE i;

	//return; //jdb2018-06-08 小计折扣不进行税的计算，付款后统一计算

	if (!TESTBIT(ApplVar.Disc.Options, BIT2))    /* percentage ? */
	{
		EntryVal = ApplVar.Entry;
		Multiply(&EntryVal,(BCD*)&THOUSAND100);		/* cc 2006-10-24 for SubDisc */
		for (i = 0; i < 8; i++)
		{
			//			if (ApplVar.Disc.Tax & (0x01 << i))
			if(ApplVar.Dept.Tax & (0x01 << i))
			{
				d1 = ApplVar.TaxItem[i];
				Multiply(&d1, &EntryVal);
				Divide(&d1,(BCD*)&MILLION10);	/* cc 2006-10-24 for SubDisc */
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
		//		EntryVal = ApplVar.Amt;
		//		d1 = ZERO;
		//d1 = ApplVar.SUB_DiscTot;
		d1 = ApplVar.SubTotal;//jdb2018-08-02 小计总额
		//		for (i = 0; i < 8; i++)
		//		{
		//			if (ApplVar.Disc.Tax & (0x01 << i))
		//			if (SalesTax & (0x01 << i))
		//				Add(&d1, &ApplVar.TaxItem[i]);
		//		}
		if (CheckNotZero(&d1))     /* active ? */
		{
			for (i = 0; i < 8; i++)
			{
				//				if ((ApplVar.Disc.Tax & (0x01 << i)) && CheckNotZero(&ApplVar.TaxItem[i]))
				if ((ApplVar.Dept.Tax & (0x01 << i)) && CheckNotZero(&ApplVar.TaxItem[i]))
				{
					if (CheckNotZero(&ApplVar.TaxItem[i]))
					{
						d2 = ApplVar.TaxItem[i];
						#if 0 //jdb2018-08-02 小计金额折扣
						Add(&ApplVar.TaxItem[i], &ApplVar.Amt);
						#else
						Multiply(&d2,(BCD*)&MILLION10);	/* cc 2006-10-24 for SubDisc */
						Divide(&d2, &d1);
						Multiply(&d2, &ApplVar.Amt);
						Divide(&d2,(BCD*)&MILLION10);	/* cc 2006-10-24 for SubDisc */
						RoundBcd(&d2, 0);
						d2.Sign &= 0xfc;
						AmtRound(1, &d2);
						Add(&ApplVar.TaxItem[i], &d2);  /* sign is already oke */
						#endif
					}
				}
			}
		}
	}
}
//cc 2006-10-28 for SubDisc<<<<<<<<<<<<<<
