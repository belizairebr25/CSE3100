# C Exam Skeletons

Small, plain-C templates intended to be copied and adapted during an open-local systems-programming exam.

## How to use these
- Open the file closest to the problem.
- Copy only the functions you need into your program.
- Rename types/functions to match the assignment.
- Read the ownership note before using a structure.
- Compile early: `gcc -Wall -Wextra -Wpedantic -g your_file.c -o your_program`
- For memory bugs: `gcc -Wall -Wextra -g -fsanitize=address,undefined your_file.c -o your_program`

These are intentionally simple templates, not a production-quality library. Most store `int` values or strings so the pointer logic is easy to see.

## Index
| File | Use it for |
|---|---|
| `01_dynamic_array_int.c` | Resizable array, append/get/set/remove |
| `02_singly_linked_list_int.c` | Nodes, head pointer, insertion/deletion/traversal |
| `03_stack_int.c` | LIFO stack |
| `04_circular_queue_int.c` | FIFO queue without shifting elements |
| `05_hash_table_string_int.c` | String keys mapped to integer values, separate chaining |
| `06_binary_search_tree_int.c` | Tree search/insert/traversal/free |
| `07_string_and_input_helpers.c` | Safe line input, tokenization patterns, dynamic string basics |
| `08_file_read_lines.c` | Read a text file line by line with `getline` |
| `09_fork_exec_wait.c` | Fork a child, exec a program, wait for it |
| `10_pipe_two_commands.c` | Connect stdout of one process to stdin of another |
| `11_mutex_thread_template.c` | Basic pthreads + mutex pattern |
| `12_error_handling_patterns.c` | Common allocation, file descriptor, and cleanup patterns |

## Important C reminders
- `malloc`/`calloc`/`realloc` allocate memory; `free` releases it.
- Never use a pointer after `free`.
- Don't overwrite the only pointer to an allocation with `realloc` directly:
  use a temporary pointer first.
- If a function receives a pointer, be clear whether it borrows it or owns it.
- File descriptors should be closed when no longer needed.
- After `fork`, parent and child are separate processes.
- `exec` replaces the current process on success; code after it runs only on failure.
- Check return values. `-1`, `NULL`, and short reads/writes often signal failure.
- For pthread programs, compile with `-pthread`.

## Quick decision guide
- Need fast indexing by position? Dynamic array.
- Need frequent insert/remove when you already have a node? Linked list.
- Need last-in-first-out? Stack.
- Need first-in-first-out? Queue.
- Need key -> value lookup? Hash table.
- Need ordered search/traversal? Binary search tree (but can degrade to O(n)).
- Need to run another program? `fork` + `exec` + `waitpid`.
- Need to connect programs? `pipe` + `dup2` + `fork` + `exec`.
- Need threads to share data? Mutex protects shared state.

## Compile notes
The files are templates and most include a tiny `main` demonstrating use. If you copy functions into an existing assignment, remove the demo `main` to avoid duplicate definitions.
