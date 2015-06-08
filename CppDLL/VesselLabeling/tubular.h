/**
* @file tubular.h
* @brief �Ǎo���햼�Ή��t���̃w�b�_�t�@�C��
* @version 3.0
* @author ���É���w ���Ȋw������ ���f�B�A�Ȋw��U �X������   351203192   ���� �N�N
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
#define TOPOLOGICAL( a ) a ## 26 ///< @brief �g�|���W���l�����ڔ����t������
#elif TOPOLOGY == 6
#define TOPOLOGICAL( a ) a ## 6 ///< @brief �g�|���W���l�����ڔ����t������
#else
#error
#endif

namespace tmatsu
{
	namespace tubular 
	{
		/// @brief �Ǎo����؍\���̎}�������^
		/// @tparam ValueType �f�[�^�^
		/// @tparam NameType ���O�̌^
		/// @tparam LikeihoodType �ޓx�̑g�̌^
		template < class ValueType, class NameType, class LikelihoodType >
		struct _Branch
		{
			typedef ValueType ValueType; ///< @brief �f�[�^�^
			size_t id; ///< @brief ���ʎq
			NameType name; ///< @brief ���O
			LikelihoodType likelihood; ///< @brief �ޓx

#ifdef _UPDATE_LIKELIHOOD
			LikelihoodType prev_likelihood; ///< @brief ���O�̖ޓx
#endif //_UPDATE_LIKELIHOOD

#ifdef _SCREEN_OUT_CANDIDATE
			typedef std::map< NameType, std::set< NameType > > CandidateType; ///< @brief ���ǖ�����\���^
			CandidateType candidate; ///< @brief ���ǖ����
#endif //_SCREEN_OUT_CANDIDATE

#ifdef _FIND_OPTIMUM_BRANCHING
			typedef std::map< NameType, LikelihoodType > PlausibilityType; ///< @brief �e���Ƃ̖ޓx��\���^
			typedef std::map< NameType, std::pair< size_t, double > > BifurcationType; ///< @brief ����������^
			typedef std::map< NameType, BifurcationType > BifurcationTableType; ///< @brief ����\�������^
			PlausibilityType plausibility; 
			BifurcationTableType bifurcation_table;
#endif //_FIND_OPTIMUM_BRANCHING

			ValueType value; ///< @brief �f�[�^
			bool operator==( size_t id )const;
			bool operator==( const NameType &name )const;
		};

		/// @brief �Ǎo����摜�������N���X
		/// @tparam ValueType �f�[�^�^
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

		typedef Image< short > CTImage; ///< @brief CT�摜�������^
		typedef Image< unsigned char > MarkImage; ///< @brief �}�[�N�摜�������^
		typedef Image< float > ThicknessImage; ///< @brief �����摜�������^
		typedef Image< unsigned char > LabelImage; ///< @brief ���x���摜�������^

		/// @brief �Ǎo����؍\�����������ۃN���X
		/// @tparam ValueType �f�[�^�^
		/// @tparam NameType �}���̌^
		/// @tparam LikeihoodType �ޓx�̑g�̌^�D���O����ޓx�ւ̘A�z�z��Ƃ݂Ȃ����Ƃ��ł��C�܂����Z���s�����Ƃ��ł��Ȃ���΂Ȃ�Ȃ�
		/// @tparam FeatureType �����x�N�g���̌^
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		class Tree : public tree< _Branch< ValueType, NameType, LikelihoodType > >
		{
		public:
			typedef _Branch< ValueType, NameType, LikelihoodType > Branch; ///< @brief �Ǎo����؍\���̎}�������^
		private:
			SizeSet siz_; ///< @brief �T�C�Y
			PitchSet pitch_; ///< @brief �s�b�`
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
			/// @brief �؍\���̍��ƂȂ��f���擾���鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @param[in] centerline �c���摜
			/// @return �؍\���̍��ƂȂ��f�̍��W
			virtual Point root_point( const ThicknessImage &centerline )const = 0;
			virtual Point root_point(const ThicknessImage &centerline, Point seedPoint) const = 0;

			/// @brief �c���𑖍���1�{�̎}�̃f�[�^���擾���鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @param[out] val �擾����f�[�^
			/// @param[in] centerline ��������c���摜�D���̉摜�͏��������Ă��悢
			/// @param[in] start_pt �����̎n�_�̍��W
			/// @return �}�̏I�_�̍��W
			virtual Point traverse_branch( ValueType &val, ThicknessImage &centerline, const Point &start_pt )const = 0;

			/// @brief ��}���擾���鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @attention ���̊֐��͑Ή��t�����s����O�ɂ��s��ꂽ��ɂ��Ăяo�����
			/// @return ��}�̒萔�Q��
			virtual const_iterator reference_branch( )const = 0;
			
			/// @brief �����ʂ𒊏o���鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @param[out] feature ���o���������ʂ��i�[����̈�
			/// @param[in] branch �����ʂ𒊏o����}
			/// @param[in] ref_branch ��}
			virtual void extract_feature( FeatureType &feature, const_iterator branch, const_iterator ref_branch )const = 0;
			
			/// @brief �w��̎}�Ɋւ���Ή��t�����ʂ̕]���l���擾���鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @param[in] result_branch �Ή��t�����ʂ̎}
			/// @param[in] truth_branch �����̎}
			/// @return �]���l
			virtual double evaluate_result( const_iterator result_branch, const_iterator truth_branch )const = 0;
			
			/// @brief ���O�t���c���摜�𕜌����鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @param[out] labeled_centerline ���O�t���c���摜���i�[����̈�
			/// @param[in] branch ��������}
			virtual void restore_centerline( LabelImage &labeled_centerline, const_iterator branch )const = 0;
			
			/// @brief ���O�t���Ǎo����摜�̕���_�𕜌����鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @param[in] tubular �Ǎo����̃}�[�N�摜
			/// @param[out] labeled_tubular ���O�t���Ǎo����摜���i�[����̈�
			/// @param[in] branch �������镪��_�̐e�}
			virtual void restore_bifurcation( const MarkImage &tubular, LabelImage &labeled_tubular, const_iterator branch )const = 0;
			
			/// @brief XML����}��ǂݍ��ޏ������z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @param[in] xml �ǂݍ���XML
			/// @param[in] elem �ǂݍ��ޗv�f���w���C�e���[�^
			/// @param[out] branch �ǂݍ��ގ}���w���C�e���[�^
			/// @retval true �}�̓ǂݍ��݂ɐ�������
			/// @retval false �}�̓ǂݍ��݂Ɏ��s�����D���̂Ƃ��C�؍\���̓ǂݍ��݂͒��~�����
			virtual bool read_branch( const XML &xml, XML::const_iterator elem, iterator branch ) = 0;
			
			/// @brief XML�Ɏ}���������ޏ������z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @param[in] xml ��������XML
			/// @param[out] elem �������ޗv�f���w���C�e���[�^
			/// @param[in] branch �������ގ}���w���C�e���[�^
			/// @retval true �}�̏������݂ɐ�������
			/// @retval false �}�̏������݂Ɏ��s�����D���̂Ƃ��C�؍\���̏������݂͒��~�����
			virtual bool write_branch( XML &xml, XML::iterator elem, const_iterator branch )const = 0;
			
			/// @brief ���W�F�N�g�����擾���鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @return ���W�F�N�g��
			virtual NameType reject_name( )const = 0;
			
			/// @brief ���̐e�}�̖ޓx�Ƃ��Ĉ�����ޓx�̋K��l���擾���鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @return �ޓx�̋K��l
			virtual LikelihoodType default_likelihood( )const = 0;
			
			/// @brief ���W�F�N�g�����܂ޖ��O�̑������擾���鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @return ���O�̑���
			virtual size_t name_number( )const = 0;
			
			/// @brief ���O�𕶎���ɕϊ����鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @param[in] name �ϊ����閼�O
			/// @return ���O��\��������
			virtual std::string to_string( const NameType &name )const = 0;
			
			/// @brief ���O���C���f�b�N�X�ɕϊ����鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @attention �e���O��ϊ��������ʂ����ꂼ��0, 1, ..., name_number( ) - 1�ɂȂ�Ȃ���΂Ȃ�Ȃ�
			/// @param[in] name �ϊ����閼�O
			/// @return ���O�ɑΉ�����C���f�b�N�X
			virtual size_t to_index( const NameType &name )const = 0;
			
			/// @brief ������𖼑O�ɕϊ����鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @param[in] str �ϊ����镶����
			/// @return ������̕\�����O
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

		/// @brief ���얳��
		template < class ValueType >
		inline Image< ValueType >::Image( )
		{
		}
	
		/// @brief �摜�𕡐�����
		/// @param[in] img ��������摜
		template < class ValueType >
		inline Image< ValueType >::Image( const Image &img ) : tmatsu::Image< ValueType >( img )
		{
		}

		/// @brief �^�̈قȂ�摜�𕡐�����
		/// @param[in] img ��������摜
		template < class ValueType >
		template < class ImageType >
		inline Image< ValueType >::Image( const ImageType &img ) : tmatsu::Image< ValueType >( img )
		{
		}

		/// @brief �f�[�^�^�̈قȂ�摜�𕡐�����
		/// @param[in] img ��������摜
		template < class ValueType >
		template < class Type >
		inline Image< ValueType >::Image( const Image< Type > &img ) : tmatsu::Image< ValueType >( img )
		{
		}

		/// @brief �摜���t�@�C������ǂݍ���
		/// @param[in] callback �R�[���o�b�N�֐�
		/// @param[in] file_name �ǂݍ��ރt�@�C����
		/// @param[in] hdr_name �w�b�_�t�@�C�����D�K��l��NULL�DNULL���w�肳�ꂽ�ꍇ�Cfile_name��".header"��t�������w�b�_�t�@�C�������ݒ肳���
		template < class Type >
		template < class Functor >
		inline Image< Type >::Image( Functor callback, const char *file_name, const char *hdr_name ) : tmatsu::Image< ValueType >( callback, file_name, hdr_name )
		{
		}
	
		/// @brief �����l���L�^���ꂽ�c���摜���擾����
		/// @attention �o�͉摜�̃f�[�^�^�͕����_�����^�łȂ���΂Ȃ�Ȃ�
		/// @param[out] centerline �c�����i�[����摜
		template < class ValueType >
		template < class ImageType >
		inline void Image< ValueType >::thinning( ImageType &centerline )const
		{
			Image img; // �ꎞ�摜
			ImageType thickness; // �����摜

			// ����������
			centerline = *this;

			// ���𖄂߂�
			mist::remove_hole_region( *this, img );

			// �א�������
			TOPOLOGICAL( mist::euclidean::thinning )( img, centerline );

			// �����ϊ�����
			mist::euclidean::distance_transform( *this, thickness );
			img.clear( );

			// ������������
			for ( size_t i = 0; i < centerline.size( ); i++ )
			{
				if ( centerline[ i ] )
				{
					centerline[ i ] = static_cast< ImageType::value_type >( std::sqrt( thickness[ i ] ) * centerline.reso1( ) );
				}
			}
		}

		/// @brief �摜�𕡐�����
		/// @param[in] img ��������摜
		/// @return �������g�̎Q��
		template < class ValueType >
		inline Image< ValueType > &Image< ValueType >::operator=( const Image &img )
		{
			if ( this != &img )
			{
				tmatsu::Image< ValueType >::operator=( img );
			}

			return *this;
		}

		/// @brief �^�̈قȂ�摜�𕡐�����
		/// @param[in] img ��������摜
		/// @return �������g�̎Q��
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

		/// @brief �f�[�^�^�̈قȂ�摜�𕡐�����
		/// @param[in] img ��������摜
		/// @return �������g�̎Q��
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

		/// @brief ���얳��
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( )
		{
		}
	
		/// @brief �Ǎo����؍\���𕡐�����
		/// @param[in] tree ��������Ǎo����؍\��
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( const Tree &tree ) : siz_( tree.siz_ ), pitch_( tree.pitch_ ), tree< _Branch< ValueType, NameType, LikelihoodType > >( tree )
		{
		}
	
		/// @brief �c���摜����؍\�����\�z����
		/// @param[in] centerline �c���摜
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( const ThicknessImage &centerline )
		{
			construct( centerline );
		}
	
		/// @brief �t�@�C������؍\����ǂݍ���
		/// @param[in] file_name �ǂݍ���XML�t�@�C����
		/// @param[in] siz �摜�̃T�C�Y
		/// @param[in] pitch �摜�̃s�b�`
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline Tree< ValueType, NameType, LikelihoodType, FeatureType >::Tree( const char *file_name, const SizeSet &siz, const PitchSet &pitch )
		{
			read( file_name, siz, pitch );
		}

		/// @brief �c���摜����؍\�����\�z����
		/// @param[in] centerline �c���摜
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::construct( const ThicknessImage &centerline )
		{
			ThicknessImage temp = centerline; // �ꎞ�摜
			std::queue< std::tr1::tuple< size_t, Point, ThicknessImage::value_type > > queue; // �҂��s��
			size_t id = 1; // ���ʎq
	
			// �T�C�Y�ƃs�b�`��ݒ肷��
			siz_.x = centerline.width( );
			siz_.y = centerline.height( );
			siz_.z = centerline.depth( );
			pitch_.x = centerline.reso1( );
			pitch_.y = centerline.reso2( );
			pitch_.z = centerline.reso3( );

			// �؍\������ɂ���
			clear( );

			// �e�}�𑖍�����
			do
			{
				size_t parent_id = queue.empty( ) ? 0 : std::tr1::get< 0 >( queue.front( ) ); //�e�̎��ʎq
				Point start_pt = queue.empty( ) ? root_point( temp ) : std::tr1::get< 1 >( queue.front( ) ); //�n�_
				Point end_pt; // �I�_
				iterator parent_branch = end( ); // �e�}
				iterator branch; // ��������}
		
				// �����𕜌�����
				if ( !queue.empty( ) )
				{
					// �����𕜌�����
					temp( start_pt ) = std::tr1::get< 2 >( queue.front( ) );

					// �f�L���[����
					queue.pop( );
				}
		
				// �e�}��T������
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
		
				// �}��}������
				branch = insert( parent_branch, Branch( ) );
				branch->id = id++;
				branch->name = reject_name( );
				end_pt = traverse_branch( branch->value, temp, start_pt );
		
				// �q�����G���L���[
				for ( size_t i = 0; i < TOPOLOGY; i++ )
				{
					try
					{
						Point next_pt = end_pt + TOPOLOGICAL( neighborhood )[ i ]; // �q���̎}�̎n�_

						if ( temp( next_pt ) > 0 )
						{
							// ��������f�̂Ƃ�
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

			// ���ʎq������������
			for ( iterator iter = begin( ); iter != end( ); iter++ )
			{
				iter->id = 0;
			}

			// �㏈�����s��
			construct_postprocess( );

			// ���ʎq��U�蒼��
			id = 1;

			for ( iterator iter = preorder_begin( ); iter != preorder_end( ); iter++ )
			{
				iter->id = id++;
			}
		}

		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::construct(const ThicknessImage &centerline, const Point& seedPoint)
		{
			ThicknessImage temp = centerline; // �ꎞ�摜
			std::queue< std::tr1::tuple< size_t, Point, ThicknessImage::value_type > > queue; // �҂��s��
			size_t id = 1; // ���ʎq

			// �T�C�Y�ƃs�b�`��ݒ肷��
			siz_.x = centerline.width();
			siz_.y = centerline.height();
			siz_.z = centerline.depth();
			pitch_.x = centerline.reso1();
			pitch_.y = centerline.reso2();
			pitch_.z = centerline.reso3();

			// �؍\������ɂ���
			clear();

			// �e�}�𑖍�����
			do
			{
				size_t parent_id = queue.empty() ? 0 : std::tr1::get< 0 >(queue.front()); //�e�̎��ʎq
				Point start_pt = queue.empty() ? root_point(temp, seedPoint) : std::tr1::get< 1 >(queue.front()); //�n�_
				Point end_pt; // �I�_
				iterator parent_branch = end(); // �e�}
				iterator branch; // ��������}

				// �����𕜌�����
				if (!queue.empty())
				{
					// �����𕜌�����
					temp(start_pt) = std::tr1::get< 2 >(queue.front());

					// �f�L���[����
					queue.pop();
				}

				// �e�}��T������
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

				// �}��}������
				branch = insert(parent_branch, Branch());
				branch->id = id++;
				branch->name = reject_name();
				end_pt = traverse_branch(branch->value, temp, start_pt);

				// �q�����G���L���[
				for (size_t i = 0; i < TOPOLOGY; i++)
				{
					try
					{
						Point next_pt = end_pt + TOPOLOGICAL(neighborhood)[i]; // �q���̎}�̎n�_

						if (temp(next_pt) > 0)
						{
							// ��������f�̂Ƃ�
							queue.push(std::tr1::make_tuple(branch->id, next_pt, temp(next_pt)));
							temp(next_pt) = 0;
						}
					}

					catch (ThicknessImage::InvalidPoint)
					{
					}
				}
			} while (!queue.empty());

			// ���ʎq������������
			for (iterator iter = begin(); iter != end(); iter++)
			{
				iter->id = 0;
			}

			// �㏈�����s��
			construct_postprocess();

			// ���ʎq��U�蒼��
			id = 1;

			for (iterator iter = preorder_begin(); iter != preorder_end(); iter++)
			{
				iter->id = id++;
			}
		}

		/// @brief �����ʂ𒊏o����
		/// @param[in] features ���o���������ʂ��i�[����R���e�i�DSTL�R���e�i�Ƃ݂Ȃ��Ȃ���΂Ȃ�Ȃ�
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		template < class Container >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::extract( Container &features )const
		{
			const_iterator ref_branch = reference_branch( ); // ��}
		
			// �R���e�i����ɂ���
			features.clear( );

			// �����ʂ𒊏o����
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				FeatureType feature; // �����x�N�g��
				
				extract_feature( feature, iter, ref_branch );
				features.insert( features.end( ), feature );
			}
		}
		
		/// @brief �Ή��t�����s��
		/// @param[out] result �Ή��t�����ʂ��i�[����؍\��
		/// @param[in] likelihood_func �ޓx�֐��D�ȉ��̂悤�ȌĂяo�����ł���֐��Ƃ݂Ȃ��Ȃ���΂Ȃ�Ȃ�
		/// @code
		/// NameType name; // ���O
		/// FeatureType feature; // �����x�N�g��
		/// LikelihoodType likelihood // �ޓx
		/// 
		/// likelihood = likelihood_func( name, feature );
		/// @endcode
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		template < class Functor >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::labeling( Tree &result, Functor likelihood_func )const
		{
			std::set< NameType > preprocessed_names; // �O�����ɂ��Ή��t���閼�O
			std::set< NameType > names; // �ޓx�֐��ɂ��Ή��t���閼�O
			std::set< NameType > combined_names; // �S�Ă̖��O
			
			// �o�͂�����������
			result = *this;

			for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				iter->name = result.reject_name( );
			}

			// �O�������s��
			result.labeling_preprocess( );

			// �O�����ɂ��Ή��t����ꂽ���O�����߂�
			for ( const_iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				if ( iter->name != reject_name( ) )
				{
					preprocessed_names.insert( iter->name );
				}
			}

#ifdef _CALC_LIKELIHOOD
			// �ޓx���v�Z����
			for ( iterator iter = result.preorder_begin( ); iter != result.preorder_end( ); iter++ )
			{
				if ( iter->name == reject_name( ) )
				{
					FeatureType feature; // �����x�N�g��
					const_iterator ref_branch = result.reference_branch( ); // ��}
					LikelihoodType parent_likelihood; // �e�}�̖ޓx
			
					// ����������
					iter->likelihood.clear( );
			
					// �����ʂ𒊏o����
					result.extract_feature( feature, iter, ref_branch );

					// �e�}�̖ޓx���v�Z����
					if ( result.is_root( iter ) )
					{
						parent_likelihood = result.default_likelihood( );
					}
					else
					{
						double parent_sum = 0; // �e�}�̖ޓx�̑��a

						parent_likelihood = result.parent( iter )->likelihood;

						// �e�}�̖ޓx�̑��a���v�Z����
						for ( LikelihoodType::const_iterator parent_iter = parent_likelihood.begin( ); parent_iter != parent_likelihood.end( ); parent_iter++ )
						{
							parent_sum += parent_iter->second;
						}

						// �e�}�̖ޓx�𐳋K������
						if ( parent_sum )
						{
							for ( LikelihoodType::iterator parent_iter = parent_likelihood.begin( ); parent_iter != parent_likelihood.end( ); parent_iter++ )
							{
								parent_iter->second /= parent_sum;
							}
						}
					}	

					// �ޓx���Z�o����
					for ( LikelihoodType::const_iterator parent_iter = parent_likelihood.begin( ); parent_iter != parent_likelihood.end( ); parent_iter++ )
					{
						LikelihoodType likelihood = likelihood_func( parent_iter->first, feature ); // �ޓx
						double avg = 0, var = 0; // �ޓx�̓��v��
						size_t cnt = 0; // ���O�̐�

						// �ޓx�֐��ɂ��Ή��t���閼�O�����߁C�O�����őΉ��t����ꂽ���O�����O����
						for ( LikelihoodType::iterator child_iter = likelihood.begin( ); child_iter != likelihood.end( ); )
						{
							if ( preprocessed_names.find( child_iter->first ) != preprocessed_names.end( ) )
							{
								child_iter = likelihood.erase( child_iter );
							}
							else
							{
								// ���O��}������
								names.insert( child_iter->first );

								// �w���֐����Ƃ�
								//child_iter->second = std::exp( child_iter->second );

								// ���v�ʂ��v�Z����
								avg += child_iter->second;
								var += std::pow( child_iter->second, 2 );
								cnt++;

								// �O�i����
								child_iter++;
							}
						}

						avg /= cnt;
						var = var / cnt - std::pow( avg, 2 );

						// ���K������
						for ( LikelihoodType::iterator child_iter = likelihood.begin( ); child_iter != likelihood.end( ); child_iter++ )
						{
							child_iter->second = std::max( ( child_iter->second - avg ) / ( var ? std::sqrt( var ) : 1 ), 0.0 );
							//child_iter->second = std::exp( ( child_iter->second - avg ) / ( var ? std::sqrt( var ) : 1 ) );
						}

#ifdef _FIND_OPTIMUM_BRANCHING
						// �e���Ƃ̖ޓx���i�[����
						iter->plausibility.insert( Branch::PlausibilityType::value_type( parent_iter->first, likelihood ) );

						// �e�ޓx�𐳋K������
						for ( Branch::PlausibilityType::iterator plausibility_iter = iter->plausibility.begin( ); plausibility_iter != iter->plausibility.end( ); plausibility_iter++ )
						{
							double likelihood_sum = 0; // �ޓx�̑��a
							
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

						// �e�̖ޓx��������
						for ( LikelihoodType::iterator child_iter = likelihood.begin( ); child_iter != likelihood.end( ); child_iter++ )
						{
							child_iter->second *= parent_iter->second;
						}

						// �ޓx�����Z����
						iter->likelihood += likelihood;
					}
				}
				else
				{
					iter->likelihood.insert( iter->likelihood.end( ), LikelihoodType::value_type( iter->name, 1 ) );
				}
			}

			// �S�Ă̖��O�����߂�
			combined_names.insert( names.begin( ), names.end( ) );
			combined_names.insert( preprocessed_names.begin( ), preprocessed_names.end( ) );

			// ���K�����s��
			for ( std::set< NameType >::const_iterator name_iter = combined_names.begin( ); name_iter != combined_names.end( ); name_iter++ )
			{
				double likelihood_sum = 0; // �ޓx�̑��a
						
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
			FeatureType feature; // �����x�N�g��
			std::map< NameType, std::set< NameType > > integrity; // �\�ȕ���

			result.extract_feature( feature, result.begin( ), result.reference_branch( ) );

			// �\�ȕ�����v�Z����
			for ( std::set< NameType >::const_iterator iter = combined_names.begin( ); iter != combined_names.end( ); iter++ )
			{
				LikelihoodType likelihood = likelihood_func( *iter, feature ); // �ޓx
				std::map< NameType, std::set< NameType > >::iterator integrity_iter = integrity.insert( integrity.end( ), std::map< NameType, std::set< NameType > >::value_type( *iter, std::set< NameType >( ) ) ); //������

				for ( LikelihoodType::const_iterator likelihood_iter = likelihood.begin( ); likelihood_iter != likelihood.end( ); likelihood_iter++ )
				{
					integrity_iter->second.insert( likelihood_iter->first );
				}
			}
#endif //_CALC_INTEGRITY

#if defined( _UPDATE_LIKELIHOOD )
			double err = DBL_MAX; //�덷

			// ���K������
			for ( iterator iter = result.begin( ); iter != result.end( ); iter++ )
			{
				double likelihood_sum = 0; //�ޓx�̑��a
			
				for ( LikelihoodType::const_iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_sum += likelihood_iter->second;
				}
		
				for ( LikelihoodType::iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_iter->second /= likelihood_sum > 0 ? likelihood_sum : 1;
				}
			}

			// �ޓx���X�V
			size_t cnt = 0; // �J�E���^

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
						double parent_consistency = 1, child_consistency = 1, sibling_consistency = 1; // �������x
			
						// �e�Ƃ̖������x
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

						// �q���Ƃ̖������x
						for ( iterator child_iter = result.child_begin( iter ); child_iter != result.child_end( iter ); child_iter++ )
						{
							double consistency = 1; // �������x

							for ( LikelihoodType::const_iterator child_likelihood_iter = child_iter->prev_likelihood.begin( ); child_likelihood_iter != child_iter->prev_likelihood.end( ); child_likelihood_iter++ )
							{
								if ( integrity[ likelihood_iter->first ].find( child_likelihood_iter->first ) == integrity[ likelihood_iter->first ].end( ) )
								{
									consistency -= child_likelihood_iter->second;
								}
							}
								
							child_consistency *= consistency;
						}

						// �Z��Ƃ̖������x
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
					FeatureType feature; // �����x�N�g��
					double max_likelihood = -DBL_MAX; // �ő�ޓx
			
					// �����ʂ𒊏o����
					result.extract_feature( feature, iter, result.reference_branch( ) );

					// �ޓx���v�Z����
					LikelihoodType likelihood = likelihood_func( result.parent( iter )->name, feature ); //�ޓx

					for ( LikelihoodType::const_iterator likelihood_iter = likelihood.begin( ); likelihood_iter != likelihood.end( ); likelihood_iter++ )
					{
						if ( iter->candidate[ result.parent( iter )->name ].find( likelihood_iter->first ) != iter->candidate[ result.parent( iter )->name ].end( ) && max_likelihood < likelihood_iter->second )
						{
							iter->name = likelihood_iter->first;
							max_likelihood = likelihood_iter->second;
						}
					}
			
					// �����ӂ邢���Ƃ�
					if ( iter->name != result.reject_name( ) )
					{
						// ����̂Ƃ�
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
						// �����̂Ƃ�
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
				iterator max_branch = result.end( ); // �ő�ޓx�����}
				double max_likelihood = 0; // �ő�ޓx

				for (  iterator iter = result.begin( ); iter != result.end( ); iter++ )
				{
					if ( !result.is_root( iter ) && iter->name == result.reject_name( ) )
					{
						for ( std::map< NameType, std::set< NameType > >::const_iterator integrity_iter = integrity.begin( ); integrity_iter != integrity.end( ); integrity_iter++ )
						{
							double ancestor_likelihood = 0; // ��c�}�̖ޓx

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

			// �������������
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
			}

#elif defined( _SELECT_MODEL )
			tree< NameType > model; // ���f��
			const_iterator ref_branch = result.reference_branch( ); // �Q�Ǝ}

			// �Q�Ǝ}�̓��[���x�[�X�Ŗ��O���Ή��t�����Ă���Ɖ��肷��
			model.insert( model.end( ), ref_branch->name );

			// �Ƃ肤�镪��p�^�[����񋓂���
			for ( tree< NameType >::iterator model_iter = model.begin( ); model_iter != model.end( ); model_iter++ )
			{
				for ( std::set< NameType >::const_iterator integrity_iter = integrity[ *model_iter ].begin( ); integrity_iter != integrity[ *model_iter ].end( ); integrity_iter++ )
				{
					bool flag = true; // �t���O

					// ���ɑ��݂��閼�O�͕s��
					for ( tree< NameType >::const_iterator ancestor_iter = model_iter; ancestor_iter != model.end( ); ancestor_iter = model.parent( ancestor_iter ) )
					{
						if ( *ancestor_iter == *integrity_iter )
						{
							flag = false;
							break;
						}
					}

					// ���O��}������
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

			// �Q�Ǝ}�����Ƃ��镔���؂�ΏۂƂ���
			result.select_model( std::find( result.preorder_begin( ), result.preorder_end( ), ref_branch->name ), model.preorder_begin( ), model );
#elif defined( _FIND_OPTIMUM_BRANCHING )
			// �œK������v�Z����
			result.find_optimum_branching( combined_names, integrity );
#else
			// �ł������ޓx�������O��Ή��t����
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
			// ���������s��
			for ( std::set< NameType >::const_iterator iter = names.begin( ); iter != names.end( ); iter++ )
			{
			}
#endif //_CORRECT_ERROR

			// �㏈�����s��
			result.labeling_postprocess( );
		}

		/// @brief �Ή��t�����ʂ�]������
		/// @param[out] confusion ���ʂ��i�[����R���t���[�W�����}�g���b�N�X
		/// @param[in] truth ����؍\���D�e�q�֌W������łȂ���΂Ȃ�Ȃ�
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::evaluate( Matrix &confusion, const Tree &truth )const
		{
			// ����������
			confusion.resize( name_number( ), name_number( ) );
			confusion.fill( 0 );

			// �]������
			for ( const_iterator result_iter = begin( ), truth_iter = truth.begin( ); result_iter != end( ) && truth_iter != truth.end( ); result_iter++, truth_iter++ )
			{
#ifndef _EVALUATE_LIKELIHOOD_RANKING
#ifdef _EVALUATE_HEAD_ONLY
				bool exist = false; // ���݃t���O

				for ( const_iterator parent_iter = truth.parent( truth_iter ); parent_iter != truth.end( ); parent_iter = truth.parent( parent_iter ) )
				{
					if ( parent_iter->name == truth_iter->name )
					{
						exist = true;
						break;
					}
				}

				// �e�ɓ������O�����݂��Ȃ��}�̂ݕ]������
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
			
		/// @brief ���O�t���c���摜�𕜌�����
		/// @param[out] labeled_centerline ���O�t���c���摜���i�[����摜
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::restore( LabelImage &labeled_centerline )const
		{
			// ����������
			labeled_centerline.resize( siz_.x, siz_.y, siz_.z );
			labeled_centerline.reso( pitch_.x, pitch_.y, pitch_.z );
			labeled_centerline.fill( 0 );
		
			// ��������
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				restore_centerline( labeled_centerline, iter );
			}
		}
		
		/// @brief ���O�t���Ǎo����摜�𕜌�����
		/// @param[in] tubular �Ǎo����̈�摜
		/// @param[out] labeled_tubular ���O�t���Ǎo����摜���i�[����摜
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::restore( const MarkImage &tubular, LabelImage &labeled_tubular )const
		{
			std::list< Point > voxels; // �������ꂽ��f

			// �c���𕜌�����
			restore( labeled_tubular );

			// ����_�𕜌�����
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( !is_leaf( iter ) )
				{
					restore_bifurcation( tubular, labeled_tubular, iter );
				}
			}

			// �������ꂽ��f�����߂�
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

			// ��������
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

		/// @brief �t�@�C������؍\����ǂݍ���
		/// @param[in] file_name �ǂݍ���XML�t�@�C����
		/// @param[in] siz �摜�̃T�C�Y
		/// @param[in] pitch �摜�̃s�b�`
		/// @retval true �؍\���̓ǂݍ��݂ɐ�������
		/// @retval false �؍\���̓ǂݍ��݂Ɏ��s�����D���炭�C�t�@�C�����J���Ȃ��������܂��̓t�@�C���̓��e���s��������
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline bool Tree< ValueType, NameType, LikelihoodType, FeatureType >::read( const char *file_name, const SizeSet &siz, const PitchSet &pitch )
		{
			XML xml; // XML
			
			// XML��ǂݍ���
			if ( !xml.read( file_name ) )
			{
				return false;
			}
			
			// �G���[�`�F�b�N
			if ( xml.empty( ) || xml.root( ).tag( ).name( ) != "VESSEL" )
			{
				return false;
			}

			// �T�C�Y�ƃs�b�`��ݒ肷��
			siz_ = siz;
			pitch_ = pitch;

			// �؍\������ɂ���
			clear( );
			
			// XML����؍\����ǂݍ���
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

		/// @brief �t�@�C���֖؍\������������
		/// @param[in] file_name ��������XML�t�@�C����
		/// @retval true �؍\���̏������݂ɐ�������
		/// @retval false �؍\���̏������݂Ɏ��s�����D���炭�C�t�@�C�����J���Ȃ�����
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline bool Tree< ValueType, NameType, LikelihoodType, FeatureType >::write( const char *file_name )const
		{
			XML xml; // XML

			// ���[�g�v�f��ǉ�����
			xml.insert( xml.begin( ), XML::Element( "VESSEL" ) );

			// �e�}����������
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				XML::iterator branch_elem = xml.insert( xml.end( ), XML::Element( "BRANCH" ) ); // BRANCH�v�f

				// ��������������
				branch_elem->tag( ).insert( branch_elem->tag( ).end( ), XML::Element::Tag::value_type( "id", std::to_string( iter->id ) ) );
				branch_elem->tag( ).insert( branch_elem->tag( ).end( ), XML::Element::Tag::value_type( "name", to_string( iter->name ) ) );

				// �e����������
				if( !is_root( iter ) )
				{
					XML::iterator parent_elem = xml.insert( branch_elem, XML::Element( "PARENT" ) ); // PARENT�v�f

					branch_elem = xml.parent( parent_elem );
					parent_elem->tag( ).insert( parent_elem->tag( ).end( ), XML::Element::Tag::value_type( "id", std::to_string( parent( iter )->id ) ) );
				}

				// �q������������
				for ( const_iterator child_iter = child_begin( iter ); child_iter != child_end( iter ); child_iter++ )
				{
					XML::iterator child_elem = xml.insert( branch_elem, XML::Element( "CHILD" ) ); // CHILD�v�f

					branch_elem = xml.parent( child_elem );
					child_elem->tag( ).insert( child_elem->tag( ).end( ), XML::Element::Tag::value_type( "id", std::to_string( child_iter->id ) ) );
				}

				// �}����������
				if ( !write_branch( xml, branch_elem, iter ) )
				{
					return false;
				}
			}

			return xml.write( file_name );
		}

		/// @brief �摜�̃T�C�Y���擾����
		/// @return �摜�̃T�C�Y�̑g
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline const SizeSet &Tree< ValueType, NameType, LikelihoodType, FeatureType >::get_size( )const
		{
			return siz_;
		}

		/// @brief �摜�̃T�C�Y��ݒ肷��
		/// @param[in] siz �ݒ肷��摜�̃T�C�Y�̑g
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::set_size( const SizeSet &siz )
		{
			siz_ = siz;
		}

		/// @brief �摜�̃s�b�`���擾����
		/// @return �摜�̃s�b�`�̑g
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline const PitchSet &Tree< ValueType, NameType, LikelihoodType, FeatureType >::get_pitch( )const
		{
			return pitch_;
		}

		/// @brief �摜�̃s�b�`��ݒ肷��
		/// @param[in] pitch �ݒ肷��摜�̃s�b�`�̑g
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::set_pitch( const PitchSet &pitch )
		{
			pitch_ = pitch;
		}
	
		/// @brief �؍\���\�z�̌㏈�����s�����z�֐�
		/// @attention �f�t�H���g�̓���͂Ȃ����C�I�[�o�[���C�h���邱�Ƃŋ@�\���g�����邱�Ƃ��ł���
		/// @attention ���̊֐����Ă΂�鎞�_�ŁC�}�̎��ʎq�ɂ͖����Ȓl���i�[����Ă���
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::construct_postprocess( )
		{
		}
	
		/// @brief �Ή��t���̑O�������s�����z�֐�
		/// @attention �f�t�H���g�̓���͂Ȃ����C�I�[�o�[���C�h���邱�Ƃŋ@�\���g�����邱�Ƃ��ł���
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::labeling_preprocess( )
		{
		}
	
		/// @brief �Ή��t���̌㏈�����s�����z�֐�
		/// @attention �f�t�H���g�̓���͂Ȃ����C�I�[�o�[���C�h���邱�Ƃŋ@�\���g�����邱�Ƃ��ł���
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::labeling_postprocess( )
		{
		}

#ifdef _FIND_MAX_LIKELIHOOD
		/// @brief �ޓx���ő�ƂȂ�o�H��T�������̏I�[�}�ƌo�H��̍ő�ޓx�̑g��Ԃ�
		/// @param[in] name ���ڂ��閼�O
		/// @param[in] branch �T�����镔���؂̍����w���C�e���[�^
		/// @return �ޓx���ő�ƂȂ�o�H�̏I�[�}�ƌo�H��̍ő�ޓx�̑g
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline std::pair< typename Tree< ValueType, NameType, LikelihoodType, FeatureType >::iterator, double > Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_max_likelihood( const NameType &name, iterator branch )
		{
			std::pair< iterator, double > max_likelihood( branch, 0 ); // �ޓx���ő�ƂȂ�o�H�̏I�[�}�ƌo�H��̍ő�ޓx�̑g

			// �q����T������
			for ( iterator iter = child_begin( branch ); iter != child_end( branch ); iter++ )
			{
				std::pair< iterator, double > temp = find_max_likelihood( name, iter ); // �ꎞ�ϐ�

				if ( max_likelihood.second < temp.second )
				{
					max_likelihood = temp;
				}
			}

			// �ő�ޓx���X�V����
			if ( branch->likelihood.find( name ) != branch->likelihood.end( ) && max_likelihood.second < branch->likelihood[ name ] )
			{
				max_likelihood.second = branch->likelihood[ name ];
			}

			return max_likelihood;
		}
#endif //_FIND_MAX_LIKELIHOOD

#ifdef _SELECT_MODEL
		/// @brief ���f���I���Ɋ�Â��Ή��t�����s��
		/// @param[inout] branch �Ή��t�����s���}���w���C�e���[�^
		/// @param[in] selected_model ���ݑI������Ă��郂�f�����w���C�e���[�^
		/// @param[in] model ���f���؍\��
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::select_model( iterator branch, typename tree< NameType >::const_iterator selected_model, const tree< NameType > &model )
		{
			tree< NameType >::const_iterator max_model = selected_model; // �ő�X�R�A�������f��
			double max_score = -DBL_MAX; // �ő�X�R�A
			double ancestor_score = calc_ancestor_score( branch, *selected_model ); // ��c�X�R�A

			// ����X�R�A����������
			for ( tree< NameType >::const_iterator child_iter = model.child_begin( selected_model ); child_iter != model.child_end( selected_model ); child_iter++ )
			{
				double descendant_score = find_max_descendant_score( branch, child_iter, model ); // �q���X�R�A
				double self_score = calc_self_score( branch, *child_iter ); // ���g�X�R�A
						
				if ( max_score < ( ancestor_score + descendant_score ) * self_score )
				{
					max_model = child_iter;
					max_score = ( ancestor_score + descendant_score ) * self_score;
				}
			}
	
			// �����X�R�A����������
			{
				double descendant_score = find_max_descendant_score( branch, selected_model, model ); // �q���X�R�A
				double self_score = calc_self_score( branch, *selected_model ); // ���g�X�R�A

				if ( max_score < descendant_score * self_score )
				{
					max_model = selected_model;
				}
			}

			// �Ή��t�����s��
			if ( branch->name == reject_name( ) )
			{
				branch->name = *max_model;
			}

			// �q�}�̑Ή��t�����s��
			for ( iterator child_iter = child_begin( branch ); child_iter != child_end( branch ); child_iter++ )
			{
				select_model( child_iter, max_model, model );
			}
		}
	
		/// @brief �ő�̎q���X�R�A�����߂�
		/// @param[in] branch 
		/// @param[in] selected_model 
		/// @param[in] model 
		/// @return �ő�̎q���X�R�A
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_max_descendant_score( iterator branch, typename tree< NameType >::const_iterator selected_model, const tree< NameType > &model )
		{
			double max_score = model.is_leaf( selected_model ) ? 1 : 0; // �ő�X�R�A

			// �q����T������
			for ( tree< NameType >::const_iterator child_iter = model.child_begin( selected_model ); child_iter != model.child_end( selected_model ); child_iter++ )
			{	
				double score = find_max_descendant_score( branch, child_iter, model ); // �q���̍ő�X�R�A
				double descendant_score = calc_descendant_score( branch, *child_iter ); // �X�R�A

				if ( max_score < score + descendant_score )
				{
					max_score = score + descendant_score;
				}
			}

			return max_score;
		}

		/// @brief �w��̎}�̎w��̖��O�Ɋւ��鎩�g�X�R�A���v�Z����
		/// @param[in] branch �X�R�A���v�Z����}���w���C�e���[�^
		/// @param[in] name �X�R�A���v�Z���閼�O
		/// @return ���g�X�R�A
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_self_score( iterator branch, NameType name )
		{
			double pos = 0, neg = 0; // �X�R�A

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

		/// @brief �w��̎}�̎w��̖��O�Ɋւ����c�X�R�A���v�Z����
		/// @param[in] branch �X�R�A���v�Z����}���w���C�e���[�^
		/// @param[in] name �X�R�A���v�Z���閼�O
		/// @return ��c�X�R�A
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_ancestor_score( iterator branch, NameType name )
		{
			double pos = 0, neg = 0; //�X�R�A

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

		/// @brief �w��̎}�̎w��̖��O�Ɋւ���q���X�R�A���v�Z����
		/// @param[in] branch �X�R�A���v�Z����}���w���C�e���[�^
		/// @param[in] name �X�R�A���v�Z���閼�O
		/// @return �q���X�R�A
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_descendant_score( iterator branch, NameType name )
		{
			double pos = 0, neg = 0; //�X�R�A

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
		/// @brief ����\���v�Z����
		/// @param[in,out] branch ����\���v�Z����}
		/// @param[in] names ���O�̏W��
		/// @param[in] integrity ������
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_table( iterator branch, std::set< NameType > names, std::map< NameType, std::set< NameType > > &integrity )
		{
			// �e���O�ɂ��ĕ���\���v�Z����
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				Branch::BifurcationType temp_bifurcation; // ������ꎞ�I�Ɋi�[����̈�
				std::set< Branch::BifurcationType > bifurcation_candidates; // ����̌��
				std::set< Branch::BifurcationType >::iterator max_bifurcation; // �X�R�A���ő剻���镪��
				double max_score = 0; // �ő�X�R�A

				// ���W�F�N�g���͌v�Z���Ȃ�
				if ( *name_iter == reject_name( ) )
				{
					continue;
				}
				
				// ������v�Z����
				for ( iterator iter = subtree( branch ).begin( ); iter != subtree( branch ).end( ); iter++ )
				{
					bifurcation_candidates.insert( calc_bifurcation( branch, iter, *name_iter, integrity, temp_bifurcation ) );
				}
				
				// �X�R�A���ő剻���镪���}������
				max_bifurcation = bifurcation_candidates.begin( );
				
				for ( std::set< Branch::BifurcationType >::iterator bifurcation_iter = bifurcation_candidates.begin( ); bifurcation_iter != bifurcation_candidates.end( ); bifurcation_iter++ )
				{
					double score = calc_score( *bifurcation_iter ); // �X�R�A

					if ( max_score < score )
					{
						max_bifurcation = bifurcation_iter;
						max_score = score;
					}
				}

				branch->bifurcation_table.insert( Branch::BifurcationTableType::value_type( *name_iter, *max_bifurcation ) );
			}
		}
			
		/// @brief �w��̌��ǂ��w��̎}����w��̎q���̎}�ɉ�������ꍇ�̕�������߂�
		/// @param[in] branch ��������߂�}
		/// @param[in] descendant_branch ��������q���̎}
		/// @param[in] name �������錌��
		/// @param[in] integrity �\�ȕ���
		/// @param[out] descendant ������i�[����R���e�i
		/// @return �w��̃R���e�i�̎Q��
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline typename Tree< ValueType, NameType, LikelihoodType, FeatureType >::Branch::BifurcationType &Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_bifurcation( iterator branch, iterator descendant_branch, NameType name, std::map< NameType, std::set< NameType > > &integrity, typename Branch::BifurcationType &bifurcation )
		{
			// �\�[�g�̂��߂̔�r�֐�
			struct SortPred
			{
				// �֐��Ăяo�����Z�q
				bool operator( )( const std::tr1::tuple< NameType, size_t, double > &l, const std::tr1::tuple< NameType, size_t, double > &r )
				{
					return std::get< 2 >( l ) > std::get< 2 >( r );
				}
			};

			// �폜�̂��߂̔�r�֐�
			class RemovePred
			{
				NameType name_; // �폜���閼�O
				size_t id_; // �폜���鎯�ʎq
			public:
				// �R���X�g���N�^
				RemovePred( NameType name, size_t id ) : name_( name ), id_( id )
				{
				}

				//�@�֐��Ăяo�����Z�q
				bool operator( )( const std::tr1::tuple< NameType, size_t, double > &a )
				{
					return std::get< 0 >( a ) == name_ || std::get< 1 >( a ) == id_;
				}
			};

			std::list< std::tr1::tuple< NameType, size_t, double > > scores; // �X�R�A
			
			// ����\������������
			bifurcation.clear( );

			// �I�[�X�R�A�����߂�
			bifurcation.insert( Branch::BifurcationType::value_type( name, std::make_pair( descendant_branch->id, calc_tl( descendant_branch, name ) ) ) );
			
			// ����X�R�A�����߂�
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
		
		/// @brief �w��̎}���w��̌��ǂ̏I�[�}�ƂȂ�X�R�A���v�Z����
		/// @param[in] branch �X�R�A���v�Z����}
		/// @param[in] name �X�R�A���v�Z���錌��
		/// @return �w��̎}���w��̌��ǂ̏I�[�}�ƂȂ�X�R�A
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline double Tree< ValueType, NameType, LikelihoodType, FeatureType >::calc_tl( iterator branch, NameType name )
		{
#ifdef _IGNORE_TAIL_SCORE
			return 0;
#endif //_IGNORE_TAIL_SCORE

			if ( branch->name == reject_name( ) )
			{
				double ext_max = 0; // �����X�R�A�̍ő�
					
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
			
		/// @brief �w��̎}���w��̌��ǂ̊J�n�}�ƂȂ�X�R�A���v�Z����
		/// @param[in] branch �X�R�A���v�Z����}
		/// @param[in] name �X�R�A���v�Z���錌��
		/// @param[in] parent_name �w��̎}�̐e�ɕt�^����錌�ǖ�
		/// @return �w��̎}���w��̌��ǂ̊J�n�}�ƂȂ�X�R�A
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
		
		/// @brief ����̃X�R�A���v�Z����
		/// @param[in] �X�R�A���v�Z���镪��
		/// @return �w��̕���̃X�R�A
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
			
		/// @brief �œK��������߂邱�Ƃɂ�郉�x�����O���s��
		/// @param[in] names ���O�̏W��
		/// @param[in] integrity ������
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_optimum_branching( std::set< NameType > names, std::map< NameType, std::set< NameType > > &integrity )
		{
			// �e�}�̖ޓx�𐳋K������
			for ( iterator iter = begin( ); iter != end( ); iter++ )
			{
				double likelihood_sum = 0; // �ޓx�̑��a
	
				for ( LikelihoodType::const_iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_sum += likelihood_iter->second;
				}
		
				for ( LikelihoodType::iterator likelihood_iter = iter->likelihood.begin( ); likelihood_iter != iter->likelihood.end( ); likelihood_iter++ )
				{
					likelihood_iter->second /= likelihood_sum > 0 ? likelihood_sum : 1;
				}
			}

			// ����\���v�Z����
			for ( iterator iter = postorder_begin( ); iter != postorder_end( ); iter++ )
			{
				calc_table( iter, names, integrity );
			}

			// ����O���t�����߂�
			tree< std::pair< NameType, size_t > > bifurcation_tree; // ����c���[
			Branch::BifurcationTableType bifurcation_graph, optimum_branching; // ����O���t
			const_iterator ref_branch = reference_branch( ); // �Q�Ǝ}

			// �Q�Ǝ}�͑Ή��Ώە����؂̍��ł��胋�[���x�[�X�Ŗ��O���Ή��t�����Ă���Ɖ��肵����c���[�̍���}������
			bifurcation_tree.insert( bifurcation_tree.end( ), std::make_pair( ref_branch->name, ref_branch->id ) );
			
			// �Ƃ肤�镪��p�^�[����񋓂���
			for ( tree< std::pair< NameType, size_t > >::iterator tree_iter = bifurcation_tree.preorder_begin( ); tree_iter != bifurcation_tree.preorder_end( ); tree_iter++ )
			{
				iterator start_branch = std::find( begin( ), end( ), tree_iter->second ); // ���ݒ��ڂ��Ă��錌�ǂ̊J�n�}

				// �����}������
				for ( std::set< NameType >::const_iterator integrity_iter = integrity[ tree_iter->first ].begin( ); integrity_iter != integrity[ tree_iter->first ].end( ); integrity_iter++ )
				{
					bool flag = true; // �t���O

					// ���W�F�N�g���͑}�����Ȃ�
					if ( *integrity_iter == reject_name( ) )
					{
						continue;
					}

					// ���ɑ��݂��閼�O�͑}�����Ȃ�
					for ( tree< std::pair< NameType, size_t > >::const_iterator ancestor_iter = tree_iter; ancestor_iter != bifurcation_tree.end( ); ancestor_iter = bifurcation_tree.parent( ancestor_iter ) )
					{
						if ( ancestor_iter->first == *integrity_iter )
						{
							flag = false;
							break;
						}
					}

					// ���O��}������
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

			// ����O���t�����߂�
			for ( std::map< NameType, std::set< NameType > >::const_iterator integrity_parent_iter = integrity.begin( ); integrity_parent_iter != integrity.end( ); integrity_parent_iter++ )
			{
				Branch::BifurcationType bifurcation; // ����

				// ���W�F�N�g���̓O���t�Ɋ܂߂Ȃ�
				if ( integrity_parent_iter->first == reject_name( ) )
				{
					continue;
				}

				for ( std::set< NameType >::const_iterator integrity_child_iter = integrity_parent_iter->second.begin( ); integrity_child_iter != integrity_parent_iter->second.end( ); integrity_child_iter++ )
				{
					std::set< std::pair< double, double > > scores; // �X�R�A�̏W��
					double wt_sum = 0; // �d�݂̑��a
					double score_sum = 0; // �X�R�A�̉��d�a

					// ���W�F�N�g���Ǝ��ȃ��[�v�̓O���t�Ɋ܂߂Ȃ�
					if ( *integrity_child_iter == reject_name( ) || integrity_parent_iter->first == *integrity_child_iter )
					{
						continue;
					}

					// �Ώۂ̕���p�^�[�������m�[�h�ɂ��ăX�R�A���擾����
					for ( tree< std::pair< NameType, size_t > >::const_iterator tree_iter = bifurcation_tree.begin( ); tree_iter != bifurcation_tree.end( ); tree_iter++ )
					{
						if ( !bifurcation_tree.is_root( tree_iter ) && bifurcation_tree.parent( tree_iter )->first == integrity_parent_iter->first && tree_iter->first == *integrity_child_iter )
						{
							double wt = bifurcation_tree.depth( tree_iter ) < 2 ? 1.0 : std::find( begin( ), end( ), bifurcation_tree.parent( tree_iter )->second )->plausibility[ bifurcation_tree.parent( bifurcation_tree.parent( tree_iter ) )->first ][ integrity_parent_iter->first ]; // �d��
							double score = std::find( begin( ), end( ), bifurcation_tree.parent( tree_iter )->second )->bifurcation_table[ integrity_parent_iter->first ][ *integrity_child_iter ].second; // �X�R�A

							scores.insert( std::make_pair( wt, score ) );
						}
					}

					// �d�݂𐳋K������
					for ( std::set< std::pair< double, double > >::const_iterator score_iter = scores.begin( );score_iter != scores.end( ); score_iter++ )
					{
						wt_sum += score_iter->first;
					}

					// �X�R�A�̉��d�a�����߂�
					for ( std::set< std::pair< double, double > >::const_iterator score_iter = scores.begin( );score_iter != scores.end( ); score_iter++ )
					{
						score_sum += score_iter->second;// score_iter->first * score_iter->second / ( wt_sum > 0 ? wt_sum : 1 );
					}

					// �����}������
					bifurcation.insert( bifurcation.end( ), Branch::BifurcationType::value_type( *integrity_child_iter, std::make_pair( 0, score_sum ) ) );
				}

				bifurcation_graph.insert( bifurcation_graph.end( ), Branch::BifurcationTableType::value_type( integrity_parent_iter->first, bifurcation ) );
			}

			// ����O���t�̍œK��������߂�
			find_optimum_branching( bifurcation_graph, optimum_branching );

			// �Ή��t�����s��
			std::stack< iterator > stack; // �Ή��t�����s���}�̃X�^�b�N

			stack.push( std::find( begin( ), end( ), ref_branch->id ) );

			while ( !stack.empty( ) )
			{
				iterator tgt_branch = stack.top( ); // �Ή��t�����s�����ǂ̍ŏ��̎}

				// �X�^�b�N����}���|�b�v����
				stack.pop( );

				// �Ή��t�����s��
				for ( iterator iter = std::find( begin( ), end( ), tgt_branch->bifurcation_table[ tgt_branch->name ][ tgt_branch->name ].first ); iter != parent( tgt_branch ); iter = parent( iter ) )
				{
					iter->name = tgt_branch->name;
				}

				// �Ή��t�������ǂ��番�򂷂錌�ǂ��v�b�V������
				for ( Branch::BifurcationType::const_iterator bifurcation_iter = optimum_branching[ tgt_branch->name ].begin( ); bifurcation_iter != optimum_branching[ tgt_branch->name ].end( ); bifurcation_iter++ )
				{
					if ( tgt_branch->bifurcation_table[ tgt_branch->name ].find( bifurcation_iter->first ) != tgt_branch->bifurcation_table[ tgt_branch->name ].end( ) )
					{
						iterator next_branch = std::find( begin( ), end( ), tgt_branch->bifurcation_table[ tgt_branch->name ][ bifurcation_iter->first ].first ); // ���ɑΉ��t�����s���}

						next_branch->name = bifurcation_iter->first;
						stack.push( next_branch );
					}
				}
			}

			// ��������
#ifdef _IGNORE_TAIL_SCORE
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				std::stack< iterator > stack; // �}�̃X�^�b�N

				// �}���X�^�b�N�ɐς�
				for ( iterator iter = find_max_plausibility( *name_iter, std::find( postorder_begin( ), postorder_end( ), *name_iter ) ).first; iter != end( ) && iter->name == reject_name( ); iter = parent( iter ) )
				{
					stack.push( iter );
				}

				// ���ǂ���������
				for ( ; !stack.empty( ); stack.pop( ) )
				{
					bool cont = !is_root( stack.top( ) ) && parent( stack.top( ) )->name == *name_iter; // �p���t���O

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
			
		/// @brief �œK��������߂�
		/// @param[in] directed_graph �œK��������߂�L���O���t
		/// @param[out] optimum_branching �w��̃O���t�̍œK����
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline void Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_optimum_branching( const typename Branch::BifurcationTableType &directed_graph, typename Branch::BifurcationTableType &optimum_branching )const
		{
			mist::matrix< double > graph_mat( name_number( ), name_number( ) ), branching_mat; // �אڍs��

			// ����O���t����אڍs����쐬����
			graph_mat.fill( -1 );

			for ( Branch::BifurcationTableType::const_iterator parent_iter = directed_graph.begin( ); parent_iter != directed_graph.end( ); parent_iter++ )
			{
				for ( Branch::BifurcationType::const_iterator child_iter = parent_iter->second.begin( ); child_iter != parent_iter->second.end( ); child_iter++ )
				{
					graph_mat( to_index( parent_iter->first ), to_index( child_iter->first ) ) = child_iter->second.second;
				}
			}

			// �G�h�����Y�@��K�p����
			edmonds( graph_mat, branching_mat, std::greater< double >( ) );

			// �אڍs�񂩂�œK������쐬����
			optimum_branching.clear( );

			for ( Branch::BifurcationTableType::const_iterator parent_iter = directed_graph.begin( ); parent_iter != directed_graph.end( ); parent_iter++ )
			{
				Branch::BifurcationType bifurcation; // ����

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
		/// @brief �ޓx���ő�ƂȂ�o�H��T�������̏I�[�}�ƌo�H��̍ő�ޓx�̑g��Ԃ�
		/// @param[in] name ���ڂ��閼�O
		/// @param[in] branch �T�����镔���؂̍����w���C�e���[�^
		/// @return �ޓx���ő�ƂȂ�o�H�̏I�[�}�ƌo�H��̍ő�ޓx�̑g
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline std::pair< typename Tree< ValueType, NameType, LikelihoodType, FeatureType >::iterator, double > Tree< ValueType, NameType, LikelihoodType, FeatureType >::find_max_plausibility( const NameType &name, iterator branch )
		{
			std::pair< iterator, double > max_plausibility( branch, 0 ); // �ޓx���ő�ƂȂ�o�H�̏I�[�}�ƌo�H��̍ő�ޓx�̑g

			// �q����T������
			for ( iterator iter = child_begin( branch ); iter != child_end( branch ); iter++ )
			{
				std::pair< iterator, double > temp = find_max_plausibility( name, iter ); // �ꎞ�ϐ�

				if ( max_plausibility.second < temp.second )
				{
					max_plausibility = temp;
				}
			}

			// �ő�ޓx���X�V����
			if ( branch != end( ) && max_plausibility.second < branch->plausibility[ name ][ name ] )
			{
				max_plausibility.second = branch->plausibility[ name ][ name ];
			}

			return max_plausibility;
		}
#endif //_IGNORE_TAIL_SCORE
#endif //_FIND_OPTIMUM_BRANCHING

		/// @brief XML����}��ǂݍ���
		/// @param[in] xml �ǂݍ���XML
		/// @param[in] id �ǂݍ��ގ}�̎��ʎq
		/// @param[out] branch �ǂݍ��ގ}���w���C�e���[�^
		/// @retval true �}�̓ǂݍ��݂ɐ�������
		/// @retval false �}�̓ǂݍ��݂Ɏ��s�����D���炭�CXML�̓��e���s��������
		template < class ValueType, class NameType, class LikelihoodType, class FeatureType >
		inline bool Tree< ValueType, NameType, LikelihoodType, FeatureType >::read( const XML &xml, const std::string &id, iterator branch )
		{
			// �v�f�T���֐�
			class FindElem
			{
				std::string tag_name_; // �^�O��
				std::string id_; // ���ʎq
			public:
				// �R���X�g���N�^
				FindElem( const std::string &tag_name, const std::string &id ) : tag_name_( tag_name ), id_( id )
				{
				}
		
				// �֐��Ăяo�����Z�q
				bool operator( )( const XML::Element &elem )
				{
					return elem.tag( ).name( ) == tag_name_ && elem.tag( )[ "id" ] == id_;
				}
			};

			try
			{
				XML::const_iterator branch_elem = std::find_if( xml.child_begin( xml.preorder_begin( ) ), xml.child_end( xml.preorder_begin( ) ), FindElem( "BRANCH", id ) ); //BRANCH�v�f
				
				// �G���[�`�F�b�N
				if ( branch_elem == xml.child_end( xml.preorder_begin( ) ) )
				{
					return false;
				}
				
				// ������ǂݍ���
				branch->id = std::stoi( branch_elem->tag( )[ "id" ] );
				branch->name = to_name( branch_elem->tag( )[ "name" ] );
				
				// �q����ǂݍ���
				for( XML::const_iterator child_iter = std::find( xml.child_begin( branch_elem ), xml.child_end( branch_elem ), "CHILD" ); child_iter != xml.child_end( branch_elem ); child_iter = std::find( ++child_iter, xml.child_end( branch_elem ), "CHILD" ) )
				{
					iterator child_branch = insert( branch, value_type( ) ); // �q��
						
					// �q����ǂݍ���
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
		
		/// @brief �}���w��̎��ʎq�����������肷��
		/// @param[in] id ���肷�鎯�ʎq
		/// @retval true �}�͎w��̎��ʎq������
		/// @retval false �}�͎w��̎��ʎq�������Ȃ�
		template < class ValueType, class NameType, class LikelihoodType >
		inline bool _Branch< ValueType, NameType, LikelihoodType >::operator==( size_t id )const
		{
			return _Branch::id == id;
		}
		
		/// @brief �}���w��̖��O�����������肷��
		/// @param[in] name ���肷�閼�O
		/// @retval true �}�͎w��̖��O������
		/// @retval false �}�͎w��̖��O�������Ȃ�
		template < class ValueType, class NameType, class LikelihoodType >
		inline bool _Branch< ValueType, NameType, LikelihoodType >::operator==( const NameType &name )const
		{
			return _Branch::name == name;
		}
	}
}

#endif //_TMATSU_TUBULAR_H_