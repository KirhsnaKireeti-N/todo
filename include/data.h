#define TASK_IMPORTANT 1 << 0
#define TASK_URGENT 1 << 1

struct task {
    char strtask[256];
    unsigned char metadata;
};

int load(const char *file, struct task **todolist);
void task_add(const char *file, char *strtask, int strtasklen, unsigned char metadata);
void task_remove(const char *file, int i);
void save(const char *file, struct task *ptodolist, int n);
