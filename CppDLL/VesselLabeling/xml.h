/**
* @file xml.h
* @brief XMLNXÌwb_t@C
* @version 3.0
* @author ¼Ã®åw îñÈw¤È fBAÈwêU X¤º   351203192   ¼ú± NN
* @date 2013/10/18
*/

#ifndef _TMATSU_XML_H_
#define _TMATSU_XML_H_

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cctype>
#include "dictionary.h"
#include "tree.h"

#define DEFAULT_VERSION "1.0" ///< @brief version®«Ìùèl
#define DEFAULT_ENCODING "shift-jis" ///< @brief encoding®«Ìùèl
#define DEFAULT_STANDALONE "yes" ///< @brief standalone®«Ìùèl

namespace tmatsu
{
	/// @brief vfðeLXgÌ®IzñÆµÄµ¤NX
	class _Element : public std::vector< std::string >
	{
	public:
		/// @brief ^Oð®«ÌAzzñÆµÄµ¤NX
		class Tag : public tmatsu::dictionary< std::string, std::string >
		{
		public:
			typedef invalid_key InvalidProperty; ///< @brief ³ø®«QÆáO
		private:
			std::string name_; ///< @brief ^O¼
		public:
			Tag( );
			Tag( const Tag &tag );
			Tag( const std::string &name );
			std::string &name( );
			const std::string &name( )const;
		};
	private:
		Tag tag_; ///< @brief ^O
	public:
		_Element( );
		_Element( const _Element &elem );
		_Element( const Tag &tag );
		_Element( const std::string &tag_name );
		Tag &tag( );
		const Tag &tag( )const;
		bool operator==( const std::string &tag_name )const;
	};

	/// @brief XMLðØ\¢ÆµÄµ¤NX
	class XML : public tree< _Element >
	{
	public:
		typedef _Element Element; ///< @brief vfðµ¤NX
	private:
		std::string version_; ///< @brief version®«
		std::string encoding_; ///< @brief encoding®«
		std::string standalone_; ///< @brief standalone®«
	public:
		XML( );
		XML( const XML &xml );
		XML( const char *file_name );
		XML( const std::string &version, const std::string &encoding, const std::string &standalone );
		~XML( );
		std::string &version( );
		const std::string &version( )const;
		std::string &encoding( );
		const std::string &encoding( )const;
		std::string &standalone( );
		const std::string &standalone( )const;
		bool read( const char *file_name );
		bool write( const char *file_name )const;
	private:
		iterator read( std::stringstream &ss, iterator elem );
		void write( std::ofstream &ofs, const_iterator elem )const;
		static void get_str( std::stringstream &ss, std::string &str, char delim, bool term_with_space = false );
	};
	
	/// @brief e®«ðùèlÅú»·é
	inline XML::XML( ) : version_( DEFAULT_VERSION ), encoding_( DEFAULT_ENCODING ), standalone_( DEFAULT_STANDALONE )
	{
	}
	
	/// @brief XMLð¡»·é
	/// @param[in] xml ¡»·éXML
	inline XML::XML( const XML &xml ) : version_( xml.version_ ), encoding_( xml.encoding_ ), standalone_( xml.standalone_ ), tree< _Element >( xml )
	{
	}
	
	/// @brief XMLðt@C©çÇÝÞ
	/// @attention XMLt@CÌàeÉæÁÄÍ³íÉÇÝßÈ¢Â\«ª é
	/// @param[in] file_name ÇÝÞXMLt@C¼
	inline XML::XML( const char *file_name ) : version_( DEFAULT_VERSION ), encoding_( DEFAULT_ENCODING ), standalone_( DEFAULT_STANDALONE )
	{
		read( file_name );
	}
	
	/// @brief e®«ðÝè·é
	/// @param[in] version version®«
	/// @param[in] encoding encoding®«
	/// @param[in] standalone standalone®«
	inline XML::XML( const std::string &version, const std::string &encoding, const std::string &standalone ) : version_( version ), encoding_( encoding ), standalone_( standalone )
	{
	}

	/// @brief ®ìÈµ
	inline XML::~XML( )
	{
	}
	
	/// @brief version®«ðæ¾·é
	/// @return version®«ÌQÆ
	inline std::string &XML::version( )
	{
		return version_;
	}
	
	/// @brief version®«ðæ¾·é
	/// @return version®«ÌèQÆ
	inline const std::string &XML::version( )const
	{
		return version_;
	}
	
	/// @brief encoding®«ðæ¾·é
	/// @return encoding®«ÌQÆ
	inline std::string &XML::encoding( )
	{
		return encoding_;
	}
	
	/// @brief encoding®«ðæ¾·é
	/// @return encoding®«ÌèQÆ
	inline const std::string &XML::encoding( )const
	{
		return encoding_;
	}
		
	/// @brief standalone®«ðæ¾·é
	/// @return standalone®«ÌQÆ
	inline std::string &XML::standalone( )
	{
		return standalone_;
	}
		
	/// @brief standalone®«ðæ¾·é
	/// @return standalone®«ÌèQÆ
	inline const std::string &XML::standalone( )const
	{
		return standalone_;
	}

	/// @brief XMLðt@C©çÇÝÞ
	/// @attention XMLt@CÌàeÉæÁÄÍ³íÉÇÝßÈ¢Â\«ª é
	/// @param[in] file_name ÇÝÞXMLt@C¼
	/// @retval true t@CÌÇÝÝÉ¬÷µ½
	/// @retval false t@CÌÇÝÝÉ¸sµ½D°ç­Ct@CªJ¯È©Á½
	inline bool XML::read( const char *file_name )
	{
		std::ifstream ifs( file_name ); // üÍt@CXg[
		std::stringstream ss; // üÍ¶ñXg[
		std::string buf; // obt@

		// G[`FbN
		if ( !ifs )
		{
			return false;
		}

		// ½Ì©t@CðSÄÇÝÜÈ¢ÆG[ª¶¶é°êª é
		while ( !ifs.eof( ) )
		{
			ss << static_cast< char >( ifs.get( ) );
		}

		// Ø\¢ðóÉ·é
		clear( );

		// u<?xmlvðÇÝòÎ·
		ss.ignore( INT_MAX, 'l' );

		// ®«ðÇÝÞ
		while ( true )
		{
			// óðÇÝòÎ·
			while ( std::isspace( ss.peek( ) ) )
			{
				ss.ignore( );
			}

			if ( ss.peek( ) != '?' )
			{
				// ®«Ì¼OðÇÝÞ
				get_str( ss, buf, '=', true );

				//u ="vðÇÝòÎ·
				ss.ignore( INT_MAX, '\"' );

				if ( buf == "version" )
				{
					// version®«ðÇÝÞ
					get_str( ss, version_, '\"' );
				}
				else if ( buf == "encoding" )
				{
					// encoding®«ðÇÝÞ
					get_str( ss, encoding_, '\"' );
				} 
				else if ( buf == "standalone" )
				{
					// standalone®«ðÇÝÞ
					get_str( ss, standalone_, '\"' );
				}
					
				// u"vðÇÝòÎ·
				ss.ignore( INT_MAX, '\"' );
			}
			else
			{
				// u?>vðÇÝòÎ·
				ss.ignore( INT_MAX, '>' );

				break;
			}
		}

		// vfðÇÝÞ
		read( ss, insert( end( ), Element( ) ) );

		return true;
	}

	/// @brief XMLðt@CÉ«Þ
	/// @param[in] file_name «ÞXMLt@C¼
	/// @retval true «ÝÉ¬÷µ½
	/// @retval false «Ýµ¸sµ½D°ç­Ct@CªJ¯È©Á½
	inline bool XML::write( const char *file_name )const
	{
		std::ofstream ofs( file_name ); // oÍt@CXg[

		// G[`FbN
		if ( !ofs )
		{
			return false;
		}

		// XMLé¾ð«Þ
		ofs << "<?xml";
		
		if ( !version_.empty( ) )
		{
			ofs << " version=\"" << ( version_ != "1.1" ? "1.0" : "1.1" ) << '\"';
		}
		
		if ( !encoding_.empty( ) )
		{
			ofs << " encoding=\"" << encoding_ << '\"';
		}
		
		if ( !standalone_.empty( ) )
		{
			ofs << " standalone=\"" << ( standalone_ != "no" ? "yes" : "no" ) << '\"';
		}

		ofs << "?>" << std::endl;

		// vfð«Þ
		write( ofs, preorder_begin( ) );

		return true;
	}

	/// @brief vfðXg[©çÇÝÞ
	/// @param[in] ss ÇÝÞüÍXg[
	/// @param[in] elem ÇÝÞvfðw·Ce[^
	/// @return ÇÝñ¾vfðw·Ce[^
	inline XML::iterator XML::read( std::stringstream &ss, iterator elem )
	{
		bool cont = true; // p±tO

		// u<vðÇÝòÎ·
		ss.ignore( INT_MAX, '<' );

		// ^OðÇÝÞ
		get_str( ss, elem->tag( ).name( ), '>', true );

		if ( !elem->tag( ).name( ).empty( ) && elem->tag( ).name( )[ elem->tag( ).name( ).size( ) - 1 ] == '/' )
		{
			// ó^OÌÆ«
			elem->tag( ).name( ).resize( elem->tag( ).name( ).size( ) - 1 );

			return elem;
		}

		// ®«ðÇÝÞ
		while ( cont )
		{
			// óðÇÝòÎ·
			while ( std::isspace( ss.peek( ) ) )
			{
				ss.ignore( );
			}

			switch ( ss.peek( ) )
			{
			case '/':
				// u/>vðÇÝòÎ·
				ss.ignore( INT_MAX, '>' );

				return elem;
			case '>':
				// u>vðÇÝòÎ·
				ss.ignore( INT_MAX, '>' );
				cont = false;
					
				break;
			default:
				std::string prop, val; // obt@

				// ®«Ì¼OðÇÝÞ
				get_str( ss, prop, '=', true );

				// u="vðÇÝòÎ·
				ss.ignore( INT_MAX, '\"' );

				// ®«ÌlðÇÝÞ
				get_str( ss, val, '\"' );

				// u"vðÇÝòÎ·
				ss.ignore( INT_MAX, '\"' );

				// ®«ðÝè·é
				elem->tag( ).insert( elem->tag( ).end( ), Element::Tag::value_type( prop, val ) );
			}
		}

		// àeCRgCI¹^OÜ½ÍqvfðÇÝÞ
		while ( true )
		{
			// óðÇÝòÎ·
			while ( std::isspace( ss.peek( ) ) )
			{
				ss.ignore( );
			}

			if ( ss.peek( ) == '<' )
			{
				// u<vðÇÝòÎ·
				ss.ignore( INT_MAX, '<' );

				if ( ss.peek( ) == '/' )
				{
					// I¹^OðÇÝòÎ·
					ss.ignore( INT_MAX, '>' );

					break;
				}
				else if ( ss.peek( ) == '!' )
				{
					char temp[ 4 ]; // êÌæ

					ss.get( temp, 4 );
					temp[ 3 ] = '\0';

					if ( std::strcmp( temp, "!--" ) )
					{
						// qvfðÇÝÞ
						ss.putback( temp[ 2 ] );
						ss.putback( temp[ 1 ] );
						ss.putback( temp[ 0 ] );
						ss.putback( '<' );
						elem = parent( read( ss, insert( elem, Element( ) ) ) );
					}
					else
					{
						// RgðÇÝòÎ·
						ss.get( temp, 4 );

						while ( std::strcmp( temp, "-->" ) )
						{
							temp[ 0 ] = temp[ 1 ];
							temp[ 1 ] = temp[ 2 ];
							temp[ 2 ] = ss.get( );
						}
					}
				}
				else
				{
					// qvfðÇÝÞ
					ss.putback( '<' );
					elem = parent( read( ss, insert( elem, Element( ) ) ) );
				}
			}
			else
			{
				// àeðÇÝÞ
				get_str( ss, *elem->insert( elem->end( ), std::string( ) ), '<' );
			}
		}

		return elem;
	}

	/// @brief vfðt@CXg[É«Þ
	/// @param[in] ofs «ÞoÍt@CXg[
	/// @param[in] elem «Þvfðw·Ce[^
	inline void XML::write( std::ofstream &ofs, const_iterator elem )const
	{
		// Jn^OÌ«ÝðJn·é
		for ( size_t i = 0; i < depth( elem ); i++ )
		{
			ofs << '\t';
		}

		ofs << '<' << elem->tag( ).name( );

		// ®«ð«Þ
		for ( Element::Tag::const_iterator iter = elem->tag( ).begin( ); iter != elem->tag( ).end( ); iter++ )
		{
			ofs << ' ' << iter->first << "=\"" << iter->second << '\"';
		}

		// Jn^OÌ«ÝðI¹·é
		ofs << ( !is_leaf( elem ) || !elem->empty( ) ? "" : " /" ) << '>';

		if ( !is_leaf( elem ) || elem->empty( ) )
		{
			ofs << std::endl;
		}
		
		Element::const_iterator content_iter = elem->begin( ); // àeð¸·éCe[^
		const_iterator child_iter = child_begin( elem ); // qvfð¸·éCe[^

		while ( content_iter != elem->end( ) || child_iter != child_end( elem ) )
		{
			// àeð«Þ
			if ( content_iter != elem->end( ) )
			{
				if ( !is_leaf( elem ) )
				{
					for ( size_t i = 0; i < depth( elem ) + 1; i++ )
					{
						ofs << '\t';
					}
				}

				ofs << *content_iter;

				if ( !is_leaf( elem ) )
				{
					ofs << std::endl;
				}

				content_iter++;
			}

			// qvfð«Þ
			if ( child_iter != child_end( elem ) )
			{
				write( ofs, child_iter );
				child_iter++;
			}
		}

		// I¹^Oð«Þ
		if ( !is_leaf( elem ) || !elem->empty( ) )
		{
			if ( !is_leaf( elem ) )
			{
				for ( size_t i = 0; i < depth( elem ); i++ )
				{
					ofs << '\t';
				}
			}
		
			ofs << "</" << elem->tag( ).name( ) << '>' << std::endl;
		}
	}
	
	/// @brief Xg[©ç¶ñðæ¾·é
	/// @param[in] ss üÍXg[
	/// @param[in] delim I[LD±êÌ¼OÜÅÇÝÝðs¤
	/// @param[in] term_with_space óðI[LÆµÄµ¤tODKèlÍfalse
	inline void XML::get_str( std::stringstream &ss, std::string &str, char delim, bool term_with_space )
	{
		// ú»·é
		str.clear( );

		// ÇÝÞ
		while ( ss.peek( ) != delim )
		{
			if ( term_with_space && std::isspace( ss.peek( ) ) )
			{
				break;
			}
			else
			{
				str += ss.get( );
			}
		}
	}

	/// @brief ®ìÈµ
	inline _Element::_Element( )
	{
	}

	/// @brief vfð¡»·é
	/// @param[in] elem ¡»·évf
	inline _Element::_Element( const _Element &elem ) : tag_( elem.tag_ ), std::vector< std::string >( elem )
	{
	}

	/// @brief ^OðÝè·é
	/// @param[in] tag Ýè·é^O
	inline _Element::_Element( const Tag &tag ) : tag_( tag )
	{
	}

	/// @brief ^O¼ðÝè·é
	/// @param[in] tag_name Ýè·é^O¼
	inline _Element::_Element( const std::string &tag_name ) : tag_( Tag( tag_name ) )
	{
	}

	/// @brief ^Oðæ¾·é
	/// @return ^OÌQÆ
	inline _Element::Tag &_Element::tag( )
	{
		return tag_;
	}

	/// @brief ^Oðæ¾·é
	/// @return ^OÌèQÆ
	inline const _Element::Tag &_Element::tag( )const
	{
		return tag_;
	}
		
	/// @brief vfªwèÌ^O¼ðÂ©»è·é
	/// @param[in] tag_name »è·é^O¼
	/// @retval true vfÍwèÌ^O¼ðÂ
	/// @retval false vfÍwèÌ^O¼ð½È¢
	inline bool _Element::operator==( const std::string &tag_name )const
	{
		return tag_.name( ) == tag_name;
	}

	/// @brief ®ìÈµ
	inline _Element::Tag::Tag( )
	{
	}

	/// @brief ^Oð¡»·é
	/// @param[in] tag ¡»·é^O
	inline _Element::Tag::Tag( const Tag &tag ) : name_( tag.name_ ), dictionary< std::string, std::string >( tag )
	{
	}

	/// @brief ^O¼ðÝè·é
	/// @param[in] name Ýè·é^O¼
	inline _Element::Tag::Tag( const std::string &name ) : name_( name )
	{
	}

	/// @brief ^O¼ðæ¾·é
	/// @return ^O¼ÌQÆ
	inline std::string &_Element::Tag::name( )
	{
		return name_;
	}

	/// @brief ^O¼ðæ¾·é
	/// @return ^O¼ÌèQÆ
	inline const std::string &_Element::Tag::name( )const
	{
		return name_;
	}
}

#endif //_TMATSU_XML_H_