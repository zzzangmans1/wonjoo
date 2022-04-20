using System;
using System.Collections.Generic;
using System.Text;
struct EthernetIIProtocol
{
    public byte[] Destination;      /* 목적지의 2계층 주소를 나타낸다.
                                     * 6Byte 주소의 첫번째 비트가 "1' 이면 Multicast를 의미한다.
                                     * 6Byte 주소의 모든 비트가 "1" 이면 Broadcast를 의미한다.
                                     */

    public byte[] Source;           /* 출발지의 2계층 주소를 나타낸다.
                                     */

    public byte[] Type;             /* MAC 프레임의 데이터 부분에 실려있는 상위 계층의 프로토콜 종류를 표시하는 영역
                                     * 2Byte
                                     * 0x0000 ~ 0x05dc      IEEE 802.3 Data Length
                                     * 0x0600               XNS IDP
                                     * 0x0800               IP
                                     * 0x0805               X.25 PLP
                                     * 0x0806               ARP
                                     * 0x8035               RARP
                                     * 0x8137               Netware IPX
                                     * 0x8191               NetBIOS
                                     */
}

struct InternetProtocol
{
    public byte[] Virsion;               /* TCP/IP 제품은 IPv4를 사용한다.
                                        * 4Bit
                                        */
    public byte[] HeaderLength;          /* IP 헤더의 길이를 32비트 단위로 나타낸다. 대부분의 IP 헤더의 길이는 20바이트입니다.
                                        * 필드 값은 거의다 항상 5다. (5 * 32 = 160Bit or 20Byte)
                                        * 4Bit
                                        */
    public byte[] TypeOfServiceFlags;   /* 서비스의 우선순위를 제공한다.
                                        * 0 - 보통 : 보통 데이터로 취급, 대부분의 IP 데이터는 이 분류에 속한다.
                                        * 1 - 비용최소화 : IP가 가장 비용이 적게 드는 경로를 통해 패킷을 라우트할 것을 요구하는 경우 사용 0 - 보통 1 - 저비용
                                        * 2 - 신뢰성 최대화 : IP가 가장 신뢰성 있는 네트워크를 통해 패킷을 라우터할 것을 요구하는 경우 사용 0 - 보통 1 - 높은신뢰성
                                        * 4 - 처리량 최대화 : IP가 보통 데이터 패킷을 더 많은 처리를 하면서 라우트할 것을 요구하는 경우 사용 0 - 보통 1 - 높은 처리량
                                        * 8 - 지연 최소화 : IP가 보통 데이터 보다 더 신속한 패킷의 라우트를 요구하는 경우 사용 0 - 보통 1 - 지연 줄임
                                        * 15 - 보안 최대화 : IP가 가장 안전한 경로를 통해 패킷을 라우트할 것을 요구할 수 있다고 한다. 실험용
                                        */
    public byte[] TotalPacketLength;    /* 전체 패킷의 길이를 바이트 단위로 나타낸다.
                                         * 16Bit
                                         */
    public byte[] Fragmentidentifier;   /* 분열이 발생한 경우, 조각을 다시 결핍하기 위해 원래의 데이터를 식별하기 위해서 사용한다.
                                         * 16Bit
                                         */
    public byte[] FragmentationFlags;   /* 처음 1bit는 항상 0으로 설정, 나머지 2비트의 용도는 다음과 같다.
                                         * May Fragment : IP 라우터에 의해 분열되는 여부를 나타낸다. 0 - 분열 가능 1 - 분열 방지
                                         * More Fragments : 원래 데이터의 분열된 조각이 더 있는지 여부 판단. 0 - 마지막 조각 기본값 1 - 조각이 더 있음
                                         * 3Bit
                                         */
    public byte[] FragmentationOffset;   /* 8바이트 오프셋으로 조각에 저장된 원래 데이터의 바이트 범위를 나타낸다.
                                          * 13Bit
                                         */
    public byte[] TimeToLive;           /* 데이터를 전달할 수 없는 것으로 판단되어 소멸되기 이전에 데이터가 이동할 수 있는 단계의 수를 나타낸다.
                                         * TimeToLive 필드는 1에서 255사이의 값을 지정하며 라우터들은 패킷을 전달할 때마다 이 값을 하나씩 감소시킨다.
                                         * 8Bit
                                         */
    public byte[] ProtocolIdentifier;   /* 상위 계층 프로토콜
                                         * 1 - ICMP, 2 - IGMP, 6 - TCP, 17 - UDP
                                         * 8Bit
                                         */
    public byte[] HeaderChecksum;       /* IP 헤더의 체크섬을 저장, 라우터를 지나갈 때 마다 재 계산을 하기 때문에 속도가 떨어진다.
                                         * 16Bit
                                         */
    public byte[] SourceIPAddress;      /* 출발지 IP 주소
                                         * 32Bit
                                         */
    public byte[] DestionationIPAdrress;/* 목적지 IP 주소
                                         * 32Bit
                                         */
}

struct Tcp
{
    public byte[] SourcePort;                   /* 출발지 포트번호를 표시한다. 응용 서비스에 따라 포트번호가 정해져 있는 것도 있지만, 
                                                 * 대부분의 경우 처음 세그먼트를 전송하는 측에서 임의의 번호를 사용한다.
                                                 * 16Bit
                                                 */
    public byte[] DestinationPort;              /* 목적지 포트번호를 표시한다. 응용 서비스에 따라 포트번호가 정해져 있다.
                                                 * 16Bit
                                                 */
    public byte[] SequenceNumber;               /* TCP 순서번호를 표시한다. 통신을 시작하는 양단의 장비들의 별개로 임의의 번호이다.
                                                 * 32Bit
                                                 */
    public byte[] AcknowledgmentNumber;         /* 상대방이 보낸 세그먼트를 잘 받았다는 것을 알려주기 위한 번호이다.
                                                 * 32Bit
                                                 */
    public byte[] Offset;                       /* TCP 헤더 길이를 4바이트 단위로 표시한다. TCP 헤더는 최소 20, 최대 60Byte이다.
                                                 * 4Bit
                                                 */
    public byte[] Reserved;                     /* 사용하지 않는 필드이며 모두 0으로 표시한다.
                                                 * 4Bit
                                                 */
    public byte[] Flags;                        /* 제어비트(Control Bit)라고도 하며, 세그먼트의 종류를 표시하는 필드이다.
                                                 * 8BIt
                                                 */
    public byte[] WindowSize;                   /* 상대방의 확인 없이 전송할 수 있는 최대 바이트 수를 표시한다.
                                                 * 16Bit
                                                 */
    public byte[] CheckSum;                     /* 헤더와 데이터의 에러를 확인하기 위한 필드이다.
                                                 * 16Bit
                                                 */
    public byte[] UrgentPointer;                /* 현재의 순서 번호부터 긴급포인트에 표시된 바이트까지가 긴급한 데이터임을 표시한다.
                                                 * 16Bit
                                                 */
   // public byte[] Option;
}

struct Udp
{
    public byte[] SourcePort;
    public byte[] DestinationPort;
    public byte[] Length;
    public byte[] CheckSum;
}

namespace ProtocolHeaderVeiwer
{
    public class ProtocolHeaderVeiw
    {
        public static string PrintIPProtocol(byte[] data, int start, int end)
        {
            string ret = "";
            InternetProtocol IP;
            IP.Virsion = new byte[1];
            IP.HeaderLength = new byte[1];
            IP.TypeOfServiceFlags = new byte[1];
            IP.TotalPacketLength = new byte[2];
            IP.Fragmentidentifier = new byte[2];
            IP.FragmentationFlags = new byte[1];
            IP.FragmentationOffset = new byte[2];
            IP.TimeToLive = new byte[1];
            IP.ProtocolIdentifier = new byte[1];
            IP.HeaderChecksum = new byte[2];
            IP.SourceIPAddress = new byte[4];
            IP.DestionationIPAdrress = new byte[4];

            

            int i, j = 0, k = 0, l = 0, m = 0, n = 0;
            for (i = start; i < end; i++)
            {
                if (i == start)
                {
                    IP.Virsion[0] = (byte)((data[i] >> 4) & 0xF);
                    IP.HeaderLength[0] = (byte)(data[i] & 0xF);
                }
                else if (i == (start + 1)) IP.TypeOfServiceFlags[0] = data[i];
                else if (i < (start + 4))
                {
                    IP.TotalPacketLength[j++] = data[i];
                }
                else if (i < (start + 6))
                {

                    IP.Fragmentidentifier[k++] = data[i];
                }
                else if (i < (start + 7))
                {
                    IP.FragmentationFlags[0] = (byte)(data[i] >> 5);
                    IP.FragmentationOffset[0] = (byte)(data[i] & 0x1F);
                }
                else if (i < (start + 8)) IP.FragmentationOffset[1] = data[i];
                else if (i < (start + 9)) IP.TimeToLive[0] = data[i];
                else if (i < (start + 10)) IP.ProtocolIdentifier[0] = data[i];
                else if (i < (start + 12)) IP.HeaderChecksum[l++] = data[i];
                else if (i < (start + 16)) IP.SourceIPAddress[m++] = data[i];
                else if (i < (start + 20)) IP.DestionationIPAdrress[n++] = data[i];
            }
            Console.WriteLine("====================== Internet Protocol start=======================");
            Console.WriteLine("Version : {0:X02}", IP.Virsion[0]);
            Console.WriteLine("Header Length : {0:X02}", IP.HeaderLength[0]);
            Console.WriteLine("Type of Service : {0:X02}", IP.TypeOfServiceFlags[0]);
            Console.Write("Total Length : ");
            for (i = 0; i < 2; i++) Console.Write("{0:X02}", IP.TotalPacketLength[i]);
            Console.WriteLine("");
            Console.Write("Fragment Identifier : ");
            for (i = 0; i < 2; i++) Console.Write("{0:X02}", IP.Fragmentidentifier[i]);
            Console.WriteLine("");
            Console.WriteLine("Fragmentation Flags : {0:X02}", IP.FragmentationFlags[0]);
            Console.Write("Fragment Offset : ");
            for (i = 0; i < 2; i++) Console.Write("{0:X02}", IP.FragmentationOffset[i]);
            Console.WriteLine("");
            Console.WriteLine("Time To Live : {0:X02}", IP.TimeToLive[0]);
            Console.WriteLine("Protocol Identifier : {0:X02}", IP.ProtocolIdentifier[0]);
            if (IP.ProtocolIdentifier[0] == 6) ret = "TCP";
            if (IP.ProtocolIdentifier[0] == 17) ret = "UDP";
            if (IP.ProtocolIdentifier[0] == 1) ret = "ICMP";
            if (IP.ProtocolIdentifier[0] == 2) ret = "IGMP";
            Console.Write("Header Checksum : ");
            for (i = 0; i < 2; i++) Console.Write("{0:X02}", IP.HeaderChecksum[i]);
            Console.WriteLine("");
            Console.Write("Source IP : ");
            for (i = 0; i < 4; i++) Console.Write("{0}.", IP.SourceIPAddress[i]);
            Console.WriteLine("");
            Console.Write("Destination IP : ");
            for (i = 0; i < 4; i++) Console.Write("{0}.", IP.DestionationIPAdrress[i]);
            Console.WriteLine("");
            Console.WriteLine("====================== Internet Protocol end=======================");
            return ret;
        }

        public static void PrintEthernetProtocol(byte[] data, int start, int end)
        {
            EthernetIIProtocol EIIP;
            EIIP.Destination = new byte[6];
            EIIP.Source = new byte[6];
            EIIP.Type = new byte[2];

            int i, j = 0, k = 0, l = 0;
            Console.WriteLine("====================== Ethernet Protocol start=======================");
            for (i = start; i < end; i++)
            {
                if (i < 6) EIIP.Destination[j++] = data[i];
                else if (i < 12) EIIP.Source[k++] = data[i];
                else if (i < 14) EIIP.Type[l++] = data[i];
            }
            Console.Write("Destination : ");
            for (i = 0; i < 6; i++)
            {
                Console.Write("{0:X02}:", EIIP.Destination[i]);
            }
            Console.WriteLine("");
            Console.Write("Source : ");
            for (i = 0; i < 6; i++)
            {
                Console.Write("{0:X02}:", EIIP.Source[i]);
            }
            Console.WriteLine("");
            Console.Write("Type : 0x");
            for (i = 0; i < 2; i++)
            {
                Console.Write("{0:X02}", EIIP.Type[i]);
            }
            Console.WriteLine("");
            Console.WriteLine("====================== Ethernet Protocol end=======================");
        }

        public static void PrintTCP(byte[] data, int start, int end)
        {
            Tcp tcp;
            tcp.SourcePort = new byte[2];
            tcp.DestinationPort = new byte[2];
            tcp.SequenceNumber = new byte[4];
            tcp.AcknowledgmentNumber = new byte[4];
            tcp.Offset = new byte[1];
            tcp.Reserved = new byte[1];
            tcp.Flags = new byte[1];
            tcp.WindowSize = new byte[2];
            tcp.CheckSum = new byte[2];
            tcp.UrgentPointer = new byte[2];
            int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0, o = 0, p = 0;
            for (i = start; i < end; i++)
            {
                if (i < (start + 2)) tcp.SourcePort[j++] = data[i];
                else if (i < (start + 4)) tcp.DestinationPort[k++] = data[i];
                else if (i < (start + 8)) tcp.SequenceNumber[l++] = data[i];
                else if (i < (start + 12)) tcp.AcknowledgmentNumber[m++] = data[i];
                else if (i < (start + 13))
                {
                    tcp.Offset[0] = (byte)((data[i] >> 4) & 0xF);
                    tcp.Reserved[0] = (byte)(data[i] & 0xF);
                }
                else if (i < (start + 14)) tcp.Flags[0] = data[i];
                else if (i < (start + 16)) tcp.WindowSize[n++] = data[i];
                else if (i < (start + 18)) tcp.CheckSum[o++] = data[i];
                else if (i < (start + 20)) tcp.UrgentPointer[p++] = data[i];

            }
            Console.WriteLine("====================== TCP start=======================");
            Console.Write("Source Port : ");
            for (i = 0; i < 2; i++) Console.Write("{0:X02}", tcp.SourcePort[i]);
            Console.WriteLine("");
            Console.Write("Destination Port : ");
            for (i = 0; i < 2; i++) Console.Write("{0:X02}", tcp.DestinationPort[i]);
            Console.WriteLine("");
            Console.Write("Sequence Number : ");
            for (i = 0; i < 4; i++) Console.Write("{0:X02}", tcp.SequenceNumber[i]);
            Console.WriteLine("");
            Console.Write("Acknowledgment Number : ");
            for (i = 0; i < 4; i++) Console.Write("{0:X02}", tcp.AcknowledgmentNumber[i]);
            Console.WriteLine("");
            Console.WriteLine("Offset : {0}", tcp.Offset[0]);
            Console.WriteLine("Reserved : {0}", tcp.Reserved[0]);
            Console.WriteLine("Flags : {0}", tcp.Flags[0]);
            Console.Write("Window size : ");
            for (i = 0; i < 2; i++) Console.Write("{0:X02}", tcp.WindowSize[i]);
            Console.WriteLine("");
            Console.Write("Checksum : ");
            for (i = 0; i < 2; i++) Console.Write("{0:X02}", tcp.CheckSum[i]);
            Console.WriteLine("");
            Console.Write("Urgent Pointer : ");
            for (i = 0; i < 2; i++) Console.Write("{0:X02}", tcp.UrgentPointer[i]);
            Console.WriteLine("");

            Console.WriteLine("====================== TCP end=======================");
        }
        public static string PrintUDP(byte[] data, int start, int end)
        {
            Udp udp;
            udp.SourcePort = new byte[2];
            udp.DestinationPort = new byte[2];
            udp.Length = new byte[2];
            udp.CheckSum = new byte[2];
            string ret = null;
            int i = 0, j = 0, k = 0, l = 0, m=0;
            for (i = start; i < end; i++)
            {
                if (i < (start + 2)) udp.SourcePort[j++] = data[i];
                else if (i < (start + 4)) udp.DestinationPort[k++] = data[i];
                else if (i < (start + 6)) udp.Length[l++] = data[i];
                else if (i < (start + 8)) udp.CheckSum[m++] = data[i];
            }
            Console.WriteLine("====================== UDP start=======================");
            Console.Write("Source Port : ");
            for (i = 0; i < 2; i++) Console.Write("{0}", udp.SourcePort[i]);
            Console.WriteLine("");
            if (udp.SourcePort[1] == 53) ret = "DNS";
            Console.Write("Destination Port : ");
            for (i = 0; i < 2; i++) Console.Write("{0}", udp.DestinationPort[i]);
            Console.WriteLine("");
            Console.Write("Length : ");
            for (i = 0; i < 2; i++) Console.Write("{0:X02}", udp.Length[i]);
            Console.WriteLine("");
            Console.Write("Checksum : ");
            for (i = 0; i < 2; i++) Console.Write("{0:X02}", udp.CheckSum[i]);
            Console.WriteLine("");
            Console.WriteLine("====================== UDP end=======================");
            return ret;
        }

        public static void PrintData(byte[] data, int start, int end)
        {
            int i, jmp = 1;
            Console.WriteLine("\n=================================== Data ================================");
            for(i = start;i<end;i++)
            {
                Console.Write("{0:X02} ", data[i]);
                if(jmp++ == 16)
                {
                    jmp = 1;
                    Console.WriteLine("");
                }
            }
            Console.WriteLine("\n=================================== Data ================================");
        }


    }
}
