# `buttons` lib


## Exported handler

The handler of the **buttons** _must be set_ in the file `buttons.c`.
There are two sets of header depending on deboucing feature status.

## Dependencies

- `utils`
  - `bool.h`
  - `pinConnections.h`
- if **`_BUTTON_ENABLE_RIT_ON_PRESS`** is enabled:
  - `RIT/RIT.h`

## Usage

| Mode | Flags to be set | Handlers exported | Handlers required - reset | Handlers required - RIT |
| --- | --- | --- | --- | --- |
| legacy usage | none | `EINTx_IRQHandler` | button initialization (`BTN_enable(button btn, unsigned int priority, BTN_sensitivity sensitivity, bool enableInterrupt)`) | none |
| debounced mode - dedicated RIT | `_BUTTONS_DEBOUNCE` and `_BUTTONS_ENABLE_RIT_ON_PRESS` | `EINTx_IRQHandler_Debounced` | button initialization (`BTN_enable(button btn, unsigned int priority, BTN_sensitivity sensitivity, bool enableInterrupt)`) | `BTN_updateStatus(desired btn)` _OR_ `BTN_updateStatusAll()` |
| debounced mode - shared RIT | `_BUTTONS_DEBOUNCE` | `EINTx_IRQHandler_Debounced` |  [RIT initialization](../RIT/README.md) |  `BTN_updateStatus(desired btn)` _OR_ `BTN_updateStatusAll()` |
