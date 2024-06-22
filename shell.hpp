#pragma once

/* includes */
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

/* defines */
#define NULL_STR ""

/* vars */
char* DELIMITER = " \t\n\a";

std::string ShellLine;
std::string shellCommand[] = {
                            "cd",
                            "help",
                            "exit"
                            };

std::vector<std::string> tokens;