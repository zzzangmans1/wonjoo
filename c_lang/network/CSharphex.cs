using System;


namespace HexaViewer
{
    public class HexaView
    {
        static void PrintLine(char Char, ushort Count)
        {
            for (int Temp = 0; Temp < Count; ++Temp)
            {
                Console.Write(Char);
            }
            Console.WriteLine();
        }
        static void PrintHead(char Line1, char Line2, ushort Count)
        {
            PrintLine(Line1, Count);
            PrintLine(Line2, Count);
            Console.Write(" ADDRESS ");

            for (int iCount = 0; iCount < 16; ++iCount)
            {
                Console.Write("{0:X02} ", iCount);
            }

            for (int iCount = 0; iCount < 16; ++iCount)
            {
                Console.Write("{0:X01}", iCount);
            }
            Console.WriteLine();
            PrintLine(Line1, Count);
        }
        static void PrintHead()
        {
            PrintHead('=', '-', 73);
        }
        static void PrintEnd()
        {
            PrintLine('=', 73);
        }
        public static void HexaViewText(byte[] Data)
        {
            ArraySegment<byte> TempDataPos;
            int iLoopTotal = Data.Length / 16;
            int iLoopRemain = Data.Length % 16;

            PrintHead();

            for (int iLoopCount = 0; iLoopCount < iLoopTotal; iLoopCount = iLoopCount + 1)
            {
                Console.Write("{0:X08} ", iLoopCount * 16);

                TempDataPos = new ArraySegment<byte>(Data, iLoopCount * 16, 16);
                foreach (var Temp in TempDataPos)
                {
                    Console.Write("{0:X02} ", Temp);
                }

                foreach (char Temp in TempDataPos)
                {
                    if (' ' > Temp)
                    {
                        Console.Write(".");
                    }
                    else if (127 < Temp)
                    {
                        Console.Write(".");
                    }
                    else
                    {
                        Console.Write("{0}", Temp);
                    }
                }
                Console.WriteLine();
            }
            if (0 != iLoopRemain)
            {
                iLoopTotal = iLoopTotal * 16;
                Console.Write("{0:X08} ", iLoopTotal);

                TempDataPos = new ArraySegment<byte>(Data, iLoopTotal, iLoopRemain);
                foreach (var Temp in TempDataPos)
                {
                    Console.Write("{0:X02} ", Temp);
                }
                for (int Temp = 0; Temp < 16 - iLoopRemain; ++Temp)
                {
                    Console.Write("   ");
                }


                foreach (char Temp in TempDataPos)
                {
                    if (' ' > Temp)
                    {
                        Console.Write(".");
                    }
                    else if (127 < Temp)
                    {
                        Console.Write(".");
                    }
                    else
                    {
                        Console.Write("{0}", Temp);
                    }
                }

                for (int Temp = 0; Temp < 16 - iLoopRemain; ++Temp)
                {
                    Console.Write(" ");
                }
            }
            Console.WriteLine();

            PrintEnd();
        }
    }
}
