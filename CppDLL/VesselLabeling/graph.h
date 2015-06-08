/**
* @file graph.h
* @brief グラフのヘッダファイル
* @version 3.0
* @author 名古屋大学 情報科学研究科 メディア科学専攻 森研究室   351203192   松﨑 哲朗
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

	/// @brief クラスカル法により最小全域木を求める
	/// @attention 非負の重みを持つエッジのみを有効なエッジとみなす
	/// @attention 入力が対称行列でなければ正しく動作しない
	/// @param[in] graph 最小全域木を求めるグラフを表す行列
	/// @param[out] tree 最小全域木を出力する行列
	template < class Graph, class Tree >
	inline void kruscal( const Graph &graph, Tree &tree )
	{
		kruscal( graph, tree, std::less< Graph::value_type >( ) );
	}

	/// @brief クラスカル法により最小全域木を求める
	/// @attention 非負の重みを持つエッジのみを有効なエッジとみなす
	/// @attention 入力が対称行列でなければ正しく動作しない
	/// @param[in] graph 最小全域木を求めるグラフを表す行列
	/// @param[out] tree 最小全域木を出力する行列
	/// @param[in] pred 比較関数
	template < class Graph, class Tree, class Predicator >
	inline void kruscal( const Graph &graph, Tree &tree, Predicator pred )
	{
		// エッジ
		struct Edge
		{
			Graph::size_type rear, front; // 接続するノード
			Graph::value_type weight; // 重み
		};
		
		// 要素の比較関数
		struct EdgePredicator
		{
			// 関数呼び出し演算子
			bool operator( )( const Edge &left, const Edge &right )
			{
				return Predicator( )( right.weight, left.weight );
			}
		};

		std::list< std::list< Graph::size_type > > forest; // 森
		std::priority_queue< Edge, std::vector< Edge >, EdgePredicator > q; // 順序付きキュー

		// 領域を確保する
		tree.resize( graph.size1( ), graph.size2( ) );
		
		for ( Graph::size_type j = 0; j < graph.size2( ); j++ )
		{
			for ( Graph::size_type i = 0; i < graph.size1( ); i++ )
			{
				tree( i, j ) = -1;
			}
		}

		// 森を作成する
		for ( Graph::size_type i = 0; i < graph.size1( ); i++ )
		{
			std::list< Graph::size_type > t; // 唯一のノードからなる木

			t.push_back( i );
			forest.push_back( t );
		}

		// エッジをソートする
		for ( Graph::size_type j = 0; j < graph.size2( ); j++ )
		{
			for ( Graph::size_type i = j + 1; i < graph.size1( ); i++ )
			{
				if ( graph( i, j ) >= 0 )
				{
					Edge edge = { i, j, graph( i, j ) }; // エッジ

					q.push( edge );
				}
			}
		}

		// キューからエッジを削除しながら木を作成する
		while ( !q.empty( ) )
		{
			Edge edge = q.top( ); // 最小の重みのエッジ
			std::list< std::list< Graph::size_type > >::iterator rear = forest.end( ), front = forest.end( ), e = forest.end( ); // エッジの両端のノードが所属する木
			
			// エッジを削除する
			q.pop( );

			// エッジの両端のノードが所属する木を取得する
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
			
			// 閉路を作らないならエッジを木に追加する
			if ( rear != front )
			{
				// エッジを追加する
				tree( edge.rear, edge.front ) = tree( edge.front, edge.rear ) = edge.weight;

				// 木をマージする
				rear->merge( *front );
				forest.erase( front );
			}
		}
	}

	/// @brief プリム法により最小全域木を求める
	/// @attention 非負の重みを持つエッジのみを有効なエッジとみなす
	/// @attention 入力が対称行列でなければ正しく動作しない
	/// @param[in] graph 最小全域木を求めるグラフを表す行列
	/// @param[out] tree 最小全域木を出力する行列
	/// @param[in] init 初期ノード
	template < class Graph, class Tree >
	inline void prim( const Graph &graph, Tree &tree, size_t init )
	{
		prim( graph, tree, init, std::less< Graph::value_type >( ) );
	}

	/// @brief プリム法により最小全域木を求める
	/// @attention 非負の重みを持つエッジのみを有効なエッジとみなす
	/// @attention 入力が対称行列でなければ正しく動作しない
	/// @param[in] graph 最小全域木を求めるグラフを表す行列
	/// @param[out] tree 最小全域木を出力する行列
	/// @param[in] init 初期ノード
	/// @param[in] pred 比較関数
	template < class Graph, class Tree, class Predicator >
	inline void prim( const Graph &graph, Tree &tree, size_t init, Predicator pred )
	{
		// エッジ
		struct Edge
		{
			std::set< Graph::size_type >::iterator rear, front; // 接続する頂点
			Graph::value_type weight; // 重み
		};

		std::set< Graph::size_type > graph_vertices, tree_vertices; // 頂点の集合

		// 領域を確保する
		tree.resize( graph.size1( ), graph.size2( ) );
		
		for ( Graph::size_type j = 0; j < graph.size2( ); j++ )
		{
			for ( Graph::size_type i = 0; i < graph.size1( ); i++ )
			{
				tree( i, j ) = -1;
			}
		}

		// 頂点の集合を初期化する
		tree_vertices.insert( init );

		for ( Graph::size_type i = 0; i < graph.size1( ); i++ )
		{
			if ( i != init )
			{
				graph_vertices.insert( i );
			}
		}

		// 全ての頂点が木に加わるまで木を拡大する
		while ( !graph_vertices.empty( ) )
		{
			Edge edge = { tree_vertices.end( ), graph_vertices.end( ), -1 }; // 重みが最小のエッジ

			// 木に含まれる頂点に接続する重みが最小のエッジを求める
			for ( std::set< Graph::size_type >::const_iterator tree_iter = tree_vertices.begin( ); tree_iter != tree_vertices.end( ); tree_iter++ )
			{
				// 重みが最小のエッジを求める
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

			// 重みが最小のエッジを木に追加する
			if ( edge.weight >= 0 )
			{
				tree( *edge.rear, *edge.front ) = tree( *edge.front, *edge.rear ) = edge.weight;
				tree_vertices.insert( *edge.front );
				graph_vertices.erase( edge.front );
			}
		}
	}

	/// @brief エドモンズ法により最適分岐を求める
	/// @attention 非負の重みを持つエッジのみを有効なエッジとみなす
	/// @param[in] digraph 最適分岐を求める有向グラフを表す行列
	/// @param[out] arborescence 最適分岐を出力する行列
	template < class Digraph, class Arborescence >
	inline void edmonds( const Digraph &digraph, Arborescence &arborescence )
	{
		edmonds( digraph, arborescence, std::less< Digraph::value_type >( ) );
	}

	/// @brief エドモンズ法により最適分岐を求める
	/// @attention 非負の重みを持つエッジのみを有効なエッジとみなす
	/// @param[in] digraph 最適分岐を求める有向グラフを表す行列
	/// @param[out] arborescence 最適分岐を出力する行列
	/// @param[in] pred 比較関数
	template < class Digraph, class Arborescence, class Predicator >
	inline void edmonds( const Digraph &digraph, Arborescence &arborescence, Predicator pred )
	{
		// エッジ
		struct Edge
		{
			Digraph::size_type rear, front; // 接続する頂点

			// デフォルトコンストラクタ
			Edge( ) : rear( -1 ), front( -1 )
			{
			}

			// コンストラクタ
			Edge( Digraph::size_type rear, Digraph::size_type front ) : rear( rear ), front( front )
			{
			}
		};

		// 閉路
		struct Circuit 
		{
			std::list< Edge > edges; // 全てのエッジ
			Edge max_edge; // 最大の重みを持つエッジ
		};

		// 終点の現在のインデックスが指定のインデックスと等しいエッジを探索する関数
		class FrontPred
		{
			std::vector< std::list< Digraph::size_type > > &current_indices_; // 現在のインデックス
			Digraph::size_type index_; // 探索するインデックス
		public:
			// コンストラクタ
			FrontPred( std::vector< std::list< Digraph::size_type > > &current_indices, Digraph::size_type index ) : current_indices_( current_indices ), index_( index )
			{
			}

			// 関数呼び出し演算子
			bool operator( )( const Edge &a )
			{
				return current_indices_[ a.front ].back( ) == index_;
			}
		};
		
		std::vector< std::list< Digraph::size_type > > current_indices( digraph.size1( ) ); // 各頂点の現在のインデックス
		Digraph current_weights = digraph; // 現在の重み
		std::list< Digraph::size_type > vertices; // バケットに入れられていない頂点
		std::list< Edge > edges; // 既にバケットに入れられたエッジ
		std::list< Circuit > circuits; // 閉路の情報

		// 領域を確保する
		arborescence.resize( digraph.size1( ), digraph.size2( ) );
		
		for ( Digraph::size_type j = 0; j < digraph.size2( ); j++ )
		{
			for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
			{
				arborescence( i, j ) = -1;
			}
		}

		// 初期化する
		for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
		{
			vertices.push_back( i );
			current_indices[ i ].push_back( i );
			current_weights( i, i ) = -1;
		}

		// 頂点を収縮させながら頂点とエッジを出力に加える
		while ( !vertices.empty( ) )
		{
			Edge edge( digraph.size1( ), vertices.front( ) ); // バケットに入れるエッジ
			Digraph::value_type min_wt = -1; // 最小の重み
			std::list< Edge >::const_iterator path = edges.end( ); // バケットに入れるエッジの終点から始点へのパス上の頂点
			
#ifdef _DEBUG_GRAPH
			std::cout << std::endl << "vertices{ ";

			for ( std::list< Digraph::size_type >::const_iterator iter = vertices.begin( ); iter != vertices.end( ); iter++ )
			{
				std::cout << *iter << ' ';
			}

			std::cout << '}' << std::endl;
#endif //_DEBUG_GRAPH

			// バケットに入れるエッジを求める
			for ( Digraph::size_type j = 0; j < digraph.size2( ); j++ )
			{
				if ( current_indices[ j ].back( ) == vertices.front( ) )
				{
					for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
					{
						Digraph::value_type wt = current_weights( i, j ); // 重み

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

			// 閉路を生じていないか確認する
			if ( edge.rear != digraph.size1( ) )
			{
				for ( path = std::find_if( edges.begin( ), edges.end( ), FrontPred( current_indices, current_indices[ edge.rear ].back( ) ) ); path != edges.end( ) && current_indices[ path->rear ].back( ) != current_indices[ edge.front ].back( ); path = std::find_if( edges.begin( ), edges.end( ), FrontPred( current_indices, current_indices[ path->rear ].back( ) ) ) )
				{
				}
			}

			if ( path == edges.end( ) )
			{
				// 閉路を生じていないならば，エッジをバケットに入れる
				if ( edge.rear != digraph.size1( ) )
				{
					edges.push_back( edge );
				}

				vertices.pop_front( );
				
			}
			else
			{
				Circuit circuit; // 閉路の情報
				Digraph::value_type max_wt = current_weights( edge.rear, edge.front ); // 最大の重み
				std::set< Digraph::size_type > vertices_to_be_shrinked; // 収縮する頂点
				
				// 閉路を走査する
				circuit.edges.push_front( edge );
				circuit.max_edge = edge;
			
				for ( path = std::find_if( edges.begin( ), edges.end( ), FrontPred( current_indices, current_indices[ edge.rear ].back( ) ) ); path != edges.end( ); path = std::find_if( edges.begin( ), edges.end( ), FrontPred( current_indices, current_indices[ path->rear ].back( ) ) ) )
				{
					Digraph::value_type wt = current_weights( path->rear, path->front ); // 重み

#ifdef _DEBUG_GRAPH
					std::cout << "traverse( " << path->rear << " -> " << path->front << " )" << std::endl;
#endif //_DEBUG_GRAPH

					// 閉路に含まれる頂点を求める
					circuit.edges.push_front( *path );

					// 最大の重みを求める
					if ( pred( max_wt, wt ) )
					{
						max_wt = wt;
						circuit.max_edge = *path;
					}

					// 閉路を一周するまで繰り返す
					if( current_indices[ path->rear ].back( ) == current_indices[ edge.front ].back( ) )
					{
						break;
					}
				}

				// 閉路を単一の頂点に収縮する
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

				// 閉路に向かうエッジを更新する
				for ( Digraph::size_type j = 0; j < digraph.size2( ); j++ )
				{
					for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
					{
						if ( current_indices[ i ].back( ) != current_indices[ circuit.edges.front( ).front ].back( ) && current_indices[ j ].back( ) == current_indices[ circuit.edges.front( ).front ].back( ) && current_weights( i, j ) >= 0 )
						{
							Digraph::size_type circuit_vertex = *++current_indices[ j ].rbegin( ); // 閉路に含まれる頂点
					
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
				
				// 閉路に含まれるエッジを削除する
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

		// 閉路を復元し，余分な辺を除去する
		for ( ; !circuits.empty( ); circuits.pop_back( ) )
		{
			std::list< Edge >::iterator circuit_edge = std::find_if( edges.begin( ), edges.end( ), FrontPred( current_indices, current_indices[ circuits.back( ).edges.front( ).front ].back( ) ) ); // 閉路外からのエッジが接続する閉路内の頂点へ接続する閉路内のエッジ
			
#ifdef _DEBUG_GRAPH
			std::cout << std::endl << "circuit{ ";

			for ( std::list< Edge >::iterator iter = circuits.back( ).edges.begin( ); iter != circuits.back( ).edges.end( ); iter++ )
			{
				std::cout << "( " << iter->rear << " -> " << iter->front << " ) ";
			}
			
			std::cout << "} with maximum edge( " << circuits.back( ).max_edge.rear << " -> " << circuits.back( ).max_edge.front << " )" << std::endl;
#endif //_DEBUG_GRAPH

			// 閉路が根ならば重みが最大でないエッジを，根でないならば閉路外からのエッジが接続する頂点を終点としないエッジを追加する
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
				
			// 収縮した閉路を元に戻す
			for ( Digraph::size_type i = 0; i < digraph.size1( ); i++ )
			{
				if ( current_indices[ i ].back( ) == current_indices[ circuits.back( ).edges.front( ).front ].back( ) )
				{
					current_indices[ i ].pop_back( );
				}
			}
		}

		// 結果を行列に出力する
		for ( std::list< Edge >::iterator iter = edges.begin( ); iter != edges.end( ); iter++ )
		{
			arborescence( iter->rear, iter->front ) = digraph( iter->rear, iter->front );
		}
	}
}

#endif //_TMATSU_GRAPH_H_