#ifndef	_io_user_
#define	_io_user_

/* Module io */

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

#ifndef	io_MSG_COUNT
#define	io_MSG_COUNT	5
#endif	/* io_MSG_COUNT */

#include <Availability.h>
#include <mach/std_types.h>
#include <mach/mig.h>
#include <mach/mig.h>
#include <mach/mach_types.h>
#include <sys/types.h>
#include <mach/mach_types.h>

#ifdef __BeforeMigUserHeader
__BeforeMigUserHeader
#endif /* __BeforeMigUserHeader */

#include <sys/cdefs.h>
__BEGIN_DECLS


/* Routine io_read */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t io_read
(
	io_t io_object,
	data_t *data,
	mach_msg_type_number_t *dataCnt,
	loff_t offset,
	vm_size_t amount
);

/* Routine io_write */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t io_write
(
	io_t io_object,
	data_t data,
	mach_msg_type_number_t dataCnt,
	loff_t offset,
	vm_size_t *amount
);

/* Routine io_seek */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t io_seek
(
	io_t io_object,
	loff_t offset,
	int whence,
	loff_t *newp
);

/* Routine io_readable */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t io_readable
(
	io_t io_object,
	vm_size_t *amount
);

/* Routine io_duplicate */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t io_duplicate
(
	io_t io_object,
	mach_port_t *newport
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

#ifndef __Request__io_subsystem__defined
#define __Request__io_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		loff_t offset;
		vm_size_t amount;
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
		loff_t offset;
	} __Request__io_write_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		loff_t offset;
		int whence;
	} __Request__io_seek_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request__io_readable_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request__io_duplicate_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__io_subsystem__defined */

/* union of all requests */

#ifndef __RequestUnion__io_subsystem__defined
#define __RequestUnion__io_subsystem__defined
union __RequestUnion__io_subsystem {
	__Request__io_read_t Request_io_read;
	__Request__io_write_t Request_io_write;
	__Request__io_seek_t Request_io_seek;
	__Request__io_readable_t Request_io_readable;
	__Request__io_duplicate_t Request_io_duplicate;
};
#endif /* !__RequestUnion__io_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__io_subsystem__defined
#define __Reply__io_subsystem__defined

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
		vm_size_t amount;
	} __Reply__io_write_t __attribute__((unused));
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
		loff_t newp;
	} __Reply__io_seek_t __attribute__((unused));
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
		vm_size_t amount;
	} __Reply__io_readable_t __attribute__((unused));
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
		mach_msg_port_descriptor_t newport;
		/* end of the kernel processed data */
	} __Reply__io_duplicate_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__io_subsystem__defined */

/* union of all replies */

#ifndef __ReplyUnion__io_subsystem__defined
#define __ReplyUnion__io_subsystem__defined
union __ReplyUnion__io_subsystem {
	__Reply__io_read_t Reply_io_read;
	__Reply__io_write_t Reply_io_write;
	__Reply__io_seek_t Reply_io_seek;
	__Reply__io_readable_t Reply_io_readable;
	__Reply__io_duplicate_t Reply_io_duplicate;
};
#endif /* !__RequestUnion__io_subsystem__defined */

#ifndef subsystem_to_name_map_io
#define subsystem_to_name_map_io \
    { "io_read", 21000 },\
    { "io_write", 21001 },\
    { "io_seek", 21002 },\
    { "io_readable", 21003 },\
    { "io_duplicate", 21004 }
#endif

#ifdef __AfterMigUserHeader
__AfterMigUserHeader
#endif /* __AfterMigUserHeader */

#endif	 /* _io_user_ */
