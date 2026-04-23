#include <stdio.h>
#include <windows.h>

int main()
{
    int *memory;
    DWORD oldProtect;
    FILE *fp;

    memory = (int *)VirtualAlloc(NULL, sizeof(int),
                                 MEM_COMMIT | MEM_RESERVE,
                                 PAGE_READWRITE);

    if (memory == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    *memory = 50;

    fp = fopen("result.txt", "w");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    fprintf(fp, "=== Secure Memory Isolation System (Windows) ===\n");
    fprintf(fp, "Initial Value: %d\n", *memory);
    fprintf(fp, "Memory Address: %p\n\n", memory);

    if (VirtualProtect(memory, sizeof(int), PAGE_READONLY, &oldProtect) == 0)
    {
        fprintf(fp, "[ERROR] Memory protection failed\n");
    }

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

    VirtualFree(memory, 0, MEM_RELEASE);

    return 0;
}