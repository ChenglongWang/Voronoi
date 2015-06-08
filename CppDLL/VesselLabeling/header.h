/**
* @file header.h
* @brief ヘッダクラスのヘッダファイル
* @version 3.0
* @author 名古屋大学 情報科学研究科 メディア科学専攻 森研究室   351203192   松﨑 哲朗
* @date 2013/10/18
*/

#ifndef _TMATSU_HEADER_H_
#define _TMATSU_HEADER_H_

#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <mist/vector.h>
#include "dictionary.h"

#undef max
#undef min

namespace tmatsu 
{
	typedef mist::vector3< size_t > SizeSet; ///< @brief サイズの組
	typedef mist::vector3< double > PitchSet; ///< @brief ピッチの組

	/// @brief 森ヘッダを連想配列として扱うクラス
	class Header : public dictionary< std::string, std::string >
	{
	public:
		typedef invalid_key InvalidKey; ///< @brief ヘッダに含まれていないキーを参照しようとすると投げられる例外
	private:
		size_t max_len_; ///< @brief キーの最大長
	public:
		Header( );
		Header( const Header &hdr );
		Header( const char *file_name );
		virtual ~Header( );
		template < class PairType >
		iterator insert( const PairType &pair );
		template < class PairType >
		iterator insert( const_iterator pos, const PairType &pair );
		template < class Iterator >
		void insert( Iterator first, Iterator last );
		bool read( const char *file_name );
		bool write( const char *file_name )const;
		std::string get_org_file( )const;
		void set_org_file( const std::string &org_file );
		SizeSet get_size( )const;
		void set_size( const SizeSet &siz );
		PitchSet get_pitch( )const;
		void set_pitch( const PitchSet &pitch );
#if !( _MSC_VER >= 1600 )
	private:
		template < class Type >
		static std::string to_string( const Type &a );
#endif
	};

	/// @brief 初期化する
	inline Header::Header( ) : max_len_( 0 )
	{
	}

	/// @brief ヘッダを複製する
	/// @param[in] hdr 複製するヘッダ
	inline Header::Header( const Header &hdr ) : dictionary< std::string, std::string >( hdr ), max_len_( hdr.max_len_ )
	{
	}

	/// @brief ヘッダをファイルから読み込む
	/// @param[in] file_name 読み込むヘッダファイル名
	inline Header::Header( const char *file_name ) : max_len_( 0 )
	{
		read( file_name );
	}

	/// @brief 動作無し
	inline Header::~Header( )
	{
	}

	/// @brief キーと値を挿入する
	/// @param[in] pair 挿入するキーと値の組
	/// @return 挿入したキーと値の組を指すイテレータ
	template < class PairType >
	inline Header::iterator Header::insert( const PairType &pair )
	{
		max_len_ = std::max( pair.first.length( ), max_len_ );

		return dictionary< std::string, std::string >::insert( end( ), pair );
	}

	/// @brief キーと値を挿入する
	/// @param[in] pos 挿入する位置の次を指すイテレータ
	/// @param[in] pair 挿入するキーと値の組
	/// @return 挿入したキーと値の組を指すイテレータ
	template < class PairType >
	inline Header::iterator Header::insert( const_iterator pos, const PairType &pair )
	{
		max_len_ = std::max( pair.first.length( ), max_len_ );

		return dictionary< std::string, std::string >::insert( pos, pair );
	}

	/// @brief キーと値を挿入する
	/// @param[in] first 挿入するキーと値の組の先頭を指すイテレータ
	/// @param[in] last 挿入するキーと値の組の末尾の次を指すイテレータ
	template< class Iterator >
	inline void Header::insert( Iterator first, Iterator last )
	{
		for ( IteratorType iter = first; iter != last; iter++ )
		{
			max_len_ = std::max( iter->first.length( ), max_len_ );
			dictionary< std::string, std::string >::insert( end( ), *iter );
		}
	}

	/// @brief ヘッダをファイルから読み込む
	/// @param[in] file_name 読み込むヘッダファイル名
	/// @retval true 読み込みに成功した
	/// @retval false 読み込みに失敗した．恐らく，ファイルが開けなかった
	inline bool Header::read( const char *file_name )
	{
		std::ifstream in_file( file_name ); // ヘッダファイル

		// エラーチェック
		if ( !in_file )
		{
			return false;
		}

		// 読み込む
		while ( true )
		{
			std::pair< std::string, std::string > buf; // バッファ
			char temp; // 一時変数

			in_file >> buf.first;

			do
			{
				in_file.get( temp );
			}
			while ( temp != '\n' && ( std::isspace( temp ) || temp == ':' ) && !in_file.eof( ) );

			while ( temp != '\n' && !in_file.eof( ) )
			{
				buf.second += temp;

				in_file.get( temp );
			}

			if ( in_file.eof( ) )
			{
				break;
			}

			max_len_ = std::max( buf.first.length( ), max_len_ );
			insert( end( ), buf );
		}

		return true;
	}

	/// @brief ヘッダをファイルへ書き込む
	/// @param[in] file_name 書き込むヘッダファイル名
	/// @retval true 書き込みに成功した
	/// @retval false 書き込みに失敗した．恐らく，ファイルが開けなかった
	inline bool Header::write( const char *file_name )const
	{
		std::ofstream out_file( file_name ); // ヘッダファイル

		// エラーチェック
		if ( !out_file )
		{
			return false;
		}

		// 書き込む
		for ( const_iterator iter = begin( ); iter != end( ); iter++ )
		{
			out_file << iter->first;

			for ( size_t i = iter->first.length( ); i < max_len_; i++ )
			{
				out_file << ' ';
			}

			out_file << " : " << iter->second << std::endl;
		}

		return true;
	}

	/// @brief ファイル名を取得する
	/// @attention 必要なキーが含まれていないとき， InvalidKey 例外を投げる
	/// @return ファイル名
	inline std::string Header::get_org_file( )const
	{
		return operator[ ]( "OrgFile" );
	}

	/// @brief ファイル名を設定する
	/// @param[in] org_file 設定するファイル名
	inline void Header::set_org_file( const std::string &org_file )
	{
		try
		{
			operator[ ]( "OrgFile" ) = org_file;
		}

		catch ( InvalidKey )
		{
			insert( value_type( "OrgFile", org_file ) );
		}
	}
	
	/// @brief サイズの組を取得する
	/// @attention 必要なキーが含まれていないとき， InvalidKey 例外を投げる
	/// @return サイズの組
	inline SizeSet Header::get_size( )const
	{
#if _MSC_VER >= 1600
		return SizeSet( std::stoi( operator[ ]( "SizeX" ) ), std::stoi( operator[ ]( "SizeY" ) ), std::stoi( operator[ ]( "SizeZ" ) ) );
#else
		return SizeSet( std::atoi( operator[ ]( "SizeX" ).c_str( ) ), std::atoi( operator[ ]( "SizeY" ).c_str( ) ), std::atoi( operator[ ]( "SizeZ" ) ).c_str( ) );
#endif
	}

	/// @brief サイズを設定する
	/// @param[in] siz 設定するサイズの組
	inline void Header::set_size( const SizeSet &siz )
	{
		try
		{
			operator[ ]( "SizeX" ) = std::to_string( static_cast< long double >( siz.x ) );
		}

		catch ( InvalidKey )
		{
			insert( value_type( "SizeX", std::to_string( static_cast< long double >( siz.x ) ) ) );
		}

		try
		{
			operator[ ]( "SizeY" ) = std::to_string( static_cast< long double >( siz.y ) );
		}

		catch ( InvalidKey )
		{
			insert( value_type( "SizeY", std::to_string( static_cast< long double >( siz.y ) ) ) );
		}

		try
		{
			operator[ ]( "SizeZ" ) = std::to_string( static_cast< long double >( siz.z ) );
		}

		catch ( InvalidKey )
		{
			insert( value_type( "SizeZ", std::to_string( static_cast< long double >( siz.z ) ) ) );
		}
	}
	
	/// @brief ピッチの組を取得する
	/// @attention 必要なキーが含まれていないとき， InvalidKey 例外を投げる
	/// @return ピッチの組
	inline PitchSet Header::get_pitch( )const
	{
#if _MSC_VER >= 1600
		return PitchSet( std::stod( operator[ ]( "PitchX" ) ), std::stod( operator[ ]( "PitchY" ) ), std::stod( operator[ ]( "PitchZ" ) ) );
#else
		return PitchSet( std::atof( operator[ ]( "PitchX" ).c_str( ) ), std::atof( operator[ ]( "PitchY" ).c_str( ) ), std::atof( operator[ ]( "PitchZ" ).c_str( ) ) );
#endif
	}

	/// @brief ピッチを設定する
	/// @param[in] pitch 設定するピッチの組
	inline void Header::set_pitch( const PitchSet &pitch )
	{
		try
		{
			operator[ ]( "PitchX" ) = std::to_string( static_cast< long double >( pitch.x ) );
		}

		catch ( InvalidKey )
		{
			insert( value_type( "PitchX", std::to_string( static_cast< long double >( pitch.x ) ) ) );
		}
		
		try
		{
			operator[ ]( "PitchY" ) = std::to_string( static_cast< long double >( pitch.y ) );
		}

		catch ( InvalidKey )
		{
			insert( value_type( "PitchY", std::to_string( static_cast< long double >( pitch.y ) ) ) );
		}
		
		try
		{
			operator[ ]( "PitchZ" ) = std::to_string( static_cast< long double >( pitch.z ) );
		}

		catch ( InvalidKey )
		{
			insert( value_type( "PitchZ", std::to_string( static_cast< long double >( pitch.z ) ) ) );
		}
	}

#if !( _MSC_VER >= 1600 )
	/// @brief 引数を文字列に変換する
	/// @param[in] a 文字列に変換する値
	/// @return 文字列
	template < class Type >
	std::string Header::to_string( const Type &a )
	{
		std::stringstream ss; // ストリングストリーム

		ss << a;
		return ss.str( );
	}
#endif
}

#endif //_TMATSU_HEADER_H_