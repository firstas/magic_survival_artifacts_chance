# magic_survival_artifacts_chance
Simulator made to calculate chances for certain conditions in average run.
Built version in .exe file.
Assumptions:
There are 112 artifacts in the whole game. Every good run player gets 40+-10 chests every run.
Runs are independent events.
Every chest contains 3 items to choose from. Exactly one has to be picked.
Not picked up are discarded, which also means, chance of them appearing again drops by half,
ut it can not become more than lower 5 times. (100% -> 50% -> 25% -> 20% -> 20% -> ...).
One of the artifacts is "key chain", which allows to reroll 3 chests.
Reroll can be performed only once per chest. When rerolled, all artifacts are discarded
and replaced with new ones.
