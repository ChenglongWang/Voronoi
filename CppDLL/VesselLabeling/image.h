/**
* @file image.h
* @brief æNXÌwb_t@C
* @version 3.0
* @author ¼Ã®åw îñÈw¤È fBAÈwêU X¤º   351203192   ¼ú± NN
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
	typedef mist::vector3< size_t > Point; ///< @brief ÀW
	typedef mist::vector3< ptrdiff_t > Shift; ///< @brief ÏÊ
	typedef mist::vector3< double > Vector; ///< @brief xNg
	typedef mist::matrix< double > Matrix; ///< @brief sñ

	const Shift neighborhood6[ ] = {
		Shift( -1, 0, 0 ),
		Shift( 0, -1, 0 ),
		Shift( 0, 0, -1 ),
		Shift( 0, 0, 1 ),
		Shift( 0, 1, 0 ),
		Shift( 1, 0, 0 )
	}; ///< @brief 6ßTÉÜÜêéæfÖÌÏÊ

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
	}; ///< @brief 18ßTÉÜÜêéæfÖÌÏÊ

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
	}; ///< @brief 26ßTÉÜÜêéæfÖÌÏÊ

	/// @brief t@CüoÍÈÇÌ@\ðÇÁµ½mist::array3Ìbp[NX
	/// @tparam ValueType f[^^
	template < class ValueType >
	class Image : public mist::array3< ValueType > 
	{
	public:	
		typedef mist::__mist_dmy_callback__ DummyCallback; ///< @brief _~[R[obN
		typedef mist::__mist_console_callback__ ConsoleCallback; ///< @brief WoÍ^R[obN
		typedef mist::__mist_progress_callback__ ProgressCallback; ///< @brief isóµ\¦^R[obN

		/// @brief æÌÍÍOÌæfðANZXµæ¤Æ·éÆ°çêéáO
		class InvalidPoint : std::exception
		{
		};
	private:
		Header hdr_; ///< @brief wb_
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

	/// @brief ®ì³µ
	template < class ValueType >
	inline Image< ValueType >::Image( )
	{
	}
	
	/// @brief æð¡»·é
	/// @param[in] img ¡»·éæ
	template < class ValueType >
	inline Image< ValueType >::Image( const Image &img ) : mist::array3< ValueType >( img ), hdr_( img.hdr_ )
	{
	}

	/// @brief ^ÌÙÈéæð¡»·é
	/// @param[in] img ¡»·éæ
	template < class ValueType >
	template < class ImageType >
	inline Image< ValueType >::Image( const ImageType &img ) : mist::array3< ValueType >( img )
	{
	}

	/// @brief f[^^ÌÙÈéæð¡»·é
	/// @param[in] img ¡»·éæ
	template < class ValueType >
	template < class Type >
	inline Image< ValueType >::Image( const Image< Type > &img ) : mist::array3< ValueType >( img ), hdr_( img.header( ) )
	{
	}

	/// @brief æðt@C©çÇÝÞ
	/// @param[in] callback R[obNÖ
	/// @param[in] file_name ÇÝÞt@C¼
	/// @param[in] hdr_name wb_t@C¼DKèlÍNULLDNULLªwè³ê½êCfile_nameÉ".header"ðtÁµ½wb_t@C¼ªÝè³êé
	template < class Type >
	template < class Functor >
	inline Image< Type >::Image( Functor callback, const char *file_name, const char *hdr_name )
	{
		read( callback, file_name, hdr_name );
	}

	/// @brief ®ì³µ
	template < class Type >
	inline Image< Type >::~Image( )
	{
	}

	/// @brief æðt@C©çÇÝÞ
	/// @param[in] callback R[obNÖ
	/// @param[in] file_name ÇÝÞt@C¼
	/// @param[in] hdr_name wb_t@C¼DKèlÍNULLDNULLªwè³ê½êCfile_nameÉ".header"ðtÁµ½wb_t@C¼ªÝè³êé
	/// @retval true æÌÇÝÝÉ¬÷µ½
	/// @retval false æÌÇÝÝÉ¸sµ½D°ç­Ct@CªJ¯È©Á½©wb_t@Cªs³¾Á½
	template < class ValueType >
	template < class Functor >
	inline bool Image< ValueType >::read( Functor callback, const char *file_name, const char *hdr_name )
	{
		SizeSet siz; // TCY
		PitchSet pitch; // sb`

		// wb_t@CðÇÝÞ
		if ( !hdr_.read( hdr_name ? hdr_name : ( std::string( file_name ) + ".header" ).c_str( ) ) )
		{
			return false;
		}
		
		// TCYÆsb`ðÇÝÞ
		try
		{
			siz = hdr_.get_size( );
			pitch = hdr_.get_pitch( );
		} 

		catch ( Header::InvalidKey )
		{
			return false;
		}

		// æt@CðÇÝÞ
		return mist::read_raw( *this, file_name, siz.x, siz.y, siz.z, pitch.x, pitch.y, pitch.z, 0, false, callback );
	}	

	/// @brief æðt@CÖ«Þ
	/// @param[in] callback R[obNÖ
	/// @param[in] file_name «Þt@C¼
	/// @param[in] hdr_name wb_t@C¼DKèlÍNULLDNULLªwè³ê½êCfile_nameÉ".header"ðtÁµ½wb_t@C¼ªÝè³êé
	/// @retval true æÌ«ÝÉ¬÷µ½
	/// @retval false æÌ«ÝÉ¸sµ½D°ç­Ct@CªJ¯È©Á½
	template < class ValueType >
	template < class Functor >
	inline bool Image< ValueType >::write( Functor callback, const char *file_name, const char *hdr_name )const
	{
		Header hdr = hdr_; // oÍwb_
		size_t offset = std::string( file_name ).find_last_of( '\\' ) + 1; //ItZbg
	
		// t@C¼ðXV·é
		hdr.set_org_file( std::string( file_name ).substr( offset, std::strlen( file_name ) - offset ) );
	
		// TCYÆsb`ðXV·é
		hdr.set_size( SizeSet( width( ), height( ), depth( ) ) );
		hdr.set_pitch( PitchSet( reso1( ), reso2( ), reso3( ) ) );

		// wb_t@Cð«Þ
		if ( !hdr.write( hdr_name ? hdr_name : ( std::string( file_name ) + ".header" ).c_str( ) ) )
		{
			return false;
		}

		// æt@CÌ«Ý
		return mist::write_raw_gz( *this, file_name, 0, false, callback );
	}

	/// @brief øÌÀWªæÌÍÍàÉ é©»è·é
	/// @param[in] x »è·éxÀW
	/// @param[in] y »è·éyÀW
	/// @param[in] z »è·ézÀW
	/// @retval true ÀWÍæÌÍÍàÉ é
	/// @retval false ÀWÍæÌÍÍOÉ é
	template < class ValueType >
	inline bool Image< ValueType >::is_valid( size_t x, size_t y, size_t z )const
	{
		return x < width( ) && y < height( ) && z < depth( );
	}

	/// @brief øÌÀWªæÌÍÍàÉ é©»è·é
	/// @param[in] pt »è·éÀW
	/// @retval true ÀWÍæÌÍÍàÉ é
	/// @retval false ÀWÍæÌÍÍOÉ é
	template < class ValueType >
	inline bool Image< ValueType >::is_valid( const Point &pt )const
	{
		return pt.x < width( ) && pt.y < height( ) && pt.z < depth( );
	}
		
	/// @brief TCYÌgðæ¾·é
	/// @return TCYÌg
	template < class ValueType >
	inline SizeSet Image< ValueType >::get_size( )const
	{
		return SizeSet( width( ), height( ), depth( ) );
	}
	
	/// @brief TCYðÝè·é
	/// @param[in] siz Ýè·éTCYÌg
	template < class ValueType >
	inline void Image< ValueType >::set_size( const SizeSet &siz )
	{
		resize( siz.x, siz.y, siz.z );
	}
	
	/// @brief sb`Ìgðæ¾·é
	/// @return sb`Ìg
	template < class ValueType >
	inline PitchSet Image< ValueType >::get_pitch( )const
	{
		return PitchSet( reso1( ), reso2( ), reso3( ) );
	}
	
	/// @brief sb`ðÝè·é
	/// @param[in] pitch Ýè·ésb`Ìg
	template < class ValueType >
	inline void Image< ValueType >::set_pitch( const PitchSet &pitch )
	{
		reso( pitch.x, pitch.y, pitch.z );
	}
	
	/// @brief wb_ðæ¾·é
	/// @return wb_ÌQÆ
	template < class ValueType >
	inline Header &Image< ValueType >::header( )
	{
		return hdr_;
	}

	/// @brief wb_ðæ¾·é
	/// @return wb_ÌèQÆ
	template < class ValueType >
	inline const Header &Image< ValueType >::header( )const
	{
		return hdr_;
	}

	/// @brief æð¡»·é
	/// @param[in] img ¡»·éæ
	/// @return ©ª©gÌQÆ
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

	/// @brief ^ÌÙÈéæð¡»·é
	/// @param[in] img ¡»·éæ
	/// @return ©ª©gÌQÆ
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

	/// @brief f[^^ÌÙÈéæð¡»·é
	/// @param[in] img ¡»·éæ
	/// @return ©ª©gÌQÆ
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

	/// @brief øÌæfðæ¾·é
	/// @attention øªæÌÍÍOÌêC InvalidPoint áOð°é
	/// @param[in] x æ¾·éæfÌxÀW
	/// @param[in] y æ¾·éæfÌyÀW
	/// @param[in] z æ¾·éæfÌzÀW
	/// @return øÌÀWÌæfÌQÆ
	template < class ValueType >
	inline ValueType &Image< ValueType >::operator( )( size_t x, size_t y, size_t z )
	{
		if ( !is_valid( x, y, z ) )
		{
			throw InvalidPoint( );
		}

		return at( x, y, z );
	}

	/// @brief øÌæfðæ¾·é
	/// @attention øªæÌÍÍOÌêC InvalidPoint áOð°é
	/// @param[in] x æ¾·éæfÌxÀW
	/// @param[in] y æ¾·éæfÌyÀW
	/// @param[in] z æ¾·éæfÌzÀW
	/// @return øÌÀWÌæfÌèQÆ
	template < class ValueType >
	inline const ValueType &Image< ValueType >::operator( )( size_t x, size_t y, size_t z )const
	{
		if ( !is_valid( x, y, z ) )
		{
			throw InvalidPoint( );
		}

		return at( x, y, z );
	}

	/// @brief øÌæfðæ¾·é
	/// @attention øªæÌÍÍOÌêC InvalidPoint áOð°é
	/// @param[in] pt æ¾·éæfÌÀW
	/// @return øÌÀWÌæfÌQÆ
	template < class ValueType >
	inline ValueType &Image< ValueType >::operator( )( const Point &pt )
	{
		if ( !is_valid( pt ) )
		{
			throw InvalidPoint( );
		}

		return at( pt.x, pt.y, pt.z );
	}

	/// @brief øÌæfðæ¾·é
	/// @attention øªæÌÍÍOÌêC InvalidPoint áOð°é
	/// @param[in] pt æ¾·éæfÌÀW
	/// @return øÌÀWÌæfÌèQÆ
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