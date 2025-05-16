/*
 * Comprehensive Examples of Linux Memory Management System Calls
 *
 * This file demonstrates the usage of the following memory management system calls:
 * 1. mmap()
 * 2. munmap()
 * 3. mprotect()
 * 4. brk()/sbrk()
 * 5. madvise()
 * 6. mlock()/munlock()
 *
 * Compile with:
 *     gcc -o memory_syscalls memory_syscalls.c
 * Run:
 *     ./memory_syscalls
 */

#include <stdio.h>      // For printf, perror
#include <stdlib.h>     // For exit codes
#include <string.h>     // For strcpy
#include <unistd.h>     // For getpagesize, sbrk
#include <sys/mman.h>   // For mmap, munmap, mprotect, madvise, mlock, munlock

/*
 * Example 1: mmap and munmap - Allocate and deallocate memory using mmap.
 */
void example_mmap() {
    printf("\n--- Example 1: mmap and munmap ---\n");
    size_t size = getpagesize();
    char *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap");
        return;
    }

    strcpy(ptr, "Hello from mmap!");
    printf("Memory content: %s\n", ptr);

    if (munmap(ptr, size) == -1) {
        perror("munmap");
    }
}

/*
 * Example 2: mprotect - Change memory protection.
 */
void example_mprotect() {
    printf("\n--- Example 2: mprotect ---\n");
    size_t size = getpagesize();
    char *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap");
        return;
    }

    strcpy(ptr, "Testing mprotect");
    printf("Before mprotect: %s\n", ptr);

    if (mprotect(ptr, size, PROT_READ) == -1) {
        perror("mprotect");
        munmap(ptr, size);
        return;
    }

    printf("After mprotect: still readable: %s\n", ptr);

    munmap(ptr, size);
}

/*
 * Example 3: sbrk - Change the end of the data segment.
 */
void example_sbrk() {
    printf("\n--- Example 3: sbrk ---\n");
    void *start = sbrk(0);

    if (sbrk(100) == (void *) -1) {
        perror("sbrk");
        return;
    }

    void *end = sbrk(0);
    printf("sbrk moved break from %p to %p\n", start, end);

    strcpy((char *)start, "Memory via sbrk");
    printf("Content: %s\n", (char *)start);

    sbrk(-100); // Clean-up (not always safe)
}

/*
 * Example 4: madvise - Provide advice to the kernel about memory usage.
 */
void example_madvise() {
    printf("\n--- Example 4: madvise ---\n");
    size_t size = getpagesize();
    void *ptr = mmap(NULL, size * 4, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap");
        return;
    }

    if (madvise(ptr, size * 4, MADV_RANDOM) == -1) {
        perror("madvise");
    } else {
        printf("madvise applied: MADV_RANDOM\n");
    }

    munmap(ptr, size * 4);
}

/*
 * Example 5: mlock and munlock - Lock and unlock memory pages.
 */
void example_mlock() {
    printf("\n--- Example 5: mlock and munlock ---\n");
    size_t size = getpagesize();
    void *ptr = malloc(size);

    if (!ptr) {
        perror("malloc");
        return;
    }

    if (mlock(ptr, size) == -1) {
        perror("mlock");
    } else {
        printf("Memory locked successfully\n");
    }

    strcpy((char *)ptr, "Locked memory");
    printf("Content: %s\n", (char *)ptr);

    if (munlock(ptr, size) == -1) {
        perror("munlock");
    } else {
        printf("Memory unlocked successfully\n");
    }

    free(ptr);
}

int main() {
    example_mmap();
    example_mprotect();
    example_sbrk();
    example_madvise();
    example_mlock();
    return 0;
}
