[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=16113891)
Project 1: Simple Shell
==================

INSTRUCTIONS
============

In this project, you will explore and extend a simple Unix shell interpreter.
In doing so, you will learn the basics of system calls for creating and
managing processes.


STEP 1:  Compile the shell
==========================

    chmod +x b.sh
    make
    make test   # Use in Step 5 to test your changes to the project
    ./shell


STEP 2:  Try using the shell
============================

  Note: You need to specify the absolute paths of commands.

  Some commands to try:
  
    /bin/ls
    /bin/ls ..
    cd /
    /bin/pwd
    /bin/bash
    exit
    ./shell     (Note: You need to be in the project directory.)
    ./shell&    (Note: You need to be in the project directory.)
    ./b.sh      (Note: You need to be in the project directory.)
    /bin/kill -s KILL nnnn      (Where nnnn is a process ID.)

  "./" means the current directory


STEP 3:  Study the implementation of the shell
==============================================

  In preparation for the questions in Step 4, please explore the source code
  for the shell contained in 'shell.c'.  You needn't understand every detail
  of the implementation, but try to familiarize yourself with the structure
  of the code, what it's doing, and the various library functions involved.
  Please use the 'man' command to browse the Unix manual pages describing
  functions with which you are unfamiliar or use Google.


STEP 4:  Questions
==================

  1. Why is it necessary to implement a change directory 'cd' command in
     the shell?  Could it be implemented by an external program instead?
     (could you call 'exec' to run 'cd'?)

    Answer: Calling cd lets the shell know that any other commands would be in a different directory than the current one, where-as using exec creates a new shell entirely.
    
  2. Explain in detail how our sample shell implements the change directory command.

    Answer: It implements the change directory command by using chdir() anytime "cd" is detected using some conditions, and checks if the directory change was successful.
     
  3. What would happen to the shell if this program did not use the fork function, but
     just used execv directly? Explain why that happens. (Try it!)

     Try <ins>temporarily</ins> changing the code 'pid_from_fork = fork();'
     to 'pid_from_fork = 0;'

    Answer: If it were to use execv directly then the current shell would be replaced rather than using fork(); which is used to create the new child process but let the parent keep running. changing to 'pid_from_fork = 0;' will have similar effect because pid_from_fork is how the shell knows to fork a new child.
     
  4. Explain what the return value of fork() means and how this program
     uses it.

    Answer: fork() either returns a value that is zero, < zero, or > zero. This value could be the PID if positive, 0 if it is the child process, and negative if the fork failed. fork() is used to create a child process while the parent process waits.
     
  5. What would happen if fork() were called prior to chdir(), and chdir()
     invoked within the forked child process?  (Try it!)
     Try <ins>temporarily</ins> changing the code for 'cd' to use fork:
     
```c
 if (fork() == 0) {
     if (chdir(exec_argv[1]))
         /* Error: change directory failed */
         fprintf(stderr, "cd: failed to chdir %s\n", exec_argv[1]);
     exit(EXIT_SUCCESS);
 }
```
    Answer: The change of directory will occur only in the child process because fork() will create the child process, causing chdir() to only run in the child.
     
  6. Can you run multiple versions of ./b.sh in the background?
     What happens to their output? (You can run processes in the background
     by adding & at the end of the command, e.g. `./b.sh&`)

     Answer: Yes, multiple versions can be run at the same time. Their output begins to overlap, but still runs normally.

  7. Can you execute a second instance of our shell from within our shell
     program (use `./shell`)?  Which shell receives your input now?

     Answer: Yes, executing a shell within the shell is similar to a process within a process, with the second shell running the input from the user. 

  8. What happens if you type CTRL-C while the countdown script `./b.sh` is
     running?  What if `./b.sh` is running in the background?

     Answer: using CTRL-C will terminate the process and countdown immediately, similar to how destroying a parent process destroys the children aswell. If run in the background then the countdown will continute to run.

  9. Can a shell kill itself?  Can a shell within a shell kill the parent
     shell? (Run ps in the shell to see the process table)

```
 ./shell
 ./shell
 /bin/kill -s KILL NNN      (Where NNN is the the parent's PID.)
```
     Answer: Yes a shell can kill itself, a shell within a shell can also kill the parent with the PID.

  10. What happens to background processes when you exit from the shell?
      Do they continue to run?  Can you see them with the 'ps' command?
```
./shell
./b.sh&
exit
ps
```
     Answer: Background processes will continue to run and can be viewed with the ps command.

STEP 5:  Modify the Project
======================

  Please make the following modifications to the given file shell.c.  As in
  Project 0, we have included some built-in test cases, which are described along with the feature requests below.

  In addition to running the tests as listed individually, you can run
  "make test" to attempt all tests on your modified code.


  1. Modify this project so that you can use 'ls' instead of '/bin/ls'
     (i.e. the shell searches the path for the command to execute.) 
     Consider the following 4 cases:
     
     * The command includes an absolute path, such as /bin/ls
     * The command starts with a dot, such as ./shell
     * The command is somewhere in the PATH, such as ls
     * The command does not exist, in which case output "Command COMMAND_NAME does not exist" and exit the process with a failure code.
     
     HINT 1: Use C getenv() function. [https://cplusplus.com/reference/cstdlib/getenv/](https://cplusplus.com/reference/cstdlib/getenv/)<br>
     HINT 2: Use C access() function. [https://www.geeksforgeeks.org/access-command-in-linux-with-examples/](https://www.geeksforgeeks.org/access-command-in-linux-with-examples/)<br>
     **NOTE: The use of any other execv function is forbidden, you must create a function that will find any command in the system PATH if it exists.**
```
Test: ./shell -test path
Test: ./shell -test invalid_path
```
  2. Modify this project so that the command prompt includes a counter that
     increments for each command executed (starting with 0).  Your
     program should use the following prompt format:
```
"Shell(pid=%1)%2> "  %1=process pid %2=counter       

(You will need to change this into a correct printf format)
Do not increment the counter if no command is supplied to execute.
```
```
Test: ./shell -test counter
```
  3. Modify this project so that '!NN' re-executes the n'th command entered.
     You can assume that NN will only be tested with values 0 through 9. If it goes over 9, loop back to 0 (use a circular queue structure) https://www.geeksforgeeks.org/introduction-to-circular-queue/ 
```
Shell(...)0> ls
Shell(...)1> !0     # re-executes ls
Shell(...)2> !1     # re-executes ls
Shell(...)3> !3     # prints "Not valid" to stderr
Shell(...)3> !4     # prints "Not valid" to stderr
Shell(...)3> pwd
Shell(...)4> !3     # re-executes pwd
.
.
.
Shell(...)9> date
Shell(...)0> !9     # re-executes date
Shell(...)1> !0     # re-executes date
Shell(...)2> !4     # re-executes pwd
```
```
Test: ./shell -test rerun
Test: ./shell -test circular
```
  4. Create a new builtin command 'history' that displays the list of executed commands stored from step 3.
```
Shell(...)0> echo true
...
Shell(...)1> ls
...
Shell(...)2> pwd
...
Shell(...)3> date
...
Shell(...)4> history 
0. echo true
1. ls
2. pwd
3. date
4. history
```
```
Test: ./shell -test history
```

  5. Modify the project so that it uses waitpid instead of wait. 
  The parent process must wait for <ins>its direct child</ins> to finish executing before showing the next shell prompt. (Necessary for 5. and 6. to behave correctly.)

  6. Create a new builtin command 'subshell' that forks the program to create
     a new subshell.  The parent shell should run the imtheparent()
     function just as if we were running an external command (like 'ls'). <br>
     HINT: What does fork() do? What program is the forked process running? The use of execv() or main() is forbidden for this step, you must find a solution with only fork(). <br> NOTE: The new subshell should start from scratch, that is the counter goes back to 0, the history of commands gets cleared, and the pid of the newshell will change to the pid of the new process.
```
 ./shell
 Shell(.n1..)0> subshell
 Shell(.n2..)0> exit  # Exits sub shell
 Shell(.n1..)0> exit  # Exits back to 'real' shell
```
  7. Create a new global variable to prevent a subshell from invoking
     a subshell invoking a subshell (i.e., more than 3 levels deep):
```
./shell
Shell(.n1..)0> subshell
Shell(.n2..)0> subshell
Shell(.n3..)0> subshell   # prints "Too deep!" to stderr
```
```
Test: ./shell -test subshell
```

The project (Step 5) will be graded on:
* correctness (60%),
* organization (10%),
* robustness (10%),
* formatting (10%), and
* documentation(10%).

Don't forget to release memory stored in pointers, output to the correct streams, and close all files/streams before the last shell terminates. All work submitted must be yours and you must be ready to defend your program if asked to do so in class or in office. Do not copy solutions from the web, I will ask you how the program works!
