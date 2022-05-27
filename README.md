# NASM-Template-MacOS
Adopted from [here](https://github.com/Satharus/NASM-Template)

##### This is a template for writing assembly using NASM on Mac OS X.

It has a template "main" file, makefile, and a few helper functions "IO.inc" to help with I/O without having to use syscalls explicitly.

## Function Guide:

|    Function    |                        Parameters                       |          Returns          |                            Description                           |
|:--------------:|:-------------------------------------------------------:|:-------------------------:|:----------------------------------------------------------------:|
| ReadChar       |                           N/A                           | al: Character read        | Reads a character from STDIN and saves it in al.                 |
| WriteChar      | al: Character                                           |            N/A            | Writes the specified ASCII character to STDOUT.                  |
| ReadString     | ecx: String max length esi: Address of storage location | eax: Number of bytes read | Reads a string from STDIN and saves it at the specified address. |
| WriteString    | ecx: Length of the string esi: Address of the string    |            N/A            | Writes the specified string to STDOUT.                           |
| WriteLine      |                           N/A                           |            N/A            | Writes a new line ('\n') to STDOUT.                              |
| WriteDec       | eax: Value to be written                                |            N/A            | Writes the specified value in base-10 (Decimal) to STDOUT.       |
| ReadHexByte    |                           N/A                           | al: Value read            | Reads a hex byte from STDIN, without 0x or h. Ex: 3A or F5       |
| WriteHex       | eax: Value to be written                                |            N/A            | Writes the specified hex value to STDOUT.                        |
| Print4x4Matrix | esi: byte array of length 16                            |            N/A            | Write the array as a 4x4 hex grid.                               |

## Usage
1. Install nasm with `brew install nasm`
2. Write assembly in program.asm
3. Compile project with `make`
4. Run `./program`

## Syscalls
On MacOS, syscalls work slightly differently. If you want to invoke a syscall, you have to OR the syscall id placed in rax with i<<24
where i is a number from 0 to 4 denoting the [syscall class](https://stackoverflow.com/questions/65528933/why-do-macos-use-absolute-memory-locations-for-system-calls).
For example, for UNIX write syscall, put 0x02000004 into rax.
The identifiers placed in rax for each syscall are also slightly different. For a list of them, refer [here](https://github.com/opensource-apple/xnu/blob/master/bsd/kern/syscalls.master).
