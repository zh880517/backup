using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Net
{
    public interface INetPacket
    {
        /// <summary>
        /// 从缓存中读取数据
        /// </summary>
        /// <param name="bytes"></param>
        /// <param name="offset"></param>
        /// <param name="length"></param>
        /// <returns>实际读取的长度</returns>
        int Read(byte[] desBuffer, int desOffset, int srcOffset, int length);

        /// <summary>
        /// 缓冲区的起始偏移
        /// </summary>
        int Offset { get; }

        /// <summary>
        /// 缓冲区长度
        /// </summary>
        int Length { get; }

        bool HasHead { get; }
        
    }

    
}
