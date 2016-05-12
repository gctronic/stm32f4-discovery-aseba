#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include "config_flash_storage.h"
#include "parameter/parameter_msgpack.h"
#include <cstdio>
#include <cstring>
#include "crc/crc32.h"

TEST_GROUP(ConfigSaveTestCase)
{
    uint8_t data[128];
    parameter_namespace_t ns;

    void setup()
    {
        mock("flash").ignoreOtherCalls();
        parameter_namespace_declare(&ns, NULL, NULL);
    }
};

TEST(ConfigSaveTestCase, SavingConfigErasesPages)
{
    mock("flash").expectOneCall("erase").withParameter("sector", data);

    config_save(data, sizeof(data), &ns);
}

TEST(ConfigSaveTestCase, SavingConfigLockUnlock)
{
    mock("flash").strictOrder();

    mock("flash").expectOneCall("unlock");
    mock("flash").expectOneCall("erase").withParameter("sector", data);
    mock("flash").expectOneCall("write"); // data
    mock("flash").expectOneCall("write"); // CRC(len)
    mock("flash").expectOneCall("write"); // len
    mock("flash").expectOneCall("write"); // crc(data)
    mock("flash").expectOneCall("lock");

    config_save(data, sizeof(data), &ns);

    // We need to manually check expectations since we tuned the flash mock to preserve ordering.
    mock("flash").checkExpectations();
}

static void err_cb(void *p, const char *id, const char *err)
{
    (void) p;
    (void) id;
    (void) err;
    char msg[128];
    snprintf(msg, sizeof msg, "MessagePack error on item \"%s\": \"%s\"", id, err);

    FAIL(msg);
}

TEST(ConfigSaveTestCase, SavingConfigWorks)
{
    // Declare a parameter and gives it a value.
    parameter_t p;
    parameter_integer_declare(&p, &ns, "foo");
    parameter_integer_set(&p, 10);

    // Check that the flash writer is used
    // Number of expected written bytes is implementation-dependent
    // Change if if necessary
    for (int i = 0; i < 8; i++) {
        mock("flash").expectOneCall("write");
    }

    // Saves the parameter, then change its value
    config_save(data, sizeof(data), &ns);
    parameter_integer_set(&p, 20);
    CHECK_EQUAL(20, parameter_integer_get(parameter_find(&ns, "/foo")));

    // Loads the config back from saved state
    // We add an offset to skip the CRC and the block length
    parameter_msgpack_read(&ns,
                           (char *)(&data[CONFIG_HEADER_SIZE]),
                           sizeof(data) - CONFIG_HEADER_SIZE,
                           err_cb, NULL);

    // Check that the parameter has the same value as saved
    CHECK_EQUAL(10, parameter_integer_get(parameter_find(&ns, "/foo")));
}

TEST_GROUP(ConfigLoadTestCase)
{
    uint8_t data[128];
    parameter_namespace_t ns;
    parameter_t foo;

    void setup()
    {
        mock("flash").ignoreOtherCalls();
        parameter_namespace_declare(&ns, NULL, NULL);
        parameter_integer_declare(&foo, &ns, "foo");
    }
};

TEST(ConfigLoadTestCase, SimpleLoad)
{
    // Set a value, then save it
    parameter_integer_set(&foo, 20);
    config_save(data, sizeof(data), &ns);

    // Change the value
    parameter_integer_set(&foo, 10);

    // Load the tree
    auto res = config_load(&ns, data, sizeof(data));

    // Value should be back to what it was
    CHECK_TRUE(res);
    CHECK_EQUAL(20, parameter_integer_get(&foo));
}

TEST(ConfigLoadTestCase, CRCIsChecked)
{
    // Set a value, then save it
    parameter_integer_set(&foo, 20);
    config_save(data, sizeof(data), &ns);

    // Change the value
    parameter_integer_set(&foo, 10);

    // Corrupt data
    data[0] ^= 0x40;

    // Load the tree
    auto res = config_load(&ns, data, sizeof(data));

    // Value should not have changed
    CHECK_FALSE(res);
    CHECK_EQUAL(10, parameter_integer_get(&foo));
}

TEST(ConfigLoadTestCase, InvalidConfigReturnsFalse)
{
    // This test checks that loading an invalid config fails, even if the
    // checksum is correct.
    // This might be the case if the layout of the parameter tree changes for
    // example.
    parameter_integer_set(&foo, 20);
    config_save(data, sizeof(data), &ns);

    // Create a new different tree
    parameter_namespace_declare(&ns, NULL, NULL);
    parameter_integer_declare(&foo, &ns, "bar");

    // Try to load it
    auto res = config_load(&ns, data, sizeof(data));

    CHECK_FALSE(res);
}

TEST_GROUP(BlockValidityTestGroup)
{
    uint8_t block[256 + CONFIG_HEADER_SIZE];

    void setup()
    {
        memset(block, 0, sizeof(block));

        mock("flash").disable();
        config_write_block_header(block, sizeof(block) - CONFIG_HEADER_SIZE);
        mock("flash").enable();
    }

};

TEST(BlockValidityTestGroup, DefaultBlockIsValid)
{
    CHECK_TRUE(config_block_is_valid(block));
}

TEST(BlockValidityTestGroup, InvalidLengthChecksumMakesItInvalid)
{
    block[0] ^= 0xaa;
    CHECK_FALSE(config_block_is_valid(block));
}

TEST(BlockValidityTestGroup, InvalidLengthIsInvalid)
{
    block[5] ^= 0xaa;
    CHECK_FALSE(config_block_is_valid(block));
}

TEST(BlockValidityTestGroup, InvalidDataCheckSumIsInvalid)
{
    block[127] ^= 0xaa;
    CHECK_FALSE(config_block_is_valid(block));
}

TEST(BlockValidityTestGroup, FreshFlashIsInvalid)
{
    /* Checks that a fresh flash page (all 0xff) is invalid. */
    memset(block, 0xff, sizeof(block));
    CHECK_FALSE(config_block_is_valid(block));
}
