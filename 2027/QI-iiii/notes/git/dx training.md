git clone <地址>：下载仓库及提交历史
git status：查看当前分支和文件状态
git diff：查看尚未暂存的修改
git log --oneline：简短格式查看提交历史
git switch -c feature/hello：创建并切换到功能分支
git add hello.txt：暂存指定文件
git commit -m "Add greeting"：记录一次本地提交
git push -u origin feature/hello：推送并建立跟踪关系
git switch main：切回主分支
git pull：获取并整合当前分支的远程更新
PR 工作流：Fork团队仓库     Clone到本地    Feature分支   修改/Add/Commit   Push到个人Fork   发起PR   Review评审   Merge到团队仓库