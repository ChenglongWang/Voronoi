/**
* @file config.h
* @brief �ݒ�̃w�b�_�t�@�C��
* @version 3.0
* @author ���É���w ���Ȋw������ ���f�B�A�Ȋw��U �X������   351203192   ���� �N�N
* @date 2013/10/18
*/

#ifndef _TMATSU_CONFIG_H_
#define _TMATSU_CONFIG_H_

#define _TARGET_HYPOGASTRIC ///< @brief ������������Ώۂɂ���

#define _CALC_LIKELIHOOD ///< @brief �ޓx�v�Z���s��
#define _CALC_INTEGRITY ///< @brief �����������߂�
//#define _UPDATE_LIKELIHOOD ///< @brief �ޓx�X�V���s��
//#define _FIND_MAX_LIKELIHOOD ///< @brief �ő�̖ޓx�����q����T�����邱�Ƃɂ��Ή��t�����s��
//#define _SCREEN_OUT_CANDIDATE ///< @brief ���̂ӂ邢���Ƃ��ɂ��Ή��t�����s��
//#define _FIND_BIFURCATION ///< @brief �����T�����邱�Ƃɂ��Ή��t�����s��
//#define _SELECT_MODEL ///< @brief ���f���I���ɂ��Ή��t�����s��
#define _FIND_OPTIMUM_BRANCHING ///< @brief �œK��������߂邱�Ƃɂ��Ή��t�����s��
//#define _IGNORE_TAIL_SCORE ///< @brief ����O���t�̏d�݂��v�Z����Ƃ��I�[�X�R�A�𖳎�����
//#define _CORRECT_ERROR ///< @brief ���������s��

//#define _WRITE_LIKELIHOOD ///< @brief �ޓx���o�͂���

//#define _EVALUATE_HEAD_ONLY ///< @brief �e���ǂ̍ŏ��̂ݕ]������
//#define _EVALUATE_LIKELIHOOD_RANKING ///< @brief �ޓx�̏�ʂɐ��������銄����]������
//#define _EVALUATE_PATTERN ///< @brief ����p�^�[�������ʂł������]������
//#define _EVALUATE_EVERY_PATTERN ///< @brief �p�^�[�����Ƃɕ]������
//#define _EVALUATE_EVERY_CASE ///< @brief �ǗႲ�Ƃɕ]������

#define _REPORT_IN_ENGLISH ///< @brief �f�f���|�[�g���p��ō쐬����

//#define _DEBUG_GRAPH ///< @brief �O���t�����Ɋւ���f�o�b�O�����o�͂���

#define TOPOLOGY 6 ///< @brief �g�|���W
#define LEARN_ITERATION 1000 ///< @brief �w�K�̌J��Ԃ���
#define PRUN_THRESHOLD 2 ///< @brief �}���肷��}�̉�f��
#define BOTTOM_THRESHOLD( dpt ) ( ( dpt ) * 0.9 ) ///< @brief ���������̏I�[�}�̌��Ƃ���}�̏I�_�̍Œ��z���W
#define THICKNESS_THRESHOLD 2.0 ///< @brief �����������ݔ���̂������l
#define SMV_THRESHOLD( avg ) ( avg ) ///< @brief �㒰�Ԗ��Ö��̌��Ƃ��镔���؂̍Œ�̎}��
#define LPV_THRESHOLD( avg ) ( avg ) ///< @brief �喬���}�̌��Ƃ��镔���؂̍Œ�̎}��
#define CROSS_THRESHOLD 2.0 ///< @brief ��������̂������l

#endif //_TMATSU_CONFIG_H_

/**
* @mainpage ���ǖ��Ή��t���v���W�F�N�g
* @version 3.0
* @author ���É���w ���Ȋw������ ���f�B�A�Ȋw��U �X������   351203192   ���� �N�N
* @date 2013/10/18
* 
* @section �g�p����ɂ�
*
* @subsection �v���p�e�B�̐ݒ�
* 
* MIST �̃C���N���[�h�f�B���N�g���̃p�X���u�ǉ��̃C���N���[�h�f�B���N�g���v�ɒǉ����܂� @n
* zconf.h, zlib.h ������f�B���N�g���̃p�X���u�ǉ��̃C���N���[�h�f�B���N�g���v�ɒǉ����܂� @n
* zlib.lib ������f�B���N�g���̃p�X���u�ǉ��̃��C�u�����f�B���N�g���v�ɒǉ����܂� @n
* zlib.lib ���u�ǉ��̈ˑ��t�@�C���v�ɒǉ����܂� @n
*
* @subsection �w�b�_�̃C���N���[�h
* @code
* #include "artery.h" // �����̌��ǖ��Ή��t�����s��
* #include "portal.h" // �Ö��̌��ǖ��Ή��t�����s��
* @endcode
* 
* @subsection �����̑Ή��t���̗�
* @code
* using namespace tmatsu;
* using namespace tmatsu::artery;
* 
* MarkImage vessel; // ���ǂ̃}�[�N�摜
* ThicknessImage centerline; // �c���摜
* LabelImage labeled_vessel; // ���ǂ̃��x���摜
* Tree tree; // �؍\��
* Tree result; // �Ή��t�����ʖ؍\��
* Tree truth; // ����؍\��
* LikelihoodFunction likelihood_func; // �ޓx�֐�
* Matrix confusion; // �R���t���[�W�����}�g���b�N�X
* 
* // ���ǂ̃}�[�N�摜��ǂݍ���
* if ( !vessel.read( MarkImage::ProgressCallback( ), "\\\\TAKA2\\tmatsu\\ACC_g001\\vessel.uc_raw.gz" ) )
* {
*		std::exit( 1 );
* }
* 
* // ����؍\����ǂݍ���
* if ( !truth.read( "\\\\TAKA2\\tmatsu\\ACC_g001\\truth.xml", vessel.get_size( ). vessel.get_pitch( ) ) )
* {
*		std::exit( 1 );
* }
* 
* // �ޓx�֐���ǂݍ���
* if ( !likelihood_func.read( "\\\\TAKA2\\tmatsu\\ACC_g001" ) )
* {
*		std::exit( 1 );
* }
* 
* // �c���𒊏o����
* vessel.thinning( centerline );
* 
* // �؍\�����\�z����
* tree.construct( centerline );
* 
* // �Ή��t�����s��
* tree.labeling( result, likelihood_func );
* 
* // ���ʂ�]������
* result.evaluate( confusion, truth );
* 
* // ���x���摜�𕜌�����
* result.restore( vessel, labeled_vessel );
* 
* // �Ή��t�����ʂ���������
* if ( !result.write( "\\\\TAKA2\\tmatsu\\ACC_g001\\result.xml" ) )
* {
*		std::exit( 1 );
* }
* 
* // �]��`���ŏ�������
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
* // NewVES�`���ŏ�������
* if ( !tree.write_secant_points( "\\\\TAKA2\\tmatsu\\ACC_g001\\sec_pt.csv" ) )
* {
*		std::exit( 1 );
* }
* 
* // �������ʂ���������
* if ( !labeled_vessel.write( LabelImage::ProgressCallback( ), "\\\\TAKA2\\tmatsu\\ACC_g001\\labeled_vessel.uc_raw.gz" ) )
* {
*		std::exit( 1 );
* }
* @endcode
* 
* @subsection �Ö��̊w�K�̗�
* @code
* using namespace tmatsu;
* using namespace tmatsu::portal;
* 
* std::list< FeatureVector > samples; // �w�K�T���v�����X�g
* LikelihoodFunction likelihood_func; // �ޓx�֐�
* 
* // �w�K�T���v����ǂݍ���
* for ( size_t i = 2; ; i++ )
* {
*		std::stringstream ss; // �X�g�����O�X�g���[��
*		std::string hdr_file_name; // �w�b�_�t�@�C����
*		std::string tree_file_name; // �؍\���t�@�C����
*		Header hdr; // �w�b�_
*		Tree tree; // �؍\��
*		std::list< FeatureVector > features; // ������
* 
*		ss << "\\\\TAKA2\\tmatsu\\ACC_g" << std::setw( 3 ) << std::setfill( '0' ) << i;
*		hdr_file_name = ss.str( ) + "\\vessel.uc_raw.gz.header";
*		tree_file_name = ss.str( ) + "\\tree.xml";
* 
*		// �w�b�_�t�@�C����ǂݍ���
*		if ( !hdr.read( hdr_file_name.c_str( ) ) )
*		{
*			break;
*		}
*
*		// �؍\����ǂݍ���
*		if ( !tree.read( tree_file_name.c_str( ), hdr.get_size( ), hdr.get_pitch( ) ) )
*		{
*			break;
*		}
* 
*		// �����ʂ𒊏o����
*		tree.extract( features );
*		samples.splice( samples.end( ), features );
* }
* 
* // �w�K����
* likelihood_func.learn( samples );
* 
* // �ޓx�֐�����������
* if ( !likelihood_func.write( "\\\\TAKA2\\tmatsu\\ACC_g001" ) )
* {
*		std::exit( 1 );
* }
* @endcode
*
* @section �����ύX����ɂ�
* tmatsu::tubular::Tree ���؍\���������N���X�̊��ƂȂ��Ă��܂� @n
* �Ǝ��̏������s�����߂ɂ͈ȉ��̃����o�֐������������h���N���X���쐬���܂� @n
* �e�֐��̏ڍׂ̓��t�@�����X���Q�Ƃ��Ă�������
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
* �܂��ȉ��̃����o�֐����������邱�Ƃœ�����g�����邱�Ƃ��ł��܂� @n
* �K�v�Ȃ���Ζ���`�ł��\���܂���
* @code
* tmatsu::tubular::Tree::construct_postprocess;
* tmatsu::tubular::Tree::labeling_preprocess;
* tmatsu::tubular::Tree::labeling_postprocess;
* @endcode
* tmatsu::vessel::Tree, tmatsu::artery::Tree, tmatsu::portal::Tree �͂��̃N���X���p�����Ă��܂� @n
* �܂����̃N���X�� tmatsu::tree �����Ƃ�STL�R���e�i�̂悤�Ɉ������Ƃ��ł��܂�
*/