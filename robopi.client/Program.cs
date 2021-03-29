using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace robopi.client
{
    class Program
    {
        #region Nested classes

        private struct EndPointEntry
        {
            #region Properties

            public IPEndPoint EndPoint { get; }

            public string Name { get; }

            #endregion

            #region ctor

            public EndPointEntry(IPEndPoint endPoint, string name)
            {
                EndPoint = endPoint;
                Name = name;
            }

            #endregion
        }

        #endregion

        static async Task Main(string[] args)
        {
            var endPoint = await SearchRobopiServer();

            if(endPoint == null)
                return;

            using var tcpClient = new TcpClient("raspberrypi", 5050);
            using var networkStream = tcpClient.GetStream();
            var buffer = new byte[4096];

            while (true)
            {
                Console.Write("Input message: ");

                var line = Console.ReadLine();
                if(string.IsNullOrWhiteSpace(line))
                    break;

                var sendBuffer = Encoding.UTF8.GetBytes(line);
                await networkStream.WriteAsync(sendBuffer, 0, sendBuffer.Length);


                var count = await networkStream.ReadAsync(buffer, 0, buffer.Length);

                Console.WriteLine("Answer: {0}", Encoding.UTF8.GetString(buffer, 0, count));
            }

        }

        private static async Task<IPEndPoint> SearchRobopiServer()
        {
            Console.WriteLine(Resources.SearchingRobopuServer);

            var endPoints = (await NetworkUtils.FindServers());
            var entries = new EndPointEntry[endPoints.Count];

            for (int p = 0; p < entries.Length; p++)
            {
                var e = await Dns.GetHostEntryAsync(endPoints[p].Address);
                var name = string.IsNullOrWhiteSpace(e.HostName)
                    ? endPoints[p].Address.ToString()
                    : e.HostName;
                entries[p] = new EndPointEntry(endPoints[p], name);
            }

            if (endPoints.Count > 1)
            {
                var line = string.Empty;
                do
                {
                    Console.WriteLine("{0}:", Resources.SelectRobopiServer);

                    for (int i = 0; i < entries.Length; i++)
                    {
                        var ipEndPoint = entries[i];
                        var endPointName = $"{ipEndPoint.Name}:{ipEndPoint.EndPoint.Port}";
                        Console.WriteLine(Resources.FindedRobopuServerFormat, i + 1, endPointName);
                    }
                    
                    line = Console.ReadLine();

                    if (int.TryParse(line, out var n)
                        && n > 0
                        && n <= endPoints.Count)
                        return endPoints[n - 1];

                } while (line != "q");

                return null;
            }
            else
                return endPoints.FirstOrDefault();
        }
    }
}
