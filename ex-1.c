#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct patient {
    int patientId;
    char name[30];
    char address[30];
    long long phNo;
    int age;
    char disease[30];
} pat;

// Function to check if patient ID already exists
int avlPatientId(int pid) {
    FILE *fp = fopen("HospitalRecord.dat", "rb");
    if (fp == NULL) return 0; 

    struct patient temp;
    while (fread(&temp, sizeof(temp), 1, fp)) {
        if (pid == temp.patientId) {
            fclose(fp);
            return 1; 
        }
    }
    fclose(fp);
    return 0; 
}

// Function to insert a new patient record
void insert() {
    int id;
    printf("Enter the Patient ID   : ");
    scanf("%d", &id);

    if (avlPatientId(id)) { 
        printf("Patient ID %d already exists in file.\n", id);
        return;
    }

    FILE *fp = fopen("HospitalRecord.dat", "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    pat.patientId = id;
    printf("Enter the Name         : ");
    scanf("%s", pat.name);
    printf("Enter the Address      : ");
    scanf("%s", pat.address);
    printf("Enter the Phone Number : ");
    scanf("%lld", &pat.phNo);
    printf("Enter the Age          : ");
    scanf("%d", &pat.age);
    printf("Enter the Disease      : ");
    scanf("%s", pat.disease);

    fwrite(&pat, sizeof(pat), 1, fp);
    fclose(fp);
    printf("\nRecord inserted successfully!\n");
}

// Function to display all records
void disp() {
    FILE *fp = fopen("HospitalRecord.dat", "rb");
    if (fp == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    printf("\n====================================================================================\n");
    printf("Patient ID\tName\t\tAddress\t\tPhone Number\tAge\tDisease\n");
    printf("====================================================================================\n");

    while (fread(&pat, sizeof(pat), 1, fp)) {
        printf("%-10d\t%-10s\t%-10s\t%lld\t%d\t%s\n",
               pat.patientId, pat.name, pat.address, pat.phNo, pat.age, pat.disease);
    }

    printf("====================================================================================\n");
    fclose(fp);
}

// Function to check if file is empty
int empty() {
    FILE *fp = fopen("HospitalRecord.dat", "rb");
    if (fp == NULL) return 0;
    if (fread(&pat, sizeof(pat), 1, fp)) {
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}

// Function to search patient by ID
void search() {
    FILE *fp = fopen("HospitalRecord.dat", "rb");
    if (fp == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    int pid, found = 0;
    printf("\nEnter the Patient ID you want to search: ");
    scanf("%d", &pid);

    while (fread(&pat, sizeof(pat), 1, fp)) {
        if (pat.patientId == pid) {
            printf("\n======================================\n");
            printf("Patient ID : %d\n", pat.patientId);
            printf("Name       : %s\n", pat.name);
            printf("Address    : %s\n", pat.address);
            printf("Phone No   : %lld\n", pat.phNo);
            printf("Age        : %d\n", pat.age);
            printf("Disease    : %s\n", pat.disease);
            printf("======================================\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nPatient ID %d not found in the file.\n", pid);

    fclose(fp);
}
// Function to delete a patient record
void deletefile() {
    FILE *fpo, *fpt;
    int pid, s;
    printf("Enter the Patient ID you want to delete: ");
    scanf("%d", &pid);
    if (avlPatientId(pid) == 0) {
        printf("Patient ID %d is not available in the file\n", pid);
        return;
    }

    fpo = fopen("HospitalRecord.dat", "rb");
    fpt = fopen("TempFile.dat", "wb");
    if (fpo == NULL || fpt == NULL) {
        printf("Error opening file!\n");
        if (fpo) fclose(fpo);
        if (fpt) fclose(fpt);
        return;
    }

    while (fread(&pat, sizeof(pat), 1, fpo)) {
        s = pat.patientId;
        if (s != pid) {
            fwrite(&pat, sizeof(pat), 1, fpt);
        }
    }
    fclose(fpo);
    fclose(fpt);

    fpo = fopen("HospitalRecord.dat", "wb");
    fpt = fopen("TempFile.dat", "rb");
    if (fpo == NULL || fpt == NULL) {
        printf("Error opening file!\n");
        if (fpo) fclose(fpo);
        if (fpt) fclose(fpt);
        return;
    }

    while (fread(&pat, sizeof(pat), 1, fpt)) {
        fwrite(&pat, sizeof(pat), 1, fpo);
    }
    printf("\nRECORD DELETED\n");
    fclose(fpo);
    fclose(fpt);
    remove("TempFile.dat");
}
// Function to update a patient record
void update() {
    FILE *fpo, *fpt;
    int pid, s, ch;
    printf("Enter Patient ID to update: ");
    scanf("%d", &pid);
    if (avlPatientId(pid) == 0) {
        printf("Patient ID %d is not available in the file\n", pid);
        return;
    }

    fpo = fopen("HospitalRecord.dat", "rb");
    fpt = fopen("TempFile.dat", "wb");
    if (fpo == NULL || fpt == NULL) {
        printf("Error opening file!\n");
        if (fpo) fclose(fpo);
        if (fpt) fclose(fpt);
        return;
    }

    while (fread(&pat, sizeof(pat), 1, fpo)) {
        s = pat.patientId;
        if (s != pid) {
            fwrite(&pat, sizeof(pat), 1, fpt);
        } else {
            printf("\n\t1. Update Name of Patient ID %d", pid);
            printf("\n\t2. Update Phone Number of Patient ID %d", pid);
            printf("\n\t3. Update Age of Patient ID %d", pid);
            printf("\n\t4. Update Disease of Patient ID %d", pid);
            printf("\n\t5. Update all fields of Patient ID %d", pid);
            printf("\nEnter your choice: ");
            scanf("%d", &ch);
            switch (ch) {
                case 1:
                    printf("Enter Name: ");
                    scanf("%s", pat.name);
                    break;
                case 2:
                    printf("Enter Phone Number: ");
                    scanf("%lld", &pat.phNo);
                    break;
                case 3:
                    printf("Enter Age: ");
                    scanf("%d", &pat.age);
                    if (pat.age < 0 || pat.age > 150) {
                        printf("Invalid age!\n");
                        fclose(fpo);
                        fclose(fpt);
                        return;
                    }
                    break;
                case 4:
                    printf("Enter Disease: ");
                    scanf("%s", pat.disease);
                    break;
                case 5:
                    printf("Enter Name: ");
                    scanf("%s", pat.name);
                    printf("Enter Address: ");
                    scanf("%s", pat.address);
                    printf("Enter Phone Number: ");
                    scanf("%lld", &pat.phNo);
                    printf("Enter Age: ");
                    scanf("%d", &pat.age);
                    if (pat.age < 0 || pat.age > 150) {
                        printf("Invalid age!\n");
                        fclose(fpo);
                        fclose(fpt);
                        return;
                    }
                    printf("Enter Disease: ");
                    scanf("%s", pat.disease);
                    break;
                default:
                    printf("Invalid Selection\n");
                    fclose(fpo);
                    fclose(fpt);
                    return;
            }
            fwrite(&pat, sizeof(pat), 1, fpt);
        }
    }
    fclose(fpo);
    fclose(fpt);

    fpo = fopen("HospitalRecord.dat", "wb");
    fpt = fopen("TempFile.dat", "rb");
    if (fpo == NULL || fpt == NULL) {
        printf("Error opening file!\n");
        if (fpo) fclose(fpo);
        if (fpt) fclose(fpt);
        return;
    }

    while (fread(&pat, sizeof(pat), 1, fpt)) {
        fwrite(&pat, sizeof(pat), 1, fpo);
    }
    printf("\nRECORD UPDATED\n");
    fclose(fpo);
    fclose(fpt);
    remove("TempFile.dat");
}

// Function to sort patient records by ID
void sort() {
    int a[100], count = 0, i, j, t;
    FILE *fp = fopen("HospitalRecord.dat", "rb");
    if (fp == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    while (fread(&pat, sizeof(pat), 1, fp)) {
        a[count] = pat.patientId;
        count++;
    }
    fclose(fp);

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (a[i] > a[j]) {
                t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
        }
    }

    printf("\nPatient ID\tName\t\tAddress\t\tPhone Number\tAge\tDisease\n\n");
    fp = fopen("HospitalRecord.dat", "rb");
    for (i = 0; i < count; i++) {
        rewind(fp);
        while (fread(&pat, sizeof(pat), 1, fp)) {
            if (a[i] == pat.patientId) {
                printf("  %d\t\t%s\t\t%s\t\t%lld\t\t%d\t%s\n",
                       pat.patientId, pat.name, pat.address, pat.phNo, pat.age, pat.disease);
            }
        }
    }
    fclose(fp);
}

// ========================= MAIN FUNCTION ==============================
int main() {
    int choice;
    do {
        printf("\n===== HOSPITAL MANAGEMENT SYSTEM =====\n");
        printf("1. Add Patient Record\n");
        printf("2. Display All Records\n");
        printf("3. Search Patient\n");
        printf("4. delete patient\n");
        printf("5. update patient id\n");
        printf("6. sort patient id\n");
        printf("7. Exit\n");
        printf("---------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                insert();
                break;
            case 2:
                if (empty() == 0)
                    printf("\nThe file is EMPTY.\n");
                else
                    disp();
                break;
            case 3:
                search();
                break;
            case 4:
                deletefile();
                break;
            case 5:
                update();
                break;
            case 6:
                if (empty() == 0)
                    printf("\nThe file is EMPTY\n");
                else
                    sort();
                break;
            case 7:
                printf("\nExiting program...\n");
                exit(0);
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    } while (choice != 7);

    return 0;
}

