#ifndef DISCOVERY_NATIVES_H
#define DISCOVERY_NATIVES_H

#ifdef __cplusplus
extern "C" {
#endif

extern AsebaNativeFunctionDescription AsebaNativeDescription_set_led;
void set_led(AsebaVMState *vm);

#define DISCOVERY_NATIVES_DESCRIPTIONS \
    &AsebaNativeDescription_set_led

#define DISCOVERY_NATIVES_FUNCTIONS \
    set_led

#ifdef __cplusplus
}
#endif

#endif /* DISCOVERY_NATIVES_H */
