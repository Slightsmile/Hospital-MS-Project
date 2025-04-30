#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

int ptn = 0;

struct patients {
    char name[50];
    char disease[50];
    int roomno;
    char admitdate[50];
    char doctorn[50];
} patients[MAX];

void removeNewline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}


void inputPatient(struct patients *p) {
    while ((getchar()) != '\n');

    printf(" Please Enter Name: ");
    fgets(p->name, sizeof(p->name), stdin);
    removeNewline(p->name);

    printf(" Please Enter Disease: ");
    fgets(p->disease, sizeof(p->disease), stdin);
    removeNewline(p->disease);

    printf(" Please Enter Admit Date: ");
    fgets(p->admitdate, sizeof(p->admitdate), stdin);
    removeNewline(p->admitdate);

    printf(" Please Enter Assigned Doctor Name: ");
    fgets(p->doctorn, sizeof(p->doctorn), stdin);
    removeNewline(p->doctorn);

    printf(" Please Enter Room No.: ");
    while (scanf("%d", &p->roomno) != 1 || p->roomno <= 0) {
        printf(" Invalid room number. Please enter a positive number: ");
        while (getchar() != '\n');
    }
}


void addpatients() {
    char ch = 'y';
    while (ch == 'y' || ch == 'Y') {
        if (ptn >= MAX) {
            printf(" Patient list is full!\n");
            break;
        }
        printf("\n--- Add Patient %d ---\n", ptn + 1);
        inputPatient(&patients[ptn]);
        ptn++;

        printf("\n Do you want to add more? (y/n): ");
        while ((getchar()) != '\n'); // Clear buffer
        scanf("%c", &ch);
    }
}


void showpatients() {
    if (ptn == 0) {
        printf("\n No patient data available.\n");
        return;
    }

    printf("\n %-5s %-20s %-15s %-15s %-20s %-10s\n", "S.No", "Name", "Disease", "Admit Date", "Doctor", "Room No");
    printf("---------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < ptn; i++) {
        printf(" %-5d %-20s %-15s %-15s %-20s %-10d\n",
               i + 1,
               patients[i].name,
               patients[i].disease,
               patients[i].admitdate,
               patients[i].doctorn,
               patients[i].roomno);
    }

    printf("---------------------------------------------------------------------------------------------\n");
}



void sortpatients() {
    for (int i = 0; i < ptn - 1; i++) {
        for (int j = 0; j < ptn - i - 1; j++) {
            if (patients[j].roomno > patients[j + 1].roomno) {
                struct patients temp = patients[j];
                patients[j] = patients[j + 1];
                patients[j + 1] = temp;
            }
        }
    }
    printf("\n Patients sorted by Room Number.\n");
}


void searchpatients() {
    if (ptn == 0) {
        printf("\n No patient records to search.\n");
        return;
    }

    int option;
    printf("\n Search by:\n 1. Name\n 2. Room Number\n\n Enter choice: ");
    scanf("%d", &option);
    while ((getchar()) != '\n');

    if (option == 1) {
        char name[50];
        printf(" Enter Name: ");
        fgets(name, sizeof(name), stdin);
        removeNewline(name);

        int found = 0;
        for (int i = 0; i < ptn; i++) {
            if (strcasecmp(patients[i].name, name) == 0) {
                printf("\n Found:\n Name: %s, Room: %d\n", patients[i].name, patients[i].roomno);
                found = 1;
            }
        }
        if (!found)
            printf("\n No patient found with name %s.\n", name);
    } else if (option == 2) {
        int room;
        printf(" Enter Room Number: ");
        scanf("%d", &room);

        int found = 0;
        for (int i = 0; i < ptn; i++) {
            if (patients[i].roomno == room) {
                printf("\n Found:\n Name: %s, Room: %d\n", patients[i].name, patients[i].roomno);
                found = 1;
            }
        }
        if (!found)
            printf("\n No patient found in Room %d.\n", room);
    } else {
        printf(" Invalid option.\n");
    }
}


void saveToFile() {
    FILE *fp = fopen("patients.txt", "w");
    if (!fp) {
        perror(" Error opening file for writing");
        return;
    }

    for (int i = 0; i < ptn; i++) {
        fprintf(fp, "%s|%s|%s|%s|%d\n",
            patients[i].name,
            patients[i].disease,
            patients[i].admitdate,
            patients[i].doctorn,
            patients[i].roomno);
    }

    fclose(fp);
    printf(" Data saved to patients.txt\n");
}

// Load patient data from file
void loadFromFile() {
    FILE *fp = fopen("patients.txt", "r");
    if (!fp) return;

    while (fscanf(fp, "%49[^|]|%49[^|]|%49[^|]|%49[^|]|%d\n",
                  patients[ptn].name,
                  patients[ptn].disease,
                  patients[ptn].admitdate,
                  patients[ptn].doctorn,
                  &patients[ptn].roomno) == 5) {
        ptn++;
    }

    fclose(fp);
}

// Display menu
void display() {
    printf("\n==== Hospital Patient Management System ====\n");
    printf(" 1. Add Patients\n");
    printf(" 2. Sort Patients by Room Number\n");
    printf(" 3. Display All Patients\n");
    printf(" 4. Search Patients\n");
    printf(" 5. Save to File\n");
    printf(" 0. Exit\n");
    printf("============================================\n");
}

int main() {
    int choice;
    loadFromFile(); // Load data at start

    while (1) {
        display();
        printf("\n\n Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf(" Invalid input.\n");
            while ((getchar()) != '\n'); // Clear buffer
            continue;
        }

        switch (choice) {
            case 1: addpatients(); break;
            case 2: sortpatients(); break;
            case 3: showpatients(); break;
            case 4: searchpatients(); break;
            case 5: saveToFile(); break;
            case 0: saveToFile(); printf(" Exiting program.\n"); return 0;
            default: printf(" Invalid choice. Try again.\n");
        }
    }

    return 0;
}
