# J03Curator v0.3

Adds separate documentation export for reviewed projects whose application usefulness is No. Does not publish, git add, commit, or push.

Workflow: `J03Curator review CraftBox` (requires typing REVIEW), `J03Curator screenshot CraftBox --select-latest` (local candidate), `J03Curator export CraftBox` (dry run), `J03Curator export CraftBox --apply` (requires typing EXPORT CraftBox).

Export deliberately skips screenshots because a selected local screenshot is not a verified screenshotpublisher approval; a reviewed and unflagged screenshot must be explicitly copied later. Existing destination source files will never be overwritten. All inputs should be reviewed for privacy, rights and secrets before public push.

Source file export consists of the fixed Qt project allowlist only. No GitHub publication or blog creation is automated. v0.2 database is preserved. The historical copyright yes flag remains permanent in v0.3; proposed historical/current rights split is NOT implemented yet.

## v0.4 changes
- `review PROJECT`: press Enter to confirm existing flags; type `cancel` to leave unchanged.
- `export PROJECT --apply`: press Enter to stage approved local materials; type `cancel` to exit.
- EOF / piped empty input fails closed.
- `plan PROJECT`: distinguishes documentation export from application release.
- No automatic Git push, Imgur upload, or WordPress publication.


## v0.5 AI handoff
`J03Curator handoff PROJECT` emits a concrete completion summary and suggested next command without executing it. `J03Curator manual` prints the manual path installed at `~/.jchjtzir/manual.md`. `plan PROJECT` displays the handoff command.

### v0.6 completion phase
`J03Curator completion start CraftBox`, then `J03Curator completion check CraftBox`; type `J03Curator completion slow CraftBox` whenever it feels too slow. Session URL is read from `~/.ckbrgj/chatgpt-session.txt`. See `manual.md`.


J03Curator v0.7 startup reminder: On its first CLI invocation after `/tmp/wzfh/started.touch` is absent, J03Curator prints the manual path for ChatGPT/Mistral Vibe and creates the marker. This is a local reminder, not an automatic file read by a remote AI.


## v0.8 AI manual refresh
This release updates the AI-only operator manual and AI workflow documentation. The compiled CLI commands are unchanged from v0.7. The manual is intended for ChatGPT/Mistral Vibe, not the end user.
