/**
* @file artery.h
* @brief 動脈名対応付けのヘッダファイル
* @version 3.0
* @author 名古屋大学 情報科学研究科 メディア科学専攻 森研究室   351203192   松﨑 哲朗
* @date 2013/10/18
*/

#ifndef _TMATSU_ARTERY_H_
#define _TMATSU_ARTERY_H_

#include <vector>
#include <mist/machine_learning.h>
#include "vessel.h"
#include <algorithm>

namespace tmatsu
{
	namespace artery
	{
		/// @brief 動脈名を表わす列挙型
		enum Name 
		{
			NAME_OTHER, ///< @brief その他
			NAME_AO, ///< @brief 腹大動脈
			NAME_CA, ///< @brief 腹腔動脈
			NAME_CHA, ///< @brief 総肝動脈
			NAME_PHA, ///< @brief 固有肝動脈
			NAME_LHA, ///< @brief 左肝動脈
			NAME_RHA, ///< @brief 右肝動脈
			NAME_GDA, ///< @brief 胃十二指腸動脈
			NAME_LGA, ///< @brief 左胃動脈
			NAME_RGA, ///< @brief 右胃動脈
			NAME_LGEA, ///< @brief 左胃大網動脈
			NAME_RGEA, ///< @brief 右胃大網動脈
			NAME_SA, ///< @brief 脾動脈
			NAME_SMA, ///< @brief 上腸間膜動脈
#ifdef _TARGET_HYPOGASTRIC
			NAME_IMA, ///< @brief 下腸間膜動脈
			NAME_LRA, ///< @brief 左腎動脈
			NAME_RRA, ///< @brief 右腎動脈
			NAME_LCIA, ///< @brief 左総腸骨動脈
			NAME_RCIA, ///< @brief 右総腸骨動脈
			NAME_LIIA, ///< @brief 左内腸骨動脈
			NAME_RIIA, ///< @brief 右内腸骨動脈
			NAME_LEIA, ///< @brief 左外腸骨動脈
			NAME_REIA, ///< @brief 右外腸骨動脈
#endif //_TARGET_HYPOGASTRIC
			NAME_END, ///< @brief 動脈名の末尾の次
			NAME_BEGIN = NAME_OTHER + 1, ///< @brief 動脈名の先頭
			NAME_NUM = NAME_END - 1 ///< @brief 動脈名の数
		};
	
		using vessel::CTImage;
		using vessel::MarkImage;
		using vessel::ThicknessImage;
		using vessel::LabelImage;
		using vessel::FeatureVector;
		using vessel::Likelihood;

		/// @brief 尤度関数を扱うクラス
		class LikelihoodFunction
		{
			vessel::Classifier classifiers_[ NAME_NUM ]; ///< @brief 識別器
		public:
			template < class Container >
			void learn( const Container &samples );
			bool read( const char *path_name );
			bool write( const char *path_name )const;
			Likelihood< Name > operator( )( Name parent, const FeatureVector &feature );
		private:
			static std::string file_name( const std::string &path_name, Name artery_name );
		};

		/// @brief 動脈木構造を扱うクラス
		class Tree : public vessel::Tree< Name >
		{
			bool exist_cia; ///< @brief CIAが存在するかどうか
		protected:
			virtual Point root_point( const ThicknessImage &centerline ) const;
			virtual Point root_point(const ThicknessImage &centerline, Point seedPoint) const;
			virtual const_iterator reference_branch( )const;
			virtual Name reject_name( )const;
			virtual Likelihood< Name > default_likelihood( )const;
			virtual size_t name_number( )const;
			virtual std::string to_string( const Name &name )const;
			virtual size_t to_index( const Name &name )const;
			virtual Name to_name( const std::string &str )const;
			virtual void construct_postprocess( );
			virtual void labeling_preprocess( );
			virtual void labeling_postprocess( );
			virtual unsigned char to_label( Name name )const;
		private:
			std::pair< iterator, iterator > find_eia_end( );
		};
		
		std::string to_string( Name name );
		size_t to_index( Name name );
		Name to_name( const std::string &str );
		Name to_name( size_t idx );

		/// @brief 最も上にある端点を取得する
		/// @param[in] centerline 芯線画像
		/// @return 最も上にある端点の座標
		Point Tree::root_point( const ThicknessImage &centerline )const
		{
			for ( size_t k = 0; k < centerline.depth( ); k++ )
			{
				for ( size_t j = 0; j < centerline.height( ); j++ )
				{
					for ( size_t i = 0; i < centerline.width( ); i++ )
					{
						try
						{
							if ( centerline( i, j, k ) )
							{
								size_t cnt = 0; // 隣接画素の数
							
								for ( size_t l = 0; l < TOPOLOGY; l++ )
								{
									try
									{
										if ( centerline( Point( i, j, k ) + TOPOLOGICAL( neighborhood )[ l ] ) )
										{
											cnt++;
										}
									}

									catch ( ThicknessImage::InvalidPoint )
									{
									}
								}

								if ( cnt == 1 )
								{
									return Point( i, j, k );
								}
							}
						}

						catch ( ThicknessImage::InvalidPoint )
						{
						}
					}
				}
			}
	
			return Point( SIZE_MAX, SIZE_MAX, SIZE_MAX );
		}

		/// @brief the closest point of the seed point is token as root.
		Point Tree::root_point( const ThicknessImage &centerline, Point seedPoint ) const
		{
			std::vector<Point> candidateRoots;
			for (size_t k = 0; k < centerline.depth(); k++)
			{
				for (size_t j = 0; j < centerline.height(); j++)
				{
					for (size_t i = 0; i < centerline.width(); i++)
					{
						try
						{
							if (centerline(i, j, k))
							{
								size_t cnt = 0;

								for (size_t l = 0; l < TOPOLOGY; l++)
								{
									if (centerline(Point(i, j, k) + TOPOLOGICAL(neighborhood)[l]))
									{
										cnt++;
									}
								}

								if (cnt == 1)
								{
									candidateRoots.push_back(Point(i, j, k));
								}
							}
						}
						catch (ThicknessImage::InvalidPoint)
						{
						}
					}
				}
			}

			if (candidateRoots.empty())
				return Point(-1, -1, -1);
			
			std::vector<double> distance(candidateRoots.size());
			for (int i = 0; i < candidateRoots.size(); ++i)
			{
				auto offset = seedPoint - candidateRoots[i];
				auto dist = std::sqrt(offset.x*offset.x + offset.y*offset.y + offset.z*offset.z);
				distance[i] = dist;
			}

			auto minmax= minmax_element(distance.begin(), distance.end());
			auto pos = std::distance(distance.begin(), minmax.first);

			return candidateRoots[abs(pos)];
		}
		
		/// @brief 基準枝を取得する
		/// @return 基準枝の定数参照
		inline Tree::const_iterator Tree::reference_branch( )const
		{	
			return preorder_begin( );
		}

		/// @brief リジェクト名を取得する
		/// @return リジェクト名
		inline Name Tree::reject_name( )const
		{
			return NAME_OTHER;
		} 

		/// @brief 根の親枝の尤度として扱われる尤度の規定値を取得する
		/// @return 尤度の規定値
		inline Likelihood< Name > Tree::default_likelihood( )const
		{
			Likelihood< Name > likelihood; //尤度

			likelihood.insert( likelihood.end( ), Likelihood< Name >::value_type( NAME_AO, 1 ) );
			
			return likelihood;
		}
		
		/// @brief リジェクト名を含む名前の総数を取得する
		/// @return 名前の総数
		inline size_t Tree::name_number( )const
		{
			return NAME_NUM + 1;
		}

		/// @brief 名前を文字列に変換する
		/// @param[in] name 変換する名前
		/// @return 名前を表す文字列
		inline std::string Tree::to_string( const Name &name )const
		{
			return artery::to_string( name );
		}

		/// @brief 名前をインデックスに変換する
		/// @param[in] name 変換する名前
		/// @return 名前に対応するインデックス
		inline size_t Tree::to_index( const Name &name )const
		{
			return artery::to_index( name );
		}
	
		/// @brief 文字列を名前に変換する
		/// @param[in] str 変換する文字列
		/// @return 文字列の表す名前
		inline Name Tree::to_name( const std::string &str )const
		{
			return artery::to_name( str );
		}
	
		/// @brief 名前をラベル値に変換する
		/// @param[in] name 変換する名前
		/// @return 名前に対応するラベル値
		inline unsigned char Tree::to_label( Name name )const
		{
			switch ( name )
			{
			case NAME_AO:
				return vessel::LABEL_AO;
			case NAME_CA:
				return vessel::LABEL_C;
			case NAME_CHA:
				return vessel::LABEL_CH;
			case NAME_PHA:
				return vessel::LABEL_PH;
			case NAME_LHA:
				return vessel::LABEL_LH;
			case NAME_RHA:
				return vessel::LABEL_RH;
			case NAME_GDA:
				return vessel::LABEL_GD;
			case NAME_LGA:
				return vessel::LABEL_LG;
			case NAME_RGA:
				return vessel::LABEL_RG;
			case NAME_LGEA:
				return vessel::LABEL_LGE;
			case NAME_RGEA:
				return vessel::LABEL_RGE;
			case NAME_SA:
				return vessel::LABEL_S;
			case NAME_SMA:
				return vessel::LABEL_SM;
#ifdef _TARGET_HYPOGASTRIC
			case NAME_IMA:
				return vessel::LABEL_IM;
			case NAME_LRA:
				return vessel::LABEL_LR;
			case NAME_RRA:
				return vessel::LABEL_RR;
			case NAME_LCIA:
				return vessel::LABEL_LCI;
			case NAME_RCIA:
				return vessel::LABEL_RCI;
			case NAME_LEIA:
				return vessel::LABEL_LEI;
			case NAME_REIA:
				return vessel::LABEL_REI;
			case NAME_LIIA:
				return vessel::LABEL_LII;
			case NAME_RIIA:
				return vessel::LABEL_RII;
#endif //_TARGET_HYPOGASTRIC
			default:
				return vessel::LABEL_OTHER;
			}
		}
			
		/// @brief 長さが短い末端の枝を削除し，兄弟枝のない枝を親枝と統合する
		inline void Tree::construct_postprocess( )
		{	
			//double avg = 0, var = 0; // 統計量
			//double th = 0; // 閾値
			//size_t cnt = 0; // 葉の数

			//// 閾値を算出する
			//for ( iterator iter = begin( ); iter != end( ); iter++ )
			//{
			//	if ( is_leaf( iter ) )
			//	{
			//		double len = length( iter ); // 長さ

			//		avg += len;
			//		var += std::pow( len, 2 );
			//		cnt++;
			//	}
			//}

			//avg /= cnt;
			//var = var / cnt - std::pow( avg, 2 );
			//th = avg;

			// 枝刈りを行う
			for ( iterator iter = begin( ); iter != end( ); )
			{
				if ( is_leaf( iter ) && ( iter->value.path.size( ) <= PRUN_THRESHOLD ) )
				{
					iter = erase( iter );
				}
				else
				{
					iter++;
				}
			}

			// 枝を統合する
			for ( iterator iter = begin( ); iter != end( ); )
			{
				if ( !has_sibling( iter ) && !is_root( iter ) )
				{
					iterator parent_branch = parent( iter ); // 親枝
					Shift shift = iter->value.start_pt - parent_branch->value.end_pt; // 親枝の終点から始点へのベクトル

					// 終点を更新する
					parent_branch->value.end_pt = iter->value.end_pt;

					// 経路を更新する
					for ( size_t i = 0; i < TOPOLOGY; i++ )
					{
						if ( TOPOLOGICAL( neighborhood )[ i ] == shift )
						{
//							parent_branch->value.path.insert( iter->value.path.end( ), i );
							parent_branch->value.path.insert(parent_branch->value.path.end(), i);
							break;
						}
					}

					parent_branch->value.path.splice( parent_branch->value.path.end( ), iter->value.path );

					// 子枝を削除する
					iter = erase( iter );
				}
				else
				{
					iter++;
				}
			}
		}
		
		/// @brief 左総腸骨動脈，右総腸骨動脈および腹部大動脈をルールベースで対応付ける
		inline void Tree::labeling_preprocess( )
		{
			std::pair< iterator, iterator > eia_end_branches = find_eia_end( ); // 外腸骨動脈の末端枝
			std::pair< iterator, iterator > eia_start_branches; // 総腸骨動脈の開始枝

			// 右総腸骨動脈を対応付ける
			for ( iterator iter = eia_end_branches.first; iter != end( ); iter = parent( iter ) )
			{
				iter->name = NAME_RCIA;
			}

			// 左総腸骨動脈および腹部大動脈を対応付ける
			for ( iterator iter = eia_end_branches.second; iter != end( ); iter = parent( iter ) )
			{
				if ( iter->name == NAME_RCIA )
				{
					iter->name = NAME_AO;
				}
				else
				{
					iter->name = NAME_LCIA;
				}
			}

			// 総腸骨動脈の開始枝を探索する
			eia_start_branches.first = std::find( begin( ), end( ), NAME_RCIA );
			eia_start_branches.second = std::find( begin( ), end( ), NAME_LCIA );

			if ( eia_start_branches.first->value.thk < eia_start_branches.second->value.thk )
			{
				std::swap( eia_start_branches.first, eia_start_branches.second );
				std::swap( eia_end_branches.first, eia_end_branches.second );
			}
			
			// 総腸骨動脈が存在しないならば名前を付けなおす
			if ( eia_start_branches.second->value.thk <= 0 || eia_start_branches.first->value.thk / eia_start_branches.second->value.thk > THICKNESS_THRESHOLD )
			{
				exist_cia = false;

				// 太い血管を腹部大動脈に塗り替える
				for ( iterator iter = eia_end_branches.first; iter != end( ) && iter->name != NAME_AO; iter = parent( iter ) )
				{
					iter->name = NAME_AO;
				}

				// 細い血管を削除する
				for ( iterator iter = eia_end_branches.second; iter != end( ) && iter->name != NAME_AO; iter = parent( iter ) )
				{
					iter->name = NAME_OTHER;
				}
			}
			else
			{
				exist_cia = true;
			}

			//exist_cia = true;

#if 0
			for ( const_iterator iter = ++preorder_begin( ); iter != preorder_end( ); iter++ )
			{
				if ( iter->name != NAME_AO && parent( iter )->name == NAME_AO )
				{
					std::cout << bifurcation_num( iter ) + 1 << std::endl;
				}
			}

			std::cin.get( );
#endif
		}
		
		/// @brief 左外腸骨動脈および右外腸骨動脈をルールベースで対応付け，余分な固有肝動脈があればそれを除去する
		inline void Tree::labeling_postprocess( )
		{
			iterator leia_end = end( ), reia_end = end( ); // 外腸骨動脈の末端枝
			iterator leia_start = end( ), reia_start = end( ); // 外腸骨動脈の開始枝
			iterator pha_start = end( ), pha_end = end( ), lha_end = end( ), rha_end = end( ); // 肝動脈

			if ( exist_cia )
			{
				// 仮の総腸骨動脈の末端枝を探索する
				for ( iterator iter = postorder_begin( ); iter != postorder_end( ); iter++ )
				{
					if ( iter->name == NAME_LCIA && is_leaf( iter ) )
					{
						leia_end = iter;
					}
					else if ( iter->name == NAME_RCIA && is_leaf( iter ) )
					{
						reia_end = iter;
					}
				}

				// 内腸骨動脈の開始枝を探索する
				for ( iterator iter = leia_end; iter != end( ) && iter->name != NAME_AO && leia_start == end( ); iter = parent( iter ) )
				{
					for ( iterator sibling_iter = child_begin( parent( iter ) ); sibling_iter != child_end( parent( iter ) ); sibling_iter++ )
					{
						if ( sibling_iter->name == NAME_LIIA )
						{
							leia_start = iter;
							break;
						}
					}
				}
				
				for ( iterator iter = reia_end; iter != end( ) && iter->name != NAME_AO && reia_start == end( ); iter = parent( iter ) )
				{
					for ( iterator sibling_iter = child_begin( parent( iter ) ); sibling_iter != child_end( parent( iter ) ); sibling_iter++ )
					{
						if ( sibling_iter->name == NAME_RIIA )
						{
							reia_start = iter;
							break;
						}
					}
				}

				// 仮の総腸骨動脈の名前を付け直す
				if ( leia_start != end( ) )
				{
					for ( iterator iter = leia_end; iter != parent( leia_start ); iter = parent( iter ) )
					{
						iter->name = NAME_LEIA;
					}
				}

				if ( reia_start != end( ) )
				{
					for ( iterator iter = reia_end; iter != parent( reia_start ); iter = parent( iter ) )
					{
						iter->name = NAME_REIA;
					}
				}
			}
			else
			{
				// 腸骨動脈を削除する
				for ( iterator iter = begin( ); iter != end( ); iter++ )
				{
					switch ( iter->name )
					{
					case NAME_LCIA:
					case NAME_RCIA:
					case NAME_LIIA:
					case NAME_RIIA:
						iter->name = NAME_OTHER;
					}
				}
			}

			// 固有肝動脈を探索する
			for ( iterator iter = preorder_begin( ); iter != preorder_end( ); iter++ )
			{
				if ( iter->name == NAME_PHA )
				{
					pha_start = iter;
					break;
				}
			}

			// 肝動脈を探索する
			for ( iterator iter = subtree( pha_start ).postorder_begin( ); iter != subtree( pha_start ).postorder_end( ); iter++ )
			{
				if ( iter->name == NAME_PHA )
				{
					pha_end = pha_end == end( ) ? iter : pha_end;
				}
				else if ( iter->name == NAME_LHA )
				{
					lha_end = lha_end == end( ) ? iter : lha_end;
				} 
				else if ( iter->name == NAME_RHA )
				{
					rha_end = rha_end == end( ) ? iter : rha_end;
				}
			}

			// 固有肝動脈を除去する
			for ( iterator iter = pha_end; iter != parent( pha_start ); iter = parent( iter ) )
			{
				if ( lha_end == end( ) && rha_end == end( ) )
				{
					iter->name = NAME_OTHER;
				}
				else if ( lha_end != end( ) && rha_end == end( ) )
				{
					iter->name = NAME_LHA;
				}
				else if ( lha_end == end( ) && rha_end != end( ) )
				{
					iter->name = NAME_RHA;
				}
			}
		}
		
		/// @brief 外腸骨動脈の末端枝の組を取得する
		/// @return 外腸骨動脈の末端枝の組．firstが右外腸骨動脈，secondが左外腸骨動脈
		inline std::pair< Tree::iterator, Tree::iterator > Tree::find_eia_end( )
		{
			std::pair< iterator, iterator > eia_end_branches = std::make_pair( end( ), end( ) );

			for ( iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( is_leaf( iter ) )
				{
					//if ( iter->value.end_pt.x <= get_size( ).x / 2 && ( eia_end_branches.first == end( ) || iter->value.end_pt.z > eia_end_branches.first->value.end_pt.z || ( iter->value.end_pt.z == eia_end_branches.first->value.end_pt.z && iter->value.thk > eia_end_branches.first->value.thk ) ) )
					//{
					//	eia_end_branches.first = iter;
					//}
					//else if ( iter->value.end_pt.x >= get_size( ).x / 2 && ( eia_end_branches.second == end( ) || iter->value.end_pt.z > eia_end_branches.second->value.end_pt.z || ( iter->value.end_pt.z == eia_end_branches.second->value.end_pt.z && iter->value.thk > eia_end_branches.second->value.thk ) ) )
					//{
					//	eia_end_branches.second = iter;
					//}
					
					if ( iter->value.end_pt.x <= get_size( ).x / 2 && iter->value.end_pt.z >= BOTTOM_THRESHOLD( get_size( ).z ) && ( eia_end_branches.first == end( ) || iter->value.thk > eia_end_branches.first->value.thk ) )
					{
						eia_end_branches.first = iter;
					}
					else if ( iter->value.end_pt.x >= get_size( ).x / 2 && iter->value.end_pt.z >= BOTTOM_THRESHOLD( get_size( ).z ) && ( eia_end_branches.second == end( ) || iter->value.thk > eia_end_branches.second->value.thk ) )
					{
						eia_end_branches.second = iter;
					}		
				}
			}

			return eia_end_branches;
		}

		/// @brief 学習を行う
		/// @param[in] samples 教師データ．STLコンテナとみなせなければならない
		template < class Container >
		inline void LikelihoodFunction::learn( const Container &samples )
		{
			for ( Name n = NAME_BEGIN; n < NAME_END; n = static_cast< Name >( n + 1 ) )
			{
				std::vector< mist::machine_learning::feature > features; // 教師データ
				size_t cnt = 0; // 教師データの数

				// 教師データの数を求める
				for ( Container::const_iterator iter = samples.begin( ); iter != samples.end( ); iter++ )
				{
					Name parent_name = to_name( iter->parent_category ); // 親枝の名前

					// データ数を数える
					if ( parent_name == n && parent_name != NAME_LEIA && parent_name != NAME_REIA )
					{
						cnt++;
					}
				}

				// 教師データを作成する
				features.reserve( cnt );
					
				for ( Container::const_iterator iter = samples.begin( ); iter != samples.end( ); iter++ )
				{
					Container::value_type sample = *iter; // 教師データ

					// 外腸骨動脈をその他で置き換える
					switch ( to_name( sample.parent_category ) )
					{
					case NAME_LEIA:
					case NAME_REIA:
						sample.parent_category = to_string( NAME_OTHER );
					}

					// 教師データを挿入する
					if ( to_name( sample.parent_category ) == n )
					{
						switch ( to_name( sample.category ) )
						{
						case NAME_LEIA:
						case NAME_REIA:
							sample.category = to_string( NAME_OTHER );
						}

						features.push_back( sample );
					}
				}

				// 学習を行う
				classifiers_[ to_index( n ) ].learn( features, LEARN_ITERATION );
			}
		}
		
		/// @brief ファイル群を読み込む
		/// @param[in] path_name 読み込むファイル群のあるパス名
		/// @retval true 読み込みに成功した
		/// @retval false 読み込みに失敗した
		inline bool LikelihoodFunction::read( const char *path_name )
		{
			for ( Name n = NAME_BEGIN; n < NAME_END; n = static_cast< Name >( n + 1 ) )
			{
				if ( !classifiers_[ to_index( n ) ].load( file_name( path_name, n ) ) )
				{
					return false;
				}
			}

			return true;
		}
	
		/// @brief ファイル群へ書き込む
		/// @param[in] path_name 書き込むファイル群のあるパス名
		/// @retval true 書き込みに成功した
		/// @retval false 書き込みに失敗した
		inline bool LikelihoodFunction::write( const char *path_name )const
		{
			for ( Name n = NAME_BEGIN; n < NAME_END; n = static_cast< Name >( n + 1 ) )
			{
				if ( !classifiers_[ to_index( n ) ].save( file_name( path_name, n ) ) )
				{
					return false;
				}
			}

			return true;
		}
	
		/// @brief 尤度の組を取得する
		/// @param[in] parent 親枝に付けられていると仮定する血管名
		/// @param[in] feature 未知データ
		/// @return 尤度の組
		inline Likelihood< Name > LikelihoodFunction::operator( )( Name parent, const FeatureVector &feature )
		{
			std::vector< mist::machine_learning::pair< double, std::string > > ranks; // ランク
			Likelihood< Name > likelihoods; // 尤度

			switch ( parent )
			{
			case NAME_AO:
			case NAME_CA:
			case NAME_CHA:
			case NAME_PHA:
			case NAME_LHA:
			case NAME_RHA:
			case NAME_GDA:
			case NAME_LGA:
			case NAME_RGA:
			case NAME_LGEA:
			case NAME_RGEA:
			case NAME_SA:
			case NAME_SMA:
#ifdef _TARGET_HYPOGASTRIC
			case NAME_IMA:
			case NAME_LRA:
			case NAME_RRA:
			case NAME_LCIA:
			case NAME_RCIA:
			case NAME_LIIA:
			case NAME_RIIA:
			case NAME_LEIA:
			case NAME_REIA:
#endif //_TARGET_HYPOGASTRIC
				// ランクを計算する
				classifiers_[ to_index( parent ) ].compute_category_ranks( feature, ranks );

				// 尤度に変換する
				likelihoods.clear( );

				for ( std::vector< mist::machine_learning::pair< double, std::string > >::const_iterator iter = ranks.begin( ); iter != ranks.end( ); iter++ )
				{
					likelihoods.insert( likelihoods.end( ), Likelihood< Name >::value_type( to_name( iter->value ), iter->key ) );
				}

				break;
			}

			return likelihoods;
		}
		
		/// @brief 識別器ファイル名を取得する
		/// @param[in] path_namr パス名
		/// @param[in] vessel_name 親枝に付けられていると仮定する血管名
		/// @param ファイル名
		inline std::string LikelihoodFunction::file_name( const std::string &path_name, Name vessel_name )
		{
			return path_name + "\\classifier" + to_string( vessel_name ) + ".txt";
		}
		
		/// @brief 名前を文字列に変換する
		/// @param[in] name 変換する名前
		/// @return 名前を表す文字列
		std::string to_string( Name name )
		{
			switch ( name )
			{
			case NAME_AO:
				return "Ao";
			case NAME_CA:
				return "CA";
			case NAME_CHA:
				return "CHA";
			case NAME_PHA:
				return "PHA";
			case NAME_LHA:
				return "LHA";
			case NAME_RHA:
				return "RHA";
			case NAME_GDA:
				return "GDA";
			case NAME_LGA:
				return "LGA";
			case NAME_RGA:
				return "RGA";
			case NAME_LGEA:
				return "LGEA";
			case NAME_RGEA:
				return "RGEA";
			case NAME_SA:
				return "SA";
			case NAME_SMA:
				return "SMA";
#ifdef _TARGET_HYPOGASTRIC
			case NAME_IMA:
				return "IMA";
			case NAME_LRA:
				return "LRA";
			case NAME_RRA:
				return "RRA";
			case NAME_LCIA:
				return "LCIA";
			case NAME_RCIA:
				return "RCIA";
			case NAME_LIIA:
				return "LIIA";
			case NAME_RIIA:
				return "RIIA";
			case NAME_LEIA:
				return "LEIA";
			case NAME_REIA:
				return "REIA";
#endif //_TARGET_HYPOGASTRIC
			default:
				return "other";
			}
		}

		/// @brief 名前をインデックスに変換する
		/// @param[in] name 変換する名前
		/// @return 名前に対応するインデックス
		size_t to_index( Name name )
		{
			return name == NAME_OTHER ? NAME_END - 1 : name - 1;
		}
	
		/// @brief 文字列を名前に変換する
		/// @param[in] str 変換する文字列
		/// @return 文字列の表す名前
		Name to_name( const std::string &str )
		{
			if ( str == "Ao" )
			{
				return NAME_AO;
			}
			else if ( str == "CA" )
			{
				return NAME_CA;
			}
			else if ( str == "CHA" )
			{
				return NAME_CHA;
			}
			else if ( str == "PHA" )
			{
				return NAME_PHA;
			}
			else if ( str == "LHA" )
			{
				return NAME_LHA;
			}
			else if ( str == "RHA" )
			{
				return NAME_RHA;
			}
			else if ( str == "GDA" )
			{
				return NAME_GDA;
			}
			else if ( str == "LGA" )
			{
				return NAME_LGA;
			}
			else if ( str == "RGA" )
			{
				return NAME_RGA;
			}
			else if ( str == "LGEA" )
			{
				return NAME_LGEA;
			}	
			else if ( str == "RGEA" )
			{
				return NAME_RGEA;
			}
			else if ( str == "SA" )
			{
				return NAME_SA;
			}
			else if ( str == "SMA" )
			{
				return NAME_SMA;
			}
#ifdef _TARGET_HYPOGASTRIC
			else if ( str == "IMA" )
			{
				return NAME_IMA;
			}
			else if ( str == "LRA" )
			{
				return NAME_LRA;
			}
			else if ( str == "RRA" )
			{
				return NAME_RRA;
			}
			else if ( str == "LCIA" )
			{
				return NAME_LCIA;
			}
			else if ( str == "RCIA" )
			{
				return NAME_RCIA;
			}
			else if ( str == "LIIA" )
			{
				return NAME_LIIA;
			}
			else if ( str == "RIIA" )
			{
				return NAME_RIIA;
			}
			else if ( str == "LEIA" )
			{
				return NAME_LEIA;
			}
			else if ( str == "REIA" )
			{
				return NAME_REIA;
			}
#endif //_TARGET_HYPOGASTRIC
			else
			{
				return NAME_OTHER;
			} 
		}
	
		/// @brief インデックスを名前に変換する
		/// @param[in] idx 変換するインデックス
		/// @return インデックスの表す名前
		Name to_name( size_t idx )
		{
			return idx == NAME_END - 1 ? NAME_OTHER : static_cast< Name >( idx + 1 );
		}
	}
}

#endif //_TMATSU_ARTERY_H_