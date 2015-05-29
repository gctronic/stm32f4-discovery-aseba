#ifndef DISCOVERY_NATIVES_H
#define DISCOVERY_NATIVES_H

#ifdef __cplusplus
extern "C" {
#endif

extern AsebaNativeFunctionDescription AsebaNativeDescription_set_led;
void set_led(AsebaVMState *vm);

extern AsebaNativeFunctionDescription AsebaNativeDescription_clear_all_leds;
void clear_all_leds(AsebaVMState *vm);

#define DISCOVERY_NATIVES_DESCRIPTIONS \
    &AsebaNativeDescription_set_led, \
    &AsebaNativeDescription_clear_all_leds

#define DISCOVERY_NATIVES_FUNCTIONS \
    set_led, \
    clear_all_leds

#ifdef __cplusplus
}
#endif

#endif /* DISCOVERY_NATIVES_H */
