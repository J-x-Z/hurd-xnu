#ifndef	_fs_server_
#define	_fs_server_

/* Module fs */

#include <string.h>
#include <mach/ndr.h>
#include <mach/boolean.h>
#include <mach/kern_return.h>
#include <mach/notify.h>
#include <mach/mach_types.h>
#include <mach/message.h>
#include <mach/mig_errors.h>
#include <mach/port.h>
	
/* BEGIN VOUCHER CODE */

#ifndef KERNEL
#if defined(__has_include)
#if __has_include(<mach/mig_voucher_support.h>)
#ifndef USING_VOUCHERS
#define USING_VOUCHERS
#endif
#ifndef __VOUCHER_FORWARD_TYPE_DECLS__
#define __VOUCHER_FORWARD_TYPE_DECLS__
#ifdef __cplusplus
extern "C" {
#endif
#ifndef __VOUCHER_FOWARD_TYPE_DECLS_SINGLE_ATTR
#define __VOUCHER_FOWARD_TYPE_DECLS_SINGLE_ATTR __unsafe_indexable
#endif
	extern boolean_t voucher_mach_msg_set(mach_msg_header_t * msg) __attribute__((weak_import));
#ifdef __cplusplus
}
#endif
#endif // __VOUCHER_FORWARD_TYPE_DECLS__
#endif // __has_include(<mach/mach_voucher_types.h>)
#endif // __has_include
#endif // !KERNEL
	
/* END VOUCHER CODE */

	
/* BEGIN MIG_STRNCPY_ZEROFILL CODE */

#if defined(__has_include)
#if __has_include(<mach/mig_strncpy_zerofill_support.h>)
#ifndef USING_MIG_STRNCPY_ZEROFILL
#define USING_MIG_STRNCPY_ZEROFILL
#endif
#ifndef __MIG_STRNCPY_ZEROFILL_FORWARD_TYPE_DECLS__
#define __MIG_STRNCPY_ZEROFILL_FORWARD_TYPE_DECLS__
#ifdef __cplusplus
extern "C" {
#endif
#ifndef __MIG_STRNCPY_ZEROFILL_FORWARD_TYPE_DECLS_CSTRING_ATTR
#define __MIG_STRNCPY_ZEROFILL_FORWARD_TYPE_DECLS_CSTRING_COUNTEDBY_ATTR(C) __unsafe_indexable
#endif
	extern int mig_strncpy_zerofill(char * dest, const char * src, int len) __attribute__((weak_import));
#ifdef __cplusplus
}
#endif
#endif /* __MIG_STRNCPY_ZEROFILL_FORWARD_TYPE_DECLS__ */
#endif /* __has_include(<mach/mig_strncpy_zerofill_support.h>) */
#endif /* __has_include */
	
/* END MIG_STRNCPY_ZEROFILL CODE */


#ifdef AUTOTEST
#ifndef FUNCTION_PTR_T
#define FUNCTION_PTR_T
typedef void (*function_ptr_t)(mach_port_t, char *, mach_msg_type_number_t);
typedef struct {
        char            * name;
        function_ptr_t  function;
} function_table_entry;
typedef function_table_entry   *function_table_t;
#endif /* FUNCTION_PTR_T */
#endif /* AUTOTEST */

#ifndef	fs_MSG_COUNT
#define	fs_MSG_COUNT	5
#endif	/* fs_MSG_COUNT */

#include <Availability.h>
#include <mach/std_types.h>
#include <mach/mig.h>
#include <mach/mig.h>
#include <mach/mach_types.h>
#include <sys/types.h>
#include <mach/mach_types.h>

#ifdef __BeforeMigServerHeader
__BeforeMigServerHeader
#endif /* __BeforeMigServerHeader */

#ifndef MIG_SERVER_ROUTINE
#define MIG_SERVER_ROUTINE
#endif


/* Routine file_check_access */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t file_check_access
(
	file_t file,
	int *allowed
);

/* Routine file_statfs */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t file_statfs
(
	file_t file,
	fsys_statfsbuf_t *info
);

/* Routine file_sync */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t file_sync
(
	file_t file,
	int wait,
	int omit_metadata
);

/* Routine file_chmod */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t file_chmod
(
	file_t chmod_file,
	mode_t new_mode
);

/* Routine file_get_translator */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t file_get_translator
(
	file_t file,
	data_t *translator,
	mach_msg_type_number_t *translatorCnt
);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
boolean_t fs_server(
		mach_msg_header_t *InHeadP,
		mach_msg_header_t *OutHeadP);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
mig_routine_t fs_server_routine(
		mach_msg_header_t *InHeadP);


/* Description of this subsystem, for use in direct RPC */
extern const struct fs_subsystem {
	mig_server_routine_t	server;	/* Server routine */
	mach_msg_id_t	start;	/* Min routine number */
	mach_msg_id_t	end;	/* Max routine number + 1 */
	unsigned int	maxsize;	/* Max msg size */
	vm_address_t	reserved;	/* Reserved */
	struct routine_descriptor	/* Array of routine descriptors */
		routine[5];
} fs_subsystem;

/* typedefs for all requests */

#ifndef __Request__fs_subsystem__defined
#define __Request__fs_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request__file_check_access_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request__file_statfs_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		int wait;
		int omit_metadata;
	} __Request__file_sync_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mode_t new_mode;
	} __Request__file_chmod_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request__file_get_translator_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__fs_subsystem__defined */


/* union of all requests */

#ifndef __RequestUnion__fs_subsystem__defined
#define __RequestUnion__fs_subsystem__defined
union __RequestUnion__fs_subsystem {
	__Request__file_check_access_t Request_file_check_access;
	__Request__file_statfs_t Request_file_statfs;
	__Request__file_sync_t Request_file_sync;
	__Request__file_chmod_t Request_file_chmod;
	__Request__file_get_translator_t Request_file_get_translator;
};
#endif /* __RequestUnion__fs_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__fs_subsystem__defined
#define __Reply__fs_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int allowed;
	} __Reply__file_check_access_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		fsys_statfsbuf_t info;
	} __Reply__file_statfs_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply__file_sync_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply__file_chmod_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t translator;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t translatorCnt;
	} __Reply__file_get_translator_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__fs_subsystem__defined */


/* union of all replies */

#ifndef __ReplyUnion__fs_subsystem__defined
#define __ReplyUnion__fs_subsystem__defined
union __ReplyUnion__fs_subsystem {
	__Reply__file_check_access_t Reply_file_check_access;
	__Reply__file_statfs_t Reply_file_statfs;
	__Reply__file_sync_t Reply_file_sync;
	__Reply__file_chmod_t Reply_file_chmod;
	__Reply__file_get_translator_t Reply_file_get_translator;
};
#endif /* __ReplyUnion__fs_subsystem__defined */

#ifndef subsystem_to_name_map_fs
#define subsystem_to_name_map_fs \
    { "file_check_access", 20000 },\
    { "file_statfs", 20001 },\
    { "file_sync", 20002 },\
    { "file_chmod", 20003 },\
    { "file_get_translator", 20004 }
#endif

#ifdef __AfterMigServerHeader
__AfterMigServerHeader
#endif /* __AfterMigServerHeader */

#endif	 /* _fs_server_ */
