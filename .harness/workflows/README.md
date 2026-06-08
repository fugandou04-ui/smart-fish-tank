# Workflows 索引

> 路径：`E:\Mavis项目\智能鱼缸\.harness\workflows\README.md`
> 维护人：Mavis Orchestrator
> 最近更新：2026-06-06

## 分支策略总览

```
main (生产)
  ↑
  └─ dev (主开发)
        ↑
        ├─ feature/<rein-scope>/<name>  (新功能)
        └─ hotfix/<rein-scope>/<name>   (紧急修复)
```

## 三个工作流

| 工作流 | 文件 | 用途 | 持续时间 | 审核要求 |
|--------|------|------|----------|----------|
| dev | `dev.md` | 主开发分支 | 永久 | verifier 必签 |
| feature | `feature.md` | 新功能开发 | 3-14 天 | verifier + 同行 review |
| hotfix | `hotfix.md` | 紧急修复 | 小时级 | verifier 加速签 |

## 触发决策树

```
收到任务
  ├─ 是否阻塞生产/采购/烧录？
  │    ├─ 是 → hotfix 流程
  │    └─ 否 → 继续判断
  ├─ 是否新增功能？
  │    ├─ 是 → feature 流程
  │    └─ 否 → 继续判断
  └─ 是否修复/重构/文档整改？
     └─ 是 → dev 流程
```

## 并行工作

- 多个 feature 可同时存在（用 git worktree 隔离）
- hotfix 期间暂停所有合入 main（除 hotfix 本身）
- 同一文档同一时刻只能有 1 个 feature 修改（避免合并冲突）

## 紧急升级路径

- 任务从 feature 升级为 hotfix：编排器在 dev 流程中发现 P0 风险 → 转 hotfix
- 任务从 hotfix 降级为 feature：发现不是紧急问题 → 转 feature

## 文档同步规则

**任何修改涉及以下真值，必须 3 处同步**：

| 真值 | 同步点 |
|------|--------|
| GPIO | README + MEMORY + 固件 config.h |
| 成本数字 | 文档 + MEMORY 口径说明 + 项目计划书 |
| 结构尺寸 | 切割方案 + 详细设计 + 装配说明书 |

## 相关文档

- `.harness/agent.md` — Harness 编排器
- `.harness/reins/<name>/agent.md` — 各 rein 边界
- `.harness/docs/code-style.md` — 代码风格
- `.harness/docs/test-policy.md` — 测试规范
- `.harness/memory/MEMORY.md` — 跨任务记忆

## 修订记录

- **2026-06-06 v1.0**：首次建立三工作流（dev/feature/hotfix），定义触发决策树、并行工作规则、紧急升级路径。
