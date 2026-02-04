#include "io.h"
#include "printk.h"

#define ATA_PRIMARY_DATA         0x1F0
#define ATA_PRIMARY_ERR          0x1F1
#define ATA_PRIMARY_SECCOUNT     0x1F2
#define ATA_PRIMARY_LBA_LOW      0x1F3
#define ATA_PRIMARY_LBA_MID      0x1F4
#define ATA_PRIMARY_LBA_HIGH     0x1F5
#define ATA_PRIMARY_DRIVE_SEL    0x1F6
#define ATA_PRIMARY_COMMAND      0x1F7

void ata_identify() {
    outb(ATA_PRIMARY_DRIVE_SEL, 0xA0);
    outb(ATA_PRIMARY_SECCOUNT, 0);
    outb(ATA_PRIMARY_LBA_LOW, 0);
    outb(ATA_PRIMARY_LBA_MID, 0);
    outb(ATA_PRIMARY_LBA_HIGH, 0);
    outb(ATA_PRIMARY_COMMAND, 0xEC); // Commande IDENTIFY
    // Attendre que BSY soit clair ou timeout
    int timeout = 100000;
    int found = 0;
    for (int i = 0; i < timeout; i++) {
        uint8_t status = inb(ATA_PRIMARY_COMMAND);
        if (status == 0) continue; // pas de réponse encore
        if (status & 0x01) {
            printk(LOG_ERR, "ATA: Error identifying drive (status %x)", status);
            return;
        }
        if (!(status & 0x80)) { // BSY clear
            found = 1;
            break;
        }
    }

    if (!found) {
        printk(LOG_ERR, "ATA: No drive detected on primary bus (timeout).");
        return;
    }

    printk(LOG_INFO, "ATA: Primary Master Drive detected.");
}