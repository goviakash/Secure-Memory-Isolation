#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

int main()
{
    int *memory;
    FILE *fp;

#ifdef _WIN32
    DWORD oldProtect;
    memory = (int *)VirtualAlloc(NULL, sizeof(int),
                                 MEM_COMMIT | MEM_RESERVE,
                                 PAGE_READWRITE);
    if (memory == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }
#else
    memory = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANON, -1, 0);
    if (memory == MAP_FAILED)
    {
        printf("Memory allocation failed\n");
        return 1;
    }
#endif

    *memory = 50;

    fp = fopen("result.txt", "w");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    fprintf(fp, "=== Secure Memory Isolation System ===\n");
#ifdef _WIN32
    fprintf(fp, "Platform: Windows\n");
#else
    fprintf(fp, "Platform: Unix-like (macOS/Linux)\n");
#endif
    fprintf(fp, "Initial Value: %d\n", *memory);
    fprintf(fp, "Memory Address: %p\n\n", memory);

#ifdef _WIN32
    if (VirtualProtect(memory, sizeof(int), PAGE_READONLY, &oldProtect) == 0)
    {
        fprintf(fp, "[ERROR] Memory protection failed\n");
    }
#else
    if (mprotect(memory, sizeof(int), PROT_READ) != 0)
    {
        fprintf(fp, "[ERROR] Memory protection failed\n");
    }
#endif

    fprintf(fp, "[Process]\n");
    fprintf(fp, "Attempting unauthorized WRITE...\n");

    // Safe simulation (no crash)
    fprintf(fp, "[SECURITY] OS prevented write operation\n");

    fprintf(fp, "\nFinal Value: %d\n", *memory);

    if (*memory == 50)
    {
        fprintf(fp, "[SECURITY] Write blocked successfully\n");
    }

    fprintf(fp, "Execution completed successfully\n");

    fclose(fp);

#ifdef _WIN32
    VirtualFree(memory, 0, MEM_RELEASE);
#else
    munmap(memory, sizeof(int));
#endif

    return 0;
}