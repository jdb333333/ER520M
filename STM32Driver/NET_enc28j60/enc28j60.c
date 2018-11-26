/******************** (C) COPYRIGHT 2012 ***************************************
 * �ļ���  ��ENC28J60.c
 * ����    ��ENC28J60����
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * http://www.gnu.org/licenses/gpl.html
 *
 * Based on the enc28j60.c file from the AVRlib library by Pascal Stang.
 * For AVRlib See http://www.procyonengineering.com/
 * Used with explicit permission of Pascal Stang.
 *
 * Title: Microchip ENC28J60 Ethernet Interface Driver
 * Chip type           : ATMEGA88 with ENC28J60
 *******************************************************************************/

#include "enc28j60.h"
#include "spi_enc28j60.h"

static u8 Enc28j60Bank;
static u16 NextPacketPtr;

/*
 * ��������enc28j60ReadOp
 * ����  �������ƼĴ���
 * ����  ����ָ���ַ
 * ���  ���Ĵ�������
 */
u8 enc28j60ReadOp(u8 op, u8 address)
{
   u8 dat = 0;

   ENC28J60_CSL();

   dat = op | (address & ADDR_MASK);
   ENC_SPI_ReadWrite(dat);
   dat = ENC_SPI_ReadWrite(0xFF);
   // do dummy read if needed (for mac and mii, see datasheet page 29)
   if (address & 0x80)
   {
      dat = ENC_SPI_ReadWrite(0xFF);
   }
   // release CS
   ENC28J60_CSH();
   return dat;
}

/*
 * ��������enc28j60WriteOp
 * ����  ������̫��������д������
 * ����  ����
 * ���  ����
 */
void enc28j60WriteOp(u8 op, u8 address, u8 data)
{
   u8 dat = 0;

   ENC28J60_CSL();
   // issue write command
   dat = op | (address & ADDR_MASK);
   ENC_SPI_ReadWrite(dat);
   // write data
   dat = data;
   ENC_SPI_ReadWrite(dat);
   ENC28J60_CSH();
}

/*
 * ��������enc28j60ReadBuffer
 * ����  ����ȡbuffer������
 * ����  �����ݳ��ȣ�������ݵ�ָ��
 * ���  ����
 */
void enc28j60ReadBuffer(u16 len, u8* data)
{
   ENC28J60_CSL();
   // issue read command
   ENC_SPI_ReadWrite(ENC28J60_READ_BUF_MEM);
   while (len--)
   {
      *data++ = (u8) ENC_SPI_ReadWrite(0);
   }
   *data = '\0';
   ENC28J60_CSH();
}

/*
 * ��������enc28j60WriteBuffer
 * ����  ����bufferд������
 * ����  �����ݳ��ȣ�������ݵ�ָ��
 * ���  ����
 */
void enc28j60WriteBuffer(u16 len, u8* data)
{
   ENC28J60_CSL();
   // issue write command
   ENC_SPI_ReadWrite(ENC28J60_WRITE_BUF_MEM);

   while (len--)
   {
      ENC_SPI_ReadWrite(*data++);
   }
   ENC28J60_CSH();
}

/*
 * ��������enc28j60SetBank
 * ����  ��ѡ����Ҫ������bank
 * ����  ������bank������Ĳ���
 * ���  ����
 */
void enc28j60SetBank(u8 address)
{
   // set the bank (if needed)
   if ((address & BANK_MASK) != Enc28j60Bank)
   {
      // set the bank
      enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1 | ECON1_BSEL0));
      enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK) >> 5);
      Enc28j60Bank = (address & BANK_MASK);
   }
}

/*
 * ��������enc28j60Read
 * ����  ����ȡ���ƼĴ���������
 * ����  �����ƼĴ����ĵ�ַ
 * ���  ����
 */
u8 enc28j60Read(u8 address)
{
   // set the bank
   enc28j60SetBank(address);
   // do the read
   return enc28j60ReadOp(ENC28J60_READ_CTRL_REG, address);
}


/*
 * ��������enc28j60Write
 * ����  ��д�����ݵ����ƼĴ���
 * ����  �����ƼĴ����ĵ�ַ������
 * ���  ����
 */
void enc28j60Write(u8 address, u8 data)
{
   // set the bank
   enc28j60SetBank(address);
   // do the write
   enc28j60WriteOp(ENC28J60_WRITE_CTRL_REG, address, data);
}

/*
 * ��������enc28j60PhyWrite
 * ����  ����phyд����Ʋ���
 * ����  ���Ĵ����ĵ�ַ������
 * ���  ����
 */
void enc28j60PhyWrite(u8 address, u16 data)
{
   // set the PHY register address
   enc28j60Write(MIREGADR, address);
   // write the PHY data
   enc28j60Write(MIWRL, data);
   enc28j60Write(MIWRH, data >> 8);
   // wait until the PHY write completes
   while (enc28j60Read(MISTAT) & MISTAT_BUSY)
   {
      //Del_10us(1);
      //_nop_();
   }
}

/*
 * ��������enc28j60clkout
 * ����  ���������ʱ��
 * ����  ��ʱ�Ӳ���
 * ���  ����
 */
void enc28j60clkout(u8 clk)
{
   //setup clkout: 2 is 12.5MHz:
   enc28j60Write(ECOCON, clk & 0x7);
}

/*
 * ��������enc28j60Init
 * ����  ����ʼ����̫��������
 * ����  ����
 * ���  ����
 */
void enc28j60Init(u8 * macaddr)
{

   //ENC28J60_RSTH();//ouhs
   ENC28J60_CSH();/* ��ֹENC28J60 */
   enc28j60WriteOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);//ENC28J60������λ
   // Del_1ms(250);
   // check CLKRDY bit to see if reset is complete
   // The CLKRDY does not work. See Rev. B4 Silicon Errata point. Just wait.
   //while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY));��ѯESTAT.CLKRDYλ
   // do bank 0 stuff
   // initialize receive buffer
   // 16-bit transfers, must write low byte first
   // set receive buffer start address���ý��ջ�������ʼ��ַ �ñ�������ÿ�ζ�ȡ������ʱ������һ�������׵�ַ
   NextPacketPtr = RXSTART_INIT;
   // Rx start���ý��ջ����� ��ʼָ��
   enc28j60Write(ERXSTL, RXSTART_INIT & 0xFF);
   enc28j60Write(ERXSTH, RXSTART_INIT >> 8);
   // set receive pointer address���ý��ջ����� ��ָ��
   enc28j60Write(ERXRDPTL, RXSTART_INIT & 0xFF);
   enc28j60Write(ERXRDPTH, RXSTART_INIT >> 8);
   // RX end���ý��ջ����� ����ָ��
   enc28j60Write(ERXNDL, RXSTOP_INIT & 0xFF);
   enc28j60Write(ERXNDH, RXSTOP_INIT >> 8);
   // TX start���÷��ͻ����� ��ʼָ��
   enc28j60Write(ETXSTL, TXSTART_INIT & 0xFF);
   enc28j60Write(ETXSTH, TXSTART_INIT >> 8);
   // TX end���÷��ͻ����� ����ָ��
   enc28j60Write(ETXNDL, TXSTOP_INIT & 0xFF);
   enc28j60Write(ETXNDH, TXSTOP_INIT >> 8);
   // do bank 1 stuff, packet filter:ʹ�ܵ������� ʹ��CRCУ�� ʹ�� ��ʽƥ���Զ�����
   // For broadcast packets we allow only ARP packtets
   // All other packets should be unicast only for our mac (MAADR)
   //
   // The pattern to match on is therefore
   // Type     ETH.DST
   // ARP      BROADCAST
   // 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
   // in binary these poitions are:11 0000 0011 1111
   // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
   enc28j60Write(ERXFCON, ERXFCON_UCEN | ERXFCON_CRCEN | ERXFCON_PMEN);
   enc28j60Write(EPMM0, 0x3f);
   enc28j60Write(EPMM1, 0x30);
   enc28j60Write(EPMCSL, 0xf9);
   enc28j60Write(EPMCSH, 0xf7);
   //
   //
   // do bank 2 stuff
   // enable MAC receiveʹ��MAC���� ����MAC������ͣ����֡ �����յ���ͣ����֡ʱֹͣ����
   enc28j60Write(MACON1, MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS);
   // bring MAC out of reset�˳���λ״̬
   enc28j60Write(MACON2, 0x00);
   /* ��0������ж�֡��60�ֽڳ� ��׷��һ��CRC ����CRCʹ�� ֡����У��ʹ�� MACȫ˫��ʹ��*/
   // enable automatic padding to 60bytes and CRC operations��ʾ ����ENC28J60��֧��802.3���Զ�Э�̻��ƣ� ���ԶԶ˵����翨��Ҫǿ������Ϊȫ˫��
   enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN | MACON3_FULDPX );//MACON3_HFRMLEN
   // set inter-frame gap (non-back-to-back)
   enc28j60Write(MAIPGL, 0x12);
   enc28j60Write(MAIPGH, 0x0C);
   // set inter-frame gap (back-to-back)
   enc28j60Write(MABBIPG, 0x12);
   // Set the maximum packet size which the controller will accept
   // Do not send packets longer than MAX_FRAMELEN:���֡����
   enc28j60Write(MAMXFLL, MAX_FRAMELEN & 0xFF);
   enc28j60Write(MAMXFLH, MAX_FRAMELEN >> 8);
   // do bank 3 stuff
   // write MAC addressд��MAC��ַ
   // NOTE: MAC address in ENC28J60 is byte-backward
   enc28j60Write(MAADR5, macaddr[0]);
   enc28j60Write(MAADR4, macaddr[1]);
   enc28j60Write(MAADR3, macaddr[2]);
   enc28j60Write(MAADR2, macaddr[3]);
   enc28j60Write(MAADR1, macaddr[4]);
   enc28j60Write(MAADR0, macaddr[5]);


   /* ����PHYΪȫ˫��  LEDBΪ������ */
   enc28j60PhyWrite(PHCON1, PHCON1_PDPXMD);


   // no loopback of transmitted frames��˫���ػ���ֹ
   enc28j60PhyWrite(PHCON2, PHCON2_HDLDIS);
   // switch to bank 0����BANK0
   enc28j60SetBank(ECON1);
   // enable interrutpsʹ���ж� ȫ���ж� �����ж� �����ж�
   enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE | EIE_PKTIE | EIE_LINKIE);//ccr2015-01-31
   // enable packet reception����ʹ��λ
   enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
}

// read the revision of the chip:

u8 enc28j60getrev(void)
{
   return(enc28j60Read(EREVID));
}

/*
 * ��������enc28j60PacketSend
 * ����  ������һ�����ݰ�
 * ����  �����ݰ����ȣ�ָ�����ݰ���ָ��
 * ���  ����
 */
void enc28j60PacketSend(u16 len, u8* packet)
{
   // Set the write pointer to start of transmit buffer area
   enc28j60Write(EWRPTL, TXSTART_INIT & 0xFF);
   enc28j60Write(EWRPTH, TXSTART_INIT >> 8);

   // Set the TXND pointer to correspond to the packet size given
   enc28j60Write(ETXNDL, (TXSTART_INIT + len) & 0xFF);
   enc28j60Write(ETXNDH, (TXSTART_INIT + len) >> 8);

   // write per-packet control byte (0x00 means use macon3 settings)
   enc28j60WriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);

   // copy the packet into the transmit buffer
   enc28j60WriteBuffer(len, packet);

   // send the contents of the transmit buffer onto the network
   enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);

   // Reset the transmit logic problem. See Rev. B4 Silicon Errata point 12.
   if ((enc28j60Read(EIR) & EIR_TXERIF))
   {
      enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRTS);
   }
}
void RS232_send_byte(uint8_t ch );

/*-----------------------------------------------------------------
 Gets a packet from the network receive buffer, if one is available.
 The packet will by headed by an ethernet header.
      maxlen  The maximum acceptable length of a retrieved packet.
      packet  Pointer where packet data should be stored.
 Returns: Packet length in bytes if a packet was retrieved, zero otherwise.
-------------------------------------------------------------------*/
u16 enc28j60PacketReceive(u16 maxlen, u8* packet)
{
   u16 rxstat;
   u16 len;
   u8 h_byte;

   // check if a packet has been received and buffered
   //if( !(enc28j60Read(EIR) & EIR_PKTIF) ){
   // The above does not work. See Rev. B4 Silicon Errata point 6.
   if (enc28j60Read(EPKTCNT) == 0)
   {
      return(0);
   }

   // Set the read pointer to the start of the received packet
   enc28j60Write(ERDPTL, (NextPacketPtr));
   enc28j60Write(ERDPTH, (NextPacketPtr) >> 8);

   // read the next packet pointer
   NextPacketPtr  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
   NextPacketPtr |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0) << 8;

   // read the packet length (see datasheet page 43)
   len  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
   len |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0) << 8;

   len -= 4; //remove the CRC count
   // read the receive status (see datasheet page 43)
   rxstat = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
   rxstat |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0) << 8;
   // limit retrieve length
   if (len > maxlen - 1)
   {
      len = maxlen - 1;
   }

   // check CRC and symbol errors (see datasheet page 44, table 7-3):
   // The ERXFCON.CRCEN is set by default. Normally we should not
   // need to check this.
   if ((rxstat & 0x80) == 0)
   {
      // invalid
      len = 0;
   }
   else
   {
      // copy the packet from the receive buffer
      enc28j60ReadBuffer(len, packet);
   }
   // Move the RX read pointer to the start of the next received packet
   // This frees the memory we just read out
   enc28j60Write(ERXRDPTL, (NextPacketPtr));
   enc28j60Write(ERXRDPTH, (NextPacketPtr) >> 8);

   // decrement the packet counter indicate we are done with this packet
   enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
   return(len);
}


