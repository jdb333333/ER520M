/******************** (C) COPYRIGHT 2012 *****************************************
 * 文件名  ：LWIP_Init.c
 * 描述    ：LWIP协议栈初始化，初始化网卡
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

struct netif enc28j60;          //netif 网络接口结构，用于描述网络硬件接口的特性
uint8_t macaddress[6] = {'E','U', 0x21, 0x11, 0x72, 0x73};//ccr2015-01-29设置网络接口的mac地址
extern __IO uint32_t LocalTime;

/**外部函数*/
extern err_t ethernetif_init( struct netif *netif );  //在ethernetif.c中定义
extern err_t ethernetif_input( struct netif *netif );  //在ethernetif.c中定义

/*
 * 函数名：LWIP_Init
 * 描述  ：初始化LWIP协议栈，主要是把enc28j60与LWIP连接起来。
            包括IP、MAC地址，接口函数
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void LwIP_Init( uint8_t IP1,uint8_t IP2,uint8_t IP3,uint8_t IP4)
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

    /*调用LWIP初始化函数，
    初始化网络接口结构体链表、内存池、pbuf结构体*/
    lwip_init();

#if LWIP_DHCP                               //若使用DHCP协议
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;
#else                                       //
    IP4_ADDR(&ipaddr, IP1, IP2, IP3, IP4);       //设置网络接口的ip地址
    IP4_ADDR(&netmask, 255, 255, 255, 0);     //子网掩码
    IP4_ADDR(&gw, IP1, IP2, 1, 1);            //网关
    macaddress[2]=IP1;      //ccr2015-01-29>>>>>
    macaddress[3]=IP2;
    macaddress[4]=IP3;
    macaddress[5]=IP4;      //ccr2015-01-29<<<<
#endif

    /*初始化enc28j60与LWIP的接口，参数为网络接口结构体、ip地址、
    子网掩码、网关、网卡信息指针、初始化函数、输入函数*/
    netif_add(&enc28j60, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

    /*把enc28j60设置为默认网卡 .*/
    netif_set_default(&enc28j60);


#if LWIP_DHCP               //若使用了DHCP
    /*  Creates a new DHCP client for this interface on the first call.
    Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
    the predefined regular intervals after starting the client.
    You can peek in the netif->dhcp struct for the actual DHCP status.*/
    dhcp_start(&enc28j60);    //启动DHCP
#endif

    /*  When the netif is fully configured this function must be called.*/
    netif_set_up(&enc28j60); //使能enc28j60接口
}

/*
 * 函数名：Time_Update
 * 描述  ：用于更新LocalTime的值
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
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
 * 在对网络各种设置和初始化完成后,必须调用
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
 * 描述  ：lwip协议栈要求周期调用一些函数
 *         tcp_tmr  etharp_tmr   dhcp_fine_tmr dhcp_coarse_tmr
 *
 * @param localtime 系统当前时钟(毫秒)
 * @param fromINT :=1,标示是在ENC28J60的中断服务程序中调用;
 *                 =0,为其它查询方式调用
 *
 * 函数void ethernetif_input（struct netif*netif）从网络接口接收以太网数据包
 * 并把其中的IP报文向IP层发送。网卡对于以太网上目的地为该网卡的包会自动启动本
 * 地DMA接收数据，并存放在RTL8019AS芯片内部RAM的接收缓冲环中，然后以中断的方式
 * 通知CPU。此时该函数使用远端DMA接收数据到系统的RAM当中。
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
                    err = ethernetif_input(&enc28j60); //轮询是否接收到数据
                //} while (enc28j60Read(EPKTCNT)>0 && err==ERR_OK);//ccr2015-01-28

            }

            /* TCP periodic process every 250 ms */
            if (localtime - TCPTimer >= TCP_TMR_INTERVAL)
            {
                TCPTimer =  localtime;
                tcp_tmr();       //每250ms调用一次
            }
            /* ARP periodic process every 5s */
            if (localtime - ARPTimer >= ARP_TMR_INTERVAL)
            {
                ARPTimer =  localtime;
                etharp_tmr();     //每5s调用一次
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
