using System;
namespace Net
{
    public class RecivePacket : INetPacket
    {
        private byte[] buffer;
        private int writePos;
        private int headLength;

        public byte[] Buffer { get { return buffer; } }

        public bool IsWriteFull { get { return writePos == buffer.Length; } }

        public int HeadLen { get { return headLength; } }

        public RecivePacket(NetPacketHead packHead)
        {
            buffer = new byte[packHead.GetPackLen() + packHead.HeadLen];
            headLength = packHead.HeadLen;
            writePos = 0;
            Write(packHead.Buffer, 0, packHead.HeadLen);
        }

        public int Length
        {
           get{ return buffer.Length;}
        }

        public int Offset
        {
           get {return 0;}
        }

        public bool HasHead { get{ return true; } }

        public int Read(byte[] desBuffer, int desOffset, int srcOffset, int length)
        {
            if (srcOffset >= buffer.Length || srcOffset < 0)
                return 0;

            if (length + desOffset > desBuffer.Length)
                length = desBuffer.Length - desOffset;

            int iReadLen = length;
            if (srcOffset + length > buffer.Length)
            {
                iReadLen = buffer.Length - srcOffset;
            }
            Array.Copy(Buffer, srcOffset, desBuffer, desOffset, iReadLen);
            return iReadLen;
        }

        public int Write(byte[] bytes, int offset, int len)
        {
            int writeLen = len;
            int capLen = buffer.Length - writePos;
            if (len > capLen)
                writeLen = capLen;
            Array.Copy(bytes, offset, buffer, writePos, writeLen);
            writePos += writeLen;
            return writeLen;
        }

        public int Write(ReadBuffer readBuffer)
        {
            int capLen = buffer.Length - writePos;
            int readlen = readBuffer.Read(buffer, writePos, capLen);
            writePos += readlen;
            return readlen;
        }
    }
}
