#include <rpc.h>

#include "uuid.h"
#include "smallsha1/sha1.h"

namespace SylUtil
{
	Uuid::Uuid()
		: m_uuid(0)
	{
	}

	bool Uuid::operator==(Uuid const& _other) const
	{
		return m_uuid == _other.m_uuid;
	}

	bool Uuid::operator!=(Uuid const& _other) const
	{
		return m_uuid != _other.m_uuid;
	}

	bool Uuid::operator<(Uuid const& _other) const
	{
		return m_uuid < _other.m_uuid;
	}

	bool Uuid::operator<=(Uuid const& _other) const
	{
		return m_uuid <= _other.m_uuid;
	}

	bool Uuid::operator>(Uuid const& _other) const
	{
		return m_uuid > _other.m_uuid;
	}

	bool Uuid::operator>=(Uuid const& _other) const
	{
		return m_uuid >= _other.m_uuid;
	}

	Uuid& Uuid::operator++()
	{
		++m_uuid;
		return *this;
	}

	Uuid Uuid::operator^(Uuid const& _other) const
	{
		Uuid result;
		result.m_uuid = m_uuid ^ _other.m_uuid;

		return result;
	}

	Uuid Uuid::distance(Uuid const& _other) const
	{
		Uuid result;
		result.m_uuid = _other.m_uuid - m_uuid; //Unsigned numbers can't overflow, but instead wrap around using the properties of modulo.

		return result;
	}

	void Uuid::sha1(std::string _str)
	{
		unsigned char hash[20];
		sha1::calc(_str.c_str(), _str.length(), hash);

		format(hash, 5);
	}

	void Uuid::setBit(unsigned char _bit)
	{
		m_uuid.SetBit(_bit);
	}

	void Uuid::setOnlyHighestBit()
	{
		ttmath::uint table_id;
		ttmath::uint index;

		if (m_uuid.FindLeadingBit(table_id, index))
		{
			m_uuid = 0;
			setBit(table_id * TTMATH_BITS_PER_UINT + index);
		}
	}

	void Uuid::setBitsUntilHighest()
	{
		ttmath::uint table_id;
		ttmath::uint index;

		if (m_uuid.FindLeadingBit(table_id, index))
		{
			m_uuid = 0;
			setBit(table_id * TTMATH_BITS_PER_UINT + index);

			m_uuid = m_uuid | (m_uuid - 1);
		}
	}

	void Uuid::setAllBits()
	{
		m_uuid.SetMax();
	}

	void Uuid::from(std::vector<unsigned char> _v, unsigned int _offset)
	{
		memcpy(&m_uuid, _v.data() + _offset, sizeof(m_uuid));
	}

	void Uuid::to(std::vector<unsigned char>& _v, unsigned int _offset) const
	{
		if (_v.size() < sizeof(m_uuid) + _offset)
		{
			_v.resize(sizeof(m_uuid) + _offset);
		}

		memcpy(_v.data() + _offset, &m_uuid, sizeof(m_uuid));
	}

	std::string Uuid::toString() const
	{
		UUID uuid;
		memcpy(&uuid, &m_uuid, sizeof(uuid));

		RPC_CSTR result;
		UuidToStringA(&uuid, &result);

		return reinterpret_cast<char*>(result);
	}

	std::string Uuid::numberToString() const
	{
		return m_uuid.ToString();
	}

	std::string Uuid::exponentToString() const
	{
		ttmath::Big<1, 2> result = 0;

		ttmath::uint table_id;
		ttmath::uint index;

		if (m_uuid.FindLeadingBit(table_id, index))
		{
			SylUtil::Uuid uuid;
			uuid.setBit(table_id * TTMATH_BITS_PER_UINT + index);

			result.Log(uuid.m_uuid, 2);
			result.Round(); //Avoid floating point errors
		}

		return result.ToString();
	}

	bool Uuid::isZero() const
	{
		return m_uuid.IsZero();
	}

	void Uuid::format(unsigned char _hash[16], int _version)
	{
		UUID uuid;
		memcpy(&uuid, _hash, sizeof(uuid));

		//Put in the variant and version bits
		uuid.Data3 &= 0x0FFF;
		uuid.Data3 |= (_version << 12);
		uuid.Data4[0] &= 0x3F;
		uuid.Data4[0] |= 0x80;

		memcpy(&m_uuid, &uuid, sizeof(m_uuid));
	}
};