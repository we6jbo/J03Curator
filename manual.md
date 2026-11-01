# J03Curator AI Operator Manual (v0.8 guidance; v0.7 CLI)

J03Curator is a local, command-line-only Qt/C++ project tracking and documentation-export tool. It does not itself push public GitHub changes, upload Imgur images, or post to WordPress.

## Paths
- CLI: `~/.local/bin/J03Curator`
- Source: `~/Projects/J03Curator/`
- Database: `~/.so672jNC/j03curator.db`
- Local Git repository: `~/.j03-software/`
- This manual: `~/.jchjtzir/manual.md`

## Commands
- `J03Curator status` checks paths and Git remote.
- `J03Curator list` lists registered projects and flags.
- `J03Curator add PROJECT ~/Projects/PROJECT/main.cpp` registers a project whose source file already exists.
- `J03Curator plan PROJECT` shows the release/hold decision and next-step handoff command.
- `J03Curator handoff PROJECT` prints a complete, copyable message for ChatGPT or Mistral Vibe with completed work, the recommended command, manual path, and source location. It never runs the next command.
- `J03Curator review PROJECT` marks the existing flags reviewed. Press Enter to confirm or type `cancel` to stop. A review alone does not grant publication permission.
- `J03Curator screenshot PROJECT` finds the latest local PNG without changes.
- `J03Curator screenshot PROJECT --select-latest` remembers ONE unapproved candidate without copying it.
- `J03Curator export PROJECT` previews local documentation export if reviewed and not restricted.
- `J03Curator export PROJECT --apply` copies approved source candidates to the local repository after Enter confirmation; it does not publish images, commit, or push.
- `J03Curator sync` previews the repository marker state; `sync --apply` may create and locally commit only the 28253.txt marker, not a general GitHub upload.

## Rules
Keep all original screenshots locally; permit at most ONE privacy-approved public screenshot per project. A screenshot candidate is not automatically approved. `copyright=1` and `useful=1` are irreversible in the current database; use caution before setting either. A reviewed not-useful application may still have approved development materials exported locally. Third-party copyrighted or private materials require separate rights and privacy review. Do not claim public release, GitHub push, Imgur upload, WordPress posting, or store submission until verified.

## Suggested CraftBox test
`J03Curator handoff CraftBox`
`J03Curator plan CraftBox`
`J03Curator export CraftBox`
If reviewing and approving source material for local export, then `J03Curator export CraftBox --apply` and press Enter. Inspect local changes with `git -C ~/.j03-software status --short`. No public push is automatic.

## Completion phase and time-pressure handoff (v0.6)

- `J03Curator completion start PROJECT`: start or restore a phase timer. Writes `/tmp/emal/time.json` plus durable `~/.local/state/j03curator/completion-time.json`.
- `J03Curator completion check PROJECT`: compare elapsed time against 16 minutes. If there is no recorded blog after more than 16 minutes, prepare an AI handoff, copy it to the clipboard if possible, and open the saved ChatGPT session URL using Chrome. This check is **on demand**, not a background timer.
- `J03Curator completion slow PROJECT`: initiate the same handoff immediately when Jeremiah says it is taking too long.
- `J03Curator completion blog PROJECT`: mark the blog as published after user confirmation. This is local bookkeeping only.

The session file must contain exactly a ChatGPT conversation URL and must reside at `~/.ckbrgj/chatgpt-session.txt`. No session contents are uploaded by J03Curator. It opens the URL in Chrome; the prompt is copied locally via wl-copy, xclip or xsel when available.

### Publication policy
Only legally redistributable, intentionally selected, useful materials should be included in the public collection. A project can have a useful educational source file even if the app itself is not suitable for app stores. Every GUI project should have one approved public screenshot before its documentation is considered complete; CLI projects should supply a suitable terminal screenshot where practicable. An approved image must still pass screenshotpublisher's exact-hash approval. Never force a screenshot into GitHub without approval. Project release eligibility is per project: J03Curator itself can be eligible for GitHub, Snap and Flatpak even if CraftBox is documentation-only. No automatic commit, push or store submission occurs in the timer commands.

## Automatic first-run manual reminder (v0.7)

At the start of **every J03Curator CLI invocation**, the application checks `/tmp/wzfh/started.touch`. If it does not exist, the application prints this exact message before creating the marker:

`Chatgpt or Mistral vibe, please read /home/we6jbo/.jchjtzir/manual.md because it has all the commands`

The message appears only on the first invocation while the `/tmp` marker exists. A reboot or removal of the marker may cause it to appear again. A failed marker write causes another attempt on the next invocation. The notice does not itself make ChatGPT or Vibe open the manual; the assistant must have local file access and follow the reminder.

You do not need to run `completion start` or `completion slow` manually; an AI agent with command access should manage the completion phase according to this manual.


## AI-ONLY OPERATING CONTRACT (v0.8)

**Audience:** ChatGPT or Mistral Vibe, not Jeremiah. Do not assign Jeremiah a manual-reading task or ask him to memorize these commands. He states the goal and approves consequential actions; the AI handles implementation and troubleshooting when it has actual T14 command access. In browser-only ChatGPT, give one safe, pasteable command at a time, interpret the returned output, and never claim to have executed local actions.

### 1. Session startup and decision hierarchy

1. When J03Curator prints the `/tmp/wzfh/started.touch` reminder, **read this complete manual** if local filesystem access permits. If the AI cannot read it, ask for the relevant text/output rather than pretending.
2. Check the installed executable (`command -v J03Curator`; `J03Curator help`; `J03Curator status`) and consult `J03Curator handoff PROJECT` first for known progress and recommended next action. The handoff is advice, not permission to publish.
3. If handoff is unavailable, inconclusive, contradictory, or recommends an unfamiliar command, consult this manual, then the source under `/home/we6jbo/Projects/J03Curator/`. Do not guess command flags.
4. Use the current project's *own* release decision. A documentation-only decision for CraftBox does not prevent J03Curator from being eligible for application releases. Separate lawful/valuable assets, source code, and AI instructions from apps that are actually worth distributing.
5. Progress one meaningful step at a time; batch *safe read-only checks* when helpful. Avoid repeated micro-steps, unnecessary rechecking, and lengthy exchanges once publication is ready.

### 2. AI owns the completion clock

Jeremiah **never** needs to type `J03Curator completion start PROJECT`, `completion check`, `completion slow`, or `completion blog`. These are for an AI agent with access to the T14 terminal. A browser-only assistant cannot start or monitor them by itself, but should explain a one-command workaround only if essential.

- **Start** `J03Curator completion start PROJECT` automatically when the project transitions from feature work to its finishing workflow: build/test/fix, documentation, legally approved source/assets and one screenshot, commit/push if approved, blog draft/publication, and follow-on writing. Do not start it merely when J03Curator is opened, or when normal coding starts. Avoid restarting an active phase or resetting a timestamp just to obtain a new window.
- **Check** `J03Curator completion check PROJECT` at meaningful milestones (after build, export, screenshot approval, GitHub verification, blog preparation) and whenever the time budget may be close. Existing v0.7 implementation checks only when invoked; there is no unattended monitoring service.
- **Escalate automatically** if 16 minutes elapse from the genuine completion start and the blog has not been confirmed as published. This ensures an intervention *before 17 minutes*. Do not wait for the user to initiate a command.
- **Escalate early** when Jeremiah explicitly says the process is taking too long, frustrating, dragging, unnecessarily complicated, or asks to speed it up. An isolated question about why a command works does not establish frustration. When ambiguity remains, simplify the workflow instead of launching an intrusive browser handoff without context.
- **Anticipate delay:** If repeated failures, approvals, or verification loops make on-time blog publication unlikely, proactively shorten the remaining steps and prepare the blog draft. Use the `slow` handoff before the 16-minute cutoff only when Jeremiah has expressed impatience or it is clear that the assistance handoff is necessary; otherwise keep the workflow moving.
- **Finish** `J03Curator completion blog PROJECT` *only when there is an actual published URL or an explicit user confirmation of successful publication*. A draft is not a published blog.

### 3. What to do when the timer triggers

The authorized AI assistant can run `J03Curator completion slow PROJECT` for an early handoff or `J03Curator completion check PROJECT` after the deadline. These commands may open the existing ChatGPT session from `~/.ckbrgj/chatgpt-session.txt` in `google-chrome-stable` and may copy an assistance prompt to the clipboard. Confirm that the file contains only the intended ChatGPT conversation URL before opening it. Inform Jeremiah briefly what opened and tell him he can paste the clipboard text into ChatGPT. Do not claim the clipboard operation worked unless confirmed, and do not transmit sensitive local files or publish automatically.

If `completion check` reports no timer, inspect the durable state at `~/.local/state/j03curator/completion-time.json` and temporary file `/tmp/emal/time.json`. If neither exists, start a new timer **at the actual current phase start**, not at an invented earlier time; report the recovery limitation. Preserve the original timestamp across crashes where durable state exists. `/tmp` is volatile and can be erased after a crash.

### 4. Practical troubleshooting command catalog

All commands below must be evaluated for the actual installed version. Prefer read-only diagnosis before changing any data.

```bash
command -v J03Curator
J03Curator help
J03Curator status
J03Curator list
J03Curator handoff PROJECT
J03Curator plan PROJECT
J03Curator screenshot PROJECT
J03Curator export PROJECT
J03Curator completion check PROJECT
cat /home/we6jbo/.jchjtzir/manual.md
ls -l /tmp/wzfh/started.touch /tmp/emal/time.json ~/.local/state/j03curator/completion-time.json
git -C ~/.j03-software status --short --untracked-files=all
git -C ~/.j03-software remote -v
GIT_PAGER=cat git -C ~/.j03-software --no-pager status
infoartrecorder list --project PROJECT
screenshotpublisher list
screenshotpublisher flags
screenshotpublisher status
```

For a build failure, check `cmake -S ~/Projects/J03Curator -B ~/Projects/J03Curator/build-j03curator` and `cmake --build ~/Projects/J03Curator/build-j03curator --parallel 2`, after backing up any changed files. Treat `taskorchestrator` and `tg-register-project` as nonfatal optional integrations on systems that do not have them.

For screenshots, insist on one exact file actually approved by the human and by screenshotpublisher's exact-hash/destination gates. `screenshotpublisher stage --id N` stages an approved local image but does not push it. Never fabricate review records or bypass a flag. If a required approval tool is missing, stop publication of that asset and ask for a safe approval workflow.

For GitHub, review exactly which files are staged, verify remote and rights, commit only approved files, then push only with the owner's permission. Verify the resulting remote commit and URL; a local `git add` or commit is not an upload. For WordPress, a drafted post is not published; obtain its actual j03.page URL before social-media content uses it. Snap, Flatpak and Snap Store releases are a separate project-specific approval workflow; a successful build is not proof of acceptance by a store.

### 5. Do not over-promise automation

This manual tells an AI what to do, but **manual text cannot itself run a timer, detect emotion, launch Chrome, or publish**. v0.7 relies on the agent explicitly invoking J03Curator commands and has no background watcher. Without local terminal access, ChatGPT can only guide the user through the next operation. A 16-minute intervention is best-effort if the agent is still active and checking progress; there is no guaranteed wall-clock alert.
