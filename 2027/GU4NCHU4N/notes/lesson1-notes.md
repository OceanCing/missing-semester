常用命令：

```bash
pwd                  # 当前目录
ls -la               # 列出全部文件（含隐藏文件）
cd /path             # 切换目录
mkdir -p a/b/c       # 递归创建目录
cp -r src dst        # 递归复制
mv old new           # 移动或重命名
rm -i file           # 删除前确认
grep -rn "text" .    # 递归搜索文本
find . -name "*.md"  # 按名字查找文件
```
## 2. Shell
- 变量赋值等号两边不能有空格：`name="value"`
- 使用变量：`$name` 或 `"${name}"`
- 单引号内不展开变量，双引号内会展开
- 管道把上一个命令的输出交给下一个命令：`ls | grep txt`
- 重定向：`>` 覆盖写入，`>>` 追加写入，`2>` 重定向错误输出

脚本基本结构：

```bash
#!/usr/bin/env bash
set -euo pipefail

name="$1"
echo "Hello, ${name}"
```
## 3. Vim
- `$?` 是上一条命令的退出码，`0` 表示成功
- `if [ -f file ]; then ... fi` 判断文件是否存在
- `for f in *.txt; do ...; done` 遍历文件

- 三种常用模式：Normal、Insert、Command
- `i` 进入插入模式，`Esc` 回到普通模式
- `:wq` 保存退出，`:q!` 不保存退出
- 移动：`h j k l`、`w`/`b`、`gg`/`G`、`0`/`$`
- 编辑：`x` 删除字符、`dd` 删除行、`yy` 复制行、`p` 粘贴、`u` 撤销
- 搜索：`/keyword`，`n` 跳到下一个结果
- 替换：`:%s/old/new/g`
## 4. Git

Git 的三个区域：工作区 → 暂存区 → 本地仓库。

```bash
git status                    # 查看状态
git add file                  # 加入暂存区
git commit -m "Message."      # 提交
git log --oneline --graph     # 查看历史
git switch -c dev-name        # 创建并切换分支
git switch master             # 切换分支
git merge dev-name            # 合并分支
git remote -v                 # 查看远程仓库
git push -u origin dev-name   # 推送并建立追踪
```

- 冲突时会看到 `<<<<<<<`、`=======`、`>>>>>>>` 标记
- 解决冲突后 `git add`，再 `git commit` 完成合并

## 5. GitHub

- 标准流程：Fork → Clone → Branch → Commit → Push → Pull Request
- PR 中 base 是目标分支，compare 是你的分支
- 向 PR 分支继续提交，PR 会自动更新
- 遇到冲突应在本地或网页解决，不要通过关闭 PR 再重开来绕过

## 6. 命令速查

| 场景 | 命令 |
| --- | --- |
| 查看目录 | `ls -la` |
| 查找文件 | `find . -name "*.md"` |
| 搜索内容 | `grep -rn "text" .` |
| 查看提交历史 | `git log --oneline` |
| 推送分支 | `git push -u origin <branch>` |
