# Math Quiz — Firmware Notes

This document records the ground-truth Math Quiz protocol as actually implemented
server-side, derived from the existing (un-modified) website sources:

- `website/Projecth/mathquiz_backend.php`
- `website/Projecth/status/mathquiz.txt`
- `website/Projecth/mathquiz.js` / `mathquiz.html`
- `website/Projecth/status_helper.php`

It is the authoritative reference for the STM32 firmware in
`src/APP/MathQuiz/` and must not contradict the live backend. The website is
treated as read-only here.

---

## 1. Status file — `/status/mathquiz.txt`

The backend appends a single line of semicolon-separated `key=value` pairs on
every state change (via `append_status_log()` inside `mathquiz_backend.php`,
which sanitises values by stripping `;`, `\r` and `\n`).

### Exact field order and value formats

```text
state=ALIVE|DEAD;game=mathquiz;room=ROOM_ID;event=EVENT_NAME;status=ROOM_STATUS;question=OP1 OP OP2;p1_answer=VALUE;p2_answer=VALUE;winner=WINNER;round=INTEGER;time=YYYY-MM-DD HH:MM:SS
```

| Key | Allowed values | Description |
| :--- | :--- | :--- |
| `state` | `ALIVE` / `DEAD` | Session lifecycle flag. `DEAD` is written on match end. |
| `game` | `mathquiz` | Game identifier (fixed). |
| `room` | alphanumeric `[A-Za-z0-9]` (sanitised) | Room code (6 chars generated; clients may pass arbitrary). |
| `event` | `create_room`, `question_generated`, `player_joined`, `player_answered`, `round_result`, `next_question`, `match_ended` | Backend trigger that caused this line; several events may share one snapshot. |
| `status` | `waiting` / `playing` / `result` | Room/round phase. **No `ended` value exists**; match end is signalled by `state=DEAD` + `event=match_ended`. |
| `question` | compact pre-formatted string, no spaces, e.g. `12*8`, `5-3`, `36/4` | Operand1, one operator (`+ - * /`), operand2. Equals PHP `question['status_text']`. |
| `p1_answer` | integer-as-string, or empty `""` | Player 1 submitted answer (blank until submitted). |
| `p2_answer` | integer-as-string, or empty `""` | Player 2 submitted answer (blank until submitted). |
| `winner` | `""` / `p1` / `p2` / `draw` | Round outcome (see game mode below). |
| `round` | integer (`1`, `2`, ...) | Current round index. |
| `time` | `YYYY-MM-DD HH:MM:SS` | Timestamp of state generation. |

### Observed snapshot (backend idle seed)

```text
state=DEAD;game=mathquiz;room=;event=idle;status=waiting;question=;p1_answer=;p2_answer=;winner=;round=0;time=2026-09-15 00:00:00
```

---

## 2. Game mode (server-defined — not chosen by firmware)

Math Quiz is a **two-player, head-to-head, first-correct-answer per-question race**:

1. **Room creation** — `action=create` makes a room, generates `round=1` and the
   first question, and writes `event=create_room` then `event=question_generated`.
   The room starts in `status=waiting` until a second player joins.
2. **Joining** — `action=join` assigns roles `p1` / `p2`, shifts the room to
   `status=playing`, and writes `event=player_joined`.
3. **Question** — both players see the same server-generated question and a
   shuffled set of multiple-choice options (only the question text is exported
   to the status file; the option set is not).
4. **Answering** — each player submits once (`action=answer`). The backend
   validates against `question['correct_answer']`:
   - A **correct** answer immediately ends the round: `status=result`,
     `winner=p1`/`p2`, that player's score increments (score lives only in the
     room JSON, **not** in the status file), and both `event=player_answered`
     and `event=round_result` are written.
   - A **both-wrong** or **timeout** (15 s, `$QUESTION_SECONDS`) round becomes
     `status=result`, `winner=draw`, no score; both `event=player_answered`
     and `event=round_result` are written.
5. **Next round** — `action=next_round` increments `round`, regenerates the
   question, clears answers/winner, sets `status=playing`, and writes
   `event=next_question` + `event=question_generated` (or back to `waiting`
   if a player has left).
6. **Match end** — `action=end_game` sets `finished=true`, writes
   `event=match_ended`, and sets `state=DEAD`.

**Implications for the renderer**

- The visible status file exposes **no running score field**. The firmware
  tracks P1/P2 win counts itself by incrementing on each `result` round whose
  `winner` is `p1` / `p2`.
- Because a correct answer ends the round immediately, a `result` snapshot can
  have one player's answer set and the other's still blank (the round was won
  before the other player answered). The firmware shows `--` for the pending
  side and a correct/incorrect indicator derived from the `winner` field
  (`p1` → P1 correct; `draw` → both, if set, are wrong).
---

## 3. Question / answer representation for the renderer

- The **question is sent as a single pre-formatted string** — the `question`
  field above, equal to PHP `question['status_text']`, e.g. `12*8`. It contains
  **no spaces**. The renderer can display it directly; if a larger presentation
  is wanted, the operator can be extracted by scanning for `+ - * /`.
- `p1_answer` and `p2_answer` arrive as **integer strings** (`"96"`), or are
  entirely absent/empty until the player submits. A blank/empty value means
  "not yet answered".
- There is no separate "correct answer" field in the status file; correctness
  is derived from `winner` (the winner's answer is the correct one).

---

## 4. Firmware module map (what this repo's code implements)

| Concern | Location |
| :--- | :--- |
| Server-state struct + delta detection | `src/APP/MathQuiz/game_int.h`, `game_prg.c` |
| Rendering (font, banners, spinner) | `src/APP/MathQuiz/Animation/MathQuiz_Animation_*.{h,c}` |
| Polling / dispatch integration | `src/APP/main.c` (`IsMathQuizGame`, `ParseMathQuizState`, `MathQuiz_vStart`/`vStop` loop) |
| Shared 3x5 font engine | `src/APP/Common/FONT3x5.{h,c}` |
| Shared loading animation spinner | `src/APP/Common/LOADING_ANIM.{h,c}` |
| Protocol reference (this file) | `MATHQUIZ_FIRMWARE_NOTES.md` |

Firmware polls `/status/mathquiz.txt` at 1000 ms, redraws only on delta, and
holds the final result ~3000 ms on `state=DEAD` / `event=match_ended` before
returning to `PollMainStatus` via `MathQuiz_vStop()`.