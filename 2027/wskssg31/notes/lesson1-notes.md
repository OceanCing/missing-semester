# Lesson 1 : Linus  Vim  Shell  Git  Github

## 1. Linux Basics

### Common Commands

- `pwd` : print current working directory
- `ls` : list files and directories
- `cd` : change the current directory
- `mkdir` : create a directory
- `touch`: create an empty file
- `cat`: display the contents of a file
- `cp`: copy files or directories
- `mv`: move or rename files
- `rm` : remove files or directories

## 2. Shell and Vim

### Shell

- `>` redirects output to a file and overwrites its previous contents
- `>>` redirects output to a file and appends to its contents
- `|` passes the output of one command to another command as input
- `&&` runs the next command only if the previous command succeeds

### Vim

- `i`: enter Insert mode
- `Esc`: return to Normal mode
- `:w`: save the file
- `:q`: quit Vim
- `:wq`: save and quit
- `:q!`: quit without saving changes

## 3. Git & Github

### Git basics

- `git status`: show the current repository status
- `git add`: add changes to the staging area
- `git commit`: save staged changes as a commit
- `git log --oneline`: show a compact commit history
- `git diff`: show changes that have not been staged
- `git clone`: copy a remote repository to the local machine
- `git push`: upload local commits to a remote repository

### Branches

- `git branch`: list local branches
- `git switch -c feature/example`: create and switch to a new branch

### Git Workflow

A typical workflow is:

1. Fork the target repository on GitHub.
2. Clone your fork to the local machine.
3. Create a feature branch.
4. Edit or create files.
5. Run `git status` and review changes with `git diff`.
6. Stage changes with `git add`.
7. Commit changes with a clear message.
8. Push the branch to the remote repository.
9. Open a Pull Request (PR) to the target repository.
