# Snake and Ladder — Firmware Notes

This document records the ground-truth Snake and Ladder protocol as actually implemented
server-side, derived from the existing (un-modified) website sources:

- `website/Projecth/snakeladder_backend.php`
- `website/Projecth/status/snakeladder.txt`
- `website/Projecth/snakeladder.js` / `snakeladder.html`
- `website/Projecth/status_helper.php`

It is the authoritative reference for the STM32 firmware in
`src/APP/SnakeAndLadder/` and must not contradict the live backend. The website is
treated as read-only here.

---

## 1. Status file — `/status/snakeladder.txt`

The backend appends a single line of semicolon-separated `key=value` pairs on
every state change (via `append_status_log()` and `write_status_file()` inside
`snakeladder_backend.php`, which sanitises values by stripping `;`, `\r`, and `\n`).

### Exact field order and value formats

```text
state=ALIVE|DEAD;game=snakeladder;room=ROOM_ID;event=EVENT_NAME;status=ROOM_STATUS;p1_pos=INTEGER;p2_pos=INTEGER;turn=TURN;last_roll=INTEGER;winner=WINNER;round=INTEGER;time=YYYY-MM-DD HH:MM:SS
```

| Key | Allowed values | Description |
| :--- | :--- | :--- |
| `state` | `ALIVE` / `DEAD` | Session lifecycle flag. `DEAD` is written on match end. |
| `game` | `snakeladder` | Game identifier (fixed). |
| `room` | alphanumeric `[A-Za-z0-9]` (sanitised) | Room code (6 chars generated; e.g. `ABC123`). |
| `event` | `idle`, `create_room`, `player_joined`, `dice_roll`, `ladder_climb`, `snake_slide`, `game_won`, `play_again`, `match_ended` | Backend trigger that caused this state transition. |
| `status` | `waiting` / `playing` / `result` / `ended` | Room/round phase. `ended` corresponds to `state=DEAD`. |
| `p1_pos` | integer (`0` to `100`) | Player 1 current tile position (`0` = start line / off-board). |
| `p2_pos` | integer (`0` to `100`) | Player 2 current tile position (`0` = start line / off-board). |
| `turn` | `P1` / `P2` / `""` | Player whose turn it is to roll (`""` when waiting or in result). |
| `last_roll` | integer (`1` to `6`) / `""` | Value of the most recent dice roll (`""` before first roll). |
| `winner` | `""` / `p1` / `p2` | Round winner (`""` during game, `p1`/`p2` when tile 100 reached). |
| `round` | integer (`0`, `1`, `2`, ...) | Current round index. |
| `time` | `YYYY-MM-DD HH:MM:SS` | Timestamp of state generation. |

### Observed snapshot (backend idle seed)

```text
state=DEAD;game=snakeladder;room=;event=idle;status=waiting;p1_pos=0;p2_pos=0;turn=;last_roll=;winner=;round=0;time=2026-09-15 00:00:00
```

---

## 2. Board Representation & Layout

The board is a classic **10x10 grid with 100 tiles** numbered 1 to 100 in serpentine (boustrophedon) order:
- Row 0 (bottom row): tiles 1 to 10 (left to right, cols 0..9).
- Row 1: tiles 20 down to 11 (right to left, cols 9..0).
- Row 2: tiles 21 to 30 (left to right, cols 0..9).
- ...
- Row 9 (top row): tiles 100 down to 91 (right to left, with tile 100 at col 0, top-left).
- Tile 0 represents the pre-game starting bench (outside the board).

### Fixed Snakes and Ladders Map

The snake and ladder endpoints are statically defined in `snakeladder_backend.php` and shared with the client. Therefore, the firmware represents them as a static compile-time lookup table rather than parsing dynamic map definitions over HTTP.

#### Ladders (bottom -> top)
- Tile 3  -> Tile 22 (+19)
- Tile 8  -> Tile 30 (+22)
- Tile 15 -> Tile 44 (+29)
- Tile 28 -> Tile 55 (+27)
- Tile 36 -> Tile 57 (+21)
- Tile 51 -> Tile 72 (+21)
- Tile 62 -> Tile 81 (+19)
- Tile 71 -> Tile 91 (+20)

#### Snakes (head -> tail)
- Tile 27 -> Tile 5  (-22)
- Tile 39 -> Tile 17 (-22)
- Tile 48 -> Tile 26 (-22)
- Tile 58 -> Tile 37 (-21)
- Tile 66 -> Tile 45 (-21)
- Tile 79 -> Tile 60 (-19)
- Tile 89 -> Tile 53 (-36)
- Tile 99 -> Tile 41 (-58)

---

## 3. Game Mechanics & Event Signaling

1. **Room Creation & Join**:
   - `action=create` initializes room with `positions=[0,0]`, `status=waiting`, `event=create_room`.
   - `action=join` adds Player 2, shifts `status=playing`, `turn=P1`, `event=player_joined`.
2. **Dice Rolls & Turns**:
   - Rolling player triggers `action=roll`.
   - Dice generates integer 1..6.
   - Exact roll required to reach tile 100 (`rawLanding <= 100`). Overshooting causes the token to bounce / stay in place.
   - Turn alternates between `P1` and `P2` unless a win occurs.
3. **Snake & Ladder Signaling**:
   - When a plain move occurs without a jump: `event=dice_roll`.
   - When landing on a ladder start: backend writes `event=dice_roll` then `event=ladder_climb`.
   - When landing on a snake start: backend writes `event=dice_roll` then `event=snake_slide`.
   - The firmware can detect the jump type either directly from the `event` field (`ladder_climb` / `snake_slide`) or by checking the static map against the player's previous position + dice roll.
4. **Win Condition & Match End**:
   - Reaching tile 100 triggers `status=result`, `winner=p1` or `p2`, and `event=game_won`.
   - Resetting for next round (`action=next_round`) resets positions to 0, increments `round`, and sets `event=play_again`.
   - Ending the match (`action=end_game`) sets `status=ended`, `state=DEAD`, and `event=match_ended`.

---

## 4. TFT Display & Renderer Layout (128x160 ST7735)

- **Header / Top Bar (Y: 0..16)**:
  - Turn indicator (`P1 TURN` / `P2 TURN`) or game state (`WAITING`, `P1 WINS!`).
  - Dice roll value (`DICE: X`).
- **Board Grid (Y: 18..137)**:
  - 10x10 squares of 12x12 pixels each (Total: 120x120 pixels).
  - Centered horizontally at X: 4..123 (leaving 4 px margin on each side).
  - Drawn statically at game start (alternating checkerboard pattern).
  - Procedural lines for ladders (e.g. golden/brown parallel lines with rungs) and snakes (green wavy paths / circles).
  - Player tokens: P1 (Cyan circle) and P2 (Orange/Yellow circle) drawn with an offset inside the tile so both are visible if they occupy the same square.
  - Targeted partial redraw: clear old token positions by restoring the tile background, then draw new token positions.
- **Footer / Status Bar (Y: 140..159)**:
  - Player positions: `P1: <pos>` and `P2: <pos>`.
  - Event banner: e.g. `LADDER!`, `SNAKE!`, `WINNER!`.

---

## 5. Firmware Module Map

| Concern | Location |
| :--- | :--- |
| Server-state struct + delta detection | `src/APP/SnakeAndLadder/game_int.h`, `game_prg.c` |
| Rendering (board, tokens, snakes/ladders, dice, banners) | `src/APP/SnakeAndLadder/Animation/SnakeLadder_Animation_*.{h,c}` |
| Polling / dispatch integration | `src/APP/main.c` (`IsSnakeLadderGame`, `ParseSnakeLadderState`, `SnakeLadder_vStart`/`vStop` loop) |
| Shared 3x5 font engine | `src/APP/Common/FONT3x5.{h,c}` |
| Shared loading animation spinner | `src/APP/Common/LOADING_ANIM.{h,c}` |
| Protocol reference (this file) | `SNAKELADDER_FIRMWARE_NOTES.md` |

