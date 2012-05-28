
#if !defined( INCLUDED_FS_PATH_H )
#define INCLUDED_FS_PATH_H

#include "stream/stringstream.h"

/// \brief A unix-style path string which can be modified at runtime.
///
/// - Maintains a path ending in a path-separator.
/// - Provides a limited STL-style interface to push and pop file or directory names at the end of the path.
class UnixPath
{
StringBuffer m_string;

void check_separator(){
	if ( !empty() && m_string.back() != '/' ) {
		m_string.push_back( '/' );
	}
}

public:
/// \brief Constructs with the directory \p root.
UnixPath( const char* root )
	: m_string( root ){
	check_separator();
}

bool empty() const {
	return m_string.empty();
}

const char* c_str() const {
	return m_string.c_str();
}

/// \brief Appends the directory \p name.
void push( const char* name ){
	m_string.push_string( name );
	check_separator();
}
/// \brief Appends the directory [\p first, \p last).
void push( const char* first, const char* last ){
	m_string.push_range( first, last );
	check_separator();
}
/// \brief Appends the filename \p name.
void push_filename( const char* name ){
	m_string.push_string( name );
}
/// \brief Removes the last directory or filename appended.
void pop(){
	if ( m_string.back() == '/' ) {
		m_string.pop_back();
	}
	while ( !empty() && m_string.back() != '/' )
	{
		m_string.pop_back();
	}
}
};

#endif
