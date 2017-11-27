/* 簡單實現 malloc()、calloc()、free() 和 realloc()。
 * 代碼源自張洋博客:
 * http://blog.codinglabs.org/articles/a-malloc-tutorial.html
 * 是基於 X86_64 體系結構，操作系統爲 Linux 下編寫的。
 * 代碼經過本人修改和封裝後，兼容 c++ 語法。
 * 对于真实世界的malloc实现，可以参考 glibc 的实现:
 * http://repo.or.cz/w/glibc.git/blob/HEAD:/malloc/malloc.c
 * ByXc
 */

#ifndef MALLOC_B_
#define MALLOC_B_

/* 由於存在虛擬的 data 字段和字節對齊，sizeof　不能正確計算 meta 長度，這裏手工設置 */
#define BLOCK_SIZE 40

typedef struct s_block * t_block;
struct s_block
{
    size_t size;    // 數據區的大小
    t_block prev;   // 指向上個塊的指針
    t_block next;   // 指向下個塊的指針
    int free;       // 是否是空閒塊
    int padding;    // 填充 4 字節，保證 meta 塊長度爲 8 的倍數
    void * ptr;     // Magic pointer, 指向 data
    char data[1];   // 這是一個虛擬字段，表示數據塊的第一個字節，長度不應計入 meta
};

void * malloc_b(size_t size);
void * calloc_b(size_t number, size_t size);
void free_b(void * p);
void * realloc_b(void * p, size_t size);

#endif
