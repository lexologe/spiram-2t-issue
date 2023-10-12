#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_attr.h>

#include <esp_log.h>
static const char* TAG = "main";

#define TEST_BUFFER_SIZE (16*1024/4)
static EXT_RAM_NOINIT_ATTR uint32_t s_noinit_buffer[TEST_BUFFER_SIZE];
static __NOINIT_ATTR uint32_t s_noinit_firstrun[1];

extern void esp_psram_extram_writeback_cache(void);

static void write_spiram_and_reset(void)
{
    // Fill the noinit buffer
    printf("Filling buffer\n");
    for (uint32_t i = 0; i < TEST_BUFFER_SIZE; i++) {
        s_noinit_buffer[i] = i ^ 0x55555555U;
    }
    printf("Flushing cache\n");
    // Flush the cache out to SPIRAM before resetting.
    esp_psram_extram_writeback_cache();
}

static void check_spiram_contents(void)
{
    // Confirm that the memory contents are still what we expect
    uint32_t error_count = 0;
    for (uint32_t i = 0; i < TEST_BUFFER_SIZE; i++) {
        if (s_noinit_buffer[i] != (i ^ 0x55555555U)) {
            error_count++;
        }
    }
    printf("Found %" PRIu32 " memory errors\n", error_count);
}

static EXT_RAM_NOINIT_ATTR uint32_t s_noinit_buffer_2[TEST_BUFFER_SIZE];

static void write_spiram_and_reset_2(void)
{
    // Fill the noinit buffer
    printf("Filling buffer 2\n");
    for (uint32_t i = 0; i < TEST_BUFFER_SIZE; i++) {
        s_noinit_buffer_2[i] = i ^ 0x55555555U;
    }
    printf("Flushing cache\n");
    // Flush the cache out to SPIRAM before resetting.
    esp_psram_extram_writeback_cache();
}

static void check_spiram_contents_2(void)
{
    // Confirm that the memory contents are still what we expect
    uint32_t error_count = 0;
    for (uint32_t i = 0; i < TEST_BUFFER_SIZE; i++) {
        if (s_noinit_buffer_2[i] != (i ^ 0x55555555U)) {
            error_count++;
        }
    }
    printf("Found %" PRIu32 " memory errors in buffer 2\n", error_count);
}

static EXT_RAM_NOINIT_ATTR uint32_t s_noinit_buffer_3[TEST_BUFFER_SIZE];

static void write_spiram_and_reset_3(void)
{
    // Fill the noinit buffer
    printf("Filling buffer 2\n");
    for (uint32_t i = 0; i < TEST_BUFFER_SIZE; i++) {
        s_noinit_buffer_3[i] = i ^ 0x55555555U;
    }
    printf("Flushing cache\n");
    // Flush the cache out to SPIRAM before resetting.
    esp_psram_extram_writeback_cache();
}

static void check_spiram_contents_3(void)
{
    // Confirm that the memory contents are still what we expect
    uint32_t error_count = 0;
    for (uint32_t i = 0; i < TEST_BUFFER_SIZE; i++) {
        if (s_noinit_buffer_3[i] != (i ^ 0x55555555U)) {
            error_count++;
        }
    }
    printf("Found %" PRIu32 " memory errors in buffer 3\n", error_count);
}


void app_main() {

  check_spiram_contents();
  check_spiram_contents_2();
  check_spiram_contents_3();
  // ESP_LOG_BUFFER_HEXDUMP(TAG, &s_noinit_buffer[0], TEST_BUFFER_SIZE, ESP_LOG_INFO);
  if(s_noinit_firstrun[0] != 0xDEADBEEF) {
    ESP_LOGI(TAG, "====> FIRST RUN");
    s_noinit_firstrun[0] = 0xDEADBEEF;
    write_spiram_and_reset();
    write_spiram_and_reset_2();
    write_spiram_and_reset_3();

    ESP_LOGI(TAG, "====> CHECK BEFORE RESTART");
    check_spiram_contents();
    check_spiram_contents_2();
    check_spiram_contents_3();

    ESP_LOGI(TAG, "====> EXPECTED RESTART");
    // Reset to test that noinit memory is left intact.
    esp_restart();
  }

}