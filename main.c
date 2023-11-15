#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_LINE_LENGTH 100

int compare_files(FILE *file1, FILE *file2) {
    char line1[MAX_LINE_LENGTH];
    char line2[MAX_LINE_LENGTH];
    int score = 100;
    rewind(file1);
    while (fgets(line1, sizeof(line1), file1) && fgets(line2, sizeof(line2), file2)) {
        if (strcmp(line1, line2) != 0) {
            score--; // Decrement the score for every differing line.
        }
    }
    fclose(file2);
    return score;
}

char* assign_grade(int score) {
    if (score >= 94) {
        return "A";
    } else if (score >= 89) {
        return "A-";
    } else if (score >= 86) {
        return "B+";
    } else if (score >= 82) {
        return "B";
    } else if (score >= 78) {
        return "B-";
    } else if (score >= 75) {
        return "C+";
    } else if (score >= 71) {
        return "C";
    } else if (score >= 68) {
        return "C-";
    } else if (score >= 65) {
        return "D+";
    } else if (score >= 61) {
        return "D";
    } else if (score >= 58) {
        return "D-";
    } else {
        return "F";
    }
}

int main() {
    const char *answer_file = "answer_code.c";
    const char *directory_name = ".";
    FILE *answer = fopen(answer_file, "r");
    if (!answer) {
        perror("Failed to open answer file");
        return 1;
    }
    FILE *student_grades = fopen("student_grades.txt", "w");
    if (!student_grades) {
        perror("Failed to create student_grades file");
        fclose(answer);
        return 1;
    }

    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(directory_name)) == NULL) {
        perror("Failed to open directory");
        fclose(answer);
        fclose(student_grades);
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".c") != NULL &&
            strcmp(entry->d_name, "main.c") != 0 && strcmp(entry->d_name, answer_file) != 0) {
            FILE *student_file = fopen(entry->d_name, "r");
            if (!student_file) {
                perror("Failed to open a student file");
                fclose(answer);
                fclose(student_grades);
                closedir(dir);
                return 1;
            }
            int score = compare_files(answer, student_file);
            fclose(student_file);

            fprintf(student_grades, "%s %d %s\n", entry->d_name, score, assign_grade(score));
        }
    }

    closedir(dir);
    fclose(answer);
    fclose(student_grades);

    // Now, we need to sort the student grades by score.
    system("sort -k 2,2nr -k 1,1 < student_grades.txt > sorted_student_grades.txt");

    return 0;
}

