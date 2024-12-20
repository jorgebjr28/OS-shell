/*************** YOU SHOULD NOT MODIFY ANYTHING IN THIS FILE ***************/

#define _GNU_SOURCE
#include <stdio.h>
#undef _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "testrunner.h"
#include "smp1_tests.h"

#define quit_if(cond) do {if (cond) exit(EXIT_FAILURE);} while(0)

/* Prepare input, reroute file descriptors, and run the program. */
void run_test(const char *input, int argc, char **argv)
{
	/* Prepare input */
	FILE *in = fopen("smp1.in", "w");
	fprintf(in, "%s", input);
	fclose(in);
	/* Reroute standard file descriptors */
	freopen("smp1.in",  "r", stdin );
	freopen("smp1.out", "w", stdout);
	freopen("smp1.err", "w", stderr);
	/* Run the program */
	quit_if(main(argc, argv) != EXIT_SUCCESS);
	fclose(stdout);
	fclose(stderr);
}

/* P5.1: Test of executing commands in the path */
// What this test does:
// Program call: "./shell"

    /* Input:
		ls
		/bin/ls
		exit
    */

    /* Expected output: 
		...
		Parent says 'child process has been forked with pid=1000
		** the output of ls **
		Parent says 'wait() returned so the child with pid=1000
		...
		Parent says 'child process has been forked with pid=1001
		** the output of /bin/ls **
		Parent says 'wait() returned so the child with pid=1001 is finished.
    */
int test_path(int argc, char **argv)
{
	// testing forbidden function
	quit_if(!system("grep execvp shell.c > /dev/null")); // Do not use execvp

	char *args[] = { "./shell", NULL };
	FILE *out, *err;
	int pid_tmp;
	/* Run the test */
	run_test("ls\n/bin/ls\nexit\n", 1, args);
	/* Check output */
	err = fopen("smp1.err", "r");
	quit_if(fscanf(err, "  Parent says 'child process has been forked with pid=%d'\n"
	                    "  Parent says 'wait() returned so the child with pid=%d is finished.'\n"
	                    "  Parent says 'child process has been forked with pid=%d'\n"
	                    "  Parent says 'wait() returned so the child with pid=%d is finished.'\n",
	               &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp) != 4);
	quit_if(!feof(err));
	fclose(err);
	return EXIT_SUCCESS;
}

/* P5.1: Test of executing commands in the path */
// What this test does:
// Program call: "./shell"

    /* Input:
		wrong
		exit
    */

    /* Expected output: 
		...
		Command wrong does not exist
		...
    */
int test_invalid_path(int argc, char **argv){
	char *args[] = { "./shell", NULL };
	FILE *out, *err;
	int invalid_command_message;
	/* Run the test */
	run_test("wrong\nexit\n", 1, args);
	/* Check output */
	err = fopen("smp1.err", "r");

	/* Next, check for the "Invalid *command* " message: */
	invalid_command_message = 0;
	/* Use a while loop because multiple processes write to stderr concurrently. */
	while (!invalid_command_message && !feof(err)) {
		char error_message[30];
		fgets(error_message, 30, err);
		fprintf(stdout, "%s\n", error_message);
		if (!strncmp(error_message, "Command wrong does not exist\n", 29))
			invalid_command_message = 1;
	}
	quit_if(!invalid_command_message);
	fclose(err);
	return EXIT_SUCCESS;
}

/* P5.2: Test of command line counter */
// What this test does:
// Program call: "./shell"

    /* Input:
		
		/bin/true
		exit
    */

    /* Expected output: 
		Shell(pid=1000)0>
		...
		Shell(pid=1000)0>
		...
		Shell(pid=1000)1>
		Exit process 1000
    */
int test_counter(int argc, char **argv)
{
	char *args[] = { "./shell", NULL };
	FILE *out, *err;
	int pid_tmp;
	/* Run the test */
	run_test("\n/bin/true\nexit\n", 1, args);
	/* Check output */
	out = fopen("smp1.out", "r");
	quit_if(fscanf(out, "Shell(pid=%d)0> Shell(pid=%d)0> Shell(pid=%d)1> Exiting process %d\n", &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp) != 4);
	quit_if(!feof(out));
	fclose(out);
	return EXIT_SUCCESS;
}

/* P5.2: Test of command line counter */
// What this test does:
// Program call: "./shell"

    /* Input:
		/bin/true (10 times)
		exit
    */

    /* Expected output: 
		Shell(pid=1000)0>
		...
		Shell(pid=1000)1>
		...
		Shell(pid=1000)2>
		...
		Shell(pid=1000)3>
		...
		Shell(pid=1000)4>
		...
		Shell(pid=1000)5>
		...
		Shell(pid=1000)6>
		...
		Shell(pid=1000)7>
		...
		Shell(pid=1000)8>
		...
		Shell(pid=1000)9>
		...
		Shell(pid=1000)0>
		Exit process 1000
    */
int test_circular(int argc, char **argv)
{
	char *args[] = { "./shell", NULL };
	FILE *out, *err;
	int pid_tmp;
	/* Run the test */
	run_test("/bin/true\n/bin/true\n/bin/true\n/bin/true\n/bin/true\n/bin/true\n/bin/true\n/bin/true\n/bin/true\n/bin/true\nexit\n", 1, args);
	/* Check output */
	out = fopen("smp1.out", "r");
	quit_if(fscanf(out, "Shell(pid=%d)0> Shell(pid=%d)1> Shell(pid=%d)2> Shell(pid=%d)3> Shell(pid=%d)4> Shell(pid=%d)5> Shell(pid=%d)6> Shell(pid=%d)7> Shell(pid=%d)8> Shell(pid=%d)9> Shell(pid=%d)0> Exiting process %d\n", &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp) != 12);
	quit_if(!feof(out));
	fclose(out);
	return EXIT_SUCCESS;
}

/* P5.3: Test of re-executing earlier commands */
// What this test does:
// Program call: "./shell"

    /* Input:
		/bin/echo test
		!1
		exit
    */

    /* Expected output: 
		Shell(pid=1000)1>
		...
		test
		...
		Shell(pid=1000)2>
		...
		test
		...
		Shell(pid=1000)3>
		Exiting process 1000
    */
int test_rerun(int argc, char **argv)
{
	char *args[] = { "./shell", NULL };
	FILE *out, *err;
	int pid_tmp, warned_not_valid;
	/* Run the test */
	run_test("/bin/echo test\n!0\n!3\nexit\n", 1, args);
	/* Check output */
	out = fopen("smp1.out", "r");
	err = fopen("smp1.err", "r");
	quit_if(fscanf(out, "Shell(pid=%d)0> test\nShell(pid=%d)1> test\nShell(pid=%d)2> Shell(pid=%d)2> Exiting process %d\n", &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp) != 5);
	/* Next, check for the "Not valid" message: */
	warned_not_valid = 0;
	/* Use a while loop because multiple processes write to stderr concurrently. */
	while (!warned_not_valid && !feof(err)) {
		char not_valid[11];
		fgets(not_valid, 11, err);
		if (!strncmp(not_valid, "Not valid\n", 10))
			warned_not_valid = 1;
	}
	quit_if(!warned_not_valid);
	quit_if(!feof(out));
	fclose(out);
	return EXIT_SUCCESS;
}

/* P5.4: Test of history command */
// What this test does:
// Program call: "./shell"

    /* Input:
		/bin/echo test
		/bin/ls
		/bin/pwd
		history
		exit
    */

    /* Expected output: 
		1. echo test
		2. echo test
		3. echo test
		4. history
    */

/* Input:
		echo test
		echo test
		echo test
		history
		exit
    */

    /* Expected output: 
		0. echo test
		1. echo test
		2. echo test
		3. history
    */
int test_history(int argc, char **argv)
{
	char *args[] = { "./shell", NULL };
	FILE *out, *err;
	int pid_tmp, warned_not_valid;
	/* Run the test */
	run_test("echo test\necho test\necho test\nhistory\nexit\n", 1, args);
	/* Check output */
	out = fopen("smp1.out", "r");
	err = fopen("smp1.err", "r");
	quit_if(fscanf(out, "Shell(pid=%d)0> test\nShell(pid=%d)1> test\nShell(pid=%d)2> test\nShell(pid=%d)3> 0. echo test\n1. echo test\n2. echo test\n3. history\nShell(pid=%d)4> Exiting process %d\n", &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp, &pid_tmp) != 6);
	fclose(out);
	return EXIT_SUCCESS;
}

/* P5.6: Test of depth-limited sub */
// What this test does:
// Program call: "./shell"

    /* Input:
		subshell
		subshell
		subshell
		exit
		exit
		exit
    */

    /* Expected output: 
		Shell(pid=1000)1>
		...
		Shell(pid=1001)1>
		...
		Shell(pid=1002)1>
		Too deep!
		Shell(pid=1002)2>
		...
    */
int test_newshell(int argc, char **argv)
{
	char *args[] = { "./shell", NULL };
	FILE *out, *err;
	int pids[4], warned_too_deep;
	/* Run the test */
	run_test("subshell\nsubshell\nsubshell\nexit\nexit\nexit\n", 1, args);
	/* Check output */
	out = fopen("smp1.out", "r");
	err = fopen("smp1.err", "r");
	/* First, check that the newshells were invoked. */
	fscanf(out, "Shell(pid=%d)0> Shell(pid=%d)0> Shell(pid=%d)0> Shell(pid=%d)1> ", &pids[0], &pids[1], &pids[2], &pids[3]);
	//fscanf(out, "Shell(pid=%d)1> Shell(pid=%d)1> Shell(pid=%d)1> Shell(pid=%d)2> ", &pids[0], &pids[1], &pids[2], &pids[3]);
	quit_if(!((pids[0] != pids[1]) && (pids[1] != pids[2]) && (pids[0] != pids[2]) && (pids[2] == pids[3])));
	/* Next, check for the "Too deep!" message: */
	warned_too_deep = 0;
	/* Use a while loop because multiple processes write to stderr concurrently. */
	while (!warned_too_deep && !feof(err)) {
		char too_deep[11];
		fgets(too_deep, 11, err);
		if (!strncmp(too_deep, "Too deep!\n", 10))
			warned_too_deep = 1;
	}
	quit_if(!warned_too_deep);
	fclose(out);
	fclose(err);
	return EXIT_SUCCESS;
}

/*
 * Main entry point for SMP1 test harness
 */
int run_smp1_tests(int argc, char **argv)
{
	/* Tests can be invoked by matching their name or their suite name 
	   or 'all' */
	testentry_t tests[] = {
		{ "path",    "smp1", test_path    },
		{ "invalid_path", "smp1", test_invalid_path},
		{ "counter", "smp1", test_counter },
		{ "rerun",   "smp1", test_rerun   },
		{ "circular", "smp1", test_circular },
		{ "history", "smp1", test_history },
		{ "subshell",     "smp1", test_newshell  }
		 };
	int result = run_testrunner(argc, argv, tests, sizeof(tests) / sizeof (testentry_t));
	unlink("smp1.in");
	unlink("smp1.out");
	unlink("smp1.err");
	return result;
}
