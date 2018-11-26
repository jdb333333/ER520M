/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of and a contribution to the lwIP TCP/IP stack.
 *
 * Credits go to Adam Dunkels (and the current maintainers) of this software.
 *
 * Christiaan Simons rewrote this file to get a more stable echo example.
 *
 **/

/* This file was modified by ST */


#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "tcp_echoserver.h"

char dispbuf[64];//ccr2015-01-23 testonly

#if LWIP_TCP

static struct tcp_pcb *tcp_echoserver_pcb;

/* ECHO protocol states */
enum tcp_echoserver_states
{
    ES_NONE = 0,
    ES_ACCEPTED,
    ES_RECEIVED,
    ES_CLOSING
};

/* structure for maintaing connection infos to be passed as argument
   to LwIP callbacks*/
struct tcp_echoserver_struct
{
    u8_t state;             /* current connection state */
    struct tcp_pcb *pcb;    /* pointer on the current tcp_pcb */
    struct pbuf *p;         /* pointer on the received/to be transmitted pbuf */
};


static err_t tcp_echoserver_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_echoserver_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_echoserver_error(void *arg, err_t err);
static err_t tcp_echoserver_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_echoserver_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static err_t tcp_echoserver_send(struct tcp_pcb *tpcb, struct tcp_echoserver_struct *es);
static void tcp_echoserver_connection_close(struct tcp_pcb *tpcb, struct tcp_echoserver_struct *es);

static err_t TCPDecodeRecord(struct tcp_pcb *tpcb,u8_t *data,u8_t size);


/**
  * @brief  Initializes the tcp echo server
  * @param  None
  * @retval None
  */
void tcp_echoserver_init(void)
{
    /* create new tcp pcb */
    tcp_echoserver_pcb = tcp_new();

    if (tcp_echoserver_pcb != NULL)
    {
        err_t err;

        /* bind echo_pcb to port 7 (ECHO protocol) */
        err = tcp_bind(tcp_echoserver_pcb, IP_ADDR_ANY, TCPIP_PORT);

        if (err == ERR_OK)
        {
            /* start tcp listening for echo_pcb */
            tcp_echoserver_pcb = tcp_listen(tcp_echoserver_pcb);

            /* initialize LwIP tcp_accept callback function */
            tcp_accept(tcp_echoserver_pcb, tcp_echoserver_accept);
        } else
        {
            LWIP_PLATFORM_ASSERT("Can not bind pcb\n");
        }
    } else
    {
        LWIP_PLATFORM_ASSERT("Can not create new pcb\n");
    }
}

/**
  * @brief  This function is the implementation of tcp_accept LwIP callback
  * @param  arg: not used
  * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp connection
  * @param  err: not used
  * @retval err_t: error status
  */
static err_t tcp_echoserver_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    err_t ret_err;
    struct tcp_echoserver_struct *es;

    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);

    /* set priority for the newly accepted tcp connection newpcb */
    tcp_setprio(newpcb, TCP_PRIO_MIN);

    /* allocate structure es to maintain tcp connection informations */
    es = (struct tcp_echoserver_struct *)mem_malloc(sizeof(struct tcp_echoserver_struct));
    if (es != NULL)
    {
        es->state = ES_ACCEPTED;
        es->pcb = newpcb;
        es->p = NULL;

        /* pass newly allocated es structure as argument to newpcb */
        tcp_arg(newpcb, es);

        /* initialize lwip tcp_recv callback function for newpcb  */
        tcp_recv(newpcb, tcp_echoserver_recv);

        /* initialize lwip tcp_err callback function for newpcb  */
        tcp_err(newpcb, tcp_echoserver_error);

        /* initialize lwip tcp_poll callback function for newpcb */
        tcp_poll(newpcb, tcp_echoserver_poll, 1);

        ret_err = ERR_OK;
    } else
    {
        /* return memory error */
        ret_err = ERR_MEM;
    }
    return ret_err;
}


/**
  * @brief  This function is the implementation for tcp_recv LwIP callback
  * @param  arg: pointer on a argument for the tcp_pcb connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  pbuf: pointer on the received pbuf
  * @param  err: error information regarding the reveived pbuf
  * @retval err_t: error code
  */
static err_t tcp_echoserver_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    struct tcp_echoserver_struct *es;
    err_t ret_err;
    static int Packages=0;//ccr2015-01-23 testonly


    LWIP_ASSERT("arg != NULL",arg != NULL);

    es = (struct tcp_echoserver_struct *)arg;

    /* if we receive an empty tcp frame from client => close connection */
    if (p == NULL)
    {
        /* remote host closed connection */
        es->state = ES_CLOSING;
        if (es->p == NULL)
        {
            /* we're done sending, close connection */
            tcp_echoserver_connection_close(tpcb, es);
        } else
        {
            /* we're not done yet */
            /* acknowledge received packet */
            tcp_sent(tpcb, tcp_echoserver_sent);

            /* send remaining data*/
            tcp_echoserver_send(tpcb, es);
        }
        ret_err = ERR_OK;
    }
    /* else : a non empty frame was received from client but for some reason err != ERR_OK */
    else if (err != ERR_OK)
    {
        /* free received pbuf*/
        if (p != NULL)
        {
            es->p = NULL;
            pbuf_free(p);
        }
        ret_err = err;
    } else if (es->state == ES_ACCEPTED)
    {
        /* first data chunk in p->payload */
        es->state = ES_RECEIVED;

        /* store reference to incoming pbuf (chain) */
        es->p = p;

        /* initialize LwIP tcp_sent callback function */
        tcp_sent(tpcb, tcp_echoserver_sent);

        /* send back the received data (echo) */
        if (tcp_echoserver_send(tpcb, es)==ERR_OK)
        {
            Packages++;//ccr2015-01-23 testonly
            sprintf(dispbuf,"Packages>%7d",Packages);//ccr2015-01-23 testonly
            DispStrXY(dispbuf,0,1);//ccr2015-01-23 testonly
        }

        ret_err = ERR_OK;
    } else if (es->state == ES_RECEIVED)
    {
        /* more data received from client and previous data has been already sent*/
        if (es->p == NULL)
        {
            es->p = p;

            /* send back received data */
            if (tcp_echoserver_send(tpcb, es)==ERR_OK)
            {
                Packages++;//ccr2015-01-23 testonly
                sprintf(dispbuf,"Packages>%7d",Packages);//ccr2015-01-23 testonly
                DispStrXY(dispbuf,0,1);//ccr2015-01-23 testonly
            }
        } else
        {
            struct pbuf *ptr;

            /* chain pbufs to the end of what we recv'ed previously  */
            ptr = es->p;
            pbuf_chain(ptr,p);

            sprintf(dispbuf,"Packages+%7d",Packages);//ccr2015-01-23 testonly
            DispStrXY(dispbuf,0,1);//ccr2015-01-23 testonly
        }
        ret_err = ERR_OK;
    }

    /* data received when connection already closed */
    else
    {
        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);

        /* free pbuf and do nothing */
        es->p = NULL;
        pbuf_free(p);
        ret_err = ERR_OK;
    }
    return ret_err;
}

/**
  * @brief  This function implements the tcp_err callback function (called
  *         when a fatal tcp_connection error occurs.
  * @param  arg: pointer on argument parameter
  * @param  err: not used
  * @retval None
  */
static void tcp_echoserver_error(void *arg, err_t err)
{
    struct tcp_echoserver_struct *es;

    LWIP_UNUSED_ARG(err);

    es = (struct tcp_echoserver_struct *)arg;
    if (es != NULL)
    {
        /*  free es structure */
        mem_free(es);
    }
}

/**
  * @brief  This function implements the tcp_poll LwIP callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
  * @retval err_t: error code
  */
static err_t tcp_echoserver_poll(void *arg, struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct tcp_echoserver_struct *es;

    es = (struct tcp_echoserver_struct *)arg;
    if (es != NULL)
    {
        if (es->p != NULL)
        {
            /* there is a remaining pbuf (chain) , try to send data */
            tcp_echoserver_send(tpcb, es);
        } else
        {
            /* no remaining pbuf (chain)  */
            if (es->state == ES_CLOSING)
            {
                /*  close tcp connection */
                tcp_echoserver_connection_close(tpcb, es);
            }
        }
        ret_err = ERR_OK;
    } else
    {
        /* nothing to be done */
        tcp_abort(tpcb);
        ret_err = ERR_ABRT;
    }
    return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data)
  * @param  None
  * @retval None
  */
static err_t tcp_echoserver_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    struct tcp_echoserver_struct *es;

    LWIP_UNUSED_ARG(len);

    es = (struct tcp_echoserver_struct *)arg;

    if (es->p != NULL)
    {
        /* still got pbufs to send */
        tcp_echoserver_send(tpcb, es);
    } else
    {
        /* if no more data to send and client closed connection*/
        if (es->state == ES_CLOSING)
            tcp_echoserver_connection_close(tpcb, es);
    }
    return ERR_OK;
}


/**
  * @brief  This function is used to send data for tcp connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  es: pointer on echo_state structure
  * @retval None
  */
static err_t tcp_echoserver_send(struct tcp_pcb *tpcb, struct tcp_echoserver_struct *es)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) &&
           (es->p != NULL) &&
           (es->p->len <= tcp_sndbuf(tpcb)))
    {

        /* get pointer on pbuf from es structure */
        ptr = es->p;

        /* enqueue data for transmission */
        if (es->state == ES_RECEIVED)
            wr_err = TCPDecodeRecord(tpcb, ptr->payload, ptr->len);
        else
            wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
        if (wr_err == ERR_OK)
        {
            u16_t plen;

            plen = ptr->len;

            /* continue with next pbuf in chain (if any) */
            es->p = ptr->next;

            if (es->p != NULL)
            {
                /* increment reference count for es->p */
                pbuf_ref(es->p);
            }

            /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
            pbuf_free(ptr);

            /* Update tcp window size to be advertized : should be called when received
            data (with the amount plen) has been processed by the application layer */
            tcp_recved(tpcb, plen);
        } else if (wr_err == ERR_MEM)
        {
            /* we are low on memory, try later / harder, defer to poll */
            es->p = ptr;
            DispStrXY("ERR_MEM",10,2);//ccr2015-01-23 testonly
        } else
        {
            /* other problem ?? */
            DispStrXY("Other ERR",0,2);//ccr2015-01-23 testonly
        }
    }
    return wr_err;
}

/**
  * @brief  This functions closes the tcp connection
  * @param  tcp_pcb: pointer on the tcp connection
  * @param  es: pointer on echo_state structure
  * @retval None
  */
static void tcp_echoserver_connection_close(struct tcp_pcb *tpcb, struct tcp_echoserver_struct *es)
{

    /* remove all callbacks */
    tcp_arg(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);

    /* delete es structure */
    if (es != NULL)
    {
        mem_free(es);
    }

    /* close tcp connection */
    tcp_close(tpcb);
}


//---------------------------------------------------------------------------
    #define WAITFORTYPE 0//等待数据类型:'F'-为流水,'U'为请求
    #define WAITFORSIZE 1
    #define WAITFORDATA 2
/**
 *  @author EutronSoftware (2014-12-06)
 * 从收到的数据中解析一条ECR文件数据
 * 解析结果存入EcrRecord指定的存储区
 * EcrRecord[0]+EcrRecord[1]:存放功能码
 * EcrRecord+2:为对应功能码的数据
 *
 * @param
 *        data:收到的数据
 *        size:返回收到的数据长度
 *
 * @return
 *         WORD:=0xffff时,无完整数据;=0时,数据结束;其它值:为收到的数据对应的功能码
 */

static err_t TCPDecodeRecord(struct tcp_pcb *tpcb,u8_t *data,u8_t size)
{
    static int PackageReceived=0;
    //Variable for TCPGetECRARecord
    u8_t EcrRecord[128];
    u8_t TCP_Command;//收到的数据类型;'F','U','D'....
    u8_t TCP_SizeMust;//必须收取的数据数目
    int TCP_BytesReceived=0;//记录已经收到的数据数目
    u8_t TCP_WaitFor=WAITFORTYPE;//=0,表示等待数据类型;=1标示等待长度字节;=2表示等待数据
    int i;

    for (i=0;i<size && TCP_BytesReceived<sizeof(EcrRecord)-2;i++)
    {
        switch (TCP_WaitFor)
        {
        case WAITFORTYPE://等待数据类型
            if (*data=='U' || *data=='D' || *data=='F')//为传送流水和请求文件命令
            {
                TCP_Command = *data;
                TCP_WaitFor = WAITFORSIZE;
                TCP_BytesReceived = 0;
                EcrRecord[TCP_BytesReceived++]=TCP_Command;
            }
            break;
        case WAITFORSIZE://等待数据长度,最长256字节
            //if (*data<=sizeof(union ECRFILEREC))
            {//为文件数据
                TCP_SizeMust = *data;
                TCP_WaitFor = WAITFORDATA;
                EcrRecord[TCP_BytesReceived++]=TCP_SizeMust;
            }
            //else//非文件数据
            //     TCP_WaitFor = WAITFORTYPE;
            break;
        case WAITFORDATA://等待收取数据
            EcrRecord[TCP_BytesReceived++]=*data;
            if (TCP_BytesReceived==TCP_SizeMust+2)
            {//收取到一个完整的数据包
                PackageReceived++;
                sprintf(dispbuf,"Recordes<%7d",PackageReceived);//ccr2015-01-23 testonly
                DispStrXY(dispbuf,0,0);//ccr2015-01-23 testonly
                //EcrRecord[TCP_BytesReceived++]=0x0d;//ccr2015-01-23 testonly
                //EcrRecord[TCP_BytesReceived++]=0x0a;//ccr2015-01-23 testonly
                TCP_WaitFor = WAITFORTYPE;
                return tcp_write(tpcb, EcrRecord, TCP_BytesReceived, 1);
            }
            break;
        }
        data++;
    }
    return ERR_OK;//解析了一部分数据
}
#endif /* LWIP_TCP */
