#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

/* Function pointers to hw3 functions */
void* (*mm_malloc)(size_t);
void* (*mm_realloc)(void*, size_t);
void (*mm_free)(void*);
void (*mm_info)();
void load_alloc_functions() {
    void *handle = dlopen("hw3lib.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    char* error;
    mm_malloc = dlsym(handle, "mm_malloc");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    mm_realloc = dlsym(handle, "mm_realloc");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    mm_free = dlsym(handle, "mm_free");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
    mm_info = dlsym(handle, "mm_info");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
}

int main() {
    load_alloc_functions();

    int *data = (int *)mm_malloc(sizeof(int));   

    // void *p[100];
    // int j = 0;
    // for (int i = 0; i < 100; i += 4,j++)
    // {
    //     p[j] = mm_malloc(i);
    // }
    
    // mm_free(p[14]);
    // mm_free(p[16]);
    // mm_free(p[18]);
    // mm_free(p[15]);
    // mm_free(p[17]);
    // mm_info();
    // strcpy(p[14],"This is a test");
    // char *np = mm_realloc(p[14], 4);
    // printf("%s\n", np);}
    void *t;
    for (int i = 0; i < 10; ++i)
    {
        t = malloc(i);
        printf("%p\n",t);
    }
    assert(data != NULL);

    //data[0] = 0x162;
    mm_free(data);
    printf("malloc test successful!\n");
    return 0;
}
