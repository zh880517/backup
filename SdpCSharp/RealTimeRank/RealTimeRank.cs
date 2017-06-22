using System;
using System.Collections.Generic;

namespace RealTimeRank
{
    public abstract class RealTimeRank<TKey>
    {
        protected List<IRankUnit<TKey>> Ranks = new List<IRankUnit<TKey>>();
        protected Dictionary<TKey, IRankUnit<TKey>> Indexs = new Dictionary<TKey, IRankUnit<TKey>>();

        public void AddValue(TKey key, long addValue)
        {
            var unit = GetUnitWithCreate(key);
            unit.RankValue += addValue;
            if (addValue < 0)
            {
                for (int i=unit.RankIndex - 1; i<Ranks.Count; ++i)
                {
                    var front = GetUnitByIndex(i);
                    var back = GetUnitByIndex(i + 1);
                    if (front == null || back == null || front.RankValue > back.RankValue)
                        break;
                    Swap(front, back);
                }
            } 
            else
            {
                for (int i = unit.RankIndex - 1; i > 0; --i)
                {
                    var front = GetUnitByIndex(i - 1);
                    var back = GetUnitByIndex(i);
                    if (front == null || back == null || front.RankValue > back.RankValue)
                        break;
                    Swap(front, back);
                }
            }
        }

        public void SetValue(TKey key, long newValue)
        {
            var unit = GetUnitWithCreate(key);
            long oldValue = unit.RankValue;
            unit.RankValue = newValue;
            if (oldValue > newValue)
            {
                for (int i = unit.RankIndex - 1; i < Ranks.Count; ++i)
                {
                    var front = GetUnitByIndex(i);
                    var back = GetUnitByIndex(i + 1);
                    if (front == null || back == null || front.RankValue > back.RankValue)
                        break;
                    Swap(front, back);
                }
            }
            else
            {
                for (int i = unit.RankIndex - 1; i > 0; --i)
                {
                    var front = GetUnitByIndex(i - 1);
                    var back = GetUnitByIndex(i);
                    if (front == null || back == null || front.RankValue > back.RankValue)
                        break;
                    Swap(front, back);
                }
            }
        }

        public void Check()
        {
            for (int i=1; i<Ranks.Count; ++i)
            {
                var front = GetUnitByIndex(i - 1);
                var back = GetUnitByIndex(i);
                if (front.RankValue < back.RankValue)
                {
                    Console.WriteLine(front.RankIndex);
                }
            }
        }

        protected IRankUnit<TKey> GetUnitWithCreate(TKey key)
        {

            IRankUnit<TKey> unit = null;
            if (!Indexs.TryGetValue(key, out unit))
            {
                unit = CreateUnit(key, 0);
                Indexs.Add(key, unit);
                Ranks.Add(unit);
                unit.RankIndex = Ranks.Count;
            }

            return unit;
        }

        protected IRankUnit<TKey> GetUnitByIndex(int iIndex)
        {
            if (iIndex >= 0&& iIndex < Ranks.Count)
            {
                return Ranks[iIndex];
            }
            return null;
        }

        private void Swap(IRankUnit<TKey> unit1, IRankUnit<TKey> unit2)
        {
            int temp = unit1.RankIndex;
            unit1.RankIndex = unit2.RankIndex;
            unit2.RankIndex = temp;
            Ranks[unit1.RankIndex - 1] = unit1;
            Ranks[unit2.RankIndex - 1] = unit2;
        }

        protected abstract IRankUnit<TKey> CreateUnit(TKey key, long initValue);
    }
}
