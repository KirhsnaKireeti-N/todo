#include "ansi_color.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *todofp = "todo.csv";
int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("%s <command> [arguments]\n", argv[0]);
        printf("\t<list(l)>\t[-]\n\t<add(a)>\t[task]\n\t<done(d)>\t[taskID]\n\t<clear(c)>\n");

        return 0;
    }
    if (!strcmp(argv[1], "list") || !strcmp(argv[1], "l")) {
        struct task *todolist;
        int n = load(todofp, &todolist);

        for (int i = 0; i < n; ++i) {
            auto color = ANSI_COLOR_WHITE;
            auto colorcode = todolist[i].metadata;

            color = colorcode & TASK_IMPORTANT ? ANSI_COLOR_GREEN : color;
            color = colorcode & TASK_URGENT ? ANSI_COLOR_RED : color;
            color = (colorcode & TASK_IMPORTANT) && (colorcode & TASK_URGENT) ? ANSI_COLOR_RED_UL : color;
            printf("%s%d: %s" ANSI_COLOR_RESET "\n", color, i + 1, todolist[i].strtask);
        }
    }
    if (!strcmp(argv[1], "add") || !strcmp(argv[1], "a")) {
        // input : strtask
        char task[256];
        int index = 0, len;
        for (int i = 2; i < argc; ++i) {
            len = strlen(argv[i]);
            strncpy(task + index, argv[i], len);
            index += len;
            task[index++] = ' ';
        }

        // input : metadata
        unsigned char metadata = 0;
        char c;

        printf("Important[y]? ");
        scanf("%c", &c);
        while (fgetc(stdin) != '\n')
            ;
        metadata |= (c == 'y' ? TASK_IMPORTANT : metadata);

        printf("Urgent[y]? ");
        scanf("%c", &c);
        while (fgetc(stdin) != '\n')
            ;
        metadata |= (c == 'y' ? TASK_URGENT : metadata);

        task_add(todofp, task, index, metadata);
        return 0;
    }
    if (!strcmp(argv[1], "done") || !strcmp(argv[1], "d")) {
        task_remove(todofp, atoi(argv[2]));
    }
    if (!strcmp(argv[1], "clear") || !strcmp(argv[1], "c")) {
        FILE *f = fopen(todofp, "w");
        fclose(f);
    }
}
