# Unix File System Tree Simulator

A C program that simulates a Unix-style file system hierarchy using a tree data structure. This project implements core shell commands (`mkdir`, `ls`, `cd`, `pwd`, `rm`, `rmdir`, `touch`) by manipulating a custom tree of nodes, providing a practical application of data structures for systems programming.

## Technical Implementation

The file system is represented as a tree where each node is a `struct NODE` containing:
- `name`: The name of the file or directory (max 63 chars).
- `fileType`: `'D'` for directory, `'F'` for file.
- Pointers to its `parent`, `child` (first child), and `sibling` (next sibling).

### Key Functions Implemented

1.  **`struct NODE* splitPath(char* pathName, char* baseName, char* dirName)`**
    - **Input:** A full path string (e.g., `"/a/b/c.txt"`).
    - **Output:** Splits the path into `dirName` (`"/a/b"`) and `baseName` (`"c.txt"`).
    - **Return:** A pointer to the parent node of the `baseName` element. Returns `NULL` and prints an error if any directory in the path does not exist.

2.  **`void mkdir(char* pathName)`**
    - **Input:** A path for a new directory to create.
    - **Action:** Uses `splitPath()` to find the parent directory, allocates a new node, and inserts it into the tree structure. Handles error cases (e.g., path already exists, no path provided).

## How to Build & Run

1.  **Compile the program:**
    ```bash
    make
    ```
    This produces the `main` executable.

2.  **Run the program:**
    ```bash
    ./main
    ```

3.  **Run with test input (example):**
    ```bash
    # Redirects stdin from a test file and suppresses prompt messages
    ./main < test_cases/test01.in 2>/dev/null > my_output.out
    ```

4.  **Compare your output to the expected output:**
    ```bash
    diff my_output.out test_cases/test01.out
    ```

## 💻 Available Commands

| Command | Description | Example |
| :--- | :--- | :--- |
| `mkdir <path>` | Creates a new directory | `mkdir /a/b` |
| `touch <path>` | Creates a new file | `touch file.txt` |
| `ls` | Lists contents of the current directory | `ls` |
| `cd <path>` | Changes the current directory | `cd /a` |
| `pwd` | Prints the absolute path of the current directory | `pwd` |
| `rm <path>` | Removes a file | `rm file.txt` |
| `rmdir <path>` | Removes an empty directory | `rmdir /a/b` |
| `tree` | Displays the entire file system hierarchy | `tree` |
| `menu` | Prints the list of available commands | `menu` |
| `quit` | Exits the program | `quit` |

## Testing

The `test_cases` directory contains input files (`testXX.in`) and their expected output (`testXX.out`). To ensure correctness, test your implementation against all provided cases.

**Example Test Command:**
```bash
./main < test_cases/test05.in 2>/dev/null > mytest5.out
diff mytest5.out test_cases/test05.out

