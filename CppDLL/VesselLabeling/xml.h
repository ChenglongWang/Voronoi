/**
* @file xml.h
* @brief XML�N���X�̃w�b�_�t�@�C��
* @version 3.0
* @author ���É���w ���Ȋw������ ���f�B�A�Ȋw��U �X������   351203192   ���� �N�N
* @date 2013/10/18
*/

#ifndef _TMATSU_XML_H_
#define _TMATSU_XML_H_

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cctype>
#include "dictionary.h"
#include "tree.h"

#define DEFAULT_VERSION "1.0" ///< @brief version�����̊���l
#define DEFAULT_ENCODING "shift-jis" ///< @brief encoding�����̊���l
#define DEFAULT_STANDALONE "yes" ///< @brief standalone�����̊���l

namespace tmatsu
{
	/// @brief �v�f���e�L�X�g�̓��I�z��Ƃ��Ĉ����N���X
	class _Element : public std::vector< std::string >
	{
	public:
		/// @brief �^�O�𑮐��̘A�z�z��Ƃ��Ĉ����N���X
		class Tag : public tmatsu::dictionary< std::string, std::string >
		{
		public:
			typedef invalid_key InvalidProperty; ///< @brief ���������Q�Ɨ�O
		private:
			std::string name_; ///< @brief �^�O��
		public:
			Tag( );
			Tag( const Tag &tag );
			Tag( const std::string &name );
			std::string &name( );
			const std::string &name( )const;
		};
	private:
		Tag tag_; ///< @brief �^�O
	public:
		_Element( );
		_Element( const _Element &elem );
		_Element( const Tag &tag );
		_Element( const std::string &tag_name );
		Tag &tag( );
		const Tag &tag( )const;
		bool operator==( const std::string &tag_name )const;
	};

	/// @brief XML��؍\���Ƃ��Ĉ����N���X
	class XML : public tree< _Element >
	{
	public:
		typedef _Element Element; ///< @brief �v�f�������N���X
	private:
		std::string version_; ///< @brief version����
		std::string encoding_; ///< @brief encoding����
		std::string standalone_; ///< @brief standalone����
	public:
		XML( );
		XML( const XML &xml );
		XML( const char *file_name );
		XML( const std::string &version, const std::string &encoding, const std::string &standalone );
		~XML( );
		std::string &version( );
		const std::string &version( )const;
		std::string &encoding( );
		const std::string &encoding( )const;
		std::string &standalone( );
		const std::string &standalone( )const;
		bool read( const char *file_name );
		bool write( const char *file_name )const;
	private:
		iterator read( std::stringstream &ss, iterator elem );
		void write( std::ofstream &ofs, const_iterator elem )const;
		static void get_str( std::stringstream &ss, std::string &str, char delim, bool term_with_space = false );
	};
	
	/// @brief �e����������l�ŏ���������
	inline XML::XML( ) : version_( DEFAULT_VERSION ), encoding_( DEFAULT_ENCODING ), standalone_( DEFAULT_STANDALONE )
	{
	}
	
	/// @brief XML�𕡐�����
	/// @param[in] xml ��������XML
	inline XML::XML( const XML &xml ) : version_( xml.version_ ), encoding_( xml.encoding_ ), standalone_( xml.standalone_ ), tree< _Element >( xml )
	{
	}
	
	/// @brief XML���t�@�C������ǂݍ���
	/// @attention XML�t�@�C���̓��e�ɂ���Ă͐���ɓǂݍ��߂Ȃ��\��������
	/// @param[in] file_name �ǂݍ���XML�t�@�C����
	inline XML::XML( const char *file_name ) : version_( DEFAULT_VERSION ), encoding_( DEFAULT_ENCODING ), standalone_( DEFAULT_STANDALONE )
	{
		read( file_name );
	}
	
	/// @brief �e������ݒ肷��
	/// @param[in] version version����
	/// @param[in] encoding encoding����
	/// @param[in] standalone standalone����
	inline XML::XML( const std::string &version, const std::string &encoding, const std::string &standalone ) : version_( version ), encoding_( encoding ), standalone_( standalone )
	{
	}

	/// @brief ����Ȃ�
	inline XML::~XML( )
	{
	}
	
	/// @brief version�������擾����
	/// @return version�����̎Q��
	inline std::string &XML::version( )
	{
		return version_;
	}
	
	/// @brief version�������擾����
	/// @return version�����̒萔�Q��
	inline const std::string &XML::version( )const
	{
		return version_;
	}
	
	/// @brief encoding�������擾����
	/// @return encoding�����̎Q��
	inline std::string &XML::encoding( )
	{
		return encoding_;
	}
	
	/// @brief encoding�������擾����
	/// @return encoding�����̒萔�Q��
	inline const std::string &XML::encoding( )const
	{
		return encoding_;
	}
		
	/// @brief standalone�������擾����
	/// @return standalone�����̎Q��
	inline std::string &XML::standalone( )
	{
		return standalone_;
	}
		
	/// @brief standalone�������擾����
	/// @return standalone�����̒萔�Q��
	inline const std::string &XML::standalone( )const
	{
		return standalone_;
	}

	/// @brief XML���t�@�C������ǂݍ���
	/// @attention XML�t�@�C���̓��e�ɂ���Ă͐���ɓǂݍ��߂Ȃ��\��������
	/// @param[in] file_name �ǂݍ���XML�t�@�C����
	/// @retval true �t�@�C���̓ǂݍ��݂ɐ�������
	/// @retval false �t�@�C���̓ǂݍ��݂Ɏ��s�����D���炭�C�t�@�C�����J���Ȃ�����
	inline bool XML::read( const char *file_name )
	{
		std::ifstream ifs( file_name ); // ���̓t�@�C���X�g���[��
		std::stringstream ss; // ���͕�����X�g���[��
		std::string buf; // �o�b�t�@

		// �G���[�`�F�b�N
		if ( !ifs )
		{
			return false;
		}

		// ���̂��t�@�C����S�ēǂݍ��܂Ȃ��ƃG���[�������鋰�ꂪ����
		while ( !ifs.eof( ) )
		{
			ss << static_cast< char >( ifs.get( ) );
		}

		// �؍\������ɂ���
		clear( );

		// �u<?xml�v��ǂݔ�΂�
		ss.ignore( INT_MAX, 'l' );

		// ������ǂݍ���
		while ( true )
		{
			// �󔒂�ǂݔ�΂�
			while ( std::isspace( ss.peek( ) ) )
			{
				ss.ignore( );
			}

			if ( ss.peek( ) != '?' )
			{
				// �����̖��O��ǂݍ���
				get_str( ss, buf, '=', true );

				//�u ="�v��ǂݔ�΂�
				ss.ignore( INT_MAX, '\"' );

				if ( buf == "version" )
				{
					// version������ǂݍ���
					get_str( ss, version_, '\"' );
				}
				else if ( buf == "encoding" )
				{
					// encoding������ǂݍ���
					get_str( ss, encoding_, '\"' );
				} 
				else if ( buf == "standalone" )
				{
					// standalone������ǂݍ���
					get_str( ss, standalone_, '\"' );
				}
					
				// �u"�v��ǂݔ�΂�
				ss.ignore( INT_MAX, '\"' );
			}
			else
			{
				// �u?>�v��ǂݔ�΂�
				ss.ignore( INT_MAX, '>' );

				break;
			}
		}

		// �v�f��ǂݍ���
		read( ss, insert( end( ), Element( ) ) );

		return true;
	}

	/// @brief XML���t�@�C���ɏ�������
	/// @param[in] file_name ��������XML�t�@�C����
	/// @retval true �������݂ɐ�������
	/// @retval false �������݂����s�����D���炭�C�t�@�C�����J���Ȃ�����
	inline bool XML::write( const char *file_name )const
	{
		std::ofstream ofs( file_name ); // �o�̓t�@�C���X�g���[��

		// �G���[�`�F�b�N
		if ( !ofs )
		{
			return false;
		}

		// XML�錾����������
		ofs << "<?xml";
		
		if ( !version_.empty( ) )
		{
			ofs << " version=\"" << ( version_ != "1.1" ? "1.0" : "1.1" ) << '\"';
		}
		
		if ( !encoding_.empty( ) )
		{
			ofs << " encoding=\"" << encoding_ << '\"';
		}
		
		if ( !standalone_.empty( ) )
		{
			ofs << " standalone=\"" << ( standalone_ != "no" ? "yes" : "no" ) << '\"';
		}

		ofs << "?>" << std::endl;

		// �v�f����������
		write( ofs, preorder_begin( ) );

		return true;
	}

	/// @brief �v�f���X�g���[������ǂݍ���
	/// @param[in] ss �ǂݍ��ޓ��̓X�g���[��
	/// @param[in] elem �ǂݍ��ޗv�f���w���C�e���[�^
	/// @return �ǂݍ��񂾗v�f���w���C�e���[�^
	inline XML::iterator XML::read( std::stringstream &ss, iterator elem )
	{
		bool cont = true; // �p���t���O

		// �u<�v��ǂݔ�΂�
		ss.ignore( INT_MAX, '<' );

		// �^�O��ǂݍ���
		get_str( ss, elem->tag( ).name( ), '>', true );

		if ( !elem->tag( ).name( ).empty( ) && elem->tag( ).name( )[ elem->tag( ).name( ).size( ) - 1 ] == '/' )
		{
			// ��^�O�̂Ƃ�
			elem->tag( ).name( ).resize( elem->tag( ).name( ).size( ) - 1 );

			return elem;
		}

		// ������ǂݍ���
		while ( cont )
		{
			// �󔒂�ǂݔ�΂�
			while ( std::isspace( ss.peek( ) ) )
			{
				ss.ignore( );
			}

			switch ( ss.peek( ) )
			{
			case '/':
				// �u/>�v��ǂݔ�΂�
				ss.ignore( INT_MAX, '>' );

				return elem;
			case '>':
				// �u>�v��ǂݔ�΂�
				ss.ignore( INT_MAX, '>' );
				cont = false;
					
				break;
			default:
				std::string prop, val; // �o�b�t�@

				// �����̖��O��ǂݍ���
				get_str( ss, prop, '=', true );

				// �u="�v��ǂݔ�΂�
				ss.ignore( INT_MAX, '\"' );

				// �����̒l��ǂݍ���
				get_str( ss, val, '\"' );

				// �u"�v��ǂݔ�΂�
				ss.ignore( INT_MAX, '\"' );

				// ������ݒ肷��
				elem->tag( ).insert( elem->tag( ).end( ), Element::Tag::value_type( prop, val ) );
			}
		}

		// ���e�C�R�����g�C�I���^�O�܂��͎q�v�f��ǂݍ���
		while ( true )
		{
			// �󔒂�ǂݔ�΂�
			while ( std::isspace( ss.peek( ) ) )
			{
				ss.ignore( );
			}

			if ( ss.peek( ) == '<' )
			{
				// �u<�v��ǂݔ�΂�
				ss.ignore( INT_MAX, '<' );

				if ( ss.peek( ) == '/' )
				{
					// �I���^�O��ǂݔ�΂�
					ss.ignore( INT_MAX, '>' );

					break;
				}
				else if ( ss.peek( ) == '!' )
				{
					char temp[ 4 ]; // �ꎞ�̈�

					ss.get( temp, 4 );
					temp[ 3 ] = '\0';

					if ( std::strcmp( temp, "!--" ) )
					{
						// �q�v�f��ǂݍ���
						ss.putback( temp[ 2 ] );
						ss.putback( temp[ 1 ] );
						ss.putback( temp[ 0 ] );
						ss.putback( '<' );
						elem = parent( read( ss, insert( elem, Element( ) ) ) );
					}
					else
					{
						// �R�����g��ǂݔ�΂�
						ss.get( temp, 4 );

						while ( std::strcmp( temp, "-->" ) )
						{
							temp[ 0 ] = temp[ 1 ];
							temp[ 1 ] = temp[ 2 ];
							temp[ 2 ] = ss.get( );
						}
					}
				}
				else
				{
					// �q�v�f��ǂݍ���
					ss.putback( '<' );
					elem = parent( read( ss, insert( elem, Element( ) ) ) );
				}
			}
			else
			{
				// ���e��ǂݍ���
				get_str( ss, *elem->insert( elem->end( ), std::string( ) ), '<' );
			}
		}

		return elem;
	}

	/// @brief �v�f���t�@�C���X�g���[���ɏ�������
	/// @param[in] ofs �������ޏo�̓t�@�C���X�g���[��
	/// @param[in] elem �������ޗv�f���w���C�e���[�^
	inline void XML::write( std::ofstream &ofs, const_iterator elem )const
	{
		// �J�n�^�O�̏������݂��J�n����
		for ( size_t i = 0; i < depth( elem ); i++ )
		{
			ofs << '\t';
		}

		ofs << '<' << elem->tag( ).name( );

		// ��������������
		for ( Element::Tag::const_iterator iter = elem->tag( ).begin( ); iter != elem->tag( ).end( ); iter++ )
		{
			ofs << ' ' << iter->first << "=\"" << iter->second << '\"';
		}

		// �J�n�^�O�̏������݂��I������
		ofs << ( !is_leaf( elem ) || !elem->empty( ) ? "" : " /" ) << '>';

		if ( !is_leaf( elem ) || elem->empty( ) )
		{
			ofs << std::endl;
		}
		
		Element::const_iterator content_iter = elem->begin( ); // ���e�𑖍�����C�e���[�^
		const_iterator child_iter = child_begin( elem ); // �q�v�f�𑖍�����C�e���[�^

		while ( content_iter != elem->end( ) || child_iter != child_end( elem ) )
		{
			// ���e����������
			if ( content_iter != elem->end( ) )
			{
				if ( !is_leaf( elem ) )
				{
					for ( size_t i = 0; i < depth( elem ) + 1; i++ )
					{
						ofs << '\t';
					}
				}

				ofs << *content_iter;

				if ( !is_leaf( elem ) )
				{
					ofs << std::endl;
				}

				content_iter++;
			}

			// �q�v�f����������
			if ( child_iter != child_end( elem ) )
			{
				write( ofs, child_iter );
				child_iter++;
			}
		}

		// �I���^�O����������
		if ( !is_leaf( elem ) || !elem->empty( ) )
		{
			if ( !is_leaf( elem ) )
			{
				for ( size_t i = 0; i < depth( elem ); i++ )
				{
					ofs << '\t';
				}
			}
		
			ofs << "</" << elem->tag( ).name( ) << '>' << std::endl;
		}
	}
	
	/// @brief �X�g���[�����當������擾����
	/// @param[in] ss ���̓X�g���[��
	/// @param[in] delim �I�[�L���D����̒��O�܂œǂݍ��݂��s��
	/// @param[in] term_with_space �󔒂��I�[�L���Ƃ��Ĉ����t���O�D�K��l��false
	inline void XML::get_str( std::stringstream &ss, std::string &str, char delim, bool term_with_space )
	{
		// ����������
		str.clear( );

		// �ǂݍ���
		while ( ss.peek( ) != delim )
		{
			if ( term_with_space && std::isspace( ss.peek( ) ) )
			{
				break;
			}
			else
			{
				str += ss.get( );
			}
		}
	}

	/// @brief ����Ȃ�
	inline _Element::_Element( )
	{
	}

	/// @brief �v�f�𕡐�����
	/// @param[in] elem ��������v�f
	inline _Element::_Element( const _Element &elem ) : tag_( elem.tag_ ), std::vector< std::string >( elem )
	{
	}

	/// @brief �^�O��ݒ肷��
	/// @param[in] tag �ݒ肷��^�O
	inline _Element::_Element( const Tag &tag ) : tag_( tag )
	{
	}

	/// @brief �^�O����ݒ肷��
	/// @param[in] tag_name �ݒ肷��^�O��
	inline _Element::_Element( const std::string &tag_name ) : tag_( Tag( tag_name ) )
	{
	}

	/// @brief �^�O���擾����
	/// @return �^�O�̎Q��
	inline _Element::Tag &_Element::tag( )
	{
		return tag_;
	}

	/// @brief �^�O���擾����
	/// @return �^�O�̒萔�Q��
	inline const _Element::Tag &_Element::tag( )const
	{
		return tag_;
	}
		
	/// @brief �v�f���w��̃^�O�����������肷��
	/// @param[in] tag_name ���肷��^�O��
	/// @retval true �v�f�͎w��̃^�O��������
	/// @retval false �v�f�͎w��̃^�O���������Ȃ�
	inline bool _Element::operator==( const std::string &tag_name )const
	{
		return tag_.name( ) == tag_name;
	}

	/// @brief ����Ȃ�
	inline _Element::Tag::Tag( )
	{
	}

	/// @brief �^�O�𕡐�����
	/// @param[in] tag ��������^�O
	inline _Element::Tag::Tag( const Tag &tag ) : name_( tag.name_ ), dictionary< std::string, std::string >( tag )
	{
	}

	/// @brief �^�O����ݒ肷��
	/// @param[in] name �ݒ肷��^�O��
	inline _Element::Tag::Tag( const std::string &name ) : name_( name )
	{
	}

	/// @brief �^�O�����擾����
	/// @return �^�O���̎Q��
	inline std::string &_Element::Tag::name( )
	{
		return name_;
	}

	/// @brief �^�O�����擾����
	/// @return �^�O���̒萔�Q��
	inline const std::string &_Element::Tag::name( )const
	{
		return name_;
	}
}

#endif //_TMATSU_XML_H_