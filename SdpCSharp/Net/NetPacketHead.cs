using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Net
{
    public class NetPacketHead
    {
        private byte[] _Buffer = new byte[6];
        private int _CurPos = 0;
        private bool _isBigPacket = false;

        public byte[] Buffer { get { return _Buffer; } }

        public bool IsOver { get { return _isBigPacket ? _CurPos == 6 : _CurPos == 2; } }

        public int HeadLen { get { return _isBigPacket ? 6 : 2; } }

        public void Reset()
        {
            _CurPos = 0;
            _isBigPacket = false;
        }

        public int GetPackLen()
        {
            if (!_isBigPacket)
            {
                return _Buffer[0] | _Buffer[1] << 8;
            }
            else
            {
                return _Buffer[2] | _Buffer[3] << 8 | _Buffer[4] << 16 | _Buffer[5] << 24;
            }
        }

        public bool Write(ReadBuffer readBuffer)
        {
            while (!readBuffer.IsOver)
            {
                int readLen = 0;
                if (_CurPos < 2)
                {
                    readLen = readBuffer.Read(_Buffer, _CurPos, 2 - _CurPos);
                    _CurPos += readLen;
                    if (_CurPos < 2)
                        break;
                   if (Buffer[0] != 0xFF && Buffer[1] != 0xFF)
                        break;
                    _isBigPacket = true;
                }
                else
                {
                    readLen = readBuffer.Read(_Buffer, _CurPos, 6 - _CurPos);
                    _CurPos += readLen;
                    if (_CurPos >= 6)
                        break;
                }
            }
            
            return IsOver;
        }
    }
}
