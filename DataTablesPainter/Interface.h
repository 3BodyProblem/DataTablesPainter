#ifndef __MEM_DATABASE_INTERFACE_H__
#define	__MEM_DATABASE_INTERFACE_H__


/**
 * @classs							RecordBlock
 * @brief							��¼���ݿ���
 */
class RecordBlock
{
public:
	RecordBlock();
	RecordBlock( const RecordBlock& record );
	RecordBlock( char* pRecord, unsigned int nRecordLen );

public:
	/**
	 * @brief						��¼����Ϊ��Ч
	 * @return						true							��Ч����
	 */
	bool							IsNone() const;

	/**
	 * @brief						��¼��ȽϺ���
	 * @param[in]					refRecord						�Ƚϱ�Ķ���
	 * @return						true							��ͬ
									false							��ͬ
	 */
	bool							Compare( const RecordBlock& refRecord );

	/**
	 * @brief						��¼��deeply copy
	 * @param[in]					refRecord						�ڴ�copy��Դͷ
	 * @return						==1								Ӱ�������
									==0								����Ҫcopy��Ŀ���Դͷ������ȫһ��
									<0								���ִ���
	 */
	int								CloneFrom( const RecordBlock& refRecord );

	/**
	 * @brief						ȡ�����ݵ�ַ
	 */
	const char*						GetPtr() const;

	/**
	 * @brief						���ݳ���
	 */
	unsigned int					Length() const;

protected:
	char*							m_pRecordData;					///< ���ݽṹ��ַ
	unsigned int					m_nRecordLen;					///< ���ݽṹ����
};


/**
 * @class							I_Table
 * @brief							���ݱ������ӿ�
 */
class I_Table
{
public:
	/**
	 * @brief						׷��������
	 * @param[in]					pRecord					��¼���ַ
	 * @param[in]					nRecordLen				��¼�峤��
	 * @return						==0						���ӳɹ�
									!=0						ʧ��
	 */
	virtual int						InsertRecord( char* pRecord, unsigned int nRecordLen ) = 0; 

	/**
	 * @brief						��������¼����
	 * @param[in]					pKeyStr					������ַ
	 * @param[in]					nKeyLen					��������
	 * @return						���ؼ�¼����
	 */
	virtual RecordBlock				SelectRecord( char* pKeyStr, unsigned int nKeyLen ) = 0;
};


/**
 * @class							I_Database
 * @brief							���ݿ�����ӿ�
 */
class I_Database
{
public:
	virtual ~I_Database();

	/**
	 * @brief						������Ϣid����Ϣ���ȣ����к��ʵ����ݱ����ã���Ԥ���������ö�Ӧ��ռ�ù�ϵ��
	 * @param[in]					nBindID				�������α�ʶ��
	 * @param[in]					nRecordWidth		���ݳ���
	 * @param[in]					nKeyStrLen			��������
	 * @return						=0					���óɹ�
									>0					���ԣ��ɹ���
									<0					���ó���
	 */
	virtual bool					CreateTable( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen ) = 0;

	/**
	 * @brief						����MessageIDȡ���Ѿ����ڵĻ��߷���һ���µ��ڴ��������
	 * @detail						��������ÿ��messageidά��һ��Ψһ�Ҷ�Ӧ���ڴ��������nBindIDֵ�����Ѿ����ڵģ����½��󷵻�
	 * @param[in]					nBindID				MessageID
	 * @return						�����Ѿ����ڵ��ڴ�����½����ڴ��
	 */
	virtual I_Table*				QueryTable( unsigned int nBindID ) = 0;

	/**
	 * @brief						�����������ݱ�
	 */
	virtual void					DeleteTables() = 0;

	/**
	 * @brief						��Ӳ�ָ̻���������
	 */
	virtual bool					LoadFromDisk( const char* pszDataFile ) = 0;

	/**
	 * @brief						���������ݴ���
	 */
	virtual bool					SaveToDisk( const char* pszDataFile ) = 0;
};


/**
 * @class							DBFactory
 * @brief							�ڴ����ݷ������������
 */
class DBFactory
{
private:
	DBFactory();

public:
	/**
	 * @brief						ȡ�õ���
	 */
	static	DBFactory&				GetFactory();

public:
	/**
	 * @brief						�������������ݿ����ָ��
	 * @return						�������ݿ�ָ��ĵ�ַ
	 */
	I_Database*						GrapDatabaseInterface();

	/**
	 * @brief						�ͷŷ�����������ݿ����
	 */
	bool							ReleaseAllDatabase();

};





#endif








