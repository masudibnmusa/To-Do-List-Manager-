#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TASKS 100
#define MAX_DESC_LENGTH 256
#define FILENAME "todo_list.dat"

typedef struct {
    char description[MAX_DESC_LENGTH];
    int completed;
    int id;
} Task;

Task tasks[MAX_TASKS];
int taskCount = 0;
int nextId = 1;

// Function prototypes
void displayMenu();
void addTask();
void viewTasks();
void markTaskComplete();
void deleteTask();
void saveToFile();
void loadFromFile();
void clearInputBuffer();
int getValidatedIntegerInput();

int main() {
    loadFromFile();
    
    printf("=== To-Do List Manager ===\n");
    
    while (1) {
        displayMenu();
        int choice = getValidatedIntegerInput();
        
        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewTasks();
                break;
            case 3:
                markTaskComplete();
                break;
            case 4:
                deleteTask();
                break;
            case 5:
                saveToFile();
                printf("Tasks saved successfully. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}

void displayMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Add Task\n");
    printf("2. View Tasks\n");
    printf("3. Mark Task as Complete\n");
    printf("4. Delete Task\n");
    printf("5. Save and Exit\n");
    printf("Enter your choice (1-5): ");
}

void addTask() {
    if (taskCount >= MAX_TASKS) {
        printf("Error: Task list is full! Maximum %d tasks allowed.\n", MAX_TASKS);
        return;
    }
    
    printf("Enter task description: ");
    
    char description[MAX_DESC_LENGTH];
    clearInputBuffer();
    if (fgets(description, MAX_DESC_LENGTH, stdin) == NULL) {
        printf("Error reading input!\n");
        return;
    }
    
    // Remove newline character if present
    description[strcspn(description, "\n")] = 0;
    
    // Validate that description is not empty
    if (strlen(description) == 0) {
        printf("Error: Task description cannot be empty!\n");
        return;
    }
    
    // Add task to array
    Task newTask;
    strcpy(newTask.description, description);
    newTask.completed = 0;
    newTask.id = nextId++;
    
    tasks[taskCount] = newTask;
    taskCount++;
    
    printf("Task added successfully with ID: %d\n", newTask.id);
}

void viewTasks() {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    
    printf("\n--- Your Tasks ---\n");
    for (int i = 0; i < taskCount; i++) {
        printf("ID: %d | %s | %s\n", 
               tasks[i].id,
               tasks[i].completed ? "[✓]" : "[ ]",
               tasks[i].description);
    }
    
    // Display statistics
    int completedCount = 0;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].completed) {
            completedCount++;
        }
    }
    printf("\nTotal: %d tasks | Completed: %d | Pending: %d\n", 
           taskCount, completedCount, taskCount - completedCount);
}

void markTaskComplete() {
    if (taskCount == 0) {
        printf("No tasks available to mark as complete.\n");
        return;
    }
    
    viewTasks();
    printf("\nEnter the ID of the task to mark as complete: ");
    int id = getValidatedIntegerInput();
    
    int found = 0;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            if (tasks[i].completed) {
                printf("Task is already marked as complete!\n");
            } else {
                tasks[i].completed = 1;
                printf("Task marked as complete!\n");
            }
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Error: Task with ID %d not found!\n", id);
    }
}

void deleteTask() {
    if (taskCount == 0) {
        printf("No tasks available to delete.\n");
        return;
    }
    
    viewTasks();
    printf("\nEnter the ID of the task to delete: ");
    int id = getValidatedIntegerInput();
    
    int foundIndex = -1;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex == -1) {
        printf("Error: Task with ID %d not found!\n", id);
        return;
    }
    
    // Confirm deletion
    printf("Are you sure you want to delete this task? (y/n): ");
    char confirm;
    clearInputBuffer();
    scanf("%c", &confirm);
    
    if (tolower(confirm) == 'y') {
        // Shift all tasks after the deleted one
        for (int i = foundIndex; i < taskCount - 1; i++) {
            tasks[i] = tasks[i + 1];
        }
        taskCount--;
        printf("Task deleted successfully!\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}

void saveToFile() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error: Could not save tasks to file!\n");
        return;
    }
    
    // Save nextId and taskCount first
    fwrite(&nextId, sizeof(int), 1, file);
    fwrite(&taskCount, sizeof(int), 1, file);
    
    // Save all tasks
    for (int i = 0; i < taskCount; i++) {
        fwrite(&tasks[i], sizeof(Task), 1, file);
    }
    
    fclose(file);
}

void loadFromFile() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("No existing task file found. Starting fresh.\n");
        return;
    }
    
    // Load nextId and taskCount
    fread(&nextId, sizeof(int), 1, file);
    fread(&taskCount, sizeof(int), 1, file);
    
    // Load all tasks
    for (int i = 0; i < taskCount; i++) {
        fread(&tasks[i], sizeof(Task), 1, file);
    }
    
    fclose(file);
    printf("Loaded %d tasks from file.\n", taskCount);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidatedIntegerInput() {
    int input;
    while (scanf("%d", &input) != 1) {
        printf("Invalid input! Please enter a number: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    return input;
}
