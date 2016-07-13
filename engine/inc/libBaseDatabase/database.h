#pragma once

#include "libBaseCommon/base_common.h"
#include "libBaseCommon/variant.h"

/*
mysql ��ȡ�����ʱ���ѡ���Ƿ񻺳�������������Ҹ������з����ó��Լ���ѡ����
1. ��ѯִ��֮������������϶�������mysql�ڲ������Ǹ���mysql�ĵ�:(you must retrieve all the rows even if you determine in mid-retrieval that you've found the information you were looking for.)
2. ������ڵ���StoreResult����UseResult+ȫ��FetchRow��ǰ��Ҫô����mysql�ڲ���Ҫô�����ⲿ���ڴ�û��ʲô��ʡ,
3. ʹ��UseResult���������ҪFetchȫ��Row֮�����֪��RowCount,��ܲ�����

���������IDbRecordset����IDbStatement,��ѡ��StoreResult�ķ���

����mysql�����˶����Ĳ�ѯ����������Ϊ�˷�ֹSQLע����������ֻ��һ�������ִ�У���ôSQLһ����ע��ͻᱨ��������statemenet�ķ�ʽ�ǲ��ᷢ��SQLע�룬���Ծ�����statemenet��ʽ
*/

#ifdef _WIN32
#	ifdef __BUILD_BASE_DATABASE_DLL__
#		define __BASE_DATABASE_API__ __declspec(dllexport)
#	else
#		define __BASE_DATABASE_API__ __declspec(dllimport)
#	endif
#else
#	define __BASE_DATABASE_API__
#endif

namespace base
{
	enum EDBErrorType
	{
		eDBET_Success				= 0,
		eDBET_LostConnection		= 1,	// ���Ӷ�ʧ
		eDBET_NoPrivileges			= 1044, // ���ݿ��û�Ȩ�޲���
		eDBET_NoDiskSpace			= 1021, // Ӳ��ʣ��ռ䲻��
		eDBET_KeyDupForUpdate		= 1022, // �ؼ����ظ������ļ�¼ʧ��
		eDBET_NoRecord				= 1032, // ��¼������
		eDBET_NoMemory				= 1037, // ϵͳ�ڴ治��
		eDBET_OutofConnection		= 1040, // �ѵ������ݿ���������������Ӵ����ݿ����������
		eDBET_SQLSyntax				= 1149, // SQL����﷨����
		eDBET_KeyDupForInsert		= 1062,	// �ؼ����ظ��������¼ʧ��
		eDBET_CommitTrans			= 1180, // �ύ����ʧ��
		eDBET_RollbackTrans			= 1181, // �ع�����ʧ��
		eDBET_Deadloop				= 1205, // ������ʱ
		eDBET_StatementReprepared	= 1615,	// Statement��Ҫ����׼��

		eDBET_Unknwon = INVALID_32BIT
	};

	class IDbConnection;
	class IDbFacade
	{
	public:
		virtual IDbConnection*	createConnection() = 0;
		virtual uint32_t		getConnectionCount() const = 0;
		virtual void			release() = 0;

	protected:
		virtual ~IDbFacade() { }
	};

	class IDbStatement;
	class IDbRecordset;
	class IDbConnection
	{
	public:
		virtual bool			connect(const char* szHost, uint16_t nPort, const char* szUser, const char* szPassword, const char* szDbname, const char* szCharacterSet) = 0;
		virtual bool			isConnect() const = 0;
		virtual void			close() = 0;
		virtual void			release() = 0;
		virtual uint32_t		getStatementCount() const = 0;
		virtual uint32_t		getRecordsetCount() const = 0;
		virtual IDbStatement*	createStatement(const char* szSQL) = 0;
		virtual IDbRecordset*	execute(const char* szSql) = 0;
		virtual bool			ping() = 0;
		virtual uint64_t		getAffectedRow() const = 0;
		virtual void*			getMysql() const = 0;
		virtual void			escape(char* szDst, const char* szSrc, size_t nLength) = 0;

	protected:
		virtual ~IDbConnection() { };
	};

	class IDbParamBinder;
	class IDbResultBinder;
	class IDbStatement
	{
	public:
		virtual uint32_t			execute() = 0;
		virtual IDbParamBinder*		getParamBinder() = 0;
		virtual IDbResultBinder*	getResultBinder() = 0;
		virtual uint32_t			getParamCount() const = 0;
		virtual uint32_t			getResultCount() const = 0;
		virtual uint64_t			getRowCount() const = 0;
		virtual int64_t				getInsertID() const = 0;
		virtual bool				fatchNextRow() = 0;
		virtual bool				locate(uint64_t nRow) = 0;
		virtual void				release() = 0;

	protected:
		virtual ~IDbStatement() { }
	};

	class IDbParamBinder
	{
	public:
		virtual void setInt64(uint32_t nIndex, int64_t& nValue) = 0;
		virtual void setUInt64(uint32_t nIndex, uint64_t& nValue) = 0;
		virtual void setInt32(uint32_t nIndex, int32_t& nValue) = 0;
		virtual void setUInt32(uint32_t nIndex, uint32_t& nValue) = 0;
		virtual void setInt16(uint32_t nIndex, int16_t& nValue) = 0;
		virtual void setUInt16(uint32_t nIndex, uint16_t& nValue) = 0;
		virtual void setInt8(uint32_t nIndex, int8_t& nValue) = 0;
		virtual void setUInt8(uint32_t nIndex, uint8_t& nValue) = 0;
		virtual void setFloat(uint32_t nIndex, float& fValue) = 0;
		virtual void setDouble(uint32_t nIndex, double& fValue) = 0;
		virtual void setBlob(uint32_t nIndex, void* pBuf, uint32_t nBufSize) = 0;
		// nBufSize �������ַ����ĳ��ȶ����ǻ���ĳ���
		virtual void setText(uint32_t nIndex, void* pBuf, uint32_t nBufSize) = 0;

		virtual void bind() = 0;

	protected:
		virtual ~IDbParamBinder() { }
	};

	class IDbResultBinder
	{
	public:
		virtual void bindInt64(uint32_t nIndex, int64_t& nValue, bool& bIsNull, bool& bError) = 0;
		virtual void bindUInt64(uint32_t nIndex, uint64_t& nValue, bool& bIsNull, bool& bError) = 0;
		virtual void bindInt32(uint32_t nIndex, int32_t& nValue, bool& bIsNull, bool& bError) = 0;
		virtual void bindUInt32(uint32_t nIndex, uint32_t& nValue, bool& bIsNull, bool& bError) = 0;
		virtual void bindInt16(uint32_t nIndex, int16_t& nValue, bool& bIsNull, bool& bError) = 0;
		virtual void bindUInt16(uint32_t nIndex, uint16_t& nValue, bool& bIsNull, bool& bError) = 0;
		virtual void bindInt8(uint32_t nIndex, int8_t& nValue, bool& bIsNull, bool& bError) = 0;
		virtual void bindUInt8(uint32_t nIndex, uint8_t& nValue, bool& bIsNull, bool& bError) = 0;
		virtual void bindFloat(uint32_t nIndex, float& fValue, bool& bIsNull, bool& bError) = 0;
		virtual void bindDouble(uint32_t nIndex, double& fValue, bool& bIsNull, bool& bError) = 0;
		virtual void bindBlob(uint32_t nIndex, void* pBuf, uint32_t nBufSize, _ulong& nResultSize, bool& bIsNull, bool& bError) = 0;
		virtual void bindText(uint32_t nIndex, void* pBuf, uint32_t nBufSize, _ulong& nResultSize, bool& bIsNull, bool& bError) = 0;

		virtual void bind() = 0;

	protected:
		virtual ~IDbResultBinder() { }
	};

	class IDbRecordset
	{
	public:
		virtual uint64_t	getRowCount() const = 0;
		virtual uint32_t	getColCount() const = 0;
		virtual bool        fatchNextRow() = 0;
		virtual bool        locate(uint64_t nRow) = 0;
		virtual CVariant	getData(uint32_t nCol) const = 0;
		virtual void        release() = 0;

	protected:
		virtual ~IDbRecordset() { }
	};

	__BASE_DATABASE_API__ void			startupDatabase();
	__BASE_DATABASE_API__ void			cleanupDatabase();

	__BASE_DATABASE_API__ IDbFacade*	createDbFacade();
}