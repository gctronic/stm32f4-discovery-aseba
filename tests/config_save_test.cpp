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
    mock("flash").expectOneCall("write"); // Empty map
    mock("flash").expectOneCall("write"); // CRC
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
    for (int i = 0; i < 6; i++) {
        mock("flash").expectOneCall("write");
    }

    // Saves the parameter, then change its value
    config_save(data, sizeof(data), &ns);
    parameter_integer_set(&p, 20);
    CHECK_EQUAL(20, parameter_integer_get(parameter_find(&ns, "/foo")));

    // Loads the config back from saved state
    // We add an offset to skip the CRC
    size_t offset = sizeof(uint32_t);
    parameter_msgpack_read(&ns,
                           (char *)(&data[offset]),
                           sizeof(data) - offset,
                           err_cb, NULL);

    // Check that the parameter has the same value as saved
    CHECK_EQUAL(10, parameter_integer_get(parameter_find(&ns, "/foo")));
}

TEST(ConfigSaveTestCase, CRCCheck)
{
    uint32_t crc, expected_crc;
    size_t offset = sizeof(crc);

    config_save(data, sizeof(data), &ns);

    expected_crc = crc32(0, &data[offset], sizeof(data)-offset);
    memcpy(&crc, data, sizeof(expected_crc));

    CHECK_EQUAL(expected_crc, crc);
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
    CHECK_EQUAL(20, parameter_integer_get(&foo));
    CHECK_TRUE(res);
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

