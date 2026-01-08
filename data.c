#include "data.h"
#include "csv.h"
#include <string.h>

static struct task *todolist;

static struct data {
    int n, taskstart;
} gdata = {.n = 0, .taskstart = 1};

static void cb_field(void *input, size_t len, void *data) {
    auto fdata = (struct data *)data;

    if (fdata->taskstart) {
        strcpy(todolist[fdata->n].strtask, (char *)input);
        fdata->taskstart = 0;
    }
    if (!fdata->taskstart)
        todolist[fdata->n].metadata = atoi((char *)input);
}

static void cb_row(int c, void *data) {
    auto fdata = (struct data *)data;

    fdata->n++;
    fdata->taskstart = 1;
}

int load(const char *file, struct task **ftodolist) {
    struct csv_parser parser;
    if (csv_init(&parser, CSV_STRICT | CSV_APPEND_NULL) == -1) {
        fprintf(stderr, "Failed to initialize csv_parser");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(file, "r");
    if (fp == nullptr) {
        fp = fopen(file, "w");
    }
    char buffer[1024];
    size_t bytes_read, n = 8;
    todolist = calloc(n, sizeof(struct task));
    while ((bytes_read = fread(buffer, 1, 1024, fp)) > 0) {
        if (csv_parse(&parser, buffer, bytes_read, cb_field, cb_row, &gdata) != bytes_read) {
            fprintf(stderr, "Error while parsing todo file: %s\n%s", file, csv_strerror(csv_error(&parser)));
        }
        if (gdata.n == n) {
            todolist = realloc(todolist, (n *= 2) * sizeof(struct task));
        }
    }

    csv_fini(&parser, cb_field, cb_row, &gdata);

    csv_free(&parser);
    fclose(fp);

    *ftodolist = todolist;

    return gdata.n;
}

void task_add(const char *file, char *strtask, int strtasklen, unsigned char metadata) {
    FILE *fp = fopen(file, "a");

    csv_fwrite(fp, strtask, strtasklen - 1);
    char str[5];
    sprintf(str, ",%02x\n", metadata);
    fputs(str, fp);
    fclose(fp);
}

void task_remove(const char *file, int i) {
    struct task *todolist;
    int n = load(file, &todolist);

    memmove(todolist + i - 1, todolist + i, ((n - 1) - i + 1) * (sizeof(struct task)));

    save(file, todolist, n - 1);
}

void save(const char *file, struct task *ftodolist, int n) {
    FILE *fp = fopen(file, "w");

    for (int i = 0; i < n; ++i) {
        csv_fwrite(fp, ftodolist[i].strtask, strlen(ftodolist[i].strtask));
        char str[5];
        sprintf(str, ",%02x\n", ftodolist[i].metadata);
        fputs(str, fp);
    }

    fclose(fp);
}
