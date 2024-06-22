/* includes*/
#include "shell.hpp"

/* builtin methods */
int shell_builtin_num() { return sizeof(shellCommand) / sizeof(shellCommand[0]); }

int shell_cd(){
    if(tokens[1] == NULL_STR) { std::cout << "shell: expected argument to cd\n"; exit(EXIT_FAILURE); }
    else { if(chdir(tokens[1].c_str()) != 0) { std::perror("shell"); } exit(EXIT_FAILURE); }

    return 1;
}

int shell_help(){
    std::cout << "-------> SHjRYOKU'S shell <-------\n\n\nWrite the command and press enter.\nThe following are built in:\n\n";

    for (int i = 0; i < shell_builtin_num(); i++)
    {
        std::cout << '\t' << shellCommand[i] << std::endl;
    }
    
    std::cout << '\n' << "Use the man commands for information on other programs.\n\n";

    exit(EXIT_SUCCESS);
    return 1;
}

int shell_exit(){
    exit(EXIT_SUCCESS);
    return 1;
}

/* methods */
void read_line(){
    std::cout << "#->  ";
    std::cin >> ShellLine;
}

std::vector<std::string> split_line(const std::string& str){
    std::stringstream ss(str);
    std::string token;

    while(std::getline(ss, token, *DELIMITER)){ tokens.push_back(token); }

    return tokens;
}

int shell_launch(const std::vector<std::string>& toks){
    pid_t child_id = fork(), wchild_id;
    int status;

    if(child_id == 0) {
        char* args[toks.size()+1];
        for (int i = 0; i < shell_builtin_num(); i++) { args[i] = const_cast<char*>(toks[i].c_str()); }
        args[toks.size()] = nullptr;

        if(execvp(args[0], args) == -1) { std::perror("execvp"); delete[] args; exit(EXIT_FAILURE); }
        delete[] args;
        exit(EXIT_FAILURE);
    } else if(child_id < 0) { std::perror("fork"); exit(EXIT_FAILURE); }
    else do{
        wchild_id = waitpid(child_id, &status, WUNTRACED);
        if(wchild_id == -1) { std::perror("waitpid"); exit(EXIT_FAILURE); }
    }while(!WIFEXITED(status) && !WIFSIGNALED(status));
}

int shell_execute(const std::vector<std::string>& toks){
    if(toks[0] == NULL_STR) { return 1; }

    for (int i = 0; i < shell_builtin_num(); i++)
    {
        if(toks[0] == shellCommand[i])
            switch (i)
            {
            case 0: return shell_cd();
            case 1: return shell_help();
            case 2: return shell_exit();
            }
    }
    
    return shell_launch(toks);
}

void shell_loop(){
    int status;

    do
    {
        read_line();
        split_line(ShellLine);
        status = shell_execute(tokens);
    } while (status);   
}

/* init */
int main(){
    shell_loop();

    return EXIT_SUCCESS;
}