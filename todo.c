#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TODOS 100
#define MAX_LENGTH 100

struct Todo
{
    char description[MAX_LENGTH];
    int completed;
    int priority;
};

// Globální proměnné
struct Todo todos[MAX_TODOS];
int todoCount = 0;

// Funkce pro přidání nového úkolu
void addTodo()
{
    if (todoCount >= MAX_TODOS)
    {
        printf("Seznam je plný!\n");
        return;
    }

    printf("Zadejte popis úkolu: ");
    getchar(); // Vyčistí buffer
    fgets(todos[todoCount].description, MAX_LENGTH, stdin);
    todos[todoCount].description[strcspn(todos[todoCount].description, "\n")] = 0; // Odstraní znak nového řádku

    printf("Zadejte prioritu (1-3): ");
    scanf("%d", &todos[todoCount].priority);

    todos[todoCount].completed = 0;
    todoCount++;

    printf("Úkol byl přidán!\n");
}

// Funkce pro zobrazení všech úkolů
void listTodos()
{
    if (todoCount == 0)
    {
        printf("Seznam je prázdný!\n");
        return;
    }

    printf("\n--- Seznam úkolů ---\n");
    for (int i = 0; i < todoCount; i++)
    {
        printf("%d. [%c] (%d) %s\n",
               i + 1,
               todos[i].completed ? 'X' : ' ',
               todos[i].priority,
               todos[i].description);
    }
}

void markCompleted()
{
    int index;
    listTodos();
    printf("Zadejte číslo úkolu ke splnění: ");
    scanf("%d", &index);

    if (index > 0 && index <= todoCount)
    {
        todos[index - 1].completed = 1;
        printf("Úkol označen jako splněný!\n");
    }
    else
    {
        printf("Neplatné číslo úkolu!\n");
    }
}

// Hlavní menu
void showMenu()
{
    printf("\n=== Správce úkolů ===\n");
    printf("1. Přidat úkol\n");
    printf("2. Zobrazit úkoly\n");
    printf("3. Označit úkol jako splněný\n");
    printf("4. Konec\n");
    printf("Vyberte možnost: ");
}

int main()
{
    int choice;

    do
    {
        showMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addTodo();
            break;
        case 2:
            listTodos();
            break;
        case 3:
            markCompleted();
            break;
        case 4:
            printf("Nashledanou!\n");
            break;
        default:
            printf("Neplatná volba!\n");
        }
    } while (choice != 4);

    return 0;
}