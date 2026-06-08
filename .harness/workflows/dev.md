# Workflow: dev — 主开发分支

> 路径：`E:\Mavis项目\智能鱼缸\.harness\workflows\dev.md`
> 维护人：Mavis Orchestrator
> 最近更新：2026-06-06

## 用途

`dev` 分支是**主开发分支**，承载所有非紧急的功能开发、文档整改、refactor 工作。

## 分支命名

```
dev
```

## 生命周期

- **创建**：项目初始化时从 main 创建
- **合并目标**：main
- **删除**：永不删除

## 工作流

```
1. 编排器拆解任务 → 委派到对应 rein
2. rein 在 dev 上提交
3. CI / verifier 审核（人工或自动）
4. 通过后合入 main
5. 更新 changelog
```

## Commit 规范

Conventional Commits 格式：

```
<type>(<scope>): <subject>

<body>

<footer>
```

**type 取值**：
- `feat` — 新功能
- `fix` — bug 修复
- `docs` — 文档变更
- `refactor` — 重构（无功能变化）
- `chore` — 杂项（依赖、配置等）
- `style` — 代码格式
- `test` — 测试
- `audit` — 文档一致性核查

**scope 取值**（按 rein 域）：
- `hw-circuit` / `hw-pcb` / `hw-bom`
- `fw-main` / `fw-config` / `fw-web`
- `struct-design` / `struct-cutting` / `struct-v2`
- `doc` / `mgmt` / `audit`

**示例**：
```
fix(fw-config): GPIO2 → GPIO7 (DS18B20 避让 strapping 引脚)

ESP32-C3 的 GPIO0~GPIO3 是 strapping 引脚，上电时影响启动模式。
DS18B20 改用 GPIO7。

Refs: 文档一致性核查报告 §1 P0
```

## 合并规则

- 必须由 verifier 在 PR 中签字
- 必须更新 `.harness/changelogs/<合并日期>.md`
- 必须更新 MEMORY.md 修订记录（如属重大变更）

## 不允许

- ❌ 跳过 verifier 直接合入
- ❌ 强制 push（除非 verifier 同意）
- ❌ 在 dev 上直接修改结构 v2.0 基准（必须经 mechanical-engineer + verifier 双签）
