#ifndef	_auth_server_
#define	_auth_server_

/* Module auth */

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

#ifndef	auth_MSG_COUNT
#define	auth_MSG_COUNT	4
#endif	/* auth_MSG_COUNT */

#include <Availability.h>
#include <mach/std_types.h>
#include <mach/mig.h>
#include <mach/mig.h>
#include <mach/mach_types.h>

#ifdef __BeforeMigServerHeader
__BeforeMigServerHeader
#endif /* __BeforeMigServerHeader */

#ifndef MIG_SERVER_ROUTINE
#define MIG_SERVER_ROUTINE
#endif


/* Routine auth_getids */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t auth_getids
(
	auth_t handle,
	idarray_t *euids,
	mach_msg_type_number_t *euidsCnt,
	idarray_t *auids,
	mach_msg_type_number_t *auidsCnt,
	idarray_t *egids,
	mach_msg_type_number_t *egidsCnt,
	idarray_t *agids,
	mach_msg_type_number_t *agidsCnt
);

/* Routine auth_makeauth */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t auth_makeauth
(
	auth_t handle,
	portarray_t other_handles,
	mach_msg_type_number_t other_handlesCnt,
	idarray_t euids,
	mach_msg_type_number_t euidsCnt,
	idarray_t auids,
	mach_msg_type_number_t auidsCnt,
	idarray_t egids,
	mach_msg_type_number_t egidsCnt,
	idarray_t agids,
	mach_msg_type_number_t agidsCnt,
	mach_port_t *newhandle
);

/* Routine auth_user_authenticate */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t auth_user_authenticate
(
	auth_t handle,
	mach_port_t rendezvous,
	mach_port_t *newport
);

/* Routine auth_server_authenticate */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t auth_server_authenticate
(
	auth_t handle,
	mach_port_t rendezvous,
	mach_port_t newport,
	idarray_t *euids,
	mach_msg_type_number_t *euidsCnt,
	idarray_t *auids,
	mach_msg_type_number_t *auidsCnt,
	idarray_t *egids,
	mach_msg_type_number_t *egidsCnt,
	idarray_t *agids,
	mach_msg_type_number_t *agidsCnt
);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
boolean_t auth_server(
		mach_msg_header_t *InHeadP,
		mach_msg_header_t *OutHeadP);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
mig_routine_t auth_server_routine(
		mach_msg_header_t *InHeadP);


/* Description of this subsystem, for use in direct RPC */
extern const struct auth_subsystem {
	mig_server_routine_t	server;	/* Server routine */
	mach_msg_id_t	start;	/* Min routine number */
	mach_msg_id_t	end;	/* Max routine number + 1 */
	unsigned int	maxsize;	/* Max msg size */
	vm_address_t	reserved;	/* Reserved */
	struct routine_descriptor	/* Array of routine descriptors */
		routine[4];
} auth_subsystem;

/* typedefs for all requests */

#ifndef __Request__auth_subsystem__defined
#define __Request__auth_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request__auth_getids_t __attribute__((unused));
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
		mach_msg_ool_ports_descriptor_t other_handles;
		mach_msg_ool_descriptor_t euids;
		mach_msg_ool_descriptor_t auids;
		mach_msg_ool_descriptor_t egids;
		mach_msg_ool_descriptor_t agids;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t other_handlesCnt;
		mach_msg_type_number_t euidsCnt;
		mach_msg_type_number_t auidsCnt;
		mach_msg_type_number_t egidsCnt;
		mach_msg_type_number_t agidsCnt;
	} __Request__auth_makeauth_t __attribute__((unused));
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
		mach_msg_port_descriptor_t rendezvous;
		/* end of the kernel processed data */
	} __Request__auth_user_authenticate_t __attribute__((unused));
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
		mach_msg_port_descriptor_t rendezvous;
		mach_msg_port_descriptor_t newport;
		/* end of the kernel processed data */
	} __Request__auth_server_authenticate_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__auth_subsystem__defined */


/* union of all requests */

#ifndef __RequestUnion__auth_subsystem__defined
#define __RequestUnion__auth_subsystem__defined
union __RequestUnion__auth_subsystem {
	__Request__auth_getids_t Request_auth_getids;
	__Request__auth_makeauth_t Request_auth_makeauth;
	__Request__auth_user_authenticate_t Request_auth_user_authenticate;
	__Request__auth_server_authenticate_t Request_auth_server_authenticate;
};
#endif /* __RequestUnion__auth_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__auth_subsystem__defined
#define __Reply__auth_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t euids;
		mach_msg_ool_descriptor_t auids;
		mach_msg_ool_descriptor_t egids;
		mach_msg_ool_descriptor_t agids;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t euidsCnt;
		mach_msg_type_number_t auidsCnt;
		mach_msg_type_number_t egidsCnt;
		mach_msg_type_number_t agidsCnt;
	} __Reply__auth_getids_t __attribute__((unused));
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
		mach_msg_port_descriptor_t newhandle;
		/* end of the kernel processed data */
	} __Reply__auth_makeauth_t __attribute__((unused));
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
	} __Reply__auth_user_authenticate_t __attribute__((unused));
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
		mach_msg_ool_descriptor_t euids;
		mach_msg_ool_descriptor_t auids;
		mach_msg_ool_descriptor_t egids;
		mach_msg_ool_descriptor_t agids;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t euidsCnt;
		mach_msg_type_number_t auidsCnt;
		mach_msg_type_number_t egidsCnt;
		mach_msg_type_number_t agidsCnt;
	} __Reply__auth_server_authenticate_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__auth_subsystem__defined */


/* union of all replies */

#ifndef __ReplyUnion__auth_subsystem__defined
#define __ReplyUnion__auth_subsystem__defined
union __ReplyUnion__auth_subsystem {
	__Reply__auth_getids_t Reply_auth_getids;
	__Reply__auth_makeauth_t Reply_auth_makeauth;
	__Reply__auth_user_authenticate_t Reply_auth_user_authenticate;
	__Reply__auth_server_authenticate_t Reply_auth_server_authenticate;
};
#endif /* __ReplyUnion__auth_subsystem__defined */

#ifndef subsystem_to_name_map_auth
#define subsystem_to_name_map_auth \
    { "auth_getids", 25000 },\
    { "auth_makeauth", 25001 },\
    { "auth_user_authenticate", 25002 },\
    { "auth_server_authenticate", 25003 }
#endif

#ifdef __AfterMigServerHeader
__AfterMigServerHeader
#endif /* __AfterMigServerHeader */

#endif	 /* _auth_server_ */
