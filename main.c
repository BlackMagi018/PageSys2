#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PCB {
    int PID;
    char **text;
    char **data;
    int text_size;
    int data_size;
} PCB;

struct freeframe {
    struct page * head;
    struct page * tail;
};

typedef struct page {
    char data[50];
    struct page * prev;
    struct page * next;
} page;

char *print_results(struct page **table) {

}

int main(int argc, char **argv) {

    FILE *tape = fopen(argv[1], "r");

    //Free Fame Linked List
    struct freeframe list;
    //Token pointer
    char *token;
    //Argument Counter
    int count = 0;

    //Creating List of PCB's
    struct PCB ** block_list = (struct PCB **) malloc(10 * sizeof(struct PCB *));
    for (int i = 0; i < 8; i++) {
        block_list[i] = malloc(sizeof(struct PCB));
    }

    //Creating Page Table
    struct page **table = (struct page **) malloc(8 * sizeof(struct page *));
    for (int i = 0; i < 8; i++) {
        struct page * frame = malloc(sizeof(struct page));
        table[i] = frame;
        sprintf(table[i]->data, "%d", i);
        if (i > 0) {
            table[i]->prev = table[i - 1];
            table[i - 1]->next = table[i];
        }
    }

    //Create Command String array
    char **parts = (char **) malloc(3 * sizeof(char *));
    for (int i = 0; i < 8; i++) {
        parts[i] = (char *) malloc(10 * sizeof(char));
    }

    //align free fame list
    list.head = table[0];
    list.tail = table[7];

    char input[100];
    printf("Click Enter to start the Simulation");
    while (getchar() && !feof(tape)) {
        fgets(input, sizeof(input), tape);
        token = strtok(input, " ");
        parts[count] = token;
        while (token != NULL) {
            count++;
            token = strtok(NULL, " ");
            parts[count] = token;
        }
        switch (count) {
            case 2:
                printf("Process %s terminating\n", parts[0]);
                break;
            case 3:
                printf("-- NEW PROCESS ARRIVING --\n");
                printf("   Process ID: %s\n", parts[0]);
                printf("   Text Size - %s\n", parts[1]);
                printf("   Data Size - %s", parts[2]);
                int ProcID = (int) strtol(parts[0],NULL,10);
                int TextSize = (int) strtol(parts[1],NULL,10);
                int DataSize = (int) strtol(parts[2],NULL,10);
                block_list[ProcID]->PID = ProcID;
                block_list[ProcID]->text_size = TextSize;
                block_list[ProcID]->data_size = DataSize;
                for(int i = 0; i <)
                break;
            default:
                break;
        }
        count = 0;
    }
    printf("Simulation Complete");
}