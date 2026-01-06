#ifndef	_fs_user_
#define	_fs_user_

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
#define	fs_MSG_COUNT	17
#endif	/* fs_MSG_COUNT */

#include <Availability.h>
#include <mach/std_types.h>
#include <mach/mig.h>
#include <mach/mig.h>
#include <mach/mach_types.h>

#ifdef __BeforeMigUserHeader
__BeforeMigUserHeader
#endif /* __BeforeMigUserHeader */

#include <sys/cdefs.h>
__BEGIN_DECLS


/* Routine dir_lookup */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t dir_lookup
(
	file_t dir,
	data_t name,
	mach_msg_type_number_t nameCnt,
	int flags,
	mode_t mode,
	int *do_retry,
	data_t *retry_name,
	mach_msg_type_number_t *retry_nameCnt,
	mach_port_t *file
);

/* Routine dir_readdir */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t dir_readdir
(
	file_t dir,
	data_t *data,
	mach_msg_type_number_t *dataCnt,
	int entry,
	int nentries,
	int bufsiz,
	int *amount
);

/* Routine dir_mkdir */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t dir_mkdir
(
	file_t dir,
	data_t name,
	mach_msg_type_number_t nameCnt,
	mode_t mode
);

/* Routine dir_rmdir */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t dir_rmdir
(
	file_t dir,
	data_t name,
	mach_msg_type_number_t nameCnt
);

/* Routine dir_unlink */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t dir_unlink
(
	file_t dir,
	data_t name,
	mach_msg_type_number_t nameCnt
);

/* Routine dir_link */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t dir_link
(
	file_t dir,
	file_t source,
	data_t name,
	mach_msg_type_number_t nameCnt,
	int excl
);

/* Routine dir_rename */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t dir_rename
(
	file_t olddir,
	data_t oldname,
	mach_msg_type_number_t oldnameCnt,
	file_t newdir,
	data_t newname,
	mach_msg_type_number_t newnameCnt,
	int excl
);

/* Routine dir_mkfile */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t dir_mkfile
(
	file_t dir,
	int flags,
	mode_t mode,
	mach_port_t *file
);

/* Routine file_getstat */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t file_getstat
(
	file_t file,
	data_t *stat_data,
	mach_msg_type_number_t *stat_dataCnt
);

/* Routine file_setstat */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t file_setstat
(
	file_t file,
	data_t stat_data,
	mach_msg_type_number_t stat_dataCnt,
	int stat_flags
);

/* Routine file_sync */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t file_sync
(
	file_t file,
	int wait,
	int omit_metadata
);

/* Routine file_set_size */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t file_set_size
(
	file_t file,
	off_t size
);

/* Routine io_read */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t io_read
(
	io_t io,
	data_t *data,
	mach_msg_type_number_t *dataCnt,
	off_t offset,
	int amount
);

/* Routine io_write */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t io_write
(
	io_t io,
	data_t data,
	mach_msg_type_number_t dataCnt,
	off_t offset,
	int *amount
);

__END_DECLS

/********************** Caution **************************/
/* The following data types should be used to calculate  */
/* maximum message sizes only. The actual message may be */
/* smaller, and the position of the arguments within the */
/* message layout may vary from what is presented here.  */
/* For example, if any of the arguments are variable-    */
/* sized, and less than the maximum is sent, the data    */
/* will be packed tight in the actual message to reduce  */
/* the presence of holes.                                */
/********************** Caution **************************/

/* typedefs for all requests */

#ifndef __Request__fs_subsystem__defined
#define __Request__fs_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int flags;
		mode_t mode;
	} __Request__dir_lookup_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		int entry;
		int nentries;
		int bufsiz;
	} __Request__dir_readdir_t __attribute__((unused));
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
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		mode_t mode;
	} __Request__dir_mkdir_t __attribute__((unused));
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
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
	} __Request__dir_rmdir_t __attribute__((unused));
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
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
	} __Request__dir_unlink_t __attribute__((unused));
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
		mach_msg_port_descriptor_t source;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int excl;
	} __Request__dir_link_t __attribute__((unused));
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
		mach_msg_ool_descriptor_t oldname;
		mach_msg_port_descriptor_t newdir;
		mach_msg_ool_descriptor_t newname;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t oldnameCnt;
		mach_msg_type_number_t newnameCnt;
		int excl;
	} __Request__dir_rename_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		int flags;
		mode_t mode;
	} __Request__dir_mkfile_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request__file_getstat_t __attribute__((unused));
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
		mach_msg_ool_descriptor_t stat_data;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t stat_dataCnt;
		int stat_flags;
	} __Request__file_setstat_t __attribute__((unused));
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
		off_t size;
	} __Request__file_set_size_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		off_t offset;
		int amount;
	} __Request__io_read_t __attribute__((unused));
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
		mach_msg_ool_descriptor_t data;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t dataCnt;
		off_t offset;
	} __Request__io_write_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__fs_subsystem__defined */

/* union of all requests */

#ifndef __RequestUnion__fs_subsystem__defined
#define __RequestUnion__fs_subsystem__defined
union __RequestUnion__fs_subsystem {
	__Request__dir_lookup_t Request_dir_lookup;
	__Request__dir_readdir_t Request_dir_readdir;
	__Request__dir_mkdir_t Request_dir_mkdir;
	__Request__dir_rmdir_t Request_dir_rmdir;
	__Request__dir_unlink_t Request_dir_unlink;
	__Request__dir_link_t Request_dir_link;
	__Request__dir_rename_t Request_dir_rename;
	__Request__dir_mkfile_t Request_dir_mkfile;
	__Request__file_getstat_t Request_file_getstat;
	__Request__file_setstat_t Request_file_setstat;
	__Request__file_sync_t Request_file_sync;
	__Request__file_set_size_t Request_file_set_size;
	__Request__io_read_t Request_io_read;
	__Request__io_write_t Request_io_write;
};
#endif /* !__RequestUnion__fs_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__fs_subsystem__defined
#define __Reply__fs_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t retry_name;
		mach_msg_port_descriptor_t file;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int do_retry;
		mach_msg_type_number_t retry_nameCnt;
	} __Reply__dir_lookup_t __attribute__((unused));
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
		mach_msg_ool_descriptor_t data;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t dataCnt;
		int amount;
	} __Reply__dir_readdir_t __attribute__((unused));
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
	} __Reply__dir_mkdir_t __attribute__((unused));
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
	} __Reply__dir_rmdir_t __attribute__((unused));
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
	} __Reply__dir_unlink_t __attribute__((unused));
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
	} __Reply__dir_link_t __attribute__((unused));
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
	} __Reply__dir_rename_t __attribute__((unused));
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
		mach_msg_port_descriptor_t file;
		/* end of the kernel processed data */
	} __Reply__dir_mkfile_t __attribute__((unused));
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
		mach_msg_ool_descriptor_t stat_data;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t stat_dataCnt;
	} __Reply__file_getstat_t __attribute__((unused));
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
	} __Reply__file_setstat_t __attribute__((unused));
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
	} __Reply__file_set_size_t __attribute__((unused));
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
		mach_msg_ool_descriptor_t data;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t dataCnt;
	} __Reply__io_read_t __attribute__((unused));
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
		int amount;
	} __Reply__io_write_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__fs_subsystem__defined */

/* union of all replies */

#ifndef __ReplyUnion__fs_subsystem__defined
#define __ReplyUnion__fs_subsystem__defined
union __ReplyUnion__fs_subsystem {
	__Reply__dir_lookup_t Reply_dir_lookup;
	__Reply__dir_readdir_t Reply_dir_readdir;
	__Reply__dir_mkdir_t Reply_dir_mkdir;
	__Reply__dir_rmdir_t Reply_dir_rmdir;
	__Reply__dir_unlink_t Reply_dir_unlink;
	__Reply__dir_link_t Reply_dir_link;
	__Reply__dir_rename_t Reply_dir_rename;
	__Reply__dir_mkfile_t Reply_dir_mkfile;
	__Reply__file_getstat_t Reply_file_getstat;
	__Reply__file_setstat_t Reply_file_setstat;
	__Reply__file_sync_t Reply_file_sync;
	__Reply__file_set_size_t Reply_file_set_size;
	__Reply__io_read_t Reply_io_read;
	__Reply__io_write_t Reply_io_write;
};
#endif /* !__RequestUnion__fs_subsystem__defined */

#ifndef subsystem_to_name_map_fs
#define subsystem_to_name_map_fs \
    { "dir_lookup", 22000 },\
    { "dir_readdir", 22001 },\
    { "dir_mkdir", 22002 },\
    { "dir_rmdir", 22003 },\
    { "dir_unlink", 22004 },\
    { "dir_link", 22005 },\
    { "dir_rename", 22006 },\
    { "dir_mkfile", 22007 },\
    { "file_getstat", 22008 },\
    { "file_setstat", 22009 },\
    { "file_sync", 22010 },\
    { "file_set_size", 22011 },\
    { "io_read", 22015 },\
    { "io_write", 22016 }
#endif

#ifdef __AfterMigUserHeader
__AfterMigUserHeader
#endif /* __AfterMigUserHeader */

#endif	 /* _fs_user_ */
