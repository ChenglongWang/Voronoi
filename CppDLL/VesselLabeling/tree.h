/**
* @file tree.h
* @brief �؍\���R���e�i�N���X�̃w�b�_�t�@�C��
* @version 3.0
* @author ���É���w ���Ȋw������ ���f�B�A�Ȋw��U �X������   351203192   ���� �N�N
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
	/// @brief �����؂������R���e�i�N���X
	/// @tparam ValueType �؍\���̃m�[�h�̊i�[����f�[�^�^
	/// @tparam Allocator �A���P�[�^�^�D�K��l��STL�̃f�t�H���g�A���P�[�^
	/// @attention ���ݓ����ŃA���P�[�^�͎g�p����Ă��Ȃ�
	template < class ValueType, class Allocator = std::allocator< ValueType > >
	class tree
	{
		/// @brief �m�[�h
		struct Node
		{
			ValueType val; ///< @brief �f�[�^
			Node *parent; ///< @brief �e
			Node *eldest_child; ///< @brief ���q
			Node *youngest_child; ///< @brief ���q
			Node *elder_sibling; ///< @brief �Z
			Node *younger_sibling; ///< @brief ��
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

		/// @brief ������
		class Subtree : public tree 
		{
		public:
			Subtree( const Node *root );
			virtual ~Subtree( );
		};

	public:
		/// @brief ��̖؍\���Ɋ܂܂��l���Q�Ƃ��悤�Ƃ���Ɠ��������O
		class invalid_tree : public std::exception
		{
		};

		typedef ValueType value_type; ///< @brief �l
		typedef Allocator allocator_type; ///< @brief �A���P�[�^
		typedef size_t size_type; ///< @brief ������������
		typedef ptrdiff_t difference_type; ///< @brief �����t������
		typedef typename Allocator::reference reference; ///< @brief �Q��
		typedef typename Allocator::const_reference const_reference; ///< @brief �萔�Q��
		typedef typename Allocator::pointer pointer; ///< @brief �|�C���^
		typedef typename Allocator::const_pointer const_pointer; ///< @brief �萔�|�C���^
	
		/// @brief �؍\�����������ɑ�������O�i�C�e���[�^
		class const_iterator
		{
		public:
			typedef std::forward_iterator_tag iterator_category; ///< @brief �C�e���[�^�J�e�S��
			typedef ValueType value_type; ///< @brief �l
			typedef size_t size_type; ///< @brief ������������
			typedef ptrdiff_t difference_type; ///< @brief �����t������
			typedef typename Allocator::reference reference; ///< @brief �Q��
			typedef typename Allocator::const_reference const_reference; ///< @brief �萔�Q��
			typedef typename Allocator::pointer pointer; ///< @brief �|�C���^
			typedef typename Allocator::const_pointer const_pointer; ///< @brief �萔�|�C���^

			/// @brief �L���ȔԒn���w���Ă��Ȃ��C�e���[�^�̒l���Q�Ƃ��悤�Ƃ���Ɠ��������O
			class invalid_iterator : public std::exception
			{
			};
		protected:
			const Node *node_; ///< @brief �m�[�h
			const Node *root_; ///< @brief ��
			const Node *( Node::*incr_ )( const Node * )const; ///< @brief �O�i�֐�
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

		/// @brief �؍\�����������ɑ�������O�i�C�e���[�^
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

		/// @brief �؍\�����t�����ɑ�������O�i�C�e���[�^
		class const_reverse_iterator
		{
		public:
			typedef std::forward_iterator_tag iterator_category; ///< @brief �C�e���[�^�J�e�S��
			typedef ValueType value_type; ///< @brief �l
			typedef size_t size_type; ///< @brief ������������
			typedef ptrdiff_t difference_type; ///< @brief �����t������
			typedef typename Allocator::reference reference; ///< @brief �Q��
			typedef typename Allocator::const_reference const_reference; ///< @brief �萔�Q��
			typedef typename Allocator::pointer pointer; ///< @brief �|�C���^
			typedef typename Allocator::const_pointer const_pointer; ///< @brief �萔�|�C���^

			/// @brief �L���ȔԒn���w���Ă��Ȃ��C�e���[�^�̒l���Q�Ƃ��悤�Ƃ���Ɠ��������O
			class invalid_iterator : public std::exception
			{
			};
		protected:
			const Node *node_; ///< @brief �m�[�h
			const Node *root_; ///< @brief ��
			const Node *( Node::*incr_ )( const Node * )const; ///< @brief �O�i�֐�
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

		/// @brief �؍\�����t�����ɑ�������O�i�C�e���[�^
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
		Node *root_; ///< @brief ��
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

	/// @brief ����������
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( ) : root_( NULL )
	{
	}
	
	/// @brief �؍\���𕡐�����
	/// @param[in] tree ��������؍\��
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( const tree &tree ) : root_( NULL )
	{
		operator=( tree );
	}

	/// @brief ����������
	/// @param[in] alloc �A���P�[�^
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( const Allocator &alloc ) : root_( NULL )
	{
	}

	/// @brief �����̐������f�[�^���m�ۂ��C������т��̎q�Ƃ���
	/// @param[in] cnt �f�[�^�̐�
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( size_t cnt ) : root_( NULL )
	{
		insert( begin( ), cnt, ValueType( ) );
	}

	/// @brief �����̐����������̃f�[�^���m�ۂ��C������т��̎q�Ƃ���
	/// @param[in] cnt �f�[�^�̐�
	/// @param[in] val �f�[�^�̏����l
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( size_t cnt, const ValueType &val ) : root_( NULL )
	{
		insert( begin( ), cnt, val );
	}

	/// @brief �����̐������f�[�^���m�ۂ��C������т��̎q�Ƃ���
	/// @param[in] cnt �f�[�^�̐�
	/// @param[in] val �f�[�^�̏����l
	/// @param[in] alloc �A���P�[�^
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( size_t cnt, const ValueType &val, const Allocator &alloc ) : root_( NULL )
	{
		insert( begin( ), cnt, val );
	}


	/// @brief �����̃f�[�^�𕡐����C������т��̎q�Ƃ���
	/// @attention �������؍\���ł���ꍇ�C�e�q�֌W�͕ۑ�����Ȃ�
	/// @param[in] first ��������f�[�^�̐擪���w���C�e���[�^
	/// @param[in] last ��������f�[�^�̖����̎����w���C�e���[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline tree< ValueType, Allocator >::tree( Iterator first, Iterator last ) : root_( NULL )
	{
		insert( begin( ), first, last );
	}

	/// @brief �����̃f�[�^�𕡐����C������т��̎q�Ƃ���
	/// @attention �������؍\���ł���ꍇ�C�e�q�֌W�͕ۑ�����Ȃ�
	/// @param[in] first ��������f�[�^�̐擪���w���C�e���[�^
	/// @param[in] last ��������f�[�^�̖����̎����w���C�e���[�^
	/// @param[in] alloc �A���P�[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline tree< ValueType, Allocator >::tree( Iterator first, Iterator last, const Allocator &alloc ) : root_( NULL )
	{
		insert( begin( ), first, last );
	}
	
	/// @brief �����̃m�[�h�����Ƃ���؍\���̐[���������쐬����
	/// @param[in] root...��������؍\���̍�
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::tree( const Node *root ) : root_( const_cast< Node * >( root ) )
	{
	}


	/// @brief �̈���J������
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::~tree( )
	{
		delete root_;
		root_ = NULL;
	}

	/// @brief �擪���w���C�e���[�^���擾����
	/// @return �擪���w���C�e���[�^
	///
	/// �g�p��
	/// @code
	/// tmatsu::tree< int > t; // �؍\��
	///
	/// // �؍\���̊e�m�[�h�̒l���󎚂���
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

	/// @brief �擪���w���萔�C�e���[�^���擾����
	/// @return �擪���w���萔�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::begin( )const	
	{
		return preorder_begin( );
	}

	/// @brief �������w���t�C�e���[�^���擾����
	/// @return �������w���t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::rbegin( )
	{
		return preorder_rbegin( );
	}

	/// @brief �������w���萔�t�C�e���[�^���擾����
	/// @return �������w���萔�t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::rbegin( )const	
	{
		return preorder_rbegin( );
	}

	/// @brief �����̎����w���C�e���[�^���擾����
	/// @return �����̎����w���C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::end( )
	{
		return preorder_end( );
	}

	/// @brief �����̎����w���萔�C�e���[�^���擾����
	/// @return �����̎����w���萔�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::end( )const
	{
		return preorder_end( );
	}

	/// @brief �擪�̑O���w���t�C�e���[�^���擾����
	/// @return �擪�̑O���w���t�C�e���[�^	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::rend( )
	{
		return preorder_rend( );
	}

	/// @brief �擪�̑O���w���萔�t�C�e���[�^���擾����
	/// @return �擪�̑O���w���萔�t�C�e���[�^	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::rend( )const
	{
		return preorder_rend( );
	}

	/// @brief �擪���w����s��(�[���D��)�̃C�e���[�^���擾����
	/// @return �擪���w����s��(�[���D��)�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::preorder_begin( )
	{
		return iterator( root_ ? root_->preorder_hd( ) : NULL, root_, &Node::preorder_incr );
	}

	/// @brief �擪���w����s��(�[���D��)�̒萔�C�e���[�^���擾����
	/// @return �擪���w����s��(�[���D��)�̒萔�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::preorder_begin( )const
	{
		return const_iterator( root_ ? root_->preorder_hd( ) : NULL, root_, &Node::preorder_incr );
	}

	/// @brief �������w����s��(�[���D��)�̋t�C�e���[�^���擾����
	/// @return �������w����s��(�[���D��)�̋t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::preorder_rbegin( )
	{
		return reverse_iterator( root_ ? root_->preorder_tl( ) : NULL, root_, &Node::preorder_decr );
	}

	/// @brief �������w����s��(�[���D��)�̒萔�t�C�e���[�^���擾����
	/// @return �������w����s��(�[���D��)�̒萔�t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::preorder_rbegin( )const
	{
		return const_reverse_iterator( root_ ? root_->preorder_tl( ) : NULL, root_, &Node::preorder_decr );
	}

	/// @brief �����̎����w����s��(�[���D��)�̃C�e���[�^���擾����
	/// @return �����̎����w����s��(�[���D��)�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::preorder_end( )
	{
		return iterator( NULL, root_, &Node::preorder_incr );
	}

	/// @brief �����̎����w����s��(�[���D��)�̒萔�C�e���[�^���擾����
	/// @return �����̎����w����s��(�[���D��)�̒萔�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::preorder_end( )const
	{
		return const_iterator( NULL, root_, &Node::preorder_incr );
	}

	/// @brief �擪�̑O���w����s��(�[���D��)�̋t�C�e���[�^���擾����
	/// @return �擪�̑O���w����s��(�[���D��)�̋t�C�e���[�^	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::preorder_rend( )
	{
		return reverse_iterator( NULL, root_, &Node::preorder_decr );
	}

	/// @brief �擪�̑O���w����s��(�[���D��)�̒萔�t�C�e���[�^���擾����
	/// @return �擪�̑O���w����s��(�[���D��)�̒萔�t�C�e���[�^	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::preorder_rend( )const
	{
		return const_reverse_iterator( NULL, root_, &Node::preorder_decr );
	}

#if _INORDER_ITERATOR
	/// @brief �擪���w�����ԏ��̃C�e���[�^���擾����
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
 	/// @return �擪���w�����ԏ��̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::inorder_begin( )
	{
		return iterator( root_ ? root_->inorder_hd( ) : NULL, NULL, &Node::inorder_incr, &Node::inorder_decr );
	}

	/// @brief �擪���w�����ԏ��̒萔�C�e���[�^���擾����
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
 	/// @return �擪���w�����ԏ��̒萔�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::inorder_begin( )const
	{
		return const_iterator( root_ ? root_->inorder_hd( ) : NULL, NULL, &Node::inorder_incr, &Node::inorder_decr );
	}

	/// @brief �������w�����ԏ��̋t�C�e���[�^���擾����
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
 	/// @return �������w�����ԏ��̋t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::inorder_rbegin( )
	{
		return reverse_iterator( root_ ? root_->inorder_tl( ) : NULL, NULL, &Node::inorder_decr, &Node::inorder_incr );
	}

	/// @brief �������w�����ԏ��̒萔�t�C�e���[�^���擾����
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
 	/// @return �������w�����ԏ��̒萔�t�C�e���[�^	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::inorder_rbegin( )const
	{
		return const_reverse_iterator( root_ ? root_->inorder_tl( ) : NULL, NULL, &Node::inorder_decr, &Node::inorder_incr );
	}

	/// @brief �����̎����w�����ԏ��̃C�e���[�^���擾����
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
 	/// @return �����̎����w�����ԏ��̃C�e���[�^	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::inorder_end( )
	{
		return iterator( NULL, root_ ? root_->inorder_tl( ) : NULL, &Node::inorder_incr, &Node::inorder_decr );
	}

	/// @brief �����̎����w�����ԏ��̒萔�C�e���[�^���擾����
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
 	/// @return �����̎����w�����ԏ��̒萔�C�e���[�^	
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::inorder_end( )const
	{
		return const_iterator( NULL, root_ ? root_->inorder_tl( ) : NULL, &Node::inorder_incr, &Node::inorder_decr );
	}

	/// @brief �擪�̑O���w�����ԏ��̋t�C�e���[�^���擾����
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
 	/// @return �擪�̑O���w�����ԏ��̋t�C�e���[�^		
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::inorder_rend( )
	{
		return reverse_iterator( NULL, root_ ? root_->inorder_hd( ) : NULL, &Node::inorder_decr, &Node::inorder_incr );
	}
	
	/// @brief �擪�̑O���w�����ԏ��̒萔�t�C�e���[�^���擾����
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
 	/// @return �擪�̑O���w�����ԏ��̒萔�t�C�e���[�^		
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::inorder_rend( )const
	{
		return const_reverse_iterator( NULL, root_ ? root_->inorder_hd( ) : NULL, &Node::inorder_decr, &Node::inorder_incr );
	}
#endif //_INORDER_ITERATOR

	/// @brief �擪���w����s���̃C�e���[�^���擾����
	/// @return �擪���w����s���̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::postorder_begin( )
	{
		return iterator( root_ ? root_->postorder_hd( ) : NULL, root_, &Node::postorder_incr );
	}

	/// @brief �擪���w����s���̒萔�C�e���[�^���擾����
	/// @return �擪���w����s���̒萔�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::postorder_begin( )const
	{
		return const_iterator( root_ ? root_->postorder_hd( ) : NULL, root_, &Node::postorder_incr );
	}

	/// @brief �������w����s���̋t�C�e���[�^���擾����
	/// @return �������w����s���̋t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::postorder_rbegin( )
	{
		return reverse_iterator( root_ ? root_->postorder_tl( ) : NULL, root_, &Node::postorder_decr );
	}

	/// @brief �������w����s���̒萔�t�C�e���[�^���擾����
	/// @return �������w����s���̒萔�t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::postorder_rbegin( )const
	{
		return const_reverse_iterator( root_ ? root_->postorder_tl( ) : NULL, root_, &Node::postorder_decr );
	}

	/// @brief �����̎����w����s���̃C�e���[�^���擾����
	/// @return �����̎����w����s���̃C�e���[�^
	template < class ValueType, class Allocator >	
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::postorder_end( )
	{
		return iterator( NULL, root_, &Node::postorder_incr );
	}
	
	/// @brief �����̎����w����s���̒萔�C�e���[�^���擾����
	/// @return �����̎����w����s���̒萔�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::postorder_end( )const
	{
		return const_iterator( NULL, root_, &Node::postorder_incr );
	}
	
	/// @brief �擪�̑O���w����s���̋t�C�e���[�^���擾����
	/// @return �擪�̑O���w����s���̋t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::postorder_rend( )
	{
		return reverse_iterator( NULL, root_, &Node::postorder_decr );
	}

	/// @brief �擪�̑O���w����s���̒萔�t�C�e���[�^���擾����
	/// @return �擪�̑O���w����s���̒萔�t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::postorder_rend( )const
	{
		return const_reverse_iterator( NULL, root_, &Node::postorder_decr );
	}

	/// @brief �擪���w�����x����(���D��)�̃C�e���[�^���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return	�擪���w�����x����(���D��)�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::levelorder_begin( )
	{
		return iterator( root_ ? root_->levelorder_hd( ) : NULL, root_, &Node::levelorder_incr );
	}

	/// @brief �擪���w�����x����(���D��)�̒萔�C�e���[�^���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return	�擪���w�����x����(���D��)�̒萔�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::levelorder_begin( )const
	{
		return const_iterator( root_ ? root_->levelorder_hd( ) : NULL, root_, &Node::levelorder_incr );
	}

	/// @brief �������w�����x����(���D��)�̋t�C�e���[�^���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return	�������w�����x����(���D��)�̋t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::levelorder_rbegin( )
	{
		return reverse_iterator( root_ ? root_->levelorder_tl( ) : NULL, root_, &Node::levelorder_decr );
	}

	/// @brief �������w�����x����(���D��)�̒萔�t�C�e���[�^���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return	�������w�����x����(���D��)�̒萔�t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::levelorder_rbegin( )const
	{
		return const_reverse_iterator( root_ ? root_->levelorder_tl( ) : NULL, root_, &Node::levelorder_decr );
	}

	/// @brief �����̎����w�����x����(���D��)�̃C�e���[�^���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return	�����̎����w�����x����(���D��)�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::levelorder_end( )
	{
		return iterator( NULL, root_, &Node::levelorder_incr );
	}
	
	/// @brief �����̎����w�����x����(���D��)�̒萔�C�e���[�^���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return	�����̎����w�����x����(���D��)�̒萔�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::levelorder_end( )const
	{
		return const_iterator( NULL, root_, &Node::levelorder_incr );
	}
	
	/// @brief �擪�̑O���w�����x����(���D��)�̋t�C�e���[�^���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return	�擪�̑O���w�����x����(���D��)�̋t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::levelorder_rend( )
	{
		return reverse_iterator( NULL, root_, &Node::levelorder_decr );
	}

	/// @brief �擪�̑O���w�����x����(���D��)�̒萔�t�C�e���[�^���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return	�擪�̑O���w�����x����(���D��)�̒萔�t�C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::levelorder_rend( )const
	{
		return const_reverse_iterator( NULL, root_, &Node::levelorder_decr );
	}

	/// @brief �擪���w���q���̃C�e���[�^���擾����
	/// @param[in] parent �擾����C�e���[�^�̐e���w���C�e���[�^
	/// @return	�擪���w���q���̃C�e���[�^
	///
	/// �g�p��
	/// @code
	/// tmatsu::tree< int > t; // �؍\��
	/// tmatsu::tree< int >::iterator node = std::find( t.begin( ), t.end( ), 100 ); // �l��100�̃m�[�h���w���C�e���[�^
	/// 
	/// // �q���̊e�m�[�h�̒l���󎚂���
	/// for ( tmatsu::tree< int >::iterator iter = t.child_begin( node ); iter != t.child_end( node ); iter++ )
	/// {
	///		std::cout << *iter << std::endl;
	/// }
	///
	/// // �Z��̊e�m�[�h�̒l���󎚂���
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

	/// @brief �擪���w���q���̒萔�C�e���[�^���擾����
	/// @param[in] parent �擾����C�e���[�^�̐e���w���C�e���[�^
	/// @return	�擪���w���q���̒萔�C�e���[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::child_begin( Iterator parent )const
	{
		return const_iterator( parent.node( ) ? parent.node( )->child_hd( ) : NULL, root_, &Node::child_incr );
	}
	
	/// @brief �������w���q���̋t�C�e���[�^���擾����
	/// @param[in] parent �擾����C�e���[�^�̐e���w���C�e���[�^
	/// @return	�������w���q���̋t�C�e���[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::child_rbegin( Iterator parent )
	{
		return reverse_iterator( parent.node( ) ? const_cast< Node * >( parent.node( )->child_tl( ) ) : NULL, root_, &Node::child_decr );
	}

	/// @brief �������w���q���̒萔�t�C�e���[�^���擾����
	/// @param[in] parent �擾����C�e���[�^�̐e���w���C�e���[�^
	/// @return	�������w���q���̒萔�t�C�e���[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::child_rbegin( Iterator parent )const
	{
		return const_reverse_iterator( parent.node( ) ? parent.node( )->child_tl( ) : NULL, root_, &Node::child_decr );
	}

	/// @brief �����̎����w���q���̃C�e���[�^���擾����
	/// @param[in] parent �擾����C�e���[�^�̐e���w���C�e���[�^
	/// @return	�����̎����w���q���̃C�e���[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::child_end( Iterator parent )
	{
		return iterator( NULL, root_, &Node::child_incr );
	}

	/// @brief �����̎����w���q���̒萔�C�e���[�^���擾����
	/// @param[in] parent �擾����C�e���[�^�̐e���w���C�e���[�^
	/// @return	�����̎����w���q���̒萔�C�e���[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::child_end( Iterator parent )const
	{
		return const_iterator( NULL, root_, &Node::child_incr );
	}

	/// @brief �擪�̑O���w���q���̋t�C�e���[�^���擾����
	/// @param[in] parent �擾����C�e���[�^�̐e���w���C�e���[�^
	/// @return	�擪�̑O���w���q���̋t�C�e���[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::child_rend( Iterator parent )
	{
		return reverse_iterator( NULL, root_, &Node::child_decr );
	}

	/// @brief �擪�̑O���w���q���̒萔�t�C�e���[�^���擾����
	/// @param[in] parent �擾����C�e���[�^�̐e���w���C�e���[�^
	/// @return	�擪�̑O���w���q���̒萔�t�C�e���[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::child_rend( Iterator parent )const
	{
		return const_reverse_iterator( NULL, root_, &Node::child_decr );
	}

	/// @brief �����̃f�[�^�������C������т��̎q�Ƃ���
	/// @attention �������؍\���ł������ꍇ�C�e�q�֌W�͕ۑ�����Ȃ�
	/// @param[in] first ��������f�[�^�̐擪���w���C�e���[�^
	/// @param[in] last ��������f�[�^�̖����̎����w���C�e���[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline void tree< ValueType, Allocator >::assign( Iterator first, Iterator last )
	{
		clear( );
		insert( begin( ), first, last );
	}

	/// @brief �����̐������f�[�^�������C������т��̎q�Ƃ���
	/// @param[in] cnt�@�������f�[�^�̐�
	/// @param[in] val �������f�[�^
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::assign( size_t cnt, const ValueType &val )
	{
		clear( );
		insert( begin( ), cnt, val );
	}

	/// @brief �؍\�������ւ���
	/// @param[inout] tree ����ւ���؍\��
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::swap( tree &tree )
	{
		std::swap( root_, tree.root_ );
	}

	/// @brief �A���P�[�^���擾����
	/// @attention ���ݓ����ŃA���P�[�^���g�p����Ă��Ȃ����ߖ��Ӗ��Ȓl���Ԃ�
	/// @return �A���P�[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::allocator_type tree< ValueType, Allocator >::get_allocator( )const
	{
		return Allocator( );
	}
	
	/// @brief �f�[�^�����擾����
	/// @return �f�[�^��
	template < class ValueType, class Allocator >
	inline size_t tree< ValueType, Allocator >::size( )const
	{
		size_t cnt = 0; //�f�[�^��
	
		for ( const_iterator iter = begin( ); iter != end( ); iter++ )
		{
			cnt++;
		}

		return cnt;
	}

	/// @brief �ő�f�[�^�����擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return �ő�f�[�^��
	template < class ValueType, class Allocator >
	inline size_t tree< ValueType, Allocator >::max_size( )const
	{
		return SIZE_MAX;
	}
	
	/// @brief �؍\�����󂩔��肷��
	/// @retval true �؍\���͋�ł���
	/// @retval fasle �؍\���͋�łȂ�
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::empty( )const
	{
		return !root_;
	}

	/// @brief �f�[�^����ύX���C������т��̎q�Ƃ��ĐV�����m�ۂ����m�[�h���f�t�H���g�R���X�g���N�^�ŏ���������
	/// @deprecated ���̒l��e�q�֌W�͕ۑ�����Ȃ����ߎg�p�͐�������Ȃ�
	/// @param[in] cnt �ύX��̃f�[�^��
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::resize( size_t cnt )
	{
		clear( );
		insert( begin( ), cnt, ValueType( ) );
	}

	/// @brief �f�[�^����ύX���C������т��̎q�Ƃ��ĐV�����m�ۂ����m�[�h�������ŏ���������
	/// @deprecated �e�q�֌W�͕ۑ�����Ȃ����ߎg�p�͐�������Ȃ�
	/// @param[in] cnt �ύX��̃f�[�^��
	/// @param[in] val �ύX��̃f�[�^
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::resize( size_t cnt, const ValueType &val )
	{
		clear( );
		insert( begin( ), cnt, val );
	}

	/// @brief �����擾����
	/// @attention �؍\������ł���ꍇ�C invalid_tree ��O�𓊂���
	/// @return ���̎Q��
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

	/// @brief �����擾����
	/// @attention �؍\������ł���ꍇ�C invalid_tree ��O�𓊂���
	/// @return ���̒萔�Q��
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

	/// @brief �����̃f�[�^��}������
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] val �}������f�[�^
	/// @return �}�������f�[�^���w���C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::insert( const_iterator parent, const ValueType &val )
	{
		if ( root_ )
		{
			Node *parent_node = parent.node( ) ? const_cast< Node * >( parent.node( ) ) : root_; // �}������m�[�h�̐e
			Node *child_node = new Node( val ); // �}������m�[�h

			// �m�[�h��A������
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
		
			// �����̈ʒu�ɑ}������
			return iterator( child_node, root_, parent.incr( ) );
		}
		else
		{
			// �؍\������̂Ƃ����Ƃ��đ}������
			return iterator( root_ = new Node( val ), root_, parent.incr( ) );
		}
	}

	/// @brief �����̃f�[�^��}������
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] val �}������f�[�^
	/// @return �}�������f�[�^���w���C�e���[�^
	template < class ValueType, class Allocator >
	template < class Type >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::insert( const_iterator parent, Type val )
	{
		return insert( parent, static_cast< ValueType >( val ) );
	}

	/// @brief �����̐������f�[�^��}������
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] cnt �}������f�[�^�̐�
	/// @param[in] val �}������f�[�^
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, size_t cnt, const ValueType &val )
	{
		for ( size_t i = 0; i < cnt; i++ )
		{
			insert( parent, val );
		}
	}

	/// @brief �����̃f�[�^��}������
	/// @attention �������؍\���ł���ꍇ�C�e�q�֌W�͕ۑ�����Ȃ�
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] first �}������f�[�^�̐擪���w���C�e���[�^
	/// @param[in] last �}������f�[�^�̖����̎����w���C�e���[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, Iterator first, Iterator last )
	{
		for ( ; first != last; first++ )
		{
			insert( parent, *first );
		}
	}

	/// @brief �����̖؍\����}������
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] tree �}������؍\��
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, const tree &tree )
	{
	}

	/// @brief �����̐������؍\����}������
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] cnt �}������؍\���̐�
	/// @param[in] tree �}������؍\��
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, size_t cnt, const tree &tree )
	{
		for ( size_t i = 0; i < cnt; i++ )
		{
			insert( parent, tree );
		}
	}
	
	/// @brief �����̃f�[�^��}������
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] younger_sibling �}������ʒu�̒���w���C�e���[�^
	/// @param[in] val �}������f�[�^
	/// @return �}�������f�[�^���w���C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, const ValueType &val )
	{
	}

	/// @brief �����̃f�[�^��}������
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] younger_sibling �}������ʒu�̒���w���C�e���[�^
	/// @param[in] val �}������f�[�^
	/// @return �}�������f�[�^���w���C�e���[�^
	template < class ValueType, class Allocator >
	template < class Type >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, Type val )
	{
		return insert( parent, younger_sibling, static_cast< ValueType >( val ) );
	}

	/// @brief �����̐������f�[�^��}������
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] younger_sibling �}������ʒu�̒���w���C�e���[�^
	/// @param[in] cnt �}������f�[�^�̐�
	/// @param[in] val �}������f�[�^
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, size_t cnt, const ValueType &val )
	{
		for ( size_t i = 0; i < cnt; i++ )
		{
			insert( parent, younger_sibling, val );
		}
	}

	/// @brief �����̃f�[�^��}������
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] younger_sibling �}������ʒu�̒���w���C�e���[�^
	/// @param[in] first �}������f�[�^�̐擪���w���C�e���[�^
	/// @param[in] last �}������f�[�^�̖����̎����w���C�e���[�^
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, Iterator first, Iterator last )
	{
		for ( ; first != last; first++ )
		{
			insert( parent, younger_sibling, *first );
		}
	}

	/// @brief �����̖؍\����}������
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] younger_sibling �}������ʒu�̒���w���C�e���[�^
	/// @param[in] tree �}������؍\��
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, const tree &tree )
	{
	}

	/// @brief �����̐������؍\����}������
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param[in] parent �}������ʒu�̐e���w���C�e���[�^
	/// @param[in] younger_sibling �}������ʒu�̒���w���C�e���[�^
	/// @param[in] cnt �}������؍\���̐�
	/// @param[in] tree �}������؍\��
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::insert( const_iterator parent, const_iterator younger_sibling, size_t cnt, const tree &tree )
	{
		for ( size_t i = 0; i < cnt; i++ )
		{
			insert( parent, younger_sibling, tree );
		}
	}

	/// @brief �����̃f�[�^���폜����
	/// @param[in] pos �폜����ʒu���w���C�e���[�^
	/// @return �폜�����f�[�^�̎����w���C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::erase( const_iterator pos )
	{
		Node *current = const_cast< Node * >( pos.node( ) ); // ���݂̃m�[�h

		if ( current )
		{
			Node *parent = pos.node( )->parent; // �폜����m�[�h�̐e
			iterator next = static_cast< iterator & >( pos ); // ���̃m�[�h���w���C�e���[�^

			// �O�i����
			next++;

			// �ĘA������
			if ( parent )
			{
				// �Z�ƒ��q�܂��͒��A������
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
			
				// ��Ɩ��q���͌Z��A������
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

				// �e�Ǝq����A������
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
			
				// �C�e���[�^�̍����폜����Ƃ������X�V����
				if ( current == next.root( ) )
				{
					next = iterator( next.node( ), current->parent, next.incr( ) );
				}
			}
			else
			{
				// �����폜����Ƃ����q�����ɕύX����
				root_ = current->eldest_child;

				if ( root_ )
				{
					// ���̎q���ƒ��A������
					if ( root_->youngest_child )
					{
						root_->youngest_child->younger_sibling = root_->younger_sibling;
					}

					// ���̒���q���ɕύX����
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

					// ���̘A����ؒf����
					root_->parent = root_->younger_sibling = NULL;
				}
			
				// �C�e���[�^�̍����폜����Ƃ������X�V����
				if ( current == next.root( ) )
				{
					next = iterator( next.node( ), root_, next.incr( ) );
				}
			}

			// �폜����
			current->eldest_child = current->younger_sibling = NULL;
			delete current;

			return next;
		}
		else
		{
			// �C�e���[�^���I�[���w���Ă���Ƃ����삵�Ȃ�
			return iterator( static_cast< iterator & >( pos ) );
		}
	}

	/// @brief �����̃f�[�^���폜����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param[in] first �폜����f�[�^�̐擪���w���C�e���[�^
	/// @param[in] last �폜����f�[�^�̖����̎����w���C�e���[�^
	/// @return �폜�����f�[�^�̎����w���C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::erase( const_iterator first, const_iterator last )
	{
	}

	/// @brief �S�Ẵf�[�^���폜����
	template < class ValueType, class Allocator >
	inline void tree< ValueType, Allocator >::clear( )
	{
		delete root_;
		root_ = NULL;
	}

	/// @brief �����̐e���w���C�e���[�^���擾����
	/// @attention ���������ł���ꍇ�C end( ) ���Ԃ�
	/// @param[in] child �擾����C�e���[�^�̎q���w���C�e���[�^
	/// @return	�����̐e���w���C�e���[�^
	///
	/// �g�p��
	/// @code
	/// tmatsu::tree< int > t; // �؍\��
	/// tmatsu::tree< int >::const_iterator node = std::find( t.begin( ), t.end( ), 100 ); // �l��100�̃m�[�h���w���C�e���[�^
	/// 
	/// // ��c�̊e�m�[�h�̒l���󎚂���
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
		
	/// @brief �C�e���[�^�̎w���m�[�h�̐[�����擾����
	/// @param[in] iter �[�����擾����m�[�h���w���C�e���[�^
	/// @return �C�e���[�^�̎w���m�[�h�̐[��
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline size_t tree< ValueType, Allocator >::depth( Iterator iter )const
	{
		size_t dpt = 0; // �[��

		for ( const Node *p = iter.node( ); p && p != root_; p = p->parent )
		{
			dpt++;
		}

		return dpt;
	}
	
	/// @brief �C�e���[�^�̎w���m�[�h���������肷��
	/// @param[in] iter ���肷��m�[�h���w���C�e���[�^
	/// @retval true �C�e���[�^�̎w���m�[�h�͍��ł���
	/// @retval false �C�e���[�^�̎w���m�[�h�͍��łȂ�
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline bool tree< ValueType, Allocator >::is_root( Iterator iter )const
	{
		return iter.node( ) == iter.root( );
	}

	/// @brief �C�e���[�^�̎w���m�[�h���t������
	/// @param[in] iter ���肷��m�[�h���w���C�e���[�^
	/// @retval true �C�e���[�^�̎w���m�[�h�͗t�ł���
	/// @retval false �C�e���[�^�̎w���m�[�h�͗t�łȂ�
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline bool tree< ValueType, Allocator >::is_leaf( Iterator iter )const
	{
		return !iter.node( )->eldest_child;
	}
	
	/// @brief �C�e���[�^�̎w���m�[�h���Z�����������
	/// @param[in] iter ���肷��m�[�h���w���C�e���[�^
	/// @retval true �C�e���[�^�̎w���m�[�h�͌Z�������
	/// @retval false �C�e���[�^�̎w���m�[�h�͌Z�������
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline bool tree< ValueType, Allocator >::has_sibling( Iterator iter )const
	{
		return ( iter.node( )->younger_sibling || iter.node( )->elder_sibling ) && !is_root( iter );
	}
	
	/// @brief iter��ref_iter�̐�c������
	/// @param[in] iter ���肷��m�[�h���w���C�e���[�^
	/// @param[in] ref_iter ��ƂȂ�m�[�h���w���C�e���[�^
	/// @retval true iter��ref_iter�̐�c�ł���
	/// @retval false iter��ref_iter�̐�c�łȂ�
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
	
	/// @brief iter��ref_iter�̎q��������
	/// @param[in] iter ���肷��m�[�h���w���C�e���[�^
	/// @param[in] ref_iter ��ƂȂ�m�[�h���w���C�e���[�^
	/// @retval true iter��ref_iter�̎q���ł���
	/// @retval false iter��ref_iter�̎q���łȂ�
	template < class ValueType, class Allocator >
	template < class Iterator, class ReferenceIterator >
	inline bool tree< ValueType, Allocator >::is_descendant( Iterator iter, ReferenceIterator ref_iter )const
	{
		return is_ancestor( ref_iter, iter );
	}

	/// @brief ���������Ƃ��镔���؂��擾����
	/// @attention �擾���镔���؂͐󂢕����Ȃ̂ŁC���̂��~�����ꍇ�̓R�s�[�R���X�g���N�^�܂��͑�����Z�q�𗘗p���ĕ����؂𕡐�����
	/// @param[in] root �擾���镔���؂̍����w���C�e���[�^
	/// @return ���������Ƃ��镔����
	///
	/// �g�p��
	/// @code
	/// tmatsu::tree< int > t; // �؍\��
	/// tmatsu::tree< int > &s = t.subtree( std::find( t.begin( ), t.end( ), 100 ) ); // �l��100�̃m�[�h�����Ƃ��镔����
	/// 
	/// // �����؂̊e�m�[�h�̒l���󎚂���
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

	/// @brief �t�����̖؍\�����擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param[in] root �擾����؍\���̍��ƂȂ�m�[�h���w���C�e���[�^
	/// @param[out] tree �擾����؍\�����i�[����̈�̎Q��
	template < class ValueType, class Allocator >
	template < class Iterator >
	inline void tree< ValueType, Allocator >::reverse( Iterator root, tree &tree )
	{
	}

	/// @brief �؍\���𕡐�����
	/// @param[in] tree ��������؍\��
	/// @return �������g�̎Q��
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator > &tree< ValueType, Allocator >::operator=( const tree &tree )
	{
		if ( this != &tree )
		{
			clear( );

			if ( tree.root_ )
			{
				Node *temp = tree.root_->younger_sibling; // ��������؍\���̍��̒��ޔ�����

				tree.root_->younger_sibling = NULL;
				root_ = new Node( *tree.root_ );
				tree.root_->younger_sibling = temp;
			}
		}

		return *this;
	}

	/// @brief �����Ɠ������؍\�������肷��
	/// @param[in] tree ���肷��؍\��
	/// @retval true �S�Ẵm�[�h�������Ɠ�����
	/// @retval false �����ƈقȂ�m�[�h�����݂���
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator==( const tree &tree )const
	{
		for ( const_iterator left = begin( ), right = tree.begin( ); left != end( ) && right != tree.end( ); left++, right++ )
		{
			size_t left_child_cnt = 0, right_child_cnt = 0; // �q���̐�

			// �l�������������ׂ�
			if ( *left != *right )
			{
				return false;
			}

			// �q���̐��������������ׂ�
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

	/// @brief �����Ɠ������Ȃ��؍\�������肷��
	/// @param[in] tree ���肷��؍\��
	/// @retval true �����ƈقȂ�m�[�h�����݂���
	/// @retval false �S�Ẵm�[�h�������Ɠ�����
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator!=( const tree &tree )const
	{
		return !operator==( tree );
	}

	/// @brief ������莫�����ŏ������؍\�������肷��
	/// @param[in] tree ���肷��؍\��
	/// @retval true ������莫�����ŏ������؍\���ł���
	/// @retval false ������莫�����ő傫���܂��͓������؍\���ł���
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator<( const tree &tree )const
	{
		const_iterator left, right; // �C�e���[�^

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

	/// @brief ������莫�����ŏ������܂��͓������؍\�������肷��
	/// @param[in] tree ���肷��؍\��
	/// @retval true ������莫�����ŏ������܂��͓������؍\���ł���
	/// @retval false ������莫�����ő傫���؍\���ł���
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator<=( const tree &tree )const
	{
		return !operator>( tree );
	}

	/// @brief ������莫�����ő傫���؍\�������肷��
	/// @param[in] tree ���肷��؍\��
	/// @retval true ������莫�����ő傫���؍\���ł���
	/// @retval false ������莫�����ŏ������܂��͓������؍\���ł���
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator>( const tree &tree )const
	{
		return tree.operator<( *this );
	}

	/// @brief ������莫�����ő傫���܂��͓������؍\�������肷��
	/// @param[in] tree ���肷��؍\��
	/// @retval true ������莫�����ő傫���܂��͓������؍\���ł���
	/// @retval false ������莫�����ŏ������؍\���ł���
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::operator>=( const tree &tree )const
	{
		return !operator<( tree );
	}

	///@brief ����������
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Node::Node( ) : parent( NULL ), eldest_child( NULL ), youngest_child( NULL ), elder_sibling( NULL ), younger_sibling( NULL )
	{
	}
		
	/// @brief �m�[�h�Ƃ��̌Z�킨��юq���𕡐�����
	/// @param[in] node ��������m�[�h
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Node::Node( const Node &node ) : val( node.val ), parent( NULL ), eldest_child( NULL ), youngest_child( NULL ), elder_sibling( NULL ), younger_sibling( NULL )
	{
		// �q���𕡐�����
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
	
		// �Z��𕡐�����
		if ( node.younger_sibling )
		{
			younger_sibling = new Node( *node.younger_sibling );
			younger_sibling->elder_sibling = this;
		}
	}

	/// @brief �����ŏ���������
	/// @param[in] val �f�[�^�̏����l
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Node::Node( const ValueType &val ) : val( val ), parent( NULL ), eldest_child( NULL ), youngest_child( NULL ), elder_sibling( NULL ), younger_sibling( NULL )
	{
	}

	/// @brief �̈���������
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Node::~Node( )
	{
		delete eldest_child;
		delete younger_sibling;
		eldest_child = younger_sibling = NULL;
	}

	/// @brief ��s��(�[���D��)�̐擪�̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ��s��(�[���D��)�̐擪�̃m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_hd( )
	{
		return this;
	}

	/// @brief ��s��(�[���D��)�̐擪�̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ��s��(�[���D��)�̐擪�̃m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_hd( )const
	{
		return this;
	}

	/// @brief ��s��(�[���D��)�̖����̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ��s��(�[���D��)�̖����̃m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_tl( )
	{
		return youngest_child ? youngest_child->preorder_tl( ) : this;
	}

	/// @brief ��s��(�[���D��)�̖����̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ��s��(�[���D��)�̖����̃m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_tl( )const
	{
		return youngest_child ? youngest_child->preorder_tl( ) : this;
	}
		
	/// @brief ��s��(�[���D��)�őO�i����
	/// @attention ����ɑ�������m�[�h�����݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @param[in] root �����w���|�C���^
	/// @return ��s��(�[���D��)�Œ���ɑ�������m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_incr( const Node *root )const
	{
		if ( eldest_child )
		{
			// �q��������Ƃ����q��Ԃ�
			return eldest_child;
		}
		else
		{
			for ( const Node *p = this; p != root; p = p->parent )
			{
				if ( p->younger_sibling )
				{
					// �������g�܂��͐�c�ɒ킪����Ƃ����Ԃ�
					return p->younger_sibling;
				}
			}
		}

		// �Ԃ��m�[�h�����݂��Ȃ��Ƃ��k���|�C���^��Ԃ�
		return NULL;
	}	

	/// @brief ��s��(�[���D��)�Ō�ނ���
	/// @attention ���O�ɑ�������m�[�h�����݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @param[in] root �����w���|�C���^
	/// @return ��s��(�[���D��)�Œ��O�ɑ�������m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::preorder_decr( const Node *root )const
	{
		if ( this == root )
		{
			// �Ԃ��m�[�h�����݂��Ȃ��Ƃ��k���|�C���^��Ԃ�
			return NULL;
		}

		for ( const Node *p = elder_sibling; p; p = p->youngest_child )
		{
			if ( !p->youngest_child )
			{
				// �Z�܂��͂��̎q���̖��q��Ԃ�
				return p;
			}
		}
			
		// ���łȂ��Ƃ��e��Ԃ�
		return parent;
	}

#ifdef _INORDER_ITERATOR
	/// @brief ���ԏ��̐擪�̃m�[�h���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���ԏ��̐擪�̃m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_hd( )
	{
		return eldest_child ? eldest_child->inorder_hd( ) : this;
	}

	/// @brief ���ԏ��̐擪�̃m�[�h���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���ԏ��̐擪�̃m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_hd( )const
	{
		return eldest_child ? eldest_child->inorder_hd( ) : this;
	}

	/// @brief ���ԏ��̖����̃m�[�h���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���ԏ��̖����̃m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_tl( )
	{
		return youngest_child ? youngest_child->inorder_tl( ) : this;
	}

	/// @brief ���ԏ��̖����̃m�[�h���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���ԏ��̖����̃m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_tl( )const
	{
		return youngest_child ? youngest_child->inorder_tl( ) : this;
	}

	/// @brief ���ԏ��őO�i����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention ����ɑ�������m�[�h�����݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @param[in] root �����w���|�C���^
	/// @return ���ԏ��Œ���ɑ�������m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_incr( const Node *root )const
	{
	}

	/// @brief ���ԏ��Ō�ނ���
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention �؍\�����񕪖؂łȂ��ꍇ�̓���͕s��
	/// @attention ���O�ɑ�������m�[�h�����݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @param[in] root �����w���|�C���^
	/// @return ���ԏ��Œ��O�ɑ�������m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::inorder_decr( const Node *root )const
	{
	}
#endif //_INORDER_ITERATOR

	/// @brief ��s���̐擪�̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ��s���̐擪�̃m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_hd( )
	{
		return eldest_child ? eldest_child->postorder_hd( ) : this;
	}

	/// @brief ��s���̐擪�̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ��s���̐擪�̃m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_hd( )const
	{
		return eldest_child ? eldest_child->postorder_hd( ) : this;
	}

	/// @brief ��s���̖����̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ��s���̖����̃m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_tl( )
	{
		return this;
	}

	/// @brief ��s���̖����̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ��s���̖����̃m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_tl( )const
	{
		return this;
	}

	/// @brief ��s���őO�i����
	/// @attention ����ɑ�������m�[�h�����݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @param[in] root �����w���|�C���^
	/// @return ��s���Œ���ɑ�������m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_incr( const Node *root )const
	{
		if ( this == root )
		{
			// �Ԃ��m�[�h�����݂��Ȃ��Ƃ��k���|�C���^��Ԃ�
			return NULL;
		}

		for ( const Node *p = younger_sibling; p; p = p->eldest_child )
		{
			if ( !p->eldest_child )
			{
				// ��܂��͂��̎q���̒��q��Ԃ�
				return p;
			}
		}
			
		// ���łȂ��Ƃ��e��Ԃ�
		return parent;
	}

	/// @brief ��s���Ō�ނ���
	/// @attention ���O�ɑ�������m�[�h�����݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @param[in] root �����w���|�C���^
	/// @return ��s���Œ��O�ɑ�������m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::postorder_decr( const Node *root )const
	{
		if ( youngest_child )
		{
			// �q��������Ƃ����q��Ԃ�
			return youngest_child;
		}
		else
		{
			for ( const Node *p = this; p != root; p = p->parent )
			{
				if ( p->elder_sibling )
				{
					// �������g�܂��͐�c�ɌZ������Ƃ����Ԃ�
					return p->elder_sibling;
				}
			}
		}

		// �Ԃ��m�[�h�����݂��Ȃ��Ƃ��k���|�C���^��Ԃ�
		return NULL;
	}

	/// @brief ���x����(���D��)�̐擪�̃m�[�h���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���x����(���D��)�̐擪�̃m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_hd( )
	{
		return this;
	}

	/// @brief ���x����(���D��)�̐擪�̃m�[�h���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���x����(���D��)�̐擪�̃m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_hd( )const
	{
		return this;
	}

	/// @brief ���x����(���D��)�̖����̃m�[�h���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���x����(���D��)�̖����̃m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_tl( )
	{
	}

	/// @brief ���x����(���D��)�̖����̃m�[�h���擾����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���x����(���D��)�̖����̃m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_tl( )const
	{
	}

	/// @brief ���x����(���D��)�őO�i����
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention ����ɑ�������m�[�h�����݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @param[in] root �����w���|�C���^
	/// @return ���x����(���D��)�Œ���ɑ�������m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_incr( const Node *root )const
	{
	}

	/// @brief ���x����(���D��)�Ō�ނ���
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @attention ���O�ɑ�������m�[�h�����݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @param[in] root �����w���|�C���^
	/// @return ���x����(���D��)�Œ��O�ɑ�������m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::levelorder_decr( const Node *root )const
	{
	}

	/// @brief ���q�̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���q�̃m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_hd( )
	{
		return eldest_child;
	}

	/// @brief ���q�̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���q�̃m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_hd( )const
	{
		return eldest_child;
	}

	/// @brief ���q�̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���q�̃m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_tl( )
	{
		return youngest_child;
	}

	/// @brief ���q�̃m�[�h���擾����
	/// @attention ���݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @return ���q�̃m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_tl( )const
	{
		return youngest_child;
	}

	/// @brief �Z��ԂőO�i����
	/// @attention ����ɑ�������m�[�h�����݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @param[in] root �����w���|�C���^
	/// @return �Z��ԂŒ���ɑ�������m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_incr( const Node *root )const
	{
		return younger_sibling;
	}

	/// @brief �Z��ԂŌ�ނ���
	/// @attention ���O�ɑ�������m�[�h�����݂��Ȃ��ꍇ�CNULL���Ԃ�
	/// @param[in] root �����w���|�C���^
	/// @return �Z��ԂŒ��O�ɑ�������m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::Node::child_decr( const Node *root )const
	{
		return elder_sibling;
	}

	/// @brief ����������
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_iterator::const_iterator( ) : node_( NULL ), root_( NULL ), incr_( &Node::preorder_incr )
	{
	}

	/// @brief �C�e���[�^�𕡐�����
	/// @param[in] iter ��������C�e���[�^
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_iterator::const_iterator( const const_iterator &iter ) : node_( iter.node_ ), root_( iter.root_ ), incr_( iter.incr_ )
	{
	}

	/// @brief �����ŏ���������
	/// @param[in] node �C�e���[�^�̎w���m�[�h�̃|�C���^ 
	/// @param[in] root ��������؍\���̍��̃|�C���^
	/// @param[in] incr �O�i�֐�
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_iterator::const_iterator( const Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const ) : node_( node ), root_( root ), incr_( incr )
	{
	}

	/// @brief �C�e���[�^���w���m�[�h���擾����
	/// @return �C�e���[�^���w���m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::const_iterator::node( )const
	{
		return node_;
	}

	/// @brief ��������؍\���̍����擾����
	/// @return ��������؍\���̍��̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::const_iterator::root( )const
	{
		return root_;
	}
	
	/// @brief �O�i�֐����擾����
	/// @return �O�i�֐�
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *( tree< ValueType, Allocator >::Node::*tree< ValueType, Allocator >::const_iterator::incr( ) )( const Node * )const
	{
		return incr_;
	}

	/// @brief �O�i����
	/// @return �O�i��̃C�e���[�^�̎Q��
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator &tree< ValueType, Allocator >::const_iterator::operator++( )
	{
		if ( node_ )
		{
			node_ = ( node_->*incr_ )( root_ );
		}

		return *this;
	}

	/// @brief �O�i����
	/// @param �_�~�[
	/// @return �O�i�O�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::const_iterator::operator++( int )
	{
		const_iterator temp = *this; // ���݂̃m�[�h

		operator++( );

		return temp;
	}

#ifdef _BIDIRECTIONAL_ITERATOR
	/// @brief ��ނ���
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return ��ތ�̃C�e���[�^�̎Q��
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

	/// @brief ��ނ���
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param �_�~�[
	/// @return ��ޑO�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_iterator tree< ValueType, Allocator >::const_iterator::operator--( int )
	{
		const_iterator temp = *this; // ���݂̃m�[�h

		operator--( );

		return temp;
	}
#endif //_BIDIRECTIONAL_ITERATOR

	/// @brief �C�e���[�^�������Ɠ����������肷��
	/// @attention �������@������ł��邩�͍l�����Ȃ�
	/// @param[in] iter ���肷��C�e���[�^
	/// @retval true �����Ɠ����m�[�h���w���Ă���
	/// @retval false �����ƈقȂ�m�[�h���w���Ă���
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::const_iterator::operator==( const const_iterator &iter )const
	{
		return node_ == iter.node_; // && incr_ == iter.incr_;
	}

	/// @brief �C�e���[�^�������Ɠ������Ȃ������肷��
	/// @attention �������@������ł��邩�͍l�����Ȃ�
	/// @param[in] iter ���肷��C�e���[�^
	/// @retval true �����ƈقȂ�m�[�h���w���Ă���
	/// @retval false �����Ɠ����m�[�h���w���Ă���
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::const_iterator::operator!=( const const_iterator &iter )const
	{
		return !operator==( iter );
	}

	/// @brief �Q�Ɣ��������s��
	/// @attention �����ȃm�[�h���w���Ă���ꍇ�C invalid_iterator ��O�𓊂���
	/// @return �C�e���[�^�̎w���f�[�^�̒萔�Q��
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

	/// @brief �Ԑڃ����o�Q�Ƃ��s��
	/// @attention �����ȃm�[�h���w���Ă���ꍇ�C invalid_iterator ��O�𓊂���
	/// @return �C�e���[�^�̎w���I�u�W�F�N�g�̒萔�|�C���^
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

	/// @brief ����������
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::iterator::iterator( )
	{
	}

	/// @brief �C�e���[�^�𕡐�����
	/// @param[in] iter ��������C�e���[�^
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::iterator::iterator( const iterator &iter ) : const_iterator( iter )
	{
	}

	/// @brief �����ŏ���������
	/// @param[in] node �C�e���[�^�̎w���m�[�h�̃|�C���^ 
	/// @param[in] root ��������؍\���̍��̃|�C���^
	/// @param[in] incr �O�i�֐�
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::iterator::iterator( Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const ) : const_iterator( node, root, incr )
	{
	}

	/// @brief �C�e���[�^���w���m�[�h���擾����
	/// @return �C�e���[�^���w���m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::iterator::node( )const
	{
		return const_cast< Node * >( node_ );
	}

	/// @brief �O�i����
	/// @return �O�i��̃C�e���[�^�̎Q��
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator &tree< ValueType, Allocator >::iterator::operator++( )
	{
		if ( node_ )
		{
			node_ = ( node_->*incr_ )( root_ );
		}
	
		return *this;
	}

	/// @brief �O�i����
	/// @param �_�~�[
	/// @return �O�i�O�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::iterator::operator++( int )
	{
		iterator temp = *this; // ���݂̃m�[�h

		operator++( );

		return temp;
	}

#ifdef _BIDIRECTIONAL_ITERATOR
	/// @brief ��ނ���
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return ��ތ�̃C�e���[�^�̎Q��
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

	/// @brief ��ނ���
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param �_�~�[
	/// @return ��ޑO�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::iterator tree< ValueType, Allocator >::iterator::operator--( int )
	{
		iterator temp = *this; // ���݂̃m�[�h

		operator--( );

		return temp;
	}
#endif //_BIDIRECTIONAL_ITERATOR

	/// @brief �Q�Ɣ��������s��
	/// @attention �����ȃm�[�h���w���Ă���ꍇ�C invalid_iterator ��O�𓊂���
	/// @return �C�e���[�^�̎w���f�[�^�̎Q��
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

	/// @brief �Ԑڃ����o�Q�Ƃ��s��
	/// @attention �����ȃm�[�h���w���Ă���ꍇ�C invalid_iterator ��O�𓊂���
	/// @return �C�e���[�^�̎w���I�u�W�F�N�g�̃|�C���^
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

	/// @brief ����������
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_reverse_iterator::const_reverse_iterator( ) : node_( NULL ), root_( NULL ), incr_( &Node::*preorder_incr )
	{
	}

	/// @brief �C�e���[�^�𕡐�����
	/// @param[in] iter ��������C�e���[�^
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_reverse_iterator::const_reverse_iterator( const const_reverse_iterator &iter ) : node_( iter.node_ ), root_( iter.root_ ), incr_( iter.incr_ )
	{
	}

	/// @brief �����ŏ���������
	/// @param[in] node �C�e���[�^�̎w���m�[�h�̃|�C���^ 
	/// @param[in] root ��������؍\���̍��̃|�C���^
	/// @param[in] incr �O�i�֐�
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::const_reverse_iterator::const_reverse_iterator( const Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const ) : node_( node ), root_( root ), incr_( incr )
	{
	}

	/// @brief �C�e���[�^���w���m�[�h���擾����
	/// @return �C�e���[�^���w���m�[�h�̒萔�|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::const_reverse_iterator::node( )const
	{
		return node_;
	}

	/// @brief ��������؍\���̍����擾����
	/// @return ��������؍\���̍��̃|�C���^
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::const_reverse_iterator::root( )const
	{
		return root_;
	}
	
	/// @brief �O�i�֐����擾����
	/// @return �O�i�֐�
	template < class ValueType, class Allocator >
	inline const typename tree< ValueType, Allocator >::Node *( tree< ValueType, Allocator >::Node::*tree< ValueType, Allocator >::const_reverse_iterator::incr( ) )( const Node * )const
	{
		return incr_;
	}

	/// @brief �O�i����
	/// @return �O�i��̃C�e���[�^�̎Q��
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator &tree< ValueType, Allocator >::const_reverse_iterator::operator++( )
	{
		if ( node_ )
		{
			node_ = ( node_->*incr_ )( root_ );
		}

		return *this;
	}

	/// @brief �O�i����
	/// @param �_�~�[
	/// @return �O�i�O�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::const_reverse_iterator::operator++( int )
	{
		const_reverse_iterator temp = *this; // ���݂̃m�[�h

		operator++( );

		return temp;
	}

#ifdef _BIDIRECTIONAL_ITERATOR
	/// @brief ��ނ���
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return ��ތ�̃C�e���[�^�̎Q��
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

	/// @brief ��ނ���
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param �_�~�[
	/// @return ��ޑO�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::const_reverse_iterator tree< ValueType, Allocator >::const_reverse_iterator::operator--( int )
	{
		const_reverse_iterator temp = *this; // ���݂̃m�[�h

		operator--( );

		return temp;
	}
#endif //_BIDIRECTIONAL_ITERATOR

	/// @brief �C�e���[�^�������Ɠ����������肷��
	/// @attention �������@������ł��邩�͍l�����Ȃ�
	/// @param[in] iter ���肷��C�e���[�^
	/// @retval true �����Ɠ����m�[�h���w���Ă���
	/// @retval false �����ƈقȂ�m�[�h���w���Ă���
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::const_reverse_iterator::operator==( const const_reverse_iterator &iter )const
	{
		return node_ == iter.node_; // && incr_ == iter.incr_;
	}

	/// @brief �C�e���[�^�������Ɠ������Ȃ������肷��
	/// @attention �������@������ł��邩�͍l�����Ȃ�
	/// @param[in] iter ���肷��C�e���[�^
	/// @retval true �����ƈقȂ�m�[�h���w���Ă���
	/// @retval false �����Ɠ����m�[�h���w���Ă���
	template < class ValueType, class Allocator >
	inline bool tree< ValueType, Allocator >::const_reverse_iterator::operator!=( const const_reverse_iterator &iter )const
	{
		return !operator==( iter );
	}

	/// @brief �Q�Ɣ��������s��
	/// @attention �����ȃm�[�h���w���Ă���ꍇ�C invalid_iterator ��O�𓊂���
	/// @return �C�e���[�^�̎w���f�[�^�̒萔�Q��
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

	/// @brief �Ԑڃ����o�Q�Ƃ��s��
	/// @attention �����ȃm�[�h���w���Ă���ꍇ�C invalid_iterator ��O�𓊂���
	/// @return �C�e���[�^�̎w���I�u�W�F�N�g�̒萔�|�C���^
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

	/// @brief ����������
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::reverse_iterator::reverse_iterator( )
	{
	}

	/// @brief �C�e���[�^�𕡐�����
	/// @param[in] iter ��������C�e���[�^
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::reverse_iterator::reverse_iterator( const reverse_iterator &iter ) : const_reverse_iterator( iter )
	{
	}

	/// @brief �����ŏ���������
	/// @param[in] node �C�e���[�^�̎w���m�[�h�̃|�C���^ 
	/// @param[in] root ��������؍\���̍��̃|�C���^
	/// @param[in] incr �O�i�֐�
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::reverse_iterator::reverse_iterator( Node *node, const Node *root, const Node *( Node::*incr )( const Node * )const ) : const_reverse_iterator( node, root, incr )
	{
	}

	/// @brief �C�e���[�^���w���m�[�h���擾����
	/// @return �C�e���[�^���w���m�[�h�̃|�C���^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::Node *tree< ValueType, Allocator >::reverse_iterator::node( )const
	{
		return const_cast< Node * >( node_ );
	}

	/// @brief �O�i����
	/// @return �O�i��̃C�e���[�^�̎Q��
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator &tree< ValueType, Allocator >::reverse_iterator::operator++( )
	{
		if ( node_ )
		{
			node_ = ( node_->*incr_ )( root_ );
		}

		return *this;
	}

	/// @brief �O�i����
	/// @param �_�~�[
	/// @return �O�i�O�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::reverse_iterator::operator++( int )
	{
		reverse_iterator temp = *this; // ���݂̃m�[�h

		operator++( );

		return temp;
	}

#ifdef _BIDIRECTIONAL_ITERATOR
	/// @brief ��ނ���
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @return ��ތ�̃C�e���[�^�̎Q��
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

	/// @brief ��ނ���
	/// @attention �������̂��ߎg�p���Ă͂Ȃ�Ȃ�
	/// @param �_�~�[
	/// @return ��ޑO�̃C�e���[�^
	template < class ValueType, class Allocator >
	inline typename tree< ValueType, Allocator >::reverse_iterator tree< ValueType, Allocator >::reverse_iterator::operator--( int )
	{
		reverse_iterator temp = *this; // ���݂̃m�[�h

		operator--( );

		return temp;
	}
#endif //_BIDIRECTIONAL_ITERATOR

	/// @brief �Q�Ɣ��������s��
	/// @attention �����ȃm�[�h���w���Ă���ꍇ�C invalid_iterator ��O�𓊂���
	/// @return �C�e���[�^�̎w���f�[�^�̎Q��
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

	/// @brief �Ԑڃ����o�Q�Ƃ��s��
	/// @attention �����ȃm�[�h���w���Ă���ꍇ�C invalid_iterator ��O�𓊂���
	/// @return �C�e���[�^�̎w���I�u�W�F�N�g�̃|�C���^
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

	/// @brief �����̃m�[�h�����Ƃ���؍\���̐󂢕������쐬����
	/// @param[in] root ��������؍\���̍�
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Subtree::Subtree( const Node *root ) : tree( root )
	{
	}

	/// @brief �Q�Ƃ�j������
	template < class ValueType, class Allocator >
	inline tree< ValueType, Allocator >::Subtree::~Subtree( )
	{
		root_ = NULL;
	}
}

#endif //_TMATSU_TREE_H_