/**
* @file vessel.h
* @brief 血管名対応付けのヘッダファイル
* @version 3.0
* @author 名古屋大学 情報科学研究科 メディア科学専攻 森研究室   351203192   松﨑 哲朗
* @date 2013/10/18
*/

#ifndef _TMATSU_VESSEL_H_
#define _TMATSU_VESSEL_H_

#include <list>
#include <stack>
#include <map>
#include <cmath>
#include "tubular.h"

namespace tmatsu 
{
	namespace vessel
	{
		using tubular::CTImage;
		using tubular::MarkImage;
		using tubular::ThicknessImage;
		using tubular::LabelImage;

		/// @brief 特徴量を表す列挙型
		enum Feature
		{
			FEATURE_LENGTH, ///< @brief 長さ
			FEATURE_DISTANCE, ///< @brief 根からの距離
			//FEATURE_DISTANCE_BIFURCATION, ///< @brief 分岐点からの距離
			FEATURE_CHILD_DISTANCE, ///< @brief 子供の最長経路の距離
			FEATURE_SIBLING_DISTANCE, ///< @brief 兄弟の最長経路の距離
			FEATURE_CHILD_BIFURCATION, ///< @brief 子供の分岐数
			FEATURE_SIBLING_BIFURCATION, ///< @brief 兄弟の分岐数
			FEATURE_CURVATURE, ///< @brief 湾曲度
			FEATURE_THICKNESS, ///< @brief 太さ
			FEATURE_PARENT_THICKNESS, ///< @brief 親の太さ
			FEATURE_SIBLING_THICKNESS, ///< @brief 兄弟の太さの平均
			FEATURE_SIBLING_THICKNESS_SD, ///< @brief 兄弟の太さの標準偏差
			FEATURE_SIBLING_THICKNESS_MAX, ///< @brief 兄弟の太さの最大値
			FEATURE_SIBLING_THICKNESS_MIN, ///< @brief 兄弟の太さの最小値
			FEATURE_CHILD_THICKNESS, ///< @brief 子供の太さの平均
			FEATURE_CHILD_THICKNESS_SD, ///< @brief 子供の太さの標準偏差
			FEATURE_CHILD_THICKNESS_MAX, ///< @brief 子供の太さの最大値
			FEATURE_CHILD_THICKNESS_MIN, ///< @brief 子供の太さの最小値
			//FEATURE_ANGLE, ///< @brief 根との角度
			//FEATURE_PARENT_ANGLE, ///< @brief 親との角度
			//FEATURE_SIBLING_ANGLE, ///< @brief 兄弟との角度の平均
			//FEATURE_SIBLING_ANGLE_SD, ///< @brief 兄弟との角度の標準偏差
			//FEATURE_SIBLING_ANGLE_MAX, ///< @brief 兄弟との角度の最大値
			//FEATURE_SIBLING_ANGLE_MIN, ///< @brief 兄弟との角度の最小値
			//FEATURE_CHILD_ANGLE, ///< @brief 子供との角度の平均
			//FEATURE_CHILD_ANGLE_SD, ///< @brief 子供との角度の標準偏差
			//FEATURE_CHILD_ANGLE_MAX, ///< @brief 子供との角度の最大値
			//FEATURE_CHILD_ANGLE_MIN, ///< @brief 子供との角度の最小値
			FEATURE_STARTING_POS_X, ///< @brief 根からの始点位置のx成分
			FEATURE_STARTING_POS_Y, ///< @brief 根からの始点位置のy成分
			FEATURE_STARTING_POS_Z, ///< @brief 根からの始点位置のz成分
			FEATURE_ENDING_POS_X, ///< @brief 根からの終点位置のx成分
			FEATURE_ENDING_POS_Y, ///< @brief 根からの終点位置のy成分
			FEATURE_ENDING_POS_Z, ///< @brief 根からの終点位置のz成分
			//FEATURE_STARTING_POS_BRANCH_X, ///< @brief 分岐点からの始点位置のx成分
			//FEATURE_STARTING_POS_BRANCH_Y, ///< @brief 分岐点からの始点位置のy成分
			//FEATURE_STARTING_POS_BRANCH_Z, ///< @brief 分岐点からの始点位置のz成分
			//FEATURE_ENDING_POS_BRANCH_X, ///< @brief 分岐点からの終点位置のx成分
			//FEATURE_ENDING_POS_BRANCH_Y, ///< @brief 分岐点からの終点位置のy成分
			//FEATURE_ENDING_POS_BRANCH_Z, ///< @brief 分岐点からの終点位置のz成分
			FEATURE_LEAF_ENDING_POS_X, ///< @brief 子孫の末端枝の終点位置のx成分の平均
			FEATURE_LEAF_ENDING_POS_X_SD, ///< @brief 子孫の末端枝の終点位置のx成分の標準偏差
			FEATURE_LEAF_ENDING_POS_X_MAX, ///< @brief 子孫の末端枝の終点位置のx成分の最大値
			FEATURE_LEAF_ENDING_POS_X_MIN, ///< @brief 子孫の末端枝の終点位置のx成分の最小値
			FEATURE_LEAF_ENDING_POS_Y, ///< @brief 子孫の末端枝の終点位置のy成分の平均
			FEATURE_LEAF_ENDING_POS_Y_SD, ///< @brief 子孫の末端枝の終点位置のy成分の標準偏差
			FEATURE_LEAF_ENDING_POS_Y_MAX, ///< @brief 子孫の末端枝の終点位置のy成分の最大値
			FEATURE_LEAF_ENDING_POS_Y_MIN, ///< @brief 子孫の末端枝の終点位置のy成分の最小値
			FEATURE_LEAF_ENDING_POS_Z, ///< @brief 子孫の末端枝の終点位置のz成分の平均
			FEATURE_LEAF_ENDING_POS_Z_SD, ///< @brief 子孫の末端枝の終点位置のz成分の標準偏差
			FEATURE_LEAF_ENDING_POS_Z_MAX, ///< @brief 子孫の末端枝の終点位置のz成分の最大値
			FEATURE_LEAF_ENDING_POS_Z_MIN, ///< @brief 子孫の末端枝の終点位置のz成分の最小値
			FEATURE_RUNNING_DIR_X, ///< @brief 走行方向のx成分
			FEATURE_RUNNING_DIR_Y, ///< @brief 走行方向のy成分
			FEATURE_RUNNING_DIR_Z, ///< @brief 走行方向のz成分
			//FEATURE_PARENT_RUNNING_DIR_X, ///< @brief 親の走行方向のx成分
			//FEATURE_PARENT_RUNNING_DIR_Y, ///< @brief 親の走行方向のy成分
			//FEATURE_PARENT_RUNNING_DIR_Z, ///< @brief 親の走行方向のz成分
			//FEATURE_SIBLING_RUNNING_DIR_X, ///< @brief 兄弟の走行方向のx成分
			//FEATURE_SIBLING_RUNNING_DIR_Y, ///< @brief 兄弟の走行方向のy成分
			//FEATURE_SIBLING_RUNNING_DIR_Z, ///< @brief 兄弟の走行方向のz成分
			FEATURE_DIM ///< @brief 特徴ベクトルの次元
		};

		/// @brief ラベル値を表す列挙型
		enum Label 
		{
			LABEL_AO = 1, ///< @brief 腹部大動脈
			LABEL_P, ///< @brief 門脈
			LABEL_C, ///< @brief 腹腔動脈
			LABEL_CH, ///< @brief 総肝動脈
			LABEL_PH, ///< @brief 固有肝動脈
			LABEL_LH, ///< @brief 左肝動脈
			LABEL_RH, ///< @brief 右肝動脈
			LABEL_GD, ///< @brief 胃十二指腸動脈
			LABEL_LG, ///< @brief 左胃動静脈
			LABEL_RG, ///< @brief 右胃動静脈
			LABEL_LGE, ///< @brief 左胃大網動静脈
			LABEL_RGE, ///< @brief 右胃大網動静脈
			LABEL_S, ///< @brief 脾動静脈
			LABEL_GCT, ///< @brief 胃結腸静脈幹
			LABEL_SM, ///< @brief 上腸間膜動静脈
			LABEL_IM, ///< @brief 下腸間膜動静脈
			LABEL_LR, ///< @brief 左腎動脈
			LABEL_RR, ///< @brief 右腎動脈
			LABEL_LCI, ///< @brief 左総腸骨動脈
			LABEL_RCI, ///< @brief 右総腸骨動脈
			LABEL_LEI, ///< @brief 左外腸骨動脈
			LABEL_REI, ///< @brief 右外腸骨動脈
			LABEL_LII, ///< @brief 左内腸骨動脈
			LABEL_RII, ///< @brief 右内腸骨動脈
			LABEL_OTHER ///< @brief その他
		};

		/// @brief 特徴ベクトルを扱う型
		struct FeatureVector : public mist::machine_learning::feature
		{
			std::string parent_category; //親のカテゴリ
		};

		/// @brief 尤度を扱う型
		/// @tparam NameType 名前の型
		template < class NameType >
		struct Likelihood : public std::map< NameType, double >
		{
			Likelihood &operator+=( const Likelihood &l );
		};

		/// @brief 血管枝のデータを扱う型
		struct BranchValue
		{
			typedef std::list< size_t > PathType; ///< @brief 経路を表す型
			double thk; ///< @brief 太さ
			Point start_pt; ///< @brief 始点
			Point end_pt; ///< @brief 終点
			PathType path; ///< @brief 経路
		};

		/// @brief 弱識別器を扱うクラス
		class WeakClassifier : public mist::machine_learning::adaboost::threshold_classifier
		{
		public:
			/// @brief 重み調整を行った上で学習を行う
			/// @param[in] features 学習サンプルのリスト
			/// @param[in] categories カテゴリのリスト
			/// @retval true 学習に成功した
			/// @retval false 学習に失敗した
			template < template < typename, typename > class FEATURE_LIST, template < typename, typename > class CATEGORY_LIST, class Allocator1, class Allocator2 >
			bool learn( const FEATURE_LIST< feature_type, Allocator1 > &features, const CATEGORY_LIST< bool, Allocator2 > &categories )
			{
				FEATURE_LIST< feature_type, Allocator1 > temp = features;
				size_t pos = 0, neg = 0;

				for ( size_t i = 0; i < categories.size( ); i++ )
				{
					categories[ i ] ? pos++ : neg++;
				}
		
				if ( pos && neg )
				{
					for ( size_t i = 0; i < temp.size( ); i++ )
					{
						temp[ i ].weight /= categories[ i ] ? pos : neg;
					}
				}

				return mist::machine_learning::adaboost::threshold_classifier::learn( temp, categories );
			}
		};

		typedef mist::machine_learning::adaboost::classifier< WeakClassifier > Classifier; ///< @brief 強識別器を扱うクラス
	
		/// @brief 血管木構造を扱う抽象クラス
		/// @tparam name 名前の型
		template < class NameType >
		class Tree : public tubular::Tree< BranchValue, NameType, Likelihood< NameType >, FeatureVector >
		{
		public:
			bool write_tree_info( const char *file_name, const ThicknessImage &centerline )const;
			bool write_centerline_info( const char *file_name, const ThicknessImage &centerline )const;
			bool write_overlay_info( const char *file_name )const;
			bool write_secant_points( const char *file_name, double pos = 0.5 )const;
			template < class Iterator >
			double length( Iterator branch )const;
			template < class StartIterator, class EndIterator >
			double length( StartIterator start_branch, EndIterator end_branch )const;
			Point secant_point( NameType name, double pos = 0.5 )const;
		protected:
			/// @brief 名前をラベル値に変換する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @param[in] name 変換する名前
			/// @return 名前に対応するラベル値
			virtual unsigned char to_label( NameType name )const = 0;

			virtual Point traverse_branch( BranchValue &branch, ThicknessImage &centerline, const Point &start_pt )const;
			virtual void extract_feature( FeatureVector &feature, const_iterator branch, const_iterator ref_branch )const;
			virtual double evaluate_result( const_iterator result_branch, const_iterator truth_branch )const;
			virtual void restore_centerline( LabelImage &labeled_centerline, const_iterator branch )const;
			virtual void restore_bifurcation( const MarkImage &vessel, LabelImage &labeled_vessel, const_iterator branch )const;
			virtual bool read_branch( const XML &xml, XML::const_iterator elem, iterator branch );
			virtual bool write_branch( XML &xml, XML::iterator elem, const_iterator branch )const;
			template < class Iterator >
			double max_length( Iterator branch )const;
			template < class Iterator >
			size_t bifurcation_num( Iterator branch )const;
			template < class VectorType >
			Vector effective( const VectorType &vector )const;
		};
	
		/// @brief 江種形式の木構造情報をファイルに書き込む
		/// @param[in] file_name 書き込むXMLファイル名
		/// @param[in] centerline 木構造に対応する芯線画像
		/// @retval true 木構造情報の書き込みに成功した
		/// @retval false 木構造情報の書き込みに失敗した
		template < class NameType >
		inline bool Tree< NameType >::write_tree_info( const char *file_name, const ThicknessImage &centerline )const
		{
			XML xml; //書き込むXML
			std::set< NameType > names; // 書き込む血管名

			// 書き込む血管名を求める
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( iter->name != reject_name( ) )
				{
					names.insert( iter->name );
				}
			}
	
			// ルート要素を書き込む
			xml.insert( xml.end( ), XML::Element( "Tree" ) );
	
			// 各血管名を書き込む
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				const_iterator end_branch = std::find( postorder_begin( ), postorder_end( ), *name_iter ); // 終端枝
				std::stack< const_iterator > stack; // スタック

				// 枝をスタックにプッシュする
				for ( const_iterator iter = end_branch; iter->name == *name_iter; iter = parent( iter ) ) 
				{
					stack.push( iter );
						
					if ( is_root( iter ) )
					{
						break;
					}
				}
			
				// 各枝を書き込む
				XML::iterator name_elem = xml.insert( xml.preorder_begin( ), XML::Element( to_string( *name_iter ) ) ); //名前要素

				while ( !stack.empty( ) )
				{
					const_iterator branch = stack.top( ); // 書き込む枝
					Point pt = branch->value.start_pt; // 走査している座標
					XML::iterator branch_elem = xml.insert( name_elem, XML::Element( "Branch" ) ); // Branch要素

					stack.pop( );

					for ( BranchValue::PathType::const_iterator path_iter = branch->value.path.begin( ); ; path_iter++ )
					{
						XML::iterator point_elem = xml.insert( branch_elem, XML::Element( "Point" ) ); // Point要素
						XML::iterator x_elem; // X要素
						XML::iterator y_elem; // Y要素
						XML::iterator z_elem; // Z要素
						XML::iterator distance_elem; // Distance要素
					
						x_elem = xml.insert( point_elem, XML::Element( "X" ) );
						x_elem->insert( x_elem->end( ), std::to_string( pt.x ) );
						y_elem = xml.insert( xml.parent( x_elem ), XML::Element( "Y" ) );
						y_elem->insert( y_elem->end( ), std::to_string( pt.y ) );
						z_elem = xml.insert( xml.parent( y_elem ), XML::Element( "Z" ) );
						z_elem->insert( z_elem->end( ), std::to_string( pt.z ) );
						distance_elem = xml.insert( xml.parent( z_elem ), XML::Element( "Distance" ) );
						distance_elem->insert( distance_elem->end( ), std::to_string( static_cast< long double >( centerline( pt ) ) ) );
						branch_elem = xml.parent( xml.parent( distance_elem ) );

						if ( path_iter == branch->value.path.end( ) )
						{
							break;
						}

						pt += TOPOLOGICAL( neighborhood )[ *path_iter ];
					}

					name_elem = xml.parent( branch_elem );
				}
			}

			return xml.write( file_name );
		}
		
		/// @brief 江種形式の芯線情報をファイルに書き込む
		/// @param[in] file_name 書き込むXMLファイル名
		/// @param[in] centerline 木構造に対応する芯線画像
		/// @retval true 芯線情報の書き込みに成功した
		/// @retval false 芯線情報の書き込みに失敗した
		template < class NameType >
		inline bool Tree< NameType >::write_centerline_info( const char *file_name, const ThicknessImage &centerline )const
		{
			XML xml; //書き込むXML

			// ルート要素を書き込む
			xml.insert( xml.end( ), XML::Element( "Tree" ) );

			// 各枝を書き込む
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				XML::iterator branch_elem = xml.insert( xml.preorder_begin( ), XML::Element( "Branch" ) ); // Branch要素

				// Index要素を書き込む
				XML::iterator index_elem = xml.insert( branch_elem, XML::Element( "Index" ) ); // Index要素

				index_elem->insert( index_elem->end( ), std::to_string( iter->id - 1 ) );

				// BranchElement要素を書き込む
				XML::iterator branch_element_elem = xml.insert( xml.parent( index_elem ), XML::Element( "BranchElement" ) ); // BranchElement要素

				Point pt = iter->value.start_pt; // 走査している座標

				for ( BranchValue::PathType::const_iterator path_iter = iter->value.path.begin( ); ; path_iter++ )
				{
					XML::iterator point_and_distance_elem = xml.insert( branch_element_elem, XML::Element( "PointAndDistance" ) ); // PointAndDistance要素
					XML::iterator x_elem; // X要素
					XML::iterator y_elem; // Y要素
					XML::iterator z_elem; // Z要素
					XML::iterator distance_elem; // Distance要素
					
					x_elem = xml.insert( point_and_distance_elem, XML::Element( "X" ) );
					x_elem->insert( x_elem->end( ), std::to_string( pt.x ) );
					y_elem = xml.insert( xml.parent( x_elem ), XML::Element( "Y" ) );
					y_elem->insert( y_elem->end( ), std::to_string( pt.y ) );
					z_elem = xml.insert( xml.parent( y_elem ), XML::Element( "Z" ) );
					z_elem->insert( z_elem->end( ), std::to_string( pt.z ) );
					distance_elem = xml.insert( xml.parent( z_elem ), XML::Element( "Distance" ) );
					distance_elem->insert( distance_elem->end( ), std::to_string( static_cast< long double >( centerline( pt ) ) ) );
					branch_element_elem = xml.parent( xml.parent( distance_elem ) );

					if ( path_iter == iter->value.path.end( ) )
					{
						break;
					}
					
					pt += TOPOLOGICAL( neighborhood )[ *path_iter ];
				}

				// Length要素を書き込む
				XML::iterator length_elem = xml.insert( xml.parent( branch_element_elem ), XML::Element( "Length" ) ); // Length要素

				length_elem->insert( length_elem->end( ), std::to_string( static_cast< long double >( length( iter ) ) ) );

				// Thickness要素を書き込む
				XML::iterator thickness_elem = xml.insert( xml.parent( length_elem ), XML::Element( "Thickness" ) ); // Thickness要素

				thickness_elem->insert( thickness_elem->end( ), std::to_string( static_cast< long double >( iter->value.thk ) ) );

				// ParentIndex要素を書き込む
				XML::iterator parent_index_elem = xml.insert( xml.parent( thickness_elem ), XML::Element( "ParentIndex" ) ); // ParentIndex要素

				if ( !is_root( iter ) )
				{
					parent_index_elem->insert( parent_index_elem->end( ), std::to_string( parent( iter )->id - 1 ) );
				}

				// ChildIndexList要素を書き込む
				XML::iterator child_index_list_elem = xml.insert( xml.parent( parent_index_elem ), XML::Element( "ChildIndexList" ) ); // ChildIndexList要素

				for ( const_iterator child_iter = child_begin( iter ); child_iter != child_end( iter ); child_iter++ )
				{
					XML::iterator child_index_elem = xml.insert( child_index_list_elem, XML::Element( "ChildIndex" ) ); // ChildIndex要素

					child_index_elem->insert( child_index_elem->end( ), std::to_string( child_iter->id - 1 ) );
					child_index_list_elem = xml.parent( child_index_elem );
				}

				// Vector要素を書き込む
				XML::iterator vector_elem = xml.insert( xml.parent( child_index_list_elem ), XML::Element( "Vector" ) ); // Vector要素
				XML::iterator x_elem; // X要素
				XML::iterator y_elem; // Y要素
				XML::iterator z_elem; // Z要素

				x_elem = xml.insert( vector_elem, XML::Element( "X" ) );
				x_elem->insert( x_elem->end( ), std::to_string( static_cast< ptrdiff_t >( iter->value.end_pt.x - iter->value.start_pt.x ) ) );
				y_elem = xml.insert( xml.parent( x_elem ), XML::Element( "Y" ) );
				y_elem->insert( y_elem->end( ), std::to_string( static_cast< ptrdiff_t >( iter->value.end_pt.y - iter->value.start_pt.y ) ) );
				z_elem = xml.insert( xml.parent( y_elem ), XML::Element( "Z" ) );
				z_elem->insert( z_elem->end( ), std::to_string( static_cast< ptrdiff_t >( iter->value.end_pt.z - iter->value.start_pt.z ) ) );
			}

			return xml.write( file_name );
		}
		
		/// @brief 江種形式のオーバレイ情報をファイルに書き込む
		/// @param[in] file_name 書き込むXMLファイル名
		/// @retval true オーバレイ情報の書き込みに成功した
		/// @retval false オーバレイ情報の書き込みに失敗した
		template < class NameType >
		inline bool Tree< NameType >::write_overlay_info( const char *file_name )const
		{
			XML xml; // 書き込むXML
			std::set< NameType > names; // 書き込む血管名

			// 書き込む血管名を求める
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( iter->name != reject_name( ) )
				{
					names.insert( iter->name );
				}
			}

			// ルート要素を書き込む
			xml.insert( xml.end( ), XML::Element( "OverlayInfo" ) );

			// 各血管名を書き込む
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				Point sec_pt = secant_point( *name_iter ); // 曲線中点

				// 書き込む
				XML::iterator name_elem = xml.insert( xml.preorder_begin( ), XML::Element( to_string( *name_iter ) ) ); // 名前要素
				XML::iterator label_elem; // Label要素
				XML::iterator position_elem; // Position要素
				XML::iterator x_elem; // X要素
				XML::iterator y_elem; // Y要素
				XML::iterator z_elem; // Z要素

				label_elem = xml.insert( name_elem, XML::Element( "Label" ) );
				label_elem->insert( label_elem->end( ), std::to_string( static_cast< ptrdiff_t >( to_label( *name_iter ) ) ) );
				position_elem = xml.insert( xml.parent( label_elem ), XML::Element( "Position" ) );
				x_elem = xml.insert( position_elem, XML::Element( "X" ) );
				x_elem->insert( x_elem->end( ), std::to_string( sec_pt.x ) );
				y_elem = xml.insert( xml.parent( x_elem ), XML::Element( "Y" ) );
				y_elem->insert( y_elem->end( ), std::to_string( sec_pt.y ) );
				z_elem = xml.insert( xml.parent( y_elem ), XML::Element( "Z" ) );
				z_elem->insert( z_elem->end( ), std::to_string( sec_pt.z ) );
			}

			return xml.write( file_name );
		}
			
		/// @brief NewVES形式で各血管の曲線中点をファイルに書き込む
		/// @attention 動作確認をしていないため，使用時は注意
		/// @param[in] file_name 書き込むCSVファイル名
		/// @param[in] pos 書き込む点の位置．[0, 1]の範囲で指定し，0なら始点，1なら終点．規定値は0.5．
		/// @retval true 各血管の曲線中点の書き込みに成功した
		/// @retval false 各血管の曲線中点の書き込みに失敗した
		template < class NameType >
		bool Tree< NameType >::write_secant_points( const char *file_name, double pos )const
		{
			std::ofstream ofs( file_name ); // 出力ファイル
			std::set< NameType > names; // 書き込む血管名

			// エラーチェック
			if ( !ofs )
			{
				return false;
			}

			//書き込む血管名を求める
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( iter->name != reject_name( ) )
				{
					names.insert( iter->name );
				}
			}

			// 各血管名を書き込む
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				Point sec_pt = secant_point( *name_iter, pos ); // 曲線中点

				ofs << to_string( *name_iter ) << ", " << sec_pt.x << ", " << sec_pt.y << ", " << sec_pt.z << std::endl;
			}

			return true;
		}
		
		/// @brief 枝の長さを取得する
		/// @param[in] branch 長さを取得する枝
		/// @return 枝の長さ
		template < class NameType >
		template < class Iterator >
		inline double Tree< NameType >::length( Iterator branch )const
		{
			double len = 0; // 長さ
			Shift shift( 0, 0, 0 ); // 変位

			for ( BranchValue::PathType::const_iterator iter = branch->value.path.begin( ); iter != branch->value.path.end( ); iter++ )
			{
				shift += TOPOLOGICAL( neighborhood )[ *iter ];
					
				if ( std::abs( shift.x ) > 1 || std::abs( shift.y ) > 1 || std::abs( shift.z ) > 1 )
				{	
					shift -= TOPOLOGICAL( neighborhood )[ *iter ];
					len += effective( shift ).length( );
					shift = TOPOLOGICAL( neighborhood )[ *iter ];
				}
			}

			return len + effective( shift ).length( );
		}
		
		/// @brief 経路の長さを取得する
		/// @attention end_branchはstart_branchの子でなければならない
		/// @param[in] start_branch 長さを取得する経路の開始枝の親
		/// @param[in] end_branch 長さを取得する経路の終端枝
		/// @return 経路の長さ
		template < class NameType >
		template < class StartIterator, class EndIterator >
		inline double Tree< NameType >::length( StartIterator start_branch, EndIterator end_branch )const
		{
			double len = 0; // 長さ

			for ( ; end_branch != end( ) && end_branch != start_branch; end_branch = parent( end_branch ) )
			{
				len += length( end_branch );
			}

			return len;
		}
			
		/// @brief 指定の血管の曲線中点を計算する
		/// @attention 血管が複数箇所にある場合，選考順で最初に発見された血管について計算する
		/// @param[in] name 曲線中点を計算する血管の名前
		/// @param[in] pos 書き込む点の位置．[0, 1]の範囲で指定し，0なら始点，1なら終点．規定値は0.5．
		/// @return 指定の血管の曲線中点
		template < class NameType >
		Point Tree< NameType >::secant_point( NameType name, double pos )const
		{
			const_iterator end_branch = std::find( postorder_begin( ), postorder_end( ), name ); // 終端枝
			size_t voxel_cnt = 0; // 画素数

			if ( end_branch != postorder_end( ) )
			{
				Point sec_pt = end_branch->value.end_pt; // 曲線中点

				// 画素数を求める
				for ( const_iterator iter = end_branch; iter->name == name; iter = parent( iter ) ) 
				{
					voxel_cnt += iter->value.path.size( ) + 1;

					if ( is_root( iter ) )
					{
						break;
					}
				}

				// 曲線中点を計算する
				pos = std::min( std::max( pos, 0.0 ), 1.0 );
				voxel_cnt = static_cast< size_t >( voxel_cnt * ( 1 - pos ) );

				for ( const_iterator iter = end_branch; ; iter = parent( iter ) ) 
				{
					if ( voxel_cnt < iter->value.path.size( ) + 1 ) 
					{
						sec_pt = iter->value.end_pt;

						for ( BranchValue::PathType::const_reverse_iterator path_iter = iter->value.path.rbegin( ); path_iter != iter->value.path.rend( ) && --voxel_cnt > 0; path_iter++ ) 
						{
							sec_pt -= TOPOLOGICAL( neighborhood )[ *path_iter ];
						}

						break;
					} 
					else 
					{
						voxel_cnt -= iter->value.path.size( ) + 1;
					}
				}

				return sec_pt;
			}
			else
			{
				return Point( 0, 0, 0 );
			}
		}
	
		/// @brief 芯線を走査し1本の枝のデータを取得する
		/// @param[out] val 取得するデータ
		/// @param[in] centerline 走査する芯線画像．この画像は書き換えてもよい
		/// @param[in] start_pt 走査の始点の座標
		/// @return 枝の終点の座標
		template < class NameType >
		inline Point Tree< NameType >::traverse_branch( BranchValue &val, ThicknessImage &centerline, const Point &start_pt )const
		{
			// 始点を設定する
			val.start_pt = val.end_pt = start_pt;
		
			// 各画素を走査する
			while ( true )
			{
				Point next_pt; // 次の座標
				size_t connect_num = 0; // 連結している画素数

				// 特徴量を更新
				val.thk += centerline( val.end_pt );

				// 走査済み画素を削除する
				centerline( val.end_pt ) = 0;

				// 座標を更新する
				for ( size_t i = 0; i < TOPOLOGY; i++ )
				{
					try
					{
						Point connect_pt = val.end_pt + TOPOLOGICAL( neighborhood )[ i ]; // 隣接している座標

						if ( centerline( connect_pt ) > 0 )
						{
							// 次の画素のとき
							val.path.insert( val.path.end( ), i );
							next_pt = connect_pt;
							connect_num++;
						}
					}

					catch ( ThicknessImage::InvalidPoint )
					{
					}
				}

				if ( connect_num == 1 )
				{
					val.end_pt = next_pt;
				}
				else
				{
					// 分岐点のとき隣接未走査画素を経路から削除する
					for ( size_t i = 0; i < connect_num; i++ )
					{
						val.path.pop_back( );
					}

					break;
				}
			} 

			// 太さを確定する
			val.thk /= val.path.size( ) + 1;

			return val.end_pt;
		}
		
		/// @brief 特徴量を抽出する
		/// @param[out] feature 抽出した特徴量を格納する領域
		/// @param[in] branch 特徴量を抽出する枝
		/// @param[in] ref_branch 基準枝
		template < class NameType >
		inline void Tree< NameType >::extract_feature( FeatureVector &feature, const_iterator branch, const_iterator ref_branch )const
		{
			Vector start_pos, end_pos, start_pos_bifurcation, end_pos_bifurcation, leaf_pos( 0, 0, 0 ), leaf_pos_var( 0, 0, 0 ), leaf_pos_max( -DBL_MAX, -DBL_MAX, -DBL_MAX ), leaf_pos_min( DBL_MAX, DBL_MAX, DBL_MAX ); // 位置ベクトル
			Vector run_dir, root_run_dir, parent_run_dir, sibling_run_dir; //走行方向ベクトル
			double curve = 0; // 湾曲度
			double sibling_dist = 0; // 距離
			double sibling_thk = 0, sibling_thk_var = 0, sibling_thk_max = 0, sibling_thk_min = DBL_MAX, child_thk = 0, child_thk_var = 0, child_thk_max = 0, child_thk_min = DBL_MAX; // 太さ
			//double parent_angle, sibling_angle = 0, sibling_angle_max = -M_PI / 2, sibling_angle_min = M_PI / 2, child_angle = 0, child_angle_max = -M_PI / 2, child_angle_min = M_PI / 2; // 角度
			size_t sibling_bifurcation = 0; // 分岐数
			Point pt = branch->value.start_pt; // 枝上の座標

			// 特徴量を初期化する
			feature.resize( FEATURE_DIM );
			feature.category = to_string( branch->name );
			feature.parent_category = is_root( branch ) ? to_string( reject_name( ) ) : to_string( parent( branch )->name );

			// ベクトルを計算する
			start_pos = effective( Vector( branch->value.start_pt ) - Vector( ref_branch->value.start_pt ) );
			end_pos = effective( Vector( branch->value.end_pt ) - Vector( ref_branch->value.start_pt ) );
			start_pos_bifurcation = effective( Vector( branch->value.start_pt ) - Vector( ref_branch->value.end_pt ) );
			end_pos_bifurcation = effective( Vector( branch->value.end_pt ) - Vector( ref_branch->value.end_pt ) );
			run_dir = effective( Vector( branch->value.end_pt ) - Vector( branch->value.start_pt ) ).unit( );
			root_run_dir;
			parent_run_dir = is_root( branch ) ? Vector( 0, 0, 0 ) : effective( Vector( parent( branch )->value.end_pt ) - Vector( parent( branch )->value.start_pt ) ).unit( );

			if ( !is_root( branch ) )
			{
				size_t sibling_cnt = 0; // 兄弟の数

				for ( const_iterator iter = child_begin( parent( branch ) ); iter != child_end( parent( branch ) ); iter++ )
				{
					if ( branch != iter )
					{
						sibling_run_dir += effective( Vector( iter->value.end_pt ) - Vector( iter->value.start_pt ) ).unit( );
						sibling_cnt++;
					}
				}

				sibling_run_dir /= sibling_cnt;
			}

			{
				size_t leaf_cnt = 0; // 末端枝の数

				for ( const_iterator iter = subtree( branch ).begin( ); iter != subtree( branch ).end( ); iter++ )
				{
					if ( is_leaf( iter ) )
					{
						Vector temp_pos = effective( Vector( iter->value.end_pt ) - Vector( ref_branch->value.start_pt ) ); // 末端枝の位置ベクトル

						// 末端枝の位置の平均を計算する
						leaf_pos += temp_pos;

						// 末端枝の位置の最大および最小を計算する
						leaf_pos_max.x = std::max( leaf_pos_max.x, temp_pos.x );
						leaf_pos_max.y = std::max( leaf_pos_max.y, temp_pos.y );
						leaf_pos_max.z = std::max( leaf_pos_max.z, temp_pos.z );
						leaf_pos_min.x = std::min( leaf_pos_min.x, temp_pos.x );
						leaf_pos_min.y = std::min( leaf_pos_min.y, temp_pos.y );
						leaf_pos_min.z = std::min( leaf_pos_min.z, temp_pos.z );

						// 末端枝の位置の分散を計算する
						temp_pos.x *= temp_pos.x;
						temp_pos.y *= temp_pos.y;
						temp_pos.z *= temp_pos.z;
						leaf_pos_var += temp_pos;

						leaf_cnt++;
					}
				}

				leaf_pos.x /= leaf_cnt;
				leaf_pos.y /= leaf_cnt;
				leaf_pos.z /= leaf_cnt;
				leaf_pos_var.x = leaf_pos_var.x / leaf_cnt - std::pow( leaf_pos.x, 2 );
				leaf_pos_var.y = leaf_pos_var.y / leaf_cnt - std::pow( leaf_pos.y, 2 );
				leaf_pos_var.z = leaf_pos_var.z / leaf_cnt - std::pow( leaf_pos.z, 2 );
			}

			// 湾曲度を計算する
			for ( BranchValue::PathType::const_iterator iter = branch->value.path.begin( ); iter != branch->value.path.end( ); iter++ ) 
			{
				// 座標を更新する
				pt += TOPOLOGICAL( neighborhood )[ *iter ];

				Vector pos = effective( Vector( pt ) - Vector( branch->value.start_pt ) ); // 画素の位置ベクトル

				// 位置ベクトルとその走行方向ベクトルへの正射影との差から法線ベクトルの長さを計算する
				curve += ( pos - ( pos.inner( run_dir ) * run_dir ) ).length( );
			}

			curve /= branch->value.path.size( ) + 1;

			// 太さを計算する
			if ( is_root( branch ) )
			{
				sibling_thk_max = sibling_thk_min = 0;
			}
			else
			{
				size_t sibling_cnt = 0; // 兄弟の数

				for ( const_iterator iter = child_begin( parent( branch ) ); iter != child_end( parent( branch ) ); iter++ )
				{
					if ( branch != iter )
					{
						sibling_thk += iter->value.thk;
						sibling_thk_var += std::pow( iter->value.thk, 2 );
						sibling_thk_max = std::max( sibling_thk_max, iter->value.thk / branch->value.thk );
						sibling_thk_min = std::min( sibling_thk_min, iter->value.thk / branch->value.thk );
						sibling_cnt++;
					}
				}
					
				sibling_thk /= sibling_cnt * branch->value.thk;
				sibling_thk_var = sibling_cnt <= 1 ? 0 : sibling_thk_var / ( sibling_cnt * std::pow( branch->value.thk, 2 ) ) - std::pow( sibling_thk, 2 );
			}

			if ( is_leaf( branch ) )
			{
				child_thk_max = child_thk_min = 0;
			}
			else
			{
				size_t child_cnt = 0; //子供の数
					
				for ( const_iterator iter = child_begin( branch ); iter != child_end( branch ); iter++ )
				{
					child_thk += iter->value.thk;
					child_thk_var += std::pow( iter->value.thk, 2 );
					child_thk_max = std::max( child_thk_max, iter->value.thk / branch->value.thk );
					child_thk_min = std::min( child_thk_min, iter->value.thk / branch->value.thk );
					child_cnt++;
				}

				child_thk /= child_cnt * branch->value.thk;
				child_thk_var = child_thk_var / ( child_cnt * std::pow( branch->value.thk, 2 ) ) - std::pow( child_thk, 2 );
			}

			// 角度を算出する
			//parent_angle = is_root( branch ) ? std::acos( run_dir.inner( effective( Vector( parent( branch )->value.end_pt ) - Vector( parent( branch )->value.start_pt ) ).unit( ) ) ) : 0;
			//
			//if ( !is_root( branch ) )
			//{
			//	for ( const_iterator iter = child_begin( parent( branch ) ); iter != child_end( parent( branch ) ); iter++ )
			//	{
			//		double angle = std::acos( run_dir.inner( effective( Vector( iter->value.end_pt ) - Vector( iter->value.start_pt ) ).unit( ) ) ); // 角度
			//		size_t sibling_cnt = 0; // 兄弟の数

			//		if ( branch != iter )
			//		{
			//			sibling_angle += angle;
			//			sibling_angle_max = std::max( sibling_angle_max, angle );
			//			sibling_angle_min = std::min( sibling_angle_min, angle );
			//			sibling_cnt++;
			//		}
			//	}

			//	sibling_angle /= sibling_cnt;
			//}
			//else
			//{
			//	sibling_angle_max = sibling_angle_min = 0;
			//}

			//if ( !is_leaf( branch ) )
			//{
			//	size_t child_cnt = 0; // 子供の数
	
			//	for ( const_iterator iter = child_begin( branch ); iter != child_end( branch ); iter++ )
			//	{
			//		double angle = std::acos( run_dir.inner( effective( Vector( iter->value.end_pt ) - Vector( iter->value.start_pt ) ).unit( ) ) ); // 角度

			//		child_angle += angle;
			//		child_angle_max = std::max( child_angle_max, angle );
			//		child_angle_min = std::min( child_angle_min, angle );
			//		child_cnt++;
			//	}

			//	child_angle /= child_cnt++;
			//}
			//else
			//{
			//	child_angle_max = child_angle_min = 0;
			//}

			// 距離を計算する
			if ( !is_root( branch ) )
			{
				for ( const_iterator iter = child_begin( parent( branch ) ); iter != child_end( parent( branch ) ); iter++ )
				{
					if ( branch != iter )
					{
						sibling_dist = std::max( sibling_dist, max_length( iter ) );
					}
				}
			}

			// 分岐数を計算する
			if ( !is_root( branch ) )
			{
				for ( const_iterator iter = child_begin( parent( branch ) ); iter != child_end( parent( branch ) ); iter++ )
				{
					if ( branch != iter )
					{
						sibling_bifurcation += bifurcation_num( iter );
					}
				}
			}

			// 特徴量を格納する
			feature[ FEATURE_LENGTH ] = length( branch );
			feature[ FEATURE_DISTANCE ] = length( parent( ref_branch ), parent( branch ) );
			//feature[ FEATURE_DISTANCE_BIFURCATION ] = 0;
			feature[ FEATURE_CHILD_DISTANCE ] = max_length( branch );
			feature[ FEATURE_SIBLING_DISTANCE ] = sibling_dist;
			feature[ FEATURE_CHILD_BIFURCATION ] = static_cast< double >( bifurcation_num( branch ) );
			feature[ FEATURE_SIBLING_BIFURCATION ] = static_cast< double >( sibling_bifurcation );
			feature[ FEATURE_CURVATURE ] = curve;
			feature[ FEATURE_THICKNESS ] = branch->value.thk / ref_branch->value.thk;
			feature[ FEATURE_PARENT_THICKNESS ] = is_root( branch ) ? 1 : parent( branch )->value.thk / branch->value.thk;
			feature[ FEATURE_SIBLING_THICKNESS ] = sibling_thk;
			feature[ FEATURE_SIBLING_THICKNESS_SD ] = std::sqrt( sibling_thk_var );
			feature[ FEATURE_SIBLING_THICKNESS_MAX ] = sibling_thk_max;
			feature[ FEATURE_SIBLING_THICKNESS_MIN ] = sibling_thk_min;
			feature[ FEATURE_CHILD_THICKNESS ] = child_thk;
			feature[ FEATURE_CHILD_THICKNESS_SD ] = std::sqrt( child_thk_var );
			feature[ FEATURE_CHILD_THICKNESS_MAX ] = child_thk_max;
			feature[ FEATURE_CHILD_THICKNESS_MIN ] = child_thk_min;
			//feature[ FEATURE_ANGLE ] = std::acos( run_dir.inner( root_run_dir ) );
			//feature[ FEATURE_PARENT_ANGLE ] = parent_angle;
			//feature[ FEATURE_SIBLING_ANGLE ] = sibling_angle;
			//feature[ FEATURE_SIBLING_ANGLE_SD ] = 0;
			//feature[ FEATURE_SIBLING_ANGLE_MAX ] = sibling_angle_max;
			//feature[ FEATURE_SIBLING_ANGLE_MIN ] = sibling_angle_min;
			//feature[ FEATURE_CHILD_ANGLE ] = child_angle;
			//feature[ FEATURE_CHILD_ANGLE_SD ] = 0;
			//feature[ FEATURE_CHILD_ANGLE_MAX ] = child_angle_max;
			//feature[ FEATURE_CHILD_ANGLE_MIN ] = child_angle_min;
			feature[ FEATURE_STARTING_POS_X ] = start_pos.x;
			feature[ FEATURE_STARTING_POS_Y ] = start_pos.y;
			feature[ FEATURE_STARTING_POS_Z ] = start_pos.z;
			feature[ FEATURE_ENDING_POS_X ] = end_pos.x;
			feature[ FEATURE_ENDING_POS_Y ] = end_pos.y;
			feature[ FEATURE_ENDING_POS_Z ] = end_pos.z;
			feature[ FEATURE_LEAF_ENDING_POS_X ] = leaf_pos.x;
			feature[ FEATURE_LEAF_ENDING_POS_X_SD ] = std::sqrt( leaf_pos_var.x );
			feature[ FEATURE_LEAF_ENDING_POS_X_MAX ] = leaf_pos_max.x;
			feature[ FEATURE_LEAF_ENDING_POS_X_MIN ] = leaf_pos_min.x;
			feature[ FEATURE_LEAF_ENDING_POS_Y ] = leaf_pos.y;
			feature[ FEATURE_LEAF_ENDING_POS_Y_SD ] = std::sqrt( leaf_pos_var.y );
			feature[ FEATURE_LEAF_ENDING_POS_Y_MAX ] = leaf_pos_max.y;
			feature[ FEATURE_LEAF_ENDING_POS_Y_MIN ] = leaf_pos_min.y;
			feature[ FEATURE_LEAF_ENDING_POS_Z ] = leaf_pos.z;
			feature[ FEATURE_LEAF_ENDING_POS_Z_SD ] = std::sqrt( leaf_pos_var.z );
			feature[ FEATURE_LEAF_ENDING_POS_Z_MAX ] = leaf_pos_max.z;
			feature[ FEATURE_LEAF_ENDING_POS_Z_MIN ] = leaf_pos_min.z;
			//feature[ FEATURE_STARTING_POS_BRANCH_X ] = start_pos_branch.x;
			//feature[ FEATURE_STARTING_POS_BRANCH_Y ] = start_pos_branch.y;
			//feature[ FEATURE_STARTING_POS_BRANCH_Z ] = start_pos_branch.z;
			//feature[ FEATURE_ENDING_POS_BRANCH_X ] = end_pos_branch.x;
			//feature[ FEATURE_ENDING_POS_BRANCH_Y ] = end_pos_branch.y;
			//feature[ FEATURE_ENDING_POS_BRANCH_Z ] = end_pos_branch.z;
			feature[ FEATURE_RUNNING_DIR_X ] = run_dir.x; // 以前と異なる値
			feature[ FEATURE_RUNNING_DIR_Y ] = run_dir.y; // 以前と異なる値
			feature[ FEATURE_RUNNING_DIR_Z ] = run_dir.z; // 以前と異なる値
			//feature[ FEATURE_PARENT_RUNNING_DIR_X ] = parent_run_dir.x;
			//feature[ FEATURE_PARENT_RUNNING_DIR_Y ] = parent_run_dir.y;
			//feature[ FEATURE_PARENT_RUNNING_DIR_Z ] = parent_run_dir.z;
			//feature[ FEATURE_SIBLING_RUNNING_DIR_X ] = sibling_run_dir.x;
			//feature[ FEATURE_SIBLING_RUNNING_DIR_Y ] = sibling_run_dir.y;
			//feature[ FEATURE_SIBLING_RUNNING_DIR_Z ] = sibling_run_dir.z;
		}

		/// @brief 指定の枝に関する対応付け結果の評価値を取得する
		/// @param[in] result_branch 対応付け結果の枝
		/// @param[in] truth_branch 正解の枝
		/// @return 評価値
		template < class NameType >
		inline double Tree< NameType >::evaluate_result( const_iterator result_branch, const_iterator truth_branch )const
		{
#ifndef _EVALUATE_HEAD_ONLY
			return length( result_branch );
#else //_EVALUATE_HEAD_ONLY
			return 1;
#endif //_EVALUATE_HEAD_ONLY
		}
		
		/// @brief 名前付き芯線画像を復元する
		/// @param[out] labeled_centerline 名前付き芯線画像を格納する領域
		/// @param[in] branch 復元する枝
		template < class NameType >
		inline void Tree< NameType >::restore_centerline( LabelImage &labeled_centerline, const_iterator branch )const
		{
			Point pt = branch->value.start_pt; // 走査している座標

			// 復元する
			labeled_centerline( pt ) = to_label( branch->name );
				
			for ( BranchValue::PathType::const_iterator iter = branch->value.path.begin( ); iter != branch->value.path.end( ); iter++ )
			{
				// 座標を更新する
				pt += TOPOLOGICAL( neighborhood )[ *iter ];

				// 画像をマークする
				labeled_centerline( pt ) = to_label( branch->name );
			}
		}	

		/// @brief 名前付き血管画像の分岐点を復元する
		/// @param[in] vessel 血管のマーク画像
		/// @param[out] labeled_vessel 名前付き血管画像を格納する領域
		/// @param[in] branch 復元する分岐点の親枝
		template < class NameType >
		inline void Tree< NameType >::restore_bifurcation( const MarkImage &vessel, LabelImage &labeled_vessel, const_iterator branch )const
		{
			for ( size_t k = static_cast< size_t >( branch->value.end_pt.z - branch->value.thk / labeled_vessel.reso3( ) ); k <= static_cast< size_t >( branch->value.end_pt.z + branch->value.thk / labeled_vessel.reso3( ) ); k++ )
			{
				for ( size_t j = static_cast< size_t >( branch->value.end_pt.y - branch->value.thk / labeled_vessel.reso2( ) ); j <= static_cast< size_t >( branch->value.end_pt.y + branch->value.thk / labeled_vessel.reso2( ) ); j++ )
				{
					for ( size_t i = static_cast< size_t >( branch->value.end_pt.x - branch->value.thk / labeled_vessel.reso1( ) ); i <= static_cast< size_t >( branch->value.end_pt.x + branch->value.thk / labeled_vessel.reso1( ) ); i++ )
					{
						try
						{
							if ( vessel( i, j, k ) && !labeled_vessel( i, j, k ) )
							{
								labeled_vessel( i, j, k ) = to_label( branch->name );
							}
						}

						catch ( MarkImage::InvalidPoint )
						{
						}
					}
				}
			}
		}

		/// @brief XMLから枝を読み込む
		/// @param[in] xml 読み込むXML
		/// @param[in] elem 読み込む要素を指すイテレータ
		/// @param[out] branch 読み込む枝を指すイテレータ
		/// @retval true 枝の読み込みに成功した
		/// @retval false 枝の読み込みに失敗した
		template < class NameType >
		inline bool Tree< NameType >::read_branch( const XML &xml, XML::const_iterator elem, iterator branch )
		{
			// 要素探索関数
			class FindElem
			{
				std::string tag_name_; // タグ名
				std::string id_; // 識別子
			public:
				// コンストラクタ
				FindElem( const std::string &tag_name, const std::string &id ) : tag_name_( tag_name ), id_( id )
				{
				}
		
				// 関数呼び出し演算子
				bool operator( )( const XML::Element &elem )
				{
					return elem.tag( ).name( ) == tag_name_ && elem.tag( )[ "id" ] == id_;
				}
			};

			try
			{
				// 太さを読み込む
				branch->value.thk = std::stod( elem->tag( )[ "radius" ] );

				// 芯線経路を読み込む
				XML::const_iterator medialline_elem = std::find( xml.child_begin( elem ), xml.child_end( elem ), "MEDIALLINE" ); // MEDIALLINE要素
				XML::const_iterator position_elem = std::find_if( xml.child_begin( medialline_elem ), xml.child_end( medialline_elem ), FindElem( "POSITION", "1" ) ); // POSITION要素
				XML::const_iterator likelihood_elem = std::find( xml.child_begin( elem ), xml.child_end( elem ), "LIKELIHOOD" ); // LIKELIHOOD要素
				Point prev_pt; // 前の座標
				size_t id = 1; // 識別子

				// エラーチェック
				if ( medialline_elem == xml.child_end( elem ) || position_elem == xml.child_end( medialline_elem ) )
				{
					return false;
				}

				// 始点を読み込む
				branch->value.start_pt.x = static_cast< size_t >( std::stod( position_elem->tag( )[ "x" ] ) / get_pitch( ).x + 0.5 );
				branch->value.start_pt.y = static_cast< size_t >( std::stod( position_elem->tag( )[ "y" ] ) / get_pitch( ).y + 0.5 );
				branch->value.start_pt.z = static_cast< size_t >( std::stod( position_elem->tag( )[ "z" ] ) / get_pitch( ).z + 0.5 );
				prev_pt = branch->value.start_pt;

				// 経路を読み込む
				for ( ; position_elem != xml.child_end( medialline_elem ); position_elem = std::find_if( xml.child_begin( medialline_elem ), xml.child_end( medialline_elem ), FindElem( "POSITION", std::to_string( static_cast< unsigned long long >( ++id ) ) ) ) )
				{
					Shift shift; // 変位
			
					// 座標を読み込む
					branch->value.end_pt.x = static_cast< size_t >( std::stod( position_elem->tag( )[ "x" ] ) / get_pitch( ).x + 0.5 );
					branch->value.end_pt.y = static_cast< size_t >( std::stod( position_elem->tag( )[ "y" ] ) / get_pitch( ).y + 0.5 );
					branch->value.end_pt.z = static_cast< size_t >( std::stod( position_elem->tag( )[ "z" ] ) / get_pitch( ).z + 0.5 );

					// インデックスを計算する
					shift = branch->value.end_pt - prev_pt;

					for ( size_t i = 0; i < TOPOLOGY; i++ )
					{
						if ( shift == TOPOLOGICAL( neighborhood )[ i ] )
						{
							branch->value.path.insert( branch->value.path.end( ), i );
					
							break;
						}
					}

					// 座標を更新する
					prev_pt = branch->value.end_pt;
				}

				// 尤度を読み込む
				if ( likelihood_elem != xml.child_end( elem ) )
				{
					branch->likelihood.clear( );
						
					for ( XML::Element::Tag::const_iterator iter = likelihood_elem->tag( ).begin( ); iter != likelihood_elem->tag( ).end( ); iter++ )
					{
						branch->likelihood.insert( branch->likelihood.end( ), Likelihood< NameType >::value_type( to_name( iter->first ), std::stod( iter->second ) ) );
					}
				}

				return true;
			}

			catch ( XML::Element::Tag::InvalidProperty )
			{
				return false;
			}
		}

		/// @brief XMLに枝を書き込む
		/// @param[in] xml 書き込むXML
		/// @param[out] elem 書き込む要素を指すイテレータ
		/// @param[in] branch 書き込む枝を指すイテレータ
		/// @retval true 枝の書き込みに成功した
		/// @retval false 枝の書き込みに失敗した
		template < class NameType >
		inline bool Tree< NameType >::write_branch( XML &xml, XML::iterator elem, const_iterator branch )const
		{
			// 太さを書き込む
			elem->tag( ).insert( elem->tag( ).end( ), XML::Element::Tag::value_type( "radius", std::to_string( static_cast< long double >( branch->value.thk ) ) ) );

			// 芯線経路を書き込む
			XML::iterator medialline_elem = xml.insert( elem, XML::Element( "MEDIALLINE" ) ); // MEDIALLINE要素
			Point pt = branch->value.start_pt; // 走査している座標
			size_t id = 1; // 識別子

			for ( BranchValue::PathType::const_iterator iter = branch->value.path.begin( ); ; iter++ )
			{
				XML::iterator position_elem = xml.insert( medialline_elem, XML::Element( "POSITION" ) ); // POSITION要素

				medialline_elem = xml.parent( position_elem );
				position_elem->tag( ).insert( position_elem->tag( ).end( ), XML::Element::Tag::value_type( "id", std::to_string( static_cast< unsigned long long >( id++ ) ) ) );
				position_elem->tag( ).insert( position_elem->tag( ).end( ), XML::Element::Tag::value_type( "x", std::to_string( static_cast< long double >( pt.x * get_pitch( ).x ) ) ) );
				position_elem->tag( ).insert( position_elem->tag( ).end( ), XML::Element::Tag::value_type( "y", std::to_string( static_cast< long double >( pt.y * get_pitch( ).y ) ) ) );
				position_elem->tag( ).insert( position_elem->tag( ).end( ), XML::Element::Tag::value_type( "z", std::to_string( static_cast< long double >( pt.z * get_pitch( ).z ) ) ) );
		
				if ( iter == branch->value.path.end( ) )
				{
					break;
				}
				
				pt += TOPOLOGICAL( neighborhood )[ *iter ];
			}

#ifdef _WRITE_LIKELIHOOD
			// 尤度を書き込む
			if ( !branch->likelihood.empty( ) ) 
			{
				XML::iterator likelihood_elem = xml.insert( xml.parent( medialline_elem ), XML::Element( "LIKELIHOOD" ) ); // LIKELIHOOD要素

				for ( Likelihood< NameType >::const_iterator likelihood_iter = branch->likelihood.begin( ); likelihood_iter != branch->likelihood.end( ); likelihood_iter++ ) 
				{
					likelihood_elem->tag( ).insert( likelihood_elem->tag( ).end( ), XML::Element::Tag::value_type( to_string( likelihood_iter->first ), std::to_string( static_cast< long double >( likelihood_iter->second ) ) ) );
				}
			}
#endif //_WRITE_LIKELIHOOD

			return true;
		}
		
		/// @brief 末端までの最長経路の長さを取得する
		/// @param[in] branch 末端までの最長経路の長さを取得する枝
		/// @return 末端までの最長経路の長さ
		template < class NameType >
		template < class Iterator >
		inline double Tree< NameType >::max_length( Iterator branch )const
		{
			double max_len = 0; //長さ

			for ( const_iterator iter = child_begin( branch ); iter != child_end( branch ); iter++ )
			{
				max_len = std::max( max_len, max_length( iter ) );
			}

			return max_len + length( branch );
		}
			
		/// @brief 子孫の分岐数を取得する
		/// @param[in] branch 子孫の分岐数を取得する枝
		/// @return 子孫の分岐数
		template < class NameType >
		template < class Iterator >
		size_t Tree< NameType >::bifurcation_num( Iterator branch )const
		{
			size_t cnt = 0; // 分岐数

			for ( const_iterator iter = subtree( branch ).begin( ); iter != subtree( branch ).end( ); iter++ )
			{
				if ( !is_leaf( iter ) )
				{
					cnt++;
				}
			}

			return cnt;
		}

		/// @brief ベクトルを解像度を考慮した実効ベクトルに変換する
		/// @param[in] vec 実効ベクトルに変換するベクトル
		/// @return 実効ベクトル
		template < class NameType >
		template < class VectorType >
		inline Vector Tree< NameType >::effective( const VectorType &vec )const
		{
			return Vector( vec.x * get_pitch( ).x, vec.y * get_pitch( ).y, vec.z * get_pitch( ).z );
		}
	
		/// @brief 尤度を加算する
		/// @param[in] l 加算される尤度
		/// @return 自分自身の参照
		template < class NameType >
		inline Likelihood< NameType > &Likelihood< NameType >::operator+=( const Likelihood &l )
		{
			for ( const_iterator iter = l.begin( ); iter != l.end( ); iter++ )
			{
				operator[ ]( iter->first ) += iter->second;
			}

			return *this;
		}
	}
}

#endif //_TMATSU_VESSEL_H_