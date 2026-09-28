# Testing Cheats

Purpose: keep a small, explicit cheat list for gameplay-repro routes where survival or power level is blocking debugging. These are for local testing only, not normal gameplay defaults. On first launch, LodRecomp creates `cheats.cfg` in the active config folder with every cheat line commented out.

## Recommended enable list

| Enable for repro | Cheat | GameShark / Action Replay code | Current LodRecomp support | Notes |
|---|---|---:|---|---|
| Yes | Infinite health / energy | `811CAB3A 2AF8` | Implemented: `infinite_health = true` or `LOD_CHEAT_INFINITE_HEALTH=1` | Re-applied every VI. Best first choice when the goal is just to survive until a suspected crash or cutscene. |
| Yes | Invincibility | `8102A9D0 1000` | Implemented: `invincibility = true` or `LOD_CHEAT_INVINCIBILITY=1` | Stronger than health refill. Use when hit reactions or damage routines interrupt repro movement. |
| Maybe | Do not take damage | `8108B81C 2400` | Implemented: `no_damage = true` or `LOD_CHEAT_NO_DAMAGE=1` | Alternative to invincibility. Use as a comparison if invincibility changes behavior too much. |
| Yes | Max power-ups | `801CAE23 0002` | Implemented: `max_power = true` or `LOD_CHEAT_MAX_POWER=1` | Sets the current power-up level to max. Useful for boss-route repros. |
| Maybe | Infinite red jewels | `801CAB45 0064` | Implemented: `infinite_red_jewels = true` or `LOD_CHEAT_INFINITE_RED_JEWELS=1` | Useful if sub-weapon use is required to reach or survive a repro. |
| Maybe | Infinite money | `811CAB42 FFFF` | Implemented: `infinite_money = true` or `LOD_CHEAT_INFINITE_MONEY=1` | Useful for merchant/Renon route testing, but avoid when reproducing money-dependent Renon behavior. |
| Maybe | Have all and infinite items | `50002A01 0000`<br>`801CAB47 0001` | Implemented: `have_all_items = true` or `LOD_CHEAT_HAVE_ALL_ITEMS=1` | Broad state mutation. Prefer targeted health/power cheats first. |

## Config file

`cheats.cfg` is written next to `graphics.json`, `controls.json`, and `audio.json`. Every line is commented out by default. To enable a cheat, remove the leading `#` and set it to `true`, for example:

```ini
infinite_health = true
max_power = true
```

## Launch examples

Environment variables still override `cheats.cfg` for diagnostics:

```sh
LOD_CHEAT_INFINITE_HEALTH=1 ./build/LodRecomp
```

If adding more runtime toggles, keep them explicit and default-off, for example:

```sh
LOD_CHEAT_INFINITE_HEALTH=1 \
LOD_CHEAT_INVINCIBILITY=1 \
LOD_CHEAT_MAX_POWER=1 \
./build/LodRecomp
```

## Source notes

- Implemented runtime cheats re-apply the LoD USA GameShark/Action Replay writes each VI. `cheats.cfg` is the player-facing default-off path; matching `LOD_CHEAT_*` environment variables override it for diagnostics.
- External code lists agree on the key testing cheats above: GameHacking.org lists infinite energy, invincibility, no-damage, max power-ups, red jewels, money, and all-items codes; Cheat Code Central lists max power-ups as `801CAE23 0002`; CodeJunkies lists the alternate infinite-health value `811CAB3A 2710`.
- Prefer one cheat at a time while debugging state bugs. Broad inventory or money cheats can mask route/event problems.
