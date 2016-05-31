#ifndef CONFIG_SAVE_H
#define CONFIG_SAVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "parameter/parameter.h"

#define CONFIG_HEADER_SIZE (3 * sizeof(uint32_t))

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
 * @note If no valid block is found the parameter tree is unchanged.
 */
bool config_load(parameter_namespace_t *ns, void *src);

/** Returns true if the block at the given address has a valid checksum. */
bool config_block_is_valid(void *block);

/** Writes a header for the block at the address dst.
 *
 * @note The block first bytes (CONFIG_HEADER_SIZE) must be available to write
 * the checksum to the block. */
void config_write_block_header(void *dst, uint32_t len);

uint32_t config_block_get_length(void *block);

/** Returns a pointer to the first free usable area of the block. */
void *config_block_find_first_free(void *block);

/** Returns a pointer to the last used block. */
void *config_block_find_last_used(void *p);
#ifdef __cplusplus
}
#endif

#endif
