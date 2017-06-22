using System;
using System.Net.Sockets;

namespace Net
{
    public class WriteBuffer
    {
        private INetPacket Packet;
        private int WritePos;
        private byte[] HeadBuffer = new byte[6];
        private int HeadLenth;
        private int HeadWritePos;

        public void SetPacket(INetPacket packet)
        {
            if (Packet == packet)
                return;
            Packet = packet;
            WritePos = 0;
            HeadLenth = 0;
            HeadWritePos = 0;
            if (packet != null)
            {
                WriteLength();
            }
        }

        public bool IsComplete()
        {
            if (Packet == null)
                return true;
            return HeadWritePos>= HeadLenth && (Packet.Length + Packet.Offset) <= WritePos;
        }

        public void Write(SocketAsyncEventArgs sendSAEA)
        {
            if (Packet == null)
                return;
            int writeLen = 0;
            while (writeLen < sendSAEA.Buffer.Length)
            {
                if (HeadWritePos >= HeadLenth)
                {
                    int len = Packet.Read(sendSAEA.Buffer, writeLen, WritePos, sendSAEA.Buffer.Length);
                    WritePos += len;
                    writeLen += len;
                    break;
                }
                else
                {
                    int len = HeadLenth - HeadWritePos;
                    if (len > sendSAEA.Buffer.Length)
                        len = sendSAEA.Buffer.Length;
                    Array.Copy(HeadBuffer, HeadWritePos, sendSAEA.Buffer, writeLen, len);
                    writeLen += len;
                    HeadWritePos += len;
                }
            }
            sendSAEA.SetBuffer(0, writeLen);
        }
        private void WriteLength()
        {
            if (!Packet.HasHead)
            {
                int len = Packet.Length;
                if (len < 0xFFFF)
                {
                    HeadBuffer[0] = (byte)len;
                    HeadBuffer[1] = (byte)(len >> 8);
                    HeadLenth = 2;
                }
                else
                {
                    HeadBuffer[0] = 0xFF;
                    HeadBuffer[1] = 0xFF;
                    HeadBuffer[2] = (byte)len;
                    HeadBuffer[3] = (byte)(len >> 8);
                    HeadBuffer[4] = (byte)(len >> 16);
                    HeadBuffer[5] = (byte)(len >> 24);
                    HeadLenth = 6;
                }
            }
        }
    }
}
