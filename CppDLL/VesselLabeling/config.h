/**
* @file config.h
* @brief 設定のヘッダファイル
* @version 3.0
* @author 名古屋大学 情報科学研究科 メディア科学専攻 森研究室   351203192   松﨑 哲朗
* @date 2013/10/18
*/

#ifndef _TMATSU_CONFIG_H_
#define _TMATSU_CONFIG_H_

#define _TARGET_HYPOGASTRIC ///< @brief 下腹部動脈を対象にする

#define _CALC_LIKELIHOOD ///< @brief 尤度計算を行う
#define _CALC_INTEGRITY ///< @brief 整合性を求める
//#define _UPDATE_LIKELIHOOD ///< @brief 尤度更新を行う
//#define _FIND_MAX_LIKELIHOOD ///< @brief 最大の尤度を持つ子孫を探索することにより対応付けを行う
//#define _SCREEN_OUT_CANDIDATE ///< @brief 候補のふるい落としにより対応付けを行う
//#define _FIND_BIFURCATION ///< @brief 分岐を探索することにより対応付けを行う
//#define _SELECT_MODEL ///< @brief モデル選択により対応付けを行う
#define _FIND_OPTIMUM_BRANCHING ///< @brief 最適分岐を求めることにより対応付けを行う
//#define _IGNORE_TAIL_SCORE ///< @brief 分岐グラフの重みを計算するとき終端スコアを無視する
//#define _CORRECT_ERROR ///< @brief 誤り訂正を行う

//#define _WRITE_LIKELIHOOD ///< @brief 尤度を出力する

//#define _EVALUATE_HEAD_ONLY ///< @brief 各血管の最初のみ評価する
//#define _EVALUATE_LIKELIHOOD_RANKING ///< @brief 尤度の上位に正解がある割合を評価する
//#define _EVALUATE_PATTERN ///< @brief 分岐パターンを識別できたか評価する
//#define _EVALUATE_EVERY_PATTERN ///< @brief パターンごとに評価する
//#define _EVALUATE_EVERY_CASE ///< @brief 症例ごとに評価する

#define _REPORT_IN_ENGLISH ///< @brief 診断レポートを英語で作成する

//#define _DEBUG_GRAPH ///< @brief グラフ処理に関するデバッグ情報を出力する

#define TOPOLOGY 6 ///< @brief トポロジ
#define LEARN_ITERATION 1000 ///< @brief 学習の繰り返し回数
#define PRUN_THRESHOLD 2 ///< @brief 枝刈りする枝の画素数
#define BOTTOM_THRESHOLD( dpt ) ( ( dpt ) * 0.9 ) ///< @brief 腸骨動脈の終端枝の候補とする枝の終点の最低のz座標
#define THICKNESS_THRESHOLD 2.0 ///< @brief 腸骨動脈存在判定のしきい値
#define SMV_THRESHOLD( avg ) ( avg ) ///< @brief 上腸間膜静脈の候補とする部分木の最低の枝数
#define LPV_THRESHOLD( avg ) ( avg ) ///< @brief 門脈左枝の候補とする部分木の最低の枝数
#define CROSS_THRESHOLD 2.0 ///< @brief 交差判定のしきい値

#endif //_TMATSU_CONFIG_H_

/**
* @mainpage 血管名対応付けプロジェクト
* @version 3.0
* @author 名古屋大学 情報科学研究科 メディア科学専攻 森研究室   351203192   松﨑 哲朗
* @date 2013/10/18
* 
* @section 使用するには
*
* @subsection プロパティの設定
* 
* MIST のインクルードディレクトリのパスを「追加のインクルードディレクトリ」に追加します @n
* zconf.h, zlib.h があるディレクトリのパスを「追加のインクルードディレクトリ」に追加します @n
* zlib.lib があるディレクトリのパスを「追加のライブラリディレクトリ」に追加します @n
* zlib.lib を「追加の依存ファイル」に追加します @n
*
* @subsection ヘッダのインクルード
* @code
* #include "artery.h" // 動脈の血管名対応付けを行う
* #include "portal.h" // 静脈の血管名対応付けを行う
* @endcode
* 
* @subsection 動脈の対応付けの例
* @code
* using namespace tmatsu;
* using namespace tmatsu::artery;
* 
* MarkImage vessel; // 血管のマーク画像
* ThicknessImage centerline; // 芯線画像
* LabelImage labeled_vessel; // 血管のラベル画像
* Tree tree; // 木構造
* Tree result; // 対応付け結果木構造
* Tree truth; // 正解木構造
* LikelihoodFunction likelihood_func; // 尤度関数
* Matrix confusion; // コンフュージョンマトリックス
* 
* // 血管のマーク画像を読み込む
* if ( !vessel.read( MarkImage::ProgressCallback( ), "\\\\TAKA2\\tmatsu\\ACC_g001\\vessel.uc_raw.gz" ) )
* {
*		std::exit( 1 );
* }
* 
* // 正解木構造を読み込む
* if ( !truth.read( "\\\\TAKA2\\tmatsu\\ACC_g001\\truth.xml", vessel.get_size( ). vessel.get_pitch( ) ) )
* {
*		std::exit( 1 );
* }
* 
* // 尤度関数を読み込む
* if ( !likelihood_func.read( "\\\\TAKA2\\tmatsu\\ACC_g001" ) )
* {
*		std::exit( 1 );
* }
* 
* // 芯線を抽出する
* vessel.thinning( centerline );
* 
* // 木構造を構築する
* tree.construct( centerline );
* 
* // 対応付けを行う
* tree.labeling( result, likelihood_func );
* 
* // 結果を評価する
* result.evaluate( confusion, truth );
* 
* // ラベル画像を復元する
* result.restore( vessel, labeled_vessel );
* 
* // 対応付け結果を書き込む
* if ( !result.write( "\\\\TAKA2\\tmatsu\\ACC_g001\\result.xml" ) )
* {
*		std::exit( 1 );
* }
* 
* // 江種形式で書き込む
* if ( !tree.write_tree_info( "\\\\TAKA2\\tmatsu\\ACC_g001\\tree_info.xml", centerline ) )
* {
*		std::exit( 1 );
* }
* 
* if ( !tree.write_centerline_info( "\\\\TAKA2\\tmatsu\\ACC_g001\\centerline_info.xml", centerline ) )
* {
*		std::exit( 1 );
* }
* 
* if ( !tree.write_overlay_info( "\\\\TAKA2\\tmatsu\\ACC_g001\\overlay_info.xml" ) )
* {
*		std::exit( 1 );
* }
* 
* // NewVES形式で書き込む
* if ( !tree.write_secant_points( "\\\\TAKA2\\tmatsu\\ACC_g001\\sec_pt.csv" ) )
* {
*		std::exit( 1 );
* }
* 
* // 復元結果を書き込む
* if ( !labeled_vessel.write( LabelImage::ProgressCallback( ), "\\\\TAKA2\\tmatsu\\ACC_g001\\labeled_vessel.uc_raw.gz" ) )
* {
*		std::exit( 1 );
* }
* @endcode
* 
* @subsection 静脈の学習の例
* @code
* using namespace tmatsu;
* using namespace tmatsu::portal;
* 
* std::list< FeatureVector > samples; // 学習サンプルリスト
* LikelihoodFunction likelihood_func; // 尤度関数
* 
* // 学習サンプルを読み込む
* for ( size_t i = 2; ; i++ )
* {
*		std::stringstream ss; // ストリングストリーム
*		std::string hdr_file_name; // ヘッダファイル名
*		std::string tree_file_name; // 木構造ファイル名
*		Header hdr; // ヘッダ
*		Tree tree; // 木構造
*		std::list< FeatureVector > features; // 特徴量
* 
*		ss << "\\\\TAKA2\\tmatsu\\ACC_g" << std::setw( 3 ) << std::setfill( '0' ) << i;
*		hdr_file_name = ss.str( ) + "\\vessel.uc_raw.gz.header";
*		tree_file_name = ss.str( ) + "\\tree.xml";
* 
*		// ヘッダファイルを読み込む
*		if ( !hdr.read( hdr_file_name.c_str( ) ) )
*		{
*			break;
*		}
*
*		// 木構造を読み込む
*		if ( !tree.read( tree_file_name.c_str( ), hdr.get_size( ), hdr.get_pitch( ) ) )
*		{
*			break;
*		}
* 
*		// 特徴量を抽出する
*		tree.extract( features );
*		samples.splice( samples.end( ), features );
* }
* 
* // 学習する
* likelihood_func.learn( samples );
* 
* // 尤度関数を書き込む
* if ( !likelihood_func.write( "\\\\TAKA2\\tmatsu\\ACC_g001" ) )
* {
*		std::exit( 1 );
* }
* @endcode
*
* @section 動作を変更するには
* tmatsu::tubular::Tree が木構造を扱うクラスの基底となっています @n
* 独自の処理を行うためには以下のメンバ関数を実装した派生クラスを作成します @n
* 各関数の詳細はリファレンスを参照してください
* @code
* tmatsu::tubular::Tree::root_point;
* tmatsu::tubular::Tree::traverse_branch;
* tmatsu::tubular::Tree::reference_branch;
* tmatsu::tubular::Tree::extract_feature;
* tmatsu::tubular::Tree::evaluate_result;
* tmatsu::tubular::Tree::restore_centerline;
* tmatsu::tubular::Tree::restore_bifurcation;
* tmatsu::tubular::Tree::read_branch;
* tmatsu::tubular::Tree::write_branch;
* tmatsu::tubular::Tree::reject_name;
* tmatsu::tubular::Tree::default_likelihood;
* tmatsu::tubular::Tree::name_number;
* tmatsu::tubular::Tree::to_string;
* tmatsu::tubular::Tree::to_index;
* tmatsu::tubular::Tree::to_name;
* @endcode
* また以下のメンバ関数を実装することで動作を拡張することができます @n
* 必要なければ未定義でも構いません
* @code
* tmatsu::tubular::Tree::construct_postprocess;
* tmatsu::tubular::Tree::labeling_preprocess;
* tmatsu::tubular::Tree::labeling_postprocess;
* @endcode
* tmatsu::vessel::Tree, tmatsu::artery::Tree, tmatsu::portal::Tree はこのクラスを継承しています @n
* またこのクラスは tmatsu::tree を基底としSTLコンテナのように扱うことができます
*/