#ifndef CONFIG_SAVE_H
#define CONFIG_SAVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "parameter/parameter.h"

/** Erase config sector.
 */
void config_erase(void *dst);

/** Writes the given parameter namespace to flash , prepending it with a CRC
 * for integrity checks.
 */
void config_save(void *dst, size_t dst_len, parameter_namespace_t *ns);

/** Loads the configuration from the flash
 *
 * @returns true if the operation was successful.
 * @note If the CRC value does not match the parameter tree is unchanged.
 */
bool config_load(parameter_namespace_t *ns, void *src, size_t src_len);

/** Returns true if the block at the given address has a valid checksum. */
bool config_block_is_valid(void *block);

#ifdef __cplusplus
}
#endif

#endif
