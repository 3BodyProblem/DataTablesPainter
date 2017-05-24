#ifndef __INFRASTRUCTURE__HASH_H__
#define __INFRASTRUCTURE__HASH_H__


#include <exception>
#include <functional>
#include <algorithm>


/**
 * @class			UInt2UIntHash
 * @brief			整型值到位置值的哈希映射表
 * @note			默认值的情况下，数据表长度是哈希桶长度的3倍(无锁)
 * @author			barry
 * @date			2017/4/2
 */
template<typename T_KEY_TYPE, typename T_VALUE_TYPE, const unsigned int MAX_BUCKET_SIZE=1024, const unsigned int MAX_DATATABLE_NUM=MAX_BUCKET_SIZE*3>
class CollisionHash
{
	struct T_ListNode
	{
	public:
		T_ListNode() : nDataPos( -1 ), pPrevNode(NULL), pNextNode( NULL ), nHashKey( 0 ) {};
		bool			IsNull()	{	return nDataPos < 0;	}		///< 节点是否有有效值
		bool			HasNext()	{	return pNextNode != NULL;	}	///< 节点是否有下一个值
		bool			IsFirst()	{	return pPrevNode == pNextNode && NULL != pNextNode;	}
		void			Clear()		{	nDataPos = -1; pNextNode = NULL; pPrevNode = NULL, nHashKey = 0;	}
	public:
		T_KEY_TYPE		nHashKey;									///< 健值保存
		int				nDataPos;									///< 数据所在的索引位置
		void*			pPrevNode;									///< 具有相同key的上一个值的索引位置
		void*			pNextNode;									///< 具有相同key的下一个值的索引位置
	};
public:
	CollisionHash();

	/**
	 * @brief			根据位置索引取得映射值
	 * @param[in]		nIndex		位置索引
	 * @return			取得对应值
	 */
	T_VALUE_TYPE*		Index( unsigned int nIndex );

	/**
	 * @brief			根据key取得对应的映射值
	 * @param[in]		nKey		索引值
	 * @return			返回映射值
	 * @note			如果key不存在，则抛出runtime异常
	 */
	T_VALUE_TYPE*		operator[]( T_KEY_TYPE nKey );

	/**
	 * @brief			设置键值对
	 * @param[in]		nKey		键值
	 * @param[in]		oData		数值
	 * @return			==1			设置成功
						==0			已经存在，不需要新建
						<0			失败
	 */
	int					NewKey( T_KEY_TYPE nKey, T_VALUE_TYPE oData );

	/**
	 * @brief			删除键值对
	 * @param[in]		nKey		键值
	 * @return			==1			设置成功
						==0			已经不存在，不需要删除
						<0			失败
	 */
	int					DeleteKey( T_KEY_TYPE nKey );

	/**
	 * @brief			获取元素的总数量
	 */
	unsigned int		Size();

	/**
	 * @brief			清空所有数据
	 */
	void				Clear();

protected:
	/**
	 * @brief			将一个节点从列表中移除
	 * @detail			打断链表/删除节点/重接节点前后两节点 + 将打断链表的后段前移 + 根据打断处地址要标准重新计算各节点的前后指针的偏移
	 * @param[in]		pNode2Erase				要移除节点的地址
	 * @param[in]		nErasePos				要移除节点的索引
	 * @param[in]		pArrayOfNode			节点列表的首地址
	 * @param[in,out]	nNodeTotalNumber		有效节点的个数
	 * return			true					操作成功
	 */
	bool				Erase1NodeFromList( struct T_ListNode* pNode2Erase, unsigned int nErasePos, struct T_ListNode* pArrayOfNode, unsigned int& nNodeTotalNumber );

private:
	T_ListNode			m_BucketOfHash[MAX_BUCKET_SIZE];			///< 哈稀桶
	T_ListNode			m_CollisionBucket[MAX_DATATABLE_NUM];		///< 哈稀碰撞桶
	T_VALUE_TYPE		m_ArrayOfData[MAX_DATATABLE_NUM];			///< 数据缓存表
	unsigned int		m_nUsedNumOfCollisionBucket;				///< 已经使用的节点数量(碰撞桶)
	unsigned int		m_nUsedNumOfArrayData;						///< 已经使用的数据缓存节点数据
};


template<typename T_KEY_TYPE, typename T_VALUE_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
CollisionHash<T_KEY_TYPE,T_VALUE_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::CollisionHash()
 : m_nUsedNumOfCollisionBucket( 0 ), m_nUsedNumOfArrayData( 0 )
{
	Clear();
}

template<typename T_KEY_TYPE, typename T_VALUE_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
void CollisionHash<T_KEY_TYPE,T_VALUE_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::Clear()
{
	m_nUsedNumOfArrayData = 0;
	m_nUsedNumOfCollisionBucket = 0;

	for( unsigned int n = 0; n < MAX_DATATABLE_NUM; n++ )
	{
		m_ArrayOfData[n] = T_VALUE_TYPE();
	}

	std::for_each( m_BucketOfHash, m_BucketOfHash+MAX_BUCKET_SIZE, std::mem_fun_ref(&T_ListNode::Clear) );
	std::for_each( m_CollisionBucket, m_CollisionBucket+MAX_DATATABLE_NUM, std::mem_fun_ref(&T_ListNode::Clear) );
}

template<typename T_KEY_TYPE, typename T_VALUE_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
int CollisionHash<T_KEY_TYPE,T_VALUE_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::NewKey( T_KEY_TYPE nKey, T_VALUE_TYPE oData )
{
	struct T_ListNode*		pLastNode = NULL;
	T_KEY_TYPE				nKeyPos = nKey % MAX_BUCKET_SIZE;
	struct T_ListNode*		pNode = m_BucketOfHash + nKeyPos;

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

template<typename T_KEY_TYPE, typename T_VALUE_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
bool CollisionHash<T_KEY_TYPE,T_VALUE_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::Erase1NodeFromList( struct T_ListNode* pNode2Erase
																									, unsigned int nErasePos
																									, struct T_ListNode* pArrayOfNode
																									, unsigned int& nNodeTotalNumber )
{
	struct T_ListNode*		pFirstCopyNode = pNode2Erase + 1;

	///< 先从链表中移除节点
	((struct T_ListNode*)(pNode2Erase->pPrevNode))->pNextNode = pNode2Erase->pNextNode;
	((struct T_ListNode*)(pNode2Erase->pNextNode))->pPrevNode = pNode2Erase->pPrevNode;

	///< 移掉中间被移除节点的被占空间（通过内存移动+指针重新计算偏移）
	nNodeTotalNumber -= 1;
	::memmove( pNode2Erase, pFirstCopyNode, nNodeTotalNumber - nErasePos );

	///< 在高于被移除节点地址的元素指针都进行前移sizeof(struct T_ListNode)
	for( unsigned int n = 0; n < nNodeTotalNumber; n++ )
	{
		struct T_ListNode&	refNode = pArrayOfNode[n];

		if( refNode.pPrevNode > pArrayOfNode && refNode.pPrevNode < (pArrayOfNode + nNodeTotalNumber + 1) )
		{
			if( refNode.pPrevNode > pNode2Erase )
			{
				refNode.pPrevNode = ((char*)(refNode.pPrevNode)) + sizeof(struct T_ListNode);
			}

			if( refNode.pNextNode > pNode2Erase )
			{
				refNode.pNextNode = ((char*)(refNode.pNextNode)) + sizeof(struct T_ListNode);
			}
		}

	}

	return true;
}

template<typename T_KEY_TYPE, typename T_VALUE_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
int CollisionHash<T_KEY_TYPE,T_VALUE_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::DeleteKey( T_KEY_TYPE nKey )
{
	T_KEY_TYPE				nKeyPos = nKey % MAX_BUCKET_SIZE;
	struct T_ListNode*		pNode = m_BucketOfHash + nKeyPos;

	while( false == pNode->IsNull() )
	{
		if( nKey == pNode->nHashKey )					///< 找到节点位置
		{
			if( pNode >= (m_BucketOfHash+0) && pNode < (m_BucketOfHash+MAX_BUCKET_SIZE) )				///< 节点在哈希桶内(头节点)
			{
				if( NULL == pNode->pNextNode )
				{
					pNode->Clear();
				}
				else
				{
					struct T_ListNode*	pNodeInCollisionBucket = (struct T_ListNode*)(pNode->pNextNode);
					::memcpy( pNode, pNodeInCollisionBucket, sizeof(struct T_ListNode) );
					pNode->pPrevNode = pNode;
					((struct T_ListNode*)(pNode->pNextNode))->pPrevNode = pNode;

					Erase1NodeFromList( pNodeInCollisionBucket, (pNodeInCollisionBucket-m_CollisionBucket)/sizeof(struct T_ListNode), m_CollisionBucket, m_nUsedNumOfCollisionBucket );
				}
			}
			else if( pNode >= (m_CollisionBucket+0) && pNode < (m_CollisionBucket+MAX_DATATABLE_NUM) )	///< 节点在碰撞桶内(非头节点)
			{
				Erase1NodeFromList( pNode, nKeyPos, m_CollisionBucket, m_nUsedNumOfCollisionBucket );
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

template<typename T_KEY_TYPE, typename T_VALUE_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
unsigned int CollisionHash<T_KEY_TYPE,T_VALUE_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::Size()
{
	return m_nUsedNumOfArrayData;
}

template<typename T_KEY_TYPE, typename T_VALUE_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
T_VALUE_TYPE* CollisionHash<T_KEY_TYPE,T_VALUE_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::Index( unsigned int nIndex )
{
	struct T_ListNode*		pNode = m_BucketOfHash + nIndex;

	if( false == pNode->IsNull() )
	{
		return NULL;
	}

	return &(m_ArrayOfData[pNode->nDataPos]);
}

template<typename T_KEY_TYPE, typename T_VALUE_TYPE, const unsigned int MAX_BUCKET_SIZE, const unsigned int MAX_DATATABLE_NUM>
T_VALUE_TYPE* CollisionHash<T_KEY_TYPE,T_VALUE_TYPE,MAX_BUCKET_SIZE,MAX_DATATABLE_NUM>::operator[]( T_KEY_TYPE nKey )
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






