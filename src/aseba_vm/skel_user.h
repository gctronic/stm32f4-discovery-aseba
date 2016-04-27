#ifndef SKEL_USER_H
#define SKEL_USER_H

#ifdef __cplusplus
extern "C" {
#endif

extern const AsebaVMDescription vmDescription;
extern const AsebaLocalEventDescription localEvents[];

extern AsebaNativeFunctionPointer nativeFunctions[];
extern const AsebaNativeFunctionDescription* nativeFunctionsDescription[];
extern const int nativeFunctions_length;

#ifdef __cplusplus
}
#endif

#endif /* SKEL_USER_H */
