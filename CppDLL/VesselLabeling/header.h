/**
* @file header.h
* @brief �w�b�_�N���X�̃w�b�_�t�@�C��
* @version 3.0
* @author ���É���w ���Ȋw������ ���f�B�A�Ȋw��U �X������   351203192   ���� �N�N
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
	typedef mist::vector3< size_t > SizeSet; ///< @brief �T�C�Y�̑g
	typedef mist::vector3< double > PitchSet; ///< @brief �s�b�`�̑g

	/// @brief �X�w�b�_��A�z�z��Ƃ��Ĉ����N���X
	class Header : public dictionary< std::string, std::string >
	{
	public:
		typedef invalid_key InvalidKey; ///< @brief �w�b�_�Ɋ܂܂�Ă��Ȃ��L�[���Q�Ƃ��悤�Ƃ���Ɠ��������O
	private:
		size_t max_len_; ///< @brief �L�[�̍ő咷
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

	/// @brief ����������
	inline Header::Header( ) : max_len_( 0 )
	{
	}

	/// @brief �w�b�_�𕡐�����
	/// @param[in] hdr ��������w�b�_
	inline Header::Header( const Header &hdr ) : dictionary< std::string, std::string >( hdr ), max_len_( hdr.max_len_ )
	{
	}

	/// @brief �w�b�_���t�@�C������ǂݍ���
	/// @param[in] file_name �ǂݍ��ރw�b�_�t�@�C����
	inline Header::Header( const char *file_name ) : max_len_( 0 )
	{
		read( file_name );
	}

	/// @brief ���얳��
	inline Header::~Header( )
	{
	}

	/// @brief �L�[�ƒl��}������
	/// @param[in] pair �}������L�[�ƒl�̑g
	/// @return �}�������L�[�ƒl�̑g���w���C�e���[�^
	template < class PairType >
	inline Header::iterator Header::insert( const PairType &pair )
	{
		max_len_ = std::max( pair.first.length( ), max_len_ );

		return dictionary< std::string, std::string >::insert( end( ), pair );
	}

	/// @brief �L�[�ƒl��}������
	/// @param[in] pos �}������ʒu�̎����w���C�e���[�^
	/// @param[in] pair �}������L�[�ƒl�̑g
	/// @return �}�������L�[�ƒl�̑g���w���C�e���[�^
	template < class PairType >
	inline Header::iterator Header::insert( const_iterator pos, const PairType &pair )
	{
		max_len_ = std::max( pair.first.length( ), max_len_ );

		return dictionary< std::string, std::string >::insert( pos, pair );
	}

	/// @brief �L�[�ƒl��}������
	/// @param[in] first �}������L�[�ƒl�̑g�̐擪���w���C�e���[�^
	/// @param[in] last �}������L�[�ƒl�̑g�̖����̎����w���C�e���[�^
	template< class Iterator >
	inline void Header::insert( Iterator first, Iterator last )
	{
		for ( IteratorType iter = first; iter != last; iter++ )
		{
			max_len_ = std::max( iter->first.length( ), max_len_ );
			dictionary< std::string, std::string >::insert( end( ), *iter );
		}
	}

	/// @brief �w�b�_���t�@�C������ǂݍ���
	/// @param[in] file_name �ǂݍ��ރw�b�_�t�@�C����
	/// @retval true �ǂݍ��݂ɐ�������
	/// @retval false �ǂݍ��݂Ɏ��s�����D���炭�C�t�@�C�����J���Ȃ�����
	inline bool Header::read( const char *file_name )
	{
		std::ifstream in_file( file_name ); // �w�b�_�t�@�C��

		// �G���[�`�F�b�N
		if ( !in_file )
		{
			return false;
		}

		// �ǂݍ���
		while ( true )
		{
			std::pair< std::string, std::string > buf; // �o�b�t�@
			char temp; // �ꎞ�ϐ�

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

	/// @brief �w�b�_���t�@�C���֏�������
	/// @param[in] file_name �������ރw�b�_�t�@�C����
	/// @retval true �������݂ɐ�������
	/// @retval false �������݂Ɏ��s�����D���炭�C�t�@�C�����J���Ȃ�����
	inline bool Header::write( const char *file_name )const
	{
		std::ofstream out_file( file_name ); // �w�b�_�t�@�C��

		// �G���[�`�F�b�N
		if ( !out_file )
		{
			return false;
		}

		// ��������
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

	/// @brief �t�@�C�������擾����
	/// @attention �K�v�ȃL�[���܂܂�Ă��Ȃ��Ƃ��C InvalidKey ��O�𓊂���
	/// @return �t�@�C����
	inline std::string Header::get_org_file( )const
	{
		return operator[ ]( "OrgFile" );
	}

	/// @brief �t�@�C������ݒ肷��
	/// @param[in] org_file �ݒ肷��t�@�C����
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
	
	/// @brief �T�C�Y�̑g���擾����
	/// @attention �K�v�ȃL�[���܂܂�Ă��Ȃ��Ƃ��C InvalidKey ��O�𓊂���
	/// @return �T�C�Y�̑g
	inline SizeSet Header::get_size( )const
	{
#if _MSC_VER >= 1600
		return SizeSet( std::stoi( operator[ ]( "SizeX" ) ), std::stoi( operator[ ]( "SizeY" ) ), std::stoi( operator[ ]( "SizeZ" ) ) );
#else
		return SizeSet( std::atoi( operator[ ]( "SizeX" ).c_str( ) ), std::atoi( operator[ ]( "SizeY" ).c_str( ) ), std::atoi( operator[ ]( "SizeZ" ) ).c_str( ) );
#endif
	}

	/// @brief �T�C�Y��ݒ肷��
	/// @param[in] siz �ݒ肷��T�C�Y�̑g
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
	
	/// @brief �s�b�`�̑g���擾����
	/// @attention �K�v�ȃL�[���܂܂�Ă��Ȃ��Ƃ��C InvalidKey ��O�𓊂���
	/// @return �s�b�`�̑g
	inline PitchSet Header::get_pitch( )const
	{
#if _MSC_VER >= 1600
		return PitchSet( std::stod( operator[ ]( "PitchX" ) ), std::stod( operator[ ]( "PitchY" ) ), std::stod( operator[ ]( "PitchZ" ) ) );
#else
		return PitchSet( std::atof( operator[ ]( "PitchX" ).c_str( ) ), std::atof( operator[ ]( "PitchY" ).c_str( ) ), std::atof( operator[ ]( "PitchZ" ).c_str( ) ) );
#endif
	}

	/// @brief �s�b�`��ݒ肷��
	/// @param[in] pitch �ݒ肷��s�b�`�̑g
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
	/// @brief �����𕶎���ɕϊ�����
	/// @param[in] a ������ɕϊ�����l
	/// @return ������
	template < class Type >
	std::string Header::to_string( const Type &a )
	{
		std::stringstream ss; // �X�g�����O�X�g���[��

		ss << a;
		return ss.str( );
	}
#endif
}

#endif //_TMATSU_HEADER_H_