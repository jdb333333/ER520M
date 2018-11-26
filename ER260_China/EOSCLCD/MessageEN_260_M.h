#ifndef MESSAGEH_EN
#define MESSAGEH_EN

//jdb2018-07-20 ������Ϣ
#define TAXANAME "TAX_A"             
#define TAXBNAME "TAX_B"                 
#define TAXCNAME "TAX_C"               
#define TAXDNAME "TAX_D"              
#define TAXENAME "TAX_E"                
#define TAXFNAME "TAX_F"                 
#define TAXZERONAME "0_VAT" //��˰      
#define TAXVATNAME "SIN_VAT" //��˰      
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
#define RETNUMPRE "J" //jdb2018-07-31 �˻�Ʊ�ݺ�ǰ׺

#define RETURNSLIP "RETORNO DE DESLIZAMIENTO"
#define TOTALACCSALES "TOTAL DE VENTAS ACUMULADAS"
#define INPUTREFNUM "  REF#"       
#define RECOVRECPARAM "RECOV ECR PARAM"

#define Modifier1             "INSTRUCCION"        //"˵��    "
#define ClerkRec1             "CAJERO"        //"�տ�Ա  "
#define GroupRec1             "GRUPO"        //"����    "
#define DeptRec1              "DEPT"        //"����    "
#define PLURec1               "ARTICULO"        //"��Ʒ    "
#define SalesPersonRec1       "VENDEDOR"        //"ӪҵԱ  "
#define OffRec1               "OFERTA"        //"�������"

#define XZReportList1     "CIERRE DIARIO"        //"�����ձ���",
#define XZReportList2     "SEMANA-VENTA"        //"�����±���",
#define XZReportList3     "DOC-ARTICULO"        //"��Ʒ����",
#define XZReportList4     "MESA CUENTA"        //"���ʱ���",
#define XZReportList5     "DOC-TIEMPO"        //"ʱ�α���",
#define XZReportList6     "DIA CAJERO"        //"�տ�Ա�ձ���",
#define XZReportList7     "SEMANA CAJERO"        //"�տ�Ա�±���",
#define XZReportList8     "D-TODOCAJERO"        //"ȫ�տ�Ա�ձ���",
#define XZReportList9     "S-TODOCAJERO"        //"ȫ�տ�Ա�±���",
#if (salNumber)
#define XZReportList10    "DIA-VENDEDOR"        //"ӪҵԱ�ձ���"
#define XZReportList11    "SEMANA VEND."        //"ӪҵԱ�±���"
#define XZReportList12    "DIA-TODOVEND."        //"ȫӪҵԱ�ձ���",
#define XZReportList13    "SEM-TODOVENDEDO"        //"ȫӪҵԱ�±���",
#endif
#if (BARCUSTOMER==1)
#define XZReportList14    "SOCIO/BARCODE"        //"�����Ա����",
#endif

#define PortType1               "PC"        //"  ����",
#define PortType2               "SCANER"        //"����ǹ",
#define PortType3               "BALANZ"        //"���ӳ�",
#define PortType4               "COCINA"        //"��  ��",
#define PortType5               "TICKET"        //'Ʊ��'��20040322
#define PortEXTPRN      "EP-PORT"//ccr2016-12-14 ExtPrinter����
#define PortType6               "MR IC"        //-------090421 BY ZY//"��Ƶ��"
#define PortType7               "E CARD"        //"Eͨ��"

#define KClose                  "  CERRAR"        //"   �� ",

#define SysFlags0		        "LOCAL NO:"	// "���̱��"
#define SysFlags1               "CAJA NO: "        //"�տ����:"
#define SysFlags2               "ROUNDING:"        //"���뷽ʽ:"
#define SysFlags3               "CAMBIA PVP:"        //"��Ʒ���:"
#define SysFlags4               "SILBATO"        //"������:"
#define SysFlags5               "DESIGNA C"        //"ǿ���տ�Ա"
#define SysFlags6               "CAMBIAR C"        //"�ı��տ�Ա"
#define CLERKPASS               "CLAVE C."        //"�տ�Ա����"
#define SysFlags8               "PRINT.VEND."        //"��ӡӪҵԱ"
#define SysFlags9               "PRINT C."        //"��ӡ�տ�Ա"
#define SysFlags10              "PRINT DOC#"        //"��ӡ�վݺ�"
#define SysFlags11              "OSCURO PRINT"        //"��Ũ�ȴ�ӡ"
#define SysFlags12              "DESIGNA V."        //"ǿ��ӪҵԱ"
#define SysFlags13              "IMPRIME#"        //"��ӡ����:"
#define SysFlags14              "PULSES.TOTAL:"        //"��С�Ƽ�:"
#define SysFlags15              "IMPR.VENTA"        //"��ӡ��Ŀ��"
#define SysFlags16              "IMPRIME DOC:"        //"��ӡСƱ:"
#define SysFlags17              "CABEZA-TIC:"        //"СƱƱͷ:"
#define SysFlags18              "BLOQ SHIFT:"        //"����SHIFT:"
#define SysFlags19              "MUESTRAHORA:"        //"��ʾʱ��:"
#define SysFlags20              "COPIA DOC:"        //"�����վ�:"
#define SysFlags21              "ESPECIAL #:"        //"��������:"
#define SysFlags22              "MONTA DIA:" //-----090421 BY ZY//"������־"
#define SysFlags23              "PRINT.MESA:"        //"��̨��ӡ:"
#define SysFlags24              "PRINT.STCOK:"        //"��ӡ���"
#define SysFlags25              "INTERRUPTOR:"        //"��ӡ����:"
#define SysFlags26              "OBL.MESA:"        //"���뿪̨:"
#define SysFlags27              "CONSERVA#"        //"�����վݺ�"
#define SysFlags28              "COPYCOCINA:"        //"���Ƴ���:"
#define SysFlags29              "RECUPERA#"        //"�վݺŸ�λ"
#define SysFlags30              "ALTA IMPRIME:"    //-----090421 BY ZY//"���ߴ�ӡ"
#define SysFlags31              "PARIDAD:"   //-----090421 BY ZY//"�Աȶ�:"
#define SysFlags32              "CBZ.CENTRO:"  // "Ʊͷ���д�ӡ"
#define SysFlags33              "PIE CENTRO:"  // "Ʊβ���д�ӡ"
#define SysFlags34              "ALTA CABEZA:"  // "Ʊͷ���ߴ�ӡ"
#define SysFlags35              "ALTA PIE:"  // "Ʊβ���ߴ�ӡ"
//ccr2016-03-21#define SysFlags36              "To .CSV "
#define SysFlags37              "PRINT FECHA"
#define SysFlags38              "PRINT HORA"
#define SysFlags39              "PRINT INDEX"    //"��ӡȡ�˺�"
#define SysFlags40              "HEART BEAT"    //ccr2015-08-03"�����������Heart beat"
#define SysFlags41              "PreFix1:"      //ccr2016-08-02
#define SysFlags42              "PreFix2:"      //ccr2016-08-02


#define GrapType1               "MARCA"        //"�̵�ͼ��",
#define GrapType2               "PREMIO"        //"�н�ͼ��:",
#define GrapType3               "LOGO-PIE"        //"Ʊβͼ��:",
#define GrapType4               "IMPRIME ?"        //"��ӡ����?",

#define GrapSet1                "NO#:"        //"���:",
#define GrapSet2                "FOTO REF."        //"ͼ����:",
#define GrapSet3                "INICIO "        //"��ʼ����:",
#define GrapSet4                "FECHA-FIN:"        //"��������:",
  /////==============CHN msg===============//////

#define CurrentMode1            "ABIERTO "  //"Ӫҵ",
#define CurrentMode2            "LEE CUENTA   "  //"����",
#define CurrentMode3            "QUITA CUENTA   "  //"����",
#define CurrentMode4            "CONFIG. "  //"����",
#define CurrentMode5            "GERENTE  "  //"����",

#define SetupMode1              "FUNCION GRAL."  //---090421 BY ZY// "�ۺϹ���",
#define SetupMode2              "GESTION-ART."  // "��Ʒ����",
#define SetupKeyboard           "KEYBOARD"      //DEFKEYBOARD  //
#define msgCOMMHOST             "COMM-HOST"
#define SetupMode3              "VICE SET"    // "ͨѶ������",
#define SetupMode4              "CONFIG.VENTA"    // "���۹�������",
#define SetupMode5              "ACCOUNT"   // "��Ա������",
#define SetupMode6              "CABEZA-PIE"  // "Ʊ����ʽ",
#define SetupMode7              "GESTION-DOC    "    // "������",
#define msgDISCOUNT             "DISCOUNT"      //MENUDISC //
#define SetupMode8              "CONFIGURA"   // "��������",
#define SetupMode9              "CAJERO-CLAVE"    // "����,
#define SetupMode10             "PROBAR CAJA"       // "�տ��ά��",


#define SysSettings1            "CONFIG.SISTEMA"     // "ϵͳ��������",
#define SysSettings2            "CONFIG.FECHA"     // "����",
#define SysSettings3            "CONFIG.HORA"     // "ʱ��",
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define msgSETEJOURNAL          "CONFIG.EJ"      //SETEJOURNAL //EJOURNAL
#endif

#define SysSettings4            "CONFIG.TECLADO"     // "����",
#define MESSKEYMACRO            "KEYB. MACRO" // "���̺�"
#define MESSKEYTYPE             "KEYBOARD TYPE"             // "����ģʽ"

#define SysSettings5            "TECLADOCOMIDA"      // "���������л�",
#define SysSettings6            "TECLADOSUPERM"    // "���м����л�",
#define SysSettings8            "GPRS FUNCTION"          //"GPRSͨ��"  //SETGPRS //ccr2014-11-11 NEWSETUP

#define SetupCaption1           "GRUPO"			//"����"
#define SetupCaption2           "DEPT"	//"����",
#define SetupCaption3           "ARTICULO"			//"��Ʒ",
#define SetupCaption4           "IMPRIME ART.#"			//"��ӡ��Ʒ�嵥",
#define msgSETTEST              "PRUEBA DE ECR"//"ϵͳ����"            //SETTEST    
#define msgSETCLEARPLU          "BORRADO DE PLU"//"�����Ʒ"        //SETCLEARPLU  
#define msgSETPLULIST           "PRINT DEPT/PLU"//"��ӡ��Ʒ�嵥"      //SETPLULIST          
#define msgSETTSTVER	        "PRINT VERSION"//"��ӡ�汾"      //SETTSTVER
#define msgSETCONFIGPLU         "FORM PLU"//"���õ�Ʒ"          //SETCONFIGPLU                
#define msgSETCONFIGDEPT        "FORM DEPT" //���ò���         //SETCONFIGDEPT
#define msgSETINSTALL           "INSTAL/BACKUP"//"��װ/����"        //SETINSTALL               
#define msgSETPRINTFAT          "PRINT MAT"//"��ӡ�ļ������"   //SETPRINTFAT


#define SetupCaption5           "COMPUTER"			// "ͨѶ��1",
#define SetupCaption6           "SCANNER"			// "ͨѶ��2",
#define SetupCaption7           "SCALE"			// "ͨѶ�� 3",
//#define SetupCaption8           "PORT-4"			// "ͨѶ�� 4",
#define SetupCaption9           "ETHERNET"			// "��������",

#define SetupCaption10          "PRINT DOC"			// "Ʊ�ݴ�ӡ",
#define SetupCaption11          "PRINT.COCINA"			// "������ӡ",
#define SetupEXTPrinter         "EXTERNAL PRINT"    //SETEXTPRINTER

#define SetupCaption12          "OFERTA"				// "�Żݼ�",
#define SetupCaption13          "CONFIG.PREMIO"			// "�н�����",
#define SetupCaption14          "DESCUENTO"				// "�ۿ�",
#define SetupCaption15          "CAJA +/- �"				// "�������",
#define SetupCaption16          "REVISAR"			// "��������",
#define SetupCaption17          "PAGO FORMA "			// "���ʽ",
#define SetupCaption18          "DIVISA"        // "���",
#define SetupCaption19          "CAJA"        // "Ǯ��",
#define SetupCaption20          "IMPUESTO%"        // "˰��",

#define SetupCaption21          "PONER CUENTA"  // "IC������",                                
#define SetAddNewECRVIP         "ANDIR CUENTA"                         
#define SetPrintECRVIP          "IMPRESION CUENTA"                    
#define SetCleanALLVIP          "BORRAR CUENTA"                           

#define SetupCaption22          "PERDER.IC"        // "IC����ʧ",
#define SetupCaption23          "PRINT IC"        // "��ӡ����Ϣ",
#define SetupCaption24          "CABEZA-TIC"        // "Ʊͷ",
#define SetupCaption25          "PIE-TIC"        // "Ʊβ",
#define SetupCaption26          "CABEZA-DOC"        // "Ʊ��Ʊͷ",
#define SetupCaption27          "PIE FOTO "        // "ͼ��",
#define SetupCaption28          "IMPRIME FOTO"  // "��ӡͼ��",
#define SetupCaption29          "CONFIG.DOC"        // "���ñ�������",
#define SetupCaption30          "DOC MODULO"  // "����Ԫ",
#define SetupCaption31          "CONFIG.HORA"        // "����ʱ������",
#define SetupCaption32          "QUITA TODODOC"     // "���屨��",
#define SetupCaption33          "IMPRIME C/C"         // "��ӡ��ˮ",

#define SetupCaption34          " MESA"  // "��̨",
#define SetupCaption35          "ALIMENTACION"  // "��������",
#define SetupCaption36          "MENU"      //ZY  // "�˵�˵��",
#define SetupCaption37          "FIRMAR CUENTA"          //ZY  // "ǩ��",
#define SetupCaption38          "CAJERO"  // "�տ�Ա",
#define SetupCaption39          "VENDEDOR"  // "ӪҵԱ",
#define SetupCaption40          "CONFIG.CLAVE"          //ZY  // "���õ�����",
#define SetupCaption41          "CLAVE-GERENTE"          //ZY  // "��������",
#define SetupCaption42          "CLAVE-LEER"     //ZY  // "ֻ����������",
#define SetupCaption43          "CLAVE-QUITA"     //ZY  // "���ʱ�������",
#define SetupCaption44          "MEMORIA TEST"        //ZY  // "��ӡ����",
#define SetupCaption45          "DISPLAY TEST"       //ZY  //"��ʾ����",
#define SetupCaption451         "COMPRADORES"      // ���Բ���                          
#define SetupCaption46          "PRUEBA DE IMPRESORA"      //ZY  //"��ӡ�汾",          
#define SetupTestRAM           "CHEQUEO DE RAM"        //SETTSTMEM	       //RAM����       
#define SetupCaption47          "PRUEBA DE TIEMPO"      //ZY  //"ʱ�Ӳ���",         
#define SetupCaption48          "SILBATO TEST"     //ZY  //"����������",
#define SetupCaption49          "CAJA TEST"     //ZY  //"Ǯ�����",
#define SetupCaption50          "TECLADO TEST"       //ZY  //"���̲���",
#define SetupCaption51          "SERIE TEST"        //ZY  //"���ڲ���",
#define SetupCaption52          "AUTO-TEST"       //ZY  //"�Զ�����",

#define msgSETPRNMENU           "PRINT MENU"      //SETPRNMENU   //

#define OperationN1             "GRABAR#"  // "��¼#",
#define OperationN2             "GRABAR?"           //ZY  // "��¼?",

#define OperationN12            " CONTRASENA"  // "����",
#define OperationN13            "CONFIR.CLA"  // "����ȷ��",
#define OperationN16            "SUPERMERCADO"  // "���а�",
#define OperationN17            "RESTAURANT"  // "������",
#define OperationN18            "RESTRAURANTE"  // "����",


#define CaptionN1               "TOTAL"  // "Ӧ��",
#define CaptionN3               "PVP"  // "�۸�",
#define CaptionN8               "VUELTO"  // "����",

#define CaptionN39              "ANADIR?"  // "����?",
#define CaptionN40              "BORRAR?"  // "ɾ��?",
#define CaptionN47              "OPCION:"  // "ѡ��:",
#define CaptionN48              "FIJO"  // "�̶�",��Ҫ����SETINPUTWD�Ŀ��
#define CaptionN49              "FLEXI."  // "����",��Ҫ����SETINPUTWD�Ŀ��
#define CaptionN50              "FIJO/FLEX."  // "��/��",��Ҫ����SETINPUTWD�Ŀ��
#define CaptionN62              "CONFIG.PREMIO:"  // "�н��趨:",
#define CaptionN63              "PUNTOS:"  // "���ѻ���:",
#define CaptionN64              "ALTERA PVP"  // "�ı䵥Ʒ�۸�",
#define CaptionN65              "COCINA#"  // "������ӡ����"��
#define CaptionN66              " +STOCK"  // "���ӿ��",
#define CaptionN67              "-STOCK"  // "���ٿ��",
#define CaptionN68              "TECLAR "  // "����",
#define CaptionN69              "TOTAL "  // "����",


#define CaptionE1               "ERROR RS232"  // "RS232��",
#define CaptionE2               "CORRECTO RS232"  // "RS232��ȷ",
#define CaptionE3               "RTC ERROR!"  // "RTC ��!",
#define CaptionE4               "CTC ERROR!"  // "CTC ��!",
#define CaptionE5               "FPROM ERROR!"  // "FPROM��!",
#define CaptionE6               "INICIAR..."  // "��ʼ��...",
#define CaptionE7               "YA INICIADO."  // "��ʼ������.",
#define CaptionE8               "ERROR-"  // "����-",//����<SCREENWD

#define CaptionE9               "CLAVE ERROR!"  // "����� ��",
#define CaptionE10              "GRABADO CLAVE"  // "�����ѱ���",
#define CaptionE15              "CONFIRMAR?"  // "ȷ��?",
#define CaptionE16              "TERMINADO"  // "���",

#define CaptionE25              "NO APAQUE EN ACTUALIZAR"  // "���³�������ϵ�",
#define CaptionE26              "DESBORDADO DE ARCHIVO!"  // "�ļ��ռ�Խ��",                
#define CaptionE27              "DESBORDADO MEMORIA��"  // �ڴ������
#define CaptionE28              "MEMORIA LLENA"  // "��ˮ�洢����",
#define CaptionE29              "CAMBIA ROLLO"  // "�������ӡֽ",
#define CaptionE30              "MEMORIA EXTERNAL ERR��"  // "��չ�ڴ����",



#define CaptionE32              "INVALIDO!"  // "��Ч����!",
#define CaptionE33              "FECHA INVALIDO!"  // "��Ч����!",
#define CaptionE34              "HORA INVALIDO"  // "��Чʱ��!",
#define CaptionE35              "NO TECLEAR!"  // "��ֹ����!",          
#define CaptionE36              "PLU ES VACIO!"  // "PLU����Ϊ��!",
#define CaptionE37              "PLU DOC LLENO!"  // "PLU�ļ����!",
#define CaptionE38              "MESA OCUPADA!"  // "��̨��ռ��!",
#define CaptionE39              "MESA ABIERTA!"  // "��̨�Ѵ�!",
#define CaptionE40              "MESA EQUIVOCADA#"  // "��̨�Ų���!",          
#define CaptionE41              "MESA NO ERROR!"  // "��������̨��",                         
#define CaptionE42              "MESA LIBRE"  // "û�п�̨",
#define CaptionE43              "CUENTA-MESA LLENA"  // "��̨�������",
#define CaptionE44              "NO MODIF.HORA"  // "��ֹ�޸�ʱ��",
#define CaptionE45              "EN VENTA!"  // "����������!",
#define CaptionE46              "RAM-VENTA LLENA"  // "���ۻ�������",
#define CaptionE47              "STOCK ARTICULO"  // "��Ʒû���۳�",
#define CaptionE48              "DURANTE COBRO!"  // "���ڽ�����!",
#define CaptionE49              "DATOS LARGO"  // "�������ݳ���",
#define CaptionE50              "DURANTE PAGO!"  // "���ڸ���!",
#define CaptionE51              "CLIENTE LLENO!"  // "�������!",
#define CaptionE52              "FALTA PAGO"  // "û��ȷ�Ͻ���",
#define CaptionE53              "NO CANCELE PAGO"  // "��ֹȡ������",
#define CaptionE54              "SIN ESTE COMIDA!"  // "�޲���!",
#define CaptionE55              "SIN ESTE C!"  // "�޴��տ�Ա!",
#define CaptionE56              "CAMBIADO C"  // "�տ�Ա�ı�",
#define CaptionE57              "SIN ESTE DOC!"  // "�޴��౨��!",
#define CaptionE58              "FALLO PRINT DOC"  // "�����ӡ�ж�",
#define CaptionE59              "OBL.GERENTE"  // "�����ھ���",
#define CaptionE60              "NO REPARTICION-CUENTA"  // "���ܷ���ʱ��",
#define CaptionE61              "OBL.NO#!"  // "�����������",
#define CaptionE62              "KEY POS ERROR"  // "Կ�׵�λ��",
#define CaptionE63              "SIN AUTORIZACION!"  // "δ��Ȩ!",
#define CaptionE64              "RESIGNA C"  // "��ָ���տ�Ա",
#define CaptionE65              "RESIGNA VEND"  // "��ָ��ӪҵԱ",
#define CaptionE66              "NOCAMIBE PLU-PVP��"  // "��ֹPLU��ۣ�",
#define CaptionE67              "CLAVE ERROR!"  //  "���벻��!",
#define CaptionE68              "COCINA ERR"  // "������ӡ����",
#define CaptionE69              "PRINT ERROR"  // "Ʊ�ݴ�ӡ����", //ccr2017-08-31
#if defined(CASE_INNERDOTPRN)
#define CaptionE70              "LEVANTA EJE"  // "��ӡѹ��̧��",
#else
#define CaptionE70              "SIN ROLLO!"  // "��ӡѹ��̧��",
#endif
#define CaptionE71              "SIN ROLLO!"  // "��ӡֽ����!",
#define CaptionE72              "TEMPERATURA ALTA"  // "��ӡ�¶�̫��",
#define CaptionE73              "SIN DEFINIR BOTON!"  // "����δ����!",
#define CaptionE74              "OBLIGATORIO�"  // "����������",
#define CaptionE75              "NO-FORMA-PAGO"  // "��ֹ���ָ���",
#define CaptionE76              "SIN ESTE FUNCION"  // "�����޴˹���",
#define CaptionE77              "FALTA PULSES.TOTAL!"  // "δ��С�Ƽ�!",
#define CaptionE78              "EN GESTION-STOCK"  // "���ڹ�����",
#define CaptionE79              "GRABA C/C ERR"  // "������ˮ��",
#define CaptionE80              "COM-MODEM ERR"  // "MODEMͨѶ��!",


#define CaptionE81              "MONTA-T ERR!"  // "����������!"
#define CaptionE82              "POS BARCODE ERR!"  // "POS�����!"
#define CaptionE83              "T.DATOS ERR!"  // "�����ݴ�!"
#define CaptionE84              "T.CADUCADA!"  // "Ϊ���ڿ�!"
#define CaptionE85              "T.PERDIDA!"  // "Ϊ��ʧ��!"
#define CaptionE86              "CLIENTE TIPO ERR!"  // "�ͻ�����!"
#define CaptionE87              "T.NUEVA!"  // "Ϊ�¿�!"
#define CaptionE88              "T.INVALIDA!"  // "���ǹ��￨!"
#define CaptionE89              "LEE T.ERR!"  // "д������!"
#define CaptionE90              "T.NO ERR!"  // "���Ų���!"
#define CaptionE91              "NO T.DTO!"  // "�����ۿۿ�!"
#define CaptionE92              "NO T.CASH!"  // "�����ֽ�!"
#define CaptionE93              "NO T.CREDITO!"  // "�������ʿ�!"
#define CaptionE94              "NO USE IC!"  // "����IC��!"
#define CaptionE95              "QUITA T.ERR!"  // "�忨����!",
#define CaptionE96              "DATOS LLENOS!"  // "�������!",
#define CaptionE97              "RECARGA IC ERR!"  // "IC��ֵ����!",
#define CaptionE98              "INICIA IC ERR"  // "IC��ʼ������",
#define CaptionE99              "NO INICIE IC!"  // "��ֹ��ʼ��!",
#define CaptionE100             "FALTA SALDO!"  // "IC����!",
#define CaptionE101             "PAGO ERR CONIC!"  // "IC�������!",
#define CaptionE102             "IP DIRECCION ERR!"  // "IP��ַ��"
#define CaptionE103             "NO CHIP CARD!"  //  ,
#define CaptionE105		        "HAY PEDIDO ABIERTO"    //CWXXI73//Ccr							//CWXXI73

#define CaptionERR105             "BATERIA BAJA!"  // "��ص�����",
#define CaptionERR106             "NO ABRIR STOCK!"	//20070806"û������"
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
#define CaptionERR87              "IP/PORT:NULL"    //û�����÷�����IP�Ͷ˿�
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


#define CaptionE104             "ABRE CAJA"  // "��Ǯ��",
#define CaptionE106             "HAY PEDIDO ABIERTO"    //YOUGUADAN
#define CaptionE107             " CANT.ARTI.PVP"    //SHURUQTYAMT
#define CaptionE108             "PASA A"    //ZHUANRUDAO

#define MsgPUTOUT               "PO"   	                //PUTOUT
#define MsgRECIEVIN             "RA" 		            //RECIEVIN

#define DownLoad1               ">> LETRA"  // "���������ֿ�",
#define DownLoad2               ">> FOTO"  // "��������ͼ��",


#define OptionN1                " SI"  // "��",
#define OptionN2                " NO"  // "��",
#define OptionN3                "NOMBRE:"  // "����:",
#define OptionN4                "DEPT:"  // "����:",
#define OptionN5                "GRUPO:"  // "����:",
#define OptionN6                "IMPRIMIR:"  // "��ӡ:",
#define OptionN7                "OPCION:"  // "ѡ��:",
#define OptionN8                "CONEXION:"  // "����:",
#define OptionN9                "IMPUESTO%:"  // "˰��:",
#define OptionN10               "LOCACION NO:"  // "λ�ñ��:",
#define OptionN11               "PRECIO 1:"  // "�۸� 1:",
#define OptionN12               "PRECIO 2:"  // "�۸� 2:",
#define OptionN13               "PRECIO 3:"  // "�۸� 3:",
#define OptionN14               "PRECIO 4:"  // "�۸� 4:",

#define OptionN15               "COSTE:"  // "����:",
#define OptionN16               "DESCUENTO(%):"  // "�ۿ���(%):",
#define OptionN17               "MAXIMO:"  // "���ֵ:",
#define OptionN18               "DESCUENTO:"  // "�ۿ���:",
#define OptionN19               "MAXIMO:"  // "���ֵ:",
#define OptionN20               "IMPUESTO%:"  // "����:",
#define OptionN21               "COSTE:"  // "�����:",
#define OptionN22               "PRECIO:"  // "������:",
#define OptionN23               "ESTADO:"  // "״̬:",
#define OptionN24               "CAJA:"  // "Ǯ��:",
#define MSGOTD                  "OTD"                    //OTDMSG
#define OptionN25               "IMPRIME COCINA:"  // "������ӡ:",
#define MSGPERIOD               "PERIOD"                      //PERMSG
#define MSGPOINTER              "PUNTERO"                     //POIMSG         

#define OptionN26               "PERIODO:"  // "ǰ׺��",,6���ַ�
#define OptionN27               "CONEXION:"  // "����:",
#define OptionN28               "FUNCION "  // "���빦����:",
#define OptionN29               "PULSE TO:"  // "�����ܼ�:",
#define OptionN30               "TIPO:"  // "����:",
#define OptionN31               "INICIO:"  // "��ʼ����:",
#define OptionN32               "FECHA-FIN:"  // "��������:",
#define OptionN33               "HORA-INICIO:"  // "��ʼʱ��:",
#define OptionN34               "HORA-FIN:"  // "����ʱ��:",
#define OptionN35               "SEMANA:"  // "����:",
#define OptionN36               "DESCUENTO :"  // "�ۿ�:",
#define OptionN37               "MASIVA:"  // "�������:",
#define OptionN38               "PVP:"  // "����:",
#define OptionN39               "OFERTA:"  // "�Żݼ�:",
#define OptionN40               "PROTOCOL"  // "ͨѶЭ��:",
#define OptionN41               "TELIFONO NO#:"  // "�绰����:",
#define OptionN42               "CONTRASENA:"  // "����:",
#define OptionN43               "FRECUENCIA:"  // "Ƶ��:",
#define OptionN44               "MIN.�:"  // "��С���:",
#define OptionN45               "INTERFAZ :"  // "ͨѶ��:",
#define OptionN46               "PUNTOS �:"  // "�Ƶ���:",
#define OptionN47               "FOTO NO:"  // "ͼ�����:",
#define OptionN48               "HORA"  // "ʱ��:",
#define OptionN49               "FECHA"  // "����:",
#define OptionN50               "PRINTER"
#define OptionN51               "TYPE:"      //DZCHXHAO//"���ͺ�:",

#define WelcomeN1                "CAJA REGISTRADORA!"  // "�� �� �� �� �� �� �� ��\n    ��ӭʹ���Ŵ��տ��" */
#define WelcomeN2                " DO THE BEST! \nMade by EUTRON"  // "���������Ŵ����\n�Ŵ�����ʯ�տ��" */

#define MessagesN1                "CAJA BLOQUEADA"  // "  �տ��������",
#define MessagesN2                "ENCIENDE MODO"  // "������ʽ��������",
#define MessagesN3                "FACTURA"  // "��ʽ��Ʊ",                  																						//XSHFPIAO
#define MessagesN4                "COCINA#"  // "������ӡ��#",																						//CHFDYJHAO
#define MessagesN13               "<>"
#define MessagesN14               "TOTAL"  ////KPTOTAL


#define CaptionM1                 "ENERO"  // "һ��",
#define CaptionM2                 "FEBRERO"  // "����",
#define CaptionM3                 "MARZO"  //  "����",
#define CaptionM4                 "ABRIL"  //  "����",
#define CaptionM5                 "MAYO"  // "����",
#define CaptionM6                 "JUNIO"  // "����",
#define CaptionM7                 "JULIO"  // "����",
#define CaptionM8                 "AGOSTO"  // "����",
#define CaptionM9                 "SEPTIEMBRE"  // "����",
#define CaptionM10                "OCTUBRE"  // "ʮ��",
#define CaptionM11                "NOVIEMBRE"  // "ʮһ��",
#define CaptionM12                "DICIEMBRE"  // "ʮ����",

#define CaptionW1                 "LUNES"  // "����һ",
#define CaptionW2                 "MARTES"  // "���ڶ�",
#define CaptionW3                 "MI."  // "������",
#define CaptionW4                 "JUEVES"  // "������",
#define CaptionW5                 "VIERNES"  // "������",
#define CaptionW6                 "SABADO"  // "������",
#define CaptionW7                 "DOMINGO"  // "������",


#define ClueN1                    "TECLAR CONFIRMA"  // "��ȷ�ϼ�...",

  //Option for KP
#define OptionS1                  "PRINT SUM:"  // "��ӡ�ܶ�:",
#define OptionS2                  "I.M.PLATO"  // "����ͬ��ӡ",
#define OptionS3                  "I.D.PLATO"  // "�˲�ͬ��ӡ",
  //Option for dept
#define OptionS4                  "PVP 0"  // "��ֹ��۸�",
#define OptionS5                  "PRINT.SEP."  // "��ӡ�ָ���",
#define OptionS6                  "ITEM VENTA:"  // "��������:",
#define OptionS7                  "DTO ITEM 1:"  // "�ۿ��� 1:",
#define OptionS8                  "DTO ITEM 2:"  // "�ۿ��� 2:",
  //Options for tend
#define OptionS9                  "ABRE CAJA:"  // "��Ǯ��:",
#define OptionS10                 "TECLA VAL:"  // "������:",
#define OptionS11                 "NO PULSE:"  // "��ֹ����:",
#define OptionS12                 "TECLA NO#:"  // "�������:",
#define OptionS13                 "NO CAMBIO"  // "����ΪС��",
  //Options for PoRa
#define OptionS14                 "ABRE CAJA:"  // "��Ǯ��:",
#define OptionS15                 "PAGO FORMA"  // "���ʽ��",
#define OptionS16                 "INGRESO:"  // "������:",
#define OptionS17                 "ENTRADA/SALIDA"  // "���/����",
  //Option for Disc
#define OptionS18                 "PRINT.DTO."  // "��ӡ�ۿ۶�",
#define OptionS19                 "APLICA DTO1:"  // "ʹ���ۿ�1:",
#define OptionS20                 "APLICA DTO2:"  // "ʹ���ۿ�2:",
  //Options for currency
#define OptionS21                 "VUELTO"  // "����������",         
  //Options for Draw
#define OptionS22                 "SIN CENTIMO:"  // "��С����:",
#define OptionS23                 "ABRE CAJA:"  // "��Ǯ��:",
  //Options for Tax
#define OptionS24                 "+ VENTA:"  // "Ӫҵ/��ֵ:",
#define OptionS25                 "PRINT.TASA"  // "��ӡ˰����",
#define OptionS26                 "ES GST"  // "��ӡ0˰��",
#define OptionS27                 "IMPUESTO-GST"  // "ΪGST��˰",
#define OptionS28                 "PRINT.IGV:"  // "��ӡ˰��:",
  //Options for clerk
#define OptionS29                 "NO LIMITE:"  // "ȡ������:",
#define OptionS30                 "ALTERA LMT:"  // "��������:",
#define OptionS31                 "NO PEDIDO:"  // "ȡ������:",
#define OptionS32                 "LIMITE DEV.:"  // "�˻�����:",
#define OptionS33                 "LIMITE-DTO:"  // "%�ۿ�����:",
#define OptionS34                 "LMT.COMIS.:"  // "%�ӳ�����:",
#define OptionS35                 "LIMITE-�"  // "+-�������",
#define OptionS36                 "FORMACION:"  // "��ѵģʽ:",

#define MESS_ECR_IP         "ECR IP ADDRESS:"
#define MESS_ECR_ID         "ECR ID(0-9,A-F):"

#define EditN1                    "<BARCODE>TECLAR GRABAR\n<CONFIRMA>  ACTUAL GRABAR\n"  //---ZY  //ccr "<��¼��>  �¼�¼\n<ȷ��>  ��ǰ��¼\n",
#define EditN2                    "<BARCODE> +/- ARTICULO.\n<CONFIRMA> ARTI.\n"   //---ZY  //ccr "<��¼��>��ɾ��Ʒ\n<ȷ��>  ��ǰ��Ʒ\n",
#define EditN3                    "<SALIR>   SALIR"                       //---ZY  // "<�˳�>      �˳�",
#define EditN4                    "<CONFIRMA>  CONTINUAR\n"                 //---ZY  // "<ȷ��>      ����\n",
#define EditN5                    "ELIGE DOC"  // "��<ѡ��>����",
#define EditN7                    "TECLADO  ->TECLADO "                    //---ZY  //"����     ->��   ",
#define EditN8                    "SOLO GTE."  // "ֻ���ھ�����",
#define EditN9                    "PARA VENTA"  // "����Ӫҵ����",
#define EditN10                   "IMPRIME LETRAS"  // "��ӡ����",
#define EditN11                   "(RESERVAR)"  // "����",
#define EditN12                   "TEST DOC CLAVE"  // "�ļ��������",
#define EditN13                   "DISCAR NUMERO"  // "����",
#define EditN14                   "CONEXON MODEM!"  // "MODEM ����!",
#define EditN15                   "CODE:"  // "����:",
#define EditN16                   "DESCENTO%"  // "%�ۿ�",
#define EditN17                   "COMIS.%"  // "%�ӳ�",
#define EditN18                   "DOC NO#.:"  // "�վݺ�:",
#define EditN19                   "DOC CANTIDAD"  // "�ļ�����"��0
#define EditN20                   "DOC ESPACIO"  // "�ļ��ռ䣺"?
#define EditN21                   "CALCULA DOC"  // "ͳ���ļ�"2
#define EditN22                   "CALCULAR IMPUESTO"  // ����˰��8
#define EditN23                   "MOVIMIENTO CUENTAE SPACIO"  // "��ˮ�ռ�"
#define EditN24                   "DOC NOMBRE CANTIDAD ESPACIO"  // "�ļ���      ��¼��      �ռ��С"
#define EditN25                   "NOMBRE SERIE TIPO COMUNICACION"  // "��������    ͨ�����   ͨ��Э��"
#define EditN26                   "SUPERIOR L.:"  // '����������',
#define EditN27                   "LIN/PAG:"  // 'ÿҳ����',
#define EditN28                   "PRINT INFO"  // '��ӡ��Ϣ',
#define EditN29                   "EL SEGUINDA DOC:"  // '�ڶ���Ʊ��',
#define EditN30                   "ZONA IZQA:"  // '��߾�',
#define EditN31                   "TICKET TIPO   :"  // 'Ʊ�����',
#define EditN32                   "IMPRESORA OFF"  // "��ӡ����",
#define EditN33                   "IMPRESORA ON"  // "��ӡ����",
  //ChipCard>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define ChipCardN1                "CONTRASENA:"  //I "����:",
#define ChipCardN2                "CONFIRMA CLAVE:"  // "����ȷ��:",
#define ChipCardN3                "POS CODE:"  // "POS����:",
#define ChipCardN4                "CONFIRMA CODE:"  // "����ȷ��:",
#define ChipCardN5                "GRABADO CODE"  // "�����ѱ���",
#define ChipCardN6                "CODE ERROR ��"  // "����� ��",
#define ChipCardN7                "USE IC:"  // "����IC��:",
#define ChipCardN8                "T.DTO:"  // "�ۿۿ�:",
#define ChipCardN9                "T.CASH:"  // "�ֽ�:",
#define ChipCardN10               "T.CREDITO:"  // "���ʿ�:",
#define ChipCardN11               "PRINT IC"  // "��ӡIC��",
#define ChipCardN12               "AUTO-DTO :"  // "�Զ��ۿ�:",
#define ChipCardN13               "PUNTOS:"  // "���ѻ���:",
#define ChipCardN14               "USA � :"  // "���ֽ��:",
#define ChipCardN15               "USA PLAZO"  // "������Ч��", //
#define ChipCardN16               "PERDIDA TARJETA"  // "��ʧ����",
#define ChipCardN17               "T.NO:"  // "����:",
#define ChipCardN18               "T.TIPO:"  // "������:",
#define ChipCardN19               "SALDO:"  // "���"
#define ChipCardN20               "T.FIANZA:"  // "��Ѻ��:",
#define ChipCardN21               "CONSUMO:"  // "����:",
#define ChipCardN22               "TOTAL RECARGA:"  // "��ֵ�ܶ�:",
#define ChipCardN23               "CANT DE UTILIZA:"  // "ʹ�ô���:",
#define ChipCardN24               "PVP CATEGORIA:"  // "�۸񼶱�:",
#define ChipCardN25               "PIN CODE:"  // "PIN��:",
#define ChipCardN26               "PROTECCION NIVEL:"  // "��������:",
#define ChipCardN27               "AUTO-PAGO:"  // "�Զ�����:",
#define ChipCardN28               "HORA/CREA.T:"  // "�ƿ�����:",
#define ChipCardN29               "T.PLAZO:"  // "����Ч��:",
#define ChipCardN30               "CLIENTE:"  // "�ͻ�:",
#define ChipCardN31               "INICIAR IC"  // "��ʼ��IC",
#define ChipCardN32               "RECARGA"  // "IC����ֵ",
#define ChipCardN33               "BORRAR IC "  // "���IC��",
#define ChipCardN34               "+/- �?"  // "��ֵ/�˿�?",
#define ChipCardN35               "-/+ PUNTOS?"  // "����/�͵�?",
#define ChipCardN36               "CONFIRMA+ �?"  // "ȷ�ϳ�ֵ?",
#define ChipCardN37               "CONFIRMA- �?"  // "ȷ���˿�?",
#define ChipCardN38               "CONFIRMA + PUNTOS?"  // "ȷ���͵�?",
#define ChipCardN39               "PAGA PREMIO?"  // "�ҽ�ȷ��?",
#define ChipCardN40               "PREMIO POR PUNTO"  // "�Ƶ�ҽ�",
#define ChipCardN41               "DEVOLVE�IC"  // "IC���˿�",
#define ChipCardN42               "PUNTOS-PREMIO IC"  // "IC���ҽ�����",
#define ChipCardN43               "PUNTO-REGALO IC"  // "IC���͵���",
#define ChipCardN44               "ESPERA POR FAVOR......"  // "��ȴ�......",
#define ChipCardN45               "HAY SALDO, DEVULVE � PRIMERO...."  // "�����н��,�����˿�....",
#define ChipCardN48               "PARA  NUEVO IP, ENCIENDE"  // "Ҫ�����µ�IP,�����¿���.",
#define ChipCardN49               "QUITA DOC?"  //  "ȷ���屨��?",
#define ChipCardN50               "ESPERA IC...."  // "�ȴ�IC��....",
#define ChipCardN51               "DBLHIGH PRINT" //---zy  // "���ߴ�ӡ",

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
#define MsgMENUSENDNEWLOG  "ENVIAR NUEVO REGISTRO"      //MENUSENDNEWLOG  //��������ˮ����
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


#define EPosN1                    "EPOS CLAVE:"  // "ePOS��֤��:",
#define EPosN2                    "CUENTA EN ESPERA"  // "�ȴ���ͨ��"           
#define EPosN3                    "CUENTAS:"  // "��ͨ����:",              
#define EPosN4                    "EPOS BALANCE:"  // "��ͨ�����:",


#define ErrInfoN1                 "DATOS LARGO"  // "ͨѶ���ݹ���"
#define ErrInfoN2                 "***   FALLO PRINT, OTRAVEZ ***"  // " *** ��ӡ���жϡ����´�ӡ *** "
#define ErrInfoN3                 "Check error!"  // "����У���"
//PRINT



#define FisTextN1                 "CONTRIBUYENTE NO#:"   //"��˰�˺�:"
#define FisTextN2                 "CREA IMPUESTODOC"   //"����˰Ʊ        "
#define FisTextN3                 "LICENCIA NO# "   //"��Ȩ����        "
#define FisTextN4                 "ESCRIBIR MAQUINA NO#"   //"д������        "
#define FisTextN5                 "CONFIRMA ESCRITO"   //"д��ȷ��        "
#define FisTextN6                 "IMPRIMIR DATOS "   //"����ӡ��        "
#define FisTextN7                 "USUARIO NO# "   //"�û�����        "
#define FisTextN8                 "SIN IMPUESTO    "   //"��˰״̬        "
#define FisTextN9                 "Z-NUMERAR:        "   //"Z-����:         "
#define FisTextN10                "CANT VENTA  "   //"�ۼ�����        "
#define FisTextN11                "TOTAL IMPUESTO "   //"�ۼ�˰          "
#define FisTextN12                "SUM SIN-I.G.V"   //"����˰��        "
#define FisTextN13                "VENTA TOTAL  "   //"�����۶�        "
#define FisTextN14                "TOTAL I.G.V   "   //"��˰��          "
#define FisTextN15                "CANT IMPUESTO/DIA"   //"��˰Ʊ��        "
#define FisTextN16                "CANT DOC "   //"�վ�����        "
#define FisTextN17                "NUMERO EXPORT:    "   //"�౨���� :      "

#define CurrentTimeN1                    "HORA ACTUAL:"      //  "��ǰʱ��"
#define InputMode1				"A:" // "A��"
#define InputMode2				"ZONA:" // "����"
#define InputMode3				"ESCRIBIR:" // "ƴ��"
#define InputMode4				"WUBI:" // "�ʣ�"

#define SpecialSymbol1			"->"			//DOWNLOAD//Ccr "��",        "DESCARGAR"
#define SpecialSymbol2			"<-"			//UPLOAD//Ccr "��",          "CARGAR"
#define SpecialSymbol3			"^   v"		// "��  ��",

#define MESS_PRESSKEY   "PRESI.CUALQUIER KEY"                                       

#if defined(CASE_QRCODE)
#define InputQRCode           "QRCODE"  //MsgQRCODE
#endif

#define GPRSServerIP			"SERVIDOR IP"//"������IP"               
#define GPRSServerPort		    "PORT"//"�˿ں�"                                         

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

#define GPRSstr1 "DATA ERROR!"  // "�������ʹ�"
#define GPRSstr2 "GPRS OK"
#define GPRSstr3 "GPRS NOT OK"
#define GPRSstr4 "CONFIRM?"
#define GPRSstr5 "IP/PORT:NULL"
#define GPRSstr6 "ERROR:IP/PORT"
#define GPRSstr7 "WAITING GPRS"
#define GPRSstr8 "SI"   // "��"
#define GPRSstr9 "NO"    // "fou "
#define GPRSstr10 "ENVIANDO DATOS.."  // "���ݷ�����.. "
#define GPRSstr11 "ENVIADO DATOS.."  // "�����ѷ���.. "
#define GPRSstr28 "ENVIAR CON EXITO.."						// "���ͳɹ�.."
#define GPRSstr31 "CONECTAR ERR"	//						// "����ʧ��"
#define GPRSstr32 "ACCEPTA FALLO"                                           // "����ʧ��"
#define GPRSstr33 "CONFIRMA ERR"			// "����ȷ��ʧ��"
#define GPRSstr34 "ENVIA ERR"							// "����ʧ��"
#define GPRSstr58 "ESTABLECIENDO CONEXION.."		// "���ڽ�������.."
#define GPRSstr59 "PREPARA A CONEXION.."		// " ׼����������.."
#define GPRSstr60 "RESTAURA MODULO.."	// "���ڸ�λģ��.."
#define GPRSstr61 "YA,SALE PORFAVOR.." // "�������,���˳�"

#define GPRSxACK  "Wait ACK Error"

#endif //end defined(CASE_GPRS)
///////////////////////////////////////////////////////////////////
#define InsertUSB		"<<Insert USB"//CHRUSB  ����U�� //

//ccr090715>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define usbMAIN1		"COPIAR A USB"			//usbDAOCHU	�����տ���ļ� //
#define usbMAIN2		"COPIAR DESDE USB"			//usbDAORU �����տ���ļ� //  
#define usbMAIN3		"REGISTRAR A USB"		//usbFLOW ����������ˮ //       


#define usbFLOWNEWMESS	    "ACTUALIZACION DE EXPOR"      //1-����������   //       
#define usbFLOWALLMESS	    "EXPORTAR TODO"      //2-����ȫ������              
#define usbFLOWDELMESS	    "EXPORT/RESET"    //3-����ȫ�����ݺ����

#define usbALLFile		"TODOS LOS ARCHIVOS"		//usbALL ȫ���ļ� //    


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

////////  CSV�����ļ�����  /////////////////////////////////////////////
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
#define CLEANALLECRVIP  "BORRADO DE CUENTA?"//"�����Ա����?"        

                       //012345678901234567890123456789012
//.......�������Ӧ������.........................................
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
#define MsgNET          "MONTO NETO"  // "����",                          
#define VIPLOGOUT       "CERRAR LA SESION"                   
#define VIPTOTAL        "IMPORTE"                                     
//jdb2018-03-08
#define VIPLESSVAT		"MENOS VAT"                         
#define VIPDISC			"DESCUENTO"                             

#define VIPTOTALA       "SUMA DE IMPORTES"                                   
#define DECVIPPOINT     "DESCUENTO"//���ȱ���Ϊ4                         
#define ADDVIPPOINT     "ANDIR"//���ȱ���Ϊ4                        
#define INPUTVIPNAME    "NOMBRE:"                                 
#define PRINTKEYBOARD   "IMPRESION DE TECLADO"                               
//...........................................
#define MESSALLLOG         "REGISTRO X # DE RECIBO"            
#define MESSDATERANGE     "REGISTRO X FECHA"                
#define MESSVIPLOG        "INGRESO DE CUENTA"                                
#define MESSPRNCASH       "IMPRIMIR EFECTIVO"		//"��ӡ�����¼"               
#define MESSPRNDEPART     "IMPRIMIR DEPARTAMENTO"	//"��ӡ�����¼"          
#define MESSPRNPLU        "IMPRIMIR PLU"		//"��ӡ��Ʒ��¼"         
#define MESSPRNDISCOUNT   "IMPRIMIR DESCUENTO"	//"��ӡ�ۿۼӳ�"          
#define MESSPRNRAPO       "IMPRIMIR RECIBIDO/PAGOS"		//"��ӡ�����"           
#define MESSPRNCORRECT    "IMPRIMIR CORRECCIONES"	//"��ӡ��������"         
#define MESSPRNCANCEL     "IMPRIMIR CANCELAR"   //"��ӡ����ȡ��"                   
#define MESSPRNNOSALE     "IMPRIMIR NO VENTA"	//"��ӡ������"                   

#define MESSALLCASH		  "TODO EN EFECTIVO"		//"ȫ�����ʽ"    
#define MESSSELCASH		  "UNO DE EFECTIVO"		//"ѡ�񸶿ʽ"     
#define MESSALLDEPT		  "TODOS LOS DEPTS"		//"��ӡȫ������"          
#define MESSSELDEPT		  "UNO DE DEPTS"		//"ѡ��һ������"   
#define MESSALLPLU		  "TODOS LOS PLU"		//"��ӡȫ����Ʒ"                    
#define MESSSELPLU		  "UNO DE LOS PLU"		//"ѡ��һ����Ʒ"   
//........................................
//ccr2016-03-15>>>>������̺�>>>>>
#define MESSMACRO         "MACRO"//  "�궨��"
//....ListItems��ʾ��Ϣ....................................
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
#define It_SELEPOINT    "SELECCIONAR PUNTERO"// С��λ��       
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
#define POINTTYPE1          "1.000,00"  //��λС��
#define POINTTYPE2          "1,000.00"  //��λС��
#define POINTTYPE3          "   1.000"
#define POINTTYPE4          "   1,000"
#define POINTTYPE5          "1.000,000"  //��λС��
#define POINTTYPE6          "1,000.000"  //��λС��
//#define POINTTYPE7          "1.000,0"  //һλС��
//#define POINTTYPE8          "1,000.0"  //һλС��
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
