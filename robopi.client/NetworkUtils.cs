using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace robopi.client
{
    static class NetworkUtils
    {
        #region Private fields

        private const ushort DiscoverPort = 30000;

        #endregion

        #region Public methods

        public static async Task<IReadOnlyList<IPEndPoint>> FindServers()
        {
            var ep = new IPEndPoint(IPAddress.Broadcast, DiscoverPort);
            var sendBuffer = new byte[] { 0xFF };
            using var udp = new UdpClient();

            HashSet<IPEndPoint> endPoints = new();

            await udp.SendAsync(sendBuffer, 1, ep);

            await Task.Delay(TimeSpan.FromMilliseconds(100));

            while (udp.Available > 0)
            {
                var receive = await udp.ReceiveAsync();
                var port = BitConverter.ToUInt16(receive.Buffer, 0);
                
                endPoints.Add(new IPEndPoint(receive.RemoteEndPoint.Address, port));

                await Task.Delay(TimeSpan.FromMilliseconds(100));
            }

            return endPoints.ToArray();
        }

        #endregion
    }
}
