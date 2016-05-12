#include <string.h>
#include "config_flash_storage.h"
#include "flash/flash.h"
#include "parameter/parameter_msgpack.h"
#include "cmp/cmp.h"
#include "cmp_mem_access/cmp_mem_access.h"
#include "crc/crc32.h"

/* We cannot use a CRC start value of 0 because CRC(0, 0xffffffff) = 0xffffffff
 * which makes empty flash pages valid. */
#define CRC_INITIAL_VALUE 0xdeadbeef
#define HEADER_SIZE (3 * sizeof(uint32_t))


static size_t cmp_flash_writer(struct cmp_ctx_s *ctx, const void *data, size_t len)
{
    cmp_mem_access_t *mem = (cmp_mem_access_t*)ctx->buf;
    if (mem->index + len <= mem->size) {
        flash_write(&mem->buf[mem->index], data, len);
        mem->index += len;
        return len;
    } else {
        return 0;
    }
}

void config_erase(void *dst)
{
    flash_unlock();
    flash_sector_erase(dst);
    flash_lock();
}

void config_save(void *dst, size_t dst_len, parameter_namespace_t *ns)
{
    cmp_ctx_t cmp;
    cmp_mem_access_t mem;
    uint32_t crc, len;
    size_t offset = 0;

    cmp_mem_access_init(&cmp, &mem,
                        dst + HEADER_SIZE, dst_len - HEADER_SIZE);

    /* Replace the RAM writer with the special writer for flash. */
    cmp.write = cmp_flash_writer;

    flash_unlock();
    flash_sector_erase(dst);
    parameter_msgpack_write_cmp(ns, &cmp, NULL, NULL);

    len = cmp_mem_access_get_pos(&mem);

    /* First write length checksum. */
    crc = crc32(CRC_INITIAL_VALUE, &len, sizeof(uint32_t));
    flash_write(dst + offset, &crc, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    /* Then write the length itself. */
    flash_write(dst + offset, &len, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    /* Then write the data checksum. */
    crc = crc32(CRC_INITIAL_VALUE, dst + HEADER_SIZE, len);
    flash_write(dst + offset, &crc, sizeof(uint32_t));

    flash_lock();
}

bool config_load(parameter_namespace_t *ns, void *src, size_t src_len)
{
    int res;

    /* Compare checksum of the data block with header. */
    if (!config_block_is_valid(src)) {
        return false;
    }

    res = parameter_msgpack_read(ns, src + HEADER_SIZE, src_len - HEADER_SIZE,
                                 NULL, NULL);

    if (res != 0) {
        return false;
    }

    return true;
}

bool config_block_is_valid(void *p)
{
    uint8_t *block = (uint8_t *)p;
    uint32_t crc, length;
    size_t offset = 0;

    /* Extract length checksum. */
    memcpy(&crc, &block[offset], sizeof(crc));
    offset += sizeof(crc);

    /* Extract length. */
    memcpy(&length, &block[offset], sizeof(length));
    offset += sizeof(length);

    /* Check that the length is valid. */
    if (crc != crc32(CRC_INITIAL_VALUE, &length, sizeof(length))) {
        return false;
    }

    /* Extract data checksum. */
    memcpy(&crc, &block[offset], sizeof(crc));
    offset += sizeof(crc);

    /* Check that the data checksum is valid. */
    if (crc != crc32(CRC_INITIAL_VALUE, &block[offset], length)) {
        return false;
    }

    return true;
}
