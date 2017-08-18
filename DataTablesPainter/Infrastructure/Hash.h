#ifndef __INFRASTRUCTURE__HASH_H__
#define __INFRASTRUCTURE__HASH_H__


#include <stdexcept>
#include <exception>
#include <functional>
#include <algorithm>


/**
 * @class			T_ListNode
 * @brief			哈希节点
 * @author			barry
 * @date			2017/4/2
 */
struct T_ListNode
{
public:
	T_ListNode() : nDataPos( -1 ), pPrevNode(NULL), pNextNode( NULL ), nHashKey( 0 ), nUpdateSequence( 0 ) {};
	bool				IsNull()	{	return nDataPos < 0;	}		///< 节点是否有有效值
	bool				HasNext()	{	return pNextNode != NULL;	}	///< 节点是否有下一个值
	bool				IsFirst()	{	return pPrevNode == pNextNode && NULL != pNextNode;	}
	void				Clear()		{	nDataPos = -1; pNextNode = NULL; pPrevNode = NULL, nHashKey = 0; nUpdateSequence = 0;	}
	void				Delete()
	{
		if( NULL != pPrevNode )	{
			((struct T_ListNode*)(pPrevNode))->pNextNode = pNextNode;
		}
		if( NULL != pNextNode )	{
			((struct T_ListNode*)(pNextNode))->pPrevNode = pPrevNode;
		}
	}
public:
	unsigned __int64	nHashKey;						///< 健值保存
	int					nDataPos;						///< 数据所在的索引位置
	unsigned __int64	nUpdateSequence;				///< 更新次数序号
	void*				pPrevNode;						///< 具有相同key的上一个值的索引位置
	void*				pNextNode;						///< 具有相同key的下一个值的索引位置
};


#if _DEBUG
#define		MAX_BUCKET_SIZE			(17)
#define		MAX_DATATABLE_NUM		(1024*8)
#else
#define		MAX_BUCKET_SIZE			(1024*8)
#define		MAX_DATATABLE_NUM		(MAX_BUCKET_SIZE*5)
#endif


/**
 * @class			UInt2UIntHash
 * @brief			整型值到位置值的哈希映射表
 * @note			默认值的情况下，数据表长度是哈希桶长度的3倍(无锁)
 * @author			barry
 * @date			2017/4/2
 */
class CollisionHash
{
public:
	CollisionHash();

	/**
	 * @brief			根据key取得对应的映射值
	 * @param[in]		nKey		索引值
	 * @return			返回映射值
	 * @note			如果key不存在，则抛出runtime异常
	 */
	struct T_ListNode*	operator[]( unsigned __int64 nKey );

	/**
	 * @brief			设置键值对
	 * @param[in]		nKey		键值
	 * @param[in]		nDataPos	数据位置索引
	 * @param[in]		nSeqNo		更新流水(全局)
	 * @return			==1			设置成功
						==0			已经存在，不需要新建
						<0			失败
	 */
	int					NewKey( unsigned __int64 nKey, int nDataPos, unsigned __int64 nSeqNo );

	/**
	 * @brief			删除键值对
	 * @param[in]		nKey		键值
	 * @return			==1			设置成功
						==0			已经不存在，不需要删除
						<0			失败
	 */
	int					DeleteKey( unsigned __int64 nKey );

	/**
	 * @brief			清空所有数据
	 */
	void				Clear();

public:///< 工具方法
	/**
	 * @brief			统一调整哈稀桶和碰撞桶内指针的地址偏移
	 * @param[in]		nOffset		偏移量(>0向后偏移, <0向前偏移)
	 */
	void				CoordinateNodePtr( int nOffset );

protected:
	/**
	 * @brief			将一个节点从列表中移除
	 * @detail			打断链表/删除节点/重接节点前后两节点 + 将打断链表的后段前移 + 根据打断处地址要标准重新计算各节点的前后指针的偏移
	 * @param[in]		pNode2Erase				要移除节点的地址
	 * return			true					操作成功
	 */
	bool				CoordinateCollisionBucketPtr( struct T_ListNode* pNode2Erase );

	/**
	 * @brief			将所有哈希桶内，next指针大于碰撞桶删除节点地址的，都做往前偏移调整
	 * @param[in]		pEraseNodeInCollisionBucket	碰撞桶内被删除节点的地址
	 * @return			true					操作成功
	 */
	bool				CoordinateHashBucketPtr( struct T_ListNode* pEraseNodeInCollisionBucket );

	/**
	 * @brief			当数据数组某节点删除时，调整索引数组的数据位置指向
	 * @param[in]		pNodeArray				需要被调整的索引数据地址
	 * @param[in]		nNodeCount				索引数据的有效长度
	 * @param[in]		nEraseIndex				被删除数值在数据数组中的位置
	 */
	void				CoordinateNodeIndex( struct T_ListNode* pNodeArray, unsigned int nNodeCount, int nEraseIndex );

private:
	T_ListNode			m_BucketOfHash[MAX_BUCKET_SIZE];			///< 哈稀桶
	T_ListNode			m_CollisionBucket[MAX_DATATABLE_NUM];		///< 哈稀碰撞桶
	unsigned int		m_nUsedNumOfCollisionBucket;				///< 已经使用的节点数量(碰撞桶)
};




#endif






