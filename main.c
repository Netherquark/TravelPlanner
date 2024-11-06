#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define MAX_CITIES 15
#define INF INT_MAX

typedef struct {
    char name[50];
    int distances[MAX_CITIES];
} City;

City cities[MAX_CITIES];
int cityCount = 0;

void addCity(const char *name, int promptForDistances);
int addRoute(const char *fromCity, const char *toCity, int distance);
void displayMap();
void dijkstra(int start, int end);
void userInteraction();
int findCityIndex(const char *name);
int isValidCityName(const char *name);

void initializeCities() {
    addCity("Mumbai", 0);
    addCity("Delhi", 0);
    addCity("Bangalore", 0);
    addCity("Hyderabad", 0);
    addCity("Chennai", 0);
    addCity("Kolkata", 0);
    addCity("Ahmedabad", 0);
    addCity("Pune", 0);
    addCity("Jaipur", 0);
    addCity("Lucknow", 0);
    addCity("Chandigarh", 0);

    addRoute("Mumbai", "Bangalore", 980);
    addRoute("Mumbai", "Hyderabad", 710);
    addRoute("Mumbai", "Pune", 150);
    addRoute("Mumbai", "Ahmedabad", 530);
    addRoute("Delhi", "Hyderabad", 1580);
    addRoute("Delhi", "Bangalore", 2150);
    addRoute("Delhi", "Jaipur", 280);
    addRoute("Delhi", "Lucknow", 555);
    addRoute("Delhi", "Chandigarh", 250);
    addRoute("Bangalore", "Hyderabad", 575);
    addRoute("Bangalore", "Chennai", 345);
    addRoute("Bangalore", "Pune", 840);
    addRoute("Bangalore", "Ahmedabad", 1490);
    addRoute("Hyderabad", "Chennai", 625);
    addRoute("Hyderabad", "Pune", 560);
    addRoute("Hyderabad", "Ahmedabad", 1210);
    addRoute("Chennai", "Kolkata", 1670);
    addRoute("Chennai", "Pune", 1150);
    addRoute("Kolkata", "Delhi", 1460);
    addRoute("Kolkata", "Lucknow", 980);
    addRoute("Kolkata", "Jaipur", 1530);
    addRoute("Kolkata", "Chandigarh", 1650);
    addRoute("Ahmedabad", "Jaipur", 670);
    addRoute("Ahmedabad", "Pune", 660);
    addRoute("Pune", "Jaipur", 1180);
    addRoute("Jaipur", "Lucknow", 570);
    addRoute("Jaipur", "Chandigarh", 510);
    addRoute("Lucknow", "Chandigarh", 740);
}

void addCity(const char *name, int promptForDistances) {
    if (cityCount >= MAX_CITIES) {
        printf("Error: Maximum city limit of %d reached.\n", MAX_CITIES);
        return;
    }
    
    if (!isValidCityName(name)) {
        printf("Error: Invalid city name. Names should contain only alphabets and spaces.\n");
        return;
    }

    if (findCityIndex(name) != -1) {
        printf("Error: City '%s' already exists.\n", name);
        return;
    }

    City *city = &cities[cityCount++];
    strncpy(city->name, name, sizeof(city->name) - 1);
    city->name[sizeof(city->name) - 1] = '\0';

    for (int i = 0; i < MAX_CITIES; i++) {
        city->distances[i] = INF;
    }

    printf("City '%s' added successfully.\n", name);
}

int addRoute(const char *fromCity, const char *toCity, int distance) {
    if (distance <= 0) {
        printf("Error: Distance should be a positive integer.\n");
        return 0;
    }

    int fromIndex = findCityIndex(fromCity);
    int toIndex = findCityIndex(toCity);

    if (fromIndex == -1) {
        printf("Error: Source city '%s' not found.\n", fromCity);
        return 0;
    }
    if (toIndex == -1) {
        printf("Error: Destination city '%s' not found.\n", toCity);
        return 0;
    }

    cities[fromIndex].distances[toIndex] = distance;
    cities[toIndex].distances[fromIndex] = distance;
    printf("Route from '%s' to '%s' with distance %d added successfully.\n", fromCity, toCity, distance);
    return 1;
}

void displayMap() {
    printf("\nTravel Planner Map:\n");
    printf("=============================================\n");
    for (int i = 0; i < cityCount; i++) {
        printf("%s\n", cities[i].name);
        for (int j = 0; j < cityCount; j++) {
            if (cities[i].distances[j] != INF) {
                printf("    |--- %s [Distance: %d km]\n", cities[j].name, cities[i].distances[j]);
            }
        }
    }
    printf("=============================================\n\n");
}

void dijkstra(int start, int end) {
    int distance[MAX_CITIES];
    int visited[MAX_CITIES] = {0};
    int parent[MAX_CITIES];

    for (int i = 0; i < cityCount; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }
    distance[start] = 0;

    for (int count = 0; count < cityCount - 1; count++) {
        int min = INF, u = -1;
        
        for (int v = 0; v < cityCount; v++) {
            if (!visited[v] && distance[v] < min) {
                min = distance[v];
                u = v;
            }
        }

        if (u == -1) break;
        
        visited[u] = 1;

        for (int v = 0; v < cityCount; v++) {
            if (!visited[v] && cities[u].distances[v] != INF &&
                distance[u] + cities[u].distances[v] < distance[v]) {
                distance[v] = distance[u] + cities[u].distances[v];
                parent[v] = u;
            }
        }
    }

    if (distance[end] == INF) {
        printf("No route exists from '%s' to '%s'.\n", cities[start].name, cities[end].name);
        return;
    }

    printf("Shortest path from '%s' to '%s': %d km\n", cities[start].name, cities[end].name, distance[end]);

    printf("Path: ");
    int path[MAX_CITIES];
    int path_index = 0;
    for (int v = end; v != -1; v = parent[v]) {
        path[path_index++] = v;
    }
    for (int i = path_index - 1; i >= 0; i--) {
        printf("%s", cities[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

void userInteraction() {
    int choice;

    while (1) {
        printf("\nChoose an option:\n1. Add city\n2. Add route\n3. Display map\n4. Find route\n5. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. \n Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                char name[50];
                printf("Enter city name: ");
                while (1) {
                    if (fgets(name, sizeof(name), stdin) == NULL) {
                        printf("\n Error reading input. Try again.\n");
                        continue;
                    }
                    name[strcspn(name, "\n")] = '\0';
                    if (isValidCityName(name)) break;
                    printf("Invalid name.\nEnter a valid city name: ");
                }
                addCity(name, 1);
                break;
            }
            case 2: {
                int from, to, distance;
                printf("Enter source city number: ");
                while (scanf("%d", &from) != 1 || from < 1 || from > cityCount) {
                    printf("Invalid input. \nEnter a valid source city number: ");
                    while (getchar() != '\n');
                }

                printf("Enter destination city number: ");
                while (scanf("%d", &to) != 1 || to < 1 || to > cityCount) {
                    printf("Invalid input. \nEnter a valid destination city number: ");
                    while (getchar() != '\n');
                }

                printf("Enter distance: ");
                while (scanf("%d", &distance) != 1 || distance <= 0) {
                    printf("Invalid distance. \nEnter a positive distance: ");
                    while (getchar() != '\n');
                }
                
                addRoute(cities[from - 1].name, cities[to - 1].name, distance);
                break;
            }
            case 3:
                displayMap();
                break;
            case 4: {
                int start, end;
                printf("Enter start city number: ");
                while (scanf("%d", &start) != 1 || start < 1 || start > cityCount) {
                    printf("Invalid input. \nEnter a valid start city number: ");
                    while (getchar() != '\n');
                }
                printf("Enter end city number: ");
                while (scanf("%d", &end) != 1 || end < 1 || end > cityCount) {
                    printf("Invalid input. \nEnter a valid end city number: ");
                    while (getchar() != '\n');
                }
                dijkstra(start - 1, end - 1);
                break;
            }
            case 5:
                printf("\n Exiting the program.\n");
                return;
            default:
                printf("Invalid choice. \nPlease select a valid option.\n");
        }
    }
}

int findCityIndex(const char *name) {
    for (int i = 0; i < cityCount; i++) {
        if (strcmp(cities[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int isValidCityName(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && !isspace(name[i])) {
            return 0;
        }
    }
    return strlen(name) > 0;
}

int main() {
    printf("Welcome to the City Travel Planner!\n");
    initializeCities();
    // displayMap();
    userInteraction();
    return 0;
}
