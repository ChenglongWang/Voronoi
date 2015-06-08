/**
* @file image.h
* @brief �摜�N���X�̃w�b�_�t�@�C��
* @version 3.0
* @author ���É���w ���Ȋw������ ���f�B�A�Ȋw��U �X������   351203192   ���� �N�N
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
	typedef mist::vector3< size_t > Point; ///< @brief ���W
	typedef mist::vector3< ptrdiff_t > Shift; ///< @brief �ψ�
	typedef mist::vector3< double > Vector; ///< @brief �x�N�g��
	typedef mist::matrix< double > Matrix; ///< @brief �s��

	const Shift neighborhood6[ ] = {
		Shift( -1, 0, 0 ),
		Shift( 0, -1, 0 ),
		Shift( 0, 0, -1 ),
		Shift( 0, 0, 1 ),
		Shift( 0, 1, 0 ),
		Shift( 1, 0, 0 )
	}; ///< @brief 6�ߖT�Ɋ܂܂���f�ւ̕ψ�

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
	}; ///< @brief 18�ߖT�Ɋ܂܂���f�ւ̕ψ�

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
	}; ///< @brief 26�ߖT�Ɋ܂܂���f�ւ̕ψ�

	/// @brief �t�@�C�����o�͂Ȃǂ̋@�\��ǉ�����mist::array3�̃��b�p�[�N���X
	/// @tparam ValueType �f�[�^�^
	template < class ValueType >
	class Image : public mist::array3< ValueType > 
	{
	public:	
		typedef mist::__mist_dmy_callback__ DummyCallback; ///< @brief �_�~�[�R�[���o�b�N
		typedef mist::__mist_console_callback__ ConsoleCallback; ///< @brief �W���o�͌^�R�[���o�b�N
		typedef mist::__mist_progress_callback__ ProgressCallback; ///< @brief �i�s�󋵕\���^�R�[���o�b�N

		/// @brief �摜�͈̔͊O�̉�f���A�N�Z�X���悤�Ƃ���Ɠ��������O
		class InvalidPoint : std::exception
		{
		};
	private:
		Header hdr_; ///< @brief �w�b�_
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

	/// @brief ���얳��
	template < class ValueType >
	inline Image< ValueType >::Image( )
	{
	}
	
	/// @brief �摜�𕡐�����
	/// @param[in] img ��������摜
	template < class ValueType >
	inline Image< ValueType >::Image( const Image &img ) : mist::array3< ValueType >( img ), hdr_( img.hdr_ )
	{
	}

	/// @brief �^�̈قȂ�摜�𕡐�����
	/// @param[in] img ��������摜
	template < class ValueType >
	template < class ImageType >
	inline Image< ValueType >::Image( const ImageType &img ) : mist::array3< ValueType >( img )
	{
	}

	/// @brief �f�[�^�^�̈قȂ�摜�𕡐�����
	/// @param[in] img ��������摜
	template < class ValueType >
	template < class Type >
	inline Image< ValueType >::Image( const Image< Type > &img ) : mist::array3< ValueType >( img ), hdr_( img.header( ) )
	{
	}

	/// @brief �摜���t�@�C������ǂݍ���
	/// @param[in] callback �R�[���o�b�N�֐�
	/// @param[in] file_name �ǂݍ��ރt�@�C����
	/// @param[in] hdr_name �w�b�_�t�@�C�����D�K��l��NULL�DNULL���w�肳�ꂽ�ꍇ�Cfile_name��".header"��t�������w�b�_�t�@�C�������ݒ肳���
	template < class Type >
	template < class Functor >
	inline Image< Type >::Image( Functor callback, const char *file_name, const char *hdr_name )
	{
		read( callback, file_name, hdr_name );
	}

	/// @brief ���얳��
	template < class Type >
	inline Image< Type >::~Image( )
	{
	}

	/// @brief �摜���t�@�C������ǂݍ���
	/// @param[in] callback �R�[���o�b�N�֐�
	/// @param[in] file_name �ǂݍ��ރt�@�C����
	/// @param[in] hdr_name �w�b�_�t�@�C�����D�K��l��NULL�DNULL���w�肳�ꂽ�ꍇ�Cfile_name��".header"��t�������w�b�_�t�@�C�������ݒ肳���
	/// @retval true �摜�̓ǂݍ��݂ɐ�������
	/// @retval false �摜�̓ǂݍ��݂Ɏ��s�����D���炭�C�t�@�C�����J���Ȃ��������w�b�_�t�@�C�����s��������
	template < class ValueType >
	template < class Functor >
	inline bool Image< ValueType >::read( Functor callback, const char *file_name, const char *hdr_name )
	{
		SizeSet siz; // �T�C�Y
		PitchSet pitch; // �s�b�`

		// �w�b�_�t�@�C����ǂݍ���
		if ( !hdr_.read( hdr_name ? hdr_name : ( std::string( file_name ) + ".header" ).c_str( ) ) )
		{
			return false;
		}
		
		// �T�C�Y�ƃs�b�`��ǂݍ���
		try
		{
			siz = hdr_.get_size( );
			pitch = hdr_.get_pitch( );
		} 

		catch ( Header::InvalidKey )
		{
			return false;
		}

		// �摜�t�@�C����ǂݍ���
		return mist::read_raw( *this, file_name, siz.x, siz.y, siz.z, pitch.x, pitch.y, pitch.z, 0, false, callback );
	}	

	/// @brief �摜���t�@�C���֏�������
	/// @param[in] callback �R�[���o�b�N�֐�
	/// @param[in] file_name �������ރt�@�C����
	/// @param[in] hdr_name �w�b�_�t�@�C�����D�K��l��NULL�DNULL���w�肳�ꂽ�ꍇ�Cfile_name��".header"��t�������w�b�_�t�@�C�������ݒ肳���
	/// @retval true �摜�̏������݂ɐ�������
	/// @retval false �摜�̏������݂Ɏ��s�����D���炭�C�t�@�C�����J���Ȃ�����
	template < class ValueType >
	template < class Functor >
	inline bool Image< ValueType >::write( Functor callback, const char *file_name, const char *hdr_name )const
	{
		Header hdr = hdr_; // �o�̓w�b�_
		size_t offset = std::string( file_name ).find_last_of( '\\' ) + 1; //�I�t�Z�b�g
	
		// �t�@�C�������X�V����
		hdr.set_org_file( std::string( file_name ).substr( offset, std::strlen( file_name ) - offset ) );
	
		// �T�C�Y�ƃs�b�`���X�V����
		hdr.set_size( SizeSet( width( ), height( ), depth( ) ) );
		hdr.set_pitch( PitchSet( reso1( ), reso2( ), reso3( ) ) );

		// �w�b�_�t�@�C������������
		if ( !hdr.write( hdr_name ? hdr_name : ( std::string( file_name ) + ".header" ).c_str( ) ) )
		{
			return false;
		}

		// �摜�t�@�C���̏�������
		return mist::write_raw_gz( *this, file_name, 0, false, callback );
	}

	/// @brief �����̍��W���摜�͈͓̔��ɂ��邩���肷��
	/// @param[in] x ���肷��x���W
	/// @param[in] y ���肷��y���W
	/// @param[in] z ���肷��z���W
	/// @retval true ���W�͉摜�͈͓̔��ɂ���
	/// @retval false ���W�͉摜�͈̔͊O�ɂ���
	template < class ValueType >
	inline bool Image< ValueType >::is_valid( size_t x, size_t y, size_t z )const
	{
		return x < width( ) && y < height( ) && z < depth( );
	}

	/// @brief �����̍��W���摜�͈͓̔��ɂ��邩���肷��
	/// @param[in] pt ���肷����W
	/// @retval true ���W�͉摜�͈͓̔��ɂ���
	/// @retval false ���W�͉摜�͈̔͊O�ɂ���
	template < class ValueType >
	inline bool Image< ValueType >::is_valid( const Point &pt )const
	{
		return pt.x < width( ) && pt.y < height( ) && pt.z < depth( );
	}
		
	/// @brief �T�C�Y�̑g���擾����
	/// @return �T�C�Y�̑g
	template < class ValueType >
	inline SizeSet Image< ValueType >::get_size( )const
	{
		return SizeSet( width( ), height( ), depth( ) );
	}
	
	/// @brief �T�C�Y��ݒ肷��
	/// @param[in] siz �ݒ肷��T�C�Y�̑g
	template < class ValueType >
	inline void Image< ValueType >::set_size( const SizeSet &siz )
	{
		resize( siz.x, siz.y, siz.z );
	}
	
	/// @brief �s�b�`�̑g���擾����
	/// @return �s�b�`�̑g
	template < class ValueType >
	inline PitchSet Image< ValueType >::get_pitch( )const
	{
		return PitchSet( reso1( ), reso2( ), reso3( ) );
	}
	
	/// @brief �s�b�`��ݒ肷��
	/// @param[in] pitch �ݒ肷��s�b�`�̑g
	template < class ValueType >
	inline void Image< ValueType >::set_pitch( const PitchSet &pitch )
	{
		reso( pitch.x, pitch.y, pitch.z );
	}
	
	/// @brief �w�b�_���擾����
	/// @return �w�b�_�̎Q��
	template < class ValueType >
	inline Header &Image< ValueType >::header( )
	{
		return hdr_;
	}

	/// @brief �w�b�_���擾����
	/// @return �w�b�_�̒萔�Q��
	template < class ValueType >
	inline const Header &Image< ValueType >::header( )const
	{
		return hdr_;
	}

	/// @brief �摜�𕡐�����
	/// @param[in] img ��������摜
	/// @return �������g�̎Q��
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

	/// @brief �^�̈قȂ�摜�𕡐�����
	/// @param[in] img ��������摜
	/// @return �������g�̎Q��
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

	/// @brief �f�[�^�^�̈قȂ�摜�𕡐�����
	/// @param[in] img ��������摜
	/// @return �������g�̎Q��
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

	/// @brief �����̉�f���擾����
	/// @attention �������摜�͈̔͊O�̏ꍇ�C InvalidPoint ��O�𓊂���
	/// @param[in] x �擾�����f��x���W
	/// @param[in] y �擾�����f��y���W
	/// @param[in] z �擾�����f��z���W
	/// @return �����̍��W�̉�f�̎Q��
	template < class ValueType >
	inline ValueType &Image< ValueType >::operator( )( size_t x, size_t y, size_t z )
	{
		if ( !is_valid( x, y, z ) )
		{
			throw InvalidPoint( );
		}

		return at( x, y, z );
	}

	/// @brief �����̉�f���擾����
	/// @attention �������摜�͈̔͊O�̏ꍇ�C InvalidPoint ��O�𓊂���
	/// @param[in] x �擾�����f��x���W
	/// @param[in] y �擾�����f��y���W
	/// @param[in] z �擾�����f��z���W
	/// @return �����̍��W�̉�f�̒萔�Q��
	template < class ValueType >
	inline const ValueType &Image< ValueType >::operator( )( size_t x, size_t y, size_t z )const
	{
		if ( !is_valid( x, y, z ) )
		{
			throw InvalidPoint( );
		}

		return at( x, y, z );
	}

	/// @brief �����̉�f���擾����
	/// @attention �������摜�͈̔͊O�̏ꍇ�C InvalidPoint ��O�𓊂���
	/// @param[in] pt �擾�����f�̍��W
	/// @return �����̍��W�̉�f�̎Q��
	template < class ValueType >
	inline ValueType &Image< ValueType >::operator( )( const Point &pt )
	{
		if ( !is_valid( pt ) )
		{
			throw InvalidPoint( );
		}

		return at( pt.x, pt.y, pt.z );
	}

	/// @brief �����̉�f���擾����
	/// @attention �������摜�͈̔͊O�̏ꍇ�C InvalidPoint ��O�𓊂���
	/// @param[in] pt �擾�����f�̍��W
	/// @return �����̍��W�̉�f�̒萔�Q��
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