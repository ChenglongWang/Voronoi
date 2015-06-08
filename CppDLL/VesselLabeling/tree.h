/**
* @file tree.h
* @brief 木構造コンテナクラスのヘッダファイル
* @version 3.0
* @author 名古屋大学 情報科学研究科 メディア科学専攻 森研究室   351203192   松﨑 哲朗
* @date 2013/10/18
*/

#ifndef _TMATSU_TREE_H_
#define _TMATSU_TREE_H_

#include <memory>
#include <exception>
#include <cstddef>
#include <climits>

namespace tmatsu 
{
	/// @brief 多分木を扱うコンテナクラス
	/// @tparam ValueType 木構造のノードの格納するデータ型
	/// @tparam Allocator アロケータ型．規定値はSTLのデフォルトアロケータ
	/// @attention 現在内部でアロケータは使用されていない
	template < class ValueType, class Allocator = std::allocator< ValueType > >
	class tree
	{
		/// @brief ノード
		struct Node
		{
			ValueType val; ///< @brief データ
			Node *parent; ///< @brief 親
			Node *eldest_child; ///< @brief 長子
			Node *youngest_child; ///< @brief 末子
			Node *elder_sibling; ///< @brief 兄
			Node *younger_sibling; ///< @brief 弟
		public:
			Node( );
			Node( const Node &node );
			Node( const ValueType &val );
			~Node( );
			Node *preorder_hd( );
			const Node *preorder_hd( )const;
			Node *preorder_tl( );
			const Node *preorder_tl( )const;
			const Node *preorder_incr( const Node *root )const;
			const Node *preorder_decr( const Node *root )const;
#ifdef _INORDER_ITERATOR
			Node *inorder_hd( );
			const Node *inorder_hd( )const;
			Node *inorder_tl( );
			const Node *inorder_tl( )const;
			const Node *inorder_incr( const Node *root )const;
			const Node *inorder_decr( const Node *root )const;
#endif //_INORDER_ITERATOR
			Node *postorder_hd( );
			const Node *postorder_hd( )const;
			Node *postorder_tl( );
			const Node *postorder_tl( )const;
			const Node *postorder_incr( const Node *root )const;
			const Node *postorder_decr( const Node *root )const;
			Node *levelorder_hd( );
			const Node *levelorder_hd( )const;
			Node *levelorder_tl( );
			const Node *levelorder_tl( )const;
			const Node *levelorder_incr( const Node *root )const;
			const Node *levelorder_decr( const Node *root )const;
			Node *child_hd( );
			const Node *child_hd( )const;
			Node *child_tl( );
			const Node *child_tl( )const;
			const Node *child_incr( const Node *root )const;
			const Node *child_decr( const Node *root )const;
		};

		/// @brief 部分木
		class Subtree : public tree 
		{
		public:
			Subtree( const Node *root );
			virtual ~Subtree( );
		};

	public:
		/// @brief 空の木構造に含まれる値を参照しようとすると投げられる例外
		class invalid_tree : public std::exception
		{
		};

		typedef ValueType value_type; ///< @brief 値
		typedef Allocator allocator_type; ///< @brief アロケータ
		typedef size_t size_type; ///< @brief 符号無し整数
		typedef ptrdiff_t difference_type; ///< @brief 符号付き整数
		typedef typename Allocator::reference reference; ///< @brief 参照
		typedef typename Allocator::const_reference const_reference; ///< @brief 定数参照
		typedef typename Allocator::pointer pointer; ///< @brief ポインタ
		typedef typename Allocator::const_pointer const_pointer; ///< @brief 定数ポインタ
	
		/// @brief 木構造を順方向に走査する前進イテレータ
		class const_iterator
		{
		public:
			typedef std::forward_iterator_tag iterator_category; ///< @brief イテレータカテゴリ
			typedef ValueType value_type; ///< @brief 値
			typedef size_t size_type; ///< @brief 符号無し整数
			typedef ptrdiff_t difference_type; ///< @brief 符号付き整数
			typedef typename Allocator::reference reference; ///< @brief 参照
			typedef typename Allocator::const_reference const_reference; ///< @brief 定数参照
			typedef typename Allocator::pointer pointer; ///< @brief ポインタ
			typedef typename Allocator::const_pointer const_pointer; ///< @brief 定数ポインタ

			/// @brief 有効な番地を指していないイテレータの値を参照しようとすると投げられる例外
			class invalid_iterator : public std::exception
			{
			};
		protected:
			const Node *node_; ///< @brief ノード
			const Node *root_; ///< @brief 根
			const Node *( Node::*incr_ )( const Node * )const; ///< @brief 前進関数
		public:
			const_iterator( );
			const_iterator( const const_iterator &iter );
			const_iterator( const Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const );
			const Node *node( )const;
			const Node *root( )const;
			const Node *( Node::*incr( ) )( const Node * )const;
			const_iterator &operator++( );
			const_iterator operator++( int );
#ifdef _BIDIRECTIONAL_ITERATOR
			const_iterator &operator--( );
			const_iterator operator--( int );
#endif //_BIDIRECTIONAL_ITERATOR
			bool operator==( const const_iterator &iter )const;
			bool operator!=( const const_iterator &iter )const;
			const_reference operator*( );
			const_pointer operator->( );
		};

		/// @brief 木構造を順方向に走査する前進イテレータ
		class iterator : public const_iterator
		{
		public:
			iterator( );
			iterator( const iterator &iter );
			iterator( Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const );
			Node *node( )const;
			iterator &operator++( );
			iterator operator++( int );
#ifdef _BIDIRECTIONAL_ITERATOR
			iterator &operator--( );
			iterator operator--( int );
#endif //_BIDIRECTIONAL_ITERATOR
			reference operator*( );
			pointer operator->( );
		};

		/// @brief 木構造を逆方向に走査する前進イテレータ
		class const_reverse_iterator
		{
		public:
			typedef std::forward_iterator_tag iterator_category; ///< @brief イテレータカテゴリ
			typedef ValueType value_type; ///< @brief 値
			typedef size_t size_type; ///< @brief 符号無し整数
			typedef ptrdiff_t difference_type; ///< @brief 符号付き整数
			typedef typename Allocator::reference reference; ///< @brief 参照
			typedef typename Allocator::const_reference const_reference; ///< @brief 定数参照
			typedef typename Allocator::pointer pointer; ///< @brief ポインタ
			typedef typename Allocator::const_pointer const_pointer; ///< @brief 定数ポインタ

			/// @brief 有効な番地を指していないイテレータの値を参照しようとすると投げられる例外
			class invalid_iterator : public std::exception
			{
			};
		protected:
			const Node *node_; ///< @brief ノード
			const Node *root_; ///< @brief 根
			const Node *( Node::*incr_ )( const Node * )const; ///< @brief 前進関数
		public:
			const_reverse_iterator( );
			const_reverse_iterator( const const_reverse_iterator &iter );
			const_reverse_iterator( const Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const );
			const Node *node( )const;
			const Node *root( )const;
			const Node *( Node::*incr( ) )( const Node * )const;
			const_reverse_iterator &operator++( );
			const_reverse_iterator operator++( int );
#ifdef _BIDIRECTIONAL_ITERATOR
			const_reverse_iterator &operator--( );
			const_reverse_iterator operator--( int );
#endif //_BIDIRECTIONAL_ITERATOR
			bool operator==( const const_reverse_iterator &iter )const;
			bool operator!=( const const_reverse_iterator &iter )const;
			const_reference operator*( );
			const_pointer operator->( );
		};

		/// @brief 木構造を逆方向に走査する前進イテレータ
		class reverse_iterator : public const_reverse_iterator
		{
		public:
			reverse_iterator( );
			reverse_iterator( const reverse_iterator &iter );
			reverse_iterator( Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const );
			Node *node( )const;
			reverse_iterator &operator++( );
			reverse_iterator operator++( int );
#ifdef _BIDIRECTIONAL_ITERATOR
			reverse_iterator &operator--( );
			reverse_iterator operator--( int );
#endif //_BIDIRECTIONAL_ITERATOR
			reference operator*( );
			pointer operator->( );
		};
	private:
		Node *root_; ///< @brief 根
	public:
		tree( );
		tree( const tree &tree );
		explicit tree( const Allocator &alloc );
		explicit tree( size_t cnt );
		tree( size_t cnt, const ValueType &val );
		tree( size_t cnt, const ValueType &val, const Allocator &alloc );
		template < class Iterator >
		tree( Iterator first, Iterator last );
		template < class Iterator >
		tree( Iterator first, Iterator last, const Allocator &alloc );
		virtual ~tree( );
		iterator begin( );
		const_iterator begin( )const;
		reverse_iterator rbegin( );
		const_reverse_iterator rbegin( )const;
		iterator end( );
		const_iterator end( )const;
		reverse_iterator rend( );
		const_reverse_iterator rend( )const;
		iterator preorder_begin( );
		const_iterator preorder_begin( )const;
		reverse_iterator preorder_rbegin( );
		const_reverse_iterator preorder_rbegin( )const;
		iterator preorder_end( );
		const_iterator preorder_end( )const;
		reverse_iterator preorder_rend( );
		const_reverse_iterator preorder_rend( )const;
#ifdef _INORDER_ITERATOR
		iterator inorder_begin( );
		const_iterator inorder_begin( )const;
		reverse_iterator inorder_rbegin( );
		const_reverse_iterator inorder_rbegin( )const;
		iterator inorder_end( );
		const_iterator inorder_end( )const;
		reverse_iterator inorder_rend( );
		const_reverse_iterator inorder_rend( )const;
#endif //_INORDER_ITERATOR
		iterator postorder_begin( );
		const_iterator postorder_begin( )const;
		reverse_iterator postorder_rbegin( );
		const_reverse_iterator postorder_rbegin( )const;
		iterator postorder_end( );
		const_iterator postorder_end( )const;
		reverse_iterator postorder_rend( );
		const_reverse_iterator postorder_rend( )const;
		iterator levelorder_begin( );
		const_iterator levelorder_begin( )const;
		reverse_iterator levelorder_rbegin( );
		const_reverse_iterator levelorder_rbegin( )const;
		iterator levelorder_end( );
		const_iterator levelorder_end( )const;
		reverse_iterator levelorder_rend( );
		const_reverse_iterator levelorder_rend( )const;
		template < class Iterator >
		iterator child_begin( Iterator parent );
		template < class Iterator >
		const_iterator child_begin( Iterator parent )const;
		template < class Iterator >
		reverse_iterator child_rbegin( Iterator parent );
		template < class Iterator >
		const_reverse_iterator child_rbegin( Iterator parent )const;
		template < class Iterator >
		iterator child_end( Iterator parent );
		template < class Iterator >
		const_iterator child_end( Iterator parent )const;
		template < class Iterator >
		reverse_iterator child_rend( Iterator parent );
		template < class Iterator >
		const_reverse_iterator child_rend( Iterator parent )const;
		template < class Iterator >
		void assign( Iterator first, Iterator last );
		void assign( size_t cnt, const ValueType &val );
		void swap( tree &tree );
		allocator_type get_allocator( )const;
		size_t size( )const;
		size_t max_size( )const;
		bool empty( )const;
		void resize( size_t cnt );
		void resize( size_t cnt, const ValueType &val );
		reference root( );
		const_reference root( )const;
		iterator insert( const_iterator parent, const ValueType &val );
		template < class Type >
		iterator insert( const_iterator parent, Type val );
		void insert( const_iterator parent, size_t cnt, const ValueType &val );
		template < class Iterator >
		void insert( const_iterator parent, Iterator first, Iterator last );
		void insert( const_iterator parent, const tree &tree );
		void insert( const_iterator parent, size_t cnt, const tree &tree );
		iterator insert( const_iterator parent, const_iterator younger_sibling, const ValueType &val );
		template < class Type >
		iterator insert( const_iterator parent, const_iterator younger_sibling, Type val );
		void insert( const_iterator parent, const_iterator younger_sibling, size_t cnt, const ValueType &val );
		template < class Iterator >
		void insert( const_iterator parent, const_iterator younger_sibling, Iterator first, Iterator last );
		void insert( const_iterator parent, const_iterator younger_sibling, const tree &tree );
		void insert( const_iterator parent, const_iterator younger_sibling, size_t cnt, const tree &tree );
		iterator erase( const_iterator pos );
		iterator erase( const_iterator first, const_iterator last );
		void clear( );
		template < class Iterator >
		Iterator parent( Iterator child )const;
		template < class Iterator >
		size_t depth( Iterator iter )const;
		template < class Iterator >
		bool is_root( Iterator iter )const;
		template < class Iterator >
		bool is_leaf( Iterator iter )const;
		template < class Iterator >
		bool has_sibling( Iterator iter )const;
		template < class Iterator, class ReferenceIterator >
		bool is_ancestor( Iterator iter, ReferenceIterator ref_iter )const;
		template < class Iterator, class ReferenceIterator >
		bool is_descendant( Iterator iter, ReferenceIterator ref_iter )const;
		template < class Iterator >
		Subtree subtree( Iterator root )const;
		template < class Iterator >
		void reverse( Iterator root, tree &tree );
		tree &operator=( const tree &tree );
		bool operator==( const tree &tree )const;
		bool operator!=( const tree &tree )const;
		bool operator<( const tree &tree )const;
		bool operator<=( const tree &tree )const;
		bool operator>( const tree &tree )const;
		bool operator>=( const tree &tree )const;
	private:
		explicit tree( const Node *root );
	};

	/// @brief 初期化する
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( ) : root_( NULL )
	{
	}
	
	/// @brief 木構造を複製する
	/// @param[in] tree 複製する木構造
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( const tree &tree ) : root_( NULL )
	{
		operator=( tree );
	}

	/// @brief 初期化する
	/// @param[in] alloc アロケータ
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( const Allocator &alloc ) : root_( NULL )
	{
	}

	/// @brief 引数の数だけデータを確保し，根およびその子とする
	/// @param[in] cnt データの数
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( size_t cnt ) : root_( NULL )
	{
		insert( begin( ), cnt, ValueType( ) );
	}

	/// @brief 引数の数だけ引数のデータを確保し，根およびその子とする
	/// @param[in] cnt データの数
	/// @param[in] val データの初期値
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( size_t cnt, const ValueType &val ) : root_( NULL )
	{
		insert( begin( ), cnt, val );
	}

	/// @brief 引数の数だけデータを確保し，根およびその子とする
	/// @param[in] cnt データの数
	/// @param[in] val データの初期値
	/// @param[in] alloc アロケータ
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( size_t cnt, const ValueType &val, const Allocator &alloc ) : root_( NULL )
	{
		insert( begin( ), cnt, val );
	}


	/// @brief 引数のデータを複製し，根およびその子とする
	/// @attention 引数が木構造である場合，親子関係は保存されない
	/// @param[in] first 複製するデータの先頭を指すイテレータ
	/// @param[in] last 複製するデータの末尾の次を指すイテレータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline tree< ValueType, Allocator >::tree( Iterator first, Iterator last ) : root_( NULL )
	{
		insert( begin( ), first, last );
	}

	/// @brief 引数のデータを複製し，根およびその子とする
	/// @attention 引数が木構造である場合，親子関係は保存されない
	/// @param[in] first 複製するデータの先頭を指すイテレータ
	/// @param[in] last 複製するデータの末尾の次を指すイテレータ
	/// @param[in] alloc アロケータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline tree< ValueType, Allocator >::tree( Iterator first, Iterator last, const Allocator &alloc ) : root_( NULL )
	{
		insert( begin( ), first, last );
	}
	
	/// @brief 引数のノードを根とする木構造の深い複製を作成する
	/// @param[in] root...複製する木構造の根
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( const Node *root ) : root_( const_cast< Node * >( root ) )
	{
	}


	/// @brief 領域を開放する
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::~tree( )
	{
		delete root_;
		root_ = NULL;
	}

	/// @brief 先頭を指すイテレータを取得する
	/// @return 先頭を指すイテレータ
	///
	/// 使用例
	/// @code
	/// tmatsu::tree< int > t; // 木構造
	///
	/// // 木構造の各ノードの値を印字する
	/// for ( tmatsu::tree< int >::iterator iter = t.begin( ); iter != t.end( ); iter++ )
	/// {
	///		for ( size_t i = 0; i < t.depth( iter ); i++ )
	///		{
	///			std::cout << ' ';
	///		}
	///
	///		std::cout << *iter << std::endl;
	/// }
	/// @endcode
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::begin( )
	{
		return preorder_begin( );
	}

	/// @brief 先頭を指す定数イテレータを取得する
	/// @return 先頭を指す定数イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::begin( )const	
	{
		return preorder_begin( );
	}

	/// @brief 末尾を指す逆イテレータを取得する
	/// @return 末尾を指す逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::rbegin( )
	{
		return preorder_rbegin( );
	}

	/// @brief 末尾を指す定数逆イテレータを取得する
	/// @return 末尾を指す定数逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::rbegin( )const	
	{
		return preorder_rbegin( );
	}

	/// @brief 末尾の次を指すイテレータを取得する
	/// @return 末尾の次を指すイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::end( )
	{
		return preorder_end( );
	}

	/// @brief 末尾の次を指す定数イテレータを取得する
	/// @return 末尾の次を指す定数イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::end( )const
	{
		return preorder_end( );
	}

	/// @brief 先頭の前を指す逆イテレータを取得する
	/// @return 先頭の前を指す逆イテレータ	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::rend( )
	{
		return preorder_rend( );
	}

	/// @brief 先頭の前を指す定数逆イテレータを取得する
	/// @return 先頭の前を指す定数逆イテレータ	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::rend( )const
	{
		return preorder_rend( );
	}

	/// @brief 先頭を指す先行順(深さ優先)のイテレータを取得する
	/// @return 先頭を指す先行順(深さ優先)のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::preorder_begin( )
	{
		return iterator( root_ ? root_->preorder_hd( ) : NULL, root_, &Node::preorder_incr );
	}

	/// @brief 先頭を指す先行順(深さ優先)の定数イテレータを取得する
	/// @return 先頭を指す先行順(深さ優先)の定数イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::preorder_begin( )const
	{
		return const_iterator( root_ ? root_->preorder_hd( ) : NULL, root_, &Node::preorder_incr );
	}

	/// @brief 末尾を指す先行順(深さ優先)の逆イテレータを取得する
	/// @return 末尾を指す先行順(深さ優先)の逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::preorder_rbegin( )
	{
		return reverse_iterator( root_ ? root_->preorder_tl( ) : NULL, root_, &Node::preorder_decr );
	}

	/// @brief 末尾を指す先行順(深さ優先)の定数逆イテレータを取得する
	/// @return 末尾を指す先行順(深さ優先)の定数逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::preorder_rbegin( )const
	{
		return const_reverse_iterator( root_ ? root_->preorder_tl( ) : NULL, root_, &Node::preorder_decr );
	}

	/// @brief 末尾の次を指す先行順(深さ優先)のイテレータを取得する
	/// @return 末尾の次を指す先行順(深さ優先)のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::preorder_end( )
	{
		return iterator( NULL, root_, &Node::preorder_incr );
	}

	/// @brief 末尾の次を指す先行順(深さ優先)の定数イテレータを取得する
	/// @return 末尾の次を指す先行順(深さ優先)の定数イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::preorder_end( )const
	{
		return const_iterator( NULL, root_, &Node::preorder_incr );
	}

	/// @brief 先頭の前を指す先行順(深さ優先)の逆イテレータを取得する
	/// @return 先頭の前を指す先行順(深さ優先)の逆イテレータ	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::preorder_rend( )
	{
		return reverse_iterator( NULL, root_, &Node::preorder_decr );
	}

	/// @brief 先頭の前を指す先行順(深さ優先)の定数逆イテレータを取得する
	/// @return 先頭の前を指す先行順(深さ優先)の定数逆イテレータ	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::preorder_rend( )const
	{
		return const_reverse_iterator( NULL, root_, &Node::preorder_decr );
	}

#if _INORDER_ITERATOR
	/// @brief 先頭を指す中間順のイテレータを取得する
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 未実装のため使用してはならない
 	/// @return 先頭を指す中間順のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::inorder_begin( )
	{
		return iterator( root_ ? root_->inorder_hd( ) : NULL, NULL, &Node::inorder_incr, &Node::inorder_decr );
	}

	/// @brief 先頭を指す中間順の定数イテレータを取得する
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 未実装のため使用してはならない
 	/// @return 先頭を指す中間順の定数イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::inorder_begin( )const
	{
		return const_iterator( root_ ? root_->inorder_hd( ) : NULL, NULL, &Node::inorder_incr, &Node::inorder_decr );
	}

	/// @brief 末尾を指す中間順の逆イテレータを取得する
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 未実装のため使用してはならない
 	/// @return 末尾を指す中間順の逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::inorder_rbegin( )
	{
		return reverse_iterator( root_ ? root_->inorder_tl( ) : NULL, NULL, &Node::inorder_decr, &Node::inorder_incr );
	}

	/// @brief 末尾を指す中間順の定数逆イテレータを取得する
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 未実装のため使用してはならない
 	/// @return 末尾を指す中間順の定数逆イテレータ	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::inorder_rbegin( )const
	{
		return const_reverse_iterator( root_ ? root_->inorder_tl( ) : NULL, NULL, &Node::inorder_decr, &Node::inorder_incr );
	}

	/// @brief 末尾の次を指す中間順のイテレータを取得する
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 未実装のため使用してはならない
 	/// @return 末尾の次を指す中間順のイテレータ	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::inorder_end( )
	{
		return iterator( NULL, root_ ? root_->inorder_tl( ) : NULL, &Node::inorder_incr, &Node::inorder_decr );
	}

	/// @brief 末尾の次を指す中間順の定数イテレータを取得する
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 未実装のため使用してはならない
 	/// @return 末尾の次を指す中間順の定数イテレータ	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::inorder_end( )const
	{
		return const_iterator( NULL, root_ ? root_->inorder_tl( ) : NULL, &Node::inorder_incr, &Node::inorder_decr );
	}

	/// @brief 先頭の前を指す中間順の逆イテレータを取得する
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 未実装のため使用してはならない
 	/// @return 先頭の前を指す中間順の逆イテレータ		
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::inorder_rend( )
	{
		return reverse_iterator( NULL, root_ ? root_->inorder_hd( ) : NULL, &Node::inorder_decr, &Node::inorder_incr );
	}
	
	/// @brief 先頭の前を指す中間順の定数逆イテレータを取得する
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 未実装のため使用してはならない
 	/// @return 先頭の前を指す中間順の定数逆イテレータ		
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::inorder_rend( )const
	{
		return const_reverse_iterator( NULL, root_ ? root_->inorder_hd( ) : NULL, &Node::inorder_decr, &Node::inorder_incr );
	}
#endif //_INORDER_ITERATOR

	/// @brief 先頭を指す後行順のイテレータを取得する
	/// @return 先頭を指す後行順のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::postorder_begin( )
	{
		return iterator( root_ ? root_->postorder_hd( ) : NULL, root_, &Node::postorder_incr );
	}

	/// @brief 先頭を指す後行順の定数イテレータを取得する
	/// @return 先頭を指す後行順の定数イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::postorder_begin( )const
	{
		return const_iterator( root_ ? root_->postorder_hd( ) : NULL, root_, &Node::postorder_incr );
	}

	/// @brief 末尾を指す後行順の逆イテレータを取得する
	/// @return 末尾を指す後行順の逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::postorder_rbegin( )
	{
		return reverse_iterator( root_ ? root_->postorder_tl( ) : NULL, root_, &Node::postorder_decr );
	}

	/// @brief 末尾を指す後行順の定数逆イテレータを取得する
	/// @return 末尾を指す後行順の定数逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::postorder_rbegin( )const
	{
		return const_reverse_iterator( root_ ? root_->postorder_tl( ) : NULL, root_, &Node::postorder_decr );
	}

	/// @brief 末尾の次を指す後行順のイテレータを取得する
	/// @return 末尾の次を指す後行順のイテレータ
	template < class ValueType, class Allocator >	
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::postorder_end( )
	{
		return iterator( NULL, root_, &Node::postorder_incr );
	}
	
	/// @brief 末尾の次を指す後行順の定数イテレータを取得する
	/// @return 末尾の次を指す後行順の定数イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::postorder_end( )const
	{
		return const_iterator( NULL, root_, &Node::postorder_incr );
	}
	
	/// @brief 先頭の前を指す後行順の逆イテレータを取得する
	/// @return 先頭の前を指す後行順の逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::postorder_rend( )
	{
		return reverse_iterator( NULL, root_, &Node::postorder_decr );
	}

	/// @brief 先頭の前を指す後行順の定数逆イテレータを取得する
	/// @return 先頭の前を指す後行順の定数逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::postorder_rend( )const
	{
		return const_reverse_iterator( NULL, root_, &Node::postorder_decr );
	}

	/// @brief 先頭を指すレベル順(幅優先)のイテレータを取得する
	/// @attention 未実装のため使用してはならない
	/// @return	先頭を指すレベル順(幅優先)のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::levelorder_begin( )
	{
		return iterator( root_ ? root_->levelorder_hd( ) : NULL, root_, &Node::levelorder_incr );
	}

	/// @brief 先頭を指すレベル順(幅優先)の定数イテレータを取得する
	/// @attention 未実装のため使用してはならない
	/// @return	先頭を指すレベル順(幅優先)の定数イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::levelorder_begin( )const
	{
		return const_iterator( root_ ? root_->levelorder_hd( ) : NULL, root_, &Node::levelorder_incr );
	}

	/// @brief 末尾を指すレベル順(幅優先)の逆イテレータを取得する
	/// @attention 未実装のため使用してはならない
	/// @return	末尾を指すレベル順(幅優先)の逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::levelorder_rbegin( )
	{
		return reverse_iterator( root_ ? root_->levelorder_tl( ) : NULL, root_, &Node::levelorder_decr );
	}

	/// @brief 末尾を指すレベル順(幅優先)の定数逆イテレータを取得する
	/// @attention 未実装のため使用してはならない
	/// @return	末尾を指すレベル順(幅優先)の定数逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::levelorder_rbegin( )const
	{
		return const_reverse_iterator( root_ ? root_->levelorder_tl( ) : NULL, root_, &Node::levelorder_decr );
	}

	/// @brief 末尾の次を指すレベル順(幅優先)のイテレータを取得する
	/// @attention 未実装のため使用してはならない
	/// @return	末尾の次を指すレベル順(幅優先)のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::levelorder_end( )
	{
		return iterator( NULL, root_, &Node::levelorder_incr );
	}
	
	/// @brief 末尾の次を指すレベル順(幅優先)の定数イテレータを取得する
	/// @attention 未実装のため使用してはならない
	/// @return	末尾の次を指すレベル順(幅優先)の定数イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::levelorder_end( )const
	{
		return const_iterator( NULL, root_, &Node::levelorder_incr );
	}
	
	/// @brief 先頭の前を指すレベル順(幅優先)の逆イテレータを取得する
	/// @attention 未実装のため使用してはならない
	/// @return	先頭の前を指すレベル順(幅優先)の逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::levelorder_rend( )
	{
		return reverse_iterator( NULL, root_, &Node::levelorder_decr );
	}

	/// @brief 先頭の前を指すレベル順(幅優先)の定数逆イテレータを取得する
	/// @attention 未実装のため使用してはならない
	/// @return	先頭の前を指すレベル順(幅優先)の定数逆イテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::levelorder_rend( )const
	{
		return const_reverse_iterator( NULL, root_, &Node::levelorder_decr );
	}

	/// @brief 先頭を指す子供のイテレータを取得する
	/// @param[in] parent 取得するイテレータの親を指すイテレータ
	/// @return	先頭を指す子供のイテレータ
	///
	/// 使用例
	/// @code
	/// tmatsu::tree< int > t; // 木構造
	/// tmatsu::tree< int >::iterator node = std::find( t.begin( ), t.end( ), 100 ); // 値が100のノードを指すイテレータ
	/// 
	/// // 子供の各ノードの値を印字する
	/// for ( tmatsu::tree< int >::iterator iter = t.child_begin( node ); iter != t.child_end( node ); iter++ )
	/// {
	///		std::cout << *iter << std::endl;
	/// }
	///
	/// // 兄弟の各ノードの値を印字する
	/// for ( tmatsu::tree< int >::iterator iter = t.child_begin( t.parent( node ) ); iter != t.child_end( t.parent( node ) ); iter++ )
	/// {
	///		if ( iter != node )
	///		{
	///			std::cout << *iter << std::endl;
	///		}
	/// }
	/// @endcode
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::child_begin( Iterator parent )
	{
		return iterator( parent.node( ) ? const_cast< Node * >( parent.node( )->child_hd( ) ) : NULL, root_, &Node::child_incr );
	}

	/// @brief 先頭を指す子供の定数イテレータを取得する
	/// @param[in] parent 取得するイテレータの親を指すイテレータ
	/// @return	先頭を指す子供の定数イテレータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::child_begin( Iterator parent )const
	{
		return const_iterator( parent.node( ) ? parent.node( )->child_hd( ) : NULL, root_, &Node::child_incr );
	}
	
	/// @brief 末尾を指す子供の逆イテレータを取得する
	/// @param[in] parent 取得するイテレータの親を指すイテレータ
	/// @return	末尾を指す子供の逆イテレータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::child_rbegin( Iterator parent )
	{
		return reverse_iterator( parent.node( ) ? const_cast< Node * >( parent.node( )->child_tl( ) ) : NULL, root_, &Node::child_decr );
	}

	/// @brief 末尾を指す子供の定数逆イテレータを取得する
	/// @param[in] parent 取得するイテレータの親を指すイテレータ
	/// @return	末尾を指す子供の定数逆イテレータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::child_rbegin( Iterator parent )const
	{
		return const_reverse_iterator( parent.node( ) ? parent.node( )->child_tl( ) : NULL, root_, &Node::child_decr );
	}

	/// @brief 末尾の次を指す子供のイテレータを取得する
	/// @param[in] parent 取得するイテレータの親を指すイテレータ
	/// @return	末尾の次を指す子供のイテレータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::child_end( Iterator parent )
	{
		return iterator( NULL, root_, &Node::child_incr );
	}

	/// @brief 末尾の次を指す子供の定数イテレータを取得する
	/// @param[in] parent 取得するイテレータの親を指すイテレータ
	/// @return	末尾の次を指す子供の定数イテレータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::child_end( Iterator parent )const
	{
		return const_iterator( NULL, root_, &Node::child_incr );
	}

	/// @brief 先頭の前を指す子供の逆イテレータを取得する
	/// @param[in] parent 取得するイテレータの親を指すイテレータ
	/// @return	先頭の前を指す子供の逆イテレータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::child_rend( Iterator parent )
	{
		return reverse_iterator( NULL, root_, &Node::child_decr );
	}

	/// @brief 先頭の前を指す子供の定数逆イテレータを取得する
	/// @param[in] parent 取得するイテレータの親を指すイテレータ
	/// @return	先頭の前を指す子供の定数逆イテレータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::child_rend( Iterator parent )const
	{
		return const_reverse_iterator( NULL, root_, &Node::child_decr );
	}

	/// @brief 引数のデータを代入し，根およびその子とする
	/// @attention 引数が木構造であった場合，親子関係は保存されない
	/// @param[in] first 複製するデータの先頭を指すイテレータ
	/// @param[in] last 複製するデータの末尾の次を指すイテレータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline void tree< ValueType, Allocator >::assign( Iterator first, Iterator last )
	{
		clear( );
		insert( begin( ), first, last );
	}

	/// @brief 引数の数だけデータを代入し，根およびその子とする
	/// @param[in] cnt　代入するデータの数
	/// @param[in] val 代入するデータ
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::assign( size_t cnt, const ValueType &val )
	{
		clear( );
		insert( begin( ), cnt, val );
	}

	/// @brief 木構造を入れ替える
	/// @param[inout] tree 入れ替える木構造
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::swap( tree &tree )
	{
		std::swap( root_, tree.root_ );
	}

	/// @brief アロケータを取得する
	/// @attention 現在内部でアロケータが使用されていないため無意味な値が返る
	/// @return アロケータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::allocator_type tree< ValueType, Allocator >::get_allocator( )const
	{
		return Allocator( );
	}
	
	/// @brief データ数を取得する
	/// @return データ数
	template < class ValueType, class Allocator >
	inline size_t tree< ValueType, Allocator >::size( )const
	{
		size_t cnt = 0; //データ数
	
		for ( const_iterator iter = begin( ); iter != end( ); iter++ )
		{
			cnt++;
		}

		return cnt;
	}

	/// @brief 最大データ数を取得する
	/// @attention 未実装のため使用してはならない
	/// @return 最大データ数
	template < class ValueType, class Allocator >
	inline size_t tree< ValueType, Allocator >::max_size( )const
	{
		return SIZE_MAX;
	}
	
	/// @brief 木構造が空か判定する
	/// @retval true 木構造は空である
	/// @retval fasle 木構造は空でない
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::empty( )const
	{
		return !root_;
	}

	/// @brief データ数を変更し，根およびその子として新しく確保したノードをデフォルトコンストラクタで初期化する
	/// @deprecated 元の値や親子関係は保存されないため使用は推奨されない
	/// @param[in] cnt 変更後のデータ数
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::resize( size_t cnt )
	{
		clear( );
		insert( begin( ), cnt, ValueType( ) );
	}

	/// @brief データ数を変更し，根およびその子として新しく確保したノードを引数で初期化する
	/// @deprecated 親子関係は保存されないため使用は推奨されない
	/// @param[in] cnt 変更後のデータ数
	/// @param[in] val 変更後のデータ
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::resize( size_t cnt, const ValueType &val )
	{
		clear( );
		insert( begin( ), cnt, val );
	}

	/// @brief 根を取得する
	/// @attention 木構造が空である場合， invalid_tree 例外を投げる
	/// @return 根の参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reference tree< ValueType, Allocator >::root( )
	{
		if ( root_ )
		{
			return root_->val;
		}
		else
		{
			throw invalid_tree( );
		}
	}

	/// @brief 根を取得する
	/// @attention 木構造が空である場合， invalid_tree 例外を投げる
	/// @return 根の定数参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reference tree< ValueType, Allocator >::root( )const
	{
		if ( root_ )
		{
			return root_->val;
		}
		else
		{
			throw invalid_tree( );
		}
	}

	/// @brief 引数のデータを挿入する
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] val 挿入するデータ
	/// @return 挿入したデータを指すイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::insert( const_iterator parent, const ValueType &val )
	{
		if ( root_ )
		{
			Node *parent_node = parent.node( ) ? const_cast< Node * >( parent.node( ) ) : root_; // 挿入するノードの親
			Node *child_node = new Node( val ); // 挿入するノード

			// ノードを連結する
			child_node->parent = parent_node;
			child_node->elder_sibling = parent_node->youngest_child;
			parent_node->youngest_child = child_node;

			if ( child_node->elder_sibling )
			{
				child_node->elder_sibling->younger_sibling = child_node;
			}

			if ( !parent_node->eldest_child )
			{
				parent_node->eldest_child = child_node;
			}
		
			// 引数の位置に挿入する
			return iterator( child_node, root_, parent.incr( ) );
		}
		else
		{
			// 木構造が空のとき根として挿入する
			return iterator( root_ = new Node( val ), root_, parent.incr( ) );
		}
	}

	/// @brief 引数のデータを挿入する
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] val 挿入するデータ
	/// @return 挿入したデータを指すイテレータ
	template < class ValueType, class Allocator >
	template < class Type >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::insert( const_iterator parent, Type val )
	{
		return insert( parent, static_cast< ValueType >( val ) );
	}

	/// @brief 引数の数だけデータを挿入する
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] cnt 挿入するデータの数
	/// @param[in] val 挿入するデータ
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, size_t cnt, const ValueType &val )
	{
		for ( size_t i = 0; i < cnt; i++ )
		{
			insert( parent, val );
		}
	}

	/// @brief 引数のデータを挿入する
	/// @attention 引数が木構造である場合，親子関係は保存されない
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] first 挿入するデータの先頭を指すイテレータ
	/// @param[in] last 挿入するデータの末尾の次を指すイテレータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, Iterator first, Iterator last )
	{
		for ( ; first != last; first++ )
		{
			insert( parent, *first );
		}
	}

	/// @brief 引数の木構造を挿入する
	/// @attention 未実装のため使用してはならない
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] tree 挿入する木構造
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, const tree &tree )
	{
	}

	/// @brief 引数の数だけ木構造を挿入する
	/// @attention 未実装のため使用してはならない
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] cnt 挿入する木構造の数
	/// @param[in] tree 挿入する木構造
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, size_t cnt, const tree &tree )
	{
		for ( size_t i = 0; i < cnt; i++ )
		{
			insert( parent, tree );
		}
	}
	
	/// @brief 引数のデータを挿入する
	/// @attention 未実装のため使用してはならない
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] younger_sibling 挿入する位置の弟を指すイテレータ
	/// @param[in] val 挿入するデータ
	/// @return 挿入したデータを指すイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, const ValueType &val )
	{
	}

	/// @brief 引数のデータを挿入する
	/// @attention 未実装のため使用してはならない
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] younger_sibling 挿入する位置の弟を指すイテレータ
	/// @param[in] val 挿入するデータ
	/// @return 挿入したデータを指すイテレータ
	template < class ValueType, class Allocator >
	template < class Type >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, Type val )
	{
		return insert( parent, younger_sibling, static_cast< ValueType >( val ) );
	}

	/// @brief 引数の数だけデータを挿入する
	/// @attention 未実装のため使用してはならない
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] younger_sibling 挿入する位置の弟を指すイテレータ
	/// @param[in] cnt 挿入するデータの数
	/// @param[in] val 挿入するデータ
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, size_t cnt, const ValueType &val )
	{
		for ( size_t i = 0; i < cnt; i++ )
		{
			insert( parent, younger_sibling, val );
		}
	}

	/// @brief 引数のデータを挿入する
	/// @attention 未実装のため使用してはならない
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] younger_sibling 挿入する位置の弟を指すイテレータ
	/// @param[in] first 挿入するデータの先頭を指すイテレータ
	/// @param[in] last 挿入するデータの末尾の次を指すイテレータ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, Iterator first, Iterator last )
	{
		for ( ; first != last; first++ )
		{
			insert( parent, younger_sibling, *first );
		}
	}

	/// @brief 引数の木構造を挿入する
	/// @attention 未実装のため使用してはならない
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] younger_sibling 挿入する位置の弟を指すイテレータ
	/// @param[in] tree 挿入する木構造
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, const tree &tree )
	{
	}

	/// @brief 引数の数だけ木構造を挿入する
	/// @attention 未実装のため使用してはならない
	/// @param[in] parent 挿入する位置の親を指すイテレータ
	/// @param[in] younger_sibling 挿入する位置の弟を指すイテレータ
	/// @param[in] cnt 挿入する木構造の数
	/// @param[in] tree 挿入する木構造
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, size_t cnt, const tree &tree )
	{
		for ( size_t i = 0; i < cnt; i++ )
		{
			insert( parent, younger_sibling, tree );
		}
	}

	/// @brief 引数のデータを削除する
	/// @param[in] pos 削除する位置を指すイテレータ
	/// @return 削除したデータの次を指すイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::erase( const_iterator pos )
	{
		Node *current = const_cast< Node * >( pos.node( ) ); // 現在のノード

		if ( current )
		{
			Node *parent = pos.node( )->parent; // 削除するノードの親
			iterator next = static_cast< iterator & >( pos ); // 次のノードを指すイテレータ

			// 前進する
			next++;

			// 再連結する
			if ( parent )
			{
				// 兄と長子または弟を連結する
				if ( current->elder_sibling )
				{
					current->elder_sibling->younger_sibling = current->eldest_child ? current->eldest_child : current->younger_sibling;
				}
				
				if ( current->eldest_child )
				{
					current->eldest_child->elder_sibling = current->elder_sibling;
				}
				else if ( current->younger_sibling )
				{
					current->younger_sibling->elder_sibling = current->elder_sibling;
				}
			
				// 弟と末子又は兄を連結する
				if ( current->younger_sibling )
				{
					current->younger_sibling->elder_sibling = current->youngest_child ? current->youngest_child : current->elder_sibling;
				}
				
				if ( current->youngest_child )
				{
					current->youngest_child->younger_sibling = current->younger_sibling;
				}
				else if ( current->elder_sibling )
				{
					current->elder_sibling->younger_sibling = current->younger_sibling;
				}

				// 親と子供を連結する
				for ( Node *p = current->eldest_child; p; p = p->younger_sibling )
				{
					p->parent = parent;
				}

				if ( parent->eldest_child == current )
				{
					parent->eldest_child = current->eldest_child ? current->eldest_child : current->younger_sibling;
				}

				if ( parent->youngest_child == current )
				{
					parent->youngest_child = current->youngest_child ? current->youngest_child : current->elder_sibling;
				}
			
				// イテレータの根を削除するとき根を更新する
				if ( current == next.root( ) )
				{
					next = iterator( next.node( ), current->parent, next.incr( ) );
				}
			}
			else
			{
				// 根を削除するとき長子を根に変更する
				root_ = current->eldest_child;

				if ( root_ )
				{
					// 根の子供と弟を連結する
					if ( root_->youngest_child )
					{
						root_->youngest_child->younger_sibling = root_->younger_sibling;
					}

					// 根の弟を子供に変更する
					for ( Node *p = root_->younger_sibling; p; p = p->younger_sibling )
					{
						p->parent = root_;
					}

					if ( !root_->eldest_child )
					{
						root_->eldest_child = root_->younger_sibling;
					}

					if ( current->youngest_child != root_ )
					{
						root_->youngest_child = current->youngest_child;
					}

					// 根の連結を切断する
					root_->parent = root_->younger_sibling = NULL;
				}
			
				// イテレータの根を削除するとき根を更新する
				if ( current == next.root( ) )
				{
					next = iterator( next.node( ), root_, next.incr( ) );
				}
			}

			// 削除する
			current->eldest_child = current->younger_sibling = NULL;
			delete current;

			return next;
		}
		else
		{
			// イテレータが終端を指しているとき動作しない
			return iterator( static_cast< iterator & >( pos ) );
		}
	}

	/// @brief 引数のデータを削除する
	/// @attention 未実装のため使用してはならない
	/// @param[in] first 削除するデータの先頭を指すイテレータ
	/// @param[in] last 削除するデータの末尾の次を指すイテレータ
	/// @return 削除したデータの次を指すイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::erase( const_iterator first, const_iterator last )
	{
	}

	/// @brief 全てのデータを削除する
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::clear( )
	{
		delete root_;
		root_ = NULL;
	}

	/// @brief 引数の親を指すイテレータを取得する
	/// @attention 引数が根である場合， end( ) が返る
	/// @param[in] child 取得するイテレータの子を指すイテレータ
	/// @return	引数の親を指すイテレータ
	///
	/// 使用例
	/// @code
	/// tmatsu::tree< int > t; // 木構造
	/// tmatsu::tree< int >::const_iterator node = std::find( t.begin( ), t.end( ), 100 ); // 値が100のノードを指すイテレータ
	/// 
	/// // 先祖の各ノードの値を印字する
	/// for ( tmatsu::tree< int >::const_iterator iter = t.parent( node ); iter != t.end( ); iter = t.parent( iter ) )
	/// {
	///		for ( size_t i = 0; i < t.depth( iter ); i++ )
	///		{
	///			std::cout << ' ';
	///		}
	///
	///		std::cout << *iter << std::endl;
	/// }
	/// @endcode
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline Iterator tree< ValueType, Allocator >::parent( Iterator child )const
	{
		return Iterator( child.node( ) ? child.node( )->parent : NULL, child.root( ), child.incr( ) );
	}
		
	/// @brief イテレータの指すノードの深さを取得する
	/// @param[in] iter 深さを取得するノードを指すイテレータ
	/// @return イテレータの指すノードの深さ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline size_t tree< ValueType, Allocator >::depth( Iterator iter )const
	{
		size_t dpt = 0; // 深さ

		for ( const Node *p = iter.node( ); p && p != root_; p = p->parent )
		{
			dpt++;
		}

		return dpt;
	}
	
	/// @brief イテレータの指すノードが根か判定する
	/// @param[in] iter 判定するノードを指すイテレータ
	/// @retval true イテレータの指すノードは根である
	/// @retval false イテレータの指すノードは根でない
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline bool tree< ValueType, Allocator >::is_root( Iterator iter )const
	{
		return iter.node( ) == iter.root( );
	}

	/// @brief イテレータの指すノードが葉か判定
	/// @param[in] iter 判定するノードを指すイテレータ
	/// @retval true イテレータの指すノードは葉である
	/// @retval false イテレータの指すノードは葉でない
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline bool tree< ValueType, Allocator >::is_leaf( Iterator iter )const
	{
		return !iter.node( )->eldest_child;
	}
	
	/// @brief イテレータの指すノードが兄弟を持つか判定
	/// @param[in] iter 判定するノードを指すイテレータ
	/// @retval true イテレータの指すノードは兄弟を持つ
	/// @retval false イテレータの指すノードは兄弟を持つ
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline bool tree< ValueType, Allocator >::has_sibling( Iterator iter )const
	{
		return ( iter.node( )->younger_sibling || iter.node( )->elder_sibling ) && !is_root( iter );
	}
	
	/// @brief iterがref_iterの先祖か判定
	/// @param[in] iter 判定するノードを指すイテレータ
	/// @param[in] ref_iter 基準となるノードを指すイテレータ
	/// @retval true iterはref_iterの先祖である
	/// @retval false iterはref_iterの先祖でない
	template < class ValueType, class Allocator >
	template < class Iterator, class ReferenceIterator >
	inline bool tree< ValueType, Allocator >::is_ancestor( Iterator iter, ReferenceIterator ref_iter )const
	{
		while ( !is_root( ref_iter ) )
		{
			ref_iter = parent( ref_iter );

			if ( iter == ref_iter )
			{
				return true;
			}
		}

		return false;
	}
	
	/// @brief iterがref_iterの子孫か判定
	/// @param[in] iter 判定するノードを指すイテレータ
	/// @param[in] ref_iter 基準となるノードを指すイテレータ
	/// @retval true iterはref_iterの子孫である
	/// @retval false iterはref_iterの子孫でない
	template < class ValueType, class Allocator >
	template < class Iterator, class ReferenceIterator >
	inline bool tree< ValueType, Allocator >::is_descendant( Iterator iter, ReferenceIterator ref_iter )const
	{
		return is_ancestor( ref_iter, iter );
	}

	/// @brief 引数を根とする部分木を取得する
	/// @attention 取得する部分木は浅い複製なので，実体が欲しい場合はコピーコンストラクタまたは代入演算子を利用して部分木を複製する
	/// @param[in] root 取得する部分木の根を指すイテレータ
	/// @return 引数を根とする部分木
	///
	/// 使用例
	/// @code
	/// tmatsu::tree< int > t; // 木構造
	/// tmatsu::tree< int > &s = t.subtree( std::find( t.begin( ), t.end( ), 100 ) ); // 値が100のノードを根とする部分木
	/// 
	/// // 部分木の各ノードの値を印字する
	/// for ( tmatsu::tree< int >::const_iterator iter = s.begin( ); iter != s.end( ); iter++ )
	/// {
	///		for ( size_t i = 0; i < s.depth( iter ); i++ )
	///		{
	///			std::cout << ' ';
	///		}
	///
	///		std::cout << *iter << std::endl;
	/// }
	/// @endcode
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::Subtree tree< ValueType, Allocator >::subtree( Iterator root )const
	{
		return Subtree( root.node( ) );
	}

	/// @brief 逆向きの木構造を取得する
	/// @attention 未実装のため使用してはならない
	/// @param[in] root 取得する木構造の根となるノードを指すイテレータ
	/// @param[out] tree 取得する木構造を格納する領域の参照
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline void tree< ValueType, Allocator >::reverse( Iterator root, tree &tree )
	{
	}

	/// @brief 木構造を複製する
	/// @param[in] tree 複製する木構造
	/// @return 自分自身の参照
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator > &tree< ValueType, Allocator >::operator=( const tree &tree )
	{
		if ( this != &tree )
		{
			clear( );

			if ( tree.root_ )
			{
				Node *temp = tree.root_->younger_sibling; // 複製する木構造の根の弟を退避する

				tree.root_->younger_sibling = NULL;
				root_ = new Node( *tree.root_ );
				tree.root_->younger_sibling = temp;
			}
		}

		return *this;
	}

	/// @brief 引数と等しい木構造か判定する
	/// @param[in] tree 判定する木構造
	/// @retval true 全てのノードが引数と等しい
	/// @retval false 引数と異なるノードが存在する
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator==( const tree &tree )const
	{
		for ( const_iterator left = begin( ), right = tree.begin( ); left != end( ) && right != tree.end( ); left++, right++ )
		{
			size_t left_child_cnt = 0, right_child_cnt = 0; // 子供の数

			// 値が等しいか調べる
			if ( *left != *right )
			{
				return false;
			}

			// 子供の数が等しいか調べる
			for ( const_iterator iter = child_begin( left ); iter != child_end( left ); iter++ )
			{
				left_child_cnt++;
			}

			for ( const_iterator iter =tree.child_begin( right ); iter != tree.child_end( right ); iter++ )
			{
				right_child_cnt++;
			}

			if ( left_child_cnt != right_child_cnt )
			{
				return false;
			}
		}

		return true;
	}

	/// @brief 引数と等しくない木構造か判定する
	/// @param[in] tree 判定する木構造
	/// @retval true 引数と異なるノードが存在する
	/// @retval false 全てのノードが引数と等しい
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator!=( const tree &tree )const
	{
		return !operator==( tree );
	}

	/// @brief 引数より辞書順で小さい木構造か判定する
	/// @param[in] tree 判定する木構造
	/// @retval true 引数より辞書順で小さい木構造である
	/// @retval false 引数より辞書順で大きいまたは等しい木構造である
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator<( const tree &tree )const
	{
		const_iterator left, right; // イテレータ

		for ( left = begin( ), right = tree.begin( ); left != end( ) && right != tree.end( ); left++, right++ )
		{
			if ( *left < *right )
			{
				return true;
			}
			else if ( *right < *left )
			{
				return false;
			}
		}

		return left == end( ) && right != tree.end( );
	}

	/// @brief 引数より辞書順で小さいまたは等しい木構造か判定する
	/// @param[in] tree 判定する木構造
	/// @retval true 引数より辞書順で小さいまたは等しい木構造である
	/// @retval false 引数より辞書順で大きい木構造である
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator<=( const tree &tree )const
	{
		return !operator>( tree );
	}

	/// @brief 引数より辞書順で大きい木構造か判定する
	/// @param[in] tree 判定する木構造
	/// @retval true 引数より辞書順で大きい木構造である
	/// @retval false 引数より辞書順で小さいまたは等しい木構造である
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator>( const tree &tree )const
	{
		return tree.operator<( *this );
	}

	/// @brief 引数より辞書順で大きいまたは等しい木構造か判定する
	/// @param[in] tree 判定する木構造
	/// @retval true 引数より辞書順で大きいまたは等しい木構造である
	/// @retval false 引数より辞書順で小さい木構造である
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator>=( const tree &tree )const
	{
		return !operator<( tree );
	}

	///@brief 初期化する
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Node::Node( ) : parent( NULL ), eldest_child( NULL ), youngest_child( NULL ), elder_sibling( NULL ), younger_sibling( NULL )
	{
	}
		
	/// @brief ノードとその兄弟および子孫を複製する
	/// @param[in] node 複製するノード
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Node::Node( const Node &node ) : val( node.val ), parent( NULL ), eldest_child( NULL ), youngest_child( NULL ), elder_sibling( NULL ), younger_sibling( NULL )
	{
		// 子供を複製する
		if ( node.eldest_child )
		{
			eldest_child = new Node( *node.eldest_child );

			for ( youngest_child = eldest_child; youngest_child && youngest_child->younger_sibling; youngest_child = youngest_child->younger_sibling )
			{
				youngest_child->parent = this;
			}

			if ( youngest_child )
			{
				youngest_child->parent = this;
			}
		}
	
		// 兄弟を複製する
		if ( node.younger_sibling )
		{
			younger_sibling = new Node( *node.younger_sibling );
			younger_sibling->elder_sibling = this;
		}
	}

	/// @brief 引数で初期化する
	/// @param[in] val データの初期値
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Node::Node( const ValueType &val ) : val( val ), parent( NULL ), eldest_child( NULL ), youngest_child( NULL ), elder_sibling( NULL ), younger_sibling( NULL )
	{
	}

	/// @brief 領域を解放する
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Node::~Node( )
	{
		delete eldest_child;
		delete younger_sibling;
		eldest_child = younger_sibling = NULL;
	}

	/// @brief 先行順(深さ優先)の先頭のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 先行順(深さ優先)の先頭のノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_hd( )
	{
		return this;
	}

	/// @brief 先行順(深さ優先)の先頭のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 先行順(深さ優先)の先頭のノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_hd( )const
	{
		return this;
	}

	/// @brief 先行順(深さ優先)の末尾のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 先行順(深さ優先)の末尾のノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_tl( )
	{
		return youngest_child ? youngest_child->preorder_tl( ) : this;
	}

	/// @brief 先行順(深さ優先)の末尾のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 先行順(深さ優先)の末尾のノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_tl( )const
	{
		return youngest_child ? youngest_child->preorder_tl( ) : this;
	}
		
	/// @brief 先行順(深さ優先)で前進する
	/// @attention 直後に走査するノードが存在しない場合，NULLが返る
	/// @param[in] root 根を指すポインタ
	/// @return 先行順(深さ優先)で直後に走査するノードのポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_incr( const Node *root )const
	{
		if ( eldest_child )
		{
			// 子供がいるとき長子を返す
			return eldest_child;
		}
		else
		{
			for ( const Node *p = this; p != root; p = p->parent )
			{
				if ( p->younger_sibling )
				{
					// 自分自身または先祖に弟がいるとき弟を返す
					return p->younger_sibling;
				}
			}
		}

		// 返すノードが存在しないときヌルポインタを返す
		return NULL;
	}	

	/// @brief 先行順(深さ優先)で後退する
	/// @attention 直前に走査するノードが存在しない場合，NULLが返る
	/// @param[in] root 根を指すポインタ
	/// @return 先行順(深さ優先)で直前に走査するノードのポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_decr( const Node *root )const
	{
		if ( this == root )
		{
			// 返すノードが存在しないときヌルポインタを返す
			return NULL;
		}

		for ( const Node *p = elder_sibling; p; p = p->youngest_child )
		{
			if ( !p->youngest_child )
			{
				// 兄またはその子孫の末子を返す
				return p;
			}
		}
			
		// 根でないとき親を返す
		return parent;
	}

#ifdef _INORDER_ITERATOR
	/// @brief 中間順の先頭のノードを取得する
	/// @attention 未実装のため使用してはならない
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 存在しない場合，NULLが返る
	/// @return 中間順の先頭のノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_hd( )
	{
		return eldest_child ? eldest_child->inorder_hd( ) : this;
	}

	/// @brief 中間順の先頭のノードを取得する
	/// @attention 未実装のため使用してはならない
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 存在しない場合，NULLが返る
	/// @return 中間順の先頭のノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_hd( )const
	{
		return eldest_child ? eldest_child->inorder_hd( ) : this;
	}

	/// @brief 中間順の末尾のノードを取得する
	/// @attention 未実装のため使用してはならない
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 存在しない場合，NULLが返る
	/// @return 中間順の末尾のノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_tl( )
	{
		return youngest_child ? youngest_child->inorder_tl( ) : this;
	}

	/// @brief 中間順の末尾のノードを取得する
	/// @attention 未実装のため使用してはならない
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 存在しない場合，NULLが返る
	/// @return 中間順の末尾のノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_tl( )const
	{
		return youngest_child ? youngest_child->inorder_tl( ) : this;
	}

	/// @brief 中間順で前進する
	/// @attention 未実装のため使用してはならない
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 直後に走査するノードが存在しない場合，NULLが返る
	/// @param[in] root 根を指すポインタ
	/// @return 中間順で直後に走査するノードのポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_incr( const Node *root )const
	{
	}

	/// @brief 中間順で後退する
	/// @attention 未実装のため使用してはならない
	/// @attention 木構造が二分木でない場合の動作は不定
	/// @attention 直前に走査するノードが存在しない場合，NULLが返る
	/// @param[in] root 根を指すポインタ
	/// @return 中間順で直前に走査するノードのポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_decr( const Node *root )const
	{
	}
#endif //_INORDER_ITERATOR

	/// @brief 後行順の先頭のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 後行順の先頭のノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_hd( )
	{
		return eldest_child ? eldest_child->postorder_hd( ) : this;
	}

	/// @brief 後行順の先頭のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 後行順の先頭のノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_hd( )const
	{
		return eldest_child ? eldest_child->postorder_hd( ) : this;
	}

	/// @brief 後行順の末尾のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 後行順の末尾のノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_tl( )
	{
		return this;
	}

	/// @brief 後行順の末尾のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 後行順の末尾のノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_tl( )const
	{
		return this;
	}

	/// @brief 後行順で前進する
	/// @attention 直後に走査するノードが存在しない場合，NULLが返る
	/// @param[in] root 根を指すポインタ
	/// @return 後行順で直後に走査するノードのポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_incr( const Node *root )const
	{
		if ( this == root )
		{
			// 返すノードが存在しないときヌルポインタを返す
			return NULL;
		}

		for ( const Node *p = younger_sibling; p; p = p->eldest_child )
		{
			if ( !p->eldest_child )
			{
				// 弟またはその子孫の長子を返す
				return p;
			}
		}
			
		// 根でないとき親を返す
		return parent;
	}

	/// @brief 後行順で後退する
	/// @attention 直前に走査するノードが存在しない場合，NULLが返る
	/// @param[in] root 根を指すポインタ
	/// @return 後行順で直前に走査するノードのポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_decr( const Node *root )const
	{
		if ( youngest_child )
		{
			// 子供がいるとき末子を返す
			return youngest_child;
		}
		else
		{
			for ( const Node *p = this; p != root; p = p->parent )
			{
				if ( p->elder_sibling )
				{
					// 自分自身または先祖に兄がいるとき弟を返す
					return p->elder_sibling;
				}
			}
		}

		// 返すノードが存在しないときヌルポインタを返す
		return NULL;
	}

	/// @brief レベル順(幅優先)の先頭のノードを取得する
	/// @attention 未実装のため使用してはならない
	/// @attention 存在しない場合，NULLが返る
	/// @return レベル順(幅優先)の先頭のノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_hd( )
	{
		return this;
	}

	/// @brief レベル順(幅優先)の先頭のノードを取得する
	/// @attention 未実装のため使用してはならない
	/// @attention 存在しない場合，NULLが返る
	/// @return レベル順(幅優先)の先頭のノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_hd( )const
	{
		return this;
	}

	/// @brief レベル順(幅優先)の末尾のノードを取得する
	/// @attention 未実装のため使用してはならない
	/// @attention 存在しない場合，NULLが返る
	/// @return レベル順(幅優先)の末尾のノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_tl( )
	{
	}

	/// @brief レベル順(幅優先)の末尾のノードを取得する
	/// @attention 未実装のため使用してはならない
	/// @attention 存在しない場合，NULLが返る
	/// @return レベル順(幅優先)の末尾のノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_tl( )const
	{
	}

	/// @brief レベル順(幅優先)で前進する
	/// @attention 未実装のため使用してはならない
	/// @attention 直後に走査するノードが存在しない場合，NULLが返る
	/// @param[in] root 根を指すポインタ
	/// @return レベル順(幅優先)で直後に走査するノードのポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_incr( const Node *root )const
	{
	}

	/// @brief レベル順(幅優先)で後退する
	/// @attention 未実装のため使用してはならない
	/// @attention 直前に走査するノードが存在しない場合，NULLが返る
	/// @param[in] root 根を指すポインタ
	/// @return レベル順(幅優先)で直前に走査するノードのポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_decr( const Node *root )const
	{
	}

	/// @brief 長子のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 長子のノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_hd( )
	{
		return eldest_child;
	}

	/// @brief 長子のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 長子のノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_hd( )const
	{
		return eldest_child;
	}

	/// @brief 末子のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 末子のノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_tl( )
	{
		return youngest_child;
	}

	/// @brief 長子のノードを取得する
	/// @attention 存在しない場合，NULLが返る
	/// @return 長子のノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_tl( )const
	{
		return youngest_child;
	}

	/// @brief 兄弟間で前進する
	/// @attention 直後に走査するノードが存在しない場合，NULLが返る
	/// @param[in] root 根を指すポインタ
	/// @return 兄弟間で直後に走査するノードのポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_incr( const Node *root )const
	{
		return younger_sibling;
	}

	/// @brief 兄弟間で後退する
	/// @attention 直前に走査するノードが存在しない場合，NULLが返る
	/// @param[in] root 根を指すポインタ
	/// @return 兄弟間で直前に走査するノードのポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_decr( const Node *root )const
	{
		return elder_sibling;
	}

	/// @brief 初期化する
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_iterator::const_iterator( ) : node_( NULL ), root_( NULL ), incr_( &Node::preorder_incr )
	{
	}

	/// @brief イテレータを複製する
	/// @param[in] iter 複製するイテレータ
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_iterator::const_iterator( const const_iterator &iter ) : node_( iter.node_ ), root_( iter.root_ ), incr_( iter.incr_ )
	{
	}

	/// @brief 引数で初期化する
	/// @param[in] node イテレータの指すノードのポインタ 
	/// @param[in] root 走査する木構造の根のポインタ
	/// @param[in] incr 前進関数
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_iterator::const_iterator( const Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const ) : node_( node ), root_( root ), incr_( incr )
	{
	}

	/// @brief イテレータが指すノードを取得する
	/// @return イテレータが指すノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::const_iterator::node( )const
	{
		return node_;
	}

	/// @brief 走査する木構造の根を取得する
	/// @return 走査する木構造の根のポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::const_iterator::root( )const
	{
		return root_;
	}
	
	/// @brief 前進関数を取得する
	/// @return 前進関数
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *( tree< ValueType, Allocator >::Node::*tree< ValueType, Allocator >::const_iterator::incr( ) )( const Node * )const
	{
		return incr_;
	}

	/// @brief 前進する
	/// @return 前進後のイテレータの参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator &tree< ValueType, Allocator >::const_iterator::operator++( )
	{
		if ( node_ )
		{
			node_ = ( node_->*incr_ )( root_ );
		}

		return *this;
	}

	/// @brief 前進する
	/// @param ダミー
	/// @return 前進前のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::const_iterator::operator++( int )
	{
		const_iterator temp = *this; // 現在のノード

		operator++( );

		return temp;
	}

#ifdef _BIDIRECTIONAL_ITERATOR
	/// @brief 後退する
	/// @attention 未実装のため使用してはならない
	/// @return 後退後のイテレータの参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator &tree< ValueType, Allocator >::const_iterator::operator--( )
	{
		if ( prev_ )
		{
			node_ = prev_;
			prev_ = ( prev_->*decr_ )( );
		}
	
		return *this;
	}

	/// @brief 後退する
	/// @attention 未実装のため使用してはならない
	/// @param ダミー
	/// @return 後退前のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::const_iterator::operator--( int )
	{
		const_iterator temp = *this; // 現在のノード

		operator--( );

		return temp;
	}
#endif //_BIDIRECTIONAL_ITERATOR

	/// @brief イテレータが引数と等しいか判定する
	/// @attention 走査方法が同一であるかは考慮しない
	/// @param[in] iter 判定するイテレータ
	/// @retval true 引数と同じノードを指している
	/// @retval false 引数と異なるノードを指している
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::const_iterator::operator==( const const_iterator &iter )const
	{
		return node_ == iter.node_; // && incr_ == iter.incr_;
	}

	/// @brief イテレータが引数と等しくないか判定する
	/// @attention 走査方法が同一であるかは考慮しない
	/// @param[in] iter 判定するイテレータ
	/// @retval true 引数と異なるノードを指している
	/// @retval false 引数と同じノードを指している
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::const_iterator::operator!=( const const_iterator &iter )const
	{
		return !operator==( iter );
	}

	/// @brief 参照剥がしを行う
	/// @attention 無効なノードを指している場合， invalid_iterator 例外を投げる
	/// @return イテレータの指すデータの定数参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reference tree< ValueType, Allocator >::const_iterator::operator*( )
	{
		if ( node_ )
		{
			return node_->val;
		}
		else
		{
			throw invalid_iterator( );
		}
	}

	/// @brief 間接メンバ参照を行う
	/// @attention 無効なノードを指している場合， invalid_iterator 例外を投げる
	/// @return イテレータの指すオブジェクトの定数ポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_pointer tree< ValueType, Allocator >::const_iterator::operator->( )
	{
		if ( node_ )
		{
			return &node_->val;
		}
		else
		{
			throw invalid_iterator( );
		}
	}

	/// @brief 初期化する
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::iterator::iterator( )
	{
	}

	/// @brief イテレータを複製する
	/// @param[in] iter 複製するイテレータ
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::iterator::iterator( const iterator &iter ) : const_iterator( iter )
	{
	}

	/// @brief 引数で初期化する
	/// @param[in] node イテレータの指すノードのポインタ 
	/// @param[in] root 走査する木構造の根のポインタ
	/// @param[in] incr 前進関数
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::iterator::iterator( Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const ) : const_iterator( node, root, incr )
	{
	}

	/// @brief イテレータが指すノードを取得する
	/// @return イテレータが指すノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::iterator::node( )const
	{
		return const_cast< Node * >( node_ );
	}

	/// @brief 前進する
	/// @return 前進後のイテレータの参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator &tree< ValueType, Allocator >::iterator::operator++( )
	{
		if ( node_ )
		{
			node_ = ( node_->*incr_ )( root_ );
		}
	
		return *this;
	}

	/// @brief 前進する
	/// @param ダミー
	/// @return 前進前のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::iterator::operator++( int )
	{
		iterator temp = *this; // 現在のノード

		operator++( );

		return temp;
	}

#ifdef _BIDIRECTIONAL_ITERATOR
	/// @brief 後退する
	/// @attention 未実装のため使用してはならない
	/// @return 後退後のイテレータの参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator &tree< ValueType, Allocator >::iterator::operator--( )
	{
		if ( prev_ )
		{
			node_ = prev_;
			prev_ = ( prev_->*decr_ )( );
		}

		return *this;
	}

	/// @brief 後退する
	/// @attention 未実装のため使用してはならない
	/// @param ダミー
	/// @return 後退前のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::iterator::operator--( int )
	{
		iterator temp = *this; // 現在のノード

		operator--( );

		return temp;
	}
#endif //_BIDIRECTIONAL_ITERATOR

	/// @brief 参照剥がしを行う
	/// @attention 無効なノードを指している場合， invalid_iterator 例外を投げる
	/// @return イテレータの指すデータの参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reference tree< ValueType, Allocator >::iterator::operator*( )
	{
		if ( node_ )
		{
			return const_cast< ValueType & >( node_->val );
		}
		else
		{
			throw invalid_iterator( );
		}
	}

	/// @brief 間接メンバ参照を行う
	/// @attention 無効なノードを指している場合， invalid_iterator 例外を投げる
	/// @return イテレータの指すオブジェクトのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::pointer tree< ValueType, Allocator >::iterator::operator->( )
	{
		if ( node_ )
		{
			return const_cast< ValueType * >( &node_->val );
		}
		else
		{
			throw invalid_iterator( );
		}
	}

	/// @brief 初期化する
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_reverse_iterator::const_reverse_iterator( ) : node_( NULL ), root_( NULL ), incr_( &Node::*preorder_incr )
	{
	}

	/// @brief イテレータを複製する
	/// @param[in] iter 複製するイテレータ
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_reverse_iterator::const_reverse_iterator( const const_reverse_iterator &iter ) : node_( iter.node_ ), root_( iter.root_ ), incr_( iter.incr_ )
	{
	}

	/// @brief 引数で初期化する
	/// @param[in] node イテレータの指すノードのポインタ 
	/// @param[in] root 走査する木構造の根のポインタ
	/// @param[in] incr 前進関数
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_reverse_iterator::const_reverse_iterator( const Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const ) : node_( node ), root_( root ), incr_( incr )
	{
	}

	/// @brief イテレータが指すノードを取得する
	/// @return イテレータが指すノードの定数ポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::const_reverse_iterator::node( )const
	{
		return node_;
	}

	/// @brief 走査する木構造の根を取得する
	/// @return 走査する木構造の根のポインタ
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::const_reverse_iterator::root( )const
	{
		return root_;
	}
	
	/// @brief 前進関数を取得する
	/// @return 前進関数
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *( tree< ValueType, Allocator >::Node::*tree< ValueType, Allocator >::const_reverse_iterator::incr( ) )( const Node * )const
	{
		return incr_;
	}

	/// @brief 前進する
	/// @return 前進後のイテレータの参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator &tree< ValueType, Allocator >::const_reverse_iterator::operator++( )
	{
		if ( node_ )
		{
			node_ = ( node_->*incr_ )( root_ );
		}

		return *this;
	}

	/// @brief 前進する
	/// @param ダミー
	/// @return 前進前のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::const_reverse_iterator::operator++( int )
	{
		const_reverse_iterator temp = *this; // 現在のノード

		operator++( );

		return temp;
	}

#ifdef _BIDIRECTIONAL_ITERATOR
	/// @brief 後退する
	/// @attention 未実装のため使用してはならない
	/// @return 後退後のイテレータの参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator &tree< ValueType, Allocator >::const_reverse_iterator::operator--( )
	{
		if ( prev_ )
		{
			node_ = prev_;
			prev_ = ( prev_->*decr_ )( );
		}

		return *this;
	}

	/// @brief 後退する
	/// @attention 未実装のため使用してはならない
	/// @param ダミー
	/// @return 後退前のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::const_reverse_iterator::operator--( int )
	{
		const_reverse_iterator temp = *this; // 現在のノード

		operator--( );

		return temp;
	}
#endif //_BIDIRECTIONAL_ITERATOR

	/// @brief イテレータが引数と等しいか判定する
	/// @attention 走査方法が同一であるかは考慮しない
	/// @param[in] iter 判定するイテレータ
	/// @retval true 引数と同じノードを指している
	/// @retval false 引数と異なるノードを指している
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::const_reverse_iterator::operator==( const const_reverse_iterator &iter )const
	{
		return node_ == iter.node_; // && incr_ == iter.incr_;
	}

	/// @brief イテレータが引数と等しくないか判定する
	/// @attention 走査方法が同一であるかは考慮しない
	/// @param[in] iter 判定するイテレータ
	/// @retval true 引数と異なるノードを指している
	/// @retval false 引数と同じノードを指している
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::const_reverse_iterator::operator!=( const const_reverse_iterator &iter )const
	{
		return !operator==( iter );
	}

	/// @brief 参照剥がしを行う
	/// @attention 無効なノードを指している場合， invalid_iterator 例外を投げる
	/// @return イテレータの指すデータの定数参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reference tree< ValueType, Allocator >::const_reverse_iterator::operator*( )
	{
		if ( node_ )
		{
			return node_->val;
		}
		else
		{
			throw invalid_iterator( );
		}
	}

	/// @brief 間接メンバ参照を行う
	/// @attention 無効なノードを指している場合， invalid_iterator 例外を投げる
	/// @return イテレータの指すオブジェクトの定数ポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_pointer tree< ValueType, Allocator >::const_reverse_iterator::operator->( )
	{
		if ( node_ )
		{
			return &node_->val;
		}
		else
		{
			throw invalid_iterator( );
		}
	}

	/// @brief 初期化する
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::reverse_iterator::reverse_iterator( )
	{
	}

	/// @brief イテレータを複製する
	/// @param[in] iter 複製するイテレータ
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::reverse_iterator::reverse_iterator( const reverse_iterator &iter ) : const_reverse_iterator( iter )
	{
	}

	/// @brief 引数で初期化する
	/// @param[in] node イテレータの指すノードのポインタ 
	/// @param[in] root 走査する木構造の根のポインタ
	/// @param[in] incr 前進関数
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::reverse_iterator::reverse_iterator( Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const ) : const_reverse_iterator( node, root, incr )
	{
	}

	/// @brief イテレータが指すノードを取得する
	/// @return イテレータが指すノードのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::reverse_iterator::node( )const
	{
		return const_cast< Node * >( node_ );
	}

	/// @brief 前進する
	/// @return 前進後のイテレータの参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator &tree< ValueType, Allocator >::reverse_iterator::operator++( )
	{
		if ( node_ )
		{
			node_ = ( node_->*incr_ )( root_ );
		}

		return *this;
	}

	/// @brief 前進する
	/// @param ダミー
	/// @return 前進前のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::reverse_iterator::operator++( int )
	{
		reverse_iterator temp = *this; // 現在のノード

		operator++( );

		return temp;
	}

#ifdef _BIDIRECTIONAL_ITERATOR
	/// @brief 後退する
	/// @attention 未実装のため使用してはならない
	/// @return 後退後のイテレータの参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator &tree< ValueType, Allocator >::reverse_iterator::operator--( )
	{
		if ( prev_ )
		{
			node_ = prev_;
			prev_ = ( prev_->*decr_ )( );
		}

		return *this;
	}

	/// @brief 後退する
	/// @attention 未実装のため使用してはならない
	/// @param ダミー
	/// @return 後退前のイテレータ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::reverse_iterator::operator--( int )
	{
		reverse_iterator temp = *this; // 現在のノード

		operator--( );

		return temp;
	}
#endif //_BIDIRECTIONAL_ITERATOR

	/// @brief 参照剥がしを行う
	/// @attention 無効なノードを指している場合， invalid_iterator 例外を投げる
	/// @return イテレータの指すデータの参照
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reference tree< ValueType, Allocator >::reverse_iterator::operator*( )
	{
		if ( node_ )
		{
			return const_cast< ValueType & >( node_->val );
		}
		else
		{
			throw invalid_iterator( );
		}
	}

	/// @brief 間接メンバ参照を行う
	/// @attention 無効なノードを指している場合， invalid_iterator 例外を投げる
	/// @return イテレータの指すオブジェクトのポインタ
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::pointer tree< ValueType, Allocator >::reverse_iterator::operator->( )
	{
		if ( node_ )
		{
			return const_cast< ValueType * >( &node_->val );
		}
		else
		{
			throw invalid_iterator( );
		}
	}

	/// @brief 引数のノードを根とする木構造の浅い複製を作成する
	/// @param[in] root 複製する木構造の根
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Subtree::Subtree( const Node *root ) : tree( root )
	{
	}

	/// @brief 参照を破棄する
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Subtree::~Subtree( )
	{
		root_ = NULL;
	}
}

#endif //_TMATSU_TREE_H_