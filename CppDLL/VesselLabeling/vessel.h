/**
* @file vessel.h
* @brief ���ǖ��Ή��t���̃w�b�_�t�@�C��
* @version 3.0
* @author ���É���w ���Ȋw������ ���f�B�A�Ȋw��U �X������   351203192   ���� �N�N
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

		/// @brief �����ʂ�\���񋓌^
		enum Feature
		{
			FEATURE_LENGTH, ///< @brief ����
			FEATURE_DISTANCE, ///< @brief ������̋���
			//FEATURE_DISTANCE_BIFURCATION, ///< @brief ����_����̋���
			FEATURE_CHILD_DISTANCE, ///< @brief �q���̍Œ��o�H�̋���
			FEATURE_SIBLING_DISTANCE, ///< @brief �Z��̍Œ��o�H�̋���
			FEATURE_CHILD_BIFURCATION, ///< @brief �q���̕���
			FEATURE_SIBLING_BIFURCATION, ///< @brief �Z��̕���
			FEATURE_CURVATURE, ///< @brief �p�ȓx
			FEATURE_THICKNESS, ///< @brief ����
			FEATURE_PARENT_THICKNESS, ///< @brief �e�̑���
			FEATURE_SIBLING_THICKNESS, ///< @brief �Z��̑����̕���
			FEATURE_SIBLING_THICKNESS_SD, ///< @brief �Z��̑����̕W���΍�
			FEATURE_SIBLING_THICKNESS_MAX, ///< @brief �Z��̑����̍ő�l
			FEATURE_SIBLING_THICKNESS_MIN, ///< @brief �Z��̑����̍ŏ��l
			FEATURE_CHILD_THICKNESS, ///< @brief �q���̑����̕���
			FEATURE_CHILD_THICKNESS_SD, ///< @brief �q���̑����̕W���΍�
			FEATURE_CHILD_THICKNESS_MAX, ///< @brief �q���̑����̍ő�l
			FEATURE_CHILD_THICKNESS_MIN, ///< @brief �q���̑����̍ŏ��l
			//FEATURE_ANGLE, ///< @brief ���Ƃ̊p�x
			//FEATURE_PARENT_ANGLE, ///< @brief �e�Ƃ̊p�x
			//FEATURE_SIBLING_ANGLE, ///< @brief �Z��Ƃ̊p�x�̕���
			//FEATURE_SIBLING_ANGLE_SD, ///< @brief �Z��Ƃ̊p�x�̕W���΍�
			//FEATURE_SIBLING_ANGLE_MAX, ///< @brief �Z��Ƃ̊p�x�̍ő�l
			//FEATURE_SIBLING_ANGLE_MIN, ///< @brief �Z��Ƃ̊p�x�̍ŏ��l
			//FEATURE_CHILD_ANGLE, ///< @brief �q���Ƃ̊p�x�̕���
			//FEATURE_CHILD_ANGLE_SD, ///< @brief �q���Ƃ̊p�x�̕W���΍�
			//FEATURE_CHILD_ANGLE_MAX, ///< @brief �q���Ƃ̊p�x�̍ő�l
			//FEATURE_CHILD_ANGLE_MIN, ///< @brief �q���Ƃ̊p�x�̍ŏ��l
			FEATURE_STARTING_POS_X, ///< @brief ������̎n�_�ʒu��x����
			FEATURE_STARTING_POS_Y, ///< @brief ������̎n�_�ʒu��y����
			FEATURE_STARTING_POS_Z, ///< @brief ������̎n�_�ʒu��z����
			FEATURE_ENDING_POS_X, ///< @brief ������̏I�_�ʒu��x����
			FEATURE_ENDING_POS_Y, ///< @brief ������̏I�_�ʒu��y����
			FEATURE_ENDING_POS_Z, ///< @brief ������̏I�_�ʒu��z����
			//FEATURE_STARTING_POS_BRANCH_X, ///< @brief ����_����̎n�_�ʒu��x����
			//FEATURE_STARTING_POS_BRANCH_Y, ///< @brief ����_����̎n�_�ʒu��y����
			//FEATURE_STARTING_POS_BRANCH_Z, ///< @brief ����_����̎n�_�ʒu��z����
			//FEATURE_ENDING_POS_BRANCH_X, ///< @brief ����_����̏I�_�ʒu��x����
			//FEATURE_ENDING_POS_BRANCH_Y, ///< @brief ����_����̏I�_�ʒu��y����
			//FEATURE_ENDING_POS_BRANCH_Z, ///< @brief ����_����̏I�_�ʒu��z����
			FEATURE_LEAF_ENDING_POS_X, ///< @brief �q���̖��[�}�̏I�_�ʒu��x�����̕���
			FEATURE_LEAF_ENDING_POS_X_SD, ///< @brief �q���̖��[�}�̏I�_�ʒu��x�����̕W���΍�
			FEATURE_LEAF_ENDING_POS_X_MAX, ///< @brief �q���̖��[�}�̏I�_�ʒu��x�����̍ő�l
			FEATURE_LEAF_ENDING_POS_X_MIN, ///< @brief �q���̖��[�}�̏I�_�ʒu��x�����̍ŏ��l
			FEATURE_LEAF_ENDING_POS_Y, ///< @brief �q���̖��[�}�̏I�_�ʒu��y�����̕���
			FEATURE_LEAF_ENDING_POS_Y_SD, ///< @brief �q���̖��[�}�̏I�_�ʒu��y�����̕W���΍�
			FEATURE_LEAF_ENDING_POS_Y_MAX, ///< @brief �q���̖��[�}�̏I�_�ʒu��y�����̍ő�l
			FEATURE_LEAF_ENDING_POS_Y_MIN, ///< @brief �q���̖��[�}�̏I�_�ʒu��y�����̍ŏ��l
			FEATURE_LEAF_ENDING_POS_Z, ///< @brief �q���̖��[�}�̏I�_�ʒu��z�����̕���
			FEATURE_LEAF_ENDING_POS_Z_SD, ///< @brief �q���̖��[�}�̏I�_�ʒu��z�����̕W���΍�
			FEATURE_LEAF_ENDING_POS_Z_MAX, ///< @brief �q���̖��[�}�̏I�_�ʒu��z�����̍ő�l
			FEATURE_LEAF_ENDING_POS_Z_MIN, ///< @brief �q���̖��[�}�̏I�_�ʒu��z�����̍ŏ��l
			FEATURE_RUNNING_DIR_X, ///< @brief ���s������x����
			FEATURE_RUNNING_DIR_Y, ///< @brief ���s������y����
			FEATURE_RUNNING_DIR_Z, ///< @brief ���s������z����
			//FEATURE_PARENT_RUNNING_DIR_X, ///< @brief �e�̑��s������x����
			//FEATURE_PARENT_RUNNING_DIR_Y, ///< @brief �e�̑��s������y����
			//FEATURE_PARENT_RUNNING_DIR_Z, ///< @brief �e�̑��s������z����
			//FEATURE_SIBLING_RUNNING_DIR_X, ///< @brief �Z��̑��s������x����
			//FEATURE_SIBLING_RUNNING_DIR_Y, ///< @brief �Z��̑��s������y����
			//FEATURE_SIBLING_RUNNING_DIR_Z, ///< @brief �Z��̑��s������z����
			FEATURE_DIM ///< @brief �����x�N�g���̎���
		};

		/// @brief ���x���l��\���񋓌^
		enum Label 
		{
			LABEL_AO = 1, ///< @brief �����哮��
			LABEL_P, ///< @brief �喬
			LABEL_C, ///< @brief ���o����
			LABEL_CH, ///< @brief ���̓���
			LABEL_PH, ///< @brief �ŗL�̓���
			LABEL_LH, ///< @brief ���̓���
			LABEL_RH, ///< @brief �E�̓���
			LABEL_GD, ///< @brief �ݏ\��w������
			LABEL_LG, ///< @brief ���ݓ��Ö�
			LABEL_RG, ///< @brief �E�ݓ��Ö�
			LABEL_LGE, ///< @brief ���ݑ�ԓ��Ö�
			LABEL_RGE, ///< @brief �E�ݑ�ԓ��Ö�
			LABEL_S, ///< @brief �B���Ö�
			LABEL_GCT, ///< @brief �݌����Ö���
			LABEL_SM, ///< @brief �㒰�Ԗ����Ö�
			LABEL_IM, ///< @brief �����Ԗ����Ö�
			LABEL_LR, ///< @brief ���t����
			LABEL_RR, ///< @brief �E�t����
			LABEL_LCI, ///< @brief ������������
			LABEL_RCI, ///< @brief �E����������
			LABEL_LEI, ///< @brief ���O��������
			LABEL_REI, ///< @brief �E�O��������
			LABEL_LII, ///< @brief ������������
			LABEL_RII, ///< @brief �E����������
			LABEL_OTHER ///< @brief ���̑�
		};

		/// @brief �����x�N�g���������^
		struct FeatureVector : public mist::machine_learning::feature
		{
			std::string parent_category; //�e�̃J�e�S��
		};

		/// @brief �ޓx�������^
		/// @tparam NameType ���O�̌^
		template < class NameType >
		struct Likelihood : public std::map< NameType, double >
		{
			Likelihood &operator+=( const Likelihood &l );
		};

		/// @brief ���ǎ}�̃f�[�^�������^
		struct BranchValue
		{
			typedef std::list< size_t > PathType; ///< @brief �o�H��\���^
			double thk; ///< @brief ����
			Point start_pt; ///< @brief �n�_
			Point end_pt; ///< @brief �I�_
			PathType path; ///< @brief �o�H
		};

		/// @brief �㎯�ʊ�������N���X
		class WeakClassifier : public mist::machine_learning::adaboost::threshold_classifier
		{
		public:
			/// @brief �d�ݒ������s������Ŋw�K���s��
			/// @param[in] features �w�K�T���v���̃��X�g
			/// @param[in] categories �J�e�S���̃��X�g
			/// @retval true �w�K�ɐ�������
			/// @retval false �w�K�Ɏ��s����
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

		typedef mist::machine_learning::adaboost::classifier< WeakClassifier > Classifier; ///< @brief �����ʊ�������N���X
	
		/// @brief ���ǖ؍\�����������ۃN���X
		/// @tparam name ���O�̌^
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
			/// @brief ���O�����x���l�ɕϊ����鏃�����z�֐�
			/// @attention ���̃N���X�����̉����邽�߂ɂ́C�h���N���X���쐬���C���̊֐����������Ȃ���΂Ȃ�Ȃ�
			/// @param[in] name �ϊ����閼�O
			/// @return ���O�ɑΉ����郉�x���l
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
	
		/// @brief �]��`���̖؍\�������t�@�C���ɏ�������
		/// @param[in] file_name ��������XML�t�@�C����
		/// @param[in] centerline �؍\���ɑΉ�����c���摜
		/// @retval true �؍\�����̏������݂ɐ�������
		/// @retval false �؍\�����̏������݂Ɏ��s����
		template < class NameType >
		inline bool Tree< NameType >::write_tree_info( const char *file_name, const ThicknessImage &centerline )const
		{
			XML xml; //��������XML
			std::set< NameType > names; // �������ތ��ǖ�

			// �������ތ��ǖ������߂�
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( iter->name != reject_name( ) )
				{
					names.insert( iter->name );
				}
			}
	
			// ���[�g�v�f����������
			xml.insert( xml.end( ), XML::Element( "Tree" ) );
	
			// �e���ǖ�����������
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				const_iterator end_branch = std::find( postorder_begin( ), postorder_end( ), *name_iter ); // �I�[�}
				std::stack< const_iterator > stack; // �X�^�b�N

				// �}���X�^�b�N�Ƀv�b�V������
				for ( const_iterator iter = end_branch; iter->name == *name_iter; iter = parent( iter ) ) 
				{
					stack.push( iter );
						
					if ( is_root( iter ) )
					{
						break;
					}
				}
			
				// �e�}����������
				XML::iterator name_elem = xml.insert( xml.preorder_begin( ), XML::Element( to_string( *name_iter ) ) ); //���O�v�f

				while ( !stack.empty( ) )
				{
					const_iterator branch = stack.top( ); // �������ގ}
					Point pt = branch->value.start_pt; // �������Ă�����W
					XML::iterator branch_elem = xml.insert( name_elem, XML::Element( "Branch" ) ); // Branch�v�f

					stack.pop( );

					for ( BranchValue::PathType::const_iterator path_iter = branch->value.path.begin( ); ; path_iter++ )
					{
						XML::iterator point_elem = xml.insert( branch_elem, XML::Element( "Point" ) ); // Point�v�f
						XML::iterator x_elem; // X�v�f
						XML::iterator y_elem; // Y�v�f
						XML::iterator z_elem; // Z�v�f
						XML::iterator distance_elem; // Distance�v�f
					
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
		
		/// @brief �]��`���̐c�������t�@�C���ɏ�������
		/// @param[in] file_name ��������XML�t�@�C����
		/// @param[in] centerline �؍\���ɑΉ�����c���摜
		/// @retval true �c�����̏������݂ɐ�������
		/// @retval false �c�����̏������݂Ɏ��s����
		template < class NameType >
		inline bool Tree< NameType >::write_centerline_info( const char *file_name, const ThicknessImage &centerline )const
		{
			XML xml; //��������XML

			// ���[�g�v�f����������
			xml.insert( xml.end( ), XML::Element( "Tree" ) );

			// �e�}����������
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				XML::iterator branch_elem = xml.insert( xml.preorder_begin( ), XML::Element( "Branch" ) ); // Branch�v�f

				// Index�v�f����������
				XML::iterator index_elem = xml.insert( branch_elem, XML::Element( "Index" ) ); // Index�v�f

				index_elem->insert( index_elem->end( ), std::to_string( iter->id - 1 ) );

				// BranchElement�v�f����������
				XML::iterator branch_element_elem = xml.insert( xml.parent( index_elem ), XML::Element( "BranchElement" ) ); // BranchElement�v�f

				Point pt = iter->value.start_pt; // �������Ă�����W

				for ( BranchValue::PathType::const_iterator path_iter = iter->value.path.begin( ); ; path_iter++ )
				{
					XML::iterator point_and_distance_elem = xml.insert( branch_element_elem, XML::Element( "PointAndDistance" ) ); // PointAndDistance�v�f
					XML::iterator x_elem; // X�v�f
					XML::iterator y_elem; // Y�v�f
					XML::iterator z_elem; // Z�v�f
					XML::iterator distance_elem; // Distance�v�f
					
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

				// Length�v�f����������
				XML::iterator length_elem = xml.insert( xml.parent( branch_element_elem ), XML::Element( "Length" ) ); // Length�v�f

				length_elem->insert( length_elem->end( ), std::to_string( static_cast< long double >( length( iter ) ) ) );

				// Thickness�v�f����������
				XML::iterator thickness_elem = xml.insert( xml.parent( length_elem ), XML::Element( "Thickness" ) ); // Thickness�v�f

				thickness_elem->insert( thickness_elem->end( ), std::to_string( static_cast< long double >( iter->value.thk ) ) );

				// ParentIndex�v�f����������
				XML::iterator parent_index_elem = xml.insert( xml.parent( thickness_elem ), XML::Element( "ParentIndex" ) ); // ParentIndex�v�f

				if ( !is_root( iter ) )
				{
					parent_index_elem->insert( parent_index_elem->end( ), std::to_string( parent( iter )->id - 1 ) );
				}

				// ChildIndexList�v�f����������
				XML::iterator child_index_list_elem = xml.insert( xml.parent( parent_index_elem ), XML::Element( "ChildIndexList" ) ); // ChildIndexList�v�f

				for ( const_iterator child_iter = child_begin( iter ); child_iter != child_end( iter ); child_iter++ )
				{
					XML::iterator child_index_elem = xml.insert( child_index_list_elem, XML::Element( "ChildIndex" ) ); // ChildIndex�v�f

					child_index_elem->insert( child_index_elem->end( ), std::to_string( child_iter->id - 1 ) );
					child_index_list_elem = xml.parent( child_index_elem );
				}

				// Vector�v�f����������
				XML::iterator vector_elem = xml.insert( xml.parent( child_index_list_elem ), XML::Element( "Vector" ) ); // Vector�v�f
				XML::iterator x_elem; // X�v�f
				XML::iterator y_elem; // Y�v�f
				XML::iterator z_elem; // Z�v�f

				x_elem = xml.insert( vector_elem, XML::Element( "X" ) );
				x_elem->insert( x_elem->end( ), std::to_string( static_cast< ptrdiff_t >( iter->value.end_pt.x - iter->value.start_pt.x ) ) );
				y_elem = xml.insert( xml.parent( x_elem ), XML::Element( "Y" ) );
				y_elem->insert( y_elem->end( ), std::to_string( static_cast< ptrdiff_t >( iter->value.end_pt.y - iter->value.start_pt.y ) ) );
				z_elem = xml.insert( xml.parent( y_elem ), XML::Element( "Z" ) );
				z_elem->insert( z_elem->end( ), std::to_string( static_cast< ptrdiff_t >( iter->value.end_pt.z - iter->value.start_pt.z ) ) );
			}

			return xml.write( file_name );
		}
		
		/// @brief �]��`���̃I�[�o���C�����t�@�C���ɏ�������
		/// @param[in] file_name ��������XML�t�@�C����
		/// @retval true �I�[�o���C���̏������݂ɐ�������
		/// @retval false �I�[�o���C���̏������݂Ɏ��s����
		template < class NameType >
		inline bool Tree< NameType >::write_overlay_info( const char *file_name )const
		{
			XML xml; // ��������XML
			std::set< NameType > names; // �������ތ��ǖ�

			// �������ތ��ǖ������߂�
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( iter->name != reject_name( ) )
				{
					names.insert( iter->name );
				}
			}

			// ���[�g�v�f����������
			xml.insert( xml.end( ), XML::Element( "OverlayInfo" ) );

			// �e���ǖ�����������
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				Point sec_pt = secant_point( *name_iter ); // �Ȑ����_

				// ��������
				XML::iterator name_elem = xml.insert( xml.preorder_begin( ), XML::Element( to_string( *name_iter ) ) ); // ���O�v�f
				XML::iterator label_elem; // Label�v�f
				XML::iterator position_elem; // Position�v�f
				XML::iterator x_elem; // X�v�f
				XML::iterator y_elem; // Y�v�f
				XML::iterator z_elem; // Z�v�f

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
			
		/// @brief NewVES�`���Ŋe���ǂ̋Ȑ����_���t�@�C���ɏ�������
		/// @attention ����m�F�����Ă��Ȃ����߁C�g�p���͒���
		/// @param[in] file_name ��������CSV�t�@�C����
		/// @param[in] pos �������ޓ_�̈ʒu�D[0, 1]�͈̔͂Ŏw�肵�C0�Ȃ�n�_�C1�Ȃ�I�_�D�K��l��0.5�D
		/// @retval true �e���ǂ̋Ȑ����_�̏������݂ɐ�������
		/// @retval false �e���ǂ̋Ȑ����_�̏������݂Ɏ��s����
		template < class NameType >
		bool Tree< NameType >::write_secant_points( const char *file_name, double pos )const
		{
			std::ofstream ofs( file_name ); // �o�̓t�@�C��
			std::set< NameType > names; // �������ތ��ǖ�

			// �G���[�`�F�b�N
			if ( !ofs )
			{
				return false;
			}

			//�������ތ��ǖ������߂�
			for ( const_iterator iter = begin( ); iter != end( ); iter++ )
			{
				if ( iter->name != reject_name( ) )
				{
					names.insert( iter->name );
				}
			}

			// �e���ǖ�����������
			for ( std::set< NameType >::const_iterator name_iter = names.begin( ); name_iter != names.end( ); name_iter++ )
			{
				Point sec_pt = secant_point( *name_iter, pos ); // �Ȑ����_

				ofs << to_string( *name_iter ) << ", " << sec_pt.x << ", " << sec_pt.y << ", " << sec_pt.z << std::endl;
			}

			return true;
		}
		
		/// @brief �}�̒������擾����
		/// @param[in] branch �������擾����}
		/// @return �}�̒���
		template < class NameType >
		template < class Iterator >
		inline double Tree< NameType >::length( Iterator branch )const
		{
			double len = 0; // ����
			Shift shift( 0, 0, 0 ); // �ψ�

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
		
		/// @brief �o�H�̒������擾����
		/// @attention end_branch��start_branch�̎q�łȂ���΂Ȃ�Ȃ�
		/// @param[in] start_branch �������擾����o�H�̊J�n�}�̐e
		/// @param[in] end_branch �������擾����o�H�̏I�[�}
		/// @return �o�H�̒���
		template < class NameType >
		template < class StartIterator, class EndIterator >
		inline double Tree< NameType >::length( StartIterator start_branch, EndIterator end_branch )const
		{
			double len = 0; // ����

			for ( ; end_branch != end( ) && end_branch != start_branch; end_branch = parent( end_branch ) )
			{
				len += length( end_branch );
			}

			return len;
		}
			
		/// @brief �w��̌��ǂ̋Ȑ����_���v�Z����
		/// @attention ���ǂ������ӏ��ɂ���ꍇ�C�I�l���ōŏ��ɔ������ꂽ���ǂɂ��Čv�Z����
		/// @param[in] name �Ȑ����_���v�Z���錌�ǂ̖��O
		/// @param[in] pos �������ޓ_�̈ʒu�D[0, 1]�͈̔͂Ŏw�肵�C0�Ȃ�n�_�C1�Ȃ�I�_�D�K��l��0.5�D
		/// @return �w��̌��ǂ̋Ȑ����_
		template < class NameType >
		Point Tree< NameType >::secant_point( NameType name, double pos )const
		{
			const_iterator end_branch = std::find( postorder_begin( ), postorder_end( ), name ); // �I�[�}
			size_t voxel_cnt = 0; // ��f��

			if ( end_branch != postorder_end( ) )
			{
				Point sec_pt = end_branch->value.end_pt; // �Ȑ����_

				// ��f�������߂�
				for ( const_iterator iter = end_branch; iter->name == name; iter = parent( iter ) ) 
				{
					voxel_cnt += iter->value.path.size( ) + 1;

					if ( is_root( iter ) )
					{
						break;
					}
				}

				// �Ȑ����_���v�Z����
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
	
		/// @brief �c���𑖍���1�{�̎}�̃f�[�^���擾����
		/// @param[out] val �擾����f�[�^
		/// @param[in] centerline ��������c���摜�D���̉摜�͏��������Ă��悢
		/// @param[in] start_pt �����̎n�_�̍��W
		/// @return �}�̏I�_�̍��W
		template < class NameType >
		inline Point Tree< NameType >::traverse_branch( BranchValue &val, ThicknessImage &centerline, const Point &start_pt )const
		{
			// �n�_��ݒ肷��
			val.start_pt = val.end_pt = start_pt;
		
			// �e��f�𑖍�����
			while ( true )
			{
				Point next_pt; // ���̍��W
				size_t connect_num = 0; // �A�����Ă����f��

				// �����ʂ��X�V
				val.thk += centerline( val.end_pt );

				// �����ς݉�f���폜����
				centerline( val.end_pt ) = 0;

				// ���W���X�V����
				for ( size_t i = 0; i < TOPOLOGY; i++ )
				{
					try
					{
						Point connect_pt = val.end_pt + TOPOLOGICAL( neighborhood )[ i ]; // �אڂ��Ă�����W

						if ( centerline( connect_pt ) > 0 )
						{
							// ���̉�f�̂Ƃ�
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
					// ����_�̂Ƃ��אږ�������f���o�H����폜����
					for ( size_t i = 0; i < connect_num; i++ )
					{
						val.path.pop_back( );
					}

					break;
				}
			} 

			// �������m�肷��
			val.thk /= val.path.size( ) + 1;

			return val.end_pt;
		}
		
		/// @brief �����ʂ𒊏o����
		/// @param[out] feature ���o���������ʂ��i�[����̈�
		/// @param[in] branch �����ʂ𒊏o����}
		/// @param[in] ref_branch ��}
		template < class NameType >
		inline void Tree< NameType >::extract_feature( FeatureVector &feature, const_iterator branch, const_iterator ref_branch )const
		{
			Vector start_pos, end_pos, start_pos_bifurcation, end_pos_bifurcation, leaf_pos( 0, 0, 0 ), leaf_pos_var( 0, 0, 0 ), leaf_pos_max( -DBL_MAX, -DBL_MAX, -DBL_MAX ), leaf_pos_min( DBL_MAX, DBL_MAX, DBL_MAX ); // �ʒu�x�N�g��
			Vector run_dir, root_run_dir, parent_run_dir, sibling_run_dir; //���s�����x�N�g��
			double curve = 0; // �p�ȓx
			double sibling_dist = 0; // ����
			double sibling_thk = 0, sibling_thk_var = 0, sibling_thk_max = 0, sibling_thk_min = DBL_MAX, child_thk = 0, child_thk_var = 0, child_thk_max = 0, child_thk_min = DBL_MAX; // ����
			//double parent_angle, sibling_angle = 0, sibling_angle_max = -M_PI / 2, sibling_angle_min = M_PI / 2, child_angle = 0, child_angle_max = -M_PI / 2, child_angle_min = M_PI / 2; // �p�x
			size_t sibling_bifurcation = 0; // ����
			Point pt = branch->value.start_pt; // �}��̍��W

			// �����ʂ�����������
			feature.resize( FEATURE_DIM );
			feature.category = to_string( branch->name );
			feature.parent_category = is_root( branch ) ? to_string( reject_name( ) ) : to_string( parent( branch )->name );

			// �x�N�g�����v�Z����
			start_pos = effective( Vector( branch->value.start_pt ) - Vector( ref_branch->value.start_pt ) );
			end_pos = effective( Vector( branch->value.end_pt ) - Vector( ref_branch->value.start_pt ) );
			start_pos_bifurcation = effective( Vector( branch->value.start_pt ) - Vector( ref_branch->value.end_pt ) );
			end_pos_bifurcation = effective( Vector( branch->value.end_pt ) - Vector( ref_branch->value.end_pt ) );
			run_dir = effective( Vector( branch->value.end_pt ) - Vector( branch->value.start_pt ) ).unit( );
			root_run_dir;
			parent_run_dir = is_root( branch ) ? Vector( 0, 0, 0 ) : effective( Vector( parent( branch )->value.end_pt ) - Vector( parent( branch )->value.start_pt ) ).unit( );

			if ( !is_root( branch ) )
			{
				size_t sibling_cnt = 0; // �Z��̐�

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
				size_t leaf_cnt = 0; // ���[�}�̐�

				for ( const_iterator iter = subtree( branch ).begin( ); iter != subtree( branch ).end( ); iter++ )
				{
					if ( is_leaf( iter ) )
					{
						Vector temp_pos = effective( Vector( iter->value.end_pt ) - Vector( ref_branch->value.start_pt ) ); // ���[�}�̈ʒu�x�N�g��

						// ���[�}�̈ʒu�̕��ς��v�Z����
						leaf_pos += temp_pos;

						// ���[�}�̈ʒu�̍ő储��эŏ����v�Z����
						leaf_pos_max.x = std::max( leaf_pos_max.x, temp_pos.x );
						leaf_pos_max.y = std::max( leaf_pos_max.y, temp_pos.y );
						leaf_pos_max.z = std::max( leaf_pos_max.z, temp_pos.z );
						leaf_pos_min.x = std::min( leaf_pos_min.x, temp_pos.x );
						leaf_pos_min.y = std::min( leaf_pos_min.y, temp_pos.y );
						leaf_pos_min.z = std::min( leaf_pos_min.z, temp_pos.z );

						// ���[�}�̈ʒu�̕��U���v�Z����
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

			// �p�ȓx���v�Z����
			for ( BranchValue::PathType::const_iterator iter = branch->value.path.begin( ); iter != branch->value.path.end( ); iter++ ) 
			{
				// ���W���X�V����
				pt += TOPOLOGICAL( neighborhood )[ *iter ];

				Vector pos = effective( Vector( pt ) - Vector( branch->value.start_pt ) ); // ��f�̈ʒu�x�N�g��

				// �ʒu�x�N�g���Ƃ��̑��s�����x�N�g���ւ̐��ˉe�Ƃ̍�����@���x�N�g���̒������v�Z����
				curve += ( pos - ( pos.inner( run_dir ) * run_dir ) ).length( );
			}

			curve /= branch->value.path.size( ) + 1;

			// �������v�Z����
			if ( is_root( branch ) )
			{
				sibling_thk_max = sibling_thk_min = 0;
			}
			else
			{
				size_t sibling_cnt = 0; // �Z��̐�

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
				size_t child_cnt = 0; //�q���̐�
					
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

			// �p�x���Z�o����
			//parent_angle = is_root( branch ) ? std::acos( run_dir.inner( effective( Vector( parent( branch )->value.end_pt ) - Vector( parent( branch )->value.start_pt ) ).unit( ) ) ) : 0;
			//
			//if ( !is_root( branch ) )
			//{
			//	for ( const_iterator iter = child_begin( parent( branch ) ); iter != child_end( parent( branch ) ); iter++ )
			//	{
			//		double angle = std::acos( run_dir.inner( effective( Vector( iter->value.end_pt ) - Vector( iter->value.start_pt ) ).unit( ) ) ); // �p�x
			//		size_t sibling_cnt = 0; // �Z��̐�

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
			//	size_t child_cnt = 0; // �q���̐�
	
			//	for ( const_iterator iter = child_begin( branch ); iter != child_end( branch ); iter++ )
			//	{
			//		double angle = std::acos( run_dir.inner( effective( Vector( iter->value.end_pt ) - Vector( iter->value.start_pt ) ).unit( ) ) ); // �p�x

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

			// �������v�Z����
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

			// ���򐔂��v�Z����
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

			// �����ʂ��i�[����
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
			feature[ FEATURE_RUNNING_DIR_X ] = run_dir.x; // �ȑO�ƈقȂ�l
			feature[ FEATURE_RUNNING_DIR_Y ] = run_dir.y; // �ȑO�ƈقȂ�l
			feature[ FEATURE_RUNNING_DIR_Z ] = run_dir.z; // �ȑO�ƈقȂ�l
			//feature[ FEATURE_PARENT_RUNNING_DIR_X ] = parent_run_dir.x;
			//feature[ FEATURE_PARENT_RUNNING_DIR_Y ] = parent_run_dir.y;
			//feature[ FEATURE_PARENT_RUNNING_DIR_Z ] = parent_run_dir.z;
			//feature[ FEATURE_SIBLING_RUNNING_DIR_X ] = sibling_run_dir.x;
			//feature[ FEATURE_SIBLING_RUNNING_DIR_Y ] = sibling_run_dir.y;
			//feature[ FEATURE_SIBLING_RUNNING_DIR_Z ] = sibling_run_dir.z;
		}

		/// @brief �w��̎}�Ɋւ���Ή��t�����ʂ̕]���l���擾����
		/// @param[in] result_branch �Ή��t�����ʂ̎}
		/// @param[in] truth_branch �����̎}
		/// @return �]���l
		template < class NameType >
		inline double Tree< NameType >::evaluate_result( const_iterator result_branch, const_iterator truth_branch )const
		{
#ifndef _EVALUATE_HEAD_ONLY
			return length( result_branch );
#else //_EVALUATE_HEAD_ONLY
			return 1;
#endif //_EVALUATE_HEAD_ONLY
		}
		
		/// @brief ���O�t���c���摜�𕜌�����
		/// @param[out] labeled_centerline ���O�t���c���摜���i�[����̈�
		/// @param[in] branch ��������}
		template < class NameType >
		inline void Tree< NameType >::restore_centerline( LabelImage &labeled_centerline, const_iterator branch )const
		{
			Point pt = branch->value.start_pt; // �������Ă�����W

			// ��������
			labeled_centerline( pt ) = to_label( branch->name );
				
			for ( BranchValue::PathType::const_iterator iter = branch->value.path.begin( ); iter != branch->value.path.end( ); iter++ )
			{
				// ���W���X�V����
				pt += TOPOLOGICAL( neighborhood )[ *iter ];

				// �摜���}�[�N����
				labeled_centerline( pt ) = to_label( branch->name );
			}
		}	

		/// @brief ���O�t�����ǉ摜�̕���_�𕜌�����
		/// @param[in] vessel ���ǂ̃}�[�N�摜
		/// @param[out] labeled_vessel ���O�t�����ǉ摜���i�[����̈�
		/// @param[in] branch �������镪��_�̐e�}
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

		/// @brief XML����}��ǂݍ���
		/// @param[in] xml �ǂݍ���XML
		/// @param[in] elem �ǂݍ��ޗv�f���w���C�e���[�^
		/// @param[out] branch �ǂݍ��ގ}���w���C�e���[�^
		/// @retval true �}�̓ǂݍ��݂ɐ�������
		/// @retval false �}�̓ǂݍ��݂Ɏ��s����
		template < class NameType >
		inline bool Tree< NameType >::read_branch( const XML &xml, XML::const_iterator elem, iterator branch )
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
				// ������ǂݍ���
				branch->value.thk = std::stod( elem->tag( )[ "radius" ] );

				// �c���o�H��ǂݍ���
				XML::const_iterator medialline_elem = std::find( xml.child_begin( elem ), xml.child_end( elem ), "MEDIALLINE" ); // MEDIALLINE�v�f
				XML::const_iterator position_elem = std::find_if( xml.child_begin( medialline_elem ), xml.child_end( medialline_elem ), FindElem( "POSITION", "1" ) ); // POSITION�v�f
				XML::const_iterator likelihood_elem = std::find( xml.child_begin( elem ), xml.child_end( elem ), "LIKELIHOOD" ); // LIKELIHOOD�v�f
				Point prev_pt; // �O�̍��W
				size_t id = 1; // ���ʎq

				// �G���[�`�F�b�N
				if ( medialline_elem == xml.child_end( elem ) || position_elem == xml.child_end( medialline_elem ) )
				{
					return false;
				}

				// �n�_��ǂݍ���
				branch->value.start_pt.x = static_cast< size_t >( std::stod( position_elem->tag( )[ "x" ] ) / get_pitch( ).x + 0.5 );
				branch->value.start_pt.y = static_cast< size_t >( std::stod( position_elem->tag( )[ "y" ] ) / get_pitch( ).y + 0.5 );
				branch->value.start_pt.z = static_cast< size_t >( std::stod( position_elem->tag( )[ "z" ] ) / get_pitch( ).z + 0.5 );
				prev_pt = branch->value.start_pt;

				// �o�H��ǂݍ���
				for ( ; position_elem != xml.child_end( medialline_elem ); position_elem = std::find_if( xml.child_begin( medialline_elem ), xml.child_end( medialline_elem ), FindElem( "POSITION", std::to_string( static_cast< unsigned long long >( ++id ) ) ) ) )
				{
					Shift shift; // �ψ�
			
					// ���W��ǂݍ���
					branch->value.end_pt.x = static_cast< size_t >( std::stod( position_elem->tag( )[ "x" ] ) / get_pitch( ).x + 0.5 );
					branch->value.end_pt.y = static_cast< size_t >( std::stod( position_elem->tag( )[ "y" ] ) / get_pitch( ).y + 0.5 );
					branch->value.end_pt.z = static_cast< size_t >( std::stod( position_elem->tag( )[ "z" ] ) / get_pitch( ).z + 0.5 );

					// �C���f�b�N�X���v�Z����
					shift = branch->value.end_pt - prev_pt;

					for ( size_t i = 0; i < TOPOLOGY; i++ )
					{
						if ( shift == TOPOLOGICAL( neighborhood )[ i ] )
						{
							branch->value.path.insert( branch->value.path.end( ), i );
					
							break;
						}
					}

					// ���W���X�V����
					prev_pt = branch->value.end_pt;
				}

				// �ޓx��ǂݍ���
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

		/// @brief XML�Ɏ}����������
		/// @param[in] xml ��������XML
		/// @param[out] elem �������ޗv�f���w���C�e���[�^
		/// @param[in] branch �������ގ}���w���C�e���[�^
		/// @retval true �}�̏������݂ɐ�������
		/// @retval false �}�̏������݂Ɏ��s����
		template < class NameType >
		inline bool Tree< NameType >::write_branch( XML &xml, XML::iterator elem, const_iterator branch )const
		{
			// ��������������
			elem->tag( ).insert( elem->tag( ).end( ), XML::Element::Tag::value_type( "radius", std::to_string( static_cast< long double >( branch->value.thk ) ) ) );

			// �c���o�H����������
			XML::iterator medialline_elem = xml.insert( elem, XML::Element( "MEDIALLINE" ) ); // MEDIALLINE�v�f
			Point pt = branch->value.start_pt; // �������Ă�����W
			size_t id = 1; // ���ʎq

			for ( BranchValue::PathType::const_iterator iter = branch->value.path.begin( ); ; iter++ )
			{
				XML::iterator position_elem = xml.insert( medialline_elem, XML::Element( "POSITION" ) ); // POSITION�v�f

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
			// �ޓx����������
			if ( !branch->likelihood.empty( ) ) 
			{
				XML::iterator likelihood_elem = xml.insert( xml.parent( medialline_elem ), XML::Element( "LIKELIHOOD" ) ); // LIKELIHOOD�v�f

				for ( Likelihood< NameType >::const_iterator likelihood_iter = branch->likelihood.begin( ); likelihood_iter != branch->likelihood.end( ); likelihood_iter++ ) 
				{
					likelihood_elem->tag( ).insert( likelihood_elem->tag( ).end( ), XML::Element::Tag::value_type( to_string( likelihood_iter->first ), std::to_string( static_cast< long double >( likelihood_iter->second ) ) ) );
				}
			}
#endif //_WRITE_LIKELIHOOD

			return true;
		}
		
		/// @brief ���[�܂ł̍Œ��o�H�̒������擾����
		/// @param[in] branch ���[�܂ł̍Œ��o�H�̒������擾����}
		/// @return ���[�܂ł̍Œ��o�H�̒���
		template < class NameType >
		template < class Iterator >
		inline double Tree< NameType >::max_length( Iterator branch )const
		{
			double max_len = 0; //����

			for ( const_iterator iter = child_begin( branch ); iter != child_end( branch ); iter++ )
			{
				max_len = std::max( max_len, max_length( iter ) );
			}

			return max_len + length( branch );
		}
			
		/// @brief �q���̕��򐔂��擾����
		/// @param[in] branch �q���̕��򐔂��擾����}
		/// @return �q���̕���
		template < class NameType >
		template < class Iterator >
		size_t Tree< NameType >::bifurcation_num( Iterator branch )const
		{
			size_t cnt = 0; // ����

			for ( const_iterator iter = subtree( branch ).begin( ); iter != subtree( branch ).end( ); iter++ )
			{
				if ( !is_leaf( iter ) )
				{
					cnt++;
				}
			}

			return cnt;
		}

		/// @brief �x�N�g�����𑜓x���l�����������x�N�g���ɕϊ�����
		/// @param[in] vec �����x�N�g���ɕϊ�����x�N�g��
		/// @return �����x�N�g��
		template < class NameType >
		template < class VectorType >
		inline Vector Tree< NameType >::effective( const VectorType &vec )const
		{
			return Vector( vec.x * get_pitch( ).x, vec.y * get_pitch( ).y, vec.z * get_pitch( ).z );
		}
	
		/// @brief �ޓx�����Z����
		/// @param[in] l ���Z�����ޓx
		/// @return �������g�̎Q��
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