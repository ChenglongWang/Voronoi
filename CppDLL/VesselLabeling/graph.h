/**
* @file graph.h
* @brief �O���t�̃w�b�_�t�@�C��
* @version 3.0
* @author ���É���w ���Ȋw������ ���f�B�A�Ȋw��U �X������   351203192   ���� �N�N
* @date 2013/10/21
*/

#ifndef _TMATSU_GRAPH_H_
#define _TMATSU_GRAPH_H_

#include <vector>
#include <list>
#include <set>
#include <queue>
#include <algorithm>
#include <functional>

namespace tmatsu 
{
	template < class Graph, class Tree >
	void kruscal( const Graph &graph, Tree &tree );
	template < class Graph, class Tree, class Predicator >
	void kruscal( const Graph &graph, Tree &tree, Predicator pred );
	template < class Graph, class Tree >
	void prim( const Graph &graph, Tree &tree, size_t init );
	template < class Graph, class Tree, class Predicator >
	void prim( const Graph &graph, Tree &tree, size_t init, Predicator pred );
	template < class Digraph, class Arborescence >
	void edmonds( const Digraph &digraph, Arborescence &arborescence );
	template < class Digraph, class Arborescence, class Predicator >
	void edmonds( const Digraph &digraph, Arborescence &arborescence, Predicator pred );

	/// @brief �N���X�J���@�ɂ��ŏ��S��؂����߂�
	/// @attention �񕉂̏d�݂����G�b�W�݂̂�L���ȃG�b�W�Ƃ݂Ȃ�
	/// @attention ���͂��Ώ̍s��łȂ���ΐ��������삵�Ȃ�
	/// @param[in] graph �ŏ��S��؂����߂�O���t��\���s��
	/// @param[out] tree �ŏ��S��؂��o�͂���s��
	template < class Graph, class Tree >
	inline void kruscal( const Graph &graph, Tree &tree )
	{
		kruscal( graph, tree, std::less< Graph::value_type >( ) );
	}

	/// @brief �N���X�J���@�ɂ��ŏ��S��؂����߂�
	/// @attention �񕉂̏d�݂����G�b�W�݂̂�L���ȃG�b�W�Ƃ݂Ȃ�
	/// @attention ���͂��Ώ̍s��łȂ���ΐ��������삵�Ȃ�
	/// @param[in] graph �ŏ��S��؂����߂�O���t��\���s��
	/// @param[out] tree �ŏ��S��؂��o�͂���s��
	/// @param[in] pred ��r�֐�
	template < class Graph, class Tree, class Predicator >
	inline void kruscal( const Graph &graph, Tree &tree, Predicator pred )
	{
		// �G�b�W
		struct Edge
		{
			Graph::size_type rear, front; // �ڑ�����m�[�h
			Graph::value_type weight; // �d��
		};
		
		// �v�f�̔�r�֐�
		struct EdgePredicator
		{
			// �֐��Ăяo�����Z�q
			bool operator( )( const Edge &left, const Edge &right )
			{
				return Predicator( )( right.weight, left.weight );
			}
		};

		std::list< std::list< Graph::size_type > > forest; // �X
		std::priority_queue< Edge, std::vector< Edge >, EdgePredicator > q; // �����t���L���[

		// �̈���m�ۂ���
		tree.resize( graph.size1( ), graph.size2( ) );
		
		for ( Graph::size_type j = 0; j < graph.size2( ); j++ )
		{
			for ( Graph::size_type i = 0; i < graph.size1( ); i++ )
			{
				tree( i, j ) = -1;
			}
		}

		// �X���쐬����
		for ( Graph::size_type i = 0; i < graph.size1( ); i++ )
		{
			std::list< Graph::size_type > t; // �B��̃m�[�h����Ȃ��

			t.push_back( i );
			forest.push_back( t );
		}

		// �G�b�W���\�[�g����
		for ( Graph::size_type j = 0; j < graph.size2( ); j++ )
		{
			for ( Graph::size_type i = j + 1; i < graph.size1( ); i++ )
			{
				if ( graph( i, j ) >= 0 )
				{
					Edge edge = { i, j, graph( i, j ) }; // �G�b�W

					q.push( edge );
				}
			}
		}

		// �L���[����G�b�W���폜���Ȃ���؂��쐬����
		while ( !q.empty( ) )
		{
			Edge edge = q.top( ); // �ŏ��̏d�݂̃G�b�W
			std::list< std::list< Graph::size_type > >::iterator rear = forest.end( ), front = forest.end( ), e = forest.end( ); // �G�b�W�̗��[�̃m�[�h�����������
			
			// �G�b�W���폜����
			q.pop( );

			// �G�b�W�̗��[�̃m�[�h����������؂��擾����
			for ( std::list< std::list< Graph::size_type > >::iterator iter = forest.begin( ); ( rear == forest.end( ) || front == forest.end( ) ) && iter != forest.end( ); iter++ )
			{
				if ( rear == forest.end( ) && std::find( iter->begin( ), iter->end( ), edge.rear ) != iter->end( ) )
				{
					rear = iter;
				}

				if ( front == forest.end( ) && std::find( iter->begin( ), iter->end( ), edge.front ) != iter->end( ) )
				{
					front = iter;
				}
			}
			
			// �H�����Ȃ��Ȃ�G�b�W��؂ɒǉ�����
			if ( rear != front )
			{
				// �G�b�W��ǉ�����
				tree( edge.rear, edge.front ) = tree( edge.front, edge.rear ) = edge.weight;

				// �؂��}�[�W����
				rear->merge( *front );
				forest.erase( front );
			}
		}
	}

	/// @brief �v�����@�ɂ��ŏ��S��؂����߂�
	/// @attention �񕉂̏d�݂����G�b�W�݂̂�L���ȃG�b�W�Ƃ݂Ȃ�
	/// @attention ���͂��Ώ̍s��łȂ���ΐ��������삵�Ȃ�
	/// @param[in] graph �ŏ��S��؂����߂�O���t��\���s��
	/// @param[out] tree �ŏ��S��؂��o�͂���s��
	/// @param[in] init �����m�[�h
	template < class Graph, class Tree >
	inline void prim( const Graph &graph, Tree &tree, size_t init )
	{
		prim( graph, tree, init, std::less< Graph::value_type >( ) );
	}

	/// @brief �v�����@�ɂ��ŏ��S��؂����߂�
	/// @attention �񕉂̏d�݂����G�b�W�݂̂�L���ȃG�b�W�Ƃ݂Ȃ�
	/// @attention ���͂��Ώ̍s��łȂ���ΐ��������삵�Ȃ�
	/// @param[in] graph �ŏ��S��؂����߂�O���t��\���s��
	/// @param[out] tree �ŏ��S��؂��o�͂���s��
	/// @param[in] init �����m�[�h
	/// @param[in] pred ��r�֐�
	template < class Graph, class Tree, class Predicator >
	inline void prim( const Graph &graph, Tree &tree, size_t init, Predicator pred )
	{
		// �G�b�W
		struct Edge
		{
			std::set< Graph::size_type >::iterator rear, front; // �ڑ����钸�_
			Graph::value_type weight; // �d��
		};

		std::set< Graph::size_type > graph_vertices, tree_vertices; // ���_�̏W��

		// �̈���m�ۂ���
		tree.resize( graph.size1( ), graph.size2( ) );
		
		for ( Graph::size_type j = 0; j < graph.size2( ); j++ )
		{
			for ( Graph::size_type i = 0; i < graph.size1( ); i++ )
			{
				tree( i, j ) = -1;
			}
		}

		// ���_�̏W��������������
		tree_vertices.insert( init );

		for ( Graph::size_type i = 0; i < graph.size1( ); i++ )
		{
			if ( i != init )
			{
				graph_vertices.insert( i );
			}
		}

		// �S�Ă̒��_���؂ɉ����܂Ŗ؂��g�傷��
		while ( !graph_vertices.empty( ) )
		{
			Edge edge = { tree_vertices.end( ), graph_vertices.end( ), -1 }; // �d�݂��ŏ��̃G�b�W

			// �؂Ɋ܂܂�钸�_�ɐڑ�����d�݂��ŏ��̃G�b�W�����߂�
			for ( std::set< Graph::size_type >::const_iterator tree_iter = tree_vertices.begin( ); tree_iter != tree_vertices.end( ); tree_iter++ )
			{
				// �d�݂��ŏ��̃G�b�W�����߂�
				for ( std::set< Graph::size_type >::const_iterator graph_iter = graph_vertices.begin( ); graph_iter != graph_vertices.end( ); graph_iter++ )
				{
					if ( edge.weight < 0 || ( graph( *tree_iter, *graph_iter ) >= 0 && pred( graph( *tree_iter, *graph_iter ), edge.weight ) ) )
					{
						edge.rear = tree_iter;
						edge.front = graph_iter;
						edge.weight = graph( *graph_iter, *tree_iter );
					}
				}
			}

			// �d�݂��ŏ��̃G�b�W��؂ɒǉ�����
			if ( edge.weight >= 0 )
			{
				tree( *edge.rear, *edge.front ) = tree( *edge.front, *edge.rear ) = edge.weight;
				tree_vertices.insert( *edge.front );
				graph_vertices.erase( edge.front );
			}
		}
	}

	/// @brief �G�h�����Y�@�ɂ��œK��������߂�
	/// @attention �񕉂̏d�݂����G�b�W�݂̂�L���ȃG�b�W�Ƃ݂Ȃ�
	/// @param[in] digraph �œK��������߂�L���O���t��\���s��
	/// @param[out] arborescence �œK������o�͂���s��
	template < class Digraph, class Arborescence >
	inline void edmonds( const Digraph &digraph, Arborescence &arborescence )
	{
		edmonds( digraph, arborescence, std::less< Digraph::value_type >( ) );
	}

	/// @brief �G�h�����Y�@�ɂ��œK��������߂�
	/// @attention �񕉂̏d�݂����G�b�W�݂̂�L���ȃG�b�W�Ƃ݂Ȃ�
	/// @param[in] digraph �œK��������߂�L���O���t��\���s��
	/// @param[out] arborescence �œK������o�͂���s��
	/// @param[in] pred ��r�֐�
	template < class Digraph, class Arborescence, class Predicator >
	inline void edmonds( const Digraph &digraph, Arborescence &arborescence, Predicator pred )
	{
		// �G�b�W
		struct Edge
		{
			Digraph::size_type rear, front; // �ڑ����钸�_

			// �f�t�H���g�R���X�g���N�^
			Edge( ) : rear( -1 ), front( -1 )
			{
			}

			// �R���X�g���N�^
			Edge( Digraph::size_type rear, Digraph::size_type front ) : rear( rear ), front( front )
			{
			}
		};

		// �H
		struct Circuit 
		{
			std::list< Edge > edges; // �S�ẴG�b�W
			Edge max_edge; // �ő�̏d�݂����G�b�W
		};

		// �I�_�̌��݂̃C���f�b�N�X���w��̃C���f�b�N�X�Ɠ������G�b�W��T������֐�
		class FrontPred
		{
			std::vector< std::list< Digraph::size_type > > &current_indices_; // ���݂̃C���f�b�N�X
			Digraph::size_type index_; // �T������C���f�b�N�X
		public:
			// �R���X�g���N�^
			FrontPred( std::vector< std::list< Digraph::size_type > > &current_indices, Digraph::size_type index ) : current_indices_( current_indices ), index_( index )
			{
			}

			// �֐��Ăяo�����Z�q
			bool operator( )( const Edge &a )
			{
				return current_indices_[ a.front ].back( ) == index_;
			}
		};
		
		std::vector< std::list< Digraph::size_type > > current_indices( digraph.size1( ) ); // �e���_�̌��݂̃C���f�b�N�X
		Digraph current_weights = digraph; // ���݂̏d��
		std::list< Digraph::size_type > vertices; // �o�P�b�g�ɓ�����Ă��Ȃ����_
		std::list< Edge > edges; // ���Ƀo�P�b�g�ɓ����ꂽ�G�b�W
		std::list< Circuit > circuits; // �H�̏��

		// �̈���m�ۂ���
		arborescence.resize( digraph.size1( ), digraph.size2( ) );
		
		for ( Digraph::size_type j = 0; j < digraph.size2( ); j++ )
		{
			for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
			{
				arborescence( i, j ) = -1;
			}
		}

		// ����������
		for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
		{
			vertices.push_back( i );
			current_indices[ i ].push_back( i );
			current_weights( i, i ) = -1;
		}

		// ���_�����k�����Ȃ��璸�_�ƃG�b�W���o�͂ɉ�����
		while ( !vertices.empty( ) )
		{
			Edge edge( digraph.size1( ), vertices.front( ) ); // �o�P�b�g�ɓ����G�b�W
			Digraph::value_type min_wt = -1; // �ŏ��̏d��
			std::list< Edge >::const_iterator path = edges.end( ); // �o�P�b�g�ɓ����G�b�W�̏I�_����n�_�ւ̃p�X��̒��_
			
#ifdef _DEBUG_GRAPH
			std::cout << std::endl << "vertices{ ";

			for ( std::list< Digraph::size_type >::const_iterator iter = vertices.begin( ); iter != vertices.end( ); iter++ )
			{
				std::cout << *iter << ' ';
			}

			std::cout << '}' << std::endl;
#endif //_DEBUG_GRAPH

			// �o�P�b�g�ɓ����G�b�W�����߂�
			for ( Digraph::size_type j = 0; j < digraph.size2( ); j++ )
			{
				if ( current_indices[ j ].back( ) == vertices.front( ) )
				{
					for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
					{
						Digraph::value_type wt = current_weights( i, j ); // �d��

						if ( wt >= 0 && ( min_wt < 0 || pred( wt, min_wt ) ) )
						{
							edge = Edge( i, j );
							min_wt = wt;
						}
					}
				}
			}

#ifdef _DEBUG_GRAPH
			if ( edge.rear != digraph.size1( ) )
			{
				std::cout << "select( " << edge.rear << " -> " << edge.front << " )" << std::endl;
			}
#endif //_DEBUG_GRAPH

			// �H�𐶂��Ă��Ȃ����m�F����
			if ( edge.rear != digraph.size1( ) )
			{
				for ( path = std::find_if( edges.begin( ), edges.end( ), FrontPred( current_indices, current_indices[ edge.rear ].back( ) ) ); path != edges.end( ) && current_indices[ path->rear ].back( ) != current_indices[ edge.front ].back( ); path = std::find_if( edges.begin( ), edges.end( ), FrontPred( current_indices, current_indices[ path->rear ].back( ) ) ) )
				{
				}
			}

			if ( path == edges.end( ) )
			{
				// �H�𐶂��Ă��Ȃ��Ȃ�΁C�G�b�W���o�P�b�g�ɓ����
				if ( edge.rear != digraph.size1( ) )
				{
					edges.push_back( edge );
				}

				vertices.pop_front( );
				
			}
			else
			{
				Circuit circuit; // �H�̏��
				Digraph::value_type max_wt = current_weights( edge.rear, edge.front ); // �ő�̏d��
				std::set< Digraph::size_type > vertices_to_be_shrinked; // ���k���钸�_
				
				// �H�𑖍�����
				circuit.edges.push_front( edge );
				circuit.max_edge = edge;
			
				for ( path = std::find_if( edges.begin( ), edges.end( ), FrontPred( current_indices, current_indices[ edge.rear ].back( ) ) ); path != edges.end( ); path = std::find_if( edges.begin( ), edges.end( ), FrontPred( current_indices, current_indices[ path->rear ].back( ) ) ) )
				{
					Digraph::value_type wt = current_weights( path->rear, path->front ); // �d��

#ifdef _DEBUG_GRAPH
					std::cout << "traverse( " << path->rear << " -> " << path->front << " )" << std::endl;
#endif //_DEBUG_GRAPH

					// �H�Ɋ܂܂�钸�_�����߂�
					circuit.edges.push_front( *path );

					// �ő�̏d�݂����߂�
					if ( pred( max_wt, wt ) )
					{
						max_wt = wt;
						circuit.max_edge = *path;
					}

					// �H���������܂ŌJ��Ԃ�
					if( current_indices[ path->rear ].back( ) == current_indices[ edge.front ].back( ) )
					{
						break;
					}
				}

				// �H��P��̒��_�Ɏ��k����
#ifdef _DEBUG_GRAPH
				std::cout << "shrink{ ";
#endif //_DEBUG_GRAPH

				for ( std::list< Edge >::iterator iter = circuit.edges.begin( ); iter != circuit.edges.end( ); iter++ )
				{
					vertices_to_be_shrinked.insert( current_indices[ iter->front ].back( ) );
				}
				
				for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
				{
					if ( vertices_to_be_shrinked.find( current_indices[ i ].back( ) ) != vertices_to_be_shrinked.end( ) )
					{
						current_indices[ i ].push_back( current_indices[ circuit.edges.front( ).front ].back( ) );

#ifdef _DEBUG_GRAPH
						std::cout << i << ' ';
#endif //_DEBUG_GRAPH

					}
				}

#ifdef _DEBUG_GRAPH
				std::cout << "} -> " << current_indices[ circuit.edges.front( ).front ].back( ) << std::endl;
#endif //_DEBUG_GRAPH

				// �H�Ɍ������G�b�W���X�V����
				for ( Digraph::size_type j = 0; j < digraph.size2( ); j++ )
				{
					for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
					{
						if ( current_indices[ i ].back( ) != current_indices[ circuit.edges.front( ).front ].back( ) && current_indices[ j ].back( ) == current_indices[ circuit.edges.front( ).front ].back( ) && current_weights( i, j ) >= 0 )
						{
							Digraph::size_type circuit_vertex = *++current_indices[ j ].rbegin( ); // �H�Ɋ܂܂�钸�_
					
							for ( std::list< Edge >::iterator iter = circuit.edges.begin( ); iter != circuit.edges.end( ); iter++ )
							{
								if ( *++current_indices[ iter->front ].rbegin( ) == circuit_vertex )
								{
									current_weights( i, j ) += max_wt - current_weights( iter->rear, iter->front );

#ifdef _DEBUG_GRAPH
									std::cout << "update( " << i << " -> " << j << " ) -> " << current_weights( i, j ) << " by ( " << iter->rear << " -> " << iter->front << " )" << std::endl;
#endif //_DEBUG_GRAPH

									break;
								}
							}
						}
					}
				}
				
				// �H�Ɋ܂܂��G�b�W���폜����
				for ( std::list< Edge >::iterator iter = edges.begin( ); iter != edges.end( ); )
				{
					if ( current_indices[ iter->rear ].back( ) == current_indices[ circuit.edges.front( ).front ].back( ) && current_indices[ iter->front ].back( ) == current_indices[ circuit.edges.front( ).front ].back( ) )
					{

#ifdef _DEBUG_GRAPH
						std::cout << "erase edge( " << iter->rear << " -> " << iter->front << " )" << std::endl;
#endif //_DEBUG_GRAPH

						iter = edges.erase( iter );
					}
					else
					{
						iter++;
					}
				}

				for ( Digraph::size_type j = 0; j < digraph.size2( ); j++ )
				{
					for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
					{
						if ( current_indices[ i ].back( ) == current_indices[ circuit.edges.front( ).front ].back( ) && current_indices[ j ].back( ) == current_indices[ circuit.edges.front( ).front ].back( ) )
						{

#ifdef _DEBUG_GRAPH
							if ( current_weights( i, j ) >= 0 )
							{
								std::cout << "erase weight( " << i << " -> " << j << " )" << std::endl;
							}
#endif //_DEBUG_GRAPH

							current_weights( i, j ) = -1;
						}
					}
				}

				circuits.push_back( circuit );
				vertices.pop_front( );
				vertices.push_back( circuit.edges.front( ).front );
			}
		}

		// �H�𕜌����C�]���ȕӂ���������
		for ( ; !circuits.empty( ); circuits.pop_back( ) )
		{
			std::list< Edge >::iterator circuit_edge = std::find_if( edges.begin( ), edges.end( ), FrontPred( current_indices, current_indices[ circuits.back( ).edges.front( ).front ].back( ) ) ); // �H�O����̃G�b�W���ڑ�����H���̒��_�֐ڑ�����H���̃G�b�W
			
#ifdef _DEBUG_GRAPH
			std::cout << std::endl << "circuit{ ";

			for ( std::list< Edge >::iterator iter = circuits.back( ).edges.begin( ); iter != circuits.back( ).edges.end( ); iter++ )
			{
				std::cout << "( " << iter->rear << " -> " << iter->front << " ) ";
			}
			
			std::cout << "} with maximum edge( " << circuits.back( ).max_edge.rear << " -> " << circuits.back( ).max_edge.front << " )" << std::endl;
#endif //_DEBUG_GRAPH

			// �H�����Ȃ�Ώd�݂��ő�łȂ��G�b�W���C���łȂ��Ȃ�ΕH�O����̃G�b�W���ڑ����钸�_���I�_�Ƃ��Ȃ��G�b�W��ǉ�����
			for ( std::list< Edge >::iterator iter = circuits.back( ).edges.begin( ); iter != circuits.back( ).edges.end( ); iter++ )
			{
				if ( *++current_indices[ iter->front ].rbegin( ) != *++current_indices[ circuit_edge == edges.end( ) ? circuits.back( ).max_edge.front : circuit_edge->front ].rbegin( ) )
				{
					edges.push_back( *iter );

#ifdef _DEBUG_GRAPH
					std::cout << "add( " << iter->rear << " -> " << iter->front << " )" << std::endl;
#endif //_DEBUG_GRAPH

				}
			}
				
			// ���k�����H�����ɖ߂�
			for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
			{
				if ( current_indices[ i ].back( ) == current_indices[ circuits.back( ).edges.front( ).front ].back( ) )
				{
					current_indices[ i ].pop_back( );
				}
			}
		}

		// ���ʂ��s��ɏo�͂���
		for ( std::list< Edge >::iterator iter = edges.begin( ); iter != edges.end( ); iter++ )
		{
			arborescence( iter->rear, iter->front ) = digraph( iter->rear, iter->front );
		}
	}
}

#endif //_TMATSU_GRAPH_H_