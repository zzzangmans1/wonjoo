using System;
using System.Net.NetworkInformation;
using SharpPcap;
using HexaViewer;
using ProtocolHeaderVeiwer;

namespace pack_cc
{
    class Program
    {
        static int pcnt = 0; //캡쳐한 패킷 개수

        static int total = 0; //캡쳐한 전체 패킷 바이트

        static void Main(string[] args)
        {

            // SharpPcap의 버전 출력
            string ver = SharpPcap.Version.VersionString;
            Console.WriteLine("SharpPcap Version : {0} ", ver);

            // 추적할 수 있는 네트워크 인터페이스 목록 참조
            CaptureDeviceList alldevs = CaptureDeviceList.Instance;

            if(alldevs.Count == 0)
            {
                Console.WriteLine("추적할 수 있는 네트워크 인터페이스 장치가 없습니다.");
                return;
            }
            int i=0;
            // 네트워크 인터페이스 목록 출력
            Console.WriteLine("==========================네트워크 디바이스 목록============================");
            foreach (ICaptureDevice dev in alldevs)
            {
                string FriendName = dev.ToString().Split('\n')[1];              // dev를 \n단위로 짜르고 [1]번 인덱스를 FriendName에 저장
                FriendName = FriendName.Substring(FriendName.IndexOf(' ') + 1); // 그리고 띄어쓰기 + 1위치에서 잘라서 FriendName에 저장
                i++;
                Console.WriteLine("{0} : {1} / {2}",i, dev.Name, FriendName);
            }
            Console.WriteLine("============================================================================");
            /*
            아마 기본 C# 기본 인터페이스 인듯
            NetworkInterface[] alldev = NetworkInterface.GetAllNetworkInterfaces();
            i = 1;
            foreach (NetworkInterface dev in alldev)
            {
                Console.WriteLine("{0}번 :  {1} / {2} / {3}",i, dev.Name, dev.NetworkInterfaceType.ToString(), dev.Id);
                i++;
            }
            */
            Console.Write("Choice Network Interface (1~{0}): ", alldevs.Count);
            int idx;
            if(int.TryParse(Console.ReadLine(), out idx))
            {
                idx = idx - 1;
            }
            //Console.WriteLine("입력한 숫자는 : {0}\n", idx);

            // 입력한 디바이스를 저장합니다.
            ICaptureDevice choice_dev = alldevs[idx];

            Console.WriteLine("**** Choice Network Interface ****\n{0}", choice_dev);

            // 네트워크 디바이스 오픈
            choice_dev.Open(DeviceMode.Normal);
            // 패킷을 받아와서 Packet 변수에 담는다.
            RawCapture Packet = null;
            for (i = 0;i<10;i++)
            {
                try
                {
                    Packet = choice_dev.GetNextPacket();
                    // EthernetProtocol Print
                    ProtocolHeaderVeiw.PrintEthernetProtocol(Packet.Data, 0, 14);
                    // InternetProtocol Print
                    string protocol = ProtocolHeaderVeiw.PrintIPProtocol(Packet.Data, 14, 34);
                    Console.WriteLine("protocol : {0}", protocol);
                    // TCP
                    if (protocol == "TCP")
                    {
                        ProtocolHeaderVeiw.PrintTCP(Packet.Data, 34, 54);
                        ProtocolHeaderVeiw.PrintData(Packet.Data, 54, Packet.Data.Length);
                    }
                    else if (protocol == "UDP")
                    {
                        protocol = ProtocolHeaderVeiw.PrintUDP(Packet.Data, 34, 42);
                        if (protocol == "DNS")
                        {
                            Console.WriteLine("DNS Protocol");
                        }
                        else
                        {
                            ProtocolHeaderVeiw.PrintData(Packet.Data, 42, Packet.Data.Length);
                        }
                    }
                    // 패킷 헥스값 출력
                    
                    //HexaView.HexaViewText(Packet.Data);
                }
                catch
                {
                    Console.WriteLine("Not Pakcet");
                    continue;
                }
            }

            choice_dev.Close();






            /*
            // 패킷 도착 이벤트 핸들러 등록
            choice_dev.OnPacketArrival += Icd_OnPacketArrival;
            // 캡쳐 장치 열기
            choice_dev.Open(DeviceMode.Normal);

            // 캡쳐 시작
            choice_dev.StartCapture();

            Console.WriteLine("엔터 누르면 캡처를 멈춥니다.");
            Console.ReadLine();

            // 캡쳐 멈추기 및 캡쳐 장치 닫기
            choice_dev.StopCapture();
            choice_dev.Close();

            Console.WriteLine("{0}개의 패킷 {1} bytes 캡쳐하였습니다.", pcnt, total);

            */
        }
        // 패킷 도착할 때 이벤트 핸들러
        private static void Icd_OnPacketArrival(object sender, CaptureEventArgs e)

        {

            RawCapture rc = e.Packet;


            pcnt++;

            Console.WriteLine("{0}th {1}bytes", pcnt, rc.Data.Length);


                        total += rc.Data.Length;

        }

    
    }
}
