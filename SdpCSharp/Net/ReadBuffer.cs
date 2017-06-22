using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Net
{

    public class ReadBuffer
    {
        private byte[] _Buffer;
        private int _Offset;
        private int _Len;
        private int _CurPos;

        public bool IsOver { get { return _CurPos >= _Len + _Offset; } }

        internal void Set(byte[] buffer, int offset, int len)
        {
            _Buffer = buffer;
            _Offset = offset;
            _Len = len;
            _CurPos = offset;
        }

        internal void Clear()
        {
            _Buffer = null;
            _Offset = 0;
            _Len = 0;
            _CurPos = 0;
        }

        public int Read(byte[] desBuffer, int offset, int maxLen)
        {
            if (_Buffer == null)
                return 0;

            int capLen = _Offset + _Len - _CurPos;
            int readLen = maxLen;
            if (capLen < maxLen)
                readLen = capLen;
            Array.Copy(_Buffer, _CurPos, desBuffer, offset, readLen);
            _CurPos += readLen;
            return readLen;
        }
    }

}
