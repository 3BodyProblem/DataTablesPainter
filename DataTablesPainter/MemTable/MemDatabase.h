#ifndef __MEM_TABLE__MEM_DATABASE_H__
#define	__MEM_TABLE__MEM_DATABASE_H__


#include <set>
#include <vector>
#include "MemTable.h"
#include "../Interface.h"


namespace MemoryCollection
{


const unsigned int			MAX_TABBLE_NO = 64;				///< ���ɷ�������ݱ������


/**
 * @class			MemDatabase
 * @brief			�ڴ����ݿ�(�ڴ��ά����ȡ��)
 * @detail			��¼ÿ��BindID(messageid)��Ӧ���ݽṹ�ĳ��Ⱥͷ���ĺ��ʵ����ݱ�λ��
					&&
					���Ҹ������ϼ�¼����ѡ��+��ȡ
 * @author			barry
 * @date			2017/4/2
 */
class MemDatabase : public I_Database
{
public:
	MemDatabase();
	~MemDatabase();

public:///< Method Of Interface
	/**
	 * @brief					������Ϣid����Ϣ���ȣ����к��ʵ����ݱ����ã���Ԥ���������ö�Ӧ��ռ�ù�ϵ��
	 * @param[in]				nBindID				�������α�ʶ��
	 * @param[in]				nRecordWidth		���ݳ���
	 * @param[in]				nKeyStrLen			��������
	 * @return					true				���óɹ�
	 */
	bool						CreateTable( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen );

	/**
	 * @brief					ɾ��ָ�������ݱ�
	 * @param[in]				nBindID				���ݱ�ID
	 * @return					true				ɾ���ɹ�
	 */
	bool						DeleteTable( unsigned int nBindID );

	/**
	 * @brief					����MessageIDȡ���Ѿ����ڵĻ��߷���һ���µ��ڴ�������
	 * @detail					��������ÿ��messageidά��һ��Ψһ�Ҷ�Ӧ���ڴ������nBindIDֵ�����Ѿ����ڵģ����½��󷵻�
	 * @param[in]				nBindID				MessageID
	 * @return					�����Ѿ����ڵ��ڴ����½����ڴ��
	 */
	I_Table*					QueryTable( unsigned int nBindID );

	/**
	 * @brief					�����������ݱ�
	 * @return					==0					�ɹ�
								!=					����
	 */
	int							DeleteTables();

	/**
	 * @brief					��Ӳ�ָ̻���������
	 * @return					>0					���ݿ���������ļ�����
								<0					����
	 */
	int							LoadFromDisk( const char* pszDataFile );

	/**
	 * @brief					���������ݴ���
	 */
	bool						SaveToDisk( const char* pszDataFile );

	/**
	 * @brief					ȡ�����ݸ���������ˮ��
	 */
	unsigned __int64			GetUpdateSequence();

	/**
	 * @brief					ȡ�����ݱ������
	 * @return					����ͳ��ֵ
	 */
	unsigned int				GetTableCount();

	/**
	 * @brief					����λ������ȡ�����ݱ�Ԫ��Ϣ
	 * @param[in]				���ȡ���ݱ��λ��
	 * @param[out]				nDataID				���ݱ�ID
	 * @param[out]				nRecordLen			���ݱ��¼����
	 * @param[out]				nKeyStrLen			����������󳤶�
	 * @return					true				��ȡ�ɹ�
								false				��ȡ����
	 */
	bool						GetTableMetaByPos( unsigned int nPos, unsigned int& nDataID, unsigned int& nRecordLen, unsigned int& nKeyStrLen );

public:
	/**
	 * @brief					����˳������ֵ��ȡ�����ݱ�����
	 * @return					NULL			��Ч������ֵ�᷵��null
	 */
	I_Table*					operator[]( unsigned int nTableIndex );

private:
	CriticalObject				m_oCSLock;									///< �ڴ����
	CollisionHash				m_HashTableOfPostion;						///< ��ϡ��,msgid���ڵ�����ѡ������
	DynamicTable				m_arrayQuotationTables[MAX_TABBLE_NO];		///< ���鶯̬����
	unsigned int				m_nUsedTableNum;							///< �Ѿ�ʹ�õ����ݱ����������
	std::set<unsigned int>		m_setTableID;								///< ���ݱ�ID����
private:
	char*						m_pQueryBuffer;								///< ���ݲ�ѯ����
};




}


#endif






