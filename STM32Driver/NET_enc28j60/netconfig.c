/******************** (C) COPYRIGHT 2012 *****************************************
 * �ļ���  ��LWIP_Init.c
 * ����    ��LWIPЭ��ջ��ʼ������ʼ������
**********************************************************************************/

#include "netconfig.h"
#include "enc28j60.h"

#define SYSTEMTICK_PERIOD_MS  10
#define INPUT_TMR_INTERVAL    2

__IO uint8_t ETHERNET_Ready = 0;//ccr2015-01-22

__IO uint32_t TCPTimer = 0;
__IO uint32_t ARPTimer = 0;
__IO uint32_t INPUT_Timer = 0;

#ifdef LWIP_DHCP
__IO uint32_t DHCPfineTimer = 0;
__IO uint32_t DHCPcoarseTimer = 0;

#endif

struct netif enc28j60;          //netif ����ӿڽṹ��������������Ӳ���ӿڵ�����
uint8_t macaddress[6] = {'E','U', 0x21, 0x11, 0x72, 0x73};//ccr2015-01-29��������ӿڵ�mac��ַ
extern __IO uint32_t LocalTime;

/**�ⲿ����*/
extern err_t ethernetif_init( struct netif *netif );  //��ethernetif.c�ж���
extern err_t ethernetif_input( struct netif *netif );  //��ethernetif.c�ж���

/*
 * ��������LWIP_Init
 * ����  ����ʼ��LWIPЭ��ջ����Ҫ�ǰ�enc28j60��LWIP����������
            ����IP��MAC��ַ���ӿں���
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void LwIP_Init( uint8_t IP1,uint8_t IP2,uint8_t IP3,uint8_t IP4)
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

    /*����LWIP��ʼ��������
    ��ʼ������ӿڽṹ�������ڴ�ء�pbuf�ṹ��*/
    lwip_init();

#if LWIP_DHCP                               //��ʹ��DHCPЭ��
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;
#else                                       //
    IP4_ADDR(&ipaddr, IP1, IP2, IP3, IP4);       //��������ӿڵ�ip��ַ
    IP4_ADDR(&netmask, 255, 255, 255, 0);     //��������
    IP4_ADDR(&gw, IP1, IP2, 1, 1);            //����
    macaddress[2]=IP1;      //ccr2015-01-29>>>>>
    macaddress[3]=IP2;
    macaddress[4]=IP3;
    macaddress[5]=IP4;      //ccr2015-01-29<<<<
#endif

    /*��ʼ��enc28j60��LWIP�Ľӿڣ�����Ϊ����ӿڽṹ�塢ip��ַ��
    �������롢���ء�������Ϣָ�롢��ʼ�����������뺯��*/
    netif_add(&enc28j60, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

    /*��enc28j60����ΪĬ������ .*/
    netif_set_default(&enc28j60);


#if LWIP_DHCP               //��ʹ����DHCP
    /*  Creates a new DHCP client for this interface on the first call.
    Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
    the predefined regular intervals after starting the client.
    You can peek in the netif->dhcp struct for the actual DHCP status.*/
    dhcp_start(&enc28j60);    //����DHCP
#endif

    /*  When the netif is fully configured this function must be called.*/
    netif_set_up(&enc28j60); //ʹ��enc28j60�ӿ�
}

/*
 * ��������Time_Update
 * ����  �����ڸ���LocalTime��ֵ
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
#if (0)//ccr2015-01-20
void Time_Update(void)
{
    LocalTime += SYSTEMTICK_PERIOD_MS;
}
#endif
/**
 *
 * @author EutronSoftware (2015-01-29)
 *
 * �ڶ�����������úͳ�ʼ����ɺ�,�������
 */
void Set_ETHERNET_Ready()
{
    ETHERNET_Ready=1;
}

uint8_t Get_ETHERNET_Ready()
{
    return (ETHERNET_Ready & 1);
}


/**
 *
 * @author EutronSoftware (2015-01-29)
 * ����  ��lwipЭ��ջҪ�����ڵ���һЩ����
 *         tcp_tmr  etharp_tmr   dhcp_fine_tmr dhcp_coarse_tmr
 *
 * @param localtime ϵͳ��ǰʱ��(����)
 * @param fromINT :=1,��ʾ����ENC28J60���жϷ�������е���;
 *                 =0,Ϊ������ѯ��ʽ����
 *
 * ����void ethernetif_input��struct netif*netif��������ӿڽ�����̫�����ݰ�
 * �������е�IP������IP�㷢�͡�����������̫����Ŀ�ĵ�Ϊ�������İ����Զ�������
 * ��DMA�������ݣ��������RTL8019ASоƬ�ڲ�RAM�Ľ��ջ��廷�У�Ȼ�����жϵķ�ʽ
 * ֪ͨCPU����ʱ�ú���ʹ��Զ��DMA�������ݵ�ϵͳ��RAM���С�
 *
 */
void LwIP_Periodic_Handle(__IO uint32_t localtime,uint8_t fromINT)
{
    static u8_t critical=0;
    err_t  err;

    if (ETHERNET_Ready)//ccr2015-01-28
    {
        if (fromINT)
        {
            enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, EIE, EIE_INTIE);//ccr2015-01-27
        }
        if (!critical)//ccr2015-01-28
        {
            critical = 1;
            //ccr2015-01-26 if (localtime - INPUT_Timer >= INPUT_TMR_INTERVAL)
            {
                INPUT_Timer = localtime;

				/* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
                //do{//ccr2015-01-28 must
                    err = ethernetif_input(&enc28j60); //��ѯ�Ƿ���յ�����
                //} while (enc28j60Read(EPKTCNT)>0 && err==ERR_OK);//ccr2015-01-28

            }

            /* TCP periodic process every 250 ms */
            if (localtime - TCPTimer >= TCP_TMR_INTERVAL)
            {
                TCPTimer =  localtime;
                tcp_tmr();       //ÿ250ms����һ��
            }
            /* ARP periodic process every 5s */
            if (localtime - ARPTimer >= ARP_TMR_INTERVAL)
            {
                ARPTimer =  localtime;
                etharp_tmr();     //ÿ5s����һ��
            }

        #if LWIP_DHCP
            /* Fine DHCP periodic process every 500ms */
            if (localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
            {
                DHCPfineTimer =  localtime;
                dhcp_fine_tmr();
            }
            /* DHCP Coarse periodic process every 60s */
            if (localtime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
            {
                DHCPcoarseTimer =  localtime;
                dhcp_coarse_tmr();
            }
        #endif
        if (fromINT)
        {
            enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE);//ccr2015-01-27
        }

            critical = 0;
        }
    }
}
