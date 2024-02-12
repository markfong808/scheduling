/**
 * Driver.c
 *
 * Schedule is in the format
 *
 *  [name] [priority] [CPU burst]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"

#define SIZE 100

int main(int argc, char *argv[])
{
    FILE *in;
    char *temp;
    char task[SIZE];

    char *name;
    int priority;
    int burst;

    in = fopen(argv[1], "r");

    while (fgets(task, SIZE, in) != NULL) //read a line of text from a file
    {
        temp = strdup(task);        // duplicate a string
        name = strsep(&temp, ",");  //extract token that ends with ","
        priority = atoi(strsep(&temp, ",")); // converts the extracted token (which is a string representing an integer) to an integer using atoi()
        burst = atoi(strsep(&temp, ",")); 

        // add the task to the scheduler's list of tasks
        add(name, priority, burst);

        free(temp);
    }

    fclose(in);

    // invoke the scheduler
    schedule();

    return 0;
}