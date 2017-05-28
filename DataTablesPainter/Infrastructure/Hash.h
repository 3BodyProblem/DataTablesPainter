#ifndef __INFRASTRUCTURE__HASH_H__
#define __INFRASTRUCTURE__HASH_H__


#include <exception>
#include <functional>
#include <algorithm>


/**
 * @class			T_RECORD_POS
 * @brief			���ݼ�¼λ����Ϣ�ṹ
 * @author			barry
 * @date			2017/4/2
 */
struct T_RECORD_POS
{
	T_RECORD_POS() { nRecordPos = -1; nUpdateSequence = 0; }
	T_RECORD_POS( unsigned int nPos, unsigned __int64 nUpdateSeqNo ) { nRecordPos = nPos; nUpdateSequence = nUpdateSeqNo; }
	void				Clear()	{	nRecordPos = -1;	}
	bool				Empty() const	{	if( nRecordPos < 0 )	return true;	else	return false;	}
	int					nRecordPos;				///< ʹ�����ݱ������λ��
	unsigned __int64	nUpdateSequence;		///< ���´������
};


/**
 * @class			UInt2UIntHash
 * @brief			����ֵ��λ��ֵ�Ĺ�ϣӳ���
 * @note			Ĭ��ֵ������£����ݱ����ǹ�ϣͰ���ȵ�3��(����)
 * @author			barry
 * @date			2017/4/2
 */
template<typename T_KEY_TYPE, const unsigned int MAX_BUCKET_SIZE=1024, const unsigned int MAX_DATATABLE_NUM=MAX_BUCKET_SIZE*3>
class CollisionHash
{
	struct T_ListNode
	{
	public:
		T_ListNode() : nDataPos( -1 ), pPrevNode(NULL), pNextNode( NULL ), nHashKey( 0 ) {};
		bool			IsNull()	{	return nDataPos < 0;	}		///< �ڵ��Ƿ�����Чֵ
		bool			HasNext()	{	return pNextNode != NULL;	}	///< �ڵ��Ƿ�����һ��ֵ
		bool			IsFirst()	{	return pPrevNode == pNextNode && NULL != pNextNode;	}
		void			Clear()		{	nDataPos = -1; pNextNode = NULL; pPrevNode = NULL, nHashKey = 0;	}
	public:
		T_KEY_TYPE		nHashKey;									///< ��ֵ����
		int				nDataPos;									///< �������ڵ�����λ��
		void*			pPrevNode;									///< ������ͬkey����һ��ֵ������λ��
		void*			pNextNode;									///< ������ͬkey����һ��ֵ������λ��
	};
public:
	CollisionHash();

	/**
	 * @brief			����λ������ȡ��ӳ��ֵ
	 * @param[in]		nIndex		λ������
	 * @return			ȡ�ö�Ӧֵ
	 */
	T_RECORD_POS*		Index( unsigned int nIndex );

	/**
	 * @brief			����keyȡ�ö�Ӧ��ӳ��ֵ
	 * @param[in]		nKey		����ֵ
	 * @return			����ӳ��ֵ
	 * @note			���key�����ڣ����׳�runtime�쳣
	 */
	T_RECORD_POS*		operator[]( T_KEY_TYPE nKey );

	/**
	 * @brief			���ü�ֵ��
	 * @param[in]		nKey		��ֵ
	 * @param[in]		oData		��ֵ
	 * @return			==1			���óɹ�
						==0			�Ѿ����ڣ�����Ҫ�½�
						<0			ʧ��
	 */
	int					NewKey( T_KEY_TYPE nKey, T_RECORD_POS oData );

	/**
	 * @brief			ɾ����ֵ��
	 * @param[in]		nKey		��ֵ
	 * @return			==1			���óɹ�
						==0			�Ѿ������ڣ�����Ҫɾ��
						<0			ʧ��
	 */
	int					DeleteKey( T_KEY_TYPE nKey );

	/**
	 * @brief			��ȡԪ�ص�������
	 */
	unsigned int		Size();

	/**
	 * @brief			�����������
	 */
	void				Clear();

protected:
	/**
	 * @brief			��һ���ڵ���б����Ƴ�
	 * @detail			�������/ɾ���ڵ�/�ؽӽڵ�ǰ�����ڵ� + ���������ĺ��ǰ�� + ���ݴ�ϴ���ַҪ��׼���¼�����ڵ��ǰ��ָ���ƫ��
	 * @param[in]		pNode2Erase				Ҫ�Ƴ��ڵ�ĵ�ַ
	 * @param[in]		nErasePos				Ҫ�Ƴ��ڵ������
	 * @param[in]		pArrayOfNode			�ڵ��б���׵�ַ
	 * @param[in,out]	nNodeTotalNumber		��Ч�ڵ�ĸ���
	 * @param[in]		nPosInDataArray			ɾ���ڵ�ָ�����������������е�����λ��
	 * return			true					�����ɹ�
	 */
	bool				Erase1NodeFromList( struct T_ListNode* pNode2Erase, unsigned int nErasePos, struct T_ListNode* pArrayOfNode, unsigned int& nNodeTotalNumber, unsigned int nPosInDataArray );

	/**
	 * @brief			����������ĳ�ڵ�ɾ��ʱ�������������������λ��ָ��
	 * @param[in]		pNodeArray				��Ҫ���������������ݵ�ַ
	 * @param[in]		nNodeCount				�������ݵ���Ч����
	 * @param[in]		nEraseIndex				��ɾ����ֵ�����������е�λ��
	 */
	void				CoordinateNodeIndex( struct T_ListNode* pNodeArray, unsigned int nNodeCount, int nEraseIndex );

	/**
	 * @brief			����������ĳ�ڵ�ɾ��ʱ�������������ݵ�λ��ָ��
	 * @param[in]		pDataArray				��������
	 * @param[in]		nDataCount				��������
	 * @param[in]		nErasePos				��ɾ�����ݵ�λ������
	 */
	void				CoordinateDataIndex( T_RECORD_POS* pDataArray, unsigned int nDataCount, int nErasePos );

private:
	T_ListNode			m_BucketOfHash[MAX_BUCKET_SIZE];			///< ��ϡͰ
	T_ListNode			m_CollisionBucket[MAX_DATATABLE_NUM];		///< ��ϡ��ײͰ
	T_RECORD_POS		m_ArrayOfData[MAX_DATATABLE_NUM];			///< ���ݻ����
	unsigned int		m_nUsedNumOfCollisionBucket;				///< �Ѿ�ʹ�õĽڵ�����(��ײͰ)
	unsigned int		m_nUsedNumOfArrayData;						///< �Ѿ�ʹ�õ����ݻ���ڵ�����
};


template<typename T_KEY_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
CollisionHash<T_KEY_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::CollisionHash()
 : m_nUsedNumOfCollisionBucket( 0 ), m_nUsedNumOfArrayData( 0 )
{
	Clear();
}

template<typename T_KEY_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
void CollisionHash<T_KEY_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::Clear()
{
	m_nUsedNumOfArrayData = 0;
	m_nUsedNumOfCollisionBucket = 0;

	for( unsigned int n = 0; n < MAX_DATATABLE_NUM; n++ )
	{
		m_ArrayOfData[n] = T_RECORD_POS();
	}

	std::for_each( m_BucketOfHash, m_BucketOfHash+MAX_BUCKET_SIZE, std::mem_fun_ref(&T_ListNode::Clear) );
	std::for_each( m_CollisionBucket, m_CollisionBucket+MAX_DATATABLE_NUM, std::mem_fun_ref(&T_ListNode::Clear) );
}

template<typename T_KEY_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
int CollisionHash<T_KEY_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::NewKey( T_KEY_TYPE nKey, T_RECORD_POS oData )
{
	T_KEY_TYPE				nKeyPos = nKey % MAX_BUCKET_SIZE;
	struct T_ListNode*		pNode = m_BucketOfHash + nKeyPos;
	struct T_ListNode*		pLastNode = pNode;

	if( m_nUsedNumOfArrayData >= (MAX_DATATABLE_NUM-1) )
	{
		throw std::runtime_error( "CollisionHash::NewKey() : data buffer is full." );
	}

	if( true == pNode->IsNull() )
	{
		m_ArrayOfData[m_nUsedNumOfArrayData] = oData;
		pNode->nHashKey = nKey;
		pNode->nDataPos = m_nUsedNumOfArrayData++;
		pNode->pPrevNode = pNode;
		pNode->pNextNode = NULL;
		return 1;
	}

	while( true )
	{
		if( nKey == pNode->nHashKey )
		{
			return 0;
		}

		if( false == pNode->HasNext() )
		{
			struct T_ListNode* pNewNodeOfCollision = m_CollisionBucket+m_nUsedNumOfCollisionBucket++;

			m_ArrayOfData[m_nUsedNumOfArrayData] = oData;
			pNewNodeOfCollision->nHashKey = nKey;
			pNewNodeOfCollision->nDataPos = m_nUsedNumOfArrayData++;
			pNewNodeOfCollision->pPrevNode = pNode;
			pNewNodeOfCollision->pNextNode = NULL;

			pNode->pPrevNode = pLastNode;
			pNode->pNextNode = pNewNodeOfCollision;

			return 1;
		}
		else
		{
			pLastNode = pNode;
			pNode = (struct T_ListNode*)(pNode->pNextNode);
		}
	}

	return -1;
}

template<typename T_KEY_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
bool CollisionHash<T_KEY_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::Erase1NodeFromList( struct T_ListNode* pNode2Erase
																									, unsigned int nErasePos
																									, struct T_ListNode* pArrayOfNode
																									, unsigned int& nNodeTotalNumber
																									, unsigned int nPosInDataArray )
{
	struct T_ListNode*		pFirstCopyNode = pNode2Erase + 1;

	///< �Ƶ��м䱻�Ƴ��ڵ�ı�ռ�ռ䣨ͨ���ڴ��ƶ�+ָ�����¼���ƫ�ƣ�
	::memmove( pNode2Erase, pFirstCopyNode, (nNodeTotalNumber - nErasePos - 1)*sizeof(T_ListNode) );
	nNodeTotalNumber -= 1;

	///< �ڸ��ڱ��Ƴ��ڵ��ַ��Ԫ��ָ�붼����ǰ��sizeof(struct T_ListNode)
	for( unsigned int n = 0; n < nNodeTotalNumber; n++ )
	{
		struct T_ListNode&	refNode = pArrayOfNode[n];

		if( refNode.pPrevNode >= pArrayOfNode && refNode.pPrevNode <= (pArrayOfNode + nNodeTotalNumber) )
		{
			if( refNode.pPrevNode > pNode2Erase )
			{
				refNode.pPrevNode = ((char*)(refNode.pPrevNode)) - sizeof(struct T_ListNode);
			}
		}

		if( refNode.pNextNode >= pArrayOfNode && refNode.pNextNode <= (pArrayOfNode + nNodeTotalNumber) )
		{
			if( refNode.pNextNode > pNode2Erase )
			{
				refNode.pNextNode = ((char*)(refNode.pNextNode)) - sizeof(struct T_ListNode);
			}
		}

		if( refNode.nDataPos > nPosInDataArray )
		{
			refNode.nDataPos -= 1;
		}

	}

	return true;
}

template<typename T_KEY_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
void CollisionHash<T_KEY_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::CoordinateNodeIndex( struct T_ListNode* pNodeArray, unsigned int nNodeCount, int nEraseIndex )
{
	for( unsigned int n = 0; n < nNodeCount; n++ )
	{
		struct T_ListNode&	refNode = pNodeArray[n];

		if( refNode.nDataPos > nEraseIndex )
		{
			refNode.nDataPos -= 1;
		}
	}
}

template<typename T_KEY_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
void CollisionHash<T_KEY_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::CoordinateDataIndex( T_RECORD_POS* pDataArray, unsigned int nDataCount, int nErasePos )
{
	for( unsigned int n = 0; n < nDataCount; n++ )
	{
		T_RECORD_POS&	refData = m_ArrayOfData[n];

		if( refData.nRecordPos > nErasePos )
		{
			refData.nRecordPos -= 1;
		}
	}
}

template<typename T_KEY_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
int CollisionHash<T_KEY_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::DeleteKey( T_KEY_TYPE nKey )
{
	T_KEY_TYPE				nKeyPos = nKey % MAX_BUCKET_SIZE;
	struct T_ListNode*		pNode = m_BucketOfHash + nKeyPos;

	while( false == pNode->IsNull() )
	{
		if( nKey == pNode->nHashKey )								///< �ҵ��ڵ�λ��
		{
			unsigned int	nPosInDataArray = pNode->nDataPos;		///< �ýڵ�ָ���������������λ��

			if( pNode >= (m_BucketOfHash+0) && pNode < (m_BucketOfHash+MAX_BUCKET_SIZE) )				///< �ڵ��ڹ�ϣͰ��(ͷ�ڵ�)
			{
				///< ����λ����������(ǰ��) + ��������ڵ��ָ��(ǰ��)
				::memmove( m_ArrayOfData+nPosInDataArray, m_ArrayOfData+(nPosInDataArray+1), ((--m_nUsedNumOfArrayData)-nPosInDataArray)*sizeof(T_RECORD_POS) );
				CoordinateDataIndex( m_ArrayOfData, m_nUsedNumOfArrayData, nPosInDataArray );
				CoordinateNodeIndex( m_BucketOfHash, MAX_BUCKET_SIZE, nPosInDataArray );
				CoordinateNodeIndex( m_CollisionBucket, m_nUsedNumOfCollisionBucket, nPosInDataArray );

				if( NULL == pNode->pNextNode )	{
					pNode->Clear();
				}	else	{
					///< ����ײͰ�ĵڶ����ڵ��Ƶ���ϣͰ����Ϊ��һ���ڵ�
					struct T_ListNode*	pFirstNodeInCollisionBucket = (struct T_ListNode*)(pNode->pNextNode);
					::memcpy( pNode, pFirstNodeInCollisionBucket, sizeof(struct T_ListNode) );
					pNode->pPrevNode = NULL;
					///< ������ϣͰ�У�nextָ����ڱ��ƶ��ڵ�ĵ�ַ��ǰ��
					for( int n = 0; n < MAX_BUCKET_SIZE; n++ )	{
						struct T_ListNode&	refNode = m_BucketOfHash[n];
						if( refNode.pNextNode > pFirstNodeInCollisionBucket ) {
							refNode.pNextNode = ((char*)(refNode.pNextNode)) - sizeof(struct T_ListNode);
						}
					}

					///< ������ײͰ�У�ɾ������ڵ�������ǰ�ƣ������ڵ��е�ָ���λ������ֵ)
					int	nOffset = ((char*)pFirstNodeInCollisionBucket-(char*)m_CollisionBucket);
					Erase1NodeFromList( pFirstNodeInCollisionBucket, nOffset/sizeof(struct T_ListNode)
										, m_CollisionBucket, m_nUsedNumOfCollisionBucket, nPosInDataArray );
					///< ����󣬽�����ײͰ�Ƶ���ϣͰ�Ľڵ��nextָ�룬ָ����ײͰ����λ�õ�ַ
					if( NULL != pNode->pNextNode )
					{
						((struct T_ListNode*)(pNode->pNextNode))->pPrevNode = pNode;
					}
				}

				return 1;
			}
			else if( pNode >= (m_CollisionBucket+0) && pNode < (m_CollisionBucket+MAX_DATATABLE_NUM) )	///< �ڵ�����ײͰ��(��ͷ�ڵ�)
			{
				::memmove( m_ArrayOfData+nPosInDataArray, m_ArrayOfData+(nPosInDataArray+1), (--m_nUsedNumOfArrayData-(nPosInDataArray))*sizeof(T_RECORD_POS) );
				CoordinateDataIndex( m_ArrayOfData, m_nUsedNumOfArrayData, nPosInDataArray );
				CoordinateNodeIndex( m_BucketOfHash, MAX_BUCKET_SIZE, nPosInDataArray );
				CoordinateNodeIndex( m_CollisionBucket, m_nUsedNumOfCollisionBucket, nPosInDataArray );

				///< �ȴ��������Ƴ��ڵ�
				if( NULL != pNode->pPrevNode )	{
					((struct T_ListNode*)(pNode->pPrevNode))->pNextNode = pNode->pNextNode;
				}

				if( NULL != pNode->pNextNode )	{
					((struct T_ListNode*)(pNode->pNextNode))->pPrevNode = pNode->pPrevNode;
				}

				///< ������ϣͰ�У�nextָ����ڱ��ƶ��ڵ�ĵ�ַ��ǰ��
				for( int n = 0; n < MAX_BUCKET_SIZE; n++ )	{
					struct T_ListNode&	refNode = m_BucketOfHash[n];
					if( refNode.pNextNode > pNode ) {
						refNode.pNextNode = ((char*)(refNode.pNextNode)) - sizeof(struct T_ListNode);
					}
				}

				int	nOffset = ((char*)pNode-(char*)m_CollisionBucket);
				Erase1NodeFromList( pNode, nOffset/sizeof(struct T_ListNode)
									, m_CollisionBucket, m_nUsedNumOfCollisionBucket, nPosInDataArray );

				return 1;
			}
			else
			{
				return -1;
			}
		}
		else if( true == pNode->HasNext() )
		{
			pNode = (struct T_ListNode*)(pNode->pNextNode);
		}
		else
		{
			break;
		}
	}

	return 0;
}

template<typename T_KEY_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
unsigned int CollisionHash<T_KEY_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::Size()
{
	return m_nUsedNumOfArrayData;
}

template<typename T_KEY_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
T_RECORD_POS* CollisionHash<T_KEY_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::Index( unsigned int nIndex )
{
	struct T_ListNode*		pNode = m_BucketOfHash + nIndex;

	if( false == pNode->IsNull() )
	{
		return NULL;
	}

	return &(m_ArrayOfData[pNode->nDataPos]);
}

template<typename T_KEY_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
T_RECORD_POS* CollisionHash<T_KEY_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::operator[]( T_KEY_TYPE nKey )
{
	T_KEY_TYPE				nKeyPos = nKey % MAX_BUCKET_SIZE;
	struct T_ListNode*		pNode = m_BucketOfHash + nKeyPos;

	while( false == pNode->IsNull() )
	{
		if( nKey == pNode->nHashKey )
		{
			return &(m_ArrayOfData[pNode->nDataPos]);
		}
		else if( true == pNode->HasNext() )
		{
			pNode = (struct T_ListNode*)(pNode->pNextNode);
		}
		else
		{
			break;
		}
	}

	return NULL;
}


#endif






