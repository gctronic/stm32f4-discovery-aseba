/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for STMicroelectronics STM32F4-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F4_DISCOVERY
#define BOARD_NAME                  "STMicroelectronics STM32F4-Discovery"

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0U
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F407xx

/*
 * IO pins assignments.
 */
#define GPIOA_BUTTON                0U
#define GPIOA_PIN1                  1U
#define GPIOA_PIN2                  2U
#define GPIOA_OTG_HS_ULPI_D0        3U
#define GPIOA_DCMI_HSYNC            4U
#define GPIOA_OTG_HS_ULPI_CLK       5U
#define GPIOA_DCMI_PIXCLK           6U
#define GPIOA_SPI1_MOSI             7U
#define GPIOA_I2C3_SCL              8U
#define GPIOA_VBUS_FS               9U
#define GPIOA_OTG_FS_ID             10U
#define GPIOA_OTG_FS_DM             11U
#define GPIOA_OTG_FS_DP             12U
#define GPIOA_SWDIO                 13U
#define GPIOA_SWCLK                 14U
#define GPIOA_Output_Enable_2       15U

#define GPIOB_OTG_HS_ULPI_D1        0U
#define GPIOB_OTG_HS_ULPI_D2        1U
#define GPIOB_PIN2                  2U
#define GPIOB_SPI1_CLK              3U
#define GPIOB_SPI1_MISO             4U
#define GPIOB_OTG_HS_ULPI_D7        5U
#define GPIOB_DCMI_D5               6U
#define GPIOB_DCMI_VSYNC            7U
#define GPIOB_I2C1_SCL              8U
#define GPIOB_I2C1_SDA              9U
#define GPIOB_OTG_HS_ULPI_D3        10U
#define GPIOB_OTG_HS_ULPI_D4        11U
#define GPIOB_OTG_HS_ULPI_D5        12U
#define GPIOB_OTG_HS_ULPI_D6        13U
#define GPIOB_PIN14                 14U
#define GPIOB_PIN15                 15U

#define GPIOC_OTG_HS_ULPI_STP       0U
#define GPIOC_PIN1                  1U
#define GPIOC_OTG_HS_ULPI_DIR       2U
#define GPIOC_OTG_HS_ULPI_NXT       3U
#define GPIOC_PIN4                  4U
#define GPIOC_PIN5                  5U
#define GPIOC_DCMI_D0               6U
#define GPIOC_DCMI_D1               7U
#define GPIOC_PIN8                  8U
#define GPIOC_I2C3_SDA              9U
#define GPIOC_SPI3_CLK              10U
#define GPIOC_SPI3_MISO             11U
#define GPIOC_SPI3_MOSI             12U
#define GPIOC_PIN13                 13U
#define GPIOC_PIN14                 14U
#define GPIOC_PIN15                 15U

#define GPIOD_CAN1_RX               0U
#define GPIOD_CAN1_TX               1U
#define GPIOD_INT_ENC               2U
#define GPIOD_SPI3_CS2              3U
#define GPIOD_INT_IMU               4U
#define GPIOD_SPI3_CS1              5U
#define GPIOD_SPI1_CS1              6U
#define GPIOD_Reset_2               7U
#define GPIOD_StandBy_1             8U
#define GPIOD_PIN9                  9U
#define GPIOD_Output_Enable_1       10U
#define GPIOD_Trigger_1             11U
#define GPIOD_LED4                  12U
#define GPIOD_LED3                  13U
#define GPIOD_LED5                  14U
#define GPIOD_LED6                  15U

#define GPIOE_DCMI_D2               0U
#define GPIOE_DCMI_D3               1U
#define GPIOE_StandBy_2             2U
#define GPIOE_Trigger_2             3U
#define GPIOE_DCMI_D4               4U
#define GPIOE_DCMI_D6               5U
#define GPIOE_DCMI_D7               6U
#define GPIOE_Reset_1               7U
#define GPIOE_PIN8                  8U
#define GPIOE_Master_Clock_DCMI     9U
#define GPIOE_PIN10                 10U
#define GPIOE_PIN11                 11U
#define GPIOE_PIN12                 12U
#define GPIOE_PIN13                 13U
#define GPIOE_PIN14                 14U
#define GPIOE_PIN15                 15U

#define GPIOF_PIN0                  0U
#define GPIOF_PIN1                  1U
#define GPIOF_PIN2                  2U
#define GPIOF_PIN3                  3U
#define GPIOF_PIN4                  4U
#define GPIOF_PIN5                  5U
#define GPIOF_PIN6                  6U
#define GPIOF_PIN7                  7U
#define GPIOF_PIN8                  8U
#define GPIOF_PIN9                  9U
#define GPIOF_PIN10                 10U
#define GPIOF_PIN11                 11U
#define GPIOF_PIN12                 12U
#define GPIOF_PIN13                 13U
#define GPIOF_PIN14                 14U
#define GPIOF_PIN15                 15U

#define GPIOG_PIN0                  0U
#define GPIOG_PIN1                  1U
#define GPIOG_PIN2                  2U
#define GPIOG_PIN3                  3U
#define GPIOG_PIN4                  4U
#define GPIOG_PIN5                  5U
#define GPIOG_PIN6                  6U
#define GPIOG_PIN7                  7U
#define GPIOG_PIN8                  8U
#define GPIOG_PIN9                  9U
#define GPIOG_PIN10                 10U
#define GPIOG_PIN11                 11U
#define GPIOG_PIN12                 12U
#define GPIOG_PIN13                 13U
#define GPIOG_PIN14                 14U
#define GPIOG_PIN15                 15U

#define GPIOH_OSC_IN                0U
#define GPIOH_OSC_OUT               1U
#define GPIOH_PIN2                  2U
#define GPIOH_PIN3                  3U
#define GPIOH_PIN4                  4U
#define GPIOH_PIN5                  5U
#define GPIOH_PIN6                  6U
#define GPIOH_PIN7                  7U
#define GPIOH_PIN8                  8U
#define GPIOH_PIN9                  9U
#define GPIOH_PIN10                 10U
#define GPIOH_PIN11                 11U
#define GPIOH_PIN12                 12U
#define GPIOH_PIN13                 13U
#define GPIOH_PIN14                 14U
#define GPIOH_PIN15                 15U

#define GPIOI_PIN0                  0U
#define GPIOI_PIN1                  1U
#define GPIOI_PIN2                  2U
#define GPIOI_PIN3                  3U
#define GPIOI_PIN4                  4U
#define GPIOI_PIN5                  5U
#define GPIOI_PIN6                  6U
#define GPIOI_PIN7                  7U
#define GPIOI_PIN8                  8U
#define GPIOI_PIN9                  9U
#define GPIOI_PIN10                 10U
#define GPIOI_PIN11                 11U
#define GPIOI_PIN12                 12U
#define GPIOI_PIN13                 13U
#define GPIOI_PIN14                 14U
#define GPIOI_PIN15                 15U

/*
 * IO lines assignments.
 */
#define LINE_BUTTON                 PAL_LINE(GPIOA, 0U)
#define LINE_OTG_HS_ULPI_D0         PAL_LINE(GPIOA, 3U)
#define LINE_DCMI_HSYNC             PAL_LINE(GPIOA, 4U)
#define LINE_OTG_HS_ULPI_CLK        PAL_LINE(GPIOA, 5U)
#define LINE_DCMI_PIXCLK            PAL_LINE(GPIOA, 6U)
#define LINE_SPI1_MOSI              PAL_LINE(GPIOA, 7U)
#define LINE_I2C3_SCL               PAL_LINE(GPIOA, 8U)
#define LINE_VBUS_FS                PAL_LINE(GPIOA, 9U)
#define LINE_OTG_FS_ID              PAL_LINE(GPIOA, 10U)
#define LINE_OTG_FS_DM              PAL_LINE(GPIOA, 11U)
#define LINE_OTG_FS_DP              PAL_LINE(GPIOA, 12U)
#define LINE_SWDIO                  PAL_LINE(GPIOA, 13U)
#define LINE_SWCLK                  PAL_LINE(GPIOA, 14U)
#define LINE_Output_Enable_2        PAL_LINE(GPIOA, 15U)

#define LINE_OTG_HS_ULPI_D1         PAL_LINE(GPIOB, 0U)
#define LINE_OTG_HS_ULPI_D2         PAL_LINE(GPIOB, 1U)
#define LINE_SPI1_CLK               PAL_LINE(GPIOB, 3U)
#define LINE_SPI1_MISO              PAL_LINE(GPIOB, 4U)
#define LINE_OTG_HS_ULPI_D7         PAL_LINE(GPIOB, 5U)
#define LINE_DCMI_D5                PAL_LINE(GPIOB, 6U)
#define LINE_DCMI_VSYNC             PAL_LINE(GPIOB, 7U)
#define LINE_I2C1_SCL               PAL_LINE(GPIOB, 8U)
#define LINE_I2C1_SDA               PAL_LINE(GPIOB, 9U)
#define LINE_OTG_HS_ULPI_D3         PAL_LINE(GPIOB, 10U)
#define LINE_OTG_HS_ULPI_D4         PAL_LINE(GPIOB, 11U)
#define LINE_OTG_HS_ULPI_D5         PAL_LINE(GPIOB, 12U)
#define LINE_OTG_HS_ULPI_D6         PAL_LINE(GPIOB, 13U)

#define LINE_OTG_HS_ULPI_STP        PAL_LINE(GPIOC, 0U)
#define LINE_OTG_HS_ULPI_DIR        PAL_LINE(GPIOC, 2U)
#define LINE_OTG_HS_ULPI_NXT        PAL_LINE(GPIOC, 3U)
#define LINE_DCMI_D0                PAL_LINE(GPIOC, 6U)
#define LINE_DCMI_D1                PAL_LINE(GPIOC, 7U)
#define LINE_I2C3_SDA               PAL_LINE(GPIOC, 9U)
#define LINE_SPI3_CLK               PAL_LINE(GPIOC, 10U)
#define LINE_SPI3_MISO              PAL_LINE(GPIOC, 11U)
#define LINE_SPI3_MOSI              PAL_LINE(GPIOC, 12U)

#define LINE_CAN1_RX                PAL_LINE(GPIOD, 0U)
#define LINE_CAN1_TX                PAL_LINE(GPIOD, 1U)
#define LINE_INT_ENC                PAL_LINE(GPIOD, 2U)
#define LINE_SPI3_CS2               PAL_LINE(GPIOD, 3U)
#define LINE_INT_IMU                PAL_LINE(GPIOD, 4U)
#define LINE_SPI3_CS1               PAL_LINE(GPIOD, 5U)
#define LINE_SPI1_CS1               PAL_LINE(GPIOD, 6U)
#define LINE_Reset_2                PAL_LINE(GPIOD, 7U)
#define LINE_StandBy_1              PAL_LINE(GPIOD, 8U)
#define LINE_Output_Enable_1        PAL_LINE(GPIOD, 10U)
#define LINE_Trigger_1              PAL_LINE(GPIOD, 11U)
#define LINE_LED4                   PAL_LINE(GPIOD, 12U)
#define LINE_LED3                   PAL_LINE(GPIOD, 13U)
#define LINE_LED5                   PAL_LINE(GPIOD, 14U)
#define LINE_LED6                   PAL_LINE(GPIOD, 15U)

#define LINE_DCMI_D2                PAL_LINE(GPIOE, 0U)
#define LINE_DCMI_D3                PAL_LINE(GPIOE, 1U)
#define LINE_StandBy_2              PAL_LINE(GPIOE, 2U)
#define LINE_Trigger_2              PAL_LINE(GPIOE, 3U)
#define LINE_DCMI_D4                PAL_LINE(GPIOE, 4U)
#define LINE_DCMI_D6                PAL_LINE(GPIOE, 5U)
#define LINE_DCMI_D7                PAL_LINE(GPIOE, 6U)
#define LINE_Reset_1                PAL_LINE(GPIOE, 7U)
#define LINE_Master_Clock_DCMI      PAL_LINE(GPIOE, 9U)



#define LINE_OSC_IN                 PAL_LINE(GPIOH, 0U)
#define LINE_OSC_OUT                PAL_LINE(GPIOH, 1U)


/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 *
 * PA0  - BUTTON                    (input floating).
 * PA1  - PIN1                      (input floating).
 * PA2  - PIN2                      (input floating).
 * PA3  - OTG_HS_ULPI_D0            (alternate 10).
 * PA4  - DCMI_HSYNC                (alternate 13).
 * PA5  - OTG_HS_ULPI_CLK           (alternate 10).
 * PA6  - DCMI_PIXCLK               (alternate 13).
 * PA7  - SPI1_MOSI                 (alternate 5).
 * PA8  - I2C3_SCL                  (alternate 4).
 * PA9  - VBUS_FS                   (input floating).
 * PA10 - OTG_FS_ID                 (alternate 10).
 * PA11 - OTG_FS_DM                 (alternate 10).
 * PA12 - OTG_FS_DP                 (alternate 10).
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 * PA15 - Output_Enable_2           (output pushpull maximum).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_BUTTON) |         \
                                     PIN_MODE_INPUT(GPIOA_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN2) |           \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_HS_ULPI_D0) |\
                                     PIN_MODE_ALTERNATE(GPIOA_DCMI_HSYNC) | \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_HS_ULPI_CLK) |\
                                     PIN_MODE_ALTERNATE(GPIOA_DCMI_PIXCLK) |\
                                     PIN_MODE_ALTERNATE(GPIOA_SPI1_MOSI) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_I2C3_SCL) |   \
                                     PIN_MODE_INPUT(GPIOA_VBUS_FS) |        \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_ID) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DM) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DP) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |      \
                                     PIN_MODE_OUTPUT(GPIOA_Output_Enable_2))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_BUTTON) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_HS_ULPI_D0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_DCMI_HSYNC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_HS_ULPI_CLK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_DCMI_PIXCLK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI1_MOSI) |  \
                                     PIN_OTYPE_OPENDRAIN(GPIOA_I2C3_SCL) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_VBUS_FS) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_ID) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DM) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DP) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_Output_Enable_2))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_BUTTON) |        \
                                     PIN_OSPEED_HIGH(GPIOA_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOA_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_HS_ULPI_D0) |\
                                     PIN_OSPEED_HIGH(GPIOA_DCMI_HSYNC) |    \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_HS_ULPI_CLK) |\
                                     PIN_OSPEED_HIGH(GPIOA_DCMI_PIXCLK) |   \
                                     PIN_OSPEED_HIGH(GPIOA_SPI1_MOSI) |     \
                                     PIN_OSPEED_HIGH(GPIOA_I2C3_SCL) |      \
                                     PIN_OSPEED_HIGH(GPIOA_VBUS_FS) |       \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_ID) |     \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DM) |     \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DP) |     \
                                     PIN_OSPEED_HIGH(GPIOA_SWDIO) |         \
                                     PIN_OSPEED_HIGH(GPIOA_SWCLK) |         \
                                     PIN_OSPEED_HIGH(GPIOA_Output_Enable_2))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_BUTTON) |     \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_HS_ULPI_D0) |\
                                     PIN_PUPDR_PULLUP(GPIOA_DCMI_HSYNC) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_HS_ULPI_CLK) |\
                                     PIN_PUPDR_PULLUP(GPIOA_DCMI_PIXCLK) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_SPI1_MOSI) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_I2C3_SCL) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_VBUS_FS) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_ID) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DM) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DP) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_SWDIO) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_SWCLK) |      \
                                     PIN_PUPDR_PULLDOWN(GPIOA_Output_Enable_2))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_BUTTON) |           \
                                     PIN_ODR_HIGH(GPIOA_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOA_OTG_HS_ULPI_D0) |   \
                                     PIN_ODR_HIGH(GPIOA_DCMI_HSYNC) |       \
                                     PIN_ODR_HIGH(GPIOA_OTG_HS_ULPI_CLK) |  \
                                     PIN_ODR_HIGH(GPIOA_DCMI_PIXCLK) |      \
                                     PIN_ODR_HIGH(GPIOA_SPI1_MOSI) |        \
                                     PIN_ODR_HIGH(GPIOA_I2C3_SCL) |         \
                                     PIN_ODR_HIGH(GPIOA_VBUS_FS) |          \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_ID) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DM) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DP) |        \
                                     PIN_ODR_HIGH(GPIOA_SWDIO) |            \
                                     PIN_ODR_HIGH(GPIOA_SWCLK) |            \
                                     PIN_ODR_LOW(GPIOA_Output_Enable_2))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_BUTTON, 0) |         \
                                     PIN_AFIO_AF(GPIOA_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOA_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOA_OTG_HS_ULPI_D0, 10) |\
                                     PIN_AFIO_AF(GPIOA_DCMI_HSYNC, 13) |    \
                                     PIN_AFIO_AF(GPIOA_OTG_HS_ULPI_CLK, 10) |\
                                     PIN_AFIO_AF(GPIOA_DCMI_PIXCLK, 13) |   \
                                     PIN_AFIO_AF(GPIOA_SPI1_MOSI, 5))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_I2C3_SCL, 4) |       \
                                     PIN_AFIO_AF(GPIOA_VBUS_FS, 0) |        \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_ID, 10) |     \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DM, 10) |     \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP, 10) |     \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0) |          \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0) |          \
                                     PIN_AFIO_AF(GPIOA_Output_Enable_2, 0))

/*
 * GPIOB setup:
 *
 * PB0  - OTG_HS_ULPI_D1            (alternate 10).
 * PB1  - OTG_HS_ULPI_D2            (alternate 10).
 * PB2  - PIN2                      (input pullup).
 * PB3  - SPI1_CLK                  (alternate 5).
 * PB4  - SPI1_MISO                 (alternate 5).
 * PB5  - OTG_HS_ULPI_D7            (alternate 10).
 * PB6  - DCMI_D5                   (alternate 13).
 * PB7  - DCMI_VSYNC                (alternate 13).
 * PB8  - I2C1_SCL                  (alternate 4).
 * PB9  - I2C1_SDA                  (alternate 4).
 * PB10 - OTG_HS_ULPI_D3            (alternate 10).
 * PB11 - OTG_HS_ULPI_D4            (alternate 10).
 * PB12 - OTG_HS_ULPI_D5            (alternate 10).
 * PB13 - OTG_HS_ULPI_D6            (alternate 10).
 * PB14 - PIN14                     (input pullup).
 * PB15 - PIN15                     (input pullup).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ALTERNATE(GPIOB_OTG_HS_ULPI_D1) |\
                                     PIN_MODE_ALTERNATE(GPIOB_OTG_HS_ULPI_D2) |\
                                     PIN_MODE_INPUT(GPIOB_PIN2) |           \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI1_CLK) |   \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI1_MISO) |  \
                                     PIN_MODE_ALTERNATE(GPIOB_OTG_HS_ULPI_D7) |\
                                     PIN_MODE_ALTERNATE(GPIOB_DCMI_D5) |    \
                                     PIN_MODE_ALTERNATE(GPIOB_DCMI_VSYNC) | \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C1_SCL) |   \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C1_SDA) |   \
                                     PIN_MODE_ALTERNATE(GPIOB_OTG_HS_ULPI_D3) |\
                                     PIN_MODE_ALTERNATE(GPIOB_OTG_HS_ULPI_D4) |\
                                     PIN_MODE_ALTERNATE(GPIOB_OTG_HS_ULPI_D5) |\
                                     PIN_MODE_ALTERNATE(GPIOB_OTG_HS_ULPI_D6) |\
                                     PIN_MODE_INPUT(GPIOB_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN15))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_ULPI_D1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_ULPI_D2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI1_CLK) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI1_MISO) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_ULPI_D7) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_DCMI_D5) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_DCMI_VSYNC) | \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C1_SCL) |  \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C1_SDA) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_ULPI_D3) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_ULPI_D4) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_ULPI_D5) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_ULPI_D6) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN15))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_HIGH(GPIOB_OTG_HS_ULPI_D1) |\
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_ULPI_D2) |\
                                     PIN_OSPEED_HIGH(GPIOB_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOB_SPI1_CLK) |      \
                                     PIN_OSPEED_HIGH(GPIOB_SPI1_MISO) |     \
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_ULPI_D7) |\
                                     PIN_OSPEED_HIGH(GPIOB_DCMI_D5) |       \
                                     PIN_OSPEED_HIGH(GPIOB_DCMI_VSYNC) |    \
                                     PIN_OSPEED_HIGH(GPIOB_I2C1_SCL) |      \
                                     PIN_OSPEED_HIGH(GPIOB_I2C1_SDA) |      \
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_ULPI_D3) |\
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_ULPI_D4) |\
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_ULPI_D5) |\
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_ULPI_D6) |\
                                     PIN_OSPEED_HIGH(GPIOB_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_OTG_HS_ULPI_D1) |\
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_ULPI_D2) |\
                                     PIN_PUPDR_PULLUP(GPIOB_PIN2) |         \
                                     PIN_PUPDR_FLOATING(GPIOB_SPI1_CLK) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_SPI1_MISO) |  \
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_ULPI_D7) |\
                                     PIN_PUPDR_PULLUP(GPIOB_DCMI_D5) |      \
                                     PIN_PUPDR_PULLUP(GPIOB_DCMI_VSYNC) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SCL) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SDA) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_ULPI_D3) |\
                                     PIN_PUPDR_PULLUP(GPIOB_OTG_HS_ULPI_D4) |\
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_ULPI_D5) |\
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_ULPI_D6) |\
                                     PIN_PUPDR_PULLUP(GPIOB_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN15))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_OTG_HS_ULPI_D1) |   \
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_ULPI_D2) |   \
                                     PIN_ODR_HIGH(GPIOB_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOB_SPI1_CLK) |         \
                                     PIN_ODR_HIGH(GPIOB_SPI1_MISO) |        \
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_ULPI_D7) |   \
                                     PIN_ODR_HIGH(GPIOB_DCMI_D5) |          \
                                     PIN_ODR_HIGH(GPIOB_DCMI_VSYNC) |       \
                                     PIN_ODR_HIGH(GPIOB_I2C1_SCL) |         \
                                     PIN_ODR_HIGH(GPIOB_I2C1_SDA) |         \
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_ULPI_D3) |   \
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_ULPI_D4) |   \
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_ULPI_D5) |   \
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_ULPI_D6) |   \
                                     PIN_ODR_HIGH(GPIOB_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_OTG_HS_ULPI_D1, 10) |\
                                     PIN_AFIO_AF(GPIOB_OTG_HS_ULPI_D2, 10) |\
                                     PIN_AFIO_AF(GPIOB_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOB_SPI1_CLK, 5) |       \
                                     PIN_AFIO_AF(GPIOB_SPI1_MISO, 5) |      \
                                     PIN_AFIO_AF(GPIOB_OTG_HS_ULPI_D7, 10) |\
                                     PIN_AFIO_AF(GPIOB_DCMI_D5, 13) |       \
                                     PIN_AFIO_AF(GPIOB_DCMI_VSYNC, 13))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_I2C1_SCL, 4) |       \
                                     PIN_AFIO_AF(GPIOB_I2C1_SDA, 4) |       \
                                     PIN_AFIO_AF(GPIOB_OTG_HS_ULPI_D3, 10) |\
                                     PIN_AFIO_AF(GPIOB_OTG_HS_ULPI_D4, 10) |\
                                     PIN_AFIO_AF(GPIOB_OTG_HS_ULPI_D5, 10) |\
                                     PIN_AFIO_AF(GPIOB_OTG_HS_ULPI_D6, 10) |\
                                     PIN_AFIO_AF(GPIOB_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOB_PIN15, 0))

/*
 * GPIOC setup:
 *
 * PC0  - OTG_HS_ULPI_STP           (alternate 10).
 * PC1  - PIN1                      (input pullup).
 * PC2  - OTG_HS_ULPI_DIR           (alternate 10).
 * PC3  - OTG_HS_ULPI_NXT           (alternate 10).
 * PC4  - PIN4                      (input pullup).
 * PC5  - PIN5                      (input pullup).
 * PC6  - DCMI_D0                   (alternate 13).
 * PC7  - DCMI_D1                   (alternate 13).
 * PC8  - PIN8                      (input pullup).
 * PC9  - I2C3_SDA                  (alternate 4).
 * PC10 - SPI3_CLK                  (alternate 6).
 * PC11 - SPI3_MISO                 (alternate 6).
 * PC12 - SPI3_MOSI                 (alternate 6).
 * PC13 - PIN13                     (input pullup).
 * PC14 - PIN14                     (input pullup).
 * PC15 - PIN15                     (input pullup).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ALTERNATE(GPIOC_OTG_HS_ULPI_STP) |\
                                     PIN_MODE_INPUT(GPIOC_PIN1) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_OTG_HS_ULPI_DIR) |\
                                     PIN_MODE_ALTERNATE(GPIOC_OTG_HS_ULPI_NXT) |\
                                     PIN_MODE_INPUT(GPIOC_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN5) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_DCMI_D0) |    \
                                     PIN_MODE_ALTERNATE(GPIOC_DCMI_D1) |    \
                                     PIN_MODE_INPUT(GPIOC_PIN8) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_I2C3_SDA) |   \
                                     PIN_MODE_ALTERNATE(GPIOC_SPI3_CLK) |   \
                                     PIN_MODE_ALTERNATE(GPIOC_SPI3_MISO) |  \
                                     PIN_MODE_ALTERNATE(GPIOC_SPI3_MOSI) |  \
                                     PIN_MODE_INPUT(GPIOC_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_OTG_HS_ULPI_STP) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_OTG_HS_ULPI_DIR) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_OTG_HS_ULPI_NXT) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_DCMI_D0) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOC_DCMI_D1) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN8) |       \
                                     PIN_OTYPE_OPENDRAIN(GPIOC_I2C3_SDA) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_CLK) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_MISO) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_MOSI) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN15))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_HIGH(GPIOC_OTG_HS_ULPI_STP) |\
                                     PIN_OSPEED_HIGH(GPIOC_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOC_OTG_HS_ULPI_DIR) |\
                                     PIN_OSPEED_HIGH(GPIOC_OTG_HS_ULPI_NXT) |\
                                     PIN_OSPEED_HIGH(GPIOC_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOC_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOC_DCMI_D0) |       \
                                     PIN_OSPEED_HIGH(GPIOC_DCMI_D1) |       \
                                     PIN_OSPEED_HIGH(GPIOC_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOC_I2C3_SDA) |      \
                                     PIN_OSPEED_HIGH(GPIOC_SPI3_CLK) |      \
                                     PIN_OSPEED_HIGH(GPIOC_SPI3_MISO) |     \
                                     PIN_OSPEED_HIGH(GPIOC_SPI3_MOSI) |     \
                                     PIN_OSPEED_HIGH(GPIOC_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOC_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOC_PIN15))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_OTG_HS_ULPI_STP) |\
                                     PIN_PUPDR_PULLUP(GPIOC_PIN1) |         \
                                     PIN_PUPDR_FLOATING(GPIOC_OTG_HS_ULPI_DIR) |\
                                     PIN_PUPDR_FLOATING(GPIOC_OTG_HS_ULPI_NXT) |\
                                     PIN_PUPDR_PULLUP(GPIOC_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_DCMI_D0) |      \
                                     PIN_PUPDR_PULLUP(GPIOC_DCMI_D1) |      \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN8) |         \
                                     PIN_PUPDR_FLOATING(GPIOC_I2C3_SDA) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_CLK) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_MISO) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_MOSI) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN15))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_OTG_HS_ULPI_STP) |  \
                                     PIN_ODR_HIGH(GPIOC_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOC_OTG_HS_ULPI_DIR) |  \
                                     PIN_ODR_HIGH(GPIOC_OTG_HS_ULPI_NXT) |  \
                                     PIN_ODR_HIGH(GPIOC_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOC_DCMI_D0) |          \
                                     PIN_ODR_HIGH(GPIOC_DCMI_D1) |          \
                                     PIN_ODR_HIGH(GPIOC_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOC_I2C3_SDA) |         \
                                     PIN_ODR_HIGH(GPIOC_SPI3_CLK) |         \
                                     PIN_ODR_HIGH(GPIOC_SPI3_MISO) |        \
                                     PIN_ODR_HIGH(GPIOC_SPI3_MOSI) |        \
                                     PIN_ODR_HIGH(GPIOC_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN15))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_OTG_HS_ULPI_STP, 10) |\
                                     PIN_AFIO_AF(GPIOC_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOC_OTG_HS_ULPI_DIR, 10) |\
                                     PIN_AFIO_AF(GPIOC_OTG_HS_ULPI_NXT, 10) |\
                                     PIN_AFIO_AF(GPIOC_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOC_DCMI_D0, 13) |       \
                                     PIN_AFIO_AF(GPIOC_DCMI_D1, 13))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOC_I2C3_SDA, 4) |       \
                                     PIN_AFIO_AF(GPIOC_SPI3_CLK, 6) |       \
                                     PIN_AFIO_AF(GPIOC_SPI3_MISO, 6) |      \
                                     PIN_AFIO_AF(GPIOC_SPI3_MOSI, 6) |      \
                                     PIN_AFIO_AF(GPIOC_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOC_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOC_PIN15, 0))

/*
 * GPIOD setup:
 *
 * PD0  - CAN1_RX                   (alternate 9).
 * PD1  - CAN1_TX                   (alternate 9).
 * PD2  - INT_ENC                   (input pullup).
 * PD3  - SPI3_CS2                  (output pushpull maximum).
 * PD4  - INT_IMU                   (input pulldown).
 * PD5  - SPI3_CS1                  (output pushpull maximum).
 * PD6  - SPI1_CS1                  (output pushpull maximum).
 * PD7  - Reset_2                   (output pushpull maximum).
 * PD8  - StandBy_1                 (output pushpull maximum).
 * PD9  - PIN9                      (input pullup).
 * PD10 - Output_Enable_1           (output pushpull maximum).
 * PD11 - Trigger_1                 (output pushpull maximum).
 * PD12 - LED4                      (output pushpull maximum).
 * PD13 - LED3                      (output pushpull maximum).
 * PD14 - LED5                      (output pushpull maximum).
 * PD15 - LED6                      (output pushpull maximum).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_ALTERNATE(GPIOD_CAN1_RX) |    \
                                     PIN_MODE_ALTERNATE(GPIOD_CAN1_TX) |    \
                                     PIN_MODE_INPUT(GPIOD_INT_ENC) |        \
                                     PIN_MODE_OUTPUT(GPIOD_SPI3_CS2) |      \
                                     PIN_MODE_INPUT(GPIOD_INT_IMU) |        \
                                     PIN_MODE_OUTPUT(GPIOD_SPI3_CS1) |      \
                                     PIN_MODE_OUTPUT(GPIOD_SPI1_CS1) |      \
                                     PIN_MODE_OUTPUT(GPIOD_Reset_2) |       \
                                     PIN_MODE_OUTPUT(GPIOD_StandBy_1) |     \
                                     PIN_MODE_INPUT(GPIOD_PIN9) |           \
                                     PIN_MODE_OUTPUT(GPIOD_Output_Enable_1) |\
                                     PIN_MODE_OUTPUT(GPIOD_Trigger_1) |     \
                                     PIN_MODE_OUTPUT(GPIOD_LED4) |          \
                                     PIN_MODE_OUTPUT(GPIOD_LED3) |          \
                                     PIN_MODE_OUTPUT(GPIOD_LED5) |          \
                                     PIN_MODE_OUTPUT(GPIOD_LED6))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_CAN1_RX) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_CAN1_TX) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_INT_ENC) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_SPI3_CS2) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_INT_IMU) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_SPI3_CS1) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_SPI1_CS1) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_Reset_2) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_StandBy_1) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_Output_Enable_1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_Trigger_1) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_LED4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_LED3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_LED5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_LED6))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_CAN1_RX) |       \
                                     PIN_OSPEED_HIGH(GPIOD_CAN1_TX) |       \
                                     PIN_OSPEED_HIGH(GPIOD_INT_ENC) |       \
                                     PIN_OSPEED_HIGH(GPIOD_SPI3_CS2) |      \
                                     PIN_OSPEED_HIGH(GPIOD_INT_IMU) |       \
                                     PIN_OSPEED_HIGH(GPIOD_SPI3_CS1) |      \
                                     PIN_OSPEED_HIGH(GPIOD_SPI1_CS1) |      \
                                     PIN_OSPEED_HIGH(GPIOD_Reset_2) |       \
                                     PIN_OSPEED_HIGH(GPIOD_StandBy_1) |     \
                                     PIN_OSPEED_HIGH(GPIOD_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOD_Output_Enable_1) |\
                                     PIN_OSPEED_HIGH(GPIOD_Trigger_1) |     \
                                     PIN_OSPEED_HIGH(GPIOD_LED4) |          \
                                     PIN_OSPEED_HIGH(GPIOD_LED3) |          \
                                     PIN_OSPEED_HIGH(GPIOD_LED5) |          \
                                     PIN_OSPEED_HIGH(GPIOD_LED6))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_CAN1_RX) |    \
                                     PIN_PUPDR_FLOATING(GPIOD_CAN1_TX) |    \
                                     PIN_PUPDR_PULLUP(GPIOD_INT_ENC) |      \
                                     PIN_PUPDR_PULLDOWN(GPIOD_SPI3_CS2) |   \
                                     PIN_PUPDR_PULLDOWN(GPIOD_INT_IMU) |    \
                                     PIN_PUPDR_PULLDOWN(GPIOD_SPI3_CS1) |   \
                                     PIN_PUPDR_PULLDOWN(GPIOD_SPI1_CS1) |   \
                                     PIN_PUPDR_PULLUP(GPIOD_Reset_2) |      \
                                     PIN_PUPDR_PULLUP(GPIOD_StandBy_1) |    \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN9) |         \
                                     PIN_PUPDR_PULLDOWN(GPIOD_Output_Enable_1) |\
                                     PIN_PUPDR_PULLDOWN(GPIOD_Trigger_1) |  \
                                     PIN_PUPDR_FLOATING(GPIOD_LED4) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_LED3) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_LED5) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_LED6))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_CAN1_RX) |          \
                                     PIN_ODR_HIGH(GPIOD_CAN1_TX) |          \
                                     PIN_ODR_HIGH(GPIOD_INT_ENC) |          \
                                     PIN_ODR_LOW(GPIOD_SPI3_CS2) |          \
                                     PIN_ODR_HIGH(GPIOD_INT_IMU) |          \
                                     PIN_ODR_LOW(GPIOD_SPI3_CS1) |          \
                                     PIN_ODR_LOW(GPIOD_SPI1_CS1) |          \
                                     PIN_ODR_HIGH(GPIOD_Reset_2) |          \
                                     PIN_ODR_HIGH(GPIOD_StandBy_1) |        \
                                     PIN_ODR_HIGH(GPIOD_PIN9) |             \
                                     PIN_ODR_LOW(GPIOD_Output_Enable_1) |   \
                                     PIN_ODR_LOW(GPIOD_Trigger_1) |         \
                                     PIN_ODR_LOW(GPIOD_LED4) |              \
                                     PIN_ODR_LOW(GPIOD_LED3) |              \
                                     PIN_ODR_LOW(GPIOD_LED5) |              \
                                     PIN_ODR_LOW(GPIOD_LED6))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_CAN1_RX, 9) |        \
                                     PIN_AFIO_AF(GPIOD_CAN1_TX, 9) |        \
                                     PIN_AFIO_AF(GPIOD_INT_ENC, 0) |        \
                                     PIN_AFIO_AF(GPIOD_SPI3_CS2, 0) |       \
                                     PIN_AFIO_AF(GPIOD_INT_IMU, 0) |        \
                                     PIN_AFIO_AF(GPIOD_SPI3_CS1, 0) |       \
                                     PIN_AFIO_AF(GPIOD_SPI1_CS1, 0) |       \
                                     PIN_AFIO_AF(GPIOD_Reset_2, 0))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_StandBy_1, 0) |      \
                                     PIN_AFIO_AF(GPIOD_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOD_Output_Enable_1, 0) |\
                                     PIN_AFIO_AF(GPIOD_Trigger_1, 0) |      \
                                     PIN_AFIO_AF(GPIOD_LED4, 0) |           \
                                     PIN_AFIO_AF(GPIOD_LED3, 0) |           \
                                     PIN_AFIO_AF(GPIOD_LED5, 0) |           \
                                     PIN_AFIO_AF(GPIOD_LED6, 0))

/*
 * GPIOE setup:
 *
 * PE0  - DCMI_D2                   (alternate 13).
 * PE1  - DCMI_D3                   (alternate 13).
 * PE2  - StandBy_2                 (output pushpull maximum).
 * PE3  - Trigger_2                 (output pushpull maximum).
 * PE4  - DCMI_D4                   (alternate 13).
 * PE5  - DCMI_D6                   (alternate 13).
 * PE6  - DCMI_D7                   (output pushpull maximum).
 * PE7  - Reset_1                   (output pushpull maximum).
 * PE8  - PIN8                      (input floating).
 * PE9  - Master_Clock_DCMI         (alternate 1).
 * PE10 - PIN10                     (input floating).
 * PE11 - PIN11                     (input floating).
 * PE12 - PIN12                     (input floating).
 * PE13 - PIN13                     (input floating).
 * PE14 - PIN14                     (input floating).
 * PE15 - PIN15                     (input floating).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_ALTERNATE(GPIOE_DCMI_D2) |    \
                                     PIN_MODE_ALTERNATE(GPIOE_DCMI_D3) |    \
                                     PIN_MODE_OUTPUT(GPIOE_StandBy_2) |     \
                                     PIN_MODE_OUTPUT(GPIOE_Trigger_2) |     \
                                     PIN_MODE_ALTERNATE(GPIOE_DCMI_D4) |    \
                                     PIN_MODE_ALTERNATE(GPIOE_DCMI_D6) |    \
                                     PIN_MODE_OUTPUT(GPIOE_DCMI_D7) |       \
                                     PIN_MODE_OUTPUT(GPIOE_Reset_1) |       \
                                     PIN_MODE_INPUT(GPIOE_PIN8) |           \
                                     PIN_MODE_ALTERNATE(GPIOE_Master_Clock_DCMI) |\
                                     PIN_MODE_INPUT(GPIOE_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_DCMI_D2) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_DCMI_D3) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_StandBy_2) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOE_Trigger_2) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOE_DCMI_D4) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_DCMI_D6) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_DCMI_D7) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_Reset_1) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_Master_Clock_DCMI) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_HIGH(GPIOE_DCMI_D2) |       \
                                     PIN_OSPEED_HIGH(GPIOE_DCMI_D3) |       \
                                     PIN_OSPEED_HIGH(GPIOE_StandBy_2) |     \
                                     PIN_OSPEED_HIGH(GPIOE_Trigger_2) |     \
                                     PIN_OSPEED_HIGH(GPIOE_DCMI_D4) |       \
                                     PIN_OSPEED_HIGH(GPIOE_DCMI_D6) |       \
                                     PIN_OSPEED_HIGH(GPIOE_DCMI_D7) |       \
                                     PIN_OSPEED_HIGH(GPIOE_Reset_1) |       \
                                     PIN_OSPEED_HIGH(GPIOE_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOE_Master_Clock_DCMI) |\
                                     PIN_OSPEED_HIGH(GPIOE_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOE_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOE_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOE_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOE_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(GPIOE_DCMI_D2) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_DCMI_D3) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_StandBy_2) |    \
                                     PIN_PUPDR_PULLDOWN(GPIOE_Trigger_2) |  \
                                     PIN_PUPDR_PULLUP(GPIOE_DCMI_D4) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_DCMI_D6) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_DCMI_D7) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_Reset_1) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_Master_Clock_DCMI) |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_DCMI_D2) |          \
                                     PIN_ODR_HIGH(GPIOE_DCMI_D3) |          \
                                     PIN_ODR_HIGH(GPIOE_StandBy_2) |        \
                                     PIN_ODR_LOW(GPIOE_Trigger_2) |         \
                                     PIN_ODR_HIGH(GPIOE_DCMI_D4) |          \
                                     PIN_ODR_HIGH(GPIOE_DCMI_D6) |          \
                                     PIN_ODR_HIGH(GPIOE_DCMI_D7) |          \
                                     PIN_ODR_HIGH(GPIOE_Reset_1) |          \
                                     PIN_ODR_HIGH(GPIOE_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOE_Master_Clock_DCMI) |\
                                     PIN_ODR_HIGH(GPIOE_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_DCMI_D2, 13) |       \
                                     PIN_AFIO_AF(GPIOE_DCMI_D3, 13) |       \
                                     PIN_AFIO_AF(GPIOE_StandBy_2, 0) |      \
                                     PIN_AFIO_AF(GPIOE_Trigger_2, 0) |      \
                                     PIN_AFIO_AF(GPIOE_DCMI_D4, 13) |       \
                                     PIN_AFIO_AF(GPIOE_DCMI_D6, 13) |       \
                                     PIN_AFIO_AF(GPIOE_DCMI_D7, 13) |       \
                                     PIN_AFIO_AF(GPIOE_Reset_1, 0))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOE_Master_Clock_DCMI, 1) |\
                                     PIN_AFIO_AF(GPIOE_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN15, 0))

/*
 * GPIOF setup:
 *
 * PF0  - PIN0                      (input floating).
 * PF1  - PIN1                      (input floating).
 * PF2  - PIN2                      (input floating).
 * PF3  - PIN3                      (input floating).
 * PF4  - PIN4                      (input floating).
 * PF5  - PIN5                      (input floating).
 * PF6  - PIN6                      (input floating).
 * PF7  - PIN7                      (input floating).
 * PF8  - PIN8                      (input floating).
 * PF9  - PIN9                      (input floating).
 * PF10 - PIN10                     (input floating).
 * PF11 - PIN11                     (input floating).
 * PF12 - PIN12                     (input floating).
 * PF13 - PIN13                     (input floating).
 * PF14 - PIN14                     (input floating).
 * PF15 - PIN15                     (input floating).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN15))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_HIGH(GPIOF_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN15))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN7, 0))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN15, 0))

/*
 * GPIOG setup:
 *
 * PG0  - PIN0                      (input floating).
 * PG1  - PIN1                      (input floating).
 * PG2  - PIN2                      (input floating).
 * PG3  - PIN3                      (input floating).
 * PG4  - PIN4                      (input floating).
 * PG5  - PIN5                      (input floating).
 * PG6  - PIN6                      (input floating).
 * PG7  - PIN7                      (input floating).
 * PG8  - PIN8                      (input floating).
 * PG9  - PIN9                      (input floating).
 * PG10 - PIN10                     (input floating).
 * PG11 - PIN11                     (input floating).
 * PG12 - PIN12                     (input floating).
 * PG13 - PIN13                     (input floating).
 * PG14 - PIN14                     (input floating).
 * PG15 - PIN15                     (input floating).
 */
#define VAL_GPIOG_MODER             (PIN_MODE_INPUT(GPIOG_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN15))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOG_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN15))
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_HIGH(GPIOG_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOG_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOG_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOG_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOG_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOG_PIN15))
#define VAL_GPIOG_PUPDR             (PIN_PUPDR_FLOATING(GPIOG_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN15))
#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(GPIOG_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN15))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(GPIOG_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN7, 0))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN15, 0))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN                    (input floating).
 * PH1  - OSC_OUT                   (input floating).
 * PH2  - PIN2                      (input floating).
 * PH3  - PIN3                      (input floating).
 * PH4  - PIN4                      (input floating).
 * PH5  - PIN5                      (input floating).
 * PH6  - PIN6                      (input floating).
 * PH7  - PIN7                      (input floating).
 * PH8  - PIN8                      (input floating).
 * PH9  - PIN9                      (input floating).
 * PH10 - PIN10                     (input floating).
 * PH11 - PIN11                     (input floating).
 * PH12 - PIN12                     (input floating).
 * PH13 - PIN13                     (input floating).
 * PH14 - PIN14                     (input floating).
 * PH15 - PIN15                     (input floating).
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT) |        \
                                     PIN_MODE_INPUT(GPIOH_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN15))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN15))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_HIGH(GPIOH_OSC_IN) |        \
                                     PIN_OSPEED_HIGH(GPIOH_OSC_OUT) |       \
                                     PIN_OSPEED_HIGH(GPIOH_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN15))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN) |     \
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT) |    \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN15))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(GPIOH_OSC_IN) |           \
                                     PIN_ODR_HIGH(GPIOH_OSC_OUT) |          \
                                     PIN_ODR_HIGH(GPIOH_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN15))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN, 0) |         \
                                     PIN_AFIO_AF(GPIOH_OSC_OUT, 0) |        \
                                     PIN_AFIO_AF(GPIOH_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN7, 0))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(GPIOH_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN15, 0))

/*
 * GPIOI setup:
 *
 * PI0  - PIN0                      (input floating).
 * PI1  - PIN1                      (input floating).
 * PI2  - PIN2                      (input floating).
 * PI3  - PIN3                      (input floating).
 * PI4  - PIN4                      (input floating).
 * PI5  - PIN5                      (input floating).
 * PI6  - PIN6                      (input floating).
 * PI7  - PIN7                      (input floating).
 * PI8  - PIN8                      (input floating).
 * PI9  - PIN9                      (input floating).
 * PI10 - PIN10                     (input floating).
 * PI11 - PIN11                     (input floating).
 * PI12 - PIN12                     (input floating).
 * PI13 - PIN13                     (input floating).
 * PI14 - PIN14                     (input floating).
 * PI15 - PIN15                     (input floating).
 */
#define VAL_GPIOI_MODER             (PIN_MODE_INPUT(GPIOI_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN15))
#define VAL_GPIOI_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOI_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN15))
#define VAL_GPIOI_OSPEEDR           (PIN_OSPEED_HIGH(GPIOI_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN15))
#define VAL_GPIOI_PUPDR             (PIN_PUPDR_FLOATING(GPIOI_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN15))
#define VAL_GPIOI_ODR               (PIN_ODR_HIGH(GPIOI_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN15))
#define VAL_GPIOI_AFRL              (PIN_AFIO_AF(GPIOI_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN7, 0))
#define VAL_GPIOI_AFRH              (PIN_AFIO_AF(GPIOI_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN15, 0))


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
