#ifndef MESSAGEH_EN
#define MESSAGEH_EN

//jdb2018-07-20 新增信息
#define TAXANAME "TAX_A"             
#define TAXBNAME "TAX_B"                 
#define TAXCNAME "TAX_C"               
#define TAXDNAME "TAX_D"              
#define TAXENAME "TAX_E"                
#define TAXFNAME "TAX_F"                 
#define TAXZERONAME "0_VAT" //零税      
#define TAXVATNAME "SIN_VAT" //免税      
//#define SCDISCNAME "SC DESCUENTO"                
//#define PWDDISCNAME "PWD DESCUENTO"   
#define SCDISCNAME "SC -%"
#define PWDDISCNAME "PWD -%"
 
#define OLDGRANDTOTAL "BALANCE INICIAL"  
#define ZERORATEDSALES "VENTAS CERO"    
#define VATEXEMPTSALES "VENTAS SIN VAT"  
#define GROSSSALES "VENTA BRUTA"              
#define VATEXEMPTAMOUNT "VAT AJUSTE"   
#define VATABLESALES "VENTAS CON VAT"          
#define VATAMOUNT "IMPORTE DE VAT"           
#define NEWGRANDTOTAL "BALANCE FINAL"       
#define TOTALGROSSSALES "TOTAL DE VENTA BRUTA"    
#define NOTFOUND "NO ENCONTRADO"                
#define DISCOUNTTOTAL "DESCUENTO TOTAL"       
#define LESSVAT "MENOS VAT" 
#if ORRECEIPT
#define TRTITELE "RECIBO OFICIAL" //jdb2018-07-26
#define RECNUMPRE "OR" //jdb2018-07-26
#else
#define TRTITELE "FACTURA DE VENTA"
#define RECNUMPRE "SI"
#endif
#define RETNUMPRE "J" //jdb2018-07-31 退货票据号前缀

#define RETURNSLIP "RETORNO DE DESLIZAMIENTO"
#define TOTALACCSALES "TOTAL DE VENTAS ACUMULADAS"
#define INPUTREFNUM "  REF#"       
#define RECOVRECPARAM "RECOV ECR PARAM"

#define Modifier1             "INSTRUCCION"        //"说明    "
#define ClerkRec1             "CAJERO"        //"收款员  "
#define GroupRec1             "GRUPO"        //"柜组    "
#define DeptRec1              "DEPT"        //"部类    "
#define PLURec1               "ARTICULO"        //"商品    "
#define SalesPersonRec1       "VENDEDOR"        //"营业员  "
#define OffRec1               "OFERTA"        //"特殊出价"

#define XZReportList1     "CIERRE DIARIO"        //"销售日报表",
#define XZReportList2     "SEMANA-VENTA"        //"销售月报表",
#define XZReportList3     "DOC-ARTICULO"        //"商品报表",
#define XZReportList4     "MESA CUENTA"        //"桌帐报表",
#define XZReportList5     "DOC-TIEMPO"        //"时段报表",
#define XZReportList6     "DIA CAJERO"        //"收款员日报表",
#define XZReportList7     "SEMANA CAJERO"        //"收款员月报表",
#define XZReportList8     "D-TODOCAJERO"        //"全收款员日报表",
#define XZReportList9     "S-TODOCAJERO"        //"全收款员月报表",
#if (salNumber)
#define XZReportList10    "DIA-VENDEDOR"        //"营业员日报表"
#define XZReportList11    "SEMANA VEND."        //"营业员月报表"
#define XZReportList12    "DIA-TODOVEND."        //"全营业员日报表",
#define XZReportList13    "SEM-TODOVENDEDO"        //"全营业员月报表",
#endif
#if (BARCUSTOMER==1)
#define XZReportList14    "SOCIO/BARCODE"        //"条码会员报表",
#endif

#define PortType1               "PC"        //"  主机",
#define PortType2               "SCANER"        //"条码枪",
#define PortType3               "BALANZ"        //"电子秤",
#define PortType4               "COCINA"        //"厨  打",
#define PortType5               "TICKET"        //'票打'，20040322
#define PortEXTPRN      "EP-PORT"//ccr2016-12-14 ExtPrinter串口
#define PortType6               "MR IC"        //-------090421 BY ZY//"射频卡"
#define PortType7               "E CARD"        //"E通卡"

#define KClose                  "  CERRAR"        //"   关 ",

#define SysFlags0		        "LOCAL NO:"	// "店铺编号"
#define SysFlags1               "CAJA NO: "        //"收款机号:"
#define SysFlags2               "ROUNDING:"        //"舍入方式:"
#define SysFlags3               "CAMBIA PVP:"        //"商品变价:"
#define SysFlags4               "SILBATO"        //"蜂鸣器:"
#define SysFlags5               "DESIGNA C"        //"强制收款员"
#define SysFlags6               "CAMBIAR C"        //"改变收款员"
#define CLERKPASS               "CLAVE C."        //"收款员密码"
#define SysFlags8               "PRINT.VEND."        //"打印营业员"
#define SysFlags9               "PRINT C."        //"打印收款员"
#define SysFlags10              "PRINT DOC#"        //"打印收据号"
#define SysFlags11              "OSCURO PRINT"        //"高浓度打印"
#define SysFlags12              "DESIGNA V."        //"强制营业员"
#define SysFlags13              "IMPRIME#"        //"打印编码:"
#define SysFlags14              "PULSES.TOTAL:"        //"按小计键:"
#define SysFlags15              "IMPR.VENTA"        //"打印项目数"
#define SysFlags16              "IMPRIME DOC:"        //"打印小票:"
#define SysFlags17              "CABEZA-TIC:"        //"小票票头:"
#define SysFlags18              "BLOQ SHIFT:"        //"锁定SHIFT:"
#define SysFlags19              "MUESTRAHORA:"        //"显示时间:"
#define SysFlags20              "COPIA DOC:"        //"复制收据:"
#define SysFlags21              "ESPECIAL #:"        //"特殊条码:"
#define SysFlags22              "MONTA DIA:" //-----090421 BY ZY//"操作日志"
#define SysFlags23              "PRINT.MESA:"        //"开台打印:"
#define SysFlags24              "PRINT.STCOK:"        //"打印库存"
#define SysFlags25              "INTERRUPTOR:"        //"打印开关:"
#define SysFlags26              "OBL.MESA:"        //"必须开台:"
#define SysFlags27              "CONSERVA#"        //"保持收据号"
#define SysFlags28              "COPYCOCINA:"        //"复制厨打:"
#define SysFlags29              "RECUPERA#"        //"收据号复位"
#define SysFlags30              "ALTA IMPRIME:"    //-----090421 BY ZY//"倍高打印"
#define SysFlags31              "PARIDAD:"   //-----090421 BY ZY//"对比度:"
#define SysFlags32              "CBZ.CENTRO:"  // "票头居中打印"
#define SysFlags33              "PIE CENTRO:"  // "票尾居中打印"
#define SysFlags34              "ALTA CABEZA:"  // "票头倍高打印"
#define SysFlags35              "ALTA PIE:"  // "票尾倍高打印"
//ccr2016-03-21#define SysFlags36              "To .CSV "
#define SysFlags37              "PRINT FECHA"
#define SysFlags38              "PRINT HORA"
#define SysFlags39              "PRINT INDEX"    //"打印取菜号"
#define SysFlags40              "HEART BEAT"    //ccr2015-08-03"向服务器发送Heart beat"
#define SysFlags41              "PreFix1:"      //ccr2016-08-02
#define SysFlags42              "PreFix2:"      //ccr2016-08-02


#define GrapType1               "MARCA"        //"商店图标",
#define GrapType2               "PREMIO"        //"中奖图标:",
#define GrapType3               "LOGO-PIE"        //"票尾图像:",
#define GrapType4               "IMPRIME ?"        //"打印文字?",

#define GrapSet1                "NO#:"        //"序号:",
#define GrapSet2                "FOTO REF."        //"图像编号:",
#define GrapSet3                "INICIO "        //"开始日期:",
#define GrapSet4                "FECHA-FIN:"        //"结束日期:",
  /////==============CHN msg===============//////

#define CurrentMode1            "ABIERTO "  //"营业",
#define CurrentMode2            "LEE CUENTA   "  //"读帐",
#define CurrentMode3            "QUITA CUENTA   "  //"清帐",
#define CurrentMode4            "CONFIG. "  //"设置",
#define CurrentMode5            "GERENTE  "  //"经理",

#define SetupMode1              "FUNCION GRAL."  //---090421 BY ZY// "综合功能",
#define SetupMode2              "GESTION-ART."  // "商品管理",
#define SetupKeyboard           "KEYBOARD"      //DEFKEYBOARD  //
#define msgCOMMHOST             "COMM-HOST"
#define SetupMode3              "VICE SET"    // "通讯与外设",
#define SetupMode4              "CONFIG.VENTA"    // "销售功能设置",
#define SetupMode5              "ACCOUNT"   // "会员卡管理",
#define SetupMode6              "CABEZA-PIE"  // "票据样式",
#define SetupMode7              "GESTION-DOC    "    // "报表功能",
#define msgDISCOUNT             "DISCOUNT"      //MENUDISC //
#define SetupMode8              "CONFIGURA"   // "餐饮功能",
#define SetupMode9              "CAJERO-CLAVE"    // "密码,
#define SetupMode10             "PROBAR CAJA"       // "收款机维护",


#define SysSettings1            "CONFIG.SISTEMA"     // "系统参数设置",
#define SysSettings2            "CONFIG.FECHA"     // "日期",
#define SysSettings3            "CONFIG.HORA"     // "时间",
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define msgSETEJOURNAL          "CONFIG.EJ"      //SETEJOURNAL //EJOURNAL
#endif

#define SysSettings4            "CONFIG.TECLADO"     // "键盘",
#define MESSKEYMACRO            "KEYB. MACRO" // "键盘宏"
#define MESSKEYTYPE             "KEYBOARD TYPE"             // "键盘模式"

#define SysSettings5            "TECLADOCOMIDA"      // "餐饮键盘切换",
#define SysSettings6            "TECLADOSUPERM"    // "超市键盘切换",
#define SysSettings8            "GPRS FUNCTION"          //"GPRS通信"  //SETGPRS //ccr2014-11-11 NEWSETUP

#define SetupCaption1           "GRUPO"			//"柜组"
#define SetupCaption2           "DEPT"	//"部类",
#define SetupCaption3           "ARTICULO"			//"单品",
#define SetupCaption4           "IMPRIME ART.#"			//"打印商品清单",
#define msgSETTEST              "PRUEBA DE ECR"//"系统测试"            //SETTEST    
#define msgSETCLEARPLU          "BORRADO DE PLU"//"清除单品"        //SETCLEARPLU  
#define msgSETPLULIST           "PRINT DEPT/PLU"//"打印商品清单"      //SETPLULIST          
#define msgSETTSTVER	        "PRINT VERSION"//"打印版本"      //SETTSTVER
#define msgSETCONFIGPLU         "FORM PLU"//"配置单品"          //SETCONFIGPLU                
#define msgSETCONFIGDEPT        "FORM DEPT" //配置部类         //SETCONFIGDEPT
#define msgSETINSTALL           "INSTAL/BACKUP"//"安装/备份"        //SETINSTALL               
#define msgSETPRINTFAT          "PRINT MAT"//"打印文件分配表"   //SETPRINTFAT


#define SetupCaption5           "COMPUTER"			// "通讯口1",
#define SetupCaption6           "SCANNER"			// "通讯口2",
#define SetupCaption7           "SCALE"			// "通讯口 3",
//#define SetupCaption8           "PORT-4"			// "通讯口 4",
#define SetupCaption9           "ETHERNET"			// "网络设置",

#define SetupCaption10          "PRINT DOC"			// "票据打印",
#define SetupCaption11          "PRINT.COCINA"			// "厨房打印",
#define SetupEXTPrinter         "EXTERNAL PRINT"    //SETEXTPRINTER

#define SetupCaption12          "OFERTA"				// "优惠价",
#define SetupCaption13          "CONFIG.PREMIO"			// "中奖设置",
#define SetupCaption14          "DESCUENTO"				// "折扣",
#define SetupCaption15          "CAJA +/- �"				// "出金入金",
#define SetupCaption16          "REVISAR"			// "更正功能",
#define SetupCaption17          "PAGO FORMA "			// "付款方式",
#define SetupCaption18          "DIVISA"        // "外币",
#define SetupCaption19          "CAJA"        // "钱箱",
#define SetupCaption20          "IMPUESTO%"        // "税率",

#define SetupCaption21          "PONER CUENTA"  // "IC卡设置",                                
#define SetAddNewECRVIP         "ANDIR CUENTA"                         
#define SetPrintECRVIP          "IMPRESION CUENTA"                    
#define SetCleanALLVIP          "BORRAR CUENTA"                           

#define SetupCaption22          "PERDER.IC"        // "IC卡挂失",
#define SetupCaption23          "PRINT IC"        // "打印卡信息",
#define SetupCaption24          "CABEZA-TIC"        // "票头",
#define SetupCaption25          "PIE-TIC"        // "票尾",
#define SetupCaption26          "CABEZA-DOC"        // "票据票头",
#define SetupCaption27          "PIE FOTO "        // "图形",
#define SetupCaption28          "IMPRIME FOTO"  // "打印图形",
#define SetupCaption29          "CONFIG.DOC"        // "设置报表内容",
#define SetupCaption30          "DOC MODULO"  // "报表单元",
#define SetupCaption31          "CONFIG.HORA"        // "报表时段设置",
#define SetupCaption32          "QUITA TODODOC"     // "总清报表",
#define SetupCaption33          "IMPRIME C/C"         // "打印流水",

#define SetupCaption34          " MESA"  // "桌台",
#define SetupCaption35          "ALIMENTACION"  // "餐饮功能",
#define SetupCaption36          "MENU"      //ZY  // "菜单说明",
#define SetupCaption37          "FIRMAR CUENTA"          //ZY  // "签单",
#define SetupCaption38          "CAJERO"  // "收款员",
#define SetupCaption39          "VENDEDOR"  // "营业员",
#define SetupCaption40          "CONFIG.CLAVE"          //ZY  // "设置档密码",
#define SetupCaption41          "CLAVE-GERENTE"          //ZY  // "经理档密码",
#define SetupCaption42          "CLAVE-LEER"     //ZY  // "只读报表密码",
#define SetupCaption43          "CLAVE-QUITA"     //ZY  // "清帐报表密码",
#define SetupCaption44          "MEMORIA TEST"        //ZY  // "打印测试",
#define SetupCaption45          "DISPLAY TEST"       //ZY  //"显示测试",
#define SetupCaption451         "COMPRADORES"      // 客显测试                          
#define SetupCaption46          "PRUEBA DE IMPRESORA"      //ZY  //"打印版本",          
#define SetupTestRAM           "CHEQUEO DE RAM"        //SETTSTMEM	       //RAM测试       
#define SetupCaption47          "PRUEBA DE TIEMPO"      //ZY  //"时钟测试",         
#define SetupCaption48          "SILBATO TEST"     //ZY  //"蜂鸣器测试",
#define SetupCaption49          "CAJA TEST"     //ZY  //"钱箱测试",
#define SetupCaption50          "TECLADO TEST"       //ZY  //"键盘测试",
#define SetupCaption51          "SERIE TEST"        //ZY  //"串口测试",
#define SetupCaption52          "AUTO-TEST"       //ZY  //"自动测试",

#define msgSETPRNMENU           "PRINT MENU"      //SETPRNMENU   //

#define OperationN1             "GRABAR#"  // "记录#",
#define OperationN2             "GRABAR?"           //ZY  // "记录?",

#define OperationN12            " CONTRASENA"  // "密码",
#define OperationN13            "CONFIR.CLA"  // "密码确认",
#define OperationN16            "SUPERMERCADO"  // "超市版",
#define OperationN17            "RESTAURANT"  // "餐饮版",
#define OperationN18            "RESTRAURANTE"  // "测试",


#define CaptionN1               "TOTAL"  // "应付",
#define CaptionN3               "PVP"  // "价格",
#define CaptionN8               "VUELTO"  // "找零",

#define CaptionN39              "ANADIR?"  // "增加?",
#define CaptionN40              "BORRAR?"  // "删除?",
#define CaptionN47              "OPCION:"  // "选项:",
#define CaptionN48              "FIJO"  // "固定",不要超过SETINPUTWD的宽度
#define CaptionN49              "FLEXI."  // "浮动",不要超过SETINPUTWD的宽度
#define CaptionN50              "FIJO/FLEX."  // "固/浮",不要超过SETINPUTWD的宽度
#define CaptionN62              "CONFIG.PREMIO:"  // "中奖设定:",
#define CaptionN63              "PUNTOS:"  // "消费积分:",
#define CaptionN64              "ALTERA PVP"  // "改变单品价格",
#define CaptionN65              "COCINA#"  // "厨房打印机组"，
#define CaptionN66              " +STOCK"  // "增加库存",
#define CaptionN67              "-STOCK"  // "减少库存",
#define CaptionN68              "TECLAR "  // "输入",
#define CaptionN69              "TOTAL "  // "待付",


#define CaptionE1               "ERROR RS232"  // "RS232错",
#define CaptionE2               "CORRECTO RS232"  // "RS232正确",
#define CaptionE3               "RTC ERROR!"  // "RTC 错!",
#define CaptionE4               "CTC ERROR!"  // "CTC 错!",
#define CaptionE5               "FPROM ERROR!"  // "FPROM错!",
#define CaptionE6               "INICIAR..."  // "初始化...",
#define CaptionE7               "YA INICIADO."  // "初始化结束.",
#define CaptionE8               "ERROR-"  // "出错-",//长度<SCREENWD

#define CaptionE9               "CLAVE ERROR!"  // "密码错 ！",
#define CaptionE10              "GRABADO CLAVE"  // "密码已保存",
#define CaptionE15              "CONFIRMAR?"  // "确认?",
#define CaptionE16              "TERMINADO"  // "完成",

#define CaptionE25              "NO APAQUE EN ACTUALIZAR"  // "更新程序切勿断电",
#define CaptionE26              "DESBORDADO DE ARCHIVO!"  // "文件空间越界",                
#define CaptionE27              "DESBORDADO MEMORIA！"  // 内存溢出！
#define CaptionE28              "MEMORIA LLENA"  // "流水存储区满",
#define CaptionE29              "CAMBIA ROLLO"  // "请更换打印纸",
#define CaptionE30              "MEMORIA EXTERNAL ERR！"  // "扩展内存出错！",



#define CaptionE32              "INVALIDO!"  // "无效输入!",
#define CaptionE33              "FECHA INVALIDO!"  // "无效日期!",
#define CaptionE34              "HORA INVALIDO"  // "无效时间!",
#define CaptionE35              "NO TECLEAR!"  // "禁止输入!",          
#define CaptionE36              "PLU ES VACIO!"  // "PLU代码为空!",
#define CaptionE37              "PLU DOC LLENO!"  // "PLU文件溢出!",
#define CaptionE38              "MESA OCUPADA!"  // "桌台被占用!",
#define CaptionE39              "MESA ABIERTA!"  // "桌台已打开!",
#define CaptionE40              "MESA EQUIVOCADA#"  // "桌台号不对!",          
#define CaptionE41              "MESA NO ERROR!"  // "请输入桌台号",                         
#define CaptionE42              "MESA LIBRE"  // "没有开台",
#define CaptionE43              "CUENTA-MESA LLENA"  // "桌台桌帐溢出",
#define CaptionE44              "NO MODIF.HORA"  // "禁止修改时间",
#define CaptionE45              "EN VENTA!"  // "仍在销售中!",
#define CaptionE46              "RAM-VENTA LLENA"  // "销售缓冲区满",
#define CaptionE47              "STOCK ARTICULO"  // "商品没有售出",
#define CaptionE48              "DURANTE COBRO!"  // "正在结算中!",
#define CaptionE49              "DATOS LARGO"  // "输入数据超长",
#define CaptionE50              "DURANTE PAGO!"  // "正在付帐!",
#define CaptionE51              "CLIENTE LLENO!"  // "人数溢出!",
#define CaptionE52              "FALTA PAGO"  // "没有确认结帐",
#define CaptionE53              "NO CANCELE PAGO"  // "禁止取消付款",
#define CaptionE54              "SIN ESTE COMIDA!"  // "无菜名!",
#define CaptionE55              "SIN ESTE C!"  // "无此收款员!",
#define CaptionE56              "CAMBIADO C"  // "收款员改变",
#define CaptionE57              "SIN ESTE DOC!"  // "无此类报表!",
#define CaptionE58              "FALLO PRINT DOC"  // "报表打印中断",
#define CaptionE59              "OBL.GERENTE"  // "必须在经理档",
#define CaptionE60              "NO REPARTICION-CUENTA"  // "不能分帐时用",
#define CaptionE61              "OBL.NO#!"  // "必须输入号码",
#define CaptionE62              "KEY POS ERROR"  // "钥匙档位错",
#define CaptionE63              "SIN AUTORIZACION!"  // "未授权!",
#define CaptionE64              "RESIGNA C"  // "请指定收款员",
#define CaptionE65              "RESIGNA VEND"  // "请指定营业员",
#define CaptionE66              "NOCAMIBE PLU-PVP！"  // "禁止PLU变价！",
#define CaptionE67              "CLAVE ERROR!"  //  "密码不对!",
#define CaptionE68              "COCINA ERR"  // "厨房打印机错",
#define CaptionE69              "PRINT ERROR"  // "票据打印机错", //ccr2017-08-31
#if defined(CASE_INNERDOTPRN)
#define CaptionE70              "LEVANTA EJE"  // "打印压杆抬起",
#else
#define CaptionE70              "SIN ROLLO!"  // "打印压杆抬起",
#endif
#define CaptionE71              "SIN ROLLO!"  // "打印纸用完!",
#define CaptionE72              "TEMPERATURA ALTA"  // "打印温度太高",
#define CaptionE73              "SIN DEFINIR BOTON!"  // "功能未定义!",
#define CaptionE74              "OBLIGATORIO�"  // "必须输入金额",
#define CaptionE75              "NO-FORMA-PAGO"  // "禁止多种付款",
#define CaptionE76              "SIN ESTE FUNCION"  // "记帐无此功能",
#define CaptionE77              "FALTA PULSES.TOTAL!"  // "未按小计键!",
#define CaptionE78              "EN GESTION-STOCK"  // "正在管理库存",
#define CaptionE79              "GRABA C/C ERR"  // "保存流水错",
#define CaptionE80              "COM-MODEM ERR"  // "MODEM通讯错!",


#define CaptionE81              "MONTA-T ERR!"  // "卡操作出错!"
#define CaptionE82              "POS BARCODE ERR!"  // "POS代码错!"
#define CaptionE83              "T.DATOS ERR!"  // "卡数据错!"
#define CaptionE84              "T.CADUCADA!"  // "为过期卡!"
#define CaptionE85              "T.PERDIDA!"  // "为挂失卡!"
#define CaptionE86              "CLIENTE TIPO ERR!"  // "客户类别错!"
#define CaptionE87              "T.NUEVA!"  // "为新卡!"
#define CaptionE88              "T.INVALIDA!"  // "不是购物卡!"
#define CaptionE89              "LEE T.ERR!"  // "写卡出错!"
#define CaptionE90              "T.NO ERR!"  // "卡号不对!"
#define CaptionE91              "NO T.DTO!"  // "禁用折扣卡!"
#define CaptionE92              "NO T.CASH!"  // "禁用现金卡!"
#define CaptionE93              "NO T.CREDITO!"  // "禁用赊帐卡!"
#define CaptionE94              "NO USE IC!"  // "禁用IC卡!"
#define CaptionE95              "QUITA T.ERR!"  // "清卡出错!",
#define CaptionE96              "DATOS LLENOS!"  // "数据溢出!",
#define CaptionE97              "RECARGA IC ERR!"  // "IC充值出错!",
#define CaptionE98              "INICIA IC ERR"  // "IC初始化出错",
#define CaptionE99              "NO INICIE IC!"  // "禁止初始化!",
#define CaptionE100             "FALTA SALDO!"  // "IC金额不足!",
#define CaptionE101             "PAGO ERR CONIC!"  // "IC付款出错!",
#define CaptionE102             "IP DIRECCION ERR!"  // "IP地址错"
#define CaptionE103             "NO CHIP CARD!"  //  ,
#define CaptionE105		        "HAY PEDIDO ABIERTO"    //CWXXI73//Ccr							//CWXXI73

#define CaptionERR105             "BATERIA BAJA!"  // "电池电量低",
#define CaptionERR106             "NO ABRIR STOCK!"	//20070806"没激活库存"
//ccr090715>>>>>>>>>>>>>>>>>>>>>>
#define CaptionERR76			  "INSERTA PEN DRIVE!"	//
#define CaptionERR77			  "ABRE DOC ERR!"		//
#define CaptionERR78			  "LEE PENDRIVE ERR!"		//
#define CaptionERR79			  "ESCRIBE DRIVE ERR!"		//
#define CaptionERR80			  "CREA DOC ERR!"		//
#define CaptionERR81			  "CERRA DOC ERR!"		//
#define CaptionERR82			  "Send LOG 1st"		//
#define CaptionERR83              "GPRS ERROR!"
#define CaptionERR84              "ERROR-SEND!"
#define CaptionERR85              "ERR-RECEIVE"
#define CaptionERR86              "No SERVER!"
#define CaptionERR87              "IP/PORT:NULL"    //没有设置服务器IP和端口
#define CaptionERR88              "NO SIM CARD"
#define CaptionERR89              "GPRS NOT OK"
#define CaptionERR90              "OUT OF RANGE" //CWXXI90  //
#define CaptionERR91              "NULL ACCOUNT"  //CWXXI91


//ccr2016-10-24 EJ >>>>>>>>>>>>>>>>>>>
#define EJ_CWXXI92  "ERROR DATOS EJ"   	//CWXXI92
#define EJ_CWXXI93  "NO SD TARJETA"        	//CWXXI93
#define EJ_CWXXI94  "ERROR EJ E/W"   	//CWXXI94
#define EJ_CWXXI95  "SIN REGISTR EJ"   	//CWXXI95
#define EJ_CWXXI96  "EJ CASI LLENO"   	//CWXXI96
#define EJ_CWXXI97  "EJ est? lleno"   	//CWXXI97
#define EJ_CWXXI98  "NUEVO EJ"        	//CWXXI98
#define EJ_CWXXI99  "REINICIAR EJ"   	//CWXXI99
#define EJ_CWXXI100 "EJ NO COINCIDE"     	//CWXXI100


//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#define CaptionE104             "ABRE CAJA"  // "打开钱箱",
#define CaptionE106             "HAY PEDIDO ABIERTO"    //YOUGUADAN
#define CaptionE107             " CANT.ARTI.PVP"    //SHURUQTYAMT
#define CaptionE108             "PASA A"    //ZHUANRUDAO

#define MsgPUTOUT               "PO"   	                //PUTOUT
#define MsgRECIEVIN             "RA" 		            //RECIEVIN

#define DownLoad1               ">> LETRA"  // "正在下载字库",
#define DownLoad2               ">> FOTO"  // "正在下载图形",


#define OptionN1                " SI"  // "是",
#define OptionN2                " NO"  // "否",
#define OptionN3                "NOMBRE:"  // "名称:",
#define OptionN4                "DEPT:"  // "部类:",
#define OptionN5                "GRUPO:"  // "柜组:",
#define OptionN6                "IMPRIMIR:"  // "打印:",
#define OptionN7                "OPCION:"  // "选项:",
#define OptionN8                "CONEXION:"  // "链接:",
#define OptionN9                "IMPUESTO%:"  // "税率:",
#define OptionN10               "LOCACION NO:"  // "位置编号:",
#define OptionN11               "PRECIO 1:"  // "价格 1:",
#define OptionN12               "PRECIO 2:"  // "价格 2:",
#define OptionN13               "PRECIO 3:"  // "价格 3:",
#define OptionN14               "PRECIO 4:"  // "价格 4:",

#define OptionN15               "COSTE:"  // "进价:",
#define OptionN16               "DESCUENTO(%):"  // "折扣率(%):",
#define OptionN17               "MAXIMO:"  // "最大值:",
#define OptionN18               "DESCUENTO:"  // "折扣率:",
#define OptionN19               "MAXIMO:"  // "最大值:",
#define OptionN20               "IMPUESTO%:"  // "汇率:",
#define OptionN21               "COSTE:"  // "买入价:",
#define OptionN22               "PRECIO:"  // "卖出价:",
#define OptionN23               "ESTADO:"  // "状态:",
#define OptionN24               "CAJA:"  // "钱箱:",
#define MSGOTD                  "OTD"                    //OTDMSG
#define OptionN25               "IMPRIME COCINA:"  // "厨房打印:",
#define MSGPERIOD               "PERIOD"                      //PERMSG
#define MSGPOINTER              "PUNTERO"                     //POIMSG         

#define OptionN26               "PERIODO:"  // "前缀符",,6个字符
#define OptionN27               "CONEXION:"  // "连接:",
#define OptionN28               "FUNCION "  // "输入功能码:",
#define OptionN29               "PULSE TO:"  // "按功能键:",
#define OptionN30               "TIPO:"  // "类型:",
#define OptionN31               "INICIO:"  // "开始日期:",
#define OptionN32               "FECHA-FIN:"  // "结束日期:",
#define OptionN33               "HORA-INICIO:"  // "开始时间:",
#define OptionN34               "HORA-FIN:"  // "结束时间:",
#define OptionN35               "SEMANA:"  // "星期:",
#define OptionN36               "DESCUENTO :"  // "折扣:",
#define OptionN37               "MASIVA:"  // "大包数量:",
#define OptionN38               "PVP:"  // "单价:",
#define OptionN39               "OFERTA:"  // "优惠价:",
#define OptionN40               "PROTOCOL"  // "通讯协议:",
#define OptionN41               "TELIFONO NO#:"  // "电话号码:",
#define OptionN42               "CONTRASENA:"  // "密码:",
#define OptionN43               "FRECUENCIA:"  // "频率:",
#define OptionN44               "MIN.�:"  // "最小金额:",
#define OptionN45               "INTERFAZ :"  // "通讯口:",
#define OptionN46               "PUNTOS �:"  // "计点金额:",
#define OptionN47               "FOTO NO:"  // "图象序号:",
#define OptionN48               "HORA"  // "时间:",
#define OptionN49               "FECHA"  // "日期:",
#define OptionN50               "PRINTER"
#define OptionN51               "TYPE:"      //DZCHXHAO//"秤型号:",

#define WelcomeN1                "CAJA REGISTRADORA!"  // "创 造 优 秀 优 创 造 就\n    欢迎使用优创收款机" */
#define WelcomeN2                " DO THE BEST! \nMade by EUTRON"  // "创造优秀优创造就\n优创蓝宝石收款机" */

#define MessagesN1                "CAJA BLOQUEADA"  // "  收款机已锁定",
#define MessagesN2                "ENCIENDE MODO"  // "按【方式锁】开机",
#define MessagesN3                "FACTURA"  // "形式发票",                  																						//XSHFPIAO
#define MessagesN4                "COCINA#"  // "厨房打印机#",																						//CHFDYJHAO
#define MessagesN13               "<>"
#define MessagesN14               "TOTAL"  ////KPTOTAL


#define CaptionM1                 "ENERO"  // "一月",
#define CaptionM2                 "FEBRERO"  // "二月",
#define CaptionM3                 "MARZO"  //  "三月",
#define CaptionM4                 "ABRIL"  //  "四月",
#define CaptionM5                 "MAYO"  // "五月",
#define CaptionM6                 "JUNIO"  // "六月",
#define CaptionM7                 "JULIO"  // "七月",
#define CaptionM8                 "AGOSTO"  // "八月",
#define CaptionM9                 "SEPTIEMBRE"  // "九月",
#define CaptionM10                "OCTUBRE"  // "十月",
#define CaptionM11                "NOVIEMBRE"  // "十一月",
#define CaptionM12                "DICIEMBRE"  // "十二月",

#define CaptionW1                 "LUNES"  // "星期一",
#define CaptionW2                 "MARTES"  // "星期二",
#define CaptionW3                 "MI."  // "星期三",
#define CaptionW4                 "JUEVES"  // "星期四",
#define CaptionW5                 "VIERNES"  // "星期五",
#define CaptionW6                 "SABADO"  // "星期六",
#define CaptionW7                 "DOMINGO"  // "星期日",


#define ClueN1                    "TECLAR CONFIRMA"  // "按确认键...",

  //Option for KP
#define OptionS1                  "PRINT SUM:"  // "打印总额:",
#define OptionS2                  "I.M.PLATO"  // "菜相同打印",
#define OptionS3                  "I.D.PLATO"  // "菜不同打印",
  //Option for dept
#define OptionS4                  "PVP 0"  // "禁止零价格",
#define OptionS5                  "PRINT.SEP."  // "打印分隔线",
#define OptionS6                  "ITEM VENTA:"  // "单项销售:",
#define OptionS7                  "DTO ITEM 1:"  // "折扣项 1:",
#define OptionS8                  "DTO ITEM 2:"  // "折扣项 2:",
  //Options for tend
#define OptionS9                  "ABRE CAJA:"  // "打开钱箱:",
#define OptionS10                 "TECLA VAL:"  // "输入金额:",
#define OptionS11                 "NO PULSE:"  // "禁止输入:",
#define OptionS12                 "TECLA NO#:"  // "输入号码:",
#define OptionS13                 "NO CAMBIO"  // "找零为小费",
  //Options for PoRa
#define OptionS14                 "ABRE CAJA:"  // "打开钱箱:",
#define OptionS15                 "PAGO FORMA"  // "付款方式键",
#define OptionS16                 "INGRESO:"  // "存入金额:",
#define OptionS17                 "ENTRADA/SALIDA"  // "入金/出金",
  //Option for Disc
#define OptionS18                 "PRINT.DTO."  // "打印折扣额",
#define OptionS19                 "APLICA DTO1:"  // "使用折扣1:",
#define OptionS20                 "APLICA DTO2:"  // "使用折扣2:",
  //Options for currency
#define OptionS21                 "VUELTO"  // "卖出价找零",         
  //Options for Draw
#define OptionS22                 "SIN CENTIMO:"  // "无小数点:",
#define OptionS23                 "ABRE CAJA:"  // "打开钱箱:",
  //Options for Tax
#define OptionS24                 "+ VENTA:"  // "营业/增值:",
#define OptionS25                 "PRINT.TASA"  // "打印税率项",
#define OptionS26                 "ES GST"  // "打印0税项",
#define OptionS27                 "IMPUESTO-GST"  // "为GST类税",
#define OptionS28                 "PRINT.IGV:"  // "打印税额:",
  //Options for clerk
#define OptionS29                 "NO LIMITE:"  // "取消限制:",
#define OptionS30                 "ALTERA LMT:"  // "更改限制:",
#define OptionS31                 "NO PEDIDO:"  // "取消交易:",
#define OptionS32                 "LIMITE DEV.:"  // "退货限制:",
#define OptionS33                 "LIMITE-DTO:"  // "%折扣限制:",
#define OptionS34                 "LMT.COMIS.:"  // "%加成限制:",
#define OptionS35                 "LIMITE-�"  // "+-金额限制",
#define OptionS36                 "FORMACION:"  // "培训模式:",

#define MESS_ECR_IP         "ECR IP ADDRESS:"
#define MESS_ECR_ID         "ECR ID(0-9,A-F):"

#define EditN1                    "<BARCODE>TECLAR GRABAR\n<CONFIRMA>  ACTUAL GRABAR\n"  //---ZY  //ccr "<记录号>  新纪录\n<确认>  当前纪录\n",
#define EditN2                    "<BARCODE> +/- ARTICULO.\n<CONFIRMA> ARTI.\n"   //---ZY  //ccr "<记录号>增删商品\n<确认>  当前商品\n",
#define EditN3                    "<SALIR>   SALIR"                       //---ZY  // "<退出>      退出",
#define EditN4                    "<CONFIRMA>  CONTINUAR\n"                 //---ZY  // "<确认>      继续\n",
#define EditN5                    "ELIGE DOC"  // "按<选择>报表",
#define EditN7                    "TECLADO  ->TECLADO "                    //---ZY  //"键码     ->键   ",
#define EditN8                    "SOLO GTE."  // "只能在经理档用",
#define EditN9                    "PARA VENTA"  // "可在营业档用",
#define EditN10                   "IMPRIME LETRAS"  // "打印文字",
#define EditN11                   "(RESERVAR)"  // "保留",
#define EditN12                   "TEST DOC CLAVE"  // "文件密码测试",
#define EditN13                   "DISCAR NUMERO"  // "拨号",
#define EditN14                   "CONEXON MODEM!"  // "MODEM 连接!",
#define EditN15                   "CODE:"  // "代码:",
#define EditN16                   "DESCENTO%"  // "%折扣",
#define EditN17                   "COMIS.%"  // "%加成",
#define EditN18                   "DOC NO#.:"  // "收据号:",
#define EditN19                   "DOC CANTIDAD"  // "文件数："，0
#define EditN20                   "DOC ESPACIO"  // "文件空间："?
#define EditN21                   "CALCULA DOC"  // "统计文件"2
#define EditN22                   "CALCULAR IMPUESTO"  // “计税”8
#define EditN23                   "MOVIMIENTO CUENTAE SPACIO"  // "流水空间"
#define EditN24                   "DOC NOMBRE CANTIDAD ESPACIO"  // "文件名      记录数      空间大小"
#define EditN25                   "NOMBRE SERIE TIPO COMUNICACION"  // "串口名称    通信类别   通信协议"
#define EditN26                   "SUPERIOR L.:"  // '顶部空行数',
#define EditN27                   "LIN/PAG:"  // '每页行数',
#define EditN28                   "PRINT INFO"  // '打印信息',
#define EditN29                   "EL SEGUINDA DOC:"  // '第二张票据',
#define EditN30                   "ZONA IZQA:"  // '左边距',
#define EditN31                   "TICKET TIPO   :"  // '票打机型',
#define EditN32                   "IMPRESORA OFF"  // "打印机关",
#define EditN33                   "IMPRESORA ON"  // "打印机开",
  //ChipCard>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define ChipCardN1                "CONTRASENA:"  //I "密码:",
#define ChipCardN2                "CONFIRMA CLAVE:"  // "密码确认:",
#define ChipCardN3                "POS CODE:"  // "POS代码:",
#define ChipCardN4                "CONFIRMA CODE:"  // "代码确认:",
#define ChipCardN5                "GRABADO CODE"  // "代码已保存",
#define ChipCardN6                "CODE ERROR ！"  // "代码错 ！",
#define ChipCardN7                "USE IC:"  // "启用IC卡:",
#define ChipCardN8                "T.DTO:"  // "折扣卡:",
#define ChipCardN9                "T.CASH:"  // "现金卡:",
#define ChipCardN10               "T.CREDITO:"  // "赊帐卡:",
#define ChipCardN11               "PRINT IC"  // "打印IC卡",
#define ChipCardN12               "AUTO-DTO :"  // "自动折扣:",
#define ChipCardN13               "PUNTOS:"  // "消费积分:",
#define ChipCardN14               "USA � :"  // "用现金键:",
#define ChipCardN15               "USA PLAZO"  // "启用有效期", //
#define ChipCardN16               "PERDIDA TARJETA"  // "挂失卡号",
#define ChipCardN17               "T.NO:"  // "卡号:",
#define ChipCardN18               "T.TIPO:"  // "卡类型:",
#define ChipCardN19               "SALDO:"  // "余额"
#define ChipCardN20               "T.FIANZA:"  // "卡押金:",
#define ChipCardN21               "CONSUMO:"  // "消费:",
#define ChipCardN22               "TOTAL RECARGA:"  // "充值总额:",
#define ChipCardN23               "CANT DE UTILIZA:"  // "使用次数:",
#define ChipCardN24               "PVP CATEGORIA:"  // "价格级别:",
#define ChipCardN25               "PIN CODE:"  // "PIN码:",
#define ChipCardN26               "PROTECCION NIVEL:"  // "保护级别:",
#define ChipCardN27               "AUTO-PAGO:"  // "自动结帐:",
#define ChipCardN28               "HORA/CREA.T:"  // "制卡日期:",
#define ChipCardN29               "T.PLAZO:"  // "卡有效期:",
#define ChipCardN30               "CLIENTE:"  // "客户:",
#define ChipCardN31               "INICIAR IC"  // "初始化IC",
#define ChipCardN32               "RECARGA"  // "IC卡充值",
#define ChipCardN33               "BORRAR IC "  // "清除IC卡",
#define ChipCardN34               "+/- �?"  // "充值/退款?",
#define ChipCardN35               "-/+ PUNTOS?"  // "减点/送点?",
#define ChipCardN36               "CONFIRMA+ �?"  // "确认充值?",
#define ChipCardN37               "CONFIRMA- �?"  // "确认退款?",
#define ChipCardN38               "CONFIRMA + PUNTOS?"  // "确认送点?",
#define ChipCardN39               "PAGA PREMIO?"  // "兑奖确认?",
#define ChipCardN40               "PREMIO POR PUNTO"  // "计点兑奖",
#define ChipCardN41               "DEVOLVE�IC"  // "IC卡退款",
#define ChipCardN42               "PUNTOS-PREMIO IC"  // "IC卡兑奖点数",
#define ChipCardN43               "PUNTO-REGALO IC"  // "IC卡送点数",
#define ChipCardN44               "ESPERA POR FAVOR......"  // "请等待......",
#define ChipCardN45               "HAY SALDO, DEVULVE � PRIMERO...."  // "卡内有金额,请先退款....",
#define ChipCardN48               "PARA  NUEVO IP, ENCIENDE"  // "要启用新的IP,请重新开机.",
#define ChipCardN49               "QUITA DOC?"  //  "确认清报表?",
#define ChipCardN50               "ESPERA IC...."  // "等待IC卡....",
#define ChipCardN51               "DBLHIGH PRINT" //---zy  // "倍高打印",

//ccr2015-06-15>>>>>>>>>>>>>>>>>>>>>
#define ChipCardN52             "BORRADO"          
#define ChipCardN53             "DEVOLUCION"        
#define ChipCardN54             "PAGO"                       
#define ChipCardN55             "CHARGO"        
#define ChipCardN56             "INICIAR"              
#define ChipCardN57             "TOMAR EFECTIVO"                 
#define ChipCardN58            "ANDIR PUNTOS"   //ADDVIPPOINTS        
#define ChipCardN59            "DESCUENTOS PUNTOS"   //DECVIPPOINTS        
#define ChipCardN60            "SUMAR PUNTOS"  //VIPPOINTS        
//ccr2015-06-15<<<<<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MESSEJ_X_STORE      "SALVAR EJ X"//MsgEJ_X_STORE
#define MESSEJ_Z_STORE      "SALVAR EJ Z"//MsgEJ_Z_STORE
#define MESSEJ_TR_STORE     "SALVAR EJ TR"//MsgEJ_TR_STORE
#endif
//ccr2016-02-22>>>>>>>>>>>>>>>>>>>>>
#define MsgMENUXREPORT     "X REPORT"
#define MsgMENUXEXPORT     "EXPORT(X)"
#define MsgMENUQUICKVIEW   "GRAND TOTAL"
#define MsgMENUXLOGFUNC     "VER REGISTRO"                                 
#define MsgMENUSENDNEWLOG  "ENVIAR NUEVO REGISTRO"      //MENUSENDNEWLOG  //发送新流水数据
#define MsgMENUXPASSWORD   "X-PASSWORD"
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgMENUXEJOURNAL    "E Journal"     //MENUXEJOURNAL 
#endif
//.................................
#define MsgMENUZREPORT     "Z REPORT"
#define MsgMENUZEXPORT     "EXPORT(Z)"
#define MsgMENUCLEARLOG     "CLEAR LOG"
#define MsgMENUZGRAND      "SET CLEAR TOTAL"
#define MsgMENUZPASSWORD   "Z-PASSWORD"
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgMENUZRESETEJ   "Clear EJ"     //MENUZRESETEJ
#endif
//...................................


#define EPosN1                    "EPOS CLAVE:"  // "ePOS认证码:",
#define EPosN2                    "CUENTA EN ESPERA"  // "等待易通卡"           
#define EPosN3                    "CUENTAS:"  // "易通卡号:",              
#define EPosN4                    "EPOS BALANCE:"  // "易通卡余额:",


#define ErrInfoN1                 "DATOS LARGO"  // "通讯数据过长"
#define ErrInfoN2                 "***   FALLO PRINT, OTRAVEZ ***"  // " *** 打印被中断、重新打印 *** "
#define ErrInfoN3                 "Check error!"  // "数据校验错"
//PRINT



#define FisTextN1                 "CONTRIBUYENTE NO#:"   //"纳税人号:"
#define FisTextN2                 "CREA IMPUESTODOC"   //"开出税票        "
#define FisTextN3                 "LICENCIA NO# "   //"授权号码        "
#define FisTextN4                 "ESCRIBIR MAQUINA NO#"   //"写机器号        "
#define FisTextN5                 "CONFIRMA ESCRITO"   //"写入确认        "
#define FisTextN6                 "IMPRIMIR DATOS "   //"数据印出        "
#define FisTextN7                 "USUARIO NO# "   //"用户号码        "
#define FisTextN8                 "SIN IMPUESTO    "   //"免税状态        "
#define FisTextN9                 "Z-NUMERAR:        "   //"Z-计数:         "
#define FisTextN10                "CANT VENTA  "   //"累计销售        "
#define FisTextN11                "TOTAL IMPUESTO "   //"累计税          "
#define FisTextN12                "SUM SIN-I.G.V"   //"总免税额        "
#define FisTextN13                "VENTA TOTAL  "   //"总销售额        "
#define FisTextN14                "TOTAL I.G.V   "   //"总税额          "
#define FisTextN15                "CANT IMPUESTO/DIA"   //"日税票数        "
#define FisTextN16                "CANT DOC "   //"收据总数        "
#define FisTextN17                "NUMERO EXPORT:    "   //"余报表数 :      "

#define CurrentTimeN1                    "HORA ACTUAL:"      //  "当前时间"
#define InputMode1				"A:" // "A："
#define InputMode2				"ZONA:" // "区："
#define InputMode3				"ESCRIBIR:" // "拼："
#define InputMode4				"WUBI:" // "笔："

#define SpecialSymbol1			"->"			//DOWNLOAD//Ccr "★",        "DESCARGAR"
#define SpecialSymbol2			"<-"			//UPLOAD//Ccr "☆",          "CARGAR"
#define SpecialSymbol3			"^   v"		// "↑  ↓",

#define MESS_PRESSKEY   "PRESI.CUALQUIER KEY"                                       

#if defined(CASE_QRCODE)
#define InputQRCode           "QRCODE"  //MsgQRCODE
#endif

#define GPRSServerIP			"SERVIDOR IP"//"服务器IP"               
#define GPRSServerPort		    "PORT"//"端口号"                                         

/////////////////////////////////////////////////////////
//added by lqw 090802
#if  (defined(CASE_GPRS))
#define GPRSAPNName        "APN"           //GPRSAPNNAME
#define GPRSUserName        "NOMBRE DE USUARIO"           //GPRSUSERNAME             
#define GPRSPassword        "Password"           //GPRSPASSWORD

#define GPRSSendMESS        "LECTURA DE GPRS" //ccr2014-11-11 NEWSETUP                 
#define GPRSSetMode         "FIJAR MODO ENVIO"    //gprsSETMODE //ccr2014-11-11 NEWSETUP-step2      
#define GPRSSendECRLogAll   "SEND ALL REGISTROS"    //gprsSendECRLogAll //ccr2014-11-11 NEWSETUP-step2  
#define GPRSDownloadPLU     "ACTUALIZAR PLU"    //gprsDownloadPLU //ccr2014-11-11 NEWSETUP-step2      
#define GPRSDownloadDEPT    "ACTUALIZAR DEPT"    //gprsDownloadDEPT //ccr2014-11-11 NEWSETUP-step2      
#define GPRSDownloadCLERK   "ACTUALIZAR CAJERO"    //gprsDownloadCLERK //ccr2014-11-11 NEWSETUP-step2   
#define GPRSDownloadHEAD    "ACTUALIZAR CABEZERA"       //gprsDownloadHEAD //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadTRAIL   "ACTUALIZAR RUTA"      //gprsDownloadTRAIL //ccr2014-11-11 NEWSETUP-step2 
#define GPRSDownloadALL     "ACTUALIZAR TODO"      //gprsDownloadALL //ccr2016-08-18                
#define GPRSRestart         "RESET GPRS"      //gprsRestart //ccr2016-08-26                           

#define GPRSstr1 "DATA ERROR!"  // "数据类型错"
#define GPRSstr2 "GPRS OK"
#define GPRSstr3 "GPRS NOT OK"
#define GPRSstr4 "CONFIRM?"
#define GPRSstr5 "IP/PORT:NULL"
#define GPRSstr6 "ERROR:IP/PORT"
#define GPRSstr7 "WAITING GPRS"
#define GPRSstr8 "SI"   // "是"
#define GPRSstr9 "NO"    // "fou "
#define GPRSstr10 "ENVIANDO DATOS.."  // "数据发送中.. "
#define GPRSstr11 "ENVIADO DATOS.."  // "数据已发出.. "
#define GPRSstr28 "ENVIAR CON EXITO.."						// "发送成功.."
#define GPRSstr31 "CONECTAR ERR"	//						// "连接失败"
#define GPRSstr32 "ACCEPTA FALLO"                                           // "接受失败"
#define GPRSstr33 "CONFIRMA ERR"			// "发送确认失败"
#define GPRSstr34 "ENVIA ERR"							// "发送失败"
#define GPRSstr58 "ESTABLECIENDO CONEXION.."		// "正在建立连接.."
#define GPRSstr59 "PREPARA A CONEXION.."		// " 准备建立连接.."
#define GPRSstr60 "RESTAURA MODULO.."	// "正在复位模块.."
#define GPRSstr61 "YA,SALE PORFAVOR.." // "发送完毕,请退出"

#define GPRSxACK  "Wait ACK Error"

#endif //end defined(CASE_GPRS)
///////////////////////////////////////////////////////////////////
#define InsertUSB		"<<Insert USB"//CHRUSB  插入U盘 //

//ccr090715>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define usbMAIN1		"COPIAR A USB"			//usbDAOCHU	导出收款机文件 //
#define usbMAIN2		"COPIAR DESDE USB"			//usbDAORU 导入收款机文件 //  
#define usbMAIN3		"REGISTRAR A USB"		//usbFLOW 导出销售流水 //       


#define usbFLOWNEWMESS	    "ACTUALIZACION DE EXPOR"      //1-导出新数据   //       
#define usbFLOWALLMESS	    "EXPORTAR TODO"      //2-导出全部数据              
#define usbFLOWDELMESS	    "EXPORT/RESET"    //3-导出全部数据后清空

#define usbALLFile		"TODOS LOS ARCHIVOS"		//usbALL 全部文件 //    


#define MESSPRNMENU     "<00>Print IT"
#define MESSSETMENU     "LISTA DE MANUAL"                                   
#define MESSCLEANLOG    "QUITA MEMORIA?  "
#define MESSCLEANLOGA   "TODO EL REGISTRO!"                                
#define MESSRAMOUT      "NO MAS ESPACIO"
#define MESSCLEANFILES  "QUITA DOC DATOS?"
#define MESSCONFIGERR   "CAMBIADO DATOS CONFIGURADO\nDATOS CAUSAN ERR!"
#define MESSDELPLU      "SISTEMA CAMBIADO \n      BORRADO ARTICULO!"
#define MESSPLUPRINTEND "PLU Printed O.K.\nPress <ENTER>!"


#define RECALLFLOW      "RECUPERAR REGISTRO"                                                 

#define USBREPNAME      "PROYECTO NOMBRE"           

////////  CSV导出文件标题  /////////////////////////////////////////////
#define DEPTCAP       "REFERENCIA,NOMBRE,GRUPO,OPCION,VENTA\r\n"
#define PLUCAP        "REFERENCIA,NOMBRE,GRUPO,PRECIO 1,PRECIO 2,PRECIO 3,PRECIO 4, COSTE,COMBINADA VENTA,STOCK\r\n"

#define TAXCAP        "NOMBRE,TASA IMPUESTO,OPCION\r\n"
#define HEADERCAP     "LETRA-CABEZA-DOC"
#define DISCCAP       "NOMBRE,PRINT OPCION ,OPCION,FIJO DESCUENTO,TASA IMPUESTO,MAXIMO VALOR\r\n"

#define SALPERCAP     "NOMBRE, OPCION\r\n"
#define SYSTEMCAP     "SISTEMA CONTROL SENAL\r\n"
#define COMMCAP       "NOMBRE,TIPO,COMPANERO CONVENIO,TELEFONO NO#\r\n"
#define GRAPPRNCAP    "FOTO REF.,INICIO,FECHA  FIN\r\n"
#define GROUPCAP      "NOMBRE, MAXIMO VALOR\r\n"
#define ICCARDCAP     "CONFIG.IC SOCIO TARJETA\r\n"
#define PROMCAP       "CANT PREMIO,MIN.�,PUNTOS,MIN PUNTO,FOTO NO,NOTA\r\n"
#define PBFUNCCAP     "NOMBRE,PRINT CONTROL,,OPCION\r\n"
#define CLERKCAP      "NOMBRE,OPCION, CONTRASENA\r\n"
#define MODICAP       "SABOR INSTRUCCION \r\n"
#define PBTOTALCAP    "REFERENCIA,NOMBRE"
#define ZONECAP       "TIEMPO 1,TIEMPO 2,TIEMPO 3,TIEMPO 4,TIEMPO 5,TIEMPO 6,TIEMPO 7,TIEMPO 8,TIEMPO 9,TIEMPO 10,TIEMPO 11,TIEMPO 12\r\n"
#define KEYBOARDCAP   "DEFINE TECLADO\r\n"
#define OFFCAP        "NOMBRE,TIPO,INICIO,FECHA FIN,FROM,TO ,DISCOUNT,NUMITES,PVP,PRICEP\r\n"
#define PORACAP       "NOMBRE,PRINT CONTROL,OPCION, CAJA, MAXIMO VALOR\r\n"

#define TRAILERCAP    "LETRA-PIE-DOC\r\n"
#define SLIPHEADCAP   "CABEZA-TIC\r\n"
#define ICBLOCKCAP    "IC TARJETA  LISTA NEGRA  PARA SOCIO\r\n"


#define EXPORTCSV    "1-CSV,2-BIN:"

#define CSVFLOETITL "PROYECTO NOMBRE, CANTIDAD, IMPORTE,ALBARIN NO,CAJERO,VENDEDOR, FECHA, CAJA, LOCAL,DOC TIP .\r\n"
#define CSVREPTITL  "FECHA,CAJA,LOCAL\r\n"

#define BUTTONBATLOW "BAJA BATERIA!"
#define CHARGEBATNOW "CARGA BATERIA!"

#define SRAMNOTFOUND "SRAM NO ENCONTARDO!"                           

#define INPUTPLUCODE "CODIGO DE PLU:"                                        

#define MessageE18      "!!!! FAT(PLU) ERROR !!!!"                                    

#define MESS_TABLEHOLD  " MESA SUSPENDIDA"                             

#define MESS_LOGREPET   "Re-Send LOG!"                                                                                     

#define MESS_PAYALSO   "confirmar pago?"                   


#define MGADDINV        "INVENTARIO +"  //1+SUBTOTAL                    
#define MGDECINV        "INVENTARIO -"  //2+SUBTOTAL                   
#define MGSETPRICE      "PRECIO PLU"  //3+SUBTOTAL                       

#define MGCLEARIC       "DEVOLUCION IC"               
#define MGINITIC        "INIT IC"                                                     
#define MGCHARGE        "CHARGE IC"
#define MGADDPOINT      "ADD POINT"
#define MGPRINTSTOCK   "LISTA DE STOCK"                   

#define MGPASSWORD      "CONFIG. CLAVE"                                
#define MGCLOSEPRN      "CERRAR IMPRESION"                                                                               
#define MGTRAINING      "MODO DE ENTRENAMIENTO"                                    
#define MGGOTOMG        "ADMINISTRACION"                           

//..............................................
#define PLUCODE     "CODIGO DE PLU"                                   
#define INVQTYMUST  "INGRESO DE CANTIDAD/PRECIO"          
#define ININVENTORY "Press <SUBT> Key\n Exit Inventory!"
//..............................................
#define DEVICENULL		" NULL"
#define DEVICEPORT1     "PORT#1"
#define DEVICEPORT2     "PORT#2"
#define DEVICEPORT3     "PORT#3"
#define DEVICEPORT4     "PORT#4"

//................................................
#define CLOSEPRINTER    "CERRAR IMPRESION"                                          
#define CLEARGRANDTOTAL "BORRAR GT DESPUES DE Z"                                
#define CLEANALLECRVIP  "BORRADO DE CUENTA?"//"清除会员数据?"        

                       //012345678901234567890123456789012
//.......功能码对应的名称.........................................
#define MsgKEYCORR      "CORRECCION"                                                                                 
#define MsgKEYCURR      "MONEDA"                                   
#define MsgKEYDISC      "DESCUENTO"                                      
#define MsgKEYPBFUN     "PB-FUNC"
#define MsgKEYPORA      "PORA"
#define MsgKEYPO_RA      "PAGOS/RECIDO"                                     
#define MsgKEYTEND      "TENDER"                                                                                                    
#define MsgKEYCLEAR     "BORRAR"                                                  
#define MsgKEYFEED      "AVANZE"                                   
#define MsgKEYMULT      "X"
#define MsgKEYSUBTOT    "SUBTOTAL"
#define MsgKEYPRICE     "PRICE"
#define MsgKEYPRICEN    "PRICE#"
#define MsgKEYPRICE1    "PRICE1"
#define MsgKEYPRICE2    "PRICE2"
#define MsgKEYPRICE3    "PRICE3"
#define MsgKEYPRICE4    "PRICE4"
#define MsgKEYSHIFT     "CAMBIAR"                          
#define MsgKEYDATE      "DATO"                            
#define MsgKEYWEIGHT    "PESO"                                 
#define MsgKEYCLERKN    "CAJERO # "                              
#define MsgKEYDEPT      "DEPT~"
#define MsgKEYDEPTNo    "DEPT#"
#define MsgKEYPLUNo     "PLU#"
#define MsgKEYDRAW      "ABRIR CAJA"                         
#define MsgKEYMEMO      "MODIFICAR"                                
#define MsgKEYCLERK     "CAJERO*"                                
#define MsgKEYPLUDIR    "PLU*"                                        
#define MsgKEYLOCK      "TECLA DE MODO"                             
#define MsgKEYDRAWFUN   "FUNCION GAVE/IMAG"                           
#define MsgKEYSALPN     "VENDEDOR#"                                
#define MsgKEYSALP      "VENDEDOR"                   
#define MsgKEYDOT       "'.'"
#define MsgKEYZERO2     "'00'"
#define MsgKEYNUMBER    "0~9"
#define MsgKEYSUSPEND   "HOLD"
#define MsgFUNCLOOK1    "FUNC LOOK1"
#define MsgFUNCLOOK2    "FUNC LOOK2"
#define MsgMODELOCK     "MODO TECLA"                              
#define MsgVIPLOGIN     "REGISTRO DE CUENTA"                                  
//jdb2018-03-08
#define MsgSCLOGIN     	"SC REGISTRO"                               
#define MsgPWDLOGIN     "PWD REGISTRO"                                              


#define MsgINPUTNUM     "INGRESO DE CODIGO"                                    
#define MsgCUSTOMER    "COMPRADOR"                                      
#define MsgKEYNULL      "(OTROS)"                                   

#define MsgRHEADER      "ENCABECEDO*"                                        
#define MsgRTRAILER     "LETRA PIE*"                                        
#define MsgKEYBOARD     "TECLADO*"                                      
#define MsgSYSFLAGS     "SISTEMA*"                                         
#define MsgSETDATE      "FECHA/HORA*"                                           

//...........................................
#define MsgCONFIRM      "CONFIRMADO"                                     
#define MsgNET          "MONTO NETO"  // "净额",                          
#define VIPLOGOUT       "CERRAR LA SESION"                   
#define VIPTOTAL        "IMPORTE"                                     
//jdb2018-03-08
#define VIPLESSVAT		"MENOS VAT"                         
#define VIPDISC			"DESCUENTO"                             

#define VIPTOTALA       "SUMA DE IMPORTES"                                   
#define DECVIPPOINT     "DESCUENTO"//长度必须为4                         
#define ADDVIPPOINT     "ANDIR"//长度必须为4                        
#define INPUTVIPNAME    "NOMBRE:"                                 
#define PRINTKEYBOARD   "IMPRESION DE TECLADO"                               
//...........................................
#define MESSALLLOG         "REGISTRO X # DE RECIBO"            
#define MESSDATERANGE     "REGISTRO X FECHA"                
#define MESSVIPLOG        "INGRESO DE CUENTA"                                
#define MESSPRNCASH       "IMPRIMIR EFECTIVO"		//"打印付款记录"               
#define MESSPRNDEPART     "IMPRIMIR DEPARTAMENTO"	//"打印部类记录"          
#define MESSPRNPLU        "IMPRIMIR PLU"		//"打印单品记录"         
#define MESSPRNDISCOUNT   "IMPRIMIR DESCUENTO"	//"打印折扣加成"          
#define MESSPRNRAPO       "IMPRIMIR RECIBIDO/PAGOS"		//"打印出入金"           
#define MESSPRNCORRECT    "IMPRIMIR CORRECCIONES"	//"打印更正操作"         
#define MESSPRNCANCEL     "IMPRIMIR CANCELAR"   //"打印交易取消"                   
#define MESSPRNNOSALE     "IMPRIMIR NO VENTA"	//"打印非销售"                   

#define MESSALLCASH		  "TODO EN EFECTIVO"		//"全部付款方式"    
#define MESSSELCASH		  "UNO DE EFECTIVO"		//"选择付款方式"     
#define MESSALLDEPT		  "TODOS LOS DEPTS"		//"打印全部部类"          
#define MESSSELDEPT		  "UNO DE DEPTS"		//"选择一个部类"   
#define MESSALLPLU		  "TODOS LOS PLU"		//"打印全部单品"                    
#define MESSSELPLU		  "UNO DE LOS PLU"		//"选择一个单品"   
//........................................
//ccr2016-03-15>>>>定义键盘宏>>>>>
#define MESSMACRO         "MACRO"//  "宏定义"
//....ListItems提示信息....................................
#define It_MGITEMS      "FUNCION DE ADMINISTRADOR"                                          
#define It_SELEFUNC     "FUNCION DE LA TECLA"                   
#define It_SELEITEM     "SELECCIONE UN ARTICULO"                          
#define It_SELEKEYTYPE  "TIPO DE TECLADO"                            
#define It_SELETEST     "SELECION DE ARTICULO"                             
#define It_SELELOG      "SELECION DE REGISTRO"                               
#define It_SELETEND     "SELECION DE TIPO"                                         
#define It_SELEDEPT     "SELECION DE DEPARTAMENTO"                                           
#define It_SELEPLU      "SELECION DE PLU"                                             
#define It_SELEMODI     "SELECION MODI"                                       
#define It_SELEREPORT   "IMPRIMIR REPORTE"                                           
#define It_SELEPOINT    "SELECCIONAR PUNTERO"// 小数位数       
#define It_SELEEJ       " EJ      "                            
//.........................................................
#define MESSRAMVAR      "VARIABLE"
//.........................................................
#define PLUNUMBER           "CONTADOR DE REGISTRO:"               
#define PLURANDOMSIZE     	"TAMANO DE CODIGO:"                         
#define PLUNAMESIZE		"TAMANO DE NOMBRE:"                   
#define PLUPRICESIZE        "TAMANO DE PRECIO:"                           
#define PLUPRICELEVEL       "TAMANO DE CUENTA:"                          
#define PLUCOSTSIZE         "TAMANO DE COSTO:"                             
#define PLUINVENTSIZE       "TAMANO DE STOCK:"                            
#define PLUREPQTYSIZE       "TAMANO DE REPOS,QTY:"                   
#define PLUREPAMTSIZE       "TAMANO DE REPOSI AMT:"                          
#define PLUREPDISCSIZE      "TAMANO DE REPOS, DESCUENT:"                  
#define PLUREPCOSTSIZE      "TAMANO DE REPOSI, DE COSTO:"
//.........................................................
#define POINTTYPE           "PUNTOS DECIMALES"                        
#define POINTTYPE1          "1.000,00"  //两位小数
#define POINTTYPE2          "1,000.00"  //两位小数
#define POINTTYPE3          "   1.000"
#define POINTTYPE4          "   1,000"
#define POINTTYPE5          "1.000,000"  //三位小数
#define POINTTYPE6          "1,000.000"  //三位小数
//#define POINTTYPE7          "1.000,0"  //一位小数
//#define POINTTYPE8          "1,000.0"  //一位小数
//.......................................................
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
#define MESSEJMESSAGES      "ACERCA DE EJ"    //MsgEJMESSAGES
#define MESSEJALLEJ         "TODO"
#define MESSEJDATERANGE     "EJ POR FECHA"
#define MESSEJRECEIPT       "EJ DE RECIBO"
#define MESSEJXREPORT       "EJ DE X INFORM"
#define MESSEJZREPORT       "EJ DE Z INFORM"
#define MESSEJCLERK         "EJ DE CAJERO"
#endif
//.......................................................
#define DATETYPE        "FORMAT "
#define DDMMYYYY        "DDMMYYYY"
#define MMDDYYYY        "MMDDYYYY"
#define YYYYMMDD        "YYYYMMDD"
//.......................................................
#define XUPDATEBIOS     "Can't Update \nfirmware by USB"
//ccr2016-06-22..........................................
#if defined(CASE_QRCODE)
#define MESSWXPAY       "OK WX-PAY"
#endif

#define SENDLOGAUTO     "ENVIO AUTOMATICO:"              
#define DAYSFORLOG      "DIAS PARA ENVIAR:"             
#define ACKMUSTNEED     "ACK NECESARIO:"

#define CSV_TITLE       "NOMBRE,CANTIDAD,IMPORTE\r\n"

//ccr2017-01-10>>>>>>>>>>>>>>>>>>>>>>>>>
#define Msg_NOSTOCK     "SIN STOCK DE CAMPO"                          
#define Msg_DISPSTOCK   "MOSTRAR/IMPRIMIR?"                              
//ccr2017-01-10<<<<<<<<<<<<<<<<<<<<<<<<<<

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
#define Msg_FORMAT      "BORRAR EJ"
#define Msg_FORMATED    "BORRADO"
#define Msg_RECALLEJ    "RECUERDA EJ"
#define Msg_EJNOTEMPTY  "EJ NO VACIO"

#define Msg_ECRDATEltEJ "FECHA EJ ECR"


//#define MESSEJMESSAGES      "ABOUT EJ"    //MsgEJMESSAGES
//#define MESSEJALLEJ         "ALL OF EJ"
//#define MESSEJDATERANGE     "EJ BY DATE"
//#define MESSEJRECEIPT       "EJ OF RECEIPT"
//#define MESSEJXREPORT       "EJ OF X-REPORT"
//#define MESSEJZREPORT       "EJ OF Z-REPORT"
//#define MESSEJCLERK         "EJ OF CLERK"

#endif
#define Msg_MANAGEMENT "MANAGEMENT LIST "

#endif
