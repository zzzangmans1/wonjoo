using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;
namespace Example
{
    class Program
    {
        // 실행 함수
        static void Main(string[] args)
        {

            string filepath = "C:\\Users\\lenovo\\Desktop\\";
            StreamWriter sw;
            // 현재 시간 받아오기
            string d = DateTime.Now.ToString("yyyy-MM-dd/hh_mm_ss");
            d += ".txt";
            filepath += d;
            Console.WriteLine(filepath);

            // 포트 9090으로 서버 설정
            IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 9000);
            Socket server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            // bind하고 서버 Listen 대기
            server.Bind(ipep);
            server.Listen(20);
            // 콘솔 출력
            Console.WriteLine("Server Start");
            // 다중 접속 시작
            while (true)
            {
                // ThreadPool로 접속이 되면
                ThreadPool.QueueUserWorkItem((_) =>
                {
                    // 메시지 버퍼
                    StringBuilder sb = new StringBuilder();
                    // 클라이언트 받기
                    Socket client = (Socket)_;
                    // 클라이언트 정보
                    IPEndPoint ip = (IPEndPoint)client.RemoteEndPoint;
                    // 콘솔 출력
                    Console.WriteLine("Client connected IP address = {0} : {1}", ip.Address, ip.Port);
                    // 메시지 전송
                    client.Send(Encoding.Unicode.GetBytes("Welcome server!\r\n>\0"), SocketFlags.None);
                    try
                    {
                        // 수신 대기
                        while (true)
                        {
                            // 서버로 오는 메시지를 받는다.
                            byte[] ret = new byte[2];
                            client.Receive(ret, 2, SocketFlags.None);
                            // 메시지를 unicode로 변환해서 버퍼에 넣는다.
                            sb.Append(Encoding.Unicode.GetString(ret, 0, 2));
                            // 개행 + \n이면 콘솔 출력한다.
                            if (sb.Length >= 2 && sb[sb.Length - 2] == '\r' && sb[sb.Length - 1] == '\n')
                            {
                                // exit면 접속을 끊는다.
                                if (sb.Length >= 4 && sb[sb.Length - 4] == 'e' && sb[sb.Length - 3] == 'x' && sb[sb.Length - 2] == 'i' && sb[sb.Length - 1] == 't')
                                {
                                    break;
                                }
                                // 버퍼의 메시지를 콘솔에 출력
                                string msg = sb.ToString();
                                // echo 메시지 전송
                                client.Send(Encoding.Unicode.GetBytes("echo - " + msg + ">\0"), SocketFlags.None);
                                // 콘솔 출력
                                Console.Write(msg);
                                sw = File.AppendText(filepath);
                                sw.WriteLine(msg);
                                // 버퍼를 비운다.
                                sb.Clear();
                                sw.Close();
                            }
                        }
                    }
                    catch
                    {
                        // 에러 발생하면 종료
                    }
                    // 접속 종료 메시지 콘솔 출력
                    Console.WriteLine("Client disconnected IP address = {0} : {1}", ip.Address, ip.Port);
                    // 클라이언트와 접속이 되면 Thread 생성
                }, server.Accept());
            }
            server.Close();
        }
    }
}
