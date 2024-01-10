# `RIT` lib


## Exported handler

The handler of the **RIT** _must be set_ in the file `IRQ_RIT.c`

## Dependencies

- none

## Handlers required

In reset handler:

```c
init_RIT(TIME_50MS);
enable_RIT();
```
