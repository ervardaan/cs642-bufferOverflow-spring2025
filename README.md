[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/_HnzoEU_)
# Project Buffer Overflow - COMP SCI 642: Introduction to Information Security - Fall 2024 - Prof. McDaniel

## Due date: See Canvas

## Description

In this project, you will produce overflow and code-reuse attacks. First, you
learn some attacks that invoke shared library functions with arguments obtained
from different places in memory (injected by you, or from environment variables,
or from the hard coded strings in the code). You will also write a Return
Oriented Programming (ROP) attack that combines shared library functions and ROP
gadgets from the executable code to invoke more powerful and robust attacks. The
overflow attacks will include stack overflow and heap overflow. We will use
available tool (`ROPgadget`) to extract gadgets from the executable. Successful
completion of this project relies on correct understanding of stacks, heaps,
program memory layout and a function’s stack frame.

Please follow the instructions carefully and turn in the results as directed
before the deadline.

## Dependencies and cloning instructions

> <details><summary>x86 Docker image (cross-compilation) </summary>
>
> - Install [Docker](https://docs.docker.com/engine/install/) and
>   [git](https://git-scm.com/downloads) on your machine. If you are a macOS
>   user, you should also authorize Docker to access your files, see this
>   [guide](https://support.apple.com/en-gb/guide/mac-help/mchld5a35146/mac).
>
> - Configure git on your machine and optionally [add a SSH key to your GitHub
>   account](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/about-ssh):
>     ```sh
>     git config --global user.name "Bucky Badger"
>     git config --global user.email uw-bucky-badger@wisc.edu
>     ```
>
> - Accept the GitHub Classroom link provided on the Canvas Assignment, a
>   private GitHub repository is created for you, clone it on your machine (you
>   can find the HTTPS or SSH url by clicking on the green button named *Code*):
>
>     `git clone
>     <HTTPS_OR_SSH_URL>UW-Madison-COMPSCI642/bufferoverflow-<YOUR_GITHUB_USERNAME>.git`
>
> - A `Dockerfile` is provided under `.devcontainer/` (for direct integration
> with VS Code). Using VS Code with Docker and VS Code Dev Containers extension
> as described on [this
> guide](https://gist.github.com/yohhaan/b492e165b77a84d9f8299038d21ae2c9) will
> likely be the easiest for you. If you have issues with sharing the git
> credentials with your Docker container, refer to this
> [documentation](https://code.visualstudio.com/remote/advancedcontainers/sharing-git-credentials).
>
> - In case, you would like to manually build the image and deploy the Docker
> container to test your code (if you are not using VS Code but another
> development workflow), follow the instructions below:
>
>   1. Build the Docker image (needs to be done only once normally):
>     ```sh
>     docker build --platform linux/amd64 -t cs642-bufferoverflow-docker-image .devcontainer/
>     ```
>
>   2. Every time you want to test your code and if you have exited the
>      container you had previously created, you will have to deploy a new
>      Docker container:
>     ```sh
>     docker run --platform linux/amd64 --security-opt seccomp=unconfined --rm -it \
>         -v ${PWD}:/workspace/bufferoverflow \
>         -v ${HOME}/.gitconfig:/home/vscode/.gitconfig \
>         -v ${HOME}/.ssh:/home/vscode/.ssh \
>         -w /workspace/bufferoverflow \
>         --entrypoint bash cs642-bufferoverflow-docker-image:latest
>     ```
>     Note: you may have to edit the source path for the `.gitconfig` and `.ssh`
>     volumes (for now it looks for those in your home directory on your
>     machine). These 2 volumes are needed so that your git configurations and
>     potential ssh keys are accessible from within the Docker container,
>     respectively.
>
> - You are ready to start on your project! It is highly recommended to keep
>   track of your modifications often by committing and pushing your changes to
>   your private repository.
> </details>
> 
> <details><summary> Note for M1/M2/M3 MAC Users </summary>
> Normally when we execute a binary with arguments, we launch it with the 
> following command:
>
> ```
> ./binary arg1 arg2 ...
> ```
>
> And if we want to debug it with `gdb` in the same way the binary was executed
> above, we would use the following command:
>
> ```
> gdb ./binary
> (gdb) <set breakpoints, etc.>
> (gdb) run arg1 arg2 ...
> ```
>
> However, for this project, we are building docker environment on amd64. So
> when you run the binary with arguments in the docker container, you should use
> the following command if you have a M1/M2/M3 Mac laptop:
>
> ```
> ROSETTA_DEBUGSERVER_PORT=1234 ./binary arg1 arg2 ... & gdb
> (gdb) set architecture i386:x86-64
> (gdb) file ./binary
> (gdb) target remote localhost:1234
> (gdb) <set breakpoints, etc.>
> (gdb) continue
> ```
>
> </details>

## Background

> <details><summary>Return-Oriented Programming.</summary>
>
> This [paper](https://doi.org/10.1145/2133375.2133377) by Roemer et al.
> describes the principles and capabilities of return-oriented programming. The
> critical feature of return-oriented programming is that once adversaries gain
> control of the stack pointer and/or the memory around the stack pointer (and
> if the binary has enough code), then the adversary has a Turing-complete
> environment from which to craft exploits.
> </details>
>
> <details><summary>ROPgadget</summary>
>
> [ROPgadget](https://github.com/JonathanSalwan/ROPgadget) is an open-source
> tool for extracting gadgets from binaries. For your convenience, ROPgadget has
> already been configured in the provided Dockerfile. The basic usage is below,
> but there are several options. We can generate gadgets from any binary, but we
> will use gadgets from the executable (victim).
> ```
> ROPgadget --binary task-victim-binary > gadgets
> ```
> The result is a collection of the possible gadgets available in the
> executable’s code.
> </details>
>
> <details><summary>Procedure Linkage Table (PLT)</summary>
>
> The PLT also provides some useful options for launching ROP attacks. The PLT
> provides stub code for invoking the library calls used by the executable.
> Since library code does useful things, such as invoking system calls, invoking
> this code via the PLT is often desirable. You can view the PLT stub code by
> disassembling the executable.
> ```
> objdump -dl task-victim-binary | less
> ```
> You can then search for “plt” to locate the stub code for a variety of library
> calls from the executable code. We will use a variety of PLT functions. You
> should also look at the man page of `objdump`. Certain options like `-R` might
> make it easier to locate appropriate addresses.
> </details>
>
> <details><summary>Learning about ROP Attacks (video)</summary>
>
> This [video](https://www.youtube.com/watch?v=5FJxC59hMRY) provides background
> and demonstrates examples for building ROP exploits. The video is one hour
> long, but the first 30 minutes or so are on ROP background. The second part is
> a useful hands-on information for launching ROP attacks. This video
> demonstrates how to find and invoke library functions via the PLT, which is
> fundamental to our approach.
>
> Note that program input functions are sensitive to some byte values. A
> zero-byte will terminate the read (`fscanf`). However, other byte values such
> as 8 and 15 may cause `fscanf` to terminate. Fortunately, you should not need
> these in the way the project is formulated.
> </details>
>
> <details><summary>Running PLT Payloads</summary>
>
> You can use two types of payloads use in this project: PLT calls and ROP
> gadgets. Invoking each is slightly different. Launching a PLT stub essentially
> calls the library function. Therefore, to invoke any PLT stub you will specify
> the PLT stub address on the stack at the return address. You will also have to
> build the rest of the stack as the compiler work for a function call. Above
> the PLT stub address will be the address of the instruction to run when this
> function returns (i.e., the return address for the PLT stub) and then the
> arguments to the targeted function (in order from first to last). Arguments
> may be values or pointers to data values. Our attacks will mainly need
> pointers placed on the stack.
>
> This is the format of the stack for any function call. When the stack pointer
> points to the PLT stub address on a return instruction, the PLT stub function
> will be invoked and run to completion using the arguments above the return
> address of the PLT stub, then the code referenced by the return address will
> be run. The video linked above has several examples of this to help you.
>
> To choose the PLT functions to use, use `objdump` to find the PLT stub
> addresses as described above and place those addresses on the stack.
> </details>
>
> <details><summary>Choosing and Running Gadgets</summary>
>
> In this project, you can use gadgets to remove arguments to PLT stubs from the
> stack to call the next PLT stub. The pop-ret gadgets are also discussed in the
> video linked above. See this
> [page](https://htmlpreview.github.io/?https://raw.githubusercontent.com/pubmirror/x86.renejeschke.de/master/html/file_module_x86_id_248.html)
> for information on `pop`. This
> [website](https://htmlpreview.github.io/?https://github.com/pubmirror/x86.renejeschke.de/blob/master/index.html)
> has specs for most x86 instructions, but we will only depend on a few.
>
> To launch a gadget, the return address on the stack should be assigned the
> address of the gadget (first instruction of the gadget). Gadgets may use
> values on the stack as well in order. When a gadget returns, whatever is
> present at the current stack pointer will be executed next.
>
> Once you have determined which gadgets you want to use, a challenge is to
> invoke them. `ROPgadget` provides the address of the gadgets in the victim
> executable, because the victim is loaded at the expected address, these gadget
> addresses may be used directly.
> </details>
>
> <details><summary>Crafting Exploits</summary>
>
> Crafting ROP exploits is a non-trivial exercise, requiring an understanding of
> the memory layout of the program, particularly the stack. Key to understanding
> memory layout will be use of the debugger (`gdb`).
>
> Please pay close attention to the commands used in the debugger, as you will
> want to utilize the same commands to create a split layout showing the program
> and the assembly view (layout split), step one instruction at a time (`si`),
> and print the stack. Other useful `gdb` commands are `print` for displaying
> the values in memory (`p var` to print the value of variable `var`) and `info
> register` to print the values of registers, such as the stack pointer in `rsp`
> (`i r rsp`), and `x/16gx $rsp` to print 16 entries with each entry being 8 
> bytes from the `rsp` address. See
> this [GDB quick
> reference](https://users.ece.utexas.edu/~adnan/gdb-refcard.pdf) for more
> information. The idea is that you want to overwrite the return address on the
> stack with the address of the first instruction (gadget or PLT stub) that you
> want to run. Then, you prepare the rest of the stack with the arguments and
> instructions to run when your gadgets complete.
> </details>

## Project Details

This project consists of two groups of files:

1. The victim programs `cs642-victim-task12` and `cs642-victim-task3`
(source code files available for reference) along with
their respective binaries `task12` and `task3`. You do not need to
rebuild these binaries or modify their source files for any reason: you should
learn how to accomplish each task on the binaries provided with the starter
code.

2. The attack programs that produce buffers to attack return addresses
`cs642-task1.c`, `cs642-task2.c`, and `cs642-task3.c` and
supporting code. `make all` should build attack programs corresponding to each
attack goal.

> For task 1 and task 2 you have access to a not Jedi school training program.
> Your first task is to print your name below a lightsaber id (which determines
> the color of the lightsaber). In second task you have two goals, to get four
> lightsabers under one name and use the force to get access to a new shell. For
> task 3 you have access to a program that can open the chamber of
> secrets. You use overflow techniques to invoke the right function that will
> open the door for you.
>
> <details><summary>Task 1</summary>
>
> Write the program `cs642-task1.c` to build a payload `task1-payload` so that
> it prints the string of your name like `Name:<YourFirstName_YourLastName>!`.
> This string will be part of your payload. Your payload should consist of four
> parts.
> - First, you should encode the address of the `printf` stub from the PLT at
>   the return address.
> - Second, when `printf` returns, the program should exit. There is a PLT stub
>   for this too.
> - Third, `printf` should be given an argument that is the address of the
>   hard-coded string for `Name:<YourFirstName_YourLastName>!`. You have to
>   figure out where the string can be stored.
> - Fourth, you need to add the string value
> `Name:<YourFirstName_YourLastName>!` to wherever you decided to store the
> string.
>
> You will need to determine how far the return address is from the beginning of
> the input domain buffer you want to overflow. You are requested to fill the
> space up to the return address with `A`s (It will be easier for you to debug
> if you know the hex value of `A`). Once you have all the four parts ready, and
> you know the buffer overflow value, you can start to construct the payload.
> The program includes a C macro pack that you can use to add 8-byte values
> (addresses of instructions and arguments) into the payload.
>
> Since `Name:<YourFirstName_YourLastName>!` is not an address you can use
> `memcpy` to add this to the payload. The function `write_to_file` is available
> to write the payload to a file. It takes 4 arguments:
> 1. Name of the file
> 2. Buffer to write to file
> 3. Size of the buffer in bytes
> 4. Whether to clear the file before writing or whether to append the buffer to
> the existing file.
>
> Run your crafted payload against the victim program, the output should look
> like below (may not be exactly the same, but it is important that
> `Name:<YourFirstName_YourLastName>!` appears).
>
> ```
> > ./task12
> Welcome to not a Jedi Academy for CS642
>
> <<<<<<<<<CS 642 Project 2>>>>>>>>>
> Oops! A stack address leak!
> &*Y&Q*(@*&): 0xdeadbeef
>
> Please enter the file name: task1-payload
>
> Your Jedi Name:
> This is your lightsaber ID := 601
> Name:Bucky_Badger!
> ```
>
> </details>
> <details><summary>Task 2</summary>
>
> In this task, you have two goals (1) collect more lightsabers by printing not
> just one, but four lightsaber ID under one Jedi name and (2) get access to
> launch new shell (`/bin/sh`) using the functions `get_this` and
> `and_get_that`. You have to achieve (1) by writing a program in the file
> `cs642-task2.c` to build a payload to print `This is your lightsaber ID = xxx`
> four times on your terminal with different lightsaber IDs. After packing your
> payload by enough `A`s to start overflow you can give the address of the
> function that generates lightsaber IDS as return address. Hint: make sure to
> keep your stack aligned. Do it 3 more times. For (2) You need to overwrite the
> return address and redirect the execution to `get_this` and `and_get_that`
> functions, respectively. The main challenge is that the `get_this` and
> `and_get_that` functions have arguments. You will, therefore, need to use
> gadgets in the task to clear (`pop`) the arguments from `get_this` to
> `and_get_that` or the exit stub to set the stack pointer correctly for the
> next function/stub. The video linked above demonstrates this as well. The
> output with command line should look like below (may not be exactly the same).
>
> ```
> > ./task12
> Welcome to not a Jedi Academy for CS642
>
> <<<<<<<<<CS 642 Project 2>>>>>>>>>
> Oops! A stack address leak!
> &*Y&Q*(@*&): 0xdeadbeef
>
> Please enter the file name: task2-payload
>
> Your Jedi Name:
> This is your lightsaber ID := 13796
>
> This is your lightsaber ID := 8041
>
> This is your lightsaber ID := 5996
>
> This is your lightsaber ID := 17551
> Hello There! General Kenobi. You got the shell.
> $
> ```
>
> </details>
> <details><summary>Task 3</summary>
>
> In this task you will create a payload to open the chambers of secret via
> function `parseltongue`. To achieve this you will have to overflow the heap
> instead of the stack. All you need to do is first identify how many `A`s you
> have to use to overflow the buffer so that `f->fp` points to `parseltongue`.
> It would be helpful for debugging if you can use break points in `gdb` and
> look at what is being stored at any given address. Commands like `info proc
> mappings`, `x/<integer>gx <address>` can be very handy. Use the man pages to
> know more about these commands and others. The output with command line should
> look like below (you can switch to `sh` to avoid any warning due to `bash`'s
> command substitution).
>
> ```
> > sh
> $ ./task3 `cat task3-payload`
> Enter
> sss Open! sss
> The chamber is now open
> The end
> $
> ```
>
> </details>

## Code and report

Complete the C files `cs642-task{1-2-3}.c` with the code to create the
different payloads for each attack and complete the `report.md` file with your
answers.


## How to turn in

1. Commit and push your changes to the private repository created by GitHub
   Classroom before the deadline.

2. **And** submit on Canvas before the deadline both your *GitHub username* and
   the *commit ID* that should be graded. Otherwise, you will receive a 0 for
   the assignment. Note that the TA will grade your repository as available at:
   `https://github.com/UW-Madison-COMPSCI642/bufferoverflow-<YOUR_GITHUB_USERNAME>/commit/<COMMIT_ID_TO_GRADE>`

**Tip:** you can test that the TA will obtain the correct version of your code
and that they will be able to compile it by:

- Cloning again your GitHub repository into *another* location on your machine
  or VM.

    `git clone
    <HTTPS_OR_SSH_URL>UW-Madison-COMPSCI642/bufferoverflow-<YOUR_GITHUB_USERNAME>.git`

- Checking out to the commit ID you would like to be graded.

    `git checkout <COMMIT_ID_TO_GRADE>`

- Compiling your code and testing that everything works as expected.

    `make all` and verify that the attacks still work

## Note

**Like all assignments in this class you are prohibited from copying any content
from the Internet or discussing, sharing ideas, code, configuration, text or
anything else or getting help from anyone in or outside of the class. Consulting
online sources is acceptable, but under no circumstances should *anything* be
copied. Failure to abide by this requirement will result dismissal from the
class as described in the course syllabus on Canvas.**
