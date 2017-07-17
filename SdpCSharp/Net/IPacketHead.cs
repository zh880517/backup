using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Net
{
    public interface IPacketHead
    {
        /// <summary>
        /// 包头长度
        /// </summary>
        int HeadLen { get; }

        /// <summary>
        /// 根据包头计算数据长度
        /// </summary>
        /// <param name="buffer">包头数据</param>
        /// <returns>数据长度</returns>
        int CalDateLen(byte[] buffer);

        /// <summary>
        /// 校验包头
        /// </summary>
        /// <param name="buffer">包头</param>
        /// <returns>是否通过校验</returns>
        //bool Verify(byte[] buffer);
    }


}
