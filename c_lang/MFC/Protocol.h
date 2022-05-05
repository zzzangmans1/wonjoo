// *** Protocol 구조체 정리 헤더
// 헤더 가드 시작 
// 헤더 가드 : 헤더파일이 같은 파일에서 두 번 이상 포함되지 않게 해준다.
#ifndef PROTOCOL_H
#define PROTOCOL_H

	// *** 구조체 
typedef struct ether_addr
{
	unsigned char e_host[6];
}ETHER_ADDR;
typedef struct ethernet_header
{
	ETHER_ADDR dst;
	ETHER_ADDR src;
	unsigned short type;
}ETHERNET_HEADER;
//ip_header 
typedef struct ip_header
{
	unsigned char header_len : 4;
	unsigned char ver : 4;
	unsigned char tos;
	unsigned short total_len;
	unsigned short id;
	unsigned char frag_offset : 5;
	unsigned char mf : 1;
	unsigned char df : 1;
	unsigned char rz : 1;
	unsigned char frag_offset1;
	unsigned char ttl;
	unsigned char protocol;
	//1=ICMP, 2=IGMP, 6=TCP, 8= EGP, 17=UDP, 89=OSPF 등등.....
	unsigned short cksum;
	IN_ADDR src_ip;
	IN_ADDR dst_ip;
}IP_HEADER;
typedef struct tcp_header
{
	unsigned short src_port;
	unsigned short dst_port;
	unsigned int seq;
	unsigned int ack;
	unsigned char ns : 1;
	unsigned char reserved : 3;
	unsigned char data_offset : 4;
	unsigned char flag_fin : 1;
	unsigned char flag_syn : 1;
	unsigned char flag_rst : 1;
	unsigned char flag_psh : 1;
	unsigned char flag_ack : 1;
	unsigned char flag_urg : 1;
	unsigned char flag_ece : 1;
	unsigned char flag_cwr : 1;
	unsigned short window;
	unsigned short cksum;
	unsigned short up;

}TCP_HEADER;
typedef struct udp_header
{
	unsigned short sport;
	unsigned short dport;
	unsigned short length;
	unsigned short checksum;
}UDP_HEADER;

#endif
