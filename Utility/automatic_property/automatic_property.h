#ifndef AUTOMATIC_PROPERTY_H
#define AUTOMATIC_PROPERTY_H

#ifdef _MSC_EXTENSIONS
	#define USE_MS_PROPERTY(x) x
#else
	#define USE_MS_PROPERTY(x)
#endif

// Defines a class property with read and write access, creating default getter and setter methods.
#define DEFAULT_PROPERTY_RW(access_modifier, type, name, modifier) \
	access_modifier: \
		modifier type& get##name() { return m_##name; } \
		modifier void set##name(type _value) { m_##name = _value; } \
		USE_MS_PROPERTY(_declspec(property(get = get##name, put = set##name)) type name;) \
	private: \
		modifier type m_##name; \
	access_modifier:

// Defines a class property with read access, creating a default getter method.
#define DEFAULT_PROPERTY_R(access_modifier, type, name, modifier) \
	access_modifier: \
		modifier type& get##name() { return m_##name; } \
		USE_MS_PROPERTY(_declspec(property(get = get##name)) type name;) \
	private: \
		modifier type m_##name; \
	access_modifier:

// Defines a class property with write access, creating a default setter method.
#define DEFAULT_PROPERTY_W(access_modifier, type, name, modifier) \
	access_modifier: \
		modifier void set##name(type _value) { m_##name = _value; } \
		USE_MS_PROPERTY(_declspec(property(put = set##name)) type name;) \
	private: \
		modifier type m_##name; \
	access_modifier:

// Defines a class property with read and write access.
#define CUSTOM_PROPERTY_RW(access_modifier, type, name, modifier) \
	access_modifier: \
		modifier type& get##name() { return m_##name; } \
		modifier void set##name(type _value) { m_##name = _value; } \
		USE_MS_PROPERTY(_declspec(property(get = get##name, put = set##name)) type name;) \
	private: \
		modifier type m_##name; \
	access_modifier:

// Defines a class property with read access.
#define CUSTOM_PROPERTY_R(access_modifier, type, name, modifier) \
	access_modifier: \
		modifier type& get##name() { return m_##name; } \
		USE_MS_PROPERTY(_declspec(property(get = get##name)) type name;) \
	private: \
		modifier type m_##name; \
	access_modifier:

// Defines a class property with write access.
#define CUSTOM_PROPERTY_W(access_modifier, type, name, modifier) \
	access_modifier: \
		modifier void set##name(type _value) { m_##name = _value; } \
		USE_MS_PROPERTY(_declspec(property(put = set##name)) type name;) \
	private: \
		modifier type m_##name; \
	access_modifier:

#endif