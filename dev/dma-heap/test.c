#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#define DMA_HEAP_IOCTL_ALLOC 0xc0184800
typedef unsigned long long u64;
typedef unsigned int u32;
struct dma_heap_allocation_data {
  u64 len;
  u32 fd;
  u32 fd_flags;
  u64 heap_flags;
};

void fatal(const char *msg) {
    perror(msg);
    exit(1);
}

int main(void) {
    // Open DMA-BUF
    int dma_fd = open("/dev/dma_heap/system", O_RDWR);
    if (dma_fd == -1)
      fatal("/dev/dma_heap/system");

    // Allocate DMA-BUF heap
    int dma_buf_fd = -1;
    struct dma_heap_allocation_data data;
    data.len = 0x400000;
    data.fd_flags = O_RDWR;
    data.heap_flags = 0;
    data.fd = 0;
    if (ioctl(dma_fd, DMA_HEAP_IOCTL_ALLOC, &data) < 0)
      fatal("DMA_HEAP_IOCTL_ALLOC");
    printf("[+] dma_buf_fd: %d\n", dma_buf_fd = data.fd);
    close(dma_fd);

    // wait
    puts("Press enter to mmap/write");
    scanf("%*[^\n]"); scanf("%*c");

    // Mmap DMA-BUF heap
    void *dma_buf = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE, dma_buf_fd, 0);
    printf("[+] dma_buf: %p\n", dma_buf);

    // write to DMA-BUF heap
    memcpy(dma_buf, "AAAABBBBCCCCDDDD", 16);

    // wait
    puts("Press enter to close");
    scanf("%*[^\n]"); scanf("%*c");

    // close
    close(dma_buf_fd);

    return 0;
}
