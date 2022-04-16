#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <WinSock2.h>
#include <winsock.h>

#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32")		// inet_ntoa 사용하기 위해 필요한 코드

#define FAIL			-1
#define PROMISC			1			// promiscuous mode
#define NPROMISC		0			// none promiscuous mode
#define FILETER_RULE	"tcp or udp"
#define CAPTURE_BYTES	65536	

