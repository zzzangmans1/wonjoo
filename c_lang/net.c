#include "config.h"


// global variable
int8_t errbuf[PCAP_ERRBUF_SIZE];

// functions
int32_t init();
void pcap_hand(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data);

int32_t  main() {

	if (init() == 0) {
		printf("init() FAIL\n");
		return 0;
	}
	printf("init() SUCCESS\n");
	
	return 0;
}

int32_t init()
{
	pcap_if_t* alldev = NULL, *dev = NULL;
	pcap_t* p_com;

	//pcap_addr* p_addr;

	bpf_program fp;


	size_t a = 1, b = 0;

	printf("*****************init() START****************\n\n");

	/*
		pcap_findalldevs() : pcap_open_live()로 열린 디바이스 리스트를 구한다.
		첫 번째 인자 구조체에 그 정보들을 담는다.
		두 번째 인자는 에러정보를 저장한다.
	*/
	if (pcap_findalldevs(&alldev, (char*)errbuf) == FAIL) {
		printf("pcap_findalldevs error : %s\n", errbuf);
		pcap_freealldevs(alldev);
		return 0;
	}

	/*
		디바이스 출력하는 반복문
	*/
	for(dev = alldev; dev->next != NULL; dev = dev->next)
		printf("%d : %s\t %s\n", a++, dev->name, dev->description);
	

	/*
		디바이스 선택
	*/
	printf("Choice device number : ");
	scanf("%d", &b);
	for (dev = alldev, a = 0; a < b - 1; dev = dev->next, a++);

	/*
		int pcap_lookupnet(char* device,		네트워크 디바이스의 별명
							bpf_u_int32 *netp,	네트워크 디바이스의 네트워크 주소가 저장될 주소
							bpf_u_int32 *maskp,	네트워크 디바이스의 netmask가 저장될 주소
							char *errbuf		에러 발생 시 에러 메시지 저장
							)
							성공 : 0, 실패 : -1
		네트워크 디바이스의 네트워크 주소와 netmask 정보를 가져오기 위해 사용된다.
	*/
	printf("Choice device name : %s\n", dev->description);
	
	/*
	if(pcap_lookupnet(dev->name, &netp, &maskp, (char*)errbuf) == -1)
	{
		printf("pcap_lookupnet error : %s\n", errbuf);
		pcap_freealldevs(alldev);
		return 0;
	}

	*/
	struct sockaddr_in* in_addr = (struct sockaddr_in*)dev->addresses;
	printf("address : %s\n", inet_ntoa(in_addr->sin_addr));
	
	printf("pcap_lookupnet success\n");

	/*
		pcap_t* pcap_open_live(char* device,	사용할 네트워크 디바이스명
								int snaplen,	캡쳐할 최대 bytes
								int promisc,	NIC가 promiscuous mode로 동작할 지 여부
											*	promiscuous mode 는 1 : 로컬 네트워크의 모든 패킷을 캡쳐하게 된다.
								int to_ms,		millisecond 단위의 read timeout
								char *ebuf		pcap_open_live()가 실패할 경우에만 에러 메시지가 저장된다. 성공은 NULL 이 저장된다.
								)		
		네트워크의 패킷들을 보기위해 필요한 packet capture descriptor를 얻기 위해 사용된다.
	*/
	if ((p_com = pcap_open_live(dev->name, CAPTURE_BYTES, NPROMISC, 200, (char*)errbuf)) == NULL) {
		printf("pcap_open_live error : %s\n", errbuf);
		pcap_freealldevs(alldev);
		return 0;
	}
	printf("pcap_open_live success\n");
	// pcap_open_live() 로 네트워크 디바이스를 선택하였으므로 다른 네트워크 디바이스 리스트 삭제
	pcap_freealldevs(alldev);

	/*
		int pcap_compile(pcap_t *p,				패킷 캡쳐 descriptor
						struct bpf_program *fp	필터링 룰에 따라 결정될 구조체
						char *str,				스트링 형태의 필터링 룰
						int optimize,			결과 코드를 수행할 때 최적화 여부
						bpf_u_int32 netmask		네트워크의 mask
						)
		스트링 형태의 필터링 룰을 bpf_program 구조체에 저장한다.
	*/
	if (pcap_compile(p_com, &fp, FILETER_RULE, 1, NULL) == FAIL)				// 컴파일 옵션 선택
	{
		printf("pcap_compile() error\n");
		pcap_freealldevs(alldev);
		return 0;
	}
	printf("pcap_compile success\n");

	/*
		int pcap_setfilter(pcap_t *p				패킷 캡쳐 descriptor
						, struct bpf_program *fp)	pcap_compile() 에서 결과
						성공 : 0, 실패 : -1
	*/
	if ((pcap_setfilter(p_com, &fp)) == FAIL)
	{
		printf("pcap_setfileter error\n");
		pcap_freealldevs(alldev);
		return 0;
	}
	printf("pcap_setfileter success\n");

	/*
		int pcap_loop(pcap_t *p,				pcap_open_live()에서 return 되는 descriptor
						int cnt,				읽을 패킷의 수, 0 : 무한루프
						pcap_handler callback,	패킷을 처리할 루틴
						u_char *user			패킷 데이터 포인터
						)
		패킷의 그룹을 모은다.
	*/
	pcap_loop(p_com, 0, pcap_hand, NULL);
	/*
		pcap_freealldevs(pcap_if_t*) : findalldevs 함수로 가져온 리스트 들을 해제한다.
	*/

	pcap_close(p_com);
	printf("\n\n*****************init() END****************\n");
	//system("cls");
	return 1;
}

void pcap_hand(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data)
{
	int i;


	/* 패킷 출력 */
	for (i = 1; (i < header->caplen + 1); i++)
	{
		printf("%.2x ", pkt_data[i - 1]);
		if ((i % 16) == 0) printf("\n");
	}

	printf("\n\n");
}
