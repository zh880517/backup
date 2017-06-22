#pragma once
#include <string>
/*
class IPacketHead
{
public:
	virtual ~IPacketHead() {}

	virtual int GetHeadLen() = 0;
	virtual int 
};
*/

class BufferRead
{
public:
	BufferRead(char* pData, uint32_t len)
		:m_pData(pData)
		,m_iLen(len)
	{
	}

	BufferRead(const std::string& strData)
		:m_pData(strData.data())
		,m_iLen(strData.length())
	{

	}

	BufferRead& Read(uint16_t & value)
	{
		value = 0;
		if (m_iLen >= 2)
		{
			value = m_pData[0]&0xff | m_pData[1] >> 8;
			m_pData += 2;
			m_iLen -= 2;
		}
		return *this;
	}

	BufferRead& Read(int16_t & value)
	{
		value = 0;
		if (m_iLen >= 2)
		{
			value = m_pData[0] & 0xff | m_pData[1] >> 8;
			m_pData += 2;
			m_iLen -= 2;
		}
		return *this;
	}

	BufferRead& Read(uint32_t & value)
	{
		value = 0;
		if (m_iLen >= 4)
		{
			for (auto i=0; i< 4; ++i)
			{
				value |= (m_pData[i] & 0xFF) << (i * 8);
			}
			m_pData += 4;
			m_iLen -= 4;
		}
		return *this;
	}

	BufferRead& Read(int32_t & value)
	{
		value = 0;
		if (m_iLen >= 4)
		{
			for (auto i = 0; i < 4; ++i)
			{
				value |= (m_pData[i] & 0xFF) << (i * 8);
			}
			m_pData += 4;
			m_iLen -= 4;
		}
		return *this;
	}

	BufferRead& Read(uint64_t & value)
	{
		value = 0;
		if (m_iLen >= 8)
		{
			for (auto i = 0; i < 8; ++i)
			{
				value |= ((uint64_t)(m_pData[i] & 0xFF)) << (i * 8);
			}
			m_pData += 8;
			m_iLen -= 8;
		}
		return *this;
	}

	BufferRead& Read(int64_t & value)
	{
		value = 0;
		if (m_iLen >= 8)
		{
			for (auto i = 0; i < 8; ++i)
			{
				value |= ((uint64_t)(m_pData[i] & 0xFF)) << (i * 8);
			}
			m_pData += 8;
			m_iLen -= 8;
		}
		return *this;
	}


private: 
	const char* m_pData;
	uint32_t m_iLen;
};

class BuffWriter
{
public:
	BuffWriter(std::string& buffer)
		:m_Buffer(buffer)
	{
	}

	BuffWriter& Write(uint16_t value)
	{
		m_Buffer.push_back((char)value);
		m_Buffer.push_back((char)(value >> 8));
		return *this;
	}

	BuffWriter& Write(int16_t value)
	{
		m_Buffer.push_back((char)value);
		m_Buffer.push_back((char)(value >> 8));
		return *this;
	}

	BuffWriter& Write(uint32_t value)
	{
		m_Buffer.push_back((char)value);
		m_Buffer.push_back((char)(value >> 8));
		m_Buffer.push_back((char)(value >> 16));
		m_Buffer.push_back((char)(value >> 24));
		return *this;
	}

	BuffWriter& Write(int32_t value)
	{
		m_Buffer.push_back((char)value);
		m_Buffer.push_back((char)(value >> 8));
		m_Buffer.push_back((char)(value >> 16));
		m_Buffer.push_back((char)(value >> 24));
		return *this;
	}

	BuffWriter& Write(uint64_t value)
	{
		m_Buffer.push_back((char)value);
		m_Buffer.push_back((char)(value >> 8));
		m_Buffer.push_back((char)(value >> 16));
		m_Buffer.push_back((char)(value >> 24));
		m_Buffer.push_back((char)(value >> 32));
		m_Buffer.push_back((char)(value >> 40));
		m_Buffer.push_back((char)(value >> 48));
		m_Buffer.push_back((char)(value >> 56));
		return *this;
	}

	BuffWriter& Write(int64_t value)
	{
		m_Buffer.push_back((char)value);
		m_Buffer.push_back((char)(value >> 8));
		m_Buffer.push_back((char)(value >> 16));
		m_Buffer.push_back((char)(value >> 24));
		m_Buffer.push_back((char)(value >> 32));
		m_Buffer.push_back((char)(value >> 40));
		m_Buffer.push_back((char)(value >> 48));
		m_Buffer.push_back((char)(value >> 56));
		return *this;
	}
private:
	std::string& m_Buffer;
};


class ClinetPacketHead
{
public:
	ClinetPacketHead() = default;

	ClinetPacketHead(uint16_t msgId, uint16_t len)
		:m_iDataLen(len)
		,m_iMsgId(msgId)
	{
	}

	uint16_t MsgId() { return m_iMsgId; }

	uint32_t DataLen() { return m_iDataLen; }

	uint32_t Headlen() { return 4; }

	void ReadFrom(BufferRead reader)
	{
		reader.Read(m_iMsgId).Read(m_iDataLen);
	}

	void WriteTo(BuffWriter writer)
	{
		writer.Write(m_iMsgId).Write(m_iDataLen);
	}

private:
	uint16_t m_iMsgId = 0;
	uint16_t m_iDataLen = 0;
};


class ServerPacketHead
{
public:
	ServerPacketHead() = default;
	ServerPacketHead(uint16_t msgId, uint16_t responId, uint16_t srcId, uint16_t destId, uint32_t dataLen)
		:m_iMsgId(msgId)
		,m_iResponId(responId)
		,m_iSrcId(srcId)
		,m_iDestId(destId)
		,m_iDataLen(dataLen)
	{
	}
	uint32_t Headlen() { return 12; }

	uint16_t MsgId() { return m_iMsgId; }

	uint16_t ResponId() { return m_iResponId; }

	uint16_t SrcId() { return m_iSrcId; }

	uint16_t DestId() { return m_iDestId; }

	uint32_t DataLen() { return m_iDataLen; }


	void ReadFrom(BufferRead reader)
	{
		reader.Read(m_iMsgId).Read(m_iResponId).Read(m_iSrcId).Read(m_iDestId).Read(m_iDataLen);
	}

	void WriteTo(BuffWriter writer)
	{
		writer.Write(m_iMsgId).Write(m_iResponId).Write(m_iSrcId).Write(m_iDestId).Write(m_iDataLen);
	}

private:
	uint16_t m_iMsgId = 0;
	uint16_t m_iResponId = 0;
	uint16_t m_iSrcId = 0;
	uint16_t m_iDestId = 0;
	uint32_t m_iDataLen = 0;
};