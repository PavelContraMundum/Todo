#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TODOS 100
#define MAX_LENGTH 100
#define FILENAME "todos.txt"

struct Todo
{
    char description[MAX_LENGTH];
    int completed;
    int priority;
};

// Globální proměnné
struct Todo todos[MAX_TODOS];
int todoCount = 0;

// Nová funkce pro uložení úkolů do souboru
void saveTodos()
{
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL)
    {
        printf("Chyba: Nelze otevřít soubor pro zápis!\n");
        return;
    }

    // První řádek obsahuje počet úkolů
    fprintf(file, "%d\n", todoCount);

    // Zápis jednotlivých úkolů
    for (int i = 0; i < todoCount; i++)
    {
        fprintf(file, "%s\n%d\n%d\n",
                todos[i].description,
                todos[i].completed,
                todos[i].priority);
    }

    fclose(file);
    printf("Úkoly byly úspěšně uloženy do souboru.\n");
}

// Nová funkce pro načtení úkolů ze souboru
void loadTodos()
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("Soubor s úkoly neexistuje. Začínáme s prázdným seznamem.\n");
        return;
    }

    // Čtení počtu úkolů
    if (fscanf(file, "%d\n", &todoCount) != 1)
    {
        printf("Chyba při čtení souboru!\n");
        fclose(file);
        return;
    }

    // Čtení jednotlivých úkolů
    for (int i = 0; i < todoCount; i++)
    {
        if (fgets(todos[i].description, MAX_LENGTH, file) == NULL)
            break;
        // Odstranění znaku nového řádku
        todos[i].description[strcspn(todos[i].description, "\n")] = 0;

        if (fscanf(file, "%d\n%d\n",
                   &todos[i].completed,
                   &todos[i].priority) != 2)
            break;
    }

    fclose(file);
    printf("Úkoly byly načteny ze souboru.\n");
}

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
    saveTodos(); // Automatické uložení po přidání
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
    saveTodos(); // Automatické uložení po změně
}

void deleteTodo()
{
    if (todoCount == 0)
    {
        printf("Seznam je prázdný!\n");
        return;
    }

    listTodos();

    int index;
    printf("Zadejte číslo úkolu ke smazání: ");
    if (scanf("%d", &index) != 1)
    {
        printf("Neplatný vstup!\n");
        clearInputBuffer();
        return;
    }

    if (index < 1 || index > todoCount)
    {
        printf("Neplatné číslo úkolu!\n");
        return;
    }

    // Přesun všech ostatních úkolů o jeden index dolů
    for (int i = index - 1; i < todoCount - 1; i++)
    {
        strcpy(todos[i].description, todos[i + 1].description);
        todos[i].completed = todos[i + 1].completed;
        todos[i].priority = todos[i + 1].priority;
    }

    todoCount--;
    printf("Úkol byl smazán.\n");
    saveTodos();
}

// Hlavní menu
void showMenu()
{
    printf("\n=== Správce úkolů ===\n");
    printf("1. Přidat úkol\n");
    printf("2. Zobrazit úkoly\n");
    printf("3. Označit úkol jako splněný\n");
    printf("4. Smazat úkol\n");
    printf("5. Konec\n");
    printf("Vyberte možnost: ");
}

int main()
{
    // Načtení úkolů při spuštění
    loadTodos();

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
            deleteTodo();
            break;
        case 5:
            printf("Nashledanou!\n");
            break;
        default:
            printf("Neplatná volba!\n");
        }
    } while (choice != 5);

    return 0;
}