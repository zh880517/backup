using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RealTimeRank
{
    public interface IRankUnit<TKey>
    {
        TKey Key { get; }
        long RankValue { get; set; }
        int RankIndex { get; set; }
    }

    public class TestRankUnit : IRankUnit<long>
    {
        public long Key
        {
            get
            {
                return ID;
            }
        }

        public int RankIndex
        {
            get
            {
                return Rank;
            }

            set
            {
                Rank = value;
            }
        }

        public long RankValue
        {
            get
            {
                return Value;
            }

            set
            {
                Value = value;
            }
        }

        public long ID;
        public int Rank;
        public long Value;
        
    }
}
