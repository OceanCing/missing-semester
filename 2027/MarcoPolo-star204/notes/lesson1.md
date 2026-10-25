
# Lesson 1 Notes: Linux, Vim, Shell, Git & GitHub
## 1. Linux Basic
Linux is a Unix-like open-source operating system.
Everything in Linux is treated as a file.

Common commands:
- `pwd`: print current working directory
- `ls`: list directory contents
- `cd`: change directory
- `mkdir`: create directory
- `rm`: remove files or directories
- `cp`: copy files
- `mv`: move / rename files

Path type:
- Absolute path: starts from `/`
- Relative path: relative to current directory

## 2. Vim Editor
Vim is a terminal text editor, has multiple modes.
- Normal mode: default mode, for navigation and operation
- Insert mode: for editing text (press `i`)
- Command mode: press `:`
    - `:w` save
    - `:q` quit
    - `:wq` save and quit

## 3. Shell
Shell is the command interpreter between user and OS kernel.
We can write `.sh` script to batch execute commands.
`chmod +x file.sh` add execute permission.

## 4. Git & GitHub
Git is a distributed version control system.
GitHub is a platform to host git repository.

Basic workflow:
1. `git checkout -b branchname` create new branch
2. `git add` stage files
3. `git commit -m "Message."` commit
4. `git push` push to remote
5. Open PR to target repo
