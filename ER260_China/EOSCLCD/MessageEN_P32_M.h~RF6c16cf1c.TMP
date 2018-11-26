#ifndef MESSAGEH_EN_P
#define MESSAGEH_EN_P

//jdb2018-07-26 退货时增加打印部分
#define RETFOOTER1	"¨ESTE DOCUMENTO SERA VALIDO PARA"
#if ORRECEIPT
#define FOOTER1	"¨ESTE RECEIPT SERA VALIDO PARA"
#else
#define FOOTER1	"¨ESTE RECEIPT SERA VALIDO PARA"
#endif
#define FOOTER2 "5 ANS DESDE LA FECHA DE"                                                            
#define FOOTER3 "USO PERMITIDO〃"                                                                  
#define FOOTER4 "¨ESTE DOCUMENTO NO ES VALIDO PARA"                                                              
#define FOOTER5 "INGRESO DE TAX RECLAMADO〃"                                                                             

#define REFNUM                 "REF#"  //jdb2018-07-26 退货单号REF#
#define TERMINALREPORT "INFORME FINAL" //jdb2018-07-27                                                   

// Printer
#if (PRTLEN>40)
                       //.123456789.123456789.123456789.123456789.1
#define Header1         "*       LO MEJOR PARA USTED        *"        //" 	 创 造 优 秀  优 创 造 就   *"
#define Header2         "*       LO MEJOR PARA USTED      *"         //" 	 创 造 优 秀     优 创 造 就   *"

#else
                       //.123456789.123456789.123456789.1
#define Header1         "*     GRACIAS POR SU COMPRA      *"        //" 	 创 造 优 秀  优 创 造 就   *"
#if defined(CASE_INDONESIA)
#define Header2         "*   Posibolt Cash Register     *"         //" 	 创 造 优 秀     优 创 造 就   *"
#else
#define Header2         "*    CAJAS REGISTRADORAS EUTRON     *"         //" 	 创 造 优 秀     优 创 造 就   *"
#endif
#endif
// Printer
#define ReportType0     "  GRAN TOTAL   "        //"   总  销  售   ",                                                             
#define ReportType1     "  GRUPO         "        //" 柜  组  销  售 ",//1                                               
#define ReportType2     "  DEPARTAMENTO    "        //" 部  类  销  售 ",//2                                                      
#define ReportType3     "  PLU           "        //" 商  品  销  售 ",/* 3 Used with ApplVar.Plu ApplVar.Report */        
#define ReportType4     "  OFERTA        "        //" 付  款  信  息 ",//4                                              
#define ReportType5     "  PAGO / CAJA CHICA  "        //" 出 入 金 信 息 ",//5                                             
#define ReportType6     "  CAJON        "        //" 钱  箱  信  息 ",//6                                             
#define ReportType7     "  CORRECCION    "        //" 更  改  信  息 ",//7                                                 
#define ReportType8     "  DSCTO      "        //" 折  扣  信  息 ",//8                                                    
#define ReportType9     "  MONEDA      "        //" 外  币  信  息 ",//9                                                        
#define ReportType10    "  IMPUESTO           "        //" 计  税  信  息 ",//10                                                  
#define ReportType11    " FUNCIONES PB  "        //" 桌  帐  信  息 ",//11                                                          
#define ReportType12    "PREVIO BALANCE"        //" 开  台  信  息 ",//12                                                         
#define ReportType13    "  MESA        "        //" 桌  台  信  息 ",//13                                                  
#define ReportType14    "  INVENTARIO     "        //" 商  品  库  存 ",//14                                                  
#define ReportType15    "  FACTURA PB   "   //                                                                                 
#define ReportType16    "  OFERTA BUFFER "    //                                                                                 
// Printer
#define ReportType17    "  TOTAL DE CAJERO  "  //"    收 款 员 统 计   ",                                                                   
#define ReportType18    "  TOTAL DE VENDEDOR "  //"    营 业 员 统 计   ",                                                             
// Printer
#define Trailer1        "       GRACIAS       "        //"     ~多  ~谢  ~光  ~临     ",                                     
#define Trailer2        "        ADIOS       "        //"          ~再  ~见         ",                              


#define ReportList1     "DIARIO CAJERO"        //" 收 款 员 日 报 ",                                                             
#define ReportList2     "PERIDO CAJERO"        //" 收 款 员 周 报 ",                                                             
#define ReportList3     "INFORME DIARIO"        //" 销  售  日  报 ",                                                           
#define ReportList4     "INFORME PERIODO"        //" 销  售  周  报 ",                                                                   
#define ReportList5     "DOC-ARTICULO"        //" 商  品  报  表 ",                                                     
#define ReportList6     "DOC-CUENTA-MESA"        //" 桌  帐  报  表 ",                                                                 
#define ReportList7     "DOC-TIEMPO"        //" 时  段  报  表 ",                                                                    
#define ReportList8     "D-TODOCAJERO"        //"  全收款员日报  ",                                                       
#define ReportList9     "S-TODOCAJERO"        //"  全收款员月报  ",                                                      
#define ReportList10    "DOC/DIA-VENDEDOR"        //" 营 业 员 日 报 "                                                                                                
#define ReportList11    "SEMANA VEND."        //" 营 业 员 周 报 "                                                   
#define ReportList12    "DIA-TODOVEND."        //"  全营业员日报  ",                                                  
#define ReportList13    "SEMANATODOVEN"        //"  全营业员月报  ",                                                      

#define Promotion1      "  CONGRATULACIONES   "        //"  ~恭  ~喜  ~中  ~奖",                                        

#define Correc1         "CORRECCIO "        //"更改    "//ccr2016-07-14                                                  
#define Correc2         "ANULACION    "        //"取消    "//ccr2016-07-14                                           
#define Correc3         "DEVOLUCION  "        //"退货    "                                                            
#define Correc4         "CANCELADO  "        //"取消交易"                                                                 
#define Correc5         "SUSPENDIDO "      //"挂账"                                                             


#define CURRENCY1         "DOLAR  "        //"美元    "L打印/显示                                                                 
#define CURRENCY2         "YEN     "        //"日元    "L打印/显示
#define CURRENCY3         "HK $    "        //"港币    "L打印/显示
#define CURRENCY4         "CURR   1"        //"外币1   "L打印/显示
#define CURRENCY5         "CURR   2"        //"外币2   "L打印/显示

#define DISCOUNT1          "+%"        //"+%加成"     L打印/显示
#define DISCOUNT2          "-%"        //"(-%)折扣"   L打印/显示
#define DISCOUNT3          "+NET"        //"金额加成"    L打印/显示
#define DISCOUNT4          "-NET"        //"金额折扣"    L打印/显示
//#define DISCOUNT5          "+% EN PLU"   // zy //"单项加成"                                                     
//ccr090812 for SubDisc>>>>>>>>>>>>>>>>>>>>>>>>>
#define DISCOUNT6          "SUBTOTAL+%"   //  "+%小计加成"L打印/显示
#define DISCOUNT7          "SUBTOTAL-%"   //  "-%小计折扣"L打印/显示
#define DISCOUNT8          "SUBTOTAL+"   // "小计金额加成"L打印/显示
#define DISCOUNT9          "SUBTOTAL-"   // "小计金额折扣"L打印/显示
//jdb2018-03-20
#define DISCOUNT10		   "SC DSCTO"  //-%SC小计折扣                                               
#define DISCOUNT11		   "CLAVE DSCTO"  //-%PWD小计折扣                                                 
//jdb2018-04-03
#define DISCOUNT12		   "SC SUB DSCTO"                                                
#define DISCOUNT13		   "CLAVE SUB DSCTO"                                                       

#define DRAWER1            "EFECTIVO    "        //"现金    "L打印/显示                                         
#define DRAWER2            "CHEQUE   "        //"支票    "L打印/显示                                               
#define DRAWER3            "TARJETA DE CREDITO"        //"信用卡  "L打印/显示                                                               
#define DRAWER4            "CUPON  "        //"购物券  "L打印/显示                                                                
#define DRAWER5            "CREDITO  "        //"赊账    "L打印/显示                                                                
#define DRAWER6            " TARJETA DE CHIP"        //"IC卡    "L打印/显示                                                               
#if  defined(CASE_INDONESIA)
#define DRAWER7            "DEBITO   "        //"小费    "L打印/显示                                                                
#else
#define DRAWER7            "CUOTA     "        //"小费    "L打印/显示                                
#endif
#define DRAWER8            "CAJON  "//打印/显示

#define PBFunction0         "ABRIR    "        //   "开台挂帐"                     
#define PBFunction1         "ANDIR     "        //   "开台增加"                                 
#define PBFunction2         "SERVICIO "        //   "暂结"                                                                         
#define PBFunction3         "FINALIZAR"        //   "确认结帐"                                                                     
#define PBFunction4         "IMPRESION PB"        //   "打印开台"                                                                     
#define PBFunction5         "IMPRESION DE CUENTA"        //   "打印帐单"                                      
#define PBFunction6         "CANCELADO "        //   "取消确认"                                                                       
#define PBFunction7         "DIVISION"        //   "分帐"                                                                                                                                             
#define PBFunction8         "TRANSFERENCIA"        //   "转帐"                                                                          
#define PBFunction9         "MOVER A"        //   "转桌帐"                                                                                                                                                 
#define PBFunction10        "CUBRIR "        //   "人数"                                           


#define PORAType1            "PAGOS   "        //"出金    "                                                                          
#define PORAType2            "CAJA CHICA "        //"入金    "                                                                          
#define PORAType3            "RETORNO"        //"IC卡退款"                               
#define PORAType4            "CARGA"        //"IC卡充值"                                       
#define PORAType5            "DEPOSITO"        //"退IC押金"                                            
#define PORAType6            "PRE REGISTRO"        //"收IC押金"                                       

#define TendType1             " EFECTIVO   "        //"现金    "                                                                     
#define TendType2             "CAJERO   "        //"支票    "                                                                      
#define TendType3             "TARJETA DE CREDITO"        //"信用卡  "                                                                     
#define TendType4             "CUPON "        //"购物券  "                                                                     
#define TendType5             "CRDITO  "        //"赊账"                                                                          
#define TendType6             "TARJETA DE CHIP "        //"ICCard"                                                                        
#define TendType7             "TARJETA DEDEBITO"        //"DEBIT CARD"                                                                   
#define TendType8             "EFECTIVO    "        //"CASH-8"                                                                        


#define OperationN3             "TOTAL"  // "总计",
#define OperationN4             "VUELTO"  // "转换",                                                                                
#define OperationN5             "SUBTOTAL"  // "小计",
#define OperationN6             "CAJA - �"  // "出金",
#define OperationN7             "CAJA + �"  // "入金",
#define OperationN8             "RESUMEN"  // "报表",
#define OperationN9             "ANADIR"  // "增加",
#define OperationN10            "MANTENER"  // "挂单,                                                                                   
#define OperationN11            "ANULAR"  // "取消",                                                                                  

#define OperationN14            "PRECIO"  // "单价:",     //                                                               
#define OperationN15            "CANTIDAD"  // "金额:",//                                               

#define CaptionN2               "IMPORTE NETO"  // "净额",                                                            
#define CaptionN4               "TOTAL TRADE"  // "交易数",                                                                                                                                                  
#define CaptionN5               "CANTIDAD"  // "数量",                                                                              
#define CaptionN6               "IMPORTE"  // "金额",                                                                                
#define CaptionN7               "ENTREGADO"  // "收取",                                                                               
#define CaptionN9               " DIA "  // "日",
#define CaptionN10              " SEMANA "  // "周",
#define CaptionN11              "MES"  // "月",
#define CaptionN12              "ANO"  // "年",
#define CaptionN13              "ESTANDAR"  // "标准",                                                                            
#define CaptionN14              "CAJERO"  // "收款员",                                                                             
#define CaptionN15              "   HORA"  // "时",                                                                               
#define CaptionN16              " DIA "  // "   日",
#define CaptionN17              " MES "  // "   月",
#define CaptionN18              "DEVOLUCION"  // "退货",                                                                             
#define CaptionN19              "COSTO"  // "成本",                                                                                
#define CaptionN20              "MESA# "  // "桌号#",                                                                           
#define CaptionN21              "CHEQUE NO"  // "支票号#",
#define CaptionN22              "ARTICULO"  // "项数:",                                                                                                                                                       
#define CaptionN23              "IMPONIBLE"  // "含税销售",                                                          
#define CaptionN24              "IMPUESTO "  // "税额",                                                                             
#define CaptionN25              "PAGINA"  //  "页号",                                                                                                                                                             
#define CaptionN26              "  NO ANDIR #"  // "号码:",                                                                                                                                                  
#define CaptionN27              "SERVICIO"  // "挂帐",                                                                         
#define CaptionN28              "ABRIR MESA"  // "开台",
#define CaptionN29              "CONTADOR DE Z"  // "清帐计数",                                                                     
#define CaptionN30              "BORRAR"  // "清除",                                                                            
#define CaptionN31              "INFORME -S"  // "S-报表",什么是S-REPORT? ZWQ                                                    
#if HOR_REPORT==1
#define CaptionN32              "BRUTO"  // "毛利",                                                                  
#else
#define CaptionN32              "BENEFICIO BRUTO"  // "毛利",
#endif
#define CaptionN33              "INCREMENTAR STOCK"  // "库存增减",//                                                         
#define CaptionN331             "DISMINUIR STOCK"  // "库存增减",//                                                         
#define CaptionN34              "CUENTA-MESA ANTERIOR"  // "原桌帐:",                                                                 
#define CaptionN35              "T.CREDITO"  // "信用卡#",
#define CaptionN36              "NUEVA MESA"  // "新桌帐:",                                                                   
#define CaptionN37              "CLIENTE NO"  // "人数",
#define CaptionN38              "CAMBIA ARTICULO"  // "换货",                                                                      

//Dispaly
#define CaptionN41              "DSCTO 1"  // "折扣项1",
#define CaptionN42              "DSCTO 2"  // "折扣项2",
#define CaptionN43              "IMPORTE DE VENDEDOR"  // "销售额",                                                                 
#define CaptionN44              "DTO MASIVO"  // "大包优惠",
#define CaptionN45              "STOCK CANT"  // "库存数量",
#define CaptionN46              "OFERTA"  // "特价优惠",
#define CaptionN51              "MESA TEMPORALFIN"  // "开台暂结",
#define CaptionN52              "+MESA"  // "开台增加",
#define CaptionN53              "CERRA TEMPORAL"  // "暂结",
#define CaptionN54              "CONFIRMAPAGO"  // "确认结帐",
#define CaptionN55              "IMPRIME MESA"  // "打印开台",
#define CaptionN56              "IMPRIMCUENTA"  // "打印帐单",
#define CaptionN57              "CANCELA CONFIRMA"  // "取消确认",
#define CaptionN58              "DIVIDECUENTA"  // "分帐",
#define CaptionN59              "TRANS-CUENTA"  // "转帐",
#define CaptionN60              "ALTERAR OPER"  // "更换收款员",信息为"MOV"
#define CaptionN61              "CLIENTE NO"  // "顾客人数",


#define CaptionE11              "CAJERO:"  // "收款员:",
#define CaptionE12              "VENDEDOR:"  // "营业员:",                                                                         
#define CaptionE13              "CAJA NO:"  // "收款机号:",
#define CaptionE14              "TIENDA:"  // "位置:",                                                                                                                                              

#define CaptionE17              "VERSION ACTUAL:"  // "发行版本:",
#define CaptionE18              "PUNTOS"  // "本次消费点",                                                   
#define CaptionE19              " ANDIR PLU"  // "新增单品:",                                                                  
#define CaptionE20              "BORRAR PLU"  // "删除单品:",                                                                
#define CaptionE21              "COMENZAR TEST>>"  // "开始检测>>",
#define CaptionE22              "<<YA TEST"  // "<<检测结束",
#define CaptionE23              "INVENTARIO"  // "库存",
#define CaptionE24              "BORRADO DE INFORME!"  // "报表已全清!",                                           


#define CaptionE31              "NOTA"  // "说明文字",


#define MessagesN5                "*** MODO ENTRENAMIENTO ***"  // "*** 培训状态 ***",                                       
#define MessagesN6                "***** ENCENDIDO*****"  // "***** 加电 *****",                                            
#define MessagesN7                "*** MESA DIVIDIDA ***"  // "*** 分开付帐 ***",                            
#define MessagesN8                "** COPIA DE RECIBO***"  // "*** 复制收据 ***",                                     
#define MessagesN9                "****PONER PRECIO PLU ****"  // "*** 商品变价 ***",//                                 
#define MessagesN10               "**INVENTARIO+ **"  // "*** 增加库存 ***",
#define MessagesN101              "**INVENTARIO- **"  // "*** 减少库存 ***",
#define MessagesN11               "*****CANCELAR *****"  // "***** 取消 *****",
#define MessagesN12               "**  INICIAR PROGRAMA **"  // "** 程序初始化 **",

#define EditN24                   " ARCHIVO    GRABAR    RAM"  // "文件名      记录数      空间大小"         
#define EditN25                   " PUERTO     TIPO  PROTOCOLO  "  // "串口名称    通信类别   通信协议"   

//PRINT                          /*123456789012345678901234567890123456*/
#if PRTLEN>40
#define TitleN1                   "NOMBRE  CANTIDAD.   PVP. IMPORTE"
#define TitleN3	                  "NOMBRE      CANTIDAD.  IMPORTE"  //--品名   数量   单价   金额
#define TitleN4	                  "NOMBRE                 IMPORTE"  //

#define DEPTREPORTTITLE           "DEPT      CANTIDAD      IMPORTE"
#define PLUREPORTTITLE            "NOMBRE      CANTIDAD   IMPORTE"//

#else

#define TitleN1	                  "NOMBRE  CANTIDAD.   PVP. IMPORTE"  //--品名   数量   单价   金额
#define TitleN3	                  "NOMBRE      CANTIDAD.  IMPORTE"  //--品名   数量   单价   金额
#define TitleN4	                  "NOMBRE                      IMPORTE "  //
#define DEPTREPORTTITLE         "DEPT      CANTIDAD      IMPORTE"
#define PLUREPORTTITLE          "NOMBRE      CANTIDAD   IMPORTE"//

#endif

#define OptionS37                 "LLAMADA"    //mPRNINDEX//Ccr "取菜号:",                                         


#define EditN6                    "CONSUMIDO"  // "消费金额",                                                                                                                     
#define ChipCardN46               " # DE TARJETA"  // "会员号:",                                                                 
#define ChipCardN47               "AFILIACION"  // 会 员 消 费                                                                


#define PLULIST                  "        LISTA PLU"  // "         单 品 信 息 表"                                   
#define PLUTITLE1                "NOBRE-ARTICULO          REF"  //
#define PLUTITLE2                "   DEPT EN STOCK PVP"//

#define DEPTLIST               "         INFO-DEPT"  // "         单 品 信 息 表"
#define DEPTTITLE                "NO-DEPT  GRUPO  PVP"        //



#define FisTextN18                "   IMPRIME IMPUESTO NO    "   //"    _计 _税 _印 _出     "
#define FisTextN19                "    TERMINADO PROBAR MUESTRA  "   //"    _试 _装 _结 _束     "
#define FisTextN20                "    _NOT   _FISCAL    "   //"    _非   _计   _税     "


#define PRTStyleN1                       "NORMAL IMPRIME"   //Normale
#define PRTStyleN2                       "DblHigh"   //Doppia altezza
#define PRTStyleN3                       "D~b~l~W~t~h"   //Raddoppio caratteri
#define PRTStyleN4                       "D~b~l~H~i~g~h"   //Dopia altezza + Raddoppio caratteri

#define ComTestN1                        "PORT#1 ERROR"   //"串口 1 测试出错"
#define ComTestN2                        "PORT#1 OK"   //"串口 1 测试正确"
#define ComTestN3                        "PORT#2 ERROR"    //"串口 2 测试出错"
#define ComTestN4                        "PORT#2 OK"   //"串口 2 测试正确"

#define ComTestN3ERR                      "PORT#3 ERROR"    //"串口 3 测试出错"
#define ComTestN3OK                      "PORT#3 OK"   //"串口 3 测试正确"


#define REPRN_PWON      "IMPRIME SIN LUZ........." //
#define REPRN_PAPERON   "IMPRIME SIN ROLLO......"  //

//ccr2016-01-12>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define GRAPHICSELF     "  AUTO GRAFICO"//
#define GRAPHICCAFFE    "  CAFE"//
#define GRAPHICDRESS    "  VESTIDO"//
#define GRAPHICKNIFE    "  CUCHILLO"//
#define GRAPHICHIPPY    "  HIPPY"//
#define GRAPHICBREAD    "  PAN"//
#define GRAPHICCHICKEN  "  POLLO"//
#define GRAPHICROSE1    "  ROSA"//
#define GRAPHICMUM      "  CRISANTEMO"//
#define GRAPHICICE      "  HELADO"//
#define GRAPHICGIFT     "  REGALO"//
#define GRAPHICROSE2    "  ROSA GRANDE"//
#define GRAPHICTREE     "  ARBOL DE NAVIDAD"   //
#define GRAPHICHBREAD   "  PAN CALIENTE"//
#define GRAPHICJOY      "  ALEGRIA"//
#define GRAPHICFISH     "  PESCADO"//
#define GRAPHICCARTOON  "  CARICATURA DE POLLO" //
#define GRAPHICMONEY    "  DINERO"//
#define GRAPHICEAT      "  COMIENDO"//
#define GRAPHICKITE     "  VOLANDO"            //
#define GRAPHICRUN      " CORRIENDO"//
#define GRAPHICREST     "  DESCANSO"//

#define ECRVIPTITLE     "CUENTA     IMPORTE     PUNTOS"
                       //012345678901234567890123456789012
#define CREATEDATE      "FECHA DE CREACION"

//ccr2016-06-06>>>>>>>>>>>INDIA>>>>>>>>>>>>>>>>
#define MsgEDITPLU      "EDITAR PLU"
#define MsgEDITDEPT     "EDITAR DEPART."
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
#define Msg_RECNUMFR	"EJ# DE"                  //开始收据号	//RECNUMFR
#define Msg_RECNUMTO	"EJ# PARA"                    //结束收据号	//RECNUMTO
#define Msg_EJBBIAO		"~E~J ~I~N~F~O~R~M~E"		//EJ 报表			//EJBBIAO
#define Msg_EJXINXI		"MENSAJES ACTUALES DE EJ"            //EJ信息			//EJXINXI
#define Msg_NEWEJXINXI	"EJ DESPU?S INICIALIZADO"            //EJ信息			//NEWEJXINXI

#define Msg_EJBHAO		"EJ ID"						//EJ编号			//EJBHAO
#define Msg_EJLEFT    "EJ IZQUIERDO"        //剩余空间
#define Msg_SIZEEJ		"SIZE OF EJ"				//SIZEEJ
#define Msg_EJCSHUA		"EJ INICIO"		//EJ初始化		//EJCSHUA

#define Msg_PRNEJURNAL  ">>>>IMPRESION DE TESTIGO DIARIO<<<<"
#define Msg_ENTERSETUP  ">>>>>IMPRIMIR EN CONFIGURACION<<<<<"
#define Msg_EJDATEEND   "FECHA ULTIMA EJ"
#define Msg_EJRECORDS   "ULTIMO EJ#"

#define Msg_EJINITIAL   "INICIALIZACION EJ"
#endif

#define Msg_MANAGEMENT "LISTADO DE ADMINISTRADOR "
#endif

