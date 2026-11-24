#pragma once

#define DEVICE_NAME "lidar"
#define PLATFORM     mt8516

/* The 2019 preloader maps its image at 0x00200D00 (FILE_INFO load address) with
 * 0x300 bytes of header, so its text starts at 0x00201000. This device only
 * ever runs from a copy of that image in SRAM, so the payload must not overlap
 * the preloader: it sits 4 KiB below the preloader bss. */
#define PAYLOAD_ADDR 0x00101000
#define BSS_START    0x00102180

/* Address of the preloader's g_dram_buf pointer in bss. boot_device_init()
 * dereferences it, and the crafted TEE image restores it to DRAM_BUF. */
#define SRAM_DBUF_PTR 0x00102994
#define DRAM_BUF      0x42000000

#define LK_PART_NAME  "expdb"

/* The donor passes 0x43001000 as the TEE load address into its TEE loader
 * (literal at 0x0020E498, used at 0x0020E36C). tee.c falls back to this
 * address whenever a raw BL31 is loaded and BL32 has no header address, so
 * it must be the donor's value rather than the platform default. */
#define TEE_LOAD_ADDR 0x43001000

/* Forged block device the crafted TEE image installs in bss: list head at
 * BSS_START, device at BDEV_ADDR with its read method at BDEV_ADDR + 0x20. */
#define BDEV_ADDR     0x00102A28
#define BDEV_LH_ADDR  0x00102180
#define BDEV_INIT_ADDR 0x0020EBB0

#define PART_GET_ADDR  0x00213B28
#define PART_LOAD_ADDR 0x00213D24

#define BLDR_CMD_HANDLER_ADDR 0x0020DA80

/* The preloader keeps the secure-memory size at [0x00109000 + 0x530] and
 * hands the TEE entry over through the setter at 0x00216634. */
#define TEE_SECMEM_SIZE_ADDR 0x00109530
#define TEE_SET_ENTRY_ADDR   0x00216634

#define MTEE_IMG_VFY_PUBK_ADDR   0x002224A4
#define MTEE_VERIFY_DECRYPT_ADDR 0x0021AEBC

#define USB_HANDSHAKE_ADDR    0x0020D5A8
#define USB_STRING_TABLE_ADDR 0x00109E28
