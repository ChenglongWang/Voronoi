/**
* @file dictionary.h
* @brief 連想配列コンテナクラスのヘッダファイル
* @version 3.0
* @author 名古屋大学 情報科学研究科 メディア科学専攻 森研究室   351203192   松﨑 哲朗
* @date 2013/10/18
*/

#ifndef _TMATSU_DICTIONARY_H_
#define _TMATSU_DICTIONARY_H_

#include <list>

namespace tmatsu 
{
	/// @brief 要素をソートしない連想配列のコンテナクラス
	/// @tparam KeyType キー型
	/// @tparam ValueType データ型
	/// @tparam Allocator アロケータ型．規定値はSTLのデフォルトアロケータ
	template < class KeyType, class ValueType, class Allocator = std::allocator< std::pair< KeyType, ValueType > > >
	class dictionary : public std::list< std::pair< KeyType, ValueType >, Allocator >
	{
	public:
		typedef KeyType key_type; ///< @brief キー
		typedef ValueType mapped_type; ///< @brief 値

		/// @brief 連想配列に含まれていないキーを参照しようとすると投げられる例外
		class invalid_key : public std::exception
		{
		};
	public:
		virtual ~dictionary();
		bool contains( const KeyType &key )const;
		mapped_type &operator[ ]( const KeyType &key );
		const mapped_type &operator[ ]( const KeyType &key )const;
	};

	/// @brief 動作無し
	template< class KeyType, class ValueType, class Allocator >
	inline dictionary< KeyType, ValueType, Allocator >::~dictionary( )
	{
	}

	/// @brief キーが存在するか判定する
	/// @param[in] key 判定するキー
	/// @retval true 引数のキーは存在する
	/// @retval false 引数のキーは存在しない		
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

	/// @brief 値を取得する
	/// @attention 引数のキーが存在しない場合， invalid_key 例外を投げる
	/// @attention ランダムアクセスには要素数に比例する時間が必要
	/// @param[in] key 取得する値のキー
	/// @return 引数のキーの指す値の参照
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

	/// @brief 値を取得する
	/// @attention 引数のキーが存在しない場合， invalid_key 例外を投げる
	/// @attention ランダムアクセスには要素数に比例する時間が必要
	/// @param[in] key 取得する値のキー
	/// @return 引数のキーの指す値の定数参照
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