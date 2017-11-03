#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "parameter/parameter.h"
#include "msgbus/messagebus.h"
#include "camera/dcmi_camera.h"

/** Robot wide IPC bus. */
extern messagebus_t bus;

extern parameter_namespace_t parameter_root;

extern uint8_t txComplete;

#ifdef __cplusplus
}
#endif

#endif
