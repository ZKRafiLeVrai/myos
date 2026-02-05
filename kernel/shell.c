#include "shell.h"
#include "string.h"
#include "printk.h"
#include "vga.h"
#include "vfs.h"

// Parse "command arg1 arg2 ..." into cmd_args_t
cmd_args_t shell_parse_command(char* input) {
    cmd_args_t args = {0};
    int arg_idx = 0;
    int char_idx = 0;
    int in_arg = 0;

    for (int i = 0; input[i] != '\0' && arg_idx < MAX_ARGS; i++) {
        char c = input[i];
        
        if (c == ' ' || c == '\t') {
            if (in_arg) {
                args.argv[arg_idx][char_idx] = '\0';
                arg_idx++;
                char_idx = 0;
                in_arg = 0;
            }
        } else {
            if (char_idx < MAX_ARG_LEN - 1) {
                args.argv[arg_idx][char_idx++] = c;
                in_arg = 1;
            }
        }
    }
    
    if (in_arg && arg_idx < MAX_ARGS) {
        args.argv[arg_idx][char_idx] = '\0';
        arg_idx++;
    }
    
    args.argc = arg_idx;
    return args;
}

// Show command-specific help
void shell_show_help(const char* cmd) {
    if (strcmp(cmd, "echo") == 0) {
        kprint("echo <text>  - Print text to console\n");
        kprint("Example: echo Hello World\n");
    } else if (strcmp(cmd, "test") == 0) {
        kprint("test <expr>  - Evaluate expression\n");
        kprint("Example: test -f motd.txt (check if file exists)\n");
    } else if (strcmp(cmd, "rm") == 0) {
        kprint("rm <filename> - Remove file\n");
        kprint("Example: rm nouveau.txt\n");
    } else if (strcmp(cmd, "pwd") == 0) {
        kprint("pwd          - Print working directory\n");
    } else if (strcmp(cmd, "whoami") == 0) {
        kprint("whoami       - Show current user\n");
    } else {
        kprint("Unknown command: "); kprint(cmd); kprint("\n");
    }
}

// Execute command with parsed arguments
int shell_execute_command(cmd_args_t args) {
    if (args.argc == 0) return 0;
    
    const char* cmd = args.argv[0];

    if (strcmp(cmd, "echo") == 0) {
        for (int i = 1; i < args.argc; i++) {
            kprint(args.argv[i]);
            if (i < args.argc - 1) kprint(" ");
        }
        if (args.argc > 1) kprint("\n");
        else kprint("\n");
        return 0;
    }
    
    else if (strcmp(cmd, "test") == 0) {
        if (args.argc < 2) {
            printk(LOG_ERR, "test: missing argument");
            return 1;
        }
        
        if (strcmp(args.argv[1], "-f") == 0 && args.argc >= 3) {
            // Test if file exists
            char* content = vfs_read_file(args.argv[2]);
            if (content) {
                kprint("File exists: "); kprint(args.argv[2]); kprint("\n");
                return 0;
            } else {
                kprint("File not found: "); kprint(args.argv[2]); kprint("\n");
                return 1;
            }
        } else if (strcmp(args.argv[1], "-d") == 0) {
            // Test if directory exists (always / in our simple VFS)
            kprint("Directory: /\n");
            return 0;
        } else {
            printk(LOG_ERR, "test: unknown option %s", args.argv[1]);
            return 1;
        }
    }
    
    else if (strcmp(cmd, "rm") == 0) {
        if (args.argc < 2) {
            printk(LOG_ERR, "rm: missing filename");
            return 1;
        }
        vfs_delete_file(args.argv[1]);
        return 0;
    }
    
    else if (strcmp(cmd, "pwd") == 0) {
        kprint("/\n");
        return 0;
    }
    
    else if (strcmp(cmd, "whoami") == 0) {
        kprint("root\n");
        return 0;
    }
    
    else if (strcmp(cmd, "help") == 0) {
        if (args.argc >= 2) {
            shell_show_help(args.argv[1]);
        } else {
            kprint("=== MyOS Commands ===\n");
            kprint("ls      - List files in root directory\n");
            kprint("cat     - Read file 'motd.txt'\n");
            kprint("touch   - Create 'nouveau.txt'\n");
            kprint("echo    - Print text to console\n");
            kprint("test    - Test file/directory existence\n");
            kprint("rm      - Remove file\n");
            kprint("cp      - Copy file (cp <src> <dst>)\n");
            kprint("mv      - Move/rename file (mv <src> <dst>)\n");
            kprint("hexdump - Show file in hexadecimal\n");
            kprint("pwd     - Print working directory\n");
            kprint("whoami  - Show current user\n");
            kprint("dmesg   - Show kernel log buffer\n");
            kprint("pci     - Scan PCI devices\n");
            kprint("date    - Show current time\n");
            kprint("stats   - Show kernel statistics\n");
            kprint("uname   - Show system info\n");
            kprint("clear   - Clear screen\n");
            kprint("reboot  - Reboot system\n");
            kprint("help    - Show this help (or 'help <cmd>')\n");
        }
        return 0;
    }
    
    else if (strcmp(cmd, "cp") == 0) {
        if (args.argc < 3) {
            printk(LOG_ERR, "cp: usage: cp <src> <dst>");
            return 1;
        }
        return vfs_copy_file(args.argv[1], args.argv[2]) ? 0 : 1;
    }
    
    else if (strcmp(cmd, "mv") == 0) {
        if (args.argc < 3) {
            printk(LOG_ERR, "mv: usage: mv <src> <dst>");
            return 1;
        }
        return vfs_rename_file(args.argv[1], args.argv[2]) ? 0 : 1;
    }
    
    else if (strcmp(cmd, "hexdump") == 0) {
        if (args.argc < 2) {
            printk(LOG_ERR, "hexdump: usage: hexdump <filename>");
            return 1;
        }
        vfs_hexdump_file(args.argv[1]);
        return 0;
    }
    
    else if (strcmp(cmd, "uptime") == 0) {
        /* Get uptime from vfs_get_uptime() - connected to timer */
        uint32_t uptime = vfs_get_uptime();
        uint32_t hours = uptime / 3600;
        uint32_t minutes = (uptime % 3600) / 60;
        uint32_t seconds = uptime % 60;
        printk(LOG_INFO, "Uptime: %d:%02d:%02d", hours, minutes, seconds);
        return 0;
    }
    
    else if (strcmp(cmd, "free") == 0) {
        /* Show memory stats */
        extern int placement_address;  /* From mem.c */
        int used_kb = placement_address / 1024;
        int total_kb = 4096;  /* 4MB heap */
        int free_kb = total_kb - used_kb;
        printk(LOG_INFO, "Memory: used=%dKB, free=%dKB, total=%dKB", used_kb, free_kb, total_kb);
        return 0;
    }
    
    return -1; // Unknown command
}
