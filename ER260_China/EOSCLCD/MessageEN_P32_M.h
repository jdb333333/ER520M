#ifndef MESSAGEH_EN_P
#define MESSAGEH_EN_P

//jdb2018-07-26 �˻�ʱ���Ӵ�ӡ����
#define RETFOOTER1	"��ESTE DOCUMENTO SERA VALIDO PARA"
#if ORRECEIPT
#define FOOTER1	"��ESTE RECEIPT SERA VALIDO PARA"
#else
#define FOOTER1	"��ESTE RECEIPT SERA VALIDO PARA"
#endif
#define FOOTER2 "5 ANS DESDE LA FECHA DE"                                                            
#define FOOTER3 "USO PERMITIDO��"                                                                  
#define FOOTER4 "��ESTE DOCUMENTO NO ES VALIDO PARA"                                                              
#define FOOTER5 "INGRESO DE TAX RECLAMADO��"                                                                             

#define REFNUM                 "REF#"  //jdb2018-07-26 �˻�����REF#
#define TERMINALREPORT "INFORME FINAL" //jdb2018-07-27                                                   

// Printer
#if (PRTLEN>40)
                       //.123456789.123456789.123456789.123456789.1
#define Header1         "*       LO MEJOR PARA USTED        *"        //" 	 �� �� �� ��  �� �� �� ��   *"
#define Header2         "*       LO MEJOR PARA USTED      *"         //" 	 �� �� �� ��     �� �� �� ��   *"

#else
                       //.123456789.123456789.123456789.1
#define Header1         "*     GRACIAS POR SU COMPRA      *"        //" 	 �� �� �� ��  �� �� �� ��   *"
#if defined(CASE_INDONESIA)
#define Header2         "*   Posibolt Cash Register     *"         //" 	 �� �� �� ��     �� �� �� ��   *"
#else
#define Header2         "*    CAJAS REGISTRADORAS EUTRON     *"         //" 	 �� �� �� ��     �� �� �� ��   *"
#endif
#endif
// Printer
#define ReportType0     "  GRAN TOTAL   "        //"   ��  ��  ��   ",                                                             
#define ReportType1     "  GRUPO         "        //" ��  ��  ��  �� ",//1                                               
#define ReportType2     "  DEPARTAMENTO  "        //" ��  ��  ��  �� ",//2                                                      
#define ReportType3     "  PLU           "        //" ��  Ʒ  ��  �� ",/* 3 Used with ApplVar.Plu ApplVar.Report */        
#define ReportType4     "  OFERTA        "        //" ��  ��  ��  Ϣ ",//4                                              
#define ReportType5     "  PAGO/CAJA "        //" �� �� �� �� Ϣ ",//5                                             
#define ReportType6     "  CAJON        "        //" Ǯ  ��  ��  Ϣ ",//6                                             
#define ReportType7     "  CORRECCION    "        //" ��  ��  ��  Ϣ ",//7                                                 
#define ReportType8     "  DSCTO      "        //" ��  ��  ��  Ϣ ",//8                                                    
#define ReportType9     "  MONEDA      "        //" ��  ��  ��  Ϣ ",//9                                                        
#define ReportType10    "  IMPUESTO    "        //" ��  ˰  ��  Ϣ ",//10                                                  
#define ReportType11    " FUNCIONES PB  "        //" ��  ��  ��  Ϣ ",//11                                                          
#define ReportType12    "PREVIO BALANCE"        //" ��  ̨  ��  Ϣ ",//12                                                         
#define ReportType13    "  MESA        "        //" ��  ̨  ��  Ϣ ",//13                                                  
#define ReportType14    "  INVENTARIO     "        //" ��  Ʒ  ��  �� ",//14                                                  
#define ReportType15    "  FACTURA PB   "   //                                                                                 
#define ReportType16    "  OFERTA BUFFER "    //                                                                                 
// Printer
#define ReportType17    "  TOTAL DE CAJERO  "  //"    �� �� Ա ͳ ��   ",                                                                   
#define ReportType18    "  TOTAL DE VENDEDOR "  //"    Ӫ ҵ Ա ͳ ��   ",                                                             
// Printer
#define Trailer1        "       GRACIAS       "        //"     ~��  ~л  ~��  ~��     ",                                     
#define Trailer2        "        ADIOS       "        //"          ~��  ~��         ",                              


#define ReportList1     "DIARIO CAJERO"        //" �� �� Ա �� �� ",                                                             
#define ReportList2     "PERIDO CAJERO"        //" �� �� Ա �� �� ",                                                             
#define ReportList3     "INFORME DIARIO"        //" ��  ��  ��  �� ",                                                           
#define ReportList4     "INFORME PERIODO"        //" ��  ��  ��  �� ",                                                                   
#define ReportList5     "DOC-ARTICULO"        //" ��  Ʒ  ��  �� ",                                                     
#define ReportList6     "DOC-CUENTA-MESA"        //" ��  ��  ��  �� ",                                                                 
#define ReportList7     "DOC-TIEMPO"        //" ʱ  ��  ��  �� ",                                                                    
#define ReportList8     "D-TODOCAJERO"        //"  ȫ�տ�Ա�ձ�  ",                                                       
#define ReportList9     "S-TODOCAJERO"        //"  ȫ�տ�Ա�±�  ",                                                      
#define ReportList10    "DOC/DIA-VENDEDOR"        //" Ӫ ҵ Ա �� �� "                                                                                                
#define ReportList11    "SEMANA VEND."        //" Ӫ ҵ Ա �� �� "                                                   
#define ReportList12    "DIA-TODOVEND."        //"  ȫӪҵԱ�ձ�  ",                                                  
#define ReportList13    "SEMANATODOVEN"        //"  ȫӪҵԱ�±�  ",                                                      

#define Promotion1      "  CONGRATULACIONES   "        //"  ~��  ~ϲ  ~��  ~��",                                        

#define Correc1         "CORRECCIO "        //"����    "//ccr2016-07-14                                                  
#define Correc2         "ANULACION    "        //"ȡ��    "//ccr2016-07-14                                           
#define Correc3         "DEVOLUCION  "        //"�˻�    "                                                            
#define Correc4         "CANCELADO  "        //"ȡ������"                                                                 
#define Correc5         "SUSPENDIDO "      //"����"                                                             


#define CURRENCY1         "DOLAR  "        //"��Ԫ    "L��ӡ/��ʾ                                                                 
#define CURRENCY2         "YEN     "        //"��Ԫ    "L��ӡ/��ʾ
#define CURRENCY3         "HK $    "        //"�۱�    "L��ӡ/��ʾ
#define CURRENCY4         "CURR   1"        //"���1   "L��ӡ/��ʾ
#define CURRENCY5         "CURR   2"        //"���2   "L��ӡ/��ʾ

#define DISCOUNT1          "+%"        //"+%�ӳ�"     L��ӡ/��ʾ
#define DISCOUNT2          "-%"        //"(-%)�ۿ�"   L��ӡ/��ʾ
#define DISCOUNT3          "+NET"        //"���ӳ�"    L��ӡ/��ʾ
#define DISCOUNT4          "-NET"        //"����ۿ�"    L��ӡ/��ʾ
//#define DISCOUNT5          "+% EN PLU"   // zy //"����ӳ�"                                                     
//ccr090812 for SubDisc>>>>>>>>>>>>>>>>>>>>>>>>>
#define DISCOUNT6          "SUBTOTAL+%"   //  "+%С�Ƽӳ�"L��ӡ/��ʾ
#define DISCOUNT7          "SUBTOTAL-%"   //  "-%С���ۿ�"L��ӡ/��ʾ
#define DISCOUNT8          "SUBTOTAL+"   // "С�ƽ��ӳ�"L��ӡ/��ʾ
#define DISCOUNT9          "SUBTOTAL-"   // "С�ƽ���ۿ�"L��ӡ/��ʾ
//jdb2018-03-20
#define DISCOUNT10		   "SC DSCTO"  //-%SCС���ۿ�                                               
#define DISCOUNT11		   "CLAVE DSCTO"  //-%PWDС���ۿ�                                                 
//jdb2018-04-03
#define DISCOUNT12		   "SC SUB DSCTO"                                                
#define DISCOUNT13		   "CLAVE SUB DSCTO"                                                       

#define DRAWER1            "EFECTIVO    "        //"�ֽ�    "L��ӡ/��ʾ                                         
#define DRAWER2            "CHEQUE   "        //"֧Ʊ    "L��ӡ/��ʾ                                               
#define DRAWER3            "TAR DE CRED"        //"���ÿ�  "L��ӡ/��ʾ                                                               
#define DRAWER4            "CUPON  "        //"����ȯ  "L��ӡ/��ʾ                                                                
#define DRAWER5            "CREDITO  "        //"����    "L��ӡ/��ʾ                                                                
#define DRAWER6            "TAR DE CHIP"        //"IC��    "L��ӡ/��ʾ                                                               
#if  defined(CASE_INDONESIA)
#define DRAWER7            "DEBITO   "        //"С��    "L��ӡ/��ʾ                                                                
#else
#define DRAWER7            "CUOTA     "        //"С��    "L��ӡ/��ʾ                                
#endif
#define DRAWER8            "CAJON  "//��ӡ/��ʾ

#define PBFunction0         "ABRIR    "        //   "��̨����"                     
#define PBFunction1         "ANDIR     "        //   "��̨����"                                 
#define PBFunction2         "SERVICIO "        //   "�ݽ�"                                                                         
#define PBFunction3         "FINALIZAR"        //   "ȷ�Ͻ���"                                                                     
#define PBFunction4         "IMPRESION PB"        //   "��ӡ��̨"                                                                     
#define PBFunction5         "IM DE CUENTA"        //   "��ӡ�ʵ�"                                      
#define PBFunction6         "CANCELADO "        //   "ȡ��ȷ��"                                                                       
#define PBFunction7         "DIVISION"        //   "����"                                                                                                                                             
#define PBFunction8         "TRANSFERENCIA"        //   "ת��"                                                                          
#define PBFunction9         "MOVER A"        //   "ת����"                                                                                                                                                 
#define PBFunction10        "CUBRIR "        //   "����"                                           


#define PORAType1            "PAGOS   "        //"����    "                                                                          
#define PORAType2            "CAJA CHICA "        //"���    "                                                                          
#define PORAType3            "RETORNO"        //"IC���˿�"                               
#define PORAType4            "CARGA"        //"IC����ֵ"                                       
#define PORAType5            "DEPOSITO"        //"��ICѺ��"                                            
#define PORAType6            "PRE REGISTRO"        //"��ICѺ��"                                       

#define TendType1             " EFECTIVO   "        //"�ֽ�    "                                                                     
#define TendType2             "CAJERO   "        //"֧Ʊ    "                                                                      
#define TendType3             "TAR DE CREDI"        //"���ÿ�  "                                                                     
#define TendType4             "CUPON "        //"����ȯ  "                                                                     
#define TendType5             "CRDITO  "        //"����"                                                                          
#define TendType6             "TAR DE CHIP"        //"ICCard"                                                                        
#define TendType7             "TAR DEDEBITO"        //"DEBIT CARD"                                                                   
#define TendType8             "EFECTIVO    "        //"CASH-8"                                                                        


#define OperationN3             "TOTAL"  // "�ܼ�",
#define OperationN4             "VUELTO"  // "ת��",                                                                                
#define OperationN5             "SUBTOTAL"  // "С��",
#define OperationN6             "CAJA - �"  // "����",
#define OperationN7             "CAJA + �"  // "���",
#define OperationN8             "RESUMEN"  // "����",
#define OperationN9             "ANADIR"  // "����",
#define OperationN10            "MANTENER"  // "�ҵ�,                                                                                   
#define OperationN11            "ANULAR"  // "ȡ��",                                                                                  

#define OperationN14            "PRECIO"  // "����:",     //                                                               
#define OperationN15            "CANTIDAD"  // "���:",//                                               

#define CaptionN2               "IMPORTE NETO"  // "����",                                                            
#define CaptionN4               "TOTAL TRADE"  // "������",                                                                                                                                                  
#define CaptionN5               "CANTIDAD"  // "����",                                                                              
#define CaptionN6               "IMPORTE"  // "���",                                                                                
#define CaptionN7               "ENTREGADO"  // "��ȡ",                                                                               
#define CaptionN9               " DIA "  // "��",
#define CaptionN10              " SEMANA "  // "��",
#define CaptionN11              "MES"  // "��",
#define CaptionN12              "ANO"  // "��",
#define CaptionN13              "ESTANDAR"  // "��׼",                                                                            
#define CaptionN14              "CAJERO"  // "�տ�Ա",                                                                             
#define CaptionN15              "   HORA"  // "ʱ",                                                                               
#define CaptionN16              " DIA "  // "   ��",
#define CaptionN17              " MES "  // "   ��",
#define CaptionN18              "DEVOLUCION"  // "�˻�",                                                                             
#define CaptionN19              "COSTO"  // "�ɱ�",                                                                                
#define CaptionN20              "MESA# "  // "����#",                                                                           
#define CaptionN21              "CHEQUE NO"  // "֧Ʊ��#",
#define CaptionN22              "ARTICULO"  // "����:",                                                                                                                                                       
#define CaptionN23              "IMPONIBLE"  // "��˰����",                                                          
#define CaptionN24              "IMPUESTO "  // "˰��",                                                                             
#define CaptionN25              "PAGINA"  //  "ҳ��",                                                                                                                                                             
#define CaptionN26              "  NO ANDIR #"  // "����:",                                                                                                                                                  
#define CaptionN27              "SERVICIO"  // "����",                                                                         
#define CaptionN28              "ABRIR MESA"  // "��̨",
#define CaptionN29              "CONTADOR DE Z"  // "���ʼ���",                                                                     
#define CaptionN30              "BORRAR"  // "���",                                                                            
#define CaptionN31              "INFORME -S"  // "S-����",ʲô��S-REPORT? ZWQ                                                    
#if HOR_REPORT==1
#define CaptionN32              "BRUTO"  // "ë��",                                                                  
#else
#define CaptionN32              "BENEFICIO BRUTO"  // "ë��",
#endif
#define CaptionN33              "INCREMENTAR STOCK"  // "�������",//                                                         
#define CaptionN331             "DISMINUIR STOCK"  // "�������",//                                                         
#define CaptionN34              "CUENTA-MESA ANTERIOR"  // "ԭ����:",                                                                 
#define CaptionN35              "T.CREDITO"  // "���ÿ�#",
#define CaptionN36              "NUEVA MESA"  // "������:",                                                                   
#define CaptionN37              "CLIENTE NO"  // "����",
#define CaptionN38              "CAMBIA ARTICULO"  // "����",                                                                      

//Dispaly
#define CaptionN41              "DSCTO 1"  // "�ۿ���1",
#define CaptionN42              "DSCTO 2"  // "�ۿ���2",
#define CaptionN43              "IMPORTE DE VENDEDOR"  // "���۶�",                                                                 
#define CaptionN44              "DTO MASIVO"  // "����Ż�",
#define CaptionN45              "STOCK CANT"  // "�������",
#define CaptionN46              "OFERTA"  // "�ؼ��Ż�",
#define CaptionN51              "MESA TEMPORALFIN"  // "��̨�ݽ�",
#define CaptionN52              "+MESA"  // "��̨����",
#define CaptionN53              "CERRA TEMPORAL"  // "�ݽ�",
#define CaptionN54              "CONFIRMAPAGO"  // "ȷ�Ͻ���",
#define CaptionN55              "IMPRIME MESA"  // "��ӡ��̨",
#define CaptionN56              "IMPRIMCUENTA"  // "��ӡ�ʵ�",
#define CaptionN57              "CANCELA CONFIRMA"  // "ȡ��ȷ��",
#define CaptionN58              "DIVIDECUENTA"  // "����",
#define CaptionN59              "TRANS-CUENTA"  // "ת��",
#define CaptionN60              "ALTERAR OPER"  // "�����տ�Ա",��ϢΪ"MOV"
#define CaptionN61              "CLIENTE NO"  // "�˿�����",


#define CaptionE11              "CAJERO:"  // "�տ�Ա:",
#define CaptionE12              "VENDEDOR:"  // "ӪҵԱ:",                                                                         
#define CaptionE13              "CAJA NO:"  // "�տ����:",
#define CaptionE14              "TIENDA:"  // "λ��:",                                                                                                                                              

#define CaptionE17              "VERSION ACTUAL:"  // "���а汾:",
#define CaptionE18              "PUNTOS"  // "�������ѵ�",                                                   
#define CaptionE19              " ANDIR PLU"  // "������Ʒ:",                                                                  
#define CaptionE20              "BORRAR PLU"  // "ɾ����Ʒ:",                                                                
#define CaptionE21              "COMENZAR TEST>>"  // "��ʼ���>>",
#define CaptionE22              "<<YA TEST"  // "<<������",
#define CaptionE23              "INVENTARIO"  // "���",
#define CaptionE24              "BORRADO DE INFORME!"  // "������ȫ��!",                                           


#define CaptionE31              "NOTA"  // "˵������",


#define MessagesN5                "*** MODO ENTRENAMIENTO ***"  // "*** ��ѵ״̬ ***",                                       
#define MessagesN6                "***** ENCENDIDO*****"  // "***** �ӵ� *****",                                            
#define MessagesN7                "*** MESA DIVIDIDA ***"  // "*** �ֿ����� ***",                            
#define MessagesN8                "** COPIA DE RECIBO***"  // "*** �����վ� ***",                                     
#define MessagesN9                "****PONER PRECIO PLU ****"  // "*** ��Ʒ��� ***",//                                 
#define MessagesN10               "**INVENTARIO+ **"  // "*** ���ӿ�� ***",
#define MessagesN101              "**INVENTARIO- **"  // "*** ���ٿ�� ***",
#define MessagesN11               "*****CANCELAR *****"  // "***** ȡ�� *****",
#define MessagesN12               "**  INICIAR PROGRAMA **"  // "** �����ʼ�� **",

#define EditN24                   " ARCHIVO    GRABAR    RAM"  // "�ļ���      ��¼��      �ռ��С"         
#define EditN25                   " PUERTO     TIPO  PROTOCOLO  "  // "��������    ͨ�����   ͨ��Э��"   

//PRINT                          /*123456789012345678901234567890123456*/
#if PRTLEN>40
#define TitleN1                   "NOMBRE  CANTIDAD.   PVP. IMPORTE"
#define TitleN3	                  "NOMBRE      CANTIDAD.  IMPORTE"  //--Ʒ��   ����   ����   ���
#define TitleN4	                  "NOMBRE                 IMPORTE"  //

#define DEPTREPORTTITLE           "DEPT      CANTIDAD      IMPORTE"
#define PLUREPORTTITLE            "NOMBRE      CANTIDAD   IMPORTE"//

#else

#define TitleN1	                  "NOMBRE  CANTIDAD.   PVP. IMPORTE"  //--Ʒ��   ����   ����   ���
#define TitleN3	                  "NOMBRE      CANTIDAD.  IMPORTE"  //--Ʒ��   ����   ����   ���
#define TitleN4	                  "NOMBRE                      IMPORTE "  //
#define DEPTREPORTTITLE         "DEPT      CANTIDAD      IMPORTE"
#define PLUREPORTTITLE          "NOMBRE      CANTIDAD   IMPORTE"//

#endif

#define OptionS37                 "LLAMADA"    //mPRNINDEX//Ccr "ȡ�˺�:",                                         


#define EditN6                    "CONSUMIDO"  // "���ѽ��",                                                                                                                     
#define ChipCardN46               " # DE TARJETA"  // "��Ա��:",                                                                 
#define ChipCardN47               "AFILIACION"  // �� Ա �� ��                                                                


#define PLULIST                  "        LISTA PLU"  // "         �� Ʒ �� Ϣ ��"                                   
#define PLUTITLE1                "NOBRE-ARTICULO          REF"  //
#define PLUTITLE2                "   DEPT EN STOCK PVP"//

#define DEPTLIST               "         INFO-DEPT"  // "         �� Ʒ �� Ϣ ��"
#define DEPTTITLE                "NO-DEPT  GRUPO  PVP"        //



#define FisTextN18                "   IMPRIME IMPUESTO NO    "   //"    _�� _˰ _ӡ _��     "
#define FisTextN19                "    TERMINADO PROBAR MUESTRA  "   //"    _�� _װ _�� _��     "
#define FisTextN20                "    _NOT   _FISCAL    "   //"    _��   _��   _˰     "


#define PRTStyleN1                       "NORMAL IMPRIME"   //Normale
#define PRTStyleN2                       "DblHigh"   //Doppia altezza
#define PRTStyleN3                       "D~b~l~W~t~h"   //Raddoppio caratteri
#define PRTStyleN4                       "D~b~l~H~i~g~h"   //Dopia altezza + Raddoppio caratteri

#define ComTestN1                        "PORT#1 ERROR"   //"���� 1 ���Գ���"
#define ComTestN2                        "PORT#1 OK"   //"���� 1 ������ȷ"
#define ComTestN3                        "PORT#2 ERROR"    //"���� 2 ���Գ���"
#define ComTestN4                        "PORT#2 OK"   //"���� 2 ������ȷ"

#define ComTestN3ERR                      "PORT#3 ERROR"    //"���� 3 ���Գ���"
#define ComTestN3OK                      "PORT#3 OK"   //"���� 3 ������ȷ"


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
#define Msg_RECNUMFR	"EJ# DE"                  //��ʼ�վݺ�	//RECNUMFR
#define Msg_RECNUMTO	"EJ# PARA"                    //�����վݺ�	//RECNUMTO
#define Msg_EJBBIAO		"~E~J ~I~N~F~O~R~M~E"		//EJ ����			//EJBBIAO
#define Msg_EJXINXI		"MENSAJES ACTUALES DE EJ"            //EJ��Ϣ			//EJXINXI
#define Msg_NEWEJXINXI	"EJ DESPU?S INICIALIZADO"            //EJ��Ϣ			//NEWEJXINXI

#define Msg_EJBHAO		"EJ ID"						//EJ���			//EJBHAO
#define Msg_EJLEFT    "EJ IZQUIERDO"        //ʣ��ռ�
#define Msg_SIZEEJ		"SIZE OF EJ"				//SIZEEJ
#define Msg_EJCSHUA		"EJ INICIO"		//EJ��ʼ��		//EJCSHUA

#define Msg_PRNEJURNAL  ">>>>IMPRESION DE TESTIGO DIARIO<<<<"
#define Msg_ENTERSETUP  ">>>>>IMPRIMIR EN CONFIGURACION<<<<<"
#define Msg_EJDATEEND   "FECHA ULTIMA EJ"
#define Msg_EJRECORDS   "ULTIMO EJ#"

#define Msg_EJINITIAL   "INICIALIZACION EJ"
#endif

#define Msg_MANAGEMENT "LISTADO DE ADMINISTRADOR "
#endif

