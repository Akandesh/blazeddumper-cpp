#pragma once
// Pasta'd from CS:GO Simple
enum SendPropType
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array,
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes
};

class DVariant
{
public:
	union
	{
		float	   m_Float;
		long	   m_Int;
		char*    m_pString;
		void*    m_pData;
		float	   m_Vector[3];
		__int64  m_Int64;
	};
	SendPropType  m_Type;
};

class RecvTable;

class RecvProp
{
public:
	char*                   m_pVarName;
	SendPropType            m_RecvType;
	int                     m_Flags;
	int                     m_StringBufferSize;
	int                     m_bInsideArray;
	const void*             m_pExtraData;
	RecvProp*               m_pArrayProp;
	void*					m_ArrayLengthProxy;
	void*			        m_ProxyFn;
	void*					m_DataTableProxyFn;
	RecvTable*              m_pDataTable;
	int                     m_Offset;
	int                     m_ElementStride;
	int                     m_nElements;
	const char*             m_pParentArrayPropName;
};

class RecvTable
{
public:
	RecvProp*               m_pProps;
	int                     m_nProps;
	void*                   m_pDecoder;
	char*                   m_pNetTableName;
	bool                    m_bInitialized;
	bool                    m_bInMainList;
};

struct ClientClass
{
public:
	void*		    m_pCreateFn;
	void*			m_pCreateEventFn;
	char*			m_pNetworkName;
	RecvTable*      m_pRecvTable;
	ClientClass*    m_pNext;
	int             m_ClassID;
};