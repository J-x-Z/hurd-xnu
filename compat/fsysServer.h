#ifndef	_fsys_server_
#define	_fsys_server_

/* Module fsys */

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

#ifndef	fsys_MSG_COUNT
#define	fsys_MSG_COUNT	3
#endif	/* fsys_MSG_COUNT */

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


/* Routine fsys_getroot */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t fsys_getroot
(
	fsys_t fsys,
	mach_port_t dotdot,
	idarray_t gen_uids,
	mach_msg_type_number_t gen_uidsCnt,
	idarray_t gen_gids,
	mach_msg_type_number_t gen_gidsCnt,
	int flags,
	retry_type *do_retry,
	string_t retry_name,
	mach_port_t *result,
	mach_msg_type_name_t *resultPoly
);

/* Routine fsys_goaway */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t fsys_goaway
(
	fsys_t fsys,
	int flags
);

/* Routine fsys_syncfs */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t fsys_syncfs
(
	fsys_t fsys,
	int wait,
	int do_children
);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
boolean_t fsys_server(
		mach_msg_header_t *InHeadP,
		mach_msg_header_t *OutHeadP);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
mig_routine_t fsys_server_routine(
		mach_msg_header_t *InHeadP);


/* Description of this subsystem, for use in direct RPC */
extern const struct fsys_subsystem {
	mig_server_routine_t	server;	/* Server routine */
	mach_msg_id_t	start;	/* Min routine number */
	mach_msg_id_t	end;	/* Max routine number + 1 */
	unsigned int	maxsize;	/* Max msg size */
	vm_address_t	reserved;	/* Reserved */
	struct routine_descriptor	/* Array of routine descriptors */
		routine[3];
} fsys_subsystem;

/* typedefs for all requests */

#ifndef __Request__fsys_subsystem__defined
#define __Request__fsys_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_port_descriptor_t dotdot;
		mach_msg_ool_descriptor_t gen_uids;
		mach_msg_ool_descriptor_t gen_gids;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t gen_uidsCnt;
		mach_msg_type_number_t gen_gidsCnt;
		int flags;
	} __Request__fsys_getroot_t __attribute__((unused));
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
	} __Request__fsys_goaway_t __attribute__((unused));
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
		int do_children;
	} __Request__fsys_syncfs_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__fsys_subsystem__defined */


/* union of all requests */

#ifndef __RequestUnion__fsys_subsystem__defined
#define __RequestUnion__fsys_subsystem__defined
union __RequestUnion__fsys_subsystem {
	__Request__fsys_getroot_t Request_fsys_getroot;
	__Request__fsys_goaway_t Request_fsys_goaway;
	__Request__fsys_syncfs_t Request_fsys_syncfs;
};
#endif /* __RequestUnion__fsys_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__fsys_subsystem__defined
#define __Reply__fsys_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_port_descriptor_t result;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		retry_type do_retry;
		string_t retry_name;
	} __Reply__fsys_getroot_t __attribute__((unused));
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
	} __Reply__fsys_goaway_t __attribute__((unused));
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
	} __Reply__fsys_syncfs_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__fsys_subsystem__defined */


/* union of all replies */

#ifndef __ReplyUnion__fsys_subsystem__defined
#define __ReplyUnion__fsys_subsystem__defined
union __ReplyUnion__fsys_subsystem {
	__Reply__fsys_getroot_t Reply_fsys_getroot;
	__Reply__fsys_goaway_t Reply_fsys_goaway;
	__Reply__fsys_syncfs_t Reply_fsys_syncfs;
};
#endif /* __ReplyUnion__fsys_subsystem__defined */

#ifndef subsystem_to_name_map_fsys
#define subsystem_to_name_map_fsys \
    { "fsys_getroot", 22000 },\
    { "fsys_goaway", 22001 },\
    { "fsys_syncfs", 22002 }
#endif

#ifdef __AfterMigServerHeader
__AfterMigServerHeader
#endif /* __AfterMigServerHeader */

#endif	 /* _fsys_server_ */
