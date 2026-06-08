# Workflow: feature — 新功能分支

> 路径：`E:\Mavis项目\智能鱼缸\.harness\workflows\feature.md`
> 维护人：Mavis Orchestrator
> 最近更新：2026-06-06

## 用途

承载**新功能开发**。每个 feature 一个分支，命名清晰，便于 code review 和回滚。

## 分支命名

```
feature/<rein-scope>/<short-name>
```

**示例**：
- `feature/fw-main/auto-feeder-timer` — 嵌入式工程师做自动喂食定时
- `feature/struct-cutting/acrylic-2mm-revamp` — 机械工程师做亚克力 2mm 改造
- `feature/hw-pcb/usb-c-addition` — PCB 工程师加 Type-C 接口
- `feature/mgmt/bom-cost-v2` — PM 做 BOM 成本 v2

## 生命周期

```
1. 从 dev 创建
2. rein 在 feature 分支开发
3. 提交 PR 回到 dev
4. verifier 审核 + 至少 1 名 specialist 同行 review
5. 合并 → dev
6. 删除 feature 分支
```

## 持续时长

- **建议**：3-7 天
- **最长**：不超过 14 天（超过则拆任务或同步讨论）

## Worktree 约定

本项目使用 git worktree 隔离并行工作：

```bash
# 创建 worktree + 分支
git worktree add .worktrees/feature-fw-auto-feeder -b feature/fw-main/auto-feeder-timer

# 在 worktree 中开发
cd .worktrees/feature-fw-auto-feeder
# ... 修改代码 ...

# 提交
git add .
git commit -m "feat(fw-main): auto-feeder timer support"

# 推送到远程（可选）
git push origin feature/fw-main/auto-feeder-timer

# 合并回 dev（PR 流程或直接合并）
git checkout dev
git merge feature/fw-main/auto-feeder-timer

# 清理
git worktree remove .worktrees/feature-fw-auto-feeder
git branch -d feature/fw-main/auto-feeder-timer
```

## 完成定义（DoD）

- [ ] 代码 / 文档 提交到 feature 分支
- [ ] commit message 符合 conventional commits
- [ ] 至少 1 个同行 review 签字
- [ ] verifier 审核通过
- [ ] 如有 GPIO/尺寸/成本数字变更，3 处同步（README + MEMORY + 实现）
- [ ] changelog 已更新
- [ ] feature 分支合并到 dev 后删除

## 不允许

- ❌ 跨多个 specialist 域的 feature（拆任务）
- ❌ 直接合入 main
- ❌ 长寿命分支（> 14 天视为失控）
