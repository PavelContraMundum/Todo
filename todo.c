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

// Nová funkce pro vyčištění vstupního bufferu
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Nová funkce pro kontrolu, zda je řetězec prázdný nebo obsahuje jen mezery
int isEmptyString(const char *str)
{
    while (*str != '\0')
    {
        if (!isspace((unsigned char)*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}

// Nová funkce pro bezpečné načtení čísla
int getValidNumber(int min, int max, const char *prompt)
{
    int number;
    char input[MAX_LENGTH];
    int valid = 0;

    do
    {
        printf("%s", prompt);
        if (fgets(input, MAX_LENGTH, stdin) != NULL)
        {
            // Kontrola, zda je vstup číslo
            char *endptr;
            number = strtol(input, &endptr, 10);

            // Kontrola, zda byl celý vstup převeden na číslo
            if (*endptr == '\n' && number >= min && number <= max)
            {
                valid = 1;
            }
            else
            {
                printf("Neplatný vstup! Zadejte číslo mezi %d a %d.\n", min, max);
            }
        }
    } while (!valid);

    return number;
}

void addTodo()
{
    if (todoCount >= MAX_TODOS)
    {
        printf("Seznam je plný!\n");
        return;
    }

    // Před čtením popisu vyčistíme buffer
    clearInputBuffer();

    printf("Zadejte popis úkolu: ");
    if (fgets(todos[todoCount].description, MAX_LENGTH, stdin) == NULL)
    {
        printf("Chyba při čtení vstupu!\n");
        return;
    }

    // Odstranění znaku nového řádku z popisu
    todos[todoCount].description[strcspn(todos[todoCount].description, "\n")] = 0;

    printf("Zadejte prioritu (1-3): ");
    if (scanf("%d", &todos[todoCount].priority) != 1)
    {
        printf("Neplatná priorita!\n");
        clearInputBuffer(); // Vyčistíme buffer po neplatném vstupu
        return;
    }

    // Kontrola rozsahu priority
    if (todos[todoCount].priority < 1 || todos[todoCount].priority > 3)
    {
        printf("Priorita musí být mezi 1 a 3!\n");
        return;
    }

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
    if (todoCount == 0)
    {
        printf("Seznam je prázdný!\n");
        return;
    }

    listTodos();

    int index;
    printf("Zadejte číslo úkolu ke splnění: ");
    if (scanf("%d", &index) != 1)
    {
        printf("Neplatný vstup!\n");
        clearInputBuffer(); // Vyčistíme buffer po neplatném vstupu
        return;
    }

    if (index < 1 || index > todoCount)
    {
        printf("Neplatné číslo úkolu!\n");
        return;
    }

    todos[index - 1].completed = 1;
    printf("Úkol označen jako splněný!\n");
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
        if (scanf("%d", &choice) != 1)
        {
            printf("Neplatná volba!\n");
            clearInputBuffer(); // Vyčistíme buffer po neplatném vstupu
            continue;
        }

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