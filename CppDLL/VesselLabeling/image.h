/**
* @file image.h
* @brief 画像クラスのヘッダファイル
* @version 3.0
* @author 名古屋大学 情報科学研究科 メディア科学専攻 森研究室   351203192   松﨑 哲朗
* @date 2013/10/18
*/

#ifndef _TMATSU_IMAGE_H_
#define _TMATSU_IMAGE_H_

#include <string>
#include <cstring>
#include <mist/mist.h>
#include <mist/io/raw.h>
#include <mist/vector.h>
#include <mist/matrix.h>
#include "header.h"
	
namespace tmatsu
{
	typedef mist::vector3< size_t > Point; ///< @brief 座標
	typedef mist::vector3< ptrdiff_t > Shift; ///< @brief 変位
	typedef mist::vector3< double > Vector; ///< @brief ベクトル
	typedef mist::matrix< double > Matrix; ///< @brief 行列

	const Shift neighborhood6[ ] = {
		Shift( -1, 0, 0 ),
		Shift( 0, -1, 0 ),
		Shift( 0, 0, -1 ),
		Shift( 0, 0, 1 ),
		Shift( 0, 1, 0 ),
		Shift( 1, 0, 0 )
	}; ///< @brief 6近傍に含まれる画素への変位

	const Shift neighborhood18[ ] = {
		Shift( -1, -1, 0 ),
		Shift( -1, 0, -1 ),
		Shift( -1, 0, 0 ),
		Shift( -1, 0, 1 ),
		Shift( -1, 1, 0 ),
		Shift( 0, -1, -1 ),
		Shift( 0, -1, 0 ),
		Shift( 0, -1, 1 ),
		Shift( 0, 0, -1 ),
		Shift( 0, 0, 1 ),
		Shift( 0, 1, -1 ),
		Shift( 0, 1, 0 ),
		Shift( 0, 1, 1 ),
		Shift( 1, -1, 0 ),
		Shift( 1, 0, -1 ),
		Shift( 1, 0, 0 ),
		Shift( 1, 0, 1 ),
		Shift( 1, 1, 0 )
	}; ///< @brief 18近傍に含まれる画素への変位

	const Shift neighborhood26[ ] = {
		Shift( -1, -1, -1 ),
		Shift( -1, -1, 0 ),
		Shift( -1, -1, 1 ),
		Shift( -1, 0, -1 ),
		Shift( -1, 0, 0 ),
		Shift( -1, 0, 1 ),
		Shift( -1, 1, -1 ),
		Shift( -1, 1, 0 ),
		Shift( -1, 1, 1 ),
		Shift( 0, -1, -1 ),
		Shift( 0, -1, 0 ),
		Shift( 0, -1, 1 ),
		Shift( 0, 0, -1 ),
		Shift( 0, 0, 1 ),
		Shift( 0, 1, -1 ),
		Shift( 0, 1, 0 ),
		Shift( 0, 1, 1 ),
		Shift( 1, -1, -1 ),
		Shift( 1, -1, 0 ),
		Shift( 1, -1, 1 ),
		Shift( 1, 0, -1 ),
		Shift( 1, 0, 0 ),
		Shift( 1, 0, 1 ),
		Shift( 1, 1, -1 ),
		Shift( 1, 1, 0 ),
		Shift( 1, 1, 1 )
	}; ///< @brief 26近傍に含まれる画素への変位

	/// @brief ファイル入出力などの機能を追加したmist::array3のラッパークラス
	/// @tparam ValueType データ型
	template < class ValueType >
	class Image : public mist::array3< ValueType > 
	{
	public:	
		typedef mist::__mist_dmy_callback__ DummyCallback; ///< @brief ダミーコールバック
		typedef mist::__mist_console_callback__ ConsoleCallback; ///< @brief 標準出力型コールバック
		typedef mist::__mist_progress_callback__ ProgressCallback; ///< @brief 進行状況表示型コールバック

		/// @brief 画像の範囲外の画素をアクセスしようとすると投げられる例外
		class InvalidPoint : std::exception
		{
		};
	private:
		Header hdr_; ///< @brief ヘッダ
	public:
		Image( );
		Image( const Image &img );
		template < class ImageType >
		Image( const ImageType &img );
		template < class Type >
		Image( const Image< Type > &img );
		template < class Functor >
		Image( Functor callback, const char *file_name, const char *hdr_name = NULL );
		//Added by c.wang
		Image(const mist::array3< ValueType >& img);

		virtual ~Image( );
		template < class Functor >
		bool read( Functor callback, const char *file_name, const char *hdr_name = NULL );
		template < class Functor >
		bool write( Functor callback, const char *file_name, const char *hdr_name = NULL )const;
		bool is_valid( size_t x, size_t y, size_t z )const;
		bool is_valid( const Point &pt )const;
		SizeSet get_size( )const;
		void set_size( const SizeSet &siz );
		PitchSet get_pitch( )const;
		void set_pitch( const PitchSet &pitch );
		Header &header( );
		const Header &header( )const;
		Image &operator=( const Image &img );
		template < class ImageType >
		Image &operator=( const ImageType &img );
		template < class Type >
		Image &operator=( const Image< Type > &img );
		ValueType &operator( )( size_t x, size_t y, size_t z );
		const ValueType &operator( )( size_t x, size_t y, size_t z )const;
		ValueType &operator( )( const Point &pt );
		const ValueType &operator( )( const Point &pt )const;
	};

	template < class ValueType >
	inline tmatsu::Image<ValueType>::Image(const mist::array3< ValueType >& img)
		: mist::array3<ValueType>(img)
	{
	}

	/// @brief 動作無し
	template < class ValueType >
	inline Image< ValueType >::Image( )
	{
	}
	
	/// @brief 画像を複製する
	/// @param[in] img 複製する画像
	template < class ValueType >
	inline Image< ValueType >::Image( const Image &img ) : mist::array3< ValueType >( img ), hdr_( img.hdr_ )
	{
	}

	/// @brief 型の異なる画像を複製する
	/// @param[in] img 複製する画像
	template < class ValueType >
	template < class ImageType >
	inline Image< ValueType >::Image( const ImageType &img ) : mist::array3< ValueType >( img )
	{
	}

	/// @brief データ型の異なる画像を複製する
	/// @param[in] img 複製する画像
	template < class ValueType >
	template < class Type >
	inline Image< ValueType >::Image( const Image< Type > &img ) : mist::array3< ValueType >( img ), hdr_( img.header( ) )
	{
	}

	/// @brief 画像をファイルから読み込む
	/// @param[in] callback コールバック関数
	/// @param[in] file_name 読み込むファイル名
	/// @param[in] hdr_name ヘッダファイル名．規定値はNULL．NULLが指定された場合，file_nameに".header"を付加したヘッダファイル名が設定される
	template < class Type >
	template < class Functor >
	inline Image< Type >::Image( Functor callback, const char *file_name, const char *hdr_name )
	{
		read( callback, file_name, hdr_name );
	}

	/// @brief 動作無し
	template < class Type >
	inline Image< Type >::~Image( )
	{
	}

	/// @brief 画像をファイルから読み込む
	/// @param[in] callback コールバック関数
	/// @param[in] file_name 読み込むファイル名
	/// @param[in] hdr_name ヘッダファイル名．規定値はNULL．NULLが指定された場合，file_nameに".header"を付加したヘッダファイル名が設定される
	/// @retval true 画像の読み込みに成功した
	/// @retval false 画像の読み込みに失敗した．恐らく，ファイルが開けなかったかヘッダファイルが不正だった
	template < class ValueType >
	template < class Functor >
	inline bool Image< ValueType >::read( Functor callback, const char *file_name, const char *hdr_name )
	{
		SizeSet siz; // サイズ
		PitchSet pitch; // ピッチ

		// ヘッダファイルを読み込む
		if ( !hdr_.read( hdr_name ? hdr_name : ( std::string( file_name ) + ".header" ).c_str( ) ) )
		{
			return false;
		}
		
		// サイズとピッチを読み込む
		try
		{
			siz = hdr_.get_size( );
			pitch = hdr_.get_pitch( );
		} 

		catch ( Header::InvalidKey )
		{
			return false;
		}

		// 画像ファイルを読み込む
		return mist::read_raw( *this, file_name, siz.x, siz.y, siz.z, pitch.x, pitch.y, pitch.z, 0, false, callback );
	}	

	/// @brief 画像をファイルへ書き込む
	/// @param[in] callback コールバック関数
	/// @param[in] file_name 書き込むファイル名
	/// @param[in] hdr_name ヘッダファイル名．規定値はNULL．NULLが指定された場合，file_nameに".header"を付加したヘッダファイル名が設定される
	/// @retval true 画像の書き込みに成功した
	/// @retval false 画像の書き込みに失敗した．恐らく，ファイルが開けなかった
	template < class ValueType >
	template < class Functor >
	inline bool Image< ValueType >::write( Functor callback, const char *file_name, const char *hdr_name )const
	{
		Header hdr = hdr_; // 出力ヘッダ
		size_t offset = std::string( file_name ).find_last_of( '\\' ) + 1; //オフセット
	
		// ファイル名を更新する
		hdr.set_org_file( std::string( file_name ).substr( offset, std::strlen( file_name ) - offset ) );
	
		// サイズとピッチを更新する
		hdr.set_size( SizeSet( width( ), height( ), depth( ) ) );
		hdr.set_pitch( PitchSet( reso1( ), reso2( ), reso3( ) ) );

		// ヘッダファイルを書き込む
		if ( !hdr.write( hdr_name ? hdr_name : ( std::string( file_name ) + ".header" ).c_str( ) ) )
		{
			return false;
		}

		// 画像ファイルの書き込み
		return mist::write_raw_gz( *this, file_name, 0, false, callback );
	}

	/// @brief 引数の座標が画像の範囲内にあるか判定する
	/// @param[in] x 判定するx座標
	/// @param[in] y 判定するy座標
	/// @param[in] z 判定するz座標
	/// @retval true 座標は画像の範囲内にある
	/// @retval false 座標は画像の範囲外にある
	template < class ValueType >
	inline bool Image< ValueType >::is_valid( size_t x, size_t y, size_t z )const
	{
		return x < width( ) && y < height( ) && z < depth( );
	}

	/// @brief 引数の座標が画像の範囲内にあるか判定する
	/// @param[in] pt 判定する座標
	/// @retval true 座標は画像の範囲内にある
	/// @retval false 座標は画像の範囲外にある
	template < class ValueType >
	inline bool Image< ValueType >::is_valid( const Point &pt )const
	{
		return pt.x < width( ) && pt.y < height( ) && pt.z < depth( );
	}
		
	/// @brief サイズの組を取得する
	/// @return サイズの組
	template < class ValueType >
	inline SizeSet Image< ValueType >::get_size( )const
	{
		return SizeSet( width( ), height( ), depth( ) );
	}
	
	/// @brief サイズを設定する
	/// @param[in] siz 設定するサイズの組
	template < class ValueType >
	inline void Image< ValueType >::set_size( const SizeSet &siz )
	{
		resize( siz.x, siz.y, siz.z );
	}
	
	/// @brief ピッチの組を取得する
	/// @return ピッチの組
	template < class ValueType >
	inline PitchSet Image< ValueType >::get_pitch( )const
	{
		return PitchSet( reso1( ), reso2( ), reso3( ) );
	}
	
	/// @brief ピッチを設定する
	/// @param[in] pitch 設定するピッチの組
	template < class ValueType >
	inline void Image< ValueType >::set_pitch( const PitchSet &pitch )
	{
		reso( pitch.x, pitch.y, pitch.z );
	}
	
	/// @brief ヘッダを取得する
	/// @return ヘッダの参照
	template < class ValueType >
	inline Header &Image< ValueType >::header( )
	{
		return hdr_;
	}

	/// @brief ヘッダを取得する
	/// @return ヘッダの定数参照
	template < class ValueType >
	inline const Header &Image< ValueType >::header( )const
	{
		return hdr_;
	}

	/// @brief 画像を複製する
	/// @param[in] img 複製する画像
	/// @return 自分自身の参照
	template < class ValueType >
	inline Image< ValueType > &Image< ValueType >::operator=( const Image &img )
	{
		if ( this != &img )
		{
			mist::array3< ValueType >::operator=( img );
			hdr_ = img.hdr_;
		}

		return *this;
	}

	/// @brief 型の異なる画像を複製する
	/// @param[in] img 複製する画像
	/// @return 自分自身の参照
	template < class ValueType >
	template < class ImageType > 
	inline Image< ValueType > &Image< ValueType >::operator=( const ImageType &img )
	{
		if ( reinterpret_cast< const void * >( this ) != reinterpret_cast< const void * >( &img ) )
		{
			mist::array3< ValueType >::operator=( img );
		}

		return *this;
	}

	/// @brief データ型の異なる画像を複製する
	/// @param[in] img 複製する画像
	/// @return 自分自身の参照
	template < class ValueType >
	template < class Type > 
	inline Image< ValueType > &Image< ValueType >::operator=( const Image< Type > &img )
	{
		if ( reinterpret_cast< const void * >( this ) != reinterpret_cast< const void * >( &img ) )
		{
			mist::array3< ValueType >::operator=( img );
			hdr_ = img.header( );
		}

		return *this;
	}

	/// @brief 引数の画素を取得する
	/// @attention 引数が画像の範囲外の場合， InvalidPoint 例外を投げる
	/// @param[in] x 取得する画素のx座標
	/// @param[in] y 取得する画素のy座標
	/// @param[in] z 取得する画素のz座標
	/// @return 引数の座標の画素の参照
	template < class ValueType >
	inline ValueType &Image< ValueType >::operator( )( size_t x, size_t y, size_t z )
	{
		if ( !is_valid( x, y, z ) )
		{
			throw InvalidPoint( );
		}

		return at( x, y, z );
	}

	/// @brief 引数の画素を取得する
	/// @attention 引数が画像の範囲外の場合， InvalidPoint 例外を投げる
	/// @param[in] x 取得する画素のx座標
	/// @param[in] y 取得する画素のy座標
	/// @param[in] z 取得する画素のz座標
	/// @return 引数の座標の画素の定数参照
	template < class ValueType >
	inline const ValueType &Image< ValueType >::operator( )( size_t x, size_t y, size_t z )const
	{
		if ( !is_valid( x, y, z ) )
		{
			throw InvalidPoint( );
		}

		return at( x, y, z );
	}

	/// @brief 引数の画素を取得する
	/// @attention 引数が画像の範囲外の場合， InvalidPoint 例外を投げる
	/// @param[in] pt 取得する画素の座標
	/// @return 引数の座標の画素の参照
	template < class ValueType >
	inline ValueType &Image< ValueType >::operator( )( const Point &pt )
	{
		if ( !is_valid( pt ) )
		{
			throw InvalidPoint( );
		}

		return at( pt.x, pt.y, pt.z );
	}

	/// @brief 引数の画素を取得する
	/// @attention 引数が画像の範囲外の場合， InvalidPoint 例外を投げる
	/// @param[in] pt 取得する画素の座標
	/// @return 引数の座標の画素の定数参照
	template < class ValueType >
	inline const ValueType &Image< ValueType >::operator( )( const Point &pt )const
	{
		if ( !is_valid( pt ) )
		{
			throw InvalidPoint( );
		}

		return at( pt.x, pt.y, pt.z );
	}
}

#endif //_TMATSU_IMAGE_H_