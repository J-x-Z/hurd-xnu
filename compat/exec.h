#ifndef	_exec_user_
#define	_exec_user_

/* Module exec */

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

#ifndef	exec_MSG_COUNT
#define	exec_MSG_COUNT	4
#endif	/* exec_MSG_COUNT */

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


/* Routine exec_exec */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t exec_exec
(
	exec_t execserver,
	file_t file,
	mach_port_t oldtask,
	int flags,
	data_t argv,
	mach_msg_type_number_t argvCnt,
	data_t envp,
	mach_msg_type_number_t envpCnt,
	portarray_t dtable,
	mach_msg_type_number_t dtableCnt,
	portarray_t portarray,
	mach_msg_type_number_t portarrayCnt,
	intarray_t intarray,
	mach_msg_type_number_t intarrayCnt,
	portarray_t deallocnames,
	mach_msg_type_number_t deallocnamesCnt,
	portarray_t destroynames,
	mach_msg_type_number_t destroynamesCnt
);

/* Routine exec_setexecdata */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t exec_setexecdata
(
	exec_t execserver,
	portarray_t ports,
	mach_msg_type_number_t portsCnt,
	intarray_t ints,
	mach_msg_type_number_t intsCnt
);

/* Routine exec_exec_file */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t exec_exec_file
(
	exec_t execserver,
	file_t file,
	mach_port_t task,
	int flags,
	data_t filename,
	mach_msg_type_number_t filenameCnt
);

/* Routine exec_startup_get_info */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t exec_startup_get_info
(
	mach_port_t bootstrap,
	int *user_entry,
	int *phdr_data,
	int *phdr_size,
	int *base_addr,
	int *stack_base,
	int *stack_size,
	int *flags,
	data_t *argv,
	mach_msg_type_number_t *argvCnt,
	data_t *envp,
	mach_msg_type_number_t *envpCnt,
	portarray_t *dtable,
	mach_msg_type_number_t *dtableCnt,
	portarray_t *portarray,
	mach_msg_type_number_t *portarrayCnt,
	intarray_t *intarray,
	mach_msg_type_number_t *intarrayCnt
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

#ifndef __Request__exec_subsystem__defined
#define __Request__exec_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_port_descriptor_t file;
		mach_msg_port_descriptor_t oldtask;
		mach_msg_ool_descriptor_t argv;
		mach_msg_ool_descriptor_t envp;
		mach_msg_ool_ports_descriptor_t dtable;
		mach_msg_ool_ports_descriptor_t portarray;
		mach_msg_ool_descriptor_t intarray;
		mach_msg_ool_ports_descriptor_t deallocnames;
		mach_msg_ool_ports_descriptor_t destroynames;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int flags;
		mach_msg_type_number_t argvCnt;
		mach_msg_type_number_t envpCnt;
		mach_msg_type_number_t dtableCnt;
		mach_msg_type_number_t portarrayCnt;
		mach_msg_type_number_t intarrayCnt;
		mach_msg_type_number_t deallocnamesCnt;
		mach_msg_type_number_t destroynamesCnt;
	} __Request__exec_exec_t __attribute__((unused));
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
		mach_msg_ool_ports_descriptor_t ports;
		mach_msg_ool_descriptor_t ints;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t portsCnt;
		mach_msg_type_number_t intsCnt;
	} __Request__exec_setexecdata_t __attribute__((unused));
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
		mach_msg_port_descriptor_t task;
		mach_msg_ool_descriptor_t filename;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int flags;
		mach_msg_type_number_t filenameCnt;
	} __Request__exec_exec_file_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request__exec_startup_get_info_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__exec_subsystem__defined */

/* union of all requests */

#ifndef __RequestUnion__exec_subsystem__defined
#define __RequestUnion__exec_subsystem__defined
union __RequestUnion__exec_subsystem {
	__Request__exec_exec_t Request_exec_exec;
	__Request__exec_setexecdata_t Request_exec_setexecdata;
	__Request__exec_exec_file_t Request_exec_exec_file;
	__Request__exec_startup_get_info_t Request_exec_startup_get_info;
};
#endif /* !__RequestUnion__exec_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__exec_subsystem__defined
#define __Reply__exec_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply__exec_exec_t __attribute__((unused));
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
	} __Reply__exec_setexecdata_t __attribute__((unused));
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
	} __Reply__exec_exec_file_t __attribute__((unused));
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
		mach_msg_ool_descriptor_t argv;
		mach_msg_ool_descriptor_t envp;
		mach_msg_ool_ports_descriptor_t dtable;
		mach_msg_ool_ports_descriptor_t portarray;
		mach_msg_ool_descriptor_t intarray;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int user_entry;
		int phdr_data;
		int phdr_size;
		int base_addr;
		int stack_base;
		int stack_size;
		int flags;
		mach_msg_type_number_t argvCnt;
		mach_msg_type_number_t envpCnt;
		mach_msg_type_number_t dtableCnt;
		mach_msg_type_number_t portarrayCnt;
		mach_msg_type_number_t intarrayCnt;
	} __Reply__exec_startup_get_info_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__exec_subsystem__defined */

/* union of all replies */

#ifndef __ReplyUnion__exec_subsystem__defined
#define __ReplyUnion__exec_subsystem__defined
union __ReplyUnion__exec_subsystem {
	__Reply__exec_exec_t Reply_exec_exec;
	__Reply__exec_setexecdata_t Reply_exec_setexecdata;
	__Reply__exec_exec_file_t Reply_exec_exec_file;
	__Reply__exec_startup_get_info_t Reply_exec_startup_get_info;
};
#endif /* !__RequestUnion__exec_subsystem__defined */

#ifndef subsystem_to_name_map_exec
#define subsystem_to_name_map_exec \
    { "exec_exec", 26000 },\
    { "exec_setexecdata", 26001 },\
    { "exec_exec_file", 26002 },\
    { "exec_startup_get_info", 26003 }
#endif

#ifdef __AfterMigUserHeader
__AfterMigUserHeader
#endif /* __AfterMigUserHeader */

#endif	 /* _exec_user_ */
