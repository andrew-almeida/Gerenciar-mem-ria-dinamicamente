#include <stdio.h>
#include <stdlib.h>

#define MAX_BLOCKS 100

typedef struct {
    void *ptr;
    int *ref_count;
} MemoryBlock;

MemoryBlock *block_list[MAX_BLOCKS];
int block_count = 0;

void *malloc2(size_t tamanho) {
    if (block_count >= MAX_BLOCKS) {
        printf("Erro: Limite de blocos alocados atingido\n");
        return NULL;
    }
    MemoryBlock *block = (MemoryBlock *)malloc(sizeof(MemoryBlock));
    if (block == NULL) {
        return NULL;
    }
    block->ptr = malloc(tamanho);
    if (block->ptr == NULL) {
        free(block);
        return NULL;
    }
    block->ref_count = (int *)malloc(sizeof(int));
    if (block->ref_count == NULL) {
        free(block->ptr);
        free(block);
        return NULL;
    }
    *(block->ref_count) = 1;
    block_list[block_count++] = block;
    return block;
}

void atrib2(void **endereco1, void *endereco2) {
    if (*endereco1 != NULL) {
        MemoryBlock *block1 = (MemoryBlock *)(*endereco1);
        if (--(*(block1->ref_count)) == 0) {
            free(block1->ptr);
            free(block1->ref_count);
            free(block1);
        }
    }
    *endereco1 = endereco2;
    if (*endereco1 != NULL) {
        MemoryBlock *block2 = (MemoryBlock *)(*endereco1);
        (*(block2->ref_count))++;
    }
}

void dump() {
    printf("\n--- Estado atual dos blocos alocados ---\n");
    for (int i = 0; i < block_count; i++) {
        if (block_list[i] != NULL) {
            printf("Bloco %d: Endereco = %p, RefCount = %d\n", i, block_list[i]->ptr, *(block_list[i]->ref_count));
        }
    }
    printf("----------------------------------------\n\n");
}

int main(int argc, char const *argv[]) {
    MemoryBlock *v = malloc2(sizeof(int));
    *((int *)(v->ptr)) = 10;

    MemoryBlock *w = malloc2(sizeof(int));
    *((int *)(w->ptr)) = 20;

    dump();

    atrib2((void **)&v, w);

    dump();

    MemoryBlock *c = malloc2(sizeof(char));
    *((char *)(c->ptr)) = 'Z';

    dump();

    atrib2((void **)&w, NULL);

    dump();

    return 0;
}
