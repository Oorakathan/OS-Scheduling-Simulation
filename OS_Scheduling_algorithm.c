#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_TASKS 10
#define MAX_RESOURCES 5

typedef struct {
    int id;
    char name[20];
    int cpu;
    int memory;
    int io;
    bool completed;
} Task;

typedef struct {
    int total_cpu;
    int total_memory;
    int total_io;
    int available_cpu;
    int available_memory;
    int available_io;
} ResourceManager;

Task taskQueue[MAX_TASKS];
int taskCount = 0;
ResourceManager resourceManager = {100, 200, 50, 100, 200, 50};

Task predefinedTasks[] = {
    {1, "Image Processing", 30, 60, 15, false},
    {2, "Data Analysis", 25, 50, 10, false},
    {3, "Video Rendering", 40, 80, 20, false},
    {4, "Machine Learning", 50, 100, 30, false},
    {5, "File Compression", 20, 40, 10, false},
    {6, "Database Query", 15, 30, 5, false}
};

int predefinedTaskCount = sizeof(predefinedTasks) / sizeof(predefinedTasks[0]);

void displayResources() {
    printf("-------------------------------------------------------------------\n");
    printf("\nAvailable Resources: CPU=%d, Memory=%d, IO=%d\n", resourceManager.available_cpu, resourceManager.available_memory, resourceManager.available_io);
}

void addTask(int taskIndex) {
    if (taskCount >= MAX_TASKS) {
        printf("Task queue full!\n");
        return;
    }
    Task newTask = predefinedTasks[taskIndex];
    if (newTask.cpu > resourceManager.available_cpu || newTask.memory > resourceManager.available_memory || newTask.io > resourceManager.available_io) {
        printf("-----------> Not enough resources! Schedule tasks first. <-----------\n");
        return;
    }
    resourceManager.available_cpu -= newTask.cpu;
    resourceManager.available_memory -= newTask.memory;
    resourceManager.available_io -= newTask.io;
    taskQueue[taskCount++] = newTask;
    printf("Added task: [%d] %s (CPU: %d, Memory: %d, IO: %d)\n", newTask.id, newTask.name, newTask.cpu, newTask.memory, newTask.io);
}

char* determineSchedulingAlgorithm() {
    int totalCpuRequirement = 0;
    for (int i = 0; i < taskCount; i++) {
        totalCpuRequirement += taskQueue[i].cpu;
    }
    printf("\n|---------------------------------------------------------|\n");
    if (totalCpuRequirement > (resourceManager.total_cpu / 1.5)) {
        printf("| Using Shortest Job First (SJF) Scheduling Algorithm     |\n|---------------------------------------------------------|\n");
        return "SJF";
    } else {
        printf("| Using First Come First Serve (FCFS) Scheduling Algorithm|\n|---------------------------------------------------------|\n");
        return "FCFS";
    }
}

void sortTasksBySJF() {
    for (int i = 0; i < taskCount - 1; i++) {
        for (int j = 0; j < taskCount - i - 1; j++) {
            if (taskQueue[j].cpu > taskQueue[j + 1].cpu) {
                Task temp = taskQueue[j];
                taskQueue[j] = taskQueue[j + 1];
                taskQueue[j + 1] = temp;
            }
        }
    }
}

void scheduleTasks() {
    if (taskCount == 0) {
        printf("No tasks to schedule.\n");
        return;
    }
    char* schedulingAlgorithm = determineSchedulingAlgorithm();
    if (schedulingAlgorithm[0] == 'S') {
        sortTasksBySJF();
    }
    printf("\nScheduled Tasks:\n");
    printf("----------------\n");
    for (int i = 0; i < taskCount; i++) {
        printf("Task ID: [%d] Name: %s (CPU: %d, Memory: %d, IO: %d)\n",
               taskQueue[i].id, taskQueue[i].name, taskQueue[i].cpu, taskQueue[i].memory, taskQueue[i].io);
    }
    printf("****************************************************************\n");
    for (int i = 0; i < taskCount; i++) {
        if (!taskQueue[i].completed) {
            printf("Executing Task [%d]: %s...\n", taskQueue[i].id, taskQueue[i].name);
            resourceManager.available_cpu += taskQueue[i].cpu;
            resourceManager.available_memory += taskQueue[i].memory;
            resourceManager.available_io += taskQueue[i].io;
            taskQueue[i].completed = true;
            printf("Task [%d] %s completed. Resources released.\n", taskQueue[i].id, taskQueue[i].name);
        }
    }
    taskCount = 0;
}

int main() {
    int choice;
    while (1) {
        displayResources();
        printf("\nSelect a predefined task to add (1-%d) or 0 to schedule, -1 to exit:\n\n", predefinedTaskCount);
        for (int i = 0; i < predefinedTaskCount; i++) {
            printf("%d: %s (CPU: %d, Memory: %d, IO: %d)\n", i + 1, predefinedTasks[i].name, predefinedTasks[i].cpu, predefinedTasks[i].memory, predefinedTasks[i].io);
        }
        scanf("%d", &choice);
        if (choice == -1) {
            printf("Exiting...\n");
            exit(0);
        } else if (choice == 0) {
            scheduleTasks();
        } else if (choice > 0 && choice <= predefinedTaskCount) {
            addTask(choice - 1);
        } else {
            printf("Invalid choice!\n");
        }
    }
    return 0;
}
