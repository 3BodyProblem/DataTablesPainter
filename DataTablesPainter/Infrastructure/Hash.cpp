#include "Hash.h"


CollisionHash::CollisionHash()
 : m_nUsedNumOfCollisionBucket( 0 )
{
	Clear();
}

void CollisionHash::Clear()
{
	m_nUsedNumOfCollisionBucket = 0;

	std::for_each( m_BucketOfHash, m_BucketOfHash+MAX_BUCKET_SIZE, std::mem_fun_ref(&T_ListNode::Clear) );
	std::for_each( m_CollisionBucket, m_CollisionBucket+MAX_DATATABLE_NUM, std::mem_fun_ref(&T_ListNode::Clear) );
}

int CollisionHash::NewKey( unsigned __int64 nKey, int nDataPos, unsigned __int64 nSeqNo )
{
	unsigned __int64		nKeyPos = nKey % MAX_BUCKET_SIZE;
	struct T_ListNode*		pNode = m_BucketOfHash + nKeyPos;

	if( m_nUsedNumOfCollisionBucket >= (MAX_DATATABLE_NUM-1) )
	{
		char	pszError[128*2] = { 0 };
		::sprintf( pszError, "CollisionHash::NewKey() : data buffer is full : %u >= %u", m_nUsedNumOfCollisionBucket, (MAX_DATATABLE_NUM-1) );
		throw std::runtime_error( pszError );
	}

	if( true == pNode->IsNull() )
	{
		pNode->nHashKey = nKey;
		pNode->nDataPos = nDataPos;
		pNode->nUpdateSequence = nSeqNo;
		pNode->pPrevNode = NULL;
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

			pNewNodeOfCollision->nHashKey = nKey;
			pNewNodeOfCollision->nDataPos = nDataPos;
			pNewNodeOfCollision->nUpdateSequence = nSeqNo;
			pNewNodeOfCollision->pPrevNode = pNode;
			pNewNodeOfCollision->pNextNode = NULL;

			pNode->pNextNode = pNewNodeOfCollision;

			return 1;
		}

		pNode = (struct T_ListNode*)(pNode->pNextNode);
	}

	return -1;
}

bool CollisionHash::CoordinateCollisionBucketPtr( struct T_ListNode* pNode2Erase )
{
	struct T_ListNode*		pFirstCopyNode = pNode2Erase + 1;
	int						nOffset = ((char*)pNode2Erase - (char*)m_CollisionBucket);
	unsigned int			nErasePos = nOffset/sizeof(struct T_ListNode);

	///< 移掉中间被移除节点的被占空间（通过内存移动+指针重新计算偏移）
	::memmove( pNode2Erase, pFirstCopyNode, (m_nUsedNumOfCollisionBucket - nErasePos - 1)*sizeof(T_ListNode) );
	m_nUsedNumOfCollisionBucket -= 1;

	///< 在高于被移除节点地址的元素指针都进行前移sizeof(struct T_ListNode)
	for( unsigned int n = 0; n < m_nUsedNumOfCollisionBucket; n++ )
	{
		struct T_ListNode&	refNode = m_CollisionBucket[n];

		if( refNode.pPrevNode >= m_CollisionBucket && refNode.pPrevNode <= (m_CollisionBucket + m_nUsedNumOfCollisionBucket) )
		{
			if( refNode.pPrevNode > pNode2Erase )
			{
				refNode.pPrevNode = ((char*)(refNode.pPrevNode)) - sizeof(struct T_ListNode);
			}
		}

		if( refNode.pNextNode >= m_CollisionBucket && refNode.pNextNode <= (m_CollisionBucket + m_nUsedNumOfCollisionBucket) )
		{
			if( refNode.pNextNode > pNode2Erase )
			{
				refNode.pNextNode = ((char*)(refNode.pNextNode)) - sizeof(struct T_ListNode);
			}
		}
	}

	return true;
}

bool CollisionHash::CoordinateHashBucketPtr( struct T_ListNode* pEraseNodeInCollisionBucket )
{
	for( int n = 0; n < MAX_BUCKET_SIZE; n++ )
	{
		struct T_ListNode&	refNode = m_BucketOfHash[n];

		if( refNode.pNextNode > pEraseNodeInCollisionBucket )
		{
			refNode.pNextNode = ((char*)(refNode.pNextNode)) - sizeof(struct T_ListNode);
		}
	}

	return true;
}

void CollisionHash::CoordinateNodeIndex( struct T_ListNode* pNodeArray, unsigned int nNodeCount, int nEraseIndex )
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

void CollisionHash::CoordinateNodePtr( int nOffset )
{
	for( int n = 0; n < MAX_BUCKET_SIZE; n++ )
	{
		T_ListNode&	refNode = m_BucketOfHash[n];

		if( refNode.pNextNode )
		{
			refNode.pNextNode = (char*)(refNode.pNextNode) + nOffset;
		}

		if( refNode.pPrevNode )
		{
			refNode.pPrevNode = (char*)(refNode.pPrevNode) + nOffset;
		}
	}

	for( int n = 0; n < MAX_DATATABLE_NUM; n++ )
	{
		T_ListNode& refNode = m_CollisionBucket[n];

		if( refNode.pNextNode )
		{
			refNode.pNextNode = (char*)(refNode.pNextNode) + nOffset;
		}

		if( refNode.pPrevNode )
		{
			refNode.pPrevNode = (char*)(refNode.pPrevNode) + nOffset;
		}
	}
}

int CollisionHash::DeleteKey( unsigned __int64 nKey )
{
	struct T_ListNode*		pNode = m_BucketOfHash + (nKey % MAX_BUCKET_SIZE);

	while( false == pNode->IsNull() )
	{
		if( nKey == pNode->nHashKey )								///< 找到节点位置
		{
			struct T_ListNode*	pEraseNodeInCollisionBucket = pNode;
			unsigned int		nPosInDataArray = pNode->nDataPos;	///< 该节点指向数据数组的索引位置
			bool				bNodeInHashBucket = (pNode >= (m_BucketOfHash+0) && pNode < (m_BucketOfHash+MAX_BUCKET_SIZE));
			bool				bNodeInCollisionBucket = (pNode >= (m_CollisionBucket+0) && pNode < (m_CollisionBucket+MAX_DATATABLE_NUM));
			if( false == bNodeInHashBucket && false == bNodeInCollisionBucket )	{	return -1;	}

			///< 调整位置索引数组(前移) + 调整链表节点各指向(前移)
			CoordinateNodeIndex( m_BucketOfHash, MAX_BUCKET_SIZE, nPosInDataArray );
			CoordinateNodeIndex( m_CollisionBucket, m_nUsedNumOfCollisionBucket, nPosInDataArray );

			if( true == bNodeInHashBucket )							///< 节点在哈希桶内(头节点)
			{
				if( NULL == pNode->pNextNode )	{					///< 但在无后续节点
					pNode->Clear();
					return 1;
				}

				///< 有后续节点，需将碰撞桶中的关联节点copy到关联桶
				pEraseNodeInCollisionBucket = (struct T_ListNode*)(pNode->pNextNode);
				::memcpy( pNode, pEraseNodeInCollisionBucket, sizeof(struct T_ListNode) );
				pNode->pPrevNode = NULL;
			}
			else if( true == bNodeInCollisionBucket )				///< 节点在碰撞桶内(非头节点)
			{
				pNode->Delete();
			}

			///< 调整哈希桶中，next指针高于被移动节点的地址，前移
			CoordinateHashBucketPtr( pEraseNodeInCollisionBucket );
			///< 调整碰撞桶中，删除链表节点后的数据前移（包括节点中的指针和位置索引值)
			CoordinateCollisionBucketPtr( pEraseNodeInCollisionBucket );
			///< 把调整后的碰撞桶的节点的前节点，重新指向hash桶地址
			if( NULL != pNode->pNextNode && true == bNodeInHashBucket )	{
				((struct T_ListNode*)(pNode->pNextNode))->pPrevNode = pNode;
			}

			return 1;
		}
		else if( true == pNode->HasNext() )
		{
			pNode = (struct T_ListNode*)(pNode->pNextNode);
			continue;
		}

		break;
	}

	return 0;
}

struct T_ListNode* CollisionHash::operator[]( unsigned __int64 nKey )
{
	unsigned __int64		nKeyPos = nKey % MAX_BUCKET_SIZE;
	struct T_ListNode*		pNode = m_BucketOfHash + nKeyPos;

	while( false == pNode->IsNull() )
	{
		if( nKey == pNode->nHashKey )
		{
			return pNode;
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








