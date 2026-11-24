/* Lidar (MT8167) preloader patch set.
 *
 * All four addresses were located in the 2019 donor preloader by disassembly;
 * the donor is the only preloader this device boots.
 */
#include <device.h>
#include <patch.h>
#include <tee.h>

void apply_patches(void)
{
    /* 0x00218578 is the donor's secure-boot query: it reads the boot-status
     * word and returns 1 only for the secured values, 0 otherwise. Its
     * callers gate download-agent signature verification on that result, so
     * returning zero makes an unsigned download agent acceptable. */
    patch_ret(0x00218578, 0);

    /* The donor loads the stock USB descriptor table pointer with
     * `add r5, pc` + `str r2, [r3]` at 0x00217B94. Keep the pointer
     * computation and replace the store with a NOP, so the stock table is
     * never installed and setup_usb_descriptors() owns the descriptors. */
    patch_word(0x00217B94, 0xBF00447D);

    /* RPMB anti-rollback policy, entered once per boot after the preloader has
     * read RPMB block 0. The donor compares the stored PL, TEE and LK version
     * words against the versions of the images it loaded:
     *
     * - a stored version above the loaded one prints a mismatch line and
     *   spins forever (0x20194E, 0x201998, 0x2019C6, 0x201A06, 0x201A46),
     *   which is the state a unit last booted on newer firmware is in;
     * - a loaded version above the stored one rewrites RPMB block 0, raising
     *   the rollback floor for every later boot. That write is one-way and
     *   survives every restore of BOOT0, tee1 and expdb.
     *
     * Skipping the routine at its entry avoids both outcomes. The routine's
     * only other effect is publishing the three loaded versions into the
     * boot-argument words at +0x60C/+0x60E/+0x610; those stay at their
     * zero-initialised values, which only affects the reported version
     * numbers. Nothing here writes RPMB. */
    patch_ret(0x0020192C, 0);

    /* 0x0020DF88 is the donor's TEE partition loader, called with the TEE load
     * address. Replace its entry with our loader, which loads the crafted
     * tee1 sub-images and verifies ATF and TEE. */
    patch_branch(0x0020DF88, bldr_load_tee_part);
}
