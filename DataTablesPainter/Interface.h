#ifndef __MEM_DATABASE_INTERFACE_H__
#define	__MEM_DATABASE_INTERFACE_H__


/**
 * @classs			I_Record
 * @brief			��¼�����ӿ�
 */
class I_Record
{
public:
	/**
	 * @brief						��¼����Ϊ��Ч
	 * @return						true							��Ч����
	 */
	virtual bool					IsNone() const;

	/**
	 * @brief						��¼��deeply copy
	 * @param[in]					refRecord						�ڴ�copy��Դͷ
	 * @return						==1								Ӱ�������
									==0								����Ҫcopy��Ŀ���Դͷ������ȫһ��
									<0								���ִ���
	 */
	virtual int						CloneFrom( const I_Record& refRecord );

	/**
	 * @brief						ȡ�����ݵ�ַ
	 */
	virtual const char*				GetPtr() const;

	/**
	 * @brief						���ݳ���
	 */
	virtual unsigned int			Length() const;

	/**
	 * @brief						ȡ��������
	 */
	virtual int						GetSerial() const;
};


/**
 * @class			I_Table
 * @brief			���ݱ�����ӿ�
 */
class I_Table
{
public:
	/**
	 * @brief								׷��������
	 * @param[in]							refRecord				׷�ӵ�����
	 * @return								==0						���ӳɹ�
											!=0						ʧ��
	 */
	virtual int								InsertRecord( const I_Record& refRecord ) = 0; 

	/**
	 * @brief								��������¼����
	 * @param[in]							Index					��¼����
	 * @return								���ؼ�¼����
	 */
	virtual I_Record						SelectRecord( int nRecordIndex ) = 0;
};


/**
 * @class			I_Database
 * @brief			���ݿ�����ӿ�
 */
class I_Database
{
public:
	/**
	 * @brief					������Ϣid����Ϣ���ȣ����к��ʵ����ݱ����ã���Ԥ���������ö�Ӧ��ռ�ù�ϵ��
	 * @param[in]				nBindID				�������α�ʶ��
	 * @param[in]				nRecordWidth		���ݳ���
	 * @param[in]				nKeyStrLen			��������
	 * @return					=0					���óɹ�
								>0					���ԣ��ɹ���
								<0					���ó���
	 */
	virtual bool				CreateTable( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen ) = 0;

	/**
	 * @brief					����MessageIDȡ���Ѿ����ڵĻ��߷���һ���µ��ڴ�������
	 * @detail					��������ÿ��messageidά��һ��Ψһ�Ҷ�Ӧ���ڴ������nBindIDֵ�����Ѿ����ڵģ����½��󷵻�
	 * @param[in]				nBindID				MessageID
	 * @return					�����Ѿ����ڵ��ڴ����½����ڴ��
	 */
	virtual I_Table*			QueryTable( unsigned int nBindID ) = 0;

	/**
	 * @brief					�����������ݱ�
	 */
	virtual void				DeleteTables() = 0;

	/**
	 * @brief					��Ӳ�ָ̻���������
	 */
	virtual bool				LoadFromDisk( const char* pszDataFile ) = 0;

	/**
	 * @brief					���������ݴ���
	 */
	virtual bool				SaveToDisk( const char* pszDataFile ) = 0;
};


/**
 * @class				DBFactory
 * @brief				�ڴ����ݷ����������
 */
class DBFactory
{
public:
	DBFactory();
};





#endif





