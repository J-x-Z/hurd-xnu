/*
 * mach_vm_test.c
 * 
 * Tests XNU's VM and memory object capabilities.
 * Hurd relies heavily on external memory objects (EMOs) for:
 *   - File system data (ext2fs pager)
 *   - Device memory mapping
 *   - Copy-on-write semantics
 * 
 * This test verifies basic memory allocation/mapping via Mach VM.
 */

#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <stdio.h>
#include <string.h>

int main() {
    kern_return_t kr;
    mach_vm_address_t addr = 0;
    mach_vm_size_t size = 4096; // One page

    printf("=== XNU Mach VM Verification ===\n");
    printf("Testing memory allocation and protection.\n\n");

    // 1. Allocate anonymous memory
    kr = mach_vm_allocate(mach_task_self(), &addr, size, VM_FLAGS_ANYWHERE);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] mach_vm_allocate: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Allocated %llu bytes at 0x%llx\n", size, addr);

    // 2. Write to the memory
    char *ptr = (char *)addr;
    strcpy(ptr, "Hello from XNU Mach VM!");
    printf("[OK] Written data: \"%s\"\n", ptr);

    // 3. Change protection to read-only
    kr = mach_vm_protect(mach_task_self(), addr, size, FALSE, VM_PROT_READ);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] mach_vm_protect (read-only): %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Changed protection to READ-ONLY\n");

    // 4. Verify we can still read
    printf("[OK] Read back: \"%s\"\n", ptr);

    // 5. Change protection back to read-write
    kr = mach_vm_protect(mach_task_self(), addr, size, FALSE, VM_PROT_READ | VM_PROT_WRITE);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] mach_vm_protect (rw): %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Changed protection to READ-WRITE\n");

    // 6. Deallocate
    kr = mach_vm_deallocate(mach_task_self(), addr, size);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] mach_vm_deallocate: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Deallocated memory\n");

    // 7. Test vm_copy (important for COW semantics)
    mach_vm_address_t src_addr = 0, dst_addr = 0;
    kr = mach_vm_allocate(mach_task_self(), &src_addr, size, VM_FLAGS_ANYWHERE);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] src allocate: %s\n", mach_error_string(kr));
        return 1;
    }
    kr = mach_vm_allocate(mach_task_self(), &dst_addr, size, VM_FLAGS_ANYWHERE);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] dst allocate: %s\n", mach_error_string(kr));
        return 1;
    }

    char *src_ptr = (char *)src_addr;
    char *dst_ptr = (char *)dst_addr;
    strcpy(src_ptr, "COW Test Data");

    kr = mach_vm_copy(mach_task_self(), src_addr, size, dst_addr);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] mach_vm_copy: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] mach_vm_copy succeeded\n");
    printf("     Source: \"%s\"\n", src_ptr);
    printf("     Dest:   \"%s\"\n", dst_ptr);

    if (strcmp(src_ptr, dst_ptr) == 0) {
        printf("[OK] Data matches after copy\n");
    } else {
        printf("[ERROR] Data mismatch!\n");
        return 1;
    }

    // Cleanup
    mach_vm_deallocate(mach_task_self(), src_addr, size);
    mach_vm_deallocate(mach_task_self(), dst_addr, size);

    printf("\n=== Results ===\n");
    printf("SUCCESS: Basic Mach VM operations work on XNU!\n");
    printf("This confirms memory object fundamentals are present.\n");

    return 0;
}
