/**
* @file xml.h
* @brief XMLクラスのヘッダファイル
* @version 3.0
* @author 名古屋大学 情報科学研究科 メディア科学専攻 森研究室   351203192   松﨑 哲朗
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

#define DEFAULT_VERSION "1.0" ///< @brief version属性の既定値
#define DEFAULT_ENCODING "shift-jis" ///< @brief encoding属性の既定値
#define DEFAULT_STANDALONE "yes" ///< @brief standalone属性の既定値

namespace tmatsu
{
	/// @brief 要素をテキストの動的配列として扱うクラス
	class _Element : public std::vector< std::string >
	{
	public:
		/// @brief タグを属性の連想配列として扱うクラス
		class Tag : public tmatsu::dictionary< std::string, std::string >
		{
		public:
			typedef invalid_key InvalidProperty; ///< @brief 無効属性参照例外
		private:
			std::string name_; ///< @brief タグ名
		public:
			Tag( );
			Tag( const Tag &tag );
			Tag( const std::string &name );
			std::string &name( );
			const std::string &name( )const;
		};
	private:
		Tag tag_; ///< @brief タグ
	public:
		_Element( );
		_Element( const _Element &elem );
		_Element( const Tag &tag );
		_Element( const std::string &tag_name );
		Tag &tag( );
		const Tag &tag( )const;
		bool operator==( const std::string &tag_name )const;
	};

	/// @brief XMLを木構造として扱うクラス
	class XML : public tree< _Element >
	{
	public:
		typedef _Element Element; ///< @brief 要素を扱うクラス
	private:
		std::string version_; ///< @brief version属性
		std::string encoding_; ///< @brief encoding属性
		std::string standalone_; ///< @brief standalone属性
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
	
	/// @brief 各属性を既定値で初期化する
	inline XML::XML( ) : version_( DEFAULT_VERSION ), encoding_( DEFAULT_ENCODING ), standalone_( DEFAULT_STANDALONE )
	{
	}
	
	/// @brief XMLを複製する
	/// @param[in] xml 複製するXML
	inline XML::XML( const XML &xml ) : version_( xml.version_ ), encoding_( xml.encoding_ ), standalone_( xml.standalone_ ), tree< _Element >( xml )
	{
	}
	
	/// @brief XMLをファイルから読み込む
	/// @attention XMLファイルの内容によっては正常に読み込めない可能性がある
	/// @param[in] file_name 読み込むXMLファイル名
	inline XML::XML( const char *file_name ) : version_( DEFAULT_VERSION ), encoding_( DEFAULT_ENCODING ), standalone_( DEFAULT_STANDALONE )
	{
		read( file_name );
	}
	
	/// @brief 各属性を設定する
	/// @param[in] version version属性
	/// @param[in] encoding encoding属性
	/// @param[in] standalone standalone属性
	inline XML::XML( const std::string &version, const std::string &encoding, const std::string &standalone ) : version_( version ), encoding_( encoding ), standalone_( standalone )
	{
	}

	/// @brief 動作なし
	inline XML::~XML( )
	{
	}
	
	/// @brief version属性を取得する
	/// @return version属性の参照
	inline std::string &XML::version( )
	{
		return version_;
	}
	
	/// @brief version属性を取得する
	/// @return version属性の定数参照
	inline const std::string &XML::version( )const
	{
		return version_;
	}
	
	/// @brief encoding属性を取得する
	/// @return encoding属性の参照
	inline std::string &XML::encoding( )
	{
		return encoding_;
	}
	
	/// @brief encoding属性を取得する
	/// @return encoding属性の定数参照
	inline const std::string &XML::encoding( )const
	{
		return encoding_;
	}
		
	/// @brief standalone属性を取得する
	/// @return standalone属性の参照
	inline std::string &XML::standalone( )
	{
		return standalone_;
	}
		
	/// @brief standalone属性を取得する
	/// @return standalone属性の定数参照
	inline const std::string &XML::standalone( )const
	{
		return standalone_;
	}

	/// @brief XMLをファイルから読み込む
	/// @attention XMLファイルの内容によっては正常に読み込めない可能性がある
	/// @param[in] file_name 読み込むXMLファイル名
	/// @retval true ファイルの読み込みに成功した
	/// @retval false ファイルの読み込みに失敗した．恐らく，ファイルが開けなかった
	inline bool XML::read( const char *file_name )
	{
		std::ifstream ifs( file_name ); // 入力ファイルストリーム
		std::stringstream ss; // 入力文字列ストリーム
		std::string buf; // バッファ

		// エラーチェック
		if ( !ifs )
		{
			return false;
		}

		// 何故かファイルを全て読み込まないとエラーが生じる恐れがある
		while ( !ifs.eof( ) )
		{
			ss << static_cast< char >( ifs.get( ) );
		}

		// 木構造を空にする
		clear( );

		// 「<?xml」を読み飛ばす
		ss.ignore( INT_MAX, 'l' );

		// 属性を読み込む
		while ( true )
		{
			// 空白を読み飛ばす
			while ( std::isspace( ss.peek( ) ) )
			{
				ss.ignore( );
			}

			if ( ss.peek( ) != '?' )
			{
				// 属性の名前を読み込む
				get_str( ss, buf, '=', true );

				//「 ="」を読み飛ばす
				ss.ignore( INT_MAX, '\"' );

				if ( buf == "version" )
				{
					// version属性を読み込む
					get_str( ss, version_, '\"' );
				}
				else if ( buf == "encoding" )
				{
					// encoding属性を読み込む
					get_str( ss, encoding_, '\"' );
				} 
				else if ( buf == "standalone" )
				{
					// standalone属性を読み込む
					get_str( ss, standalone_, '\"' );
				}
					
				// 「"」を読み飛ばす
				ss.ignore( INT_MAX, '\"' );
			}
			else
			{
				// 「?>」を読み飛ばす
				ss.ignore( INT_MAX, '>' );

				break;
			}
		}

		// 要素を読み込む
		read( ss, insert( end( ), Element( ) ) );

		return true;
	}

	/// @brief XMLをファイルに書き込む
	/// @param[in] file_name 書き込むXMLファイル名
	/// @retval true 書き込みに成功した
	/// @retval false 書き込みし失敗した．恐らく，ファイルが開けなかった
	inline bool XML::write( const char *file_name )const
	{
		std::ofstream ofs( file_name ); // 出力ファイルストリーム

		// エラーチェック
		if ( !ofs )
		{
			return false;
		}

		// XML宣言を書き込む
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

		// 要素を書き込む
		write( ofs, preorder_begin( ) );

		return true;
	}

	/// @brief 要素をストリームから読み込む
	/// @param[in] ss 読み込む入力ストリーム
	/// @param[in] elem 読み込む要素を指すイテレータ
	/// @return 読み込んだ要素を指すイテレータ
	inline XML::iterator XML::read( std::stringstream &ss, iterator elem )
	{
		bool cont = true; // 継続フラグ

		// 「<」を読み飛ばす
		ss.ignore( INT_MAX, '<' );

		// タグを読み込む
		get_str( ss, elem->tag( ).name( ), '>', true );

		if ( !elem->tag( ).name( ).empty( ) && elem->tag( ).name( )[ elem->tag( ).name( ).size( ) - 1 ] == '/' )
		{
			// 空タグのとき
			elem->tag( ).name( ).resize( elem->tag( ).name( ).size( ) - 1 );

			return elem;
		}

		// 属性を読み込む
		while ( cont )
		{
			// 空白を読み飛ばす
			while ( std::isspace( ss.peek( ) ) )
			{
				ss.ignore( );
			}

			switch ( ss.peek( ) )
			{
			case '/':
				// 「/>」を読み飛ばす
				ss.ignore( INT_MAX, '>' );

				return elem;
			case '>':
				// 「>」を読み飛ばす
				ss.ignore( INT_MAX, '>' );
				cont = false;
					
				break;
			default:
				std::string prop, val; // バッファ

				// 属性の名前を読み込む
				get_str( ss, prop, '=', true );

				// 「="」を読み飛ばす
				ss.ignore( INT_MAX, '\"' );

				// 属性の値を読み込む
				get_str( ss, val, '\"' );

				// 「"」を読み飛ばす
				ss.ignore( INT_MAX, '\"' );

				// 属性を設定する
				elem->tag( ).insert( elem->tag( ).end( ), Element::Tag::value_type( prop, val ) );
			}
		}

		// 内容，コメント，終了タグまたは子要素を読み込む
		while ( true )
		{
			// 空白を読み飛ばす
			while ( std::isspace( ss.peek( ) ) )
			{
				ss.ignore( );
			}

			if ( ss.peek( ) == '<' )
			{
				// 「<」を読み飛ばす
				ss.ignore( INT_MAX, '<' );

				if ( ss.peek( ) == '/' )
				{
					// 終了タグを読み飛ばす
					ss.ignore( INT_MAX, '>' );

					break;
				}
				else if ( ss.peek( ) == '!' )
				{
					char temp[ 4 ]; // 一時領域

					ss.get( temp, 4 );
					temp[ 3 ] = '\0';

					if ( std::strcmp( temp, "!--" ) )
					{
						// 子要素を読み込む
						ss.putback( temp[ 2 ] );
						ss.putback( temp[ 1 ] );
						ss.putback( temp[ 0 ] );
						ss.putback( '<' );
						elem = parent( read( ss, insert( elem, Element( ) ) ) );
					}
					else
					{
						// コメントを読み飛ばす
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
					// 子要素を読み込む
					ss.putback( '<' );
					elem = parent( read( ss, insert( elem, Element( ) ) ) );
				}
			}
			else
			{
				// 内容を読み込む
				get_str( ss, *elem->insert( elem->end( ), std::string( ) ), '<' );
			}
		}

		return elem;
	}

	/// @brief 要素をファイルストリームに書き込む
	/// @param[in] ofs 書き込む出力ファイルストリーム
	/// @param[in] elem 書き込む要素を指すイテレータ
	inline void XML::write( std::ofstream &ofs, const_iterator elem )const
	{
		// 開始タグの書き込みを開始する
		for ( size_t i = 0; i < depth( elem ); i++ )
		{
			ofs << '\t';
		}

		ofs << '<' << elem->tag( ).name( );

		// 属性を書き込む
		for ( Element::Tag::const_iterator iter = elem->tag( ).begin( ); iter != elem->tag( ).end( ); iter++ )
		{
			ofs << ' ' << iter->first << "=\"" << iter->second << '\"';
		}

		// 開始タグの書き込みを終了する
		ofs << ( !is_leaf( elem ) || !elem->empty( ) ? "" : " /" ) << '>';

		if ( !is_leaf( elem ) || elem->empty( ) )
		{
			ofs << std::endl;
		}
		
		Element::const_iterator content_iter = elem->begin( ); // 内容を走査するイテレータ
		const_iterator child_iter = child_begin( elem ); // 子要素を走査するイテレータ

		while ( content_iter != elem->end( ) || child_iter != child_end( elem ) )
		{
			// 内容を書き込む
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

			// 子要素を書き込む
			if ( child_iter != child_end( elem ) )
			{
				write( ofs, child_iter );
				child_iter++;
			}
		}

		// 終了タグを書き込む
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
	
	/// @brief ストリームから文字列を取得する
	/// @param[in] ss 入力ストリーム
	/// @param[in] delim 終端記号．これの直前まで読み込みを行う
	/// @param[in] term_with_space 空白を終端記号として扱うフラグ．規定値はfalse
	inline void XML::get_str( std::stringstream &ss, std::string &str, char delim, bool term_with_space )
	{
		// 初期化する
		str.clear( );

		// 読み込む
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

	/// @brief 動作なし
	inline _Element::_Element( )
	{
	}

	/// @brief 要素を複製する
	/// @param[in] elem 複製する要素
	inline _Element::_Element( const _Element &elem ) : tag_( elem.tag_ ), std::vector< std::string >( elem )
	{
	}

	/// @brief タグを設定する
	/// @param[in] tag 設定するタグ
	inline _Element::_Element( const Tag &tag ) : tag_( tag )
	{
	}

	/// @brief タグ名を設定する
	/// @param[in] tag_name 設定するタグ名
	inline _Element::_Element( const std::string &tag_name ) : tag_( Tag( tag_name ) )
	{
	}

	/// @brief タグを取得する
	/// @return タグの参照
	inline _Element::Tag &_Element::tag( )
	{
		return tag_;
	}

	/// @brief タグを取得する
	/// @return タグの定数参照
	inline const _Element::Tag &_Element::tag( )const
	{
		return tag_;
	}
		
	/// @brief 要素が指定のタグ名を持つか判定する
	/// @param[in] tag_name 判定するタグ名
	/// @retval true 要素は指定のタグ名を持つ
	/// @retval false 要素は指定のタグ名を持たない
	inline bool _Element::operator==( const std::string &tag_name )const
	{
		return tag_.name( ) == tag_name;
	}

	/// @brief 動作なし
	inline _Element::Tag::Tag( )
	{
	}

	/// @brief タグを複製する
	/// @param[in] tag 複製するタグ
	inline _Element::Tag::Tag( const Tag &tag ) : name_( tag.name_ ), dictionary< std::string, std::string >( tag )
	{
	}

	/// @brief タグ名を設定する
	/// @param[in] name 設定するタグ名
	inline _Element::Tag::Tag( const std::string &name ) : name_( name )
	{
	}

	/// @brief タグ名を取得する
	/// @return タグ名の参照
	inline std::string &_Element::Tag::name( )
	{
		return name_;
	}

	/// @brief タグ名を取得する
	/// @return タグ名の定数参照
	inline const std::string &_Element::Tag::name( )const
	{
		return name_;
	}
}

#endif //_TMATSU_XML_H_