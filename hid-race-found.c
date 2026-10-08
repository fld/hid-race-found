// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  HID driver for flipping gamepad byte
 *
 *  Copyright (c) 2026 Oleg Makarenko
 */

#include <linux/device.h>
#include <linux/input.h>
#include <linux/hid.h>
#include <linux/module.h>
#include <linux/usb.h>

/*
Original descriptor
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x05,        // Usage (Game Pad)
0xA1, 0x01,        // Collection (Application)
0x05, 0x09,        //   Usage Page (Button)
0x19, 0x01,        //   Usage Minimum (0x01)
0x29, 0x20,        //   Usage Maximum (0x20)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x01,        //   Logical Maximum (1)
0x75, 0x01,        //   Report Size (1)
0x95, 0x20,        //   Report Count (32)
0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection

// 23 bytes
*/


static const __u8 *cammus_report_fixup(struct hid_device *hid, __u8 *rdesc, unsigned int *rsize)
{
    if (*rsize == 23 && rdesc[2] == 0x09 && rdesc[3] == 0x05) {
        hid_info(hid,
             "fixing up race found report descriptor\n");

        rdesc[3] = 0x04;
        return rdesc;
    } else {
        hid_info(hid,
             "Descriptor size is %d, rdesc[2] is %d, rdesc[3] is %d"
             "skipping fixup\n", *rsize, rdesc[2], rdesc[3]);
    }

    return rdesc;
}

static const struct hid_device_id cammus_devices[] = {
    { HID_USB_DEVICE(0x000a, 0x000b) },
    { }
};

MODULE_DEVICE_TABLE(hid, cammus_devices);

static struct hid_driver cammus_driver = {
    .name = "hid-race-found",
    .id_table = cammus_devices,
    .report_fixup = cammus_report_fixup
};
module_hid_driver(cammus_driver);

MODULE_AUTHOR("Oleg Makarenko <oleg@makarenk.ooo>");
MODULE_DESCRIPTION("HID driver for race found, flip gamepad to joystick");
MODULE_LICENSE("GPL");
