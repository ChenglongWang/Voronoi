/**
* @file dictionary.h
* @brief �A�z�z��R���e�i�N���X�̃w�b�_�t�@�C��
* @version 3.0
* @author ���É���w ���Ȋw������ ���f�B�A�Ȋw��U �X������   351203192   ���� �N�N
* @date 2013/10/18
*/

#ifndef _TMATSU_DICTIONARY_H_
#define _TMATSU_DICTIONARY_H_

#include <list>

namespace tmatsu 
{
	/// @brief �v�f���\�[�g���Ȃ��A�z�z��̃R���e�i�N���X
	/// @tparam KeyType �L�[�^
	/// @tparam ValueType �f�[�^�^
	/// @tparam Allocator �A���P�[�^�^�D�K��l��STL�̃f�t�H���g�A���P�[�^
	template < class KeyType, class ValueType, class Allocator = std::allocator< std::pair< KeyType, ValueType > > >
	class dictionary : public std::list< std::pair< KeyType, ValueType >, Allocator >
	{
	public:
		typedef KeyType key_type; ///< @brief �L�[
		typedef ValueType mapped_type; ///< @brief �l

		/// @brief �A�z�z��Ɋ܂܂�Ă��Ȃ��L�[���Q�Ƃ��悤�Ƃ���Ɠ��������O
		class invalid_key : public std::exception
		{
		};
	public:
		virtual ~dictionary();
		bool contains( const KeyType &key )const;
		mapped_type &operator[ ]( const KeyType &key );
		const mapped_type &operator[ ]( const KeyType &key )const;
	};

	/// @brief ���얳��
	template< class KeyType, class ValueType, class Allocator >
	inline dictionary< KeyType, ValueType, Allocator >::~dictionary( )
	{
	}

	/// @brief �L�[�����݂��邩���肷��
	/// @param[in] key ���肷��L�[
	/// @retval true �����̃L�[�͑��݂���
	/// @retval false �����̃L�[�͑��݂��Ȃ�		
	template< class KeyType, class ValueType, class Allocator >
	inline bool dictionary< KeyType, ValueType, Allocator >::contains( const KeyType &key )const
	{
		for ( const_iterator iter = begin( ); iter != end( ); iter++ )
		{
			if ( iter->first == key )
			{
				return true;
			}
		}

		return false;
	}

	/// @brief �l���擾����
	/// @attention �����̃L�[�����݂��Ȃ��ꍇ�C invalid_key ��O�𓊂���
	/// @attention �����_���A�N�Z�X�ɂ͗v�f���ɔ�Ⴗ�鎞�Ԃ��K�v
	/// @param[in] key �擾����l�̃L�[
	/// @return �����̃L�[�̎w���l�̎Q��
	template< class KeyType, class ValueType, class Allocator >
	inline typename dictionary< KeyType, ValueType, Allocator >::mapped_type &dictionary< KeyType, ValueType, Allocator >::operator[ ]( const KeyType &key )
	{
		for ( iterator iter = begin( ); iter != end( ); iter++ )
		{
			if ( iter->first == key )
			{
				return iter->second;
			}
		}

		throw invalid_key( );
	}

	/// @brief �l���擾����
	/// @attention �����̃L�[�����݂��Ȃ��ꍇ�C invalid_key ��O�𓊂���
	/// @attention �����_���A�N�Z�X�ɂ͗v�f���ɔ�Ⴗ�鎞�Ԃ��K�v
	/// @param[in] key �擾����l�̃L�[
	/// @return �����̃L�[�̎w���l�̒萔�Q��
	template< class KeyType, class ValueType, class Allocator >
	inline const typename dictionary< KeyType, ValueType, Allocator >::mapped_type &dictionary< KeyType, ValueType, Allocator >::operator[ ]( const KeyType &key )const
	{
		for ( const_iterator iter = begin( ); iter != end( ); iter++ )
		{
			if ( iter->first == key )
			{
				return iter->second;
			}
		}

		throw invalid_key( );
	}
}

#endif //_TMATSU_DICTIONARY_H_