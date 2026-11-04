
#!/bin/bash
cd "$(dirname "$0")"

{
  date
  history -r "$HOME/.bash_history"
  history 10
  wc -m ../note/2026-10-17_first_homework.md
} > output.txt
