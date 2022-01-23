#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_CMDS 100

typedef struct args {
    char **args;
    int len;
} Args;

char* read_cmds(char *buffer);
Args parse_cmds(Args argument, char *buffer);
int parsed_output_redirection(Args parse);
int parsed_input_redirection(Args parse);
int parsed_pipe(Args parse);
void executeNormally(Args parse);
void execOutputRedirection(Args parse, int redirect_index);
void execInputRedirection(Args parse, int redirect_index);
void execPiped(Args parse, int piped);

int main(void) {
    Args parse;
    Args previous_parse;
    char *buffer;
    int ch, i = 0;
    int should_run = 1;
    int output_redirection, input_redirection, piped;

    parse.args = NULL;

    while (should_run) {
        printf("osh> ");
        fflush(stdout);
        
        buffer = read_cmds(buffer);
        previous_parse = parse;
        parse = parse_cmds(parse, buffer);

        if (strcmp(parse.args[0], "exit") == 0)
            should_run = 0;
    
        if (strcmp(parse.args[0], "!!") == 0) {
            parse = previous_parse;
            previous_parse = parse;
            printf("osh> ");
            while (i < parse.len)
                printf("%s ", parse.args[i++]);
            i = 0;
            putchar('\n');
            
            if (parse.args == NULL) {
                printf("No commands in history\n");
                continue;
            }
        }

        output_redirection = parsed_output_redirection(parse);
        input_redirection = parsed_input_redirection(parse);
        piped = parsed_pipe(parse);
        
        // Execute differently if output redirection was included
        if (output_redirection != 0) {
            execOutputRedirection(parse, output_redirection);
            continue;
        }
        else if(input_redirection != 0) {
            if (access(parse.args[input_redirection + 1], F_OK) != 0) 
                continue;
            execInputRedirection(parse, output_redirection);
            continue;
        }
        else if (piped != 0) {
            execPiped(parse, piped);
            continue;
        }
        else
            executeNormally(parse);
        
    }

    return 0;
}

void executeNormally(Args parse) {
    pid_t p1;

    p1 = fork();
    if (p1 == 0) {
        parse.args[parse.len] = NULL;
        execvp(parse.args[0], parse.args);
        if (execvp(parse.args[0], parse.args) == -1) {
            printf("failure to execute\n");
            exit(0);
        }
    }
    else if (p1 > 0 && (strcmp(parse.args[parse.len-1], "&") != 0)){
        wait(NULL);
    }
    else if (strcmp(parse.args[parse.len-1], "&")  == 0 && p1 > 0) {
        parse.args[parse.len] = NULL;
        execvp(parse.args[0], parse.args);
    }
}

void execOutputRedirection(Args parse, int redirect_index) {
    int file;
    pid_t pid;
    
    pid = fork();
    file = open(parse.args[redirect_index + 1], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (pid == 0) {
        parse.args[parse.len] = NULL;
        if (file == -1 || parse.args[redirect_index + 1] == NULL) {
            printf("%d\n", file);
            printf("Invalid command\n");
            exit(1);
        }
        dup2(file, STDOUT_FILENO);
        close(file);

        parse.args[redirect_index] = NULL;
        parse.args[redirect_index+1] = NULL;

        for (int i = redirect_index + 2; parse.args[i] != NULL; i++) {
            parse.args[i-2] = malloc(sizeof(strlen(parse.args[i]) + 1) * sizeof(char));
            strcpy(parse.args[i-2], parse.args[i]);
            parse.args[i] = NULL;
        }
        parse.args[parse.len] = NULL;
        execvp(parse.args[0], parse.args);
    }
    else {
        if (strcmp(parse.args[parse.len-1], "&") != 0)
            wait(NULL);
    }
}

void execInputRedirection(Args parse, int redirect_index) {
    int file;
    pid_t pid;
    
    file = open(parse.args[redirect_index+1], STDIN_FILENO, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    pid = fork();

    if (pid == 0) {
        parse.args[parse.len] = NULL;
        if (file == -1 || parse.args[redirect_index + 1] == NULL) {
            printf("%d\n", file);
            printf("Invalid command\n");
            exit(1);
        }
        dup2(file, STDIN_FILENO);
        close(file);
        
        parse.args[redirect_index] = NULL;
        parse.args[redirect_index+1] = NULL;

        for (int i = redirect_index + 2; parse.args[i] != NULL; i++) {
            parse.args[i-2] = malloc(sizeof(strlen(parse.args[i]) + 1) * sizeof(char));
            strcpy(parse.args[i-2], parse.args[i]);
            parse.args[i] = NULL;
        }
        parse.args[parse.len] = NULL;
        execvp(parse.args[0], parse.args);
    }
    else {
        if (strcmp(parse.args[parse.len-1], "&") != 0)
            wait(NULL);
    }
}

void execPiped(Args parse, int piped) {
    int pipefd1[2];
    int i;
    Args afterPipe;
    pid_t p1, p2;

    afterPipe.args = malloc(sizeof(char) * MAX_CMDS);

    for (i = 0; i < 100; i++) {
        afterPipe.args[i] = malloc(strlen(parse.args[piped+1+i] + 1) * sizeof(char));
        strcpy(afterPipe.args[i], parse.args[piped+1+i]);
        parse.args[piped+1+i] = NULL;
        afterPipe.len = i+1;
        if (parse.args[piped+1+i] == NULL) break;
    }
    parse.args[piped] = NULL;

    if (pipe(pipefd1) == -1) {
        printf("\nPipe failed\n");
        return;
    }

    p1 = fork();
    
    if (p1 < 0) {
        printf("Could not fork\n");
        return;
    }
    printf("%s\n", parse.args[0]);
    if (p1 == 0) {
        close(pipefd1[0]);
        dup2(pipefd1[1], STDOUT_FILENO);
        close(pipefd1[1]);
        printf("%s\n", parse.args[0]);
        parse.args[piped-1] = NULL;
        if (execvp(parse.args[0], parse.args) < 0) {
            printf("\nCould not execute command 1..\n");
            return;
        }
    }
    else {
        p2 = fork();
        // printf("p2\n");
        if (p1 < 0) {
            printf("Could not fork\n");
            return;
        }

        if (p2 == 0) {
            wait(NULL);
            close (pipefd1[1]);
            dup2 (pipefd1[0], STDIN_FILENO);
            close (pipefd1[0]);
            printf("%s\n", afterPipe.args[0]);
            afterPipe.args[afterPipe.len] = NULL;
            printf("%s\n", afterPipe.args[0]);
            execvp(afterPipe.args[0], afterPipe.args);
           if(execvp(afterPipe.args[0], afterPipe.args) < 0) {
               printf("Could not execute command 2..\n");
               return;
           }
        } 
        else {
            wait(NULL);
        }
    }

}

char* read_cmds(char *buffer) {
    char ch;
    int position, bufsize;

    bufsize = MAX_CMDS;

    buffer = malloc(sizeof(ch) * bufsize);
    if (buffer == NULL) {
        printf("memalloc failed in read_cmds\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        ch = getchar();

        if (ch == '\n' || ch == EOF) {
            buffer[position] = '\0';
            return buffer;
        }
        else {
            buffer[position] = ch;
        }
        position++;

        if (position >= bufsize) {
            bufsize += MAX_CMDS;
            buffer = realloc(buffer, bufsize);
            if (buffer == NULL) {
                printf("memalloc failed in read_cmds\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

Args parse_cmds(Args parse, char *buffer) {
    int i = 0;

    parse.args = malloc(sizeof(char) * strlen(buffer));
    
    parse.args[0] = strtok(buffer, " \n\t\b");
    while (parse.args[i] != NULL) {
       parse.args[++i] = strtok(NULL, " \n\t\b");
    }
    parse.args[i] = NULL;
    parse.len = i;

    return parse;
}

int parsed_output_redirection(Args parse) {
    int i = 0;

    for (int i = 0; i < parse.len; i++) {
        if(strcmp(parse.args[i], ">") == 0) {
            return i;
        }
    }   
    return 0;
}

int parsed_input_redirection(Args parse) {
    int i = 0;

    for (int i = 0; i < parse.len; i++) {
        if(strcmp(parse.args[i], "<") == 0) {
            return i;
        }
    }   
    return 0;
}

int parsed_pipe(Args parse) {
    int i = 0;

    for (int i = 0; i < parse.len; i++) {
        if(strcmp(parse.args[i], "|") == 0) {
            return i;
        }
    }   
    return 0;
}