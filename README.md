Basic implementation of a shell, allowing for piping, redirection, and background processes.

```
user >echo my file contents > file.txt
user >ls
file.txt  shell
pid:7179 status:0
user >cat file.txt
my file contents
pid:7177 status:0
pid:7181 status:0
user >cat file.txt | tr a-z A-Z
MY FILE CONTENTS
PID:7177 STATUS:0
PID:7184 STATUS:0
pid:7185 status:0
user >

```

Usage:

```
make shell
./shell
```
