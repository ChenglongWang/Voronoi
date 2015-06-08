/**
* @file tubular.h
* @brief 管腔臓器名対応付けのヘッダファイル
* @version 3.0
* @author 名古屋大学 情報科学研究科 メディア科学専攻 森研究室   351203192   松﨑 哲朗
* @date 2013/10/18
*/

#ifndef _TMATSU_TUBULAR_H_
#define _TMATSU_TUBULAR_H_

#include <string>
#include <list>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <mist/filter/distance.h>
#include <mist/filter/thinning.h>
#include <mist/filter/labeling.h>
#include "config.h"
#include "image.h"
#include "tree.h"
#include "xml.h"
#include "graph.h"

#if TOPOLOGY == 26
#define TOPOLOGICAL( a ) a ## 26 ///< @brief トポロジを考慮し接尾語を付加する
#elif TOPOLOGY == 6
#define TOPOLOGICAL( a ) a ## 6 ///< @brief トポロジを考慮し接尾語を付加する
#else
#error
#endif

namespace tmatsu
{
	namespace tubular 
	{
		/// @brief 管腔臓器木構造の枝を扱う型
		/// @tparam ValueType データ型
		/// @tparam NameType 名前の型
		/// @tparam LikeihoodType 尤度の組の型
		template < class ValueType, class NameType, class LikelihoodType >
		struct _Branch
		{
			typedef ValueType ValueType; ///< @brief データ型
			size_t id; ///< @brief 識別子
			NameType name; ///< @brief 名前
			LikelihoodType likelihood; ///< @brief 尤度

#ifdef _UPDATE_LIKELIHOOD
			LikelihoodType prev_likelihood; ///< @brief 直前の尤度
#endif //_UPDATE_LIKELIHOOD

#ifdef _SCREEN_OUT_CANDIDATE
			typedef std::map< NameType, std::set< NameType > > CandidateType; ///< @brief 血管名候補を表す型
			CandidateType candidate; ///< @brief 血管名候補
#endif //_SCREEN_OUT_CANDIDATE

#ifdef _FIND_OPTIMUM_BRANCHING
			typedef std::map< NameType, LikelihoodType > PlausibilityType; ///< @brief 親ごとの尤度を表す型
			typedef std::map< NameType, std::pair< size_t, double > > BifurcationType; ///< @brief 分岐を扱う型
			typedef std::map< NameType, BifurcationType > BifurcationTableType; ///< @brief 分岐表を扱う型
			PlausibilityType plausibility; 
			BifurcationTableType bifurcation_table;
#endif //_FIND_OPTIMUM_BRANCHING

			ValueType value; ///< @brief データ
			bool operator==( size_t id )const;
			bool operator==( const NameType &name )const;
		};

		/// @brief 管腔臓器画像を扱うクラス
		/// @tparam ValueType データ型
		template < class ValueType >
		class Image : public tmatsu::Image< ValueType >
		{
		public:
			Image( );
			Image( const Image &img );
			template < class ImageType >
			Image( const ImageType &img );
			template < class Type >
			Image( const Image< Type > &img );
			template < class Functor >
			Image( Functor callback, const char *file_name, const char *hdr_name = NULL );
			template < class ImageType >
			void thinning( ImageType &centerline )const;
			Image &operator=( const Image &img );
			template < class ImageType >
			Image &operator=( const ImageType &img );
			template < class Type >
			Image &operator=( const Image< Type > &img );
		};

		typedef Image< short > CTImage; ///< @brief CT画像を扱う型
		typedef Image< unsigned char > MarkImage; ///< @brief マーク画像を扱う型
		typedef Image< float > ThicknessImage; ///< @brief 太さ画像を扱う型
		typedef Image< unsigned char > LabelImage; ///< @brief ラベル画像を扱う型

		/// @brief 管腔臓器木構造を扱う抽象クラス
		/// @tparam ValueType データ型
		/// @tparam NameType 枝名の型
		/// @tparam LikeihoodType 尤度の組の型．名前から尤度への連想配列とみなすことができ，また加算を行うことができなければならない
		/// @tparam FeatureType 特徴ベクトルの型
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		class Tree : public tree< _Branch< ValueType, NameType, LikelihoodType > >
		{
		public:
			typedef _Branch< ValueType, NameType, LikelihoodType > Branch; ///< @brief 管腔臓器木構造の枝を扱う型
		private:
			SizeSet siz_; ///< @brief サイズ
			PitchSet pitch_; ///< @brief ピッチ
		public:
			Tree( );
			Tree( const Tree &tree );
			Tree( const ThicknessImage &centerline );
			Tree( const char *file_name, const SizeSet &siz, const PitchSet &pitch );
			void construct( const ThicknessImage &centerline );
			void construct(const ThicknessImage &centerline, const Point& seedPoint);
			template < class Container >
			void extract( Container &features )const;
			template < class Functor >
			void labeling( Tree &result, Functor likelihood_func )const;
			void evaluate( Matrix &confusion, const Tree &truth )const;
			void restore( LabelImage &labeled_centerline )const;
			void restore( const MarkImage &tubular, LabelImage &labeled_tubular )const;
			bool read( const char *file_name, const SizeSet &siz, const PitchSet &pitch );
			bool write( const char *file_name )const;
			const SizeSet &get_size( )const;
			void set_size( const SizeSet &siz );
			const PitchSet &get_pitch( )const;
			void set_pitch( const PitchSet &pitch );
		protected:
			/// @brief 木構造の根となる画素を取得する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @param[in] centerline 芯線画像
			/// @return 木構造の根となる画素の座標
			virtual Point root_point( const ThicknessImage &centerline )const = 0;
			virtual Point root_point(const ThicknessImage &centerline, Point seedPoint) const = 0;

			/// @brief 芯線を走査し1本の枝のデータを取得する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @param[out] val 取得するデータ
			/// @param[in] centerline 走査する芯線画像．この画像は書き換えてもよい
			/// @param[in] start_pt 走査の始点の座標
			/// @return 枝の終点の座標
			virtual Point traverse_branch( ValueType &val, ThicknessImage &centerline, const Point &start_pt )const = 0;

			/// @brief 基準枝を取得する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @attention この関数は対応付けが行われる前にも行われた後にも呼び出される
			/// @return 基準枝の定数参照
			virtual const_iterator reference_branch( )const = 0;
			
			/// @brief 特徴量を抽出する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @param[out] feature 抽出した特徴量を格納する領域
			/// @param[in] branch 特徴量を抽出する枝
			/// @param[in] ref_branch 基準枝
			virtual void extract_feature( FeatureType &feature, const_iterator branch, const_iterator ref_branch )const = 0;
			
			/// @brief 指定の枝に関する対応付け結果の評価値を取得する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @param[in] result_branch 対応付け結果の枝
			/// @param[in] truth_branch 正解の枝
			/// @return 評価値
			virtual double evaluate_result( const_iterator result_branch, const_iterator truth_branch )const = 0;
			
			/// @brief 名前付き芯線画像を復元する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @param[out] labeled_centerline 名前付き芯線画像を格納する領域
			/// @param[in] branch 復元する枝
			virtual void restore_centerline( LabelImage &labeled_centerline, const_iterator branch )const = 0;
			
			/// @brief 名前付き管腔臓器画像の分岐点を復元する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @param[in] tubular 管腔臓器のマーク画像
			/// @param[out] labeled_tubular 名前付き管腔臓器画像を格納する領域
			/// @param[in] branch 復元する分岐点の親枝
			virtual void restore_bifurcation( const MarkImage &tubular, LabelImage &labeled_tubular, const_iterator branch )const = 0;
			
			/// @brief XMLから枝を読み込む純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @param[in] xml 読み込むXML
			/// @param[in] elem 読み込む要素を指すイテレータ
			/// @param[out] branch 読み込む枝を指すイテレータ
			/// @retval true 枝の読み込みに成功した
			/// @retval false 枝の読み込みに失敗した．このとき，木構造の読み込みは中止される
			virtual bool read_branch( const XML &xml, XML::const_iterator elem, iterator branch ) = 0;
			
			/// @brief XMLに枝を書き込む純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @param[in] xml 書き込むXML
			/// @param[out] elem 書き込む要素を指すイテレータ
			/// @param[in] branch 書き込む枝を指すイテレータ
			/// @retval true 枝の書き込みに成功した
			/// @retval false 枝の書き込みに失敗した．このとき，木構造の書き込みは中止される
			virtual bool write_branch( XML &xml, XML::iterator elem, const_iterator branch )const = 0;
			
			/// @brief リジェクト名を取得する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @return リジェクト名
			virtual NameType reject_name( )const = 0;
			
			/// @brief 根の親枝の尤度として扱われる尤度の規定値を取得する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @return 尤度の規定値
			virtual LikelihoodType default_likelihood( )const = 0;
			
			/// @brief リジェクト名を含む名前の総数を取得する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @return 名前の総数
			virtual size_t name_number( )const = 0;
			
			/// @brief 名前を文字列に変換する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @param[in] name 変換する名前
			/// @return 名前を表す文字列
			virtual std::string to_string( const NameType &name )const = 0;
			
			/// @brief 名前をインデックスに変換する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @attention 各名前を変換した結果がそれぞれ0, 1, ..., name_number( ) - 1にならなければならない
			/// @param[in] name 変換する名前
			/// @return 名前に対応するインデックス
			virtual size_t to_index( const NameType &name )const = 0;
			
			/// @brief 文字列を名前に変換する純粋仮想関数
			/// @attention このクラスを実体化するためには，派生クラスを作成し，この関数を実装しなければならない
			/// @param[in] str 変換する文字列
			/// @return 文字列の表す名前
			virtual NameType to_name( const std::string &str )const = 0;

			virtual void construct_postprocess( );
			virtual void labeling_preprocess( );
			virtual void labeling_postprocess( );
		private:

#ifdef _FIND_MAX_LIKELIHOOD
			std::pair< iterator, double > find_max_likelihood( const NameType &name, iterator branch );
#endif //_FIND_MAX_LIKELIHOOD
			
#ifdef _SELECT_MODEL
			void select_model( iterator branch, typename tree< NameType >::const_iterator selected_model, const tree< NameType > &model );
			double find_max_descendant_score( iterator branch, typename tree< NameType >::const_iterator selected_model, const tree< NameType > &model );
			double calc_self_score( iterator branch, NameType name );
			double calc_ancestor_score( iterator branch, NameType name );
			double calc_descendant_score( iterator branch, NameType name );
#endif//_SELECT_MODEL

#ifdef _FIND_OPTIMUM_BRANCHING
			void calc_table( iterator branch, std::set< NameType > names, std::map< NameType, std::set< NameType > > &integrity );
			typename Branch::BifurcationType &calc_bifurcation( iterator branch, iterator descendant_branch, NameType name, std::map< NameType, std::set< NameType > > &integrity, typename Branch::BifurcationType &bifurcation );
			double calc_tl( iterator branch, NameType name );
			double calc_hd( iterator branch, NameType name, NameType parent_name );
			double calc_score( const typename Branch::BifurcationType &bifurcation );
			void find_optimum_branching( std::set< NameType > names, std::map< NameType, std::set< NameType > > &integrity );
			void find_optimum_branching( const typename Branch::BifurcationTableType &directed_graph, typename Branch::BifurcationTableType &optimum_branching )const;
#ifdef _IGNORE_TAIL_SCORE
			std::pair< iterator, double > find_max_plausibility( const NameType &name, iterator branch );
#endif //_IGNORE_TAIL_SCORE
#endif //_FIND_OPTIMUM_BRANCHING

			bool read( const XML &xml, const std::string &id, iterator branch );
		};

		/// @brief 動作無し
		template < class ValueType >
		inline Image< ValueType >::Image( )
		{
		}
	
		/// @brief 画像を複製する
		/// @param[in] img 複製する画像
		template < class ValueType >
		inline Image< ValueType >::Image( const Image &img ) : tmatsu::Image< ValueType >( img )
		{
		}

		/// @brief 型の異なる画像を複製する
		/// @param[in] img 複製する画像
		template < class ValueType >
		template < class ImageType >
		inline Image< ValueType >::Image( const ImageType &img ) : tmatsu::Image< ValueType >( img )
		{
		}

		/// @brief データ型の異なる画像を複製する
		/// @param[in] img 複製する画像
		template < class ValueType >
		template < class Type >
		inline Image< ValueType >::Image( const Image< Type > &img ) : tmatsu::Image< ValueType >( img )
		{
		}

		/// @brief 画像をファイルから読み込む
		/// @param[in] callback コールバック関数
		/// @param[in] file_name 読み込むファイル名
		/// @param[in] hdr_name ヘッダファイル名．規定値はNULL．NULLが指定された場合，file_nameに".header"を付加したヘッダファイル名が設定される
		template < class Type >
		template < class Functor >
		inline Image< Type >::Image( Functor callback, const char *file_name, const char *hdr_name ) : tmatsu::Image< ValueType >( callback, file_name, hdr_name )
		{
		}
	
		/// @brief 距離値が記録された芯線画像を取得する
		/// @attention 出力画像のデータ型は浮動点小数型でなければならない
		/// @param[out] centerline 芯線を格納する画像
		template < class ValueType >
		template < class ImageType >
		inline void Image< ValueType >::thinning( ImageType &centerline )const
		{
			Image img; // 一時画像
			ImageType thickness; // 太さ画像

			// 初期化する
			centerline = *this;

			// 穴を埋める
			mist::remove_hole_region( *this, img );

			// 細線化する
			TOPOLOGICAL( mist::euclidean::thinning )( img, centerline );

			// 距離変換する
			mist::euclidean::distance_transform( *this, thickness );
			img.clear( );

			// 太さを代入する
			for ( size_t i = 0; i < centerline.size( ); i++ )
			{
				if ( centerline[ i ] )
				{
					centerline[ i ] = static_cast< ImageType::value_type >( std::sqrt( thickness[ i ] ) * centerline.reso1( ) );
				}
			}
		}

		/// @brief 画像を複製する
		/// @param[in] img 複製する画像
		/// @return 自分自身の参照
		template < class ValueType >
		inline Image< ValueType > &Image< ValueType >::operator=( const Image &img )
		{
			if ( this != &img )
			{
				tmatsu::Image< ValueType >::operator=( img );
			}

			return *this;
		}

		/// @brief 型の異なる画像を複製する
		/// @param[in] img 複製する画像
		/// @return 自分自身の参照
		template < class ValueType >
		template < class ImageType > 
		inline Image< ValueType > &Image< ValueType >::operator=( const ImageType &img )
		{
			if ( reinterpret_cast< const void * >( this ) != reinterpret_cast< const void * >( &img ) )
			{
				tmatsu::Image< ValueType >::operator=( img );
			}

			return *this;
		}

		/// @brief データ型の異なる画像を複製する
		/// @param[in] img 複製する画像
		/// @return 自分自身の参照
		template < class ValueType >
		template < class Type > 
		inline Image< ValueType > &Image< ValueType >::operator=( const Image< Type > &img )
		{
			if ( reinterpret_cast< const void * >( this ) != reinterpret_cast< const void * >( &img ) )
			{
				tmatsu::Image< ValueType >::operator=( img );
			}

			return *this;
		}

		/// @brief 動作無し
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( )
		{
		}
	
		/// @brief 管腔臓器木構造を複製する
		/// @param[in] tree 複製する管腔臓器木構造
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( const Tree &tree ) : siz_( tree.siz_ ), pitch_( tree.pitch_ ), tree< _Branch< ValueType, NameType, LikelihoodType > >( tree )
		{
		}
	
		/// @brief 芯線画像から木構造を構築する
		/// @param[in] centerline 芯線画像
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( const ThicknessImage &centerline )
		{
			construct( centerline );
		}
	
		/// @brief ファイルから木構造を読み込む
		/// @param[in] file_name 読み込むXMLファイル名
		/// @param[in] siz 画像のサイズ
		/// @param[in] pitch 画像のピッチ
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( const char *file_name, const SizeSet &siz, const PitchSet &pitch )
		{
			read( file_name, siz, pitch );
		}

		/// @brief 芯線画像から木構造を構築する
		/// @param[in] centerline 芯線画像
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::construct( const ThicknessImage &centerline )
		{
			ThicknessImage temp = centerline; // 一時画像
			std::queue< std::tr1::tuple< size_t, Point, ThicknessImage::value_type > > queue; // 待ち行列
			size_t id = 1; // 識別子
	
			// サイズとピッチを設定する
			siz_.x = centerline.width( );
			siz_.y = centerline.height( );
			siz_.z = centerline.depth( );
			pitch_.x = centerline.reso1( );
			pitch_.y = centerline.reso2( );
			pitch_.z = centerline.reso3( );

			// 木構造を空にする
			clear( );

			// 各枝を走査する
			do
			{
				size_t parent_id = queue.empty( ) ? 0 : std::tr1::get< 0 >( queue.front( ) ); //親の識別子
				Point start_pt = queue.empty( ) ? root_point( temp ) : std::tr1::get< 1 >( queue.front( ) ); //始点
				Point end_pt; // 終点
				iterator parent_branch = end( ); // 親枝
				iterator branch; // 走査する枝
		
				// 太さを復元する
				if ( !queue.empty( ) )
				{
					// 太さを復元する
					temp( start_pt ) = std::tr1::get< 2 >( queue.front( ) );

					// デキューする
					queue.pop( );
				}
		
				// 親枝を探索する
				if ( parent_id )
				{
					for ( iterator iter = postorder_begin( ); iter != postorder_end( ); iter++ )
					{
						if ( iter->id == parent_id )
						{
							parent_branch = iter;
							break;
						}
					}
				}
		
				// 枝を挿入する
				branch = insert( parent_branch, Branch( ) );
				branch->id = id++;
				branch->name = reject_name( );
				end_pt = traverse_branch( branch->value, temp, start_pt );
		
				// 子供をエンキュー
				for ( size_t i = 0; i < TOPOLOGY; i++ )
				{
					try
					{
						Point next_pt = end_pt + TOPOLOGICAL( neighborhood )[ i ]; // 子供の枝の始点

						if ( temp( next_pt ) > 0 )
						{
							// 未走査画素のとき
							queue.push( std::tr1::make_tuple( branch->id, next_pt, temp( next_pt ) ) );
							temp( next_pt ) = 0;
						}
					}

					catch ( ThicknessImage::InvalidPoint )
					{
					}
				}
			}
			while ( !queue.empty( ) );

			// 識別子を初期化する
			for ( iterator iter = begin( ); iter != end( ); iter++ )
			{
				iter->id = 0;
			}

			// 後処理を行う
			construct_postprocess( );

			// 識別子を振り直す
			id = 1;

			for ( iterator iter = preorder_begin( ); iter != preorder_end( ); iter++ )
			{
				iter->id = id++;
			}
		}

		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::construct(const ThicknessImage &centerline, const Point& seedPoint)
		{
			ThicknessImage temp = centerline; // 一時画像
			std::queue< std::tr1::tuple< size_t, Point, ThicknessImage::value_type > > queue; // 待ち行列
			size_t id = 1; // 識別子

			// サイズとピッチを設定する
			siz_.x = centerline.width();
			siz_.y = centerline.height();
			siz_.z = centerline.depth();
			pitch_.x = centerline.reso1();
			pitch_.y = centerline.reso2();
			pitch_.z = centerline.reso3();

			// 木構造を空にする
			clear();

			// 各枝を走査する
			do
			{
				size_t parent_id = queue.empty() ? 0 : std::tr1::get< 0 >(queue.front()); //親の識別子
				Point start_pt = queue.empty() ? root_point(temp, seedPoint) : std::tr1::get< 1 >(queue.front()); //始点
				Point end_pt; // 終点
				iterator parent_branch = end(); // 親枝
				iterator branch; // 走査する枝

				// 太さを復元する
				if (!queue.empty())
				{
					// 太さを復元する
					temp(start_pt) = std::tr1::get< 2 >(queue.front());

					// デキューする
					queue.pop();
				}

				// 親枝を探索する
				if (parent_id)
				{
					for (iterator iter = postorder_begin(); iter != postorder_end(); iter++)
					{
						if (iter->id == parent_id)
						{
							parent_branch = iter;
							break;
						}
					}
				}

				// 枝を挿入する
				branch = insert(parent_branch, Branch());
				branch->id = id++;
				branch->name = reject_name();
				end_pt = traverse_branch(branch->value, temp, start_pt);

				// 子供をエンキュー
				for (size_t i = 0; i < TOPOLOGY; i++)
				{
					try
					{
						Point next_pt = end_pt + TOPOLOGICAL(neighborhood)[i]; // 子供の枝の始点

						if (temp(next_pt) > 0)
						{
							// 未走査画素のとき
							queue.push(std::tr1::make_tuple(branch->id, next_pt, temp(next_pt)));
							temp(next_pt) = 0;
						}
					}

					catch (ThicknessImage::InvalidPoint)
					{
					}
				}
			} while (!queue.empty());

			// 識別子を初期化する
			for (iterator iter = begin(); iter != end(); iter++)
			{
				iter->id = 0;
			}

			// 後処理を行う
			construct_postprocess();

			// 識別子を振り直す
			id = 1;

			for (iterator iter = preorder_begin(); iter != preorder_end(); iter++)
			{
				iter->id = id++;
			}
		}

		/// @brief 特徴量を抽出する
		/// @param[in] features 抽出した特徴量を格納するコンテナ．STLコンテナとみなせなければならない
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		template < class Container >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::extract( Container &features )const
		{
			const_iterator ref_branch = reference_branch( ); // 基準枝
		
			// コンテナを空にする
			features.clear( );

			// 特徴量を抽出する
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				FeatureType feature; // 特徴ベクトル
				
				extract_feature( feature, iter, ref_branch );
				features.insert( features.end( ), feature );
			}
		}
		
		/// @brief 対応付けを行う
		/// @param[out] result 対応付け結果を格納する木構造
		/// @param[in] likelihood_func 尤度関数．以下のような呼び出しができる関数とみなせなければならない
		/// @code
		/// NameType name; // 名前
		/// FeatureType feature; // 特徴ベクトル
		/// LikelihoodType likelihood // 尤度
		/// 
		/// likelihood = likelihood_func( name, feature );
		/// @endcode
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		template < class Functor >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::labeling( Tree &result, Functor likelihood_func )const
		{
			std::set< NameType > preprocessed_names; // 前処理により対応付ける名前
			std::set< NameType > names; // 尤度関数により対応付ける名前
			std::set< NameType > combined_names; // 全ての名前
			
			// 出力を初期化する
			result = *this;

			for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				iter->name = result.reject_name( );
			}

			// 前処理を行う
			result.labeling_preprocess( );

			// 前処理により対応付けられた名前を求める
			for ( const_iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				if ( iter->name != reject_name( ) )
				{
					preprocessed_names.insert( iter->name );
				}
			}

#ifdef _CALC_LIKELIHOOD
			// 尤度を計算する
			for ( iterator iter = result.preorder_begin( ); iter != result.preorder_end( ); iter++ )
			{
				if ( iter->name == reject_name( ) )
				{
					FeatureType feature; // 特徴ベクトル
					const_iterator ref_branch = result.reference_branch( ); // 基準枝
					LikelihoodType parent_likelihood; // 親枝の尤度
			
					// 初期化する
					iter->likelihood.clear( );
			
					// 特徴量を抽出する
					result.extract_feature( feature, iter, ref_branch );

					// 親枝の尤度を計算する
					if ( result.is_root( iter ) )
					{
						parent_likelihood = result.default_likelihood( );
					}
					else
					{
						double parent_sum = 0; // 親枝の尤度の総和

						parent_likelihood = result.parent( iter )->likelihood;

						// 親枝の尤度の総和を計算する
						for ( LikelihoodType::const_iterator parent_iter = parent_likelihood.begin( ); parent_iter != parent_likelihood.end( ); parent_iter++ )
						{
							parent_sum += parent_iter->second;
						}

						// 親枝の尤度を正規化する
						if ( parent_sum )
						{
							for ( LikelihoodType::iterator parent_iter = parent_likelihood.begin( ); parent_iter != parent_likelihood.end( ); parent_iter++ )
							{
								parent_iter->second /= parent_sum;
							}
						}
					}	

					// 尤度を算出する
					for ( LikelihoodType::const_iterator parent_iter = parent_likelihood.begin( ); parent_iter != parent_likelihood.end( ); parent_iter++ )
					{
						LikelihoodType likelihood = likelihood_func( parent_iter->first, feature ); // 尤度
						double avg = 0, var = 0; // 尤度の統計量
						size_t cnt = 0; // 名前の数

						// 尤度関数により対応付ける名前を求め，前処理で対応付けられた名前を除外する
						for ( LikelihoodType::iterator child_iter = likelihood.begin( ); child_iter != likelihood.end( ); )
						{
							if ( preprocessed_names.find( child_iter->first ) != preprocessed_names.end( ) )
							{
								child_iter = likelihood.erase( child_iter );
							}
							else
							{
								// 名前を挿入する
								names.insert( child_iter->first );

								// 指数関数をとる
								//child_iter->second = std::exp( child_iter->second );

								// 統計量を計算する
								avg += child_iter->second;
								var += std::pow( child_iter->second, 2 );
								cnt++;

								// 前進する
								child_iter++;
							}
						}

						avg /= cnt;
						var = var / cnt - std::pow( avg, 2 );

						// 正規化する
						for ( LikelihoodType::iterator child_iter = likelihood.begin( ); child_iter != likelihood.end( ); child_iter++ )
						{
							child_iter->second = std::max( ( child_iter->second - avg ) / ( var ? std::sqrt( var ) : 1 ), 0.0 );
							//child_iter->second = std::exp( ( child_iter->second - avg ) / ( var ? std::sqrt( var ) : 1 ) );
						}

#ifdef _FIND_OPTIMUM_BRANCHING
						// 親ごとの尤度を格納する
						iter->plausibility.insert( Branch::PlausibilityType::value_type( parent_iter->first, likelihood ) );

						// 各尤度を正規化する
						for ( Branch::PlausibilityType::iterator plausibility_iter = iter->plausibility.begin( ); plausibility_iter != iter->plausibility.end( ); plausibility_iter++ )
						{
							double likelihood_sum = 0; // 尤度の総和
							
							for ( LikelihoodType::iterator likelihood_iter = plausibility_iter->second.begin( ); likelihood_iter != plausibility_iter->second.end( ); likelihood_iter++ )
							{
								likelihood_sum += likelihood_iter->second;
							}

							for ( LikelihoodType::iterator likelihood_iter = plausibility_iter->second.begin( ); likelihood_iter != plausibility_iter->second.end( ); likelihood_iter++ )
							{
								likelihood_iter->second /= likelihood_sum > 0 ? likelihood_sum : 1;
							}
						}
#endif //_FIND_OPTIMUM_BRANCHING

						// 親の尤度をかける
						for ( LikelihoodType::iterator child_iter = likelihood.begin( ); child_iter != likelihood.end( ); child_iter++ )
						{
							child_iter->second *= parent_iter->second;
						}

						// 尤度を加算する
						iter->likelihood += likelihood;
					}
				}
				else
				{
					iter->likelihood.insert( iter->likelihood.end( ), LikelihoodType::value_type( iter->name, 1 ) );
				}
			}

			// 全ての名前を求める
			combined_names.insert( names.begin( ), names.end( ) );
			combined_names.insert( preprocessed_names.begin( ), preprocessed_names.end( ) );

			// 正規化を行う
			for ( std::set< NameType >::const_iterator name_iter = combined_names.begin( ); name_iter != combined_names.end( ); name_iter++ )
			{
				double likelihood_sum = 0; // 尤度の総和
						
				for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
				{
					likelihood_sum += iter->likelihood[ *name_iter ];
				}

				for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
				{
					iter->likelihood[ *name_iter ] /= likelihood_sum > 0 ? likelihood_sum : 1;
				}
			}
#endif //_CALC_LIKELIHOOD

#ifdef _CALC_INTEGRITY
			FeatureType feature; // 特徴ベクトル
			std::map< NameType, std::set< NameType > > integrity; // 可能な分岐

			result.extract_feature( feature, result.begin( ), result.reference_branch( ) );

			// 可能な分岐を計算する
			for ( std::set< NameType >::const_iterator iter = combined_names.begin( ); iter != combined_names.end( ); iter++ )
			{
				LikelihoodType likelihood = likelihood_func( *iter, feature ); // 尤度
				std::map< NameType, std::set< NameType > >::iterator integrity_iter = integrity.insert( integrity.end( ), std::map< NameType, std::set< NameType > >::value_type( *iter, std::set< NameType >( ) ) ); //整合性

				for ( LikelihoodType::const_iterator likelihood_iter = likelihood.begin( ); likelihood_iter != likelihood.end( ); likelihood_iter++ )
				{
					integrity_iter->second.insert( likelihood_iter->first );
				}
			}
#endif //_CALC_INTEGRITY

#if defined( _UPDATE_LIKELIHOOD )
			double err = DBL_MAX; //誤差

			// 正規化する
			for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				double likelihood_sum = 0; //尤度の総和
			
				for ( LikelihoodType::const_iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_sum += likelihood_iter->second;
				}
		
				for ( LikelihoodType::iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_iter->second /= likelihood_sum > 0 ? likelihood_sum : 1;
				}
			}

			// 尤度を更新
			size_t cnt = 0; // カウンタ

			while ( err >= 1.0e-6 && cnt++ < 1000 ) 
			{		
				err = 0;

				for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
				{
					iter->prev_likelihood = iter->likelihood;
				}

				for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
				{
					double likelihood_sum = 0;

					for ( LikelihoodType::iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
					{
						double parent_consistency = 1, child_consistency = 1, sibling_consistency = 1; // 無矛盾度
			
						// 親との無矛盾度
						if ( !result.is_root( iter ) )
						{
							for ( LikelihoodType::const_iterator parent_likelihood_iter = result.parent( iter )->prev_likelihood.begin( ); parent_likelihood_iter != result.parent( iter )->prev_likelihood.end( ); parent_likelihood_iter++ )
							{
								if ( integrity[ parent_likelihood_iter->first ].find( likelihood_iter->first ) == integrity[ parent_likelihood_iter->first ].end( ) )
								{
									parent_consistency -= parent_likelihood_iter->second;
								}
							}
						}

						// 子供との無矛盾度
						for ( iterator child_iter = result.child_begin( iter ); child_iter != result.child_end( iter ); child_iter++ )
						{
							double consistency = 1; // 無矛盾度

							for ( LikelihoodType::const_iterator child_likelihood_iter = child_iter->prev_likelihood.begin( ); child_likelihood_iter != child_iter->prev_likelihood.end( ); child_likelihood_iter++ )
							{
								if ( integrity[ likelihood_iter->first ].find( child_likelihood_iter->first ) == integrity[ likelihood_iter->first ].end( ) )
								{
									consistency -= child_likelihood_iter->second;
								}
							}
								
							child_consistency *= consistency;
						}

						// 兄弟との無矛盾度
						if ( !result.is_root( iter ) )
						{
							for ( iterator sibling_iter = result.child_begin( result.parent( iter ) ); sibling_iter != result.child_end( result.parent( iter ) ); sibling_iter++ )
							{
								if ( iter != sibling_iter )
								{
									sibling_consistency *= 1 - sibling_iter->prev_likelihood[ likelihood_iter->first ];
								}
							}
						}

						likelihood_iter->second *= parent_consistency * child_consistency * sibling_consistency;
						likelihood_sum += likelihood_iter->second;
					}

					for ( LikelihoodType::iterator likelihood_iter = iter->likelihood.begin( ), prev_likelihood_iter = iter->prev_likelihood.begin( ); likelihood_iter != iter->likelihood.end( ) && prev_likelihood_iter != iter->prev_likelihood.end( ); likelihood_iter++, prev_likelihood_iter++ )
					{
						likelihood_iter->second /= likelihood_sum > 0 ? likelihood_sum : 1;
						err += std::abs( likelihood_iter->second - prev_likelihood_iter->second );
					}
				}

				std::cout << "err[ " << cnt << " ] = " << err << std::endl;
			}
#endif //_UPDATE_LIKELIHOOD

#if defined( _FIND_MAX_LIKELIHOOD )
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				for ( iterator iter = result.find_max_likelihood( *name_iter, result.preorder_begin( ) ).first; iter != result.end( ); iter = result.parent( iter ) )
				{
					if ( preprocessed_names.find( iter->name ) == preprocessed_names.end( ) && iter->likelihood.find( *name_iter ) != iter->likelihood.end( ) && iter->likelihood[ *name_iter ] > iter->likelihood[ iter->name ] )
					{
						iter->name = *name_iter;
					}
				}
			}
#elif defined( _SCREEN_OUT_CANDIDATE )
			for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				iter->candidate = integrity;
			}

			for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				if ( !result.is_root( iter ) && iter->name == result.reject_name( ) )
				{
					FeatureType feature; // 特徴ベクトル
					double max_likelihood = -DBL_MAX; // 最大尤度
			
					// 特徴量を抽出する
					result.extract_feature( feature, iter, result.reference_branch( ) );

					// 尤度を計算する
					LikelihoodType likelihood = likelihood_func( result.parent( iter )->name, feature ); //尤度

					for ( LikelihoodType::const_iterator likelihood_iter = likelihood.begin( ); likelihood_iter != likelihood.end( ); likelihood_iter++ )
					{
						if ( iter->candidate[ result.parent( iter )->name ].find( likelihood_iter->first ) != iter->candidate[ result.parent( iter )->name ].end( ) && max_likelihood < likelihood_iter->second )
						{
							iter->name = likelihood_iter->first;
							max_likelihood = likelihood_iter->second;
						}
					}
			
					// 候補をふるい落とす
					if ( iter->name != result.reject_name( ) )
					{
						// 分岐のとき
						if ( iter->name != result.parent( iter )->name )
						{
							for ( iterator iter2 = result.begin( ); iter2 != result.end( ); iter2++ )
							{
								if ( iter2 != iter )
								{
									for ( std::map< NameType, std::set< NameType > >::iterator candidate_iter = iter2->candidate.begin( ); candidate_iter != iter2->candidate.end( ); candidate_iter++ )
									{
										if ( candidate_iter->first != iter->name )
										{
											candidate_iter->second.erase( iter->name );
										}
									}
							
									if ( !result.is_ancestor( iter2, iter ) && !result.is_descendant( iter2, iter ) )
									{
										iter2->candidate[ iter->name ].erase( iter->name );
									}
								}
							}
						}
					}
					else
					{
						// 延長のとき
						for ( iterator sibling_iter = result.child_begin( result.parent( iter ) ); sibling_iter != result.child_end( result.parent( iter ) ); sibling_iter++ )
						{
							if ( iter != sibling_iter )
							{
								for ( iterator descendant_iter = result.subtree( sibling_iter ).begin( ); descendant_iter != result.subtree( sibling_iter ).end( ); descendant_iter++ )
								{
									descendant_iter->candidate[ iter->name ].erase( iter->name );
								}
							}
						}
					}
				}
			}
#elif defined( _FIND_BIFURCATION )
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				iterator max_branch = result.end( ); // 最大尤度を持つ枝
				double max_likelihood = 0; // 最大尤度

				for (  iterator iter = result.begin( ); iter != result.end( ); iter++ )
				{
					if ( !result.is_root( iter ) && iter->name == result.reject_name( ) )
					{
						for ( std::map< NameType, std::set< NameType > >::const_iterator integrity_iter = integrity.begin( ); integrity_iter != integrity.end( ); integrity_iter++ )
						{
							double ancestor_likelihood = 0; // 先祖枝の尤度

							for ( iterator ancestor_iter = result.parent( iter ); ancestor_iter != result.end( ); ancestor_iter = result.parent( ancestor_iter ) )
							{
								if ( ancestor_iter->likelihood[ integrity_iter->first ] > ancestor_likelihood )
								{
									ancestor_likelihood = ancestor_iter->likelihood[ integrity_iter->first ];
								}
							}
					
							if ( integrity_iter->first != *name_iter && integrity_iter->second.find( *name_iter ) != integrity_iter->second.end( ) && max_likelihood < ancestor_likelihood * iter->likelihood[ *name_iter ] )
							{
								max_branch = iter;
								max_likelihood = ancestor_likelihood * iter->likelihood[ *name_iter ];
							}
						}
					}
				}

				max_branch->name = *name_iter;
			}

			// 分岐を延長する
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
			}

#elif defined( _SELECT_MODEL )
			tree< NameType > model; // モデル
			const_iterator ref_branch = result.reference_branch( ); // 参照枝

			// 参照枝はルールベースで名前が対応付けられていると仮定する
			model.insert( model.end( ), ref_branch->name );

			// とりうる分岐パターンを列挙する
			for ( tree< NameType >::iterator model_iter = model.begin( ); model_iter != model.end( ); model_iter++ )
			{
				for ( std::set< NameType >::const_iterator integrity_iter = integrity[ *model_iter ].begin( ); integrity_iter != integrity[ *model_iter ].end( ); integrity_iter++ )
				{
					bool flag = true; // フラグ

					// 既に存在する名前は不可
					for ( tree< NameType >::const_iterator ancestor_iter = model_iter; ancestor_iter != model.end( ); ancestor_iter = model.parent( ancestor_iter ) )
					{
						if ( *ancestor_iter == *integrity_iter )
						{
							flag = false;
							break;
						}
					}

					// 名前を挿入する
					if ( flag )
					{
						model_iter = model.parent( model.insert( model_iter, *integrity_iter ) );
					}
				}
			}

			for ( tree< NameType >::const_iterator iter = model.begin( ); iter != model.end( ); iter++ )
			{
				for ( size_t i = 0; i < model.depth( iter ); i++ )
				{
					std::cout << ' ';
				}

				std::cout << to_string( *iter ) << std::endl;
			}

			// 参照枝を根とする部分木を対象とする
			result.select_model( std::find( result.preorder_begin( ), result.preorder_end( ), ref_branch->name ), model.preorder_begin( ), model );
#elif defined( _FIND_OPTIMUM_BRANCHING )
			// 最適分岐を計算する
			result.find_optimum_branching( combined_names, integrity );
#else
			// 最も高い尤度を持つ名前を対応付ける
			for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				LikelihoodType::const_iterator max_likelihood_iter = iter->likelihood.end( );

				for ( LikelihoodType::const_iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					if ( max_likelihood_iter == iter->likelihood.end( ) || max_likelihood_iter->second < likelihood_iter->second )
					{
						max_likelihood_iter = likelihood_iter;
					}
				}

				iter->name = max_likelihood_iter != iter->likelihood.end( ) ? max_likelihood_iter->first : reject_name( );
			}
#endif

#ifdef _CORRECT_ERROR
			// 誤り訂正を行う
			for ( std::set< NameType >::const_iterator iter = names.begin( ); iter != names.end( ); iter++ )
			{
			}
#endif //_CORRECT_ERROR

			// 後処理を行う
			result.labeling_postprocess( );
		}

		/// @brief 対応付け結果を評価する
		/// @param[out] confusion 結果を格納するコンフュージョンマトリックス
		/// @param[in] truth 正解木構造．親子関係が同一でなければならない
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::evaluate( Matrix &confusion, const Tree &truth )const
		{
			// 初期化する
			confusion.resize( name_number( ), name_number( ) );
			confusion.fill( 0 );

			// 評価する
			for ( const_iterator result_iter = begin( ), truth_iter = truth.begin( ); result_iter != end( ) && truth_iter != truth.end( ); result_iter++, truth_iter++ )
			{
#ifndef _EVALUATE_LIKELIHOOD_RANKING
#ifdef _EVALUATE_HEAD_ONLY
				bool exist = false; // 存在フラグ

				for ( const_iterator parent_iter = truth.parent( truth_iter ); parent_iter != truth.end( ); parent_iter = truth.parent( parent_iter ) )
				{
					if ( parent_iter->name == truth_iter->name )
					{
						exist = true;
						break;
					}
				}

				// 親に同じ名前が存在しない枝のみ評価する
				if ( !exist )
#endif //_EVALUATE_HEAD_ONLY
				{
					confusion( to_index( truth_iter->name ), to_index( result_iter->name ) ) += evaluate_result( result_iter, truth_iter );
				}
#else //_EVALUATE_LIKELIHOOD_RANKING
				struct Compare
				{
					bool operator( )( const LikelihoodType::value_type &l, const LikelihoodType::value_type &r )
					{
						return l.second > r.second;
					}
				};

				std::list< LikelihoodType::value_type > list;
				const size_t k_max = 2;
				size_t k = 0;

				if ( truth_iter->name == reject_name( ) )
				{
					continue;
				}

				for ( LikelihoodType::const_iterator iter = result_iter->likelihood.begin( ); iter != result_iter->likelihood.end( ); iter++ )
				{
					list.insert( list.end( ), *iter );
				}

				list.sort( Compare( ) );
		
				for ( std::list< LikelihoodType::value_type >::const_iterator iter = list.begin( ); iter != list.end( ) && k < k_max; k++ )
				{
					if ( truth_iter->name == iter->first )
					{
						confusion( to_index( truth_iter->name ), to_index( truth_iter->name ) ) += evaluate_result( result_iter, truth_iter );
						break;
					}

					iter++;
				}

				if ( k >= k_max )
				{
					confusion( to_index( truth_iter->name ), to_index( reject_name( ) ) ) += evaluate_result( result_iter, truth_iter );
				}
#endif //_EVALUATE_LIKELIHOOD_RANKING
			}
		}
			
		/// @brief 名前付き芯線画像を復元する
		/// @param[out] labeled_centerline 名前付き芯線画像を格納する画像
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::restore( LabelImage &labeled_centerline )const
		{
			// 初期化する
			labeled_centerline.resize( siz_.x, siz_.y, siz_.z );
			labeled_centerline.reso( pitch_.x, pitch_.y, pitch_.z );
			labeled_centerline.fill( 0 );
		
			// 復元する
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				restore_centerline( labeled_centerline, iter );
			}
		}
		
		/// @brief 名前付き管腔臓器画像を復元する
		/// @param[in] tubular 管腔臓器領域画像
		/// @param[out] labeled_tubular 名前付き管腔臓器画像を格納する画像
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::restore( const MarkImage &tubular, LabelImage &labeled_tubular )const
		{
			std::list< Point > voxels; // 復元された画素

			// 芯線を復元する
			restore( labeled_tubular );

			// 分岐点を復元する
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( !is_leaf( iter ) )
				{
					restore_bifurcation( tubular, labeled_tubular, iter );
				}
			}

			// 復元された画素を求める
			for ( size_t k = 0; k < labeled_tubular.depth( ); k++ )
			{
				for ( size_t j = 0; j < labeled_tubular.height( ); j++ )
				{
					for ( size_t i = 0; i < labeled_tubular.width( ); i++ )
					{
						if ( labeled_tubular( i, j, k ) )
						{
							voxels.insert( voxels.end( ), Point( i, j, k ) );
						}
					}
				}
			}

			// 復元する
			while ( !voxels.empty( ) )
			{
				for ( std::list< Point >::iterator iter = voxels.begin( ); iter != voxels.end( ); iter = voxels.erase( iter ) )
				{
					for ( size_t i = 0; i < TOPOLOGY; i++ )
					{
						try
						{
							Point pt = *iter + TOPOLOGICAL( neighborhood )[ i ];
						
							if ( tubular( pt ) && !labeled_tubular( pt ) )
							{
								labeled_tubular( pt ) = labeled_tubular( *iter );
								voxels.insert( voxels.end( ), pt );
							}
						}

						catch ( MarkImage::InvalidPoint ) 
						{
						}
					}
				}
			}
		}

		/// @brief ファイルから木構造を読み込む
		/// @param[in] file_name 読み込むXMLファイル名
		/// @param[in] siz 画像のサイズ
		/// @param[in] pitch 画像のピッチ
		/// @retval true 木構造の読み込みに成功した
		/// @retval false 木構造の読み込みに失敗した．恐らく，ファイルを開けなかったかまたはファイルの内容が不正だった
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline bool Tree< ValueType, NameType, LikelihoodType, FeatureType >::read( const char *file_name, const SizeSet &siz, const PitchSet &pitch )
		{
			XML xml; // XML
			
			// XMLを読み込む
			if ( !xml.read( file_name ) )
			{
				return false;
			}
			
			// エラーチェック
			if ( xml.empty( ) || xml.root( ).tag( ).name( ) != "VESSEL" )
			{
				return false;
			}

			// サイズとピッチを設定する
			siz_ = siz;
			pitch_ = pitch;

			// 木構造を空にする
			clear( );
			
			// XMLから木構造を読み込む
			if ( read( xml, "1", insert( begin( ), Branch( ) ) ) )
			{
				return true;
			}
			else
			{
				clear( );

				return false;
			}
		}

		/// @brief ファイルへ木構造を書き込む
		/// @param[in] file_name 書き込むXMLファイル名
		/// @retval true 木構造の書き込みに成功した
		/// @retval false 木構造の書き込みに失敗した．恐らく，ファイルを開けなかった
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline bool Tree< ValueType, NameType, LikelihoodType, FeatureType >::write( const char *file_name )const
		{
			XML xml; // XML

			// ルート要素を追加する
			xml.insert( xml.begin( ), XML::Element( "VESSEL" ) );

			// 各枝を書き込む
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				XML::iterator branch_elem = xml.insert( xml.end( ), XML::Element( "BRANCH" ) ); // BRANCH要素

				// 属性を書き込む
				branch_elem->tag( ).insert( branch_elem->tag( ).end( ), XML::Element::Tag::value_type( "id", std::to_string( iter->id ) ) );
				branch_elem->tag( ).insert( branch_elem->tag( ).end( ), XML::Element::Tag::value_type( "name", to_string( iter->name ) ) );

				// 親を書き込む
				if( !is_root( iter ) )
				{
					XML::iterator parent_elem = xml.insert( branch_elem, XML::Element( "PARENT" ) ); // PARENT要素

					branch_elem = xml.parent( parent_elem );
					parent_elem->tag( ).insert( parent_elem->tag( ).end( ), XML::Element::Tag::value_type( "id", std::to_string( parent( iter )->id ) ) );
				}

				// 子供を書き込む
				for ( const_iterator child_iter = child_begin( iter ); child_iter != child_end( iter ); child_iter++ )
				{
					XML::iterator child_elem = xml.insert( branch_elem, XML::Element( "CHILD" ) ); // CHILD要素

					branch_elem = xml.parent( child_elem );
					child_elem->tag( ).insert( child_elem->tag( ).end( ), XML::Element::Tag::value_type( "id", std::to_string( child_iter->id ) ) );
				}

				// 枝を書き込む
				if ( !write_branch( xml, branch_elem, iter ) )
				{
					return false;
				}
			}

			return xml.write( file_name );
		}

		/// @brief 画像のサイズを取得する
		/// @return 画像のサイズの組
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline const SizeSet &Tree< ValueType, NameType, LikelihoodType, FeatureType >::get_size( )const
		{
			return siz_;
		}

		/// @brief 画像のサイズを設定する
		/// @param[in] siz 設定する画像のサイズの組
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::set_size( const SizeSet &siz )
		{
			siz_ = siz;
		}

		/// @brief 画像のピッチを取得する
		/// @return 画像のピッチの組
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline const PitchSet &Tree< ValueType, NameType, LikelihoodType, FeatureType >::get_pitch( )const
		{
			return pitch_;
		}

		/// @brief 画像のピッチを設定する
		/// @param[in] pitch 設定する画像のピッチの組
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::set_pitch( const PitchSet &pitch )
		{
			pitch_ = pitch;
		}
	
		/// @brief 木構造構築の後処理を行う仮想関数
		/// @attention デフォルトの動作はないが，オーバーライドすることで機能を拡張することができる
		/// @attention この関数が呼ばれる時点で，枝の識別子には無効な値が格納されている
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::construct_postprocess( )
		{
		}
	
		/// @brief 対応付けの前処理を行う仮想関数
		/// @attention デフォルトの動作はないが，オーバーライドすることで機能を拡張することができる
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::labeling_preprocess( )
		{
		}
	
		/// @brief 対応付けの後処理を行う仮想関数
		/// @attention デフォルトの動作はないが，オーバーライドすることで機能を拡張することができる
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::labeling_postprocess( )
		{
		}

#ifdef _FIND_MAX_LIKELIHOOD
		/// @brief 尤度が最大となる経路を探索しその終端枝と経路上の最大尤度の組を返す
		/// @param[in] name 注目する名前
		/// @param[in] branch 探索する部分木の根を指すイテレータ
		/// @return 尤度が最大となる経路の終端枝と経路上の最大尤度の組
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline std::pair< typename Tree< ValueType, NameType, LikelihoodType, FeatureType >::iterator, double > Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_max_likelihood( const NameType &name, iterator branch )
		{
			std::pair< iterator, double > max_likelihood( branch, 0 ); // 尤度が最大となる経路の終端枝と経路上の最大尤度の組

			// 子供を探索する
			for ( iterator iter = child_begin( branch ); iter != child_end( branch ); iter++ )
			{
				std::pair< iterator, double > temp = find_max_likelihood( name, iter ); // 一時変数

				if ( max_likelihood.second < temp.second )
				{
					max_likelihood = temp;
				}
			}

			// 最大尤度を更新する
			if ( branch->likelihood.find( name ) != branch->likelihood.end( ) && max_likelihood.second < branch->likelihood[ name ] )
			{
				max_likelihood.second = branch->likelihood[ name ];
			}

			return max_likelihood;
		}
#endif //_FIND_MAX_LIKELIHOOD

#ifdef _SELECT_MODEL
		/// @brief モデル選択に基づく対応付けを行う
		/// @param[inout] branch 対応付けを行う枝を指すイテレータ
		/// @param[in] selected_model 現在選択されているモデルを指すイテレータ
		/// @param[in] model モデル木構造
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::select_model( iterator branch, typename tree< NameType >::const_iterator selected_model, const tree< NameType > &model )
		{
			tree< NameType >::const_iterator max_model = selected_model; // 最大スコアを持つモデル
			double max_score = -DBL_MAX; // 最大スコア
			double ancestor_score = calc_ancestor_score( branch, *selected_model ); // 先祖スコア

			// 分岐スコアを検査する
			for ( tree< NameType >::const_iterator child_iter = model.child_begin( selected_model ); child_iter != model.child_end( selected_model ); child_iter++ )
			{
				double descendant_score = find_max_descendant_score( branch, child_iter, model ); // 子孫スコア
				double self_score = calc_self_score( branch, *child_iter ); // 自身スコア
						
				if ( max_score < ( ancestor_score + descendant_score ) * self_score )
				{
					max_model = child_iter;
					max_score = ( ancestor_score + descendant_score ) * self_score;
				}
			}
	
			// 延長スコアを検査する
			{
				double descendant_score = find_max_descendant_score( branch, selected_model, model ); // 子孫スコア
				double self_score = calc_self_score( branch, *selected_model ); // 自身スコア

				if ( max_score < descendant_score * self_score )
				{
					max_model = selected_model;
				}
			}

			// 対応付けを行う
			if ( branch->name == reject_name( ) )
			{
				branch->name = *max_model;
			}

			// 子枝の対応付けを行う
			for ( iterator child_iter = child_begin( branch ); child_iter != child_end( branch ); child_iter++ )
			{
				select_model( child_iter, max_model, model );
			}
		}
	
		/// @brief 最大の子孫スコアを求める
		/// @param[in] branch 
		/// @param[in] selected_model 
		/// @param[in] model 
		/// @return 最大の子孫スコア
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_max_descendant_score( iterator branch, typename tree< NameType >::const_iterator selected_model, const tree< NameType > &model )
		{
			double max_score = model.is_leaf( selected_model ) ? 1 : 0; // 最大スコア

			// 子供を探索する
			for ( tree< NameType >::const_iterator child_iter = model.child_begin( selected_model ); child_iter != model.child_end( selected_model ); child_iter++ )
			{	
				double score = find_max_descendant_score( branch, child_iter, model ); // 子孫の最大スコア
				double descendant_score = calc_descendant_score( branch, *child_iter ); // スコア

				if ( max_score < score + descendant_score )
				{
					max_score = score + descendant_score;
				}
			}

			return max_score;
		}

		/// @brief 指定の枝の指定の名前に関する自身スコアを計算する
		/// @param[in] branch スコアを計算する枝を指すイテレータ
		/// @param[in] name スコアを計算する名前
		/// @return 自身スコア
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_self_score( iterator branch, NameType name )
		{
			double pos = 0, neg = 0; // スコア

			for ( iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( iter == branch )
				{
					pos += iter->likelihood[ name ];
				}
				else if ( !is_ancestor( iter, branch ) && !is_descendant( iter, branch ) )
				{
					//neg += iter->likelihood[ name ];
				}
			}
	
			return pos * ( 1 - neg );
		}

		/// @brief 指定の枝の指定の名前に関する先祖スコアを計算する
		/// @param[in] branch スコアを計算する枝を指すイテレータ
		/// @param[in] name スコアを計算する名前
		/// @return 先祖スコア
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_ancestor_score( iterator branch, NameType name )
		{
			double pos = 0, neg = 0; //スコア

			for ( iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( is_ancestor( iter, branch ) )
				{
					pos += iter->likelihood[ name ];
				}
				else if ( iter != branch && !is_root( branch ) && !is_descendant( iter, parent( branch ) ) )
				{
					neg += iter->likelihood[ name ];
				}
			}
	
			return pos * ( 1 - neg );
		}

		/// @brief 指定の枝の指定の名前に関する子孫スコアを計算する
		/// @param[in] branch スコアを計算する枝を指すイテレータ
		/// @param[in] name スコアを計算する名前
		/// @return 子孫スコア
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_descendant_score( iterator branch, NameType name )
		{
			double pos = 0, neg = 0; //スコア

			for ( iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( is_descendant( iter, branch ) )
				{
					pos += iter->likelihood[ name ];
				}
				else if ( iter != branch && !is_ancestor( iter, branch ) )
				{
					neg += iter->likelihood[ name ];
				}
			}
	
			return pos * ( 1 - neg );
		}
#endif //_SELECT_MODEL
		
#ifdef _FIND_OPTIMUM_BRANCHING				
		/// @brief 分岐表を計算する
		/// @param[in,out] branch 分岐表を計算する枝
		/// @param[in] names 名前の集合
		/// @param[in] integrity 整合性
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_table( iterator branch, std::set< NameType > names, std::map< NameType, std::set< NameType > > &integrity )
		{
			// 各名前について分岐表を計算する
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				Branch::BifurcationType temp_bifurcation; // 分岐を一時的に格納する領域
				std::set< Branch::BifurcationType > bifurcation_candidates; // 分岐の候補
				std::set< Branch::BifurcationType >::iterator max_bifurcation; // スコアを最大化する分岐
				double max_score = 0; // 最大スコア

				// リジェクト名は計算しない
				if ( *name_iter == reject_name( ) )
				{
					continue;
				}
				
				// 分岐を計算する
				for ( iterator iter = subtree( branch ).begin( ); iter != subtree( branch ).end( ); iter++ )
				{
					bifurcation_candidates.insert( calc_bifurcation( branch, iter, *name_iter, integrity, temp_bifurcation ) );
				}
				
				// スコアを最大化する分岐を挿入する
				max_bifurcation = bifurcation_candidates.begin( );
				
				for ( std::set< Branch::BifurcationType >::iterator bifurcation_iter = bifurcation_candidates.begin( ); bifurcation_iter != bifurcation_candidates.end( ); bifurcation_iter++ )
				{
					double score = calc_score( *bifurcation_iter ); // スコア

					if ( max_score < score )
					{
						max_bifurcation = bifurcation_iter;
						max_score = score;
					}
				}

				branch->bifurcation_table.insert( Branch::BifurcationTableType::value_type( *name_iter, *max_bifurcation ) );
			}
		}
			
		/// @brief 指定の血管が指定の枝から指定の子供の枝に延長する場合の分岐を求める
		/// @param[in] branch 分岐を求める枝
		/// @param[in] descendant_branch 延長する子供の枝
		/// @param[in] name 延長する血管
		/// @param[in] integrity 可能な分岐
		/// @param[out] descendant 分岐を格納するコンテナ
		/// @return 指定のコンテナの参照
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline typename Tree< ValueType, NameType, LikelihoodType, FeatureType >::Branch::BifurcationType &Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_bifurcation( iterator branch, iterator descendant_branch, NameType name, std::map< NameType, std::set< NameType > > &integrity, typename Branch::BifurcationType &bifurcation )
		{
			// ソートのための比較関数
			struct SortPred
			{
				// 関数呼び出し演算子
				bool operator( )( const std::tr1::tuple< NameType, size_t, double > &l, const std::tr1::tuple< NameType, size_t, double > &r )
				{
					return std::get< 2 >( l ) > std::get< 2 >( r );
				}
			};

			// 削除のための比較関数
			class RemovePred
			{
				NameType name_; // 削除する名前
				size_t id_; // 削除する識別子
			public:
				// コンストラクタ
				RemovePred( NameType name, size_t id ) : name_( name ), id_( id )
				{
				}

				//　関数呼び出し演算子
				bool operator( )( const std::tr1::tuple< NameType, size_t, double > &a )
				{
					return std::get< 0 >( a ) == name_ || std::get< 1 >( a ) == id_;
				}
			};

			std::list< std::tr1::tuple< NameType, size_t, double > > scores; // スコア
			
			// 分岐表を初期化する
			bifurcation.clear( );

			// 終端スコアを求める
			bifurcation.insert( Branch::BifurcationType::value_type( name, std::make_pair( descendant_branch->id, calc_tl( descendant_branch, name ) ) ) );
			
			// 分岐スコアを求める
			for ( std::set< NameType >::const_iterator integrity_iter = integrity[ name ].begin( ); integrity_iter != integrity[ name ].end( ); integrity_iter++ )
			{
				if ( *integrity_iter != name && *integrity_iter != reject_name( ) )
				{
					for ( iterator iter = descendant_branch, prev_iter = end( ); iter != parent( branch ); iter = parent( prev_iter = iter ) )
					{
						for ( iterator child_iter = child_begin( iter ); child_iter != child_end( iter ); child_iter++ )
						{
							if ( child_iter != prev_iter )
							{
								scores.push_front( std::tr1::make_tuple( *integrity_iter, child_iter->id, calc_hd( child_iter, *integrity_iter, name ) ) );
							}
						}
					}
				}
			}

			scores.sort( SortPred( ) );

			while ( !scores.empty( ) )
			{
				bifurcation.insert( Branch::BifurcationType::value_type( std::get< 0 >( scores.front( ) ), std::make_pair( std::get< 1 >( scores.front( ) ), std::get< 2 >( scores.front( ) ) ) ) );
				scores.remove_if( RemovePred( std::get< 0 >( scores.front( ) ), std::get< 1 >( scores.front( ) ) ) );
			}

			return bifurcation;
		}
		
		/// @brief 指定の枝が指定の血管の終端枝となるスコアを計算する
		/// @param[in] branch スコアを計算する枝
		/// @param[in] name スコアを計算する血管
		/// @return 指定の枝が指定の血管の終端枝となるスコア
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_tl( iterator branch, NameType name )
		{
#ifdef _IGNORE_TAIL_SCORE
			return 0;
#endif //_IGNORE_TAIL_SCORE

			if ( branch->name == reject_name( ) )
			{
				double ext_max = 0; // 延長スコアの最大
					
				for ( iterator child_iter = child_begin( branch ); child_iter != child_end( branch ); child_iter++ )
				{
					bool ext_flag = true;

					for ( LikelihoodType::iterator likelihood_iter = child_iter->plausibility[ name ].begin( ); likelihood_iter != child_iter->plausibility[ name ].end( ); likelihood_iter++ )
					{
						if ( child_iter->plausibility[ name ][ name ] < likelihood_iter->second )
						{
							ext_flag = false;
							break;
						}
					}

					if ( ext_flag )
					{
						ext_max = std::max( ext_max, child_iter->plausibility[ name ][ name ] );
					}
				}

				return branch->likelihood[ name ] * ( 1 - ext_max );
			}
			else if ( branch->name == name )
			{
				for ( iterator child_iter = child_begin( branch ); child_iter != child_end( branch ); child_iter++ )
				{
					if ( child_iter->name == name )
					{
						return 0;
					}
				}

				return 1;
			}
			else
			{
				return 0;
			}
		}
			
		/// @brief 指定の枝が指定の血管の開始枝となるスコアを計算する
		/// @param[in] branch スコアを計算する枝
		/// @param[in] name スコアを計算する血管
		/// @param[in] parent_name 指定の枝の親に付与される血管名
		/// @return 指定の枝が指定の血管の開始枝となるスコア
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_hd( iterator branch, NameType name, NameType parent_name )
		{
			if ( branch->name == reject_name( ) )
			{
				return ( is_root( branch ) ? 1 : parent( branch )->likelihood[ parent_name ] ) * branch->plausibility[ parent_name ][ name ] * calc_score( branch->bifurcation_table[ name ] );
			}
			else if ( branch->name == name )
			{
				return is_root( branch ) || parent( branch )->name == parent_name ? 1 : 0;
			}
			else
			{
				return 0;
			}
		}
		
		/// @brief 分岐のスコアを計算する
		/// @param[in] スコアを計算する分岐
		/// @return 指定の分岐のスコア
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_score( const typename Branch::BifurcationType &bifurcation )
		{
			double score = 0;

			for ( Branch::BifurcationType::const_iterator iter = bifurcation.begin( ); iter != bifurcation.end( ); iter++ )
			{
				score += iter->second.second;
			}

			return score;
		}
			
		/// @brief 最適分岐を求めることによるラベリングを行う
		/// @param[in] names 名前の集合
		/// @param[in] integrity 整合性
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_optimum_branching( std::set< NameType > names, std::map< NameType, std::set< NameType > > &integrity )
		{
			// 各枝の尤度を正規化する
			for ( iterator iter = begin( ); iter != end( ); iter++ )
			{
				double likelihood_sum = 0; // 尤度の総和
	
				for ( LikelihoodType::const_iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_sum += likelihood_iter->second;
				}
		
				for ( LikelihoodType::iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_iter->second /= likelihood_sum > 0 ? likelihood_sum : 1;
				}
			}

			// 分岐表を計算する
			for ( iterator iter = postorder_begin( ); iter != postorder_end( ); iter++ )
			{
				calc_table( iter, names, integrity );
			}

			// 分岐グラフを求める
			tree< std::pair< NameType, size_t > > bifurcation_tree; // 分岐ツリー
			Branch::BifurcationTableType bifurcation_graph, optimum_branching; // 分岐グラフ
			const_iterator ref_branch = reference_branch( ); // 参照枝

			// 参照枝は対応対象部分木の根でありルールベースで名前が対応付けられていると仮定し分岐ツリーの根を挿入する
			bifurcation_tree.insert( bifurcation_tree.end( ), std::make_pair( ref_branch->name, ref_branch->id ) );
			
			// とりうる分岐パターンを列挙する
			for ( tree< std::pair< NameType, size_t > >::iterator tree_iter = bifurcation_tree.preorder_begin( ); tree_iter != bifurcation_tree.preorder_end( ); tree_iter++ )
			{
				iterator start_branch = std::find( begin( ), end( ), tree_iter->second ); // 現在注目している血管の開始枝

				// 分岐を挿入する
				for ( std::set< NameType >::const_iterator integrity_iter = integrity[ tree_iter->first ].begin( ); integrity_iter != integrity[ tree_iter->first ].end( ); integrity_iter++ )
				{
					bool flag = true; // フラグ

					// リジェクト名は挿入しない
					if ( *integrity_iter == reject_name( ) )
					{
						continue;
					}

					// 既に存在する名前は挿入しない
					for ( tree< std::pair< NameType, size_t > >::const_iterator ancestor_iter = tree_iter; ancestor_iter != bifurcation_tree.end( ); ancestor_iter = bifurcation_tree.parent( ancestor_iter ) )
					{
						if ( ancestor_iter->first == *integrity_iter )
						{
							flag = false;
							break;
						}
					}

					// 名前を挿入する
					if ( flag )
					{
						Branch::BifurcationType &bifurcation = start_branch->bifurcation_table[ tree_iter->first ];

						if ( bifurcation.find( *integrity_iter ) != bifurcation.end( ) )
						{
							tree_iter = bifurcation_tree.parent( bifurcation_tree.insert( tree_iter, std::make_pair( *integrity_iter, bifurcation[ *integrity_iter ].first ) ) );
						}
					}
				}
			}

			// 分岐グラフを求める
			for ( std::map< NameType, std::set< NameType > >::const_iterator integrity_parent_iter = integrity.begin( ); integrity_parent_iter != integrity.end( ); integrity_parent_iter++ )
			{
				Branch::BifurcationType bifurcation; // 分岐

				// リジェクト名はグラフに含めない
				if ( integrity_parent_iter->first == reject_name( ) )
				{
					continue;
				}

				for ( std::set< NameType >::const_iterator integrity_child_iter = integrity_parent_iter->second.begin( ); integrity_child_iter != integrity_parent_iter->second.end( ); integrity_child_iter++ )
				{
					std::set< std::pair< double, double > > scores; // スコアの集合
					double wt_sum = 0; // 重みの総和
					double score_sum = 0; // スコアの加重和

					// リジェクト名と自己ループはグラフに含めない
					if ( *integrity_child_iter == reject_name( ) || integrity_parent_iter->first == *integrity_child_iter )
					{
						continue;
					}

					// 対象の分岐パターンを持つノードについてスコアを取得する
					for ( tree< std::pair< NameType, size_t > >::const_iterator tree_iter = bifurcation_tree.begin( ); tree_iter != bifurcation_tree.end( ); tree_iter++ )
					{
						if ( !bifurcation_tree.is_root( tree_iter ) && bifurcation_tree.parent( tree_iter )->first == integrity_parent_iter->first && tree_iter->first == *integrity_child_iter )
						{
							double wt = bifurcation_tree.depth( tree_iter ) < 2 ? 1.0 : std::find( begin( ), end( ), bifurcation_tree.parent( tree_iter )->second )->plausibility[ bifurcation_tree.parent( bifurcation_tree.parent( tree_iter ) )->first ][ integrity_parent_iter->first ]; // 重み
							double score = std::find( begin( ), end( ), bifurcation_tree.parent( tree_iter )->second )->bifurcation_table[ integrity_parent_iter->first ][ *integrity_child_iter ].second; // スコア

							scores.insert( std::make_pair( wt, score ) );
						}
					}

					// 重みを正規化する
					for ( std::set< std::pair< double, double > >::const_iterator score_iter = scores.begin( );score_iter != scores.end( ); score_iter++ )
					{
						wt_sum += score_iter->first;
					}

					// スコアの加重和を求める
					for ( std::set< std::pair< double, double > >::const_iterator score_iter = scores.begin( );score_iter != scores.end( ); score_iter++ )
					{
						score_sum += score_iter->second;// score_iter->first * score_iter->second / ( wt_sum > 0 ? wt_sum : 1 );
					}

					// 分岐を挿入する
					bifurcation.insert( bifurcation.end( ), Branch::BifurcationType::value_type( *integrity_child_iter, std::make_pair( 0, score_sum ) ) );
				}

				bifurcation_graph.insert( bifurcation_graph.end( ), Branch::BifurcationTableType::value_type( integrity_parent_iter->first, bifurcation ) );
			}

			// 分岐グラフの最適分岐を求める
			find_optimum_branching( bifurcation_graph, optimum_branching );

			// 対応付けを行う
			std::stack< iterator > stack; // 対応付けを行う枝のスタック

			stack.push( std::find( begin( ), end( ), ref_branch->id ) );

			while ( !stack.empty( ) )
			{
				iterator tgt_branch = stack.top( ); // 対応付けを行う血管の最初の枝

				// スタックから枝をポップする
				stack.pop( );

				// 対応付けを行う
				for ( iterator iter = std::find( begin( ), end( ), tgt_branch->bifurcation_table[ tgt_branch->name ][ tgt_branch->name ].first ); iter != parent( tgt_branch ); iter = parent( iter ) )
				{
					iter->name = tgt_branch->name;
				}

				// 対応付けた血管から分岐する血管をプッシュする
				for ( Branch::BifurcationType::const_iterator bifurcation_iter = optimum_branching[ tgt_branch->name ].begin( ); bifurcation_iter != optimum_branching[ tgt_branch->name ].end( ); bifurcation_iter++ )
				{
					if ( tgt_branch->bifurcation_table[ tgt_branch->name ].find( bifurcation_iter->first ) != tgt_branch->bifurcation_table[ tgt_branch->name ].end( ) )
					{
						iterator next_branch = std::find( begin( ), end( ), tgt_branch->bifurcation_table[ tgt_branch->name ][ bifurcation_iter->first ].first ); // 次に対応付けを行う枝

						next_branch->name = bifurcation_iter->first;
						stack.push( next_branch );
					}
				}
			}

			// 延長する
#ifdef _IGNORE_TAIL_SCORE
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				std::stack< iterator > stack; // 枝のスタック

				// 枝をスタックに積む
				for ( iterator iter = find_max_plausibility( *name_iter, std::find( postorder_begin( ), postorder_end( ), *name_iter ) ).first; iter != end( ) && iter->name == reject_name( ); iter = parent( iter ) )
				{
					stack.push( iter );
				}

				// 血管を延長する
				for ( ; !stack.empty( ); stack.pop( ) )
				{
					bool cont = !is_root( stack.top( ) ) && parent( stack.top( ) )->name == *name_iter; // 継続フラグ

					for ( LikelihoodType::iterator plausibility_iter = stack.top( )->plausibility[ *name_iter ].begin( ); plausibility_iter != stack.top( )->plausibility[ *name_iter ].end( ); plausibility_iter++ )
					{
						if ( stack.top( )->plausibility[ *name_iter ][ *name_iter ] < plausibility_iter->second )
						{
							cont = false;
							break;
						}
					}

					if ( cont )
					{
						stack.top( )->name = *name_iter;
					}
					else
					{
						break;
					}
				}
			}
#endif //_IGNORE_TAIL_SCORE
		}
			
		/// @brief 最適分岐を求める
		/// @param[in] directed_graph 最適分岐を求める有向グラフ
		/// @param[out] optimum_branching 指定のグラフの最適分岐
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_optimum_branching( const typename Branch::BifurcationTableType &directed_graph, typename Branch::BifurcationTableType &optimum_branching )const
		{
			mist::matrix< double > graph_mat( name_number( ), name_number( ) ), branching_mat; // 隣接行列

			// 分岐グラフから隣接行列を作成する
			graph_mat.fill( -1 );

			for ( Branch::BifurcationTableType::const_iterator parent_iter = directed_graph.begin( ); parent_iter != directed_graph.end( ); parent_iter++ )
			{
				for ( Branch::BifurcationType::const_iterator child_iter = parent_iter->second.begin( ); child_iter != parent_iter->second.end( ); child_iter++ )
				{
					graph_mat( to_index( parent_iter->first ), to_index( child_iter->first ) ) = child_iter->second.second;
				}
			}

			// エドモンズ法を適用する
			edmonds( graph_mat, branching_mat, std::greater< double >( ) );

			// 隣接行列から最適分岐を作成する
			optimum_branching.clear( );

			for ( Branch::BifurcationTableType::const_iterator parent_iter = directed_graph.begin( ); parent_iter != directed_graph.end( ); parent_iter++ )
			{
				Branch::BifurcationType bifurcation; // 分岐

				for ( Branch::BifurcationType::const_iterator child_iter = parent_iter->second.begin( ); child_iter != parent_iter->second.end( ); child_iter++ )
				{
					if ( branching_mat( to_index( parent_iter->first ), to_index( child_iter->first ) ) >= 0 )
					{
						bifurcation.insert( *child_iter );
					}
				}

				optimum_branching.insert( Branch::BifurcationTableType::value_type( parent_iter->first, bifurcation ) );
			}
		}
		
#ifdef _IGNORE_TAIL_SCORE
		/// @brief 尤度が最大となる経路を探索しその終端枝と経路上の最大尤度の組を返す
		/// @param[in] name 注目する名前
		/// @param[in] branch 探索する部分木の根を指すイテレータ
		/// @return 尤度が最大となる経路の終端枝と経路上の最大尤度の組
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline std::pair< typename Tree< ValueType, NameType, LikelihoodType, FeatureType >::iterator, double > Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_max_plausibility( const NameType &name, iterator branch )
		{
			std::pair< iterator, double > max_plausibility( branch, 0 ); // 尤度が最大となる経路の終端枝と経路上の最大尤度の組

			// 子供を探索する
			for ( iterator iter = child_begin( branch ); iter != child_end( branch ); iter++ )
			{
				std::pair< iterator, double > temp = find_max_plausibility( name, iter ); // 一時変数

				if ( max_plausibility.second < temp.second )
				{
					max_plausibility = temp;
				}
			}

			// 最大尤度を更新する
			if ( branch != end( ) && max_plausibility.second < branch->plausibility[ name ][ name ] )
			{
				max_plausibility.second = branch->plausibility[ name ][ name ];
			}

			return max_plausibility;
		}
#endif //_IGNORE_TAIL_SCORE
#endif //_FIND_OPTIMUM_BRANCHING

		/// @brief XMLから枝を読み込む
		/// @param[in] xml 読み込むXML
		/// @param[in] id 読み込む枝の識別子
		/// @param[out] branch 読み込む枝を指すイテレータ
		/// @retval true 枝の読み込みに成功した
		/// @retval false 枝の読み込みに失敗した．恐らく，XMLの内容が不正だった
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline bool Tree< ValueType, NameType, LikelihoodType, FeatureType >::read( const XML &xml, const std::string &id, iterator branch )
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
				XML::const_iterator branch_elem = std::find_if( xml.child_begin( xml.preorder_begin( ) ), xml.child_end( xml.preorder_begin( ) ), FindElem( "BRANCH", id ) ); //BRANCH要素
				
				// エラーチェック
				if ( branch_elem == xml.child_end( xml.preorder_begin( ) ) )
				{
					return false;
				}
				
				// 属性を読み込む
				branch->id = std::stoi( branch_elem->tag( )[ "id" ] );
				branch->name = to_name( branch_elem->tag( )[ "name" ] );
				
				// 子供を読み込む
				for( XML::const_iterator child_iter = std::find( xml.child_begin( branch_elem ), xml.child_end( branch_elem ), "CHILD" ); child_iter != xml.child_end( branch_elem ); child_iter = std::find( ++child_iter, xml.child_end( branch_elem ), "CHILD" ) )
				{
					iterator child_branch = insert( branch, value_type( ) ); // 子供
						
					// 子供を読み込む
					branch = parent( child_branch );
			
					if ( !read( xml, child_iter->tag( )[ "id" ], child_branch ) )
					{
						return false;
					}
				}

				return read_branch( xml, branch_elem, branch );
			}

			catch ( XML::Element::Tag::InvalidProperty )
			{
				return false;
			}
		}
		
		/// @brief 枝が指定の識別子を持つかか判定する
		/// @param[in] id 判定する識別子
		/// @retval true 枝は指定の識別子を持つ
		/// @retval false 枝は指定の識別子を持たない
		template < class ValueType, class NameType, class LikelihoodType >
		inline bool _Branch< ValueType, NameType, LikelihoodType >::operator==( size_t id )const
		{
			return _Branch::id == id;
		}
		
		/// @brief 枝が指定の名前を持つかか判定する
		/// @param[in] name 判定する名前
		/// @retval true 枝は指定の名前を持つ
		/// @retval false 枝は指定の名前を持たない
		template < class ValueType, class NameType, class LikelihoodType >
		inline bool _Branch< ValueType, NameType, LikelihoodType >::operator==( const NameType &name )const
		{
			return _Branch::name == name;
		}
	}
}

#endif //_TMATSU_TUBULAR_H_