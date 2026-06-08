# Git 工作流 — 智能鱼缸项目

> 路径：`E:\Mavis项目\智能鱼缸\.harness\docs\git-workflow.md`
> 维护人：project-manager
> 最近更新：2026-06-06

## 分支模型

```
main (生产) ← 稳定可发布
  ↑
  └─ dev (主开发) ← 日常开发
        ↑
        ├─ feature/<rein-scope>/<name>  (新功能)
        └─ hotfix/<rein-scope>/<name>   (紧急修复)
```

详见 `.harness/workflows/`。

## 仓库设置

```bash
# 远程仓库
git remote add origin https://github.com/fugandou04-ui/smart-fish-tank.git

# 默认分支
git symbolic-ref refs/remotes/origin/HEAD refs/remotes/origin/main
```

## Worktree 工作流

本项目使用 git worktree 隔离并行工作：

```bash
# 列出 worktrees
git worktree list

# 创建新 feature worktree
git worktree add .worktrees/feature-fw-auto-feeder -b feature/fw-main/auto-feeder-timer dev

# 在 worktree 中开发
cd .worktrees/feature-fw-auto-feeder
# ... 修改 ...

# 提交
git add .
git commit -m "feat(fw-main): auto-feeder timer support"

# 推送到远程
git push origin feature/fw-main/auto-feeder-timer

# 合并回 dev
cd <repo-root>
git checkout dev
git merge feature/fw-main/auto-feeder-timer

# 清理 worktree
git worktree remove .worktrees/feature-fw-auto-feeder
git branch -d feature/fw-main/auto-feeder-timer
```

## Commit Message 规范

参考 `.harness/workflows/dev.md` 完整规范。简明格式：

```
<type>(<scope>): <subject>

<body>

<footer>
```

**type**: feat / fix / docs / refactor / chore / style / test / audit
**scope**: hw-circuit / hw-pcb / hw-bom / fw-main / fw-config / struct-* / mgmt / audit

## Commit 频率

- **小步提交**：每完成一个原子操作就 commit
- **避免巨型 commit**：单 commit 不超过 500 行变更
- **每日合并**：每个工作日结束前把 feature 合入 dev

## PR 流程

```bash
# 1. 推送到远程
git push origin feature/<name>

# 2. 在 GitHub 创建 PR
gh pr create --base dev --head feature/<name> --title "..." --body "..."

# 3. 等待 review
# - 至少 1 名同行 review
# - verifier 审核签字

# 4. 合并（squash merge）
gh pr merge --squash --delete-branch

# 5. 本地同步
git checkout dev
git pull origin dev
git worktree remove .worktrees/<name>
```

## .gitignore

```gitignore
# 编译输出
*.bin
*.elf
*.hex
.pio/
build/

# 编辑器
.vscode/
.idea/
*.swp

# 系统文件
.DS_Store
Thumbs.db

# Mavis 内部
.mavis/plans/*/workspace/
.mavis/plans/*/outputs/

# 临时
*.tmp
*.bak
*.log
```

## 紧急情况

- 主分支破损 → 立即走 hotfix 流程
- 误提交敏感信息 → `git filter-branch` / 通知所有协作者
- worktree 卡住 → `git worktree remove --force <path>`

## 修订记录

- **2026-06-06 v1.0**：建立 worktree 工作流、commit 规范、PR 流程、`.gitignore` 模板。
