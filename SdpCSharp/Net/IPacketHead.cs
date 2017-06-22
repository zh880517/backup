using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Net
{
    public interface IPacketHead
    {
        int HeadLen { get; }
        int DataLen { get; }
        byte[] HeadBuffer { get; }
    }


}
