#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "courses.dat"
#define COURSE_SIZE sizeof(COURSE)



typedef struct {
    char course_name[84];
    char schedule[4];
    unsigned enrollment;
    unsigned credit_hours;
    char padding[20];
} COURSE;



//create sections

void print_menu();
void create_course();
void read_course();
void update_course();
void delete_course();
void clear_buffer();
void display_course(const COURSE *course);


// menu
void print_menu() {
    printf("Enter one of the following actions or press CTRL-D to exit.\n");

    printf("C - create a new course record\n");
 
    printf("U - update an existing course record\n"); 

    printf("R - read an existing course record\n");

    printf("D - delete an existing course record\n");
}

void clear_buffer() {

    while (getchar() != '\n');

}




void display_course(const COURSE *course) {
    printf("Course name: %s\n", course->course_name);
    printf("Scheduled days: %s\n", course->schedule);
    printf("Credit hours: %u\n", course->credit_hours);
    printf("Enrolled Students: %u\n", course->enrollment);
}

// new
void create_course() {
    FILE *file = fopen(FILENAME, "rb+");


    if (!file) file = fopen(FILENAME, "wb+");

    if (!file) {
        perror("Error opening file");
        return;
    }

    int course_number;

    COURSE new_course = {0};

    printf("Course number: ");
    scanf("%d", &course_number);
    clear_buffer();



    printf("Course name: ");
    fgets(new_course.course_name, 84, stdin);
    new_course.course_name[strcspn(new_course.course_name, "\n")] = '\0';

    printf("Course schedule (MWF or TR): ");
    scanf("%3s", new_course.schedule);
 
   printf("Course credit hours: ");
    scanf("%u", &new_course.credit_hours);

     printf("Course enrollment: ");
     scanf("%u", &new_course.enrollment);


    fseek(file, course_number * COURSE_SIZE, SEEK_SET);
    COURSE temp; 
    if (fread(&temp, COURSE_SIZE, 1, file) == 1 && temp.course_name[0] != '\0') 
{
        printf("ERROR: course already exists\n"); 
        fclose(file);
        return; 
    }

    fseek(file, course_number * COURSE_SIZE, SEEK_SET);
    fwrite(&new_course, COURSE_SIZE, 1, file);
    fclose(file); 

    printf("Course created successfully.\n");
}

 
void read_course() 
{
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("ERROR: No courses found.\n");
        return; 
    }
 

    int course_number;

    printf("Enter a CS course number: ");
    scanf("%d", &course_number);

    clear_buffer();

   	 fseek(file, course_number * COURSE_SIZE, SEEK_SET); 
    COURSE course;
    if (fread(&course, COURSE_SIZE, 1, file) != 1 || course.course_name[0] == '\0') 
{
        printf("ERROR: course not found\n");
    } else {
        display_course(&course);
    }


    fclose(file);
}


void update_course() {
    FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("ERROR: No courses found.\n");
        return;
    }

    int course_number;
    printf("Course number: ");
    scanf("%d", &course_number);
    clear_buffer();

    fseek(file, course_number * COURSE_SIZE, SEEK_SET);
    COURSE course;
    if (fread(&course, COURSE_SIZE, 1, file) != 1 || course.course_name[0] == '\0') 
{
        printf("ERROR: course not found\n");

        fclose(file);

        return;
    } 
 
    printf("Course name [%s]: ", course.course_name);
    char input[84];
    fgets(input, 84, stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = '\0';
        strcpy(course.course_name, input);
    }



    printf("Course schedule [%s]: ", course.schedule);
    fgets(input, 4, stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = '\0';
        strcpy(course.schedule, input);
    }



    printf("Course credit hours [%u]: ", course.credit_hours);
    unsigned hours;
    if (scanf("%u", &hours) == 1 && hours > 0) 
{
        course.credit_hours = hours;
    }
    clear_buffer();

    printf("Course enrollment [%u]: ", course.enrollment);
    unsigned enrollment;
    if (scanf("%u", &enrollment) == 1 && enrollment > 0)
 {
        course.enrollment = enrollment;
  

  }
    clear_buffer();

    fseek(file, course_number * COURSE_SIZE, SEEK_SET);
    fwrite(&course, COURSE_SIZE, 1, file);
    fclose(file);



    printf("Course updated successfully.\n");
}



void delete_course() {
    FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("ERROR: No courses found.\n");
        return;
    }

    int course_number;
    printf("Enter a course number: ");
    scanf("%d", &course_number);
    clear_buffer();

    fseek(file, course_number * COURSE_SIZE, SEEK_SET);
    COURSE course = {0};
    if (fread(&course, COURSE_SIZE, 1, file) != 1 || course.course_name[0] == '\0') 
 { 
        printf("ERROR: course not found\n");
        fclose(file);
        return;
    

}

    memset(&course, 0, COURSE_SIZE);
    fseek(file, course_number * COURSE_SIZE, SEEK_SET);
    fwrite(&course, COURSE_SIZE, 1, file);
    fclose(file);

    printf("Course %d was successfully deleted.\n", course_number);
}

// main

int main() 
{
    char action;

    while (1) {
        print_menu();
        printf("Choice: ");
        if (scanf(" %c", &action) == EOF) 
{
            printf("Exiting...\n");
            break;
        }

        switch (action) 
{
            case 'C':
            case 'c':

                create_course();
                break;

            case 'R':
            case 'r':
                read_course();
                break;

            case 'U':
            case 'u':
                update_course();
                break;

            case 'D':
            case 'd':
                delete_course();
                break;



            default:
                printf("ERROR: invalid option\n");
        }
    }

    return 0;
}
