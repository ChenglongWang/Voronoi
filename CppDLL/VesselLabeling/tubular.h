/**
* @file tubular.h
* @brief Çoí¼Ît¯Ìwb_t@C
* @version 3.0
* @author ¼Ã®åw îñÈw¤È fBAÈwêU X¤º   351203192   ¼ú± NN
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
#define TOPOLOGICAL( a ) a ## 26 ///< @brief g|Wðl¶µÚöêðtÁ·é
#elif TOPOLOGY == 6
#define TOPOLOGICAL( a ) a ## 6 ///< @brief g|Wðl¶µÚöêðtÁ·é
#else
#error
#endif

namespace tmatsu
{
	namespace tubular 
	{
		/// @brief ÇoíØ\¢Ì}ðµ¤^
		/// @tparam ValueType f[^^
		/// @tparam NameType ¼OÌ^
		/// @tparam LikeihoodType ÞxÌgÌ^
		template < class ValueType, class NameType, class LikelihoodType >
		struct _Branch
		{
			typedef ValueType ValueType; ///< @brief f[^^
			size_t id; ///< @brief ¯Êq
			NameType name; ///< @brief ¼O
			LikelihoodType likelihood; ///< @brief Þx

#ifdef _UPDATE_LIKELIHOOD
			LikelihoodType prev_likelihood; ///< @brief ¼OÌÞx
#endif //_UPDATE_LIKELIHOOD

#ifdef _SCREEN_OUT_CANDIDATE
			typedef std::map< NameType, std::set< NameType > > CandidateType; ///< @brief Ç¼óâð\·^
			CandidateType candidate; ///< @brief Ç¼óâ
#endif //_SCREEN_OUT_CANDIDATE

#ifdef _FIND_OPTIMUM_BRANCHING
			typedef std::map< NameType, LikelihoodType > PlausibilityType; ///< @brief e²ÆÌÞxð\·^
			typedef std::map< NameType, std::pair< size_t, double > > BifurcationType; ///< @brief ªòðµ¤^
			typedef std::map< NameType, BifurcationType > BifurcationTableType; ///< @brief ªò\ðµ¤^
			PlausibilityType plausibility; 
			BifurcationTableType bifurcation_table;
#endif //_FIND_OPTIMUM_BRANCHING

			ValueType value; ///< @brief f[^
			bool operator==( size_t id )const;
			bool operator==( const NameType &name )const;
		};

		/// @brief Çoíæðµ¤NX
		/// @tparam ValueType f[^^
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

		typedef Image< short > CTImage; ///< @brief CTæðµ¤^
		typedef Image< unsigned char > MarkImage; ///< @brief }[Næðµ¤^
		typedef Image< float > ThicknessImage; ///< @brief ¾³æðµ¤^
		typedef Image< unsigned char > LabelImage; ///< @brief xæðµ¤^

		/// @brief ÇoíØ\¢ðµ¤ÛNX
		/// @tparam ValueType f[^^
		/// @tparam NameType }¼Ì^
		/// @tparam LikeihoodType ÞxÌgÌ^D¼O©çÞxÖÌAzzñÆÝÈ·±ÆªÅ«CÜ½ÁZðs¤±ÆªÅ«È¯êÎÈçÈ¢
		/// @tparam FeatureType Á¥xNgÌ^
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		class Tree : public tree< _Branch< ValueType, NameType, LikelihoodType > >
		{
		public:
			typedef _Branch< ValueType, NameType, LikelihoodType > Branch; ///< @brief ÇoíØ\¢Ì}ðµ¤^
		private:
			SizeSet siz_; ///< @brief TCY
			PitchSet pitch_; ///< @brief sb`
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
			/// @brief Ø\¢ÌªÆÈéæfðæ¾·é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @param[in] centerline cüæ
			/// @return Ø\¢ÌªÆÈéæfÌÀW
			virtual Point root_point( const ThicknessImage &centerline )const = 0;
			virtual Point root_point(const ThicknessImage &centerline, Point seedPoint) const = 0;

			/// @brief cüð¸µ1{Ì}Ìf[^ðæ¾·é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @param[out] val æ¾·éf[^
			/// @param[in] centerline ¸·écüæD±ÌæÍ«·¦Äàæ¢
			/// @param[in] start_pt ¸Ìn_ÌÀW
			/// @return }ÌI_ÌÀW
			virtual Point traverse_branch( ValueType &val, ThicknessImage &centerline, const Point &start_pt )const = 0;

			/// @brief î}ðæ¾·é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @attention ±ÌÖÍÎt¯ªsíêéOÉàsíê½ãÉàÄÑo³êé
			/// @return î}ÌèQÆ
			virtual const_iterator reference_branch( )const = 0;
			
			/// @brief Á¥Êðo·é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @param[out] feature oµ½Á¥Êði[·éÌæ
			/// @param[in] branch Á¥Êðo·é}
			/// @param[in] ref_branch î}
			virtual void extract_feature( FeatureType &feature, const_iterator branch, const_iterator ref_branch )const = 0;
			
			/// @brief wèÌ}ÉÖ·éÎt¯ÊÌ]¿lðæ¾·é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @param[in] result_branch Ît¯ÊÌ}
			/// @param[in] truth_branch ³ðÌ}
			/// @return ]¿l
			virtual double evaluate_result( const_iterator result_branch, const_iterator truth_branch )const = 0;
			
			/// @brief ¼Ot«cüæð³·é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @param[out] labeled_centerline ¼Ot«cüæði[·éÌæ
			/// @param[in] branch ³·é}
			virtual void restore_centerline( LabelImage &labeled_centerline, const_iterator branch )const = 0;
			
			/// @brief ¼Ot«ÇoíæÌªò_ð³·é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @param[in] tubular ÇoíÌ}[Næ
			/// @param[out] labeled_tubular ¼Ot«Çoíæði[·éÌæ
			/// @param[in] branch ³·éªò_Ìe}
			virtual void restore_bifurcation( const MarkImage &tubular, LabelImage &labeled_tubular, const_iterator branch )const = 0;
			
			/// @brief XML©ç}ðÇÝÞ¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @param[in] xml ÇÝÞXML
			/// @param[in] elem ÇÝÞvfðw·Ce[^
			/// @param[out] branch ÇÝÞ}ðw·Ce[^
			/// @retval true }ÌÇÝÝÉ¬÷µ½
			/// @retval false }ÌÇÝÝÉ¸sµ½D±ÌÆ«CØ\¢ÌÇÝÝÍ~³êé
			virtual bool read_branch( const XML &xml, XML::const_iterator elem, iterator branch ) = 0;
			
			/// @brief XMLÉ}ð«Þ¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @param[in] xml «ÞXML
			/// @param[out] elem «Þvfðw·Ce[^
			/// @param[in] branch «Þ}ðw·Ce[^
			/// @retval true }Ì«ÝÉ¬÷µ½
			/// @retval false }Ì«ÝÉ¸sµ½D±ÌÆ«CØ\¢Ì«ÝÍ~³êé
			virtual bool write_branch( XML &xml, XML::iterator elem, const_iterator branch )const = 0;
			
			/// @brief WFNg¼ðæ¾·é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @return WFNg¼
			virtual NameType reject_name( )const = 0;
			
			/// @brief ªÌe}ÌÞxÆµÄµíêéÞxÌKèlðæ¾·é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @return ÞxÌKèl
			virtual LikelihoodType default_likelihood( )const = 0;
			
			/// @brief WFNg¼ðÜÞ¼OÌðæ¾·é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @return ¼OÌ
			virtual size_t name_number( )const = 0;
			
			/// @brief ¼Oð¶ñÉÏ··é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @param[in] name Ï··é¼O
			/// @return ¼Oð\·¶ñ
			virtual std::string to_string( const NameType &name )const = 0;
			
			/// @brief ¼OðCfbNXÉÏ··é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @attention e¼OðÏ·µ½Êª»ê¼ê0, 1, ..., name_number( ) - 1ÉÈçÈ¯êÎÈçÈ¢
			/// @param[in] name Ï··é¼O
			/// @return ¼OÉÎ·éCfbNX
			virtual size_t to_index( const NameType &name )const = 0;
			
			/// @brief ¶ñð¼OÉÏ··é¼zÖ
			/// @attention ±ÌNXðÀÌ»·é½ßÉÍCh¶NXðì¬µC±ÌÖðÀµÈ¯êÎÈçÈ¢
			/// @param[in] str Ï··é¶ñ
			/// @return ¶ñÌ\·¼O
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

		/// @brief ®ì³µ
		template < class ValueType >
		inline Image< ValueType >::Image( )
		{
		}
	
		/// @brief æð¡»·é
		/// @param[in] img ¡»·éæ
		template < class ValueType >
		inline Image< ValueType >::Image( const Image &img ) : tmatsu::Image< ValueType >( img )
		{
		}

		/// @brief ^ÌÙÈéæð¡»·é
		/// @param[in] img ¡»·éæ
		template < class ValueType >
		template < class ImageType >
		inline Image< ValueType >::Image( const ImageType &img ) : tmatsu::Image< ValueType >( img )
		{
		}

		/// @brief f[^^ÌÙÈéæð¡»·é
		/// @param[in] img ¡»·éæ
		template < class ValueType >
		template < class Type >
		inline Image< ValueType >::Image( const Image< Type > &img ) : tmatsu::Image< ValueType >( img )
		{
		}

		/// @brief æðt@C©çÇÝÞ
		/// @param[in] callback R[obNÖ
		/// @param[in] file_name ÇÝÞt@C¼
		/// @param[in] hdr_name wb_t@C¼DKèlÍNULLDNULLªwè³ê½êCfile_nameÉ".header"ðtÁµ½wb_t@C¼ªÝè³êé
		template < class Type >
		template < class Functor >
		inline Image< Type >::Image( Functor callback, const char *file_name, const char *hdr_name ) : tmatsu::Image< ValueType >( callback, file_name, hdr_name )
		{
		}
	
		/// @brief £lªL^³ê½cüæðæ¾·é
		/// @attention oÍæÌf[^^Í®_¬^ÅÈ¯êÎÈçÈ¢
		/// @param[out] centerline cüði[·éæ
		template < class ValueType >
		template < class ImageType >
		inline void Image< ValueType >::thinning( ImageType &centerline )const
		{
			Image img; // êæ
			ImageType thickness; // ¾³æ

			// ú»·é
			centerline = *this;

			// ðßé
			mist::remove_hole_region( *this, img );

			// ×ü»·é
			TOPOLOGICAL( mist::euclidean::thinning )( img, centerline );

			// £Ï··é
			mist::euclidean::distance_transform( *this, thickness );
			img.clear( );

			// ¾³ðãü·é
			for ( size_t i = 0; i < centerline.size( ); i++ )
			{
				if ( centerline[ i ] )
				{
					centerline[ i ] = static_cast< ImageType::value_type >( std::sqrt( thickness[ i ] ) * centerline.reso1( ) );
				}
			}
		}

		/// @brief æð¡»·é
		/// @param[in] img ¡»·éæ
		/// @return ©ª©gÌQÆ
		template < class ValueType >
		inline Image< ValueType > &Image< ValueType >::operator=( const Image &img )
		{
			if ( this != &img )
			{
				tmatsu::Image< ValueType >::operator=( img );
			}

			return *this;
		}

		/// @brief ^ÌÙÈéæð¡»·é
		/// @param[in] img ¡»·éæ
		/// @return ©ª©gÌQÆ
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

		/// @brief f[^^ÌÙÈéæð¡»·é
		/// @param[in] img ¡»·éæ
		/// @return ©ª©gÌQÆ
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

		/// @brief ®ì³µ
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( )
		{
		}
	
		/// @brief ÇoíØ\¢ð¡»·é
		/// @param[in] tree ¡»·éÇoíØ\¢
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( const Tree &tree ) : siz_( tree.siz_ ), pitch_( tree.pitch_ ), tree< _Branch< ValueType, NameType, LikelihoodType > >( tree )
		{
		}
	
		/// @brief cüæ©çØ\¢ð\z·é
		/// @param[in] centerline cüæ
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( const ThicknessImage &centerline )
		{
			construct( centerline );
		}
	
		/// @brief t@C©çØ\¢ðÇÝÞ
		/// @param[in] file_name ÇÝÞXMLt@C¼
		/// @param[in] siz æÌTCY
		/// @param[in] pitch æÌsb`
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( const char *file_name, const SizeSet &siz, const PitchSet &pitch )
		{
			read( file_name, siz, pitch );
		}

		/// @brief cüæ©çØ\¢ð\z·é
		/// @param[in] centerline cüæ
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::construct( const ThicknessImage &centerline )
		{
			ThicknessImage temp = centerline; // êæ
			std::queue< std::tr1::tuple< size_t, Point, ThicknessImage::value_type > > queue; // Ò¿sñ
			size_t id = 1; // ¯Êq
	
			// TCYÆsb`ðÝè·é
			siz_.x = centerline.width( );
			siz_.y = centerline.height( );
			siz_.z = centerline.depth( );
			pitch_.x = centerline.reso1( );
			pitch_.y = centerline.reso2( );
			pitch_.z = centerline.reso3( );

			// Ø\¢ðóÉ·é
			clear( );

			// e}ð¸·é
			do
			{
				size_t parent_id = queue.empty( ) ? 0 : std::tr1::get< 0 >( queue.front( ) ); //eÌ¯Êq
				Point start_pt = queue.empty( ) ? root_point( temp ) : std::tr1::get< 1 >( queue.front( ) ); //n_
				Point end_pt; // I_
				iterator parent_branch = end( ); // e}
				iterator branch; // ¸·é}
		
				// ¾³ð³·é
				if ( !queue.empty( ) )
				{
					// ¾³ð³·é
					temp( start_pt ) = std::tr1::get< 2 >( queue.front( ) );

					// fL[·é
					queue.pop( );
				}
		
				// e}ðTõ·é
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
		
				// }ð}ü·é
				branch = insert( parent_branch, Branch( ) );
				branch->id = id++;
				branch->name = reject_name( );
				end_pt = traverse_branch( branch->value, temp, start_pt );
		
				// qðGL[
				for ( size_t i = 0; i < TOPOLOGY; i++ )
				{
					try
					{
						Point next_pt = end_pt + TOPOLOGICAL( neighborhood )[ i ]; // qÌ}Ìn_

						if ( temp( next_pt ) > 0 )
						{
							// ¢¸æfÌÆ«
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

			// ¯Êqðú»·é
			for ( iterator iter = begin( ); iter != end( ); iter++ )
			{
				iter->id = 0;
			}

			// ãðs¤
			construct_postprocess( );

			// ¯ÊqðUè¼·
			id = 1;

			for ( iterator iter = preorder_begin( ); iter != preorder_end( ); iter++ )
			{
				iter->id = id++;
			}
		}

		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::construct(const ThicknessImage &centerline, const Point& seedPoint)
		{
			ThicknessImage temp = centerline; // êæ
			std::queue< std::tr1::tuple< size_t, Point, ThicknessImage::value_type > > queue; // Ò¿sñ
			size_t id = 1; // ¯Êq

			// TCYÆsb`ðÝè·é
			siz_.x = centerline.width();
			siz_.y = centerline.height();
			siz_.z = centerline.depth();
			pitch_.x = centerline.reso1();
			pitch_.y = centerline.reso2();
			pitch_.z = centerline.reso3();

			// Ø\¢ðóÉ·é
			clear();

			// e}ð¸·é
			do
			{
				size_t parent_id = queue.empty() ? 0 : std::tr1::get< 0 >(queue.front()); //eÌ¯Êq
				Point start_pt = queue.empty() ? root_point(temp, seedPoint) : std::tr1::get< 1 >(queue.front()); //n_
				Point end_pt; // I_
				iterator parent_branch = end(); // e}
				iterator branch; // ¸·é}

				// ¾³ð³·é
				if (!queue.empty())
				{
					// ¾³ð³·é
					temp(start_pt) = std::tr1::get< 2 >(queue.front());

					// fL[·é
					queue.pop();
				}

				// e}ðTõ·é
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

				// }ð}ü·é
				branch = insert(parent_branch, Branch());
				branch->id = id++;
				branch->name = reject_name();
				end_pt = traverse_branch(branch->value, temp, start_pt);

				// qðGL[
				for (size_t i = 0; i < TOPOLOGY; i++)
				{
					try
					{
						Point next_pt = end_pt + TOPOLOGICAL(neighborhood)[i]; // qÌ}Ìn_

						if (temp(next_pt) > 0)
						{
							// ¢¸æfÌÆ«
							queue.push(std::tr1::make_tuple(branch->id, next_pt, temp(next_pt)));
							temp(next_pt) = 0;
						}
					}

					catch (ThicknessImage::InvalidPoint)
					{
					}
				}
			} while (!queue.empty());

			// ¯Êqðú»·é
			for (iterator iter = begin(); iter != end(); iter++)
			{
				iter->id = 0;
			}

			// ãðs¤
			construct_postprocess();

			// ¯ÊqðUè¼·
			id = 1;

			for (iterator iter = preorder_begin(); iter != preorder_end(); iter++)
			{
				iter->id = id++;
			}
		}

		/// @brief Á¥Êðo·é
		/// @param[in] features oµ½Á¥Êði[·éReiDSTLReiÆÝÈ¹È¯êÎÈçÈ¢
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		template < class Container >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::extract( Container &features )const
		{
			const_iterator ref_branch = reference_branch( ); // î}
		
			// ReiðóÉ·é
			features.clear( );

			// Á¥Êðo·é
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				FeatureType feature; // Á¥xNg
				
				extract_feature( feature, iter, ref_branch );
				features.insert( features.end( ), feature );
			}
		}
		
		/// @brief Ît¯ðs¤
		/// @param[out] result Ît¯Êði[·éØ\¢
		/// @param[in] likelihood_func ÞxÖDÈºÌæ¤ÈÄÑoµªÅ«éÖÆÝÈ¹È¯êÎÈçÈ¢
		/// @code
		/// NameType name; // ¼O
		/// FeatureType feature; // Á¥xNg
		/// LikelihoodType likelihood // Þx
		/// 
		/// likelihood = likelihood_func( name, feature );
		/// @endcode
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		template < class Functor >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::labeling( Tree &result, Functor likelihood_func )const
		{
			std::set< NameType > preprocessed_names; // OÉæèÎt¯é¼O
			std::set< NameType > names; // ÞxÖÉæèÎt¯é¼O
			std::set< NameType > combined_names; // SÄÌ¼O
			
			// oÍðú»·é
			result = *this;

			for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				iter->name = result.reject_name( );
			}

			// Oðs¤
			result.labeling_preprocess( );

			// OÉæèÎt¯çê½¼Oðßé
			for ( const_iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				if ( iter->name != reject_name( ) )
				{
					preprocessed_names.insert( iter->name );
				}
			}

#ifdef _CALC_LIKELIHOOD
			// ÞxðvZ·é
			for ( iterator iter = result.preorder_begin( ); iter != result.preorder_end( ); iter++ )
			{
				if ( iter->name == reject_name( ) )
				{
					FeatureType feature; // Á¥xNg
					const_iterator ref_branch = result.reference_branch( ); // î}
					LikelihoodType parent_likelihood; // e}ÌÞx
			
					// ú»·é
					iter->likelihood.clear( );
			
					// Á¥Êðo·é
					result.extract_feature( feature, iter, ref_branch );

					// e}ÌÞxðvZ·é
					if ( result.is_root( iter ) )
					{
						parent_likelihood = result.default_likelihood( );
					}
					else
					{
						double parent_sum = 0; // e}ÌÞxÌa

						parent_likelihood = result.parent( iter )->likelihood;

						// e}ÌÞxÌaðvZ·é
						for ( LikelihoodType::const_iterator parent_iter = parent_likelihood.begin( ); parent_iter != parent_likelihood.end( ); parent_iter++ )
						{
							parent_sum += parent_iter->second;
						}

						// e}ÌÞxð³K»·é
						if ( parent_sum )
						{
							for ( LikelihoodType::iterator parent_iter = parent_likelihood.begin( ); parent_iter != parent_likelihood.end( ); parent_iter++ )
							{
								parent_iter->second /= parent_sum;
							}
						}
					}	

					// ÞxðZo·é
					for ( LikelihoodType::const_iterator parent_iter = parent_likelihood.begin( ); parent_iter != parent_likelihood.end( ); parent_iter++ )
					{
						LikelihoodType likelihood = likelihood_func( parent_iter->first, feature ); // Þx
						double avg = 0, var = 0; // ÞxÌvÊ
						size_t cnt = 0; // ¼OÌ

						// ÞxÖÉæèÎt¯é¼OðßCOÅÎt¯çê½¼OðO·é
						for ( LikelihoodType::iterator child_iter = likelihood.begin( ); child_iter != likelihood.end( ); )
						{
							if ( preprocessed_names.find( child_iter->first ) != preprocessed_names.end( ) )
							{
								child_iter = likelihood.erase( child_iter );
							}
							else
							{
								// ¼Oð}ü·é
								names.insert( child_iter->first );

								// wÖðÆé
								//child_iter->second = std::exp( child_iter->second );

								// vÊðvZ·é
								avg += child_iter->second;
								var += std::pow( child_iter->second, 2 );
								cnt++;

								// Oi·é
								child_iter++;
							}
						}

						avg /= cnt;
						var = var / cnt - std::pow( avg, 2 );

						// ³K»·é
						for ( LikelihoodType::iterator child_iter = likelihood.begin( ); child_iter != likelihood.end( ); child_iter++ )
						{
							child_iter->second = std::max( ( child_iter->second - avg ) / ( var ? std::sqrt( var ) : 1 ), 0.0 );
							//child_iter->second = std::exp( ( child_iter->second - avg ) / ( var ? std::sqrt( var ) : 1 ) );
						}

#ifdef _FIND_OPTIMUM_BRANCHING
						// e²ÆÌÞxði[·é
						iter->plausibility.insert( Branch::PlausibilityType::value_type( parent_iter->first, likelihood ) );

						// eÞxð³K»·é
						for ( Branch::PlausibilityType::iterator plausibility_iter = iter->plausibility.begin( ); plausibility_iter != iter->plausibility.end( ); plausibility_iter++ )
						{
							double likelihood_sum = 0; // ÞxÌa
							
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

						// eÌÞxð©¯é
						for ( LikelihoodType::iterator child_iter = likelihood.begin( ); child_iter != likelihood.end( ); child_iter++ )
						{
							child_iter->second *= parent_iter->second;
						}

						// ÞxðÁZ·é
						iter->likelihood += likelihood;
					}
				}
				else
				{
					iter->likelihood.insert( iter->likelihood.end( ), LikelihoodType::value_type( iter->name, 1 ) );
				}
			}

			// SÄÌ¼Oðßé
			combined_names.insert( names.begin( ), names.end( ) );
			combined_names.insert( preprocessed_names.begin( ), preprocessed_names.end( ) );

			// ³K»ðs¤
			for ( std::set< NameType >::const_iterator name_iter = combined_names.begin( ); name_iter != combined_names.end( ); name_iter++ )
			{
				double likelihood_sum = 0; // ÞxÌa
						
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
			FeatureType feature; // Á¥xNg
			std::map< NameType, std::set< NameType > > integrity; // Â\Èªò

			result.extract_feature( feature, result.begin( ), result.reference_branch( ) );

			// Â\ÈªòðvZ·é
			for ( std::set< NameType >::const_iterator iter = combined_names.begin( ); iter != combined_names.end( ); iter++ )
			{
				LikelihoodType likelihood = likelihood_func( *iter, feature ); // Þx
				std::map< NameType, std::set< NameType > >::iterator integrity_iter = integrity.insert( integrity.end( ), std::map< NameType, std::set< NameType > >::value_type( *iter, std::set< NameType >( ) ) ); //®«

				for ( LikelihoodType::const_iterator likelihood_iter = likelihood.begin( ); likelihood_iter != likelihood.end( ); likelihood_iter++ )
				{
					integrity_iter->second.insert( likelihood_iter->first );
				}
			}
#endif //_CALC_INTEGRITY

#if defined( _UPDATE_LIKELIHOOD )
			double err = DBL_MAX; //ë·

			// ³K»·é
			for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				double likelihood_sum = 0; //ÞxÌa
			
				for ( LikelihoodType::const_iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_sum += likelihood_iter->second;
				}
		
				for ( LikelihoodType::iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_iter->second /= likelihood_sum > 0 ? likelihood_sum : 1;
				}
			}

			// ÞxðXV
			size_t cnt = 0; // JE^

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
						double parent_consistency = 1, child_consistency = 1, sibling_consistency = 1; // ³µx
			
						// eÆÌ³µx
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

						// qÆÌ³µx
						for ( iterator child_iter = result.child_begin( iter ); child_iter != result.child_end( iter ); child_iter++ )
						{
							double consistency = 1; // ³µx

							for ( LikelihoodType::const_iterator child_likelihood_iter = child_iter->prev_likelihood.begin( ); child_likelihood_iter != child_iter->prev_likelihood.end( ); child_likelihood_iter++ )
							{
								if ( integrity[ likelihood_iter->first ].find( child_likelihood_iter->first ) == integrity[ likelihood_iter->first ].end( ) )
								{
									consistency -= child_likelihood_iter->second;
								}
							}
								
							child_consistency *= consistency;
						}

						// ZíÆÌ³µx
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
					FeatureType feature; // Á¥xNg
					double max_likelihood = -DBL_MAX; // ÅåÞx
			
					// Á¥Êðo·é
					result.extract_feature( feature, iter, result.reference_branch( ) );

					// ÞxðvZ·é
					LikelihoodType likelihood = likelihood_func( result.parent( iter )->name, feature ); //Þx

					for ( LikelihoodType::const_iterator likelihood_iter = likelihood.begin( ); likelihood_iter != likelihood.end( ); likelihood_iter++ )
					{
						if ( iter->candidate[ result.parent( iter )->name ].find( likelihood_iter->first ) != iter->candidate[ result.parent( iter )->name ].end( ) && max_likelihood < likelihood_iter->second )
						{
							iter->name = likelihood_iter->first;
							max_likelihood = likelihood_iter->second;
						}
					}
			
					// óâðÓé¢Æ·
					if ( iter->name != result.reject_name( ) )
					{
						// ªòÌÆ«
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
						// ·ÌÆ«
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
				iterator max_branch = result.end( ); // ÅåÞxðÂ}
				double max_likelihood = 0; // ÅåÞx

				for (  iterator iter = result.begin( ); iter != result.end( ); iter++ )
				{
					if ( !result.is_root( iter ) && iter->name == result.reject_name( ) )
					{
						for ( std::map< NameType, std::set< NameType > >::const_iterator integrity_iter = integrity.begin( ); integrity_iter != integrity.end( ); integrity_iter++ )
						{
							double ancestor_likelihood = 0; // æc}ÌÞx

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

			// ªòð··é
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
			}

#elif defined( _SELECT_MODEL )
			tree< NameType > model; // f
			const_iterator ref_branch = result.reference_branch( ); // QÆ}

			// QÆ}Í[x[XÅ¼OªÎt¯çêÄ¢éÆ¼è·é
			model.insert( model.end( ), ref_branch->name );

			// Æè¤éªòp^[ðñ·é
			for ( tree< NameType >::iterator model_iter = model.begin( ); model_iter != model.end( ); model_iter++ )
			{
				for ( std::set< NameType >::const_iterator integrity_iter = integrity[ *model_iter ].begin( ); integrity_iter != integrity[ *model_iter ].end( ); integrity_iter++ )
				{
					bool flag = true; // tO

					// ùÉ¶Ý·é¼OÍsÂ
					for ( tree< NameType >::const_iterator ancestor_iter = model_iter; ancestor_iter != model.end( ); ancestor_iter = model.parent( ancestor_iter ) )
					{
						if ( *ancestor_iter == *integrity_iter )
						{
							flag = false;
							break;
						}
					}

					// ¼Oð}ü·é
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

			// QÆ}ðªÆ·éªØðÎÛÆ·é
			result.select_model( std::find( result.preorder_begin( ), result.preorder_end( ), ref_branch->name ), model.preorder_begin( ), model );
#elif defined( _FIND_OPTIMUM_BRANCHING )
			// ÅKªòðvZ·é
			result.find_optimum_branching( combined_names, integrity );
#else
			// Åà¢ÞxðÂ¼OðÎt¯é
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
			// ëèù³ðs¤
			for ( std::set< NameType >::const_iterator iter = names.begin( ); iter != names.end( ); iter++ )
			{
			}
#endif //_CORRECT_ERROR

			// ãðs¤
			result.labeling_postprocess( );
		}

		/// @brief Ît¯Êð]¿·é
		/// @param[out] confusion Êði[·éRt[W}gbNX
		/// @param[in] truth ³ðØ\¢DeqÖWª¯êÅÈ¯êÎÈçÈ¢
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::evaluate( Matrix &confusion, const Tree &truth )const
		{
			// ú»·é
			confusion.resize( name_number( ), name_number( ) );
			confusion.fill( 0 );

			// ]¿·é
			for ( const_iterator result_iter = begin( ), truth_iter = truth.begin( ); result_iter != end( ) && truth_iter != truth.end( ); result_iter++, truth_iter++ )
			{
#ifndef _EVALUATE_LIKELIHOOD_RANKING
#ifdef _EVALUATE_HEAD_ONLY
				bool exist = false; // ¶ÝtO

				for ( const_iterator parent_iter = truth.parent( truth_iter ); parent_iter != truth.end( ); parent_iter = truth.parent( parent_iter ) )
				{
					if ( parent_iter->name == truth_iter->name )
					{
						exist = true;
						break;
					}
				}

				// eÉ¯¶¼Oª¶ÝµÈ¢}ÌÝ]¿·é
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
			
		/// @brief ¼Ot«cüæð³·é
		/// @param[out] labeled_centerline ¼Ot«cüæði[·éæ
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::restore( LabelImage &labeled_centerline )const
		{
			// ú»·é
			labeled_centerline.resize( siz_.x, siz_.y, siz_.z );
			labeled_centerline.reso( pitch_.x, pitch_.y, pitch_.z );
			labeled_centerline.fill( 0 );
		
			// ³·é
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				restore_centerline( labeled_centerline, iter );
			}
		}
		
		/// @brief ¼Ot«Çoíæð³·é
		/// @param[in] tubular ÇoíÌææ
		/// @param[out] labeled_tubular ¼Ot«Çoíæði[·éæ
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::restore( const MarkImage &tubular, LabelImage &labeled_tubular )const
		{
			std::list< Point > voxels; // ³³ê½æf

			// cüð³·é
			restore( labeled_tubular );

			// ªò_ð³·é
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( !is_leaf( iter ) )
				{
					restore_bifurcation( tubular, labeled_tubular, iter );
				}
			}

			// ³³ê½æfðßé
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

			// ³·é
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

		/// @brief t@C©çØ\¢ðÇÝÞ
		/// @param[in] file_name ÇÝÞXMLt@C¼
		/// @param[in] siz æÌTCY
		/// @param[in] pitch æÌsb`
		/// @retval true Ø\¢ÌÇÝÝÉ¬÷µ½
		/// @retval false Ø\¢ÌÇÝÝÉ¸sµ½D°ç­Ct@CðJ¯È©Á½©Ü½Ít@CÌàeªs³¾Á½
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline bool Tree< ValueType, NameType, LikelihoodType, FeatureType >::read( const char *file_name, const SizeSet &siz, const PitchSet &pitch )
		{
			XML xml; // XML
			
			// XMLðÇÝÞ
			if ( !xml.read( file_name ) )
			{
				return false;
			}
			
			// G[`FbN
			if ( xml.empty( ) || xml.root( ).tag( ).name( ) != "VESSEL" )
			{
				return false;
			}

			// TCYÆsb`ðÝè·é
			siz_ = siz;
			pitch_ = pitch;

			// Ø\¢ðóÉ·é
			clear( );
			
			// XML©çØ\¢ðÇÝÞ
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

		/// @brief t@CÖØ\¢ð«Þ
		/// @param[in] file_name «ÞXMLt@C¼
		/// @retval true Ø\¢Ì«ÝÉ¬÷µ½
		/// @retval false Ø\¢Ì«ÝÉ¸sµ½D°ç­Ct@CðJ¯È©Á½
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline bool Tree< ValueType, NameType, LikelihoodType, FeatureType >::write( const char *file_name )const
		{
			XML xml; // XML

			// [gvfðÇÁ·é
			xml.insert( xml.begin( ), XML::Element( "VESSEL" ) );

			// e}ð«Þ
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				XML::iterator branch_elem = xml.insert( xml.end( ), XML::Element( "BRANCH" ) ); // BRANCHvf

				// ®«ð«Þ
				branch_elem->tag( ).insert( branch_elem->tag( ).end( ), XML::Element::Tag::value_type( "id", std::to_string( iter->id ) ) );
				branch_elem->tag( ).insert( branch_elem->tag( ).end( ), XML::Element::Tag::value_type( "name", to_string( iter->name ) ) );

				// eð«Þ
				if( !is_root( iter ) )
				{
					XML::iterator parent_elem = xml.insert( branch_elem, XML::Element( "PARENT" ) ); // PARENTvf

					branch_elem = xml.parent( parent_elem );
					parent_elem->tag( ).insert( parent_elem->tag( ).end( ), XML::Element::Tag::value_type( "id", std::to_string( parent( iter )->id ) ) );
				}

				// qð«Þ
				for ( const_iterator child_iter = child_begin( iter ); child_iter != child_end( iter ); child_iter++ )
				{
					XML::iterator child_elem = xml.insert( branch_elem, XML::Element( "CHILD" ) ); // CHILDvf

					branch_elem = xml.parent( child_elem );
					child_elem->tag( ).insert( child_elem->tag( ).end( ), XML::Element::Tag::value_type( "id", std::to_string( child_iter->id ) ) );
				}

				// }ð«Þ
				if ( !write_branch( xml, branch_elem, iter ) )
				{
					return false;
				}
			}

			return xml.write( file_name );
		}

		/// @brief æÌTCYðæ¾·é
		/// @return æÌTCYÌg
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline const SizeSet &Tree< ValueType, NameType, LikelihoodType, FeatureType >::get_size( )const
		{
			return siz_;
		}

		/// @brief æÌTCYðÝè·é
		/// @param[in] siz Ýè·éæÌTCYÌg
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::set_size( const SizeSet &siz )
		{
			siz_ = siz;
		}

		/// @brief æÌsb`ðæ¾·é
		/// @return æÌsb`Ìg
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline const PitchSet &Tree< ValueType, NameType, LikelihoodType, FeatureType >::get_pitch( )const
		{
			return pitch_;
		}

		/// @brief æÌsb`ðÝè·é
		/// @param[in] pitch Ýè·éæÌsb`Ìg
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::set_pitch( const PitchSet &pitch )
		{
			pitch_ = pitch;
		}
	
		/// @brief Ø\¢\zÌãðs¤¼zÖ
		/// @attention ftHgÌ®ìÍÈ¢ªCI[o[Ch·é±ÆÅ@\ðg£·é±ÆªÅ«é
		/// @attention ±ÌÖªÄÎêé_ÅC}Ì¯ÊqÉÍ³øÈlªi[³êÄ¢é
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::construct_postprocess( )
		{
		}
	
		/// @brief Ît¯ÌOðs¤¼zÖ
		/// @attention ftHgÌ®ìÍÈ¢ªCI[o[Ch·é±ÆÅ@\ðg£·é±ÆªÅ«é
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::labeling_preprocess( )
		{
		}
	
		/// @brief Ît¯Ìãðs¤¼zÖ
		/// @attention ftHgÌ®ìÍÈ¢ªCI[o[Ch·é±ÆÅ@\ðg£·é±ÆªÅ«é
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::labeling_postprocess( )
		{
		}

#ifdef _FIND_MAX_LIKELIHOOD
		/// @brief ÞxªÅåÆÈéoHðTõµ»ÌI[}ÆoHãÌÅåÞxÌgðÔ·
		/// @param[in] name Ú·é¼O
		/// @param[in] branch Tõ·éªØÌªðw·Ce[^
		/// @return ÞxªÅåÆÈéoHÌI[}ÆoHãÌÅåÞxÌg
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline std::pair< typename Tree< ValueType, NameType, LikelihoodType, FeatureType >::iterator, double > Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_max_likelihood( const NameType &name, iterator branch )
		{
			std::pair< iterator, double > max_likelihood( branch, 0 ); // ÞxªÅåÆÈéoHÌI[}ÆoHãÌÅåÞxÌg

			// qðTõ·é
			for ( iterator iter = child_begin( branch ); iter != child_end( branch ); iter++ )
			{
				std::pair< iterator, double > temp = find_max_likelihood( name, iter ); // êÏ

				if ( max_likelihood.second < temp.second )
				{
					max_likelihood = temp;
				}
			}

			// ÅåÞxðXV·é
			if ( branch->likelihood.find( name ) != branch->likelihood.end( ) && max_likelihood.second < branch->likelihood[ name ] )
			{
				max_likelihood.second = branch->likelihood[ name ];
			}

			return max_likelihood;
		}
#endif //_FIND_MAX_LIKELIHOOD

#ifdef _SELECT_MODEL
		/// @brief fIðÉîÃ­Ît¯ðs¤
		/// @param[inout] branch Ît¯ðs¤}ðw·Ce[^
		/// @param[in] selected_model »ÝIð³êÄ¢éfðw·Ce[^
		/// @param[in] model fØ\¢
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::select_model( iterator branch, typename tree< NameType >::const_iterator selected_model, const tree< NameType > &model )
		{
			tree< NameType >::const_iterator max_model = selected_model; // ÅåXRAðÂf
			double max_score = -DBL_MAX; // ÅåXRA
			double ancestor_score = calc_ancestor_score( branch, *selected_model ); // æcXRA

			// ªòXRAð¸·é
			for ( tree< NameType >::const_iterator child_iter = model.child_begin( selected_model ); child_iter != model.child_end( selected_model ); child_iter++ )
			{
				double descendant_score = find_max_descendant_score( branch, child_iter, model ); // q·XRA
				double self_score = calc_self_score( branch, *child_iter ); // ©gXRA
						
				if ( max_score < ( ancestor_score + descendant_score ) * self_score )
				{
					max_model = child_iter;
					max_score = ( ancestor_score + descendant_score ) * self_score;
				}
			}
	
			// ·XRAð¸·é
			{
				double descendant_score = find_max_descendant_score( branch, selected_model, model ); // q·XRA
				double self_score = calc_self_score( branch, *selected_model ); // ©gXRA

				if ( max_score < descendant_score * self_score )
				{
					max_model = selected_model;
				}
			}

			// Ît¯ðs¤
			if ( branch->name == reject_name( ) )
			{
				branch->name = *max_model;
			}

			// q}ÌÎt¯ðs¤
			for ( iterator child_iter = child_begin( branch ); child_iter != child_end( branch ); child_iter++ )
			{
				select_model( child_iter, max_model, model );
			}
		}
	
		/// @brief ÅåÌq·XRAðßé
		/// @param[in] branch 
		/// @param[in] selected_model 
		/// @param[in] model 
		/// @return ÅåÌq·XRA
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_max_descendant_score( iterator branch, typename tree< NameType >::const_iterator selected_model, const tree< NameType > &model )
		{
			double max_score = model.is_leaf( selected_model ) ? 1 : 0; // ÅåXRA

			// qðTõ·é
			for ( tree< NameType >::const_iterator child_iter = model.child_begin( selected_model ); child_iter != model.child_end( selected_model ); child_iter++ )
			{	
				double score = find_max_descendant_score( branch, child_iter, model ); // q·ÌÅåXRA
				double descendant_score = calc_descendant_score( branch, *child_iter ); // XRA

				if ( max_score < score + descendant_score )
				{
					max_score = score + descendant_score;
				}
			}

			return max_score;
		}

		/// @brief wèÌ}ÌwèÌ¼OÉÖ·é©gXRAðvZ·é
		/// @param[in] branch XRAðvZ·é}ðw·Ce[^
		/// @param[in] name XRAðvZ·é¼O
		/// @return ©gXRA
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_self_score( iterator branch, NameType name )
		{
			double pos = 0, neg = 0; // XRA

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

		/// @brief wèÌ}ÌwèÌ¼OÉÖ·éæcXRAðvZ·é
		/// @param[in] branch XRAðvZ·é}ðw·Ce[^
		/// @param[in] name XRAðvZ·é¼O
		/// @return æcXRA
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_ancestor_score( iterator branch, NameType name )
		{
			double pos = 0, neg = 0; //XRA

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

		/// @brief wèÌ}ÌwèÌ¼OÉÖ·éq·XRAðvZ·é
		/// @param[in] branch XRAðvZ·é}ðw·Ce[^
		/// @param[in] name XRAðvZ·é¼O
		/// @return q·XRA
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_descendant_score( iterator branch, NameType name )
		{
			double pos = 0, neg = 0; //XRA

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
		/// @brief ªò\ðvZ·é
		/// @param[in,out] branch ªò\ðvZ·é}
		/// @param[in] names ¼OÌW
		/// @param[in] integrity ®«
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_table( iterator branch, std::set< NameType > names, std::map< NameType, std::set< NameType > > &integrity )
		{
			// e¼OÉÂ¢Äªò\ðvZ·é
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				Branch::BifurcationType temp_bifurcation; // ªòðêIÉi[·éÌæ
				std::set< Branch::BifurcationType > bifurcation_candidates; // ªòÌóâ
				std::set< Branch::BifurcationType >::iterator max_bifurcation; // XRAðÅå»·éªò
				double max_score = 0; // ÅåXRA

				// WFNg¼ÍvZµÈ¢
				if ( *name_iter == reject_name( ) )
				{
					continue;
				}
				
				// ªòðvZ·é
				for ( iterator iter = subtree( branch ).begin( ); iter != subtree( branch ).end( ); iter++ )
				{
					bifurcation_candidates.insert( calc_bifurcation( branch, iter, *name_iter, integrity, temp_bifurcation ) );
				}
				
				// XRAðÅå»·éªòð}ü·é
				max_bifurcation = bifurcation_candidates.begin( );
				
				for ( std::set< Branch::BifurcationType >::iterator bifurcation_iter = bifurcation_candidates.begin( ); bifurcation_iter != bifurcation_candidates.end( ); bifurcation_iter++ )
				{
					double score = calc_score( *bifurcation_iter ); // XRA

					if ( max_score < score )
					{
						max_bifurcation = bifurcation_iter;
						max_score = score;
					}
				}

				branch->bifurcation_table.insert( Branch::BifurcationTableType::value_type( *name_iter, *max_bifurcation ) );
			}
		}
			
		/// @brief wèÌÇªwèÌ}©çwèÌqÌ}É··éêÌªòðßé
		/// @param[in] branch ªòðßé}
		/// @param[in] descendant_branch ··éqÌ}
		/// @param[in] name ··éÇ
		/// @param[in] integrity Â\Èªò
		/// @param[out] descendant ªòði[·éRei
		/// @return wèÌReiÌQÆ
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline typename Tree< ValueType, NameType, LikelihoodType, FeatureType >::Branch::BifurcationType &Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_bifurcation( iterator branch, iterator descendant_branch, NameType name, std::map< NameType, std::set< NameType > > &integrity, typename Branch::BifurcationType &bifurcation )
		{
			// \[gÌ½ßÌärÖ
			struct SortPred
			{
				// ÖÄÑoµZq
				bool operator( )( const std::tr1::tuple< NameType, size_t, double > &l, const std::tr1::tuple< NameType, size_t, double > &r )
				{
					return std::get< 2 >( l ) > std::get< 2 >( r );
				}
			};

			// íÌ½ßÌärÖ
			class RemovePred
			{
				NameType name_; // í·é¼O
				size_t id_; // í·é¯Êq
			public:
				// RXgN^
				RemovePred( NameType name, size_t id ) : name_( name ), id_( id )
				{
				}

				//@ÖÄÑoµZq
				bool operator( )( const std::tr1::tuple< NameType, size_t, double > &a )
				{
					return std::get< 0 >( a ) == name_ || std::get< 1 >( a ) == id_;
				}
			};

			std::list< std::tr1::tuple< NameType, size_t, double > > scores; // XRA
			
			// ªò\ðú»·é
			bifurcation.clear( );

			// I[XRAðßé
			bifurcation.insert( Branch::BifurcationType::value_type( name, std::make_pair( descendant_branch->id, calc_tl( descendant_branch, name ) ) ) );
			
			// ªòXRAðßé
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
		
		/// @brief wèÌ}ªwèÌÇÌI[}ÆÈéXRAðvZ·é
		/// @param[in] branch XRAðvZ·é}
		/// @param[in] name XRAðvZ·éÇ
		/// @return wèÌ}ªwèÌÇÌI[}ÆÈéXRA
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_tl( iterator branch, NameType name )
		{
#ifdef _IGNORE_TAIL_SCORE
			return 0;
#endif //_IGNORE_TAIL_SCORE

			if ( branch->name == reject_name( ) )
			{
				double ext_max = 0; // ·XRAÌÅå
					
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
			
		/// @brief wèÌ}ªwèÌÇÌJn}ÆÈéXRAðvZ·é
		/// @param[in] branch XRAðvZ·é}
		/// @param[in] name XRAðvZ·éÇ
		/// @param[in] parent_name wèÌ}ÌeÉt^³êéÇ¼
		/// @return wèÌ}ªwèÌÇÌJn}ÆÈéXRA
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
		
		/// @brief ªòÌXRAðvZ·é
		/// @param[in] XRAðvZ·éªò
		/// @return wèÌªòÌXRA
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
			
		/// @brief ÅKªòðßé±ÆÉæéxOðs¤
		/// @param[in] names ¼OÌW
		/// @param[in] integrity ®«
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_optimum_branching( std::set< NameType > names, std::map< NameType, std::set< NameType > > &integrity )
		{
			// e}ÌÞxð³K»·é
			for ( iterator iter = begin( ); iter != end( ); iter++ )
			{
				double likelihood_sum = 0; // ÞxÌa
	
				for ( LikelihoodType::const_iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_sum += likelihood_iter->second;
				}
		
				for ( LikelihoodType::iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_iter->second /= likelihood_sum > 0 ? likelihood_sum : 1;
				}
			}

			// ªò\ðvZ·é
			for ( iterator iter = postorder_begin( ); iter != postorder_end( ); iter++ )
			{
				calc_table( iter, names, integrity );
			}

			// ªòOtðßé
			tree< std::pair< NameType, size_t > > bifurcation_tree; // ªòc[
			Branch::BifurcationTableType bifurcation_graph, optimum_branching; // ªòOt
			const_iterator ref_branch = reference_branch( ); // QÆ}

			// QÆ}ÍÎÎÛªØÌªÅ è[x[XÅ¼OªÎt¯çêÄ¢éÆ¼èµªòc[Ìªð}ü·é
			bifurcation_tree.insert( bifurcation_tree.end( ), std::make_pair( ref_branch->name, ref_branch->id ) );
			
			// Æè¤éªòp^[ðñ·é
			for ( tree< std::pair< NameType, size_t > >::iterator tree_iter = bifurcation_tree.preorder_begin( ); tree_iter != bifurcation_tree.preorder_end( ); tree_iter++ )
			{
				iterator start_branch = std::find( begin( ), end( ), tree_iter->second ); // »ÝÚµÄ¢éÇÌJn}

				// ªòð}ü·é
				for ( std::set< NameType >::const_iterator integrity_iter = integrity[ tree_iter->first ].begin( ); integrity_iter != integrity[ tree_iter->first ].end( ); integrity_iter++ )
				{
					bool flag = true; // tO

					// WFNg¼Í}üµÈ¢
					if ( *integrity_iter == reject_name( ) )
					{
						continue;
					}

					// ùÉ¶Ý·é¼OÍ}üµÈ¢
					for ( tree< std::pair< NameType, size_t > >::const_iterator ancestor_iter = tree_iter; ancestor_iter != bifurcation_tree.end( ); ancestor_iter = bifurcation_tree.parent( ancestor_iter ) )
					{
						if ( ancestor_iter->first == *integrity_iter )
						{
							flag = false;
							break;
						}
					}

					// ¼Oð}ü·é
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

			// ªòOtðßé
			for ( std::map< NameType, std::set< NameType > >::const_iterator integrity_parent_iter = integrity.begin( ); integrity_parent_iter != integrity.end( ); integrity_parent_iter++ )
			{
				Branch::BifurcationType bifurcation; // ªò

				// WFNg¼ÍOtÉÜßÈ¢
				if ( integrity_parent_iter->first == reject_name( ) )
				{
					continue;
				}

				for ( std::set< NameType >::const_iterator integrity_child_iter = integrity_parent_iter->second.begin( ); integrity_child_iter != integrity_parent_iter->second.end( ); integrity_child_iter++ )
				{
					std::set< std::pair< double, double > > scores; // XRAÌW
					double wt_sum = 0; // dÝÌa
					double score_sum = 0; // XRAÌÁda

					// WFNg¼Æ©È[vÍOtÉÜßÈ¢
					if ( *integrity_child_iter == reject_name( ) || integrity_parent_iter->first == *integrity_child_iter )
					{
						continue;
					}

					// ÎÛÌªòp^[ðÂm[hÉÂ¢ÄXRAðæ¾·é
					for ( tree< std::pair< NameType, size_t > >::const_iterator tree_iter = bifurcation_tree.begin( ); tree_iter != bifurcation_tree.end( ); tree_iter++ )
					{
						if ( !bifurcation_tree.is_root( tree_iter ) && bifurcation_tree.parent( tree_iter )->first == integrity_parent_iter->first && tree_iter->first == *integrity_child_iter )
						{
							double wt = bifurcation_tree.depth( tree_iter ) < 2 ? 1.0 : std::find( begin( ), end( ), bifurcation_tree.parent( tree_iter )->second )->plausibility[ bifurcation_tree.parent( bifurcation_tree.parent( tree_iter ) )->first ][ integrity_parent_iter->first ]; // dÝ
							double score = std::find( begin( ), end( ), bifurcation_tree.parent( tree_iter )->second )->bifurcation_table[ integrity_parent_iter->first ][ *integrity_child_iter ].second; // XRA

							scores.insert( std::make_pair( wt, score ) );
						}
					}

					// dÝð³K»·é
					for ( std::set< std::pair< double, double > >::const_iterator score_iter = scores.begin( );score_iter != scores.end( ); score_iter++ )
					{
						wt_sum += score_iter->first;
					}

					// XRAÌÁdaðßé
					for ( std::set< std::pair< double, double > >::const_iterator score_iter = scores.begin( );score_iter != scores.end( ); score_iter++ )
					{
						score_sum += score_iter->second;// score_iter->first * score_iter->second / ( wt_sum > 0 ? wt_sum : 1 );
					}

					// ªòð}ü·é
					bifurcation.insert( bifurcation.end( ), Branch::BifurcationType::value_type( *integrity_child_iter, std::make_pair( 0, score_sum ) ) );
				}

				bifurcation_graph.insert( bifurcation_graph.end( ), Branch::BifurcationTableType::value_type( integrity_parent_iter->first, bifurcation ) );
			}

			// ªòOtÌÅKªòðßé
			find_optimum_branching( bifurcation_graph, optimum_branching );

			// Ît¯ðs¤
			std::stack< iterator > stack; // Ît¯ðs¤}ÌX^bN

			stack.push( std::find( begin( ), end( ), ref_branch->id ) );

			while ( !stack.empty( ) )
			{
				iterator tgt_branch = stack.top( ); // Ît¯ðs¤ÇÌÅÌ}

				// X^bN©ç}ð|bv·é
				stack.pop( );

				// Ît¯ðs¤
				for ( iterator iter = std::find( begin( ), end( ), tgt_branch->bifurcation_table[ tgt_branch->name ][ tgt_branch->name ].first ); iter != parent( tgt_branch ); iter = parent( iter ) )
				{
					iter->name = tgt_branch->name;
				}

				// Ît¯½Ç©çªò·éÇðvbV·é
				for ( Branch::BifurcationType::const_iterator bifurcation_iter = optimum_branching[ tgt_branch->name ].begin( ); bifurcation_iter != optimum_branching[ tgt_branch->name ].end( ); bifurcation_iter++ )
				{
					if ( tgt_branch->bifurcation_table[ tgt_branch->name ].find( bifurcation_iter->first ) != tgt_branch->bifurcation_table[ tgt_branch->name ].end( ) )
					{
						iterator next_branch = std::find( begin( ), end( ), tgt_branch->bifurcation_table[ tgt_branch->name ][ bifurcation_iter->first ].first ); // ÉÎt¯ðs¤}

						next_branch->name = bifurcation_iter->first;
						stack.push( next_branch );
					}
				}
			}

			// ··é
#ifdef _IGNORE_TAIL_SCORE
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				std::stack< iterator > stack; // }ÌX^bN

				// }ðX^bNÉÏÞ
				for ( iterator iter = find_max_plausibility( *name_iter, std::find( postorder_begin( ), postorder_end( ), *name_iter ) ).first; iter != end( ) && iter->name == reject_name( ); iter = parent( iter ) )
				{
					stack.push( iter );
				}

				// Çð··é
				for ( ; !stack.empty( ); stack.pop( ) )
				{
					bool cont = !is_root( stack.top( ) ) && parent( stack.top( ) )->name == *name_iter; // p±tO

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
			
		/// @brief ÅKªòðßé
		/// @param[in] directed_graph ÅKªòðßéLüOt
		/// @param[out] optimum_branching wèÌOtÌÅKªò
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_optimum_branching( const typename Branch::BifurcationTableType &directed_graph, typename Branch::BifurcationTableType &optimum_branching )const
		{
			mist::matrix< double > graph_mat( name_number( ), name_number( ) ), branching_mat; // ×Úsñ

			// ªòOt©ç×Úsñðì¬·é
			graph_mat.fill( -1 );

			for ( Branch::BifurcationTableType::const_iterator parent_iter = directed_graph.begin( ); parent_iter != directed_graph.end( ); parent_iter++ )
			{
				for ( Branch::BifurcationType::const_iterator child_iter = parent_iter->second.begin( ); child_iter != parent_iter->second.end( ); child_iter++ )
				{
					graph_mat( to_index( parent_iter->first ), to_index( child_iter->first ) ) = child_iter->second.second;
				}
			}

			// GhY@ðKp·é
			edmonds( graph_mat, branching_mat, std::greater< double >( ) );

			// ×Úsñ©çÅKªòðì¬·é
			optimum_branching.clear( );

			for ( Branch::BifurcationTableType::const_iterator parent_iter = directed_graph.begin( ); parent_iter != directed_graph.end( ); parent_iter++ )
			{
				Branch::BifurcationType bifurcation; // ªò

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
		/// @brief ÞxªÅåÆÈéoHðTõµ»ÌI[}ÆoHãÌÅåÞxÌgðÔ·
		/// @param[in] name Ú·é¼O
		/// @param[in] branch Tõ·éªØÌªðw·Ce[^
		/// @return ÞxªÅåÆÈéoHÌI[}ÆoHãÌÅåÞxÌg
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline std::pair< typename Tree< ValueType, NameType, LikelihoodType, FeatureType >::iterator, double > Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_max_plausibility( const NameType &name, iterator branch )
		{
			std::pair< iterator, double > max_plausibility( branch, 0 ); // ÞxªÅåÆÈéoHÌI[}ÆoHãÌÅåÞxÌg

			// qðTõ·é
			for ( iterator iter = child_begin( branch ); iter != child_end( branch ); iter++ )
			{
				std::pair< iterator, double > temp = find_max_plausibility( name, iter ); // êÏ

				if ( max_plausibility.second < temp.second )
				{
					max_plausibility = temp;
				}
			}

			// ÅåÞxðXV·é
			if ( branch != end( ) && max_plausibility.second < branch->plausibility[ name ][ name ] )
			{
				max_plausibility.second = branch->plausibility[ name ][ name ];
			}

			return max_plausibility;
		}
#endif //_IGNORE_TAIL_SCORE
#endif //_FIND_OPTIMUM_BRANCHING

		/// @brief XML©ç}ðÇÝÞ
		/// @param[in] xml ÇÝÞXML
		/// @param[in] id ÇÝÞ}Ì¯Êq
		/// @param[out] branch ÇÝÞ}ðw·Ce[^
		/// @retval true }ÌÇÝÝÉ¬÷µ½
		/// @retval false }ÌÇÝÝÉ¸sµ½D°ç­CXMLÌàeªs³¾Á½
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline bool Tree< ValueType, NameType, LikelihoodType, FeatureType >::read( const XML &xml, const std::string &id, iterator branch )
		{
			// vfTõÖ
			class FindElem
			{
				std::string tag_name_; // ^O¼
				std::string id_; // ¯Êq
			public:
				// RXgN^
				FindElem( const std::string &tag_name, const std::string &id ) : tag_name_( tag_name ), id_( id )
				{
				}
		
				// ÖÄÑoµZq
				bool operator( )( const XML::Element &elem )
				{
					return elem.tag( ).name( ) == tag_name_ && elem.tag( )[ "id" ] == id_;
				}
			};

			try
			{
				XML::const_iterator branch_elem = std::find_if( xml.child_begin( xml.preorder_begin( ) ), xml.child_end( xml.preorder_begin( ) ), FindElem( "BRANCH", id ) ); //BRANCHvf
				
				// G[`FbN
				if ( branch_elem == xml.child_end( xml.preorder_begin( ) ) )
				{
					return false;
				}
				
				// ®«ðÇÝÞ
				branch->id = std::stoi( branch_elem->tag( )[ "id" ] );
				branch->name = to_name( branch_elem->tag( )[ "name" ] );
				
				// qðÇÝÞ
				for( XML::const_iterator child_iter = std::find( xml.child_begin( branch_elem ), xml.child_end( branch_elem ), "CHILD" ); child_iter != xml.child_end( branch_elem ); child_iter = std::find( ++child_iter, xml.child_end( branch_elem ), "CHILD" ) )
				{
					iterator child_branch = insert( branch, value_type( ) ); // q
						
					// qðÇÝÞ
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
		
		/// @brief }ªwèÌ¯ÊqðÂ©©»è·é
		/// @param[in] id »è·é¯Êq
		/// @retval true }ÍwèÌ¯ÊqðÂ
		/// @retval false }ÍwèÌ¯Êqð½È¢
		template < class ValueType, class NameType, class LikelihoodType >
		inline bool _Branch< ValueType, NameType, LikelihoodType >::operator==( size_t id )const
		{
			return _Branch::id == id;
		}
		
		/// @brief }ªwèÌ¼OðÂ©©»è·é
		/// @param[in] name »è·é¼O
		/// @retval true }ÍwèÌ¼OðÂ
		/// @retval false }ÍwèÌ¼Oð½È¢
		template < class ValueType, class NameType, class LikelihoodType >
		inline bool _Branch< ValueType, NameType, LikelihoodType >::operator==( const NameType &name )const
		{
			return _Branch::name == name;
		}
	}
}

#endif //_TMATSU_TUBULAR_H_