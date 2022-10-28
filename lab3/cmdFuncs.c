#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/user.h>

#include "cmdFuncs.h"
#include "ast.h"

// store symbol's name and value
void bfd_func(char *file){
    long storage_needed;
    bfd *abfd;

    bfd_init(); // magic

    abfd = bfd_openr(file, NULL);
    assert(abfd != NULL);
    bfd_check_format(abfd, bfd_object);
    storage_needed = bfd_get_symtab_upper_bound(abfd);
    assert(storage_needed >= 0);

    symbol_table = (asymbol**)malloc(storage_needed);
    assert(symbol_table != 0);
    num_symbols = bfd_canonicalize_symtab(abfd, symbol_table);
    assert(num_symbols >= 0);
}

// find symbol address
long symbol_address(char *s){
    symbol_info symbolinfo;
    for(int i = 0; i < num_symbols; i++){
        if (symbol_table[i]->section == NULL) continue;

        bfd_symbol_info(symbol_table[i], &symbolinfo);
        if (strcmp(s, symbolinfo.name))  continue;

        printf("%s : 0x%x\n", symbolinfo.name, symbolinfo.value);
        return (long)symbolinfo.value;
    }
    return -1;
}

// A tedious but simple parsing function, parsing
// user input string into a command data structure.
 Cmd_t
parseCommand (char *s, int pid){
    char temp[1024];
    int i = 0;
    if (!s || !*s)
        return 0;
    // first get the command name, store it into "temp"
    while ((temp[i++]=*s++))
        if (*s==' ' || *s=='\0')
            break;
    temp[i] = '\0';
    s++;    // jump the blank

    if (0 == strcmp (temp, "b")){
        long addr;
        if (!*(s)){
            printf ("break requires an address\n");
            return 0;
        }
        if(s[0] != '0') addr = symbol_address(s);
        else addr = strtol (s, 0, 0);
        return Cmd_new_break (addr, pid);
    }

    else if (0 == strcmp (temp, "c"))
        return Cmd_new_cont (pid);

    else if (0 == strcmp (temp, "file")){
        if (!*s){
            printf ("file requires a file name\n");
            return 0;
        }
        return Cmd_new_file (s, pid);
    }

    else if (0 == strcmp (temp, "q"))
        return Cmd_new_quit (pid);

    else if (0 == strcmp (temp, "r"))
        return Cmd_new_run (pid);

    else if (0 == strcmp (temp, "regs"))
        return Cmd_new_regs (pid);

    else if (0 == strcmp (temp, "si"))
        return Cmd_new_si (pid);

    else if (0 == strcmp (temp, "x/i")){
        long addr;
        if (!*(s)){
            printf ("x/i requires an address\n");
            return 0;
        }
        if(s[0] != '0') addr = symbol_address(s);
        else addr = strtol (s, 0, 0);
        return Cmd_new_xi (addr, pid);
    }

    else if (0 == strcmp (temp, "x/x")){
        long addr;
        if (!*(s)){
            printf ("x/x requires an address\n");
            return 0;
        }
        if(s[0] != '0') addr = symbol_address(s);
        else addr = strtol (s, 0, 0);
        return Cmd_new_xx (addr, pid);
    }
    else;

    printf ("Unknown command: %s\n", temp);
    return 0;
}

// Given a command and execute it.
 void
execCommand (Cmd_t c){
    switch (c->kind){
        /*
         * Exercise 2 :
         * Now, how the breaking point command b 0xaddr is implemented?
         * In fact, there is a serious bug in current implementation we offered you.
         * Let's check where is this bug. First run this
         *      objdump -d hello
         * and figure out the address of the function print,
         * suppose that address is 0xaddr on your machine.
         * Now in the ssedb, you set up a break point on address 0xaddr
         * by typing (remember this address must be in hexadecimal, which has a leading 0x):
         *      (ssedb) b 0xaddr
         * now peek the registers:
         *      (ssedb) regs
         * What's the value of rip? Is this value right? Why?
         * And then disassembly the content at address 0xaddr.
         *      (ssedb) x/x 0xaddr
         * What's there? Have you detected the bug? How to fix this bug?
         */

        case CMD_KIND_BREAK:{
            /*
             * Todo("Exercise 2: Find out the bug in this case and fix it, submit your source code.")
             */
            unsigned long brk = 0x00000000000000cc;
            unsigned long data;
            unsigned long back;     // copy the origin data

            data = ptrace(PTRACE_PEEKDATA, c->pid, c->u.addr, 0);
            back = data;
            printf ("init data     = %lx\n", data);
            data = (data & 0xffffffffffffff00) | brk;
            printf ("revised data  = %lx\n", data);
            ptrace(PTRACE_POKEDATA, c->pid, c->u.addr, data);
            // will stop in (int 3)
            ptrace(PTRACE_CONT, c->pid, 0, 0);
            waitChild ();

            // back to origin rip
            struct user_regs_struct regs;
            ptrace (PTRACE_GETREGS, c->pid, 0, &regs);
            regs.rip--;
            ptrace (PTRACE_SETREGS, c->pid, 0, &regs);
            // write back the origin data
            ptrace(PTRACE_POKEDATA, c->pid, c->u.addr, back);

            return;
        }
        case CMD_KIND_CONT:{
            ptrace(PTRACE_CONT, c->pid, 0, 0);
            waitChild ();
            return;
        }
        case CMD_KIND_FILE:{
            kill (c->pid, SIGKILL);
            int pid = fork ();
            if (0 == pid){
                ptrace (PTRACE_TRACEME, 0, 0, 0);
                execve (c->u.filename, &c->u.filename, 0);
            }
            else{
                waitChild ();
                printf ("loading: %s (pid: %d)\n", c->u.filename, pid);
                return;
            }
            return;
        }
        case CMD_KIND_QUIT:{
            L:
            printf ("ssedb is quiting. Are you sure? (yes or no) ");
            int ch = getchar ();
            if ('y' == ch || 'Y' == ch){
                kill (c->pid, SIGKILL);
                exit (0);
            }
            else if ('n' == ch || 'N' == ch){
                getchar ();
                return;
            }
            else {
                printf("\n");
                goto L;
            }
            return ;
        }
        case CMD_KIND_REGS:{
            struct user_regs_struct regs;
            ptrace (PTRACE_GETREGS, c->pid, 0, &regs);
            printf ("rax = %lld\n"
                    "rbx = %lld\n"
                    "rcx = %lld\n"
                    "rdx = %lld\n"
                    "rsi = %lld\n"
                    "rdi = %lld\n"
                    "rsp = %p\n"
                    "rbp = %p\n"
                    "rip = %p\n",
                    regs.rax,
                    regs.rbx,
                    regs.rcx,
                    regs.rdx,
                    regs.rsi,
                    regs.rdi,
                    (char *)regs.rsp,
                    (char *)regs.rbp,
                    (char *)regs.rip);
            return;
        }
        case CMD_KIND_RUN:{
            ptrace(PTRACE_DETACH, c->pid, 0, 0);
            waitChild ();
            return;
        }
        case CMD_KIND_SI:{
            ptrace(PTRACE_SINGLESTEP, c->pid, 0, 0);
            return;
        }
        case CMD_KIND_XI:{
            /*
             * Exercise 3 :
             * There is also a command to disassembly bianry into assembly intructions,
             * but has not be completed. Now run
             *      (ssedb) x/i 0xaddr
             * you'll see an error message indicating the file position you should supply code.
             * Implement it. (Hint: manual disassemblying is tedious and error-prone,
             * so you may find some libraries are helpful, such as the zydis.)
             */

            /*
             * Todo("Exercise 3: Supply your code and Implement it.")
             */

            ZyanU8 arr[80];
            long addr = c->u.addr;
            for (int i = 0; i < 10; i++){
                unsigned long data = ptrace(PTRACE_PEEKDATA, c->pid, addr, 0);
                for(int j = 0; j < 8; j++){
                    arr[8 * i + j] = data & 0x00000000000000ff;
                    data = ((data & 0xffffffffffffff00) >> 8);
                }
                addr += 8;
            }

            ZyanU64 runtime_address = c->u.addr;
            ZyanUSize offset = 0;
            ZydisDisassembledInstruction instruction;

            while (ZYAN_SUCCESS(ZydisDisassembleIntel(
                    /* machine_mode:    */ ZYDIS_MACHINE_MODE_LONG_64,
                    /* runtime_address: */ runtime_address,
                    /* buffer:          */ arr + offset,
                    /* length:          */ sizeof(arr) - offset,
                    /* instruction:     */ &instruction
            ))) {
                printf("%016" PRIX64 "  %s\n", runtime_address, instruction.text);
                offset += instruction.info.length;
                runtime_address += instruction.info.length;
            }
            return;
        }
        case CMD_KIND_XX:{
            // By default, we disassemble 40 bytes
            int i;
            long addr = c->u.addr;

            for (i = 0; i < 10; i++){
                long data = ptrace(PTRACE_PEEKDATA, c->pid, addr, 0);
                printf ("%lx: %016lx\n", addr, data);
                addr += 8;
            }
            return;
        }
        default:
            printf ("Unknown command: ssedb bug!\n");
            return;
    }
}

 void
loopCommand (int pid){
    // buffer to hold user input, hopefully,
    // 1024 seems to be large enough.
    char buf[1024];

    while (1){
        printf ("(ssedb) ");
        gets(buf);

        Cmd_t c = parseCommand (buf, pid);
        // not a valid command, re-read
        if (!c)
            continue;

        // execute a valid command
        execCommand (c);
    }
}