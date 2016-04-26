#include <string.h>
#include "config_flash_storage.h"
#include "flash/flash.h"
#include "parameter/parameter_msgpack.h"
#include "cmp/cmp.h"
#include "cmp_mem_access/cmp_mem_access.h"
#include "crc/crc32.h"


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
    uint32_t crc;

    cmp_mem_access_init(&cmp, &mem,
                        dst + sizeof(uint32_t), dst_len - sizeof(uint32_t));

    /* Replace the RAM writer with the special writer for flash. */
    cmp.write = cmp_flash_writer;

    flash_unlock();
    flash_sector_erase(dst);
    parameter_msgpack_write_cmp(ns, &cmp, NULL, NULL);

    crc = crc32(0, dst + sizeof(uint32_t), dst_len - sizeof(uint32_t));
    flash_write(dst, &crc, sizeof(uint32_t));

    flash_lock();
}

bool config_load(parameter_namespace_t *ns, void *src, size_t src_len)
{
    size_t offset = sizeof(uint32_t);
    uint32_t crc, expected_crc;
    int res;

    /* Compare checksum of the data block with header. */
    memcpy(&expected_crc, src, sizeof(expected_crc));
    crc = crc32(0, src + offset, src_len - offset);

    if (crc != expected_crc) {
        return false;
    }

    res = parameter_msgpack_read(ns, src + offset, src_len - offset,
                                 NULL, NULL);

    if (res != 0) {
        return false;
    }

    return true;
}
