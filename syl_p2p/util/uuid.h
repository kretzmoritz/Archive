///////////////////////////////////////
//  (             (                
//  )\ )     (    )\ )             
// (()/((    )\  (()/(    (   )    
//  /(_))\ )((_)  /(_))  ))\ /((   
// (_))(()/( _   (_))_  /((_|_))\  
// / __|)(_)) |   |   \(_)) _)((_) 
// \__ \ || | |   | |) / -_)\ V /  
// |___/\_, |_|   |___/\___| \_/   
//      |__/                       
//
// 2014 Moritz Kretz
///////////////////////////////////////

#ifndef SYLUTIL_UUID_H
#define SYLUTIL_UUID_H

#include <ttmath/ttmath.h>
#include <string>
#include <vector>

namespace SylUtil
{
	class Uuid
	{
	public:
		Uuid();

		bool operator==(Uuid const& _other) const;
		bool operator!=(Uuid const& _other) const;
		bool operator<(Uuid const& _other) const;
		bool operator<=(Uuid const& _other) const;
		bool operator>(Uuid const& _other) const;
		bool operator>=(Uuid const& _other) const;
		
		Uuid& operator++();
		Uuid operator^(Uuid const& _other) const;
		Uuid distance(Uuid const& _other) const;

		void sha1(std::string _str);
		void setBit(unsigned char _bit);
		void setOnlyHighestBit();
		void setBitsUntilHighest();
		void setAllBits();
		void from(std::vector<unsigned char> _v, unsigned int _offset = 0);
		void to(std::vector<unsigned char>& _v, unsigned int _offset = 0) const;
		std::string toString() const;
		std::string numberToString() const;
		std::string exponentToString() const;
		bool isZero() const;

	private:
		void format(unsigned char _hash[16], int _version);

		ttmath::UInt<TTMATH_BITS(128)> m_uuid;
	};
};

#endif