# 分支与合并记录

控制器最初在独立 Git 仓库中开发：main 保留可运行版本，功能分支包括 feature/pid、
feature/inverse-kinematics、feature/odometry 和 feature/tf-command。
本目录从开发提交 ce89ebbeeda751093feb57bf479bebbb95ffefeb 导出，是课程提交快照。
复制时不带 .git；因此课程父仓库不会自动拥有独立开发仓库中的分支和提交图。

独立开发记录中的关键提交如下：

| 提交 | 内容 |
|---|---|
| 3a58e81 | 建立 ROS 包与兼容仿真启动文件 |
| 6a4b5ce | 四轮 PID、速度处理和里程计控制链 |
| a4fdbf8 | 轮速符号说明与后退测试 |
| 9b4264b | 保留单位与运动学约定，完成冲突合并 |
| ddebf75 | 实测轮速的正运动学测试 |
| ea35163 | 键盘、查看界面与仿真验收脚本 |
| ce89ebb | 操作指南、代码讲解与实测结果 |

要看课程仓库中本次整理的变更，在 missing-semester 根目录使用：

```bash
git log --oneline -- 2027/SaoF-A/homework/hero_chassis_controller
git show --stat HEAD
```

2026-11-11 做了一次本地分支合并练习：main 在 docs/design.md 的接口说明中补充单位，
feature/inverse-kinematics 在同一位置补充坐标方向和轮速公式。合并时 Git 报告内容冲突。
这是同一个工作空间中的分支练习，没有多人协作记录。

处理步骤：

1. `git status` 找出标记为 unmerged 的 docs/design.md。
2. 打开文件，比较 HEAD 与功能分支在冲突标记之间的内容。
3. 同时保留单位、坐标方向和公式，删除三个冲突标记。
4. `git add docs/design.md` 标记解决，然后 `git commit` 完成合并。
5. 检查合并后的说明与配置一致，并重新执行运动学测试。

提交范围为本 ROS 包。构建产物、编辑器私有设置、运行日志和本地工作说明不纳入仓库。
上游描述模型与参考控制器在工作空间中分别保留自己的 Git 仓库，不复制到本包。
