/**
 * @file c64-dual-joystick-usb.c
 */

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "joystick.h"
#include "uart.h"
#include "usbdrv.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uchar                l_report_buffer1[4] = { 0, 0, 0, 0 };
static uchar                l_report_buffer2[4] = { 0, 0, 0, 0 };
static uchar                l_idle_rate = 0; // infinite
static const char           l_desc0_str[] = { 4, 3, 0x09, 0x04 };
static PROGMEM const char   l_vendor_str[]   = "christian";
static PROGMEM const char   l_joystick_str[] = "Retro-Joystick #x";

// --- Global variables --------------------------------------------------------

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

/** 
 * USB configuration descriptor.
 * Needed by "usbdrv" module.
 */
PROGMEM const char usbDescriptorConfiguration[] = {
    9,                  // sizeof(usbDescriptorConfiguration): length of descriptor in bytes
    USBDESCR_CONFIG,    // descriptor type
    9 + (2*(9+9+7)), 0, // total length of data returned (including inlined descriptors)
    2,                  // number of interfaces in this configuration
    1,                  // index of this configuration
    0,                  // configuration name string index
    (1 << 7),           // USB powered
    USB_CFG_MAX_BUS_POWER / 2, // max USB current in 2mA units

    /* interface 1 descriptor follows inline: */
    9,                  // sizeof(usbDescrInterface): length of descriptor in bytes
    USBDESCR_INTERFACE, // descriptor type
    0,                  // index of this interface
    0,                  // alternate setting for this interface
    1,                  // endpoints excl 0: number of endpoint descriptors to follow
    USB_CFG_INTERFACE_CLASS,
    USB_CFG_INTERFACE_SUBCLASS,
    USB_CFG_INTERFACE_PROTOCOL,
    3,                  // string index for interface
    9,                  // sizeof(usbDescrHID): length of descriptor in bytes
    USBDESCR_HID,       // descriptor type: HID
    0x11, 0x01,         // BCD representation of HID version 1.11
    0x00,               // target country code (No specific country code)
    0x01,               // number of HID Report (or other HID class) Descriptor infos to follow
    0x22,               // descriptor type: report
    USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH, 0,  // total length of report descriptor

    /* endpoint descriptor for endpoint 1 */
    7,                  // sizeof(usbDescrEndpoint)
    USBDESCR_ENDPOINT,  // descriptor type = endpoint
    (char)0x81,         // IN endpoint number 1
    0x03,               // attrib: Interrupt endpoint
    8, 0,               // maximum packet size
    USB_CFG_INTR_POLL_INTERVAL, // in ms

    /* interface 2 descriptor follows inline: */
    9,                  // sizeof(usbDescrInterface): length of descriptor in bytes
    USBDESCR_INTERFACE, // descriptor type
    1,                  // index of this interface
    0,                  // alternate setting for this interface
    1,                  // endpoints excl 0: number of endpoint descriptors to follow
    USB_CFG_INTERFACE_CLASS,
    USB_CFG_INTERFACE_SUBCLASS,
    USB_CFG_INTERFACE_PROTOCOL,
    4,                  // string index for interface
    9,                  // sizeof(usbDescrHID): length of descriptor in bytes
    USBDESCR_HID,       // descriptor type: HID
    0x11, 0x01,         // BCD representation of HID version
    0x00,               // target country code
    0x02,               // number of HID Report (or other HID class) Descriptor infos to follow
    0x22,               // descriptor type: report
    USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH, 0,  // total length of report descriptor

    /* endpoint descriptor for endpoint 3 */
    7,                  // sizeof(usbDescrEndpoint)
    USBDESCR_ENDPOINT,  // descriptor type = endpoint
    (char)(0x80 | USB_CFG_EP3_NUMBER), // IN endpoint number 3
    0x03,               // attrib: Interrupt endpoint
    8, 0,               // maximum packet size
    USB_CFG_INTR_POLL_INTERVAL, // in ms
};


/** 
 * USB HID report descriptor.
 * Needed by "usbdrv" module.
 */
PROGMEM const char usbHidReportDescriptor[88] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)
    // axis
    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION(Physical)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x75, 0x08,                    //     REPORT_SIZE (8 bits)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    // button
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x01,                    //   USAGE_MAXIMUM (one Button )
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x02,                    //   REPORT_COUNT (1, one button)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0,                          // END_COLLECTION

    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)
    // axis
    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION(Physical)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x75, 0x08,                    //     REPORT_SIZE (8 bits)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    // button
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x01,                    //   USAGE_MAXIMUM (one Button )
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x02,                    //   REPORT_COUNT (1, one button)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0,                          // END_COLLECTION
};

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void encode_report_joystick1 (void)
{
    // x-axis
    if (joy_1_is_left()) {
        l_report_buffer1[0] = -127;
    } else if (joy_1_is_right()) {
        l_report_buffer1[0] = 127;
    } else {
        l_report_buffer1[0] = 0;
    }

    // y-axis
    if (joy_1_is_up()) {
        l_report_buffer1[1] = -127;
    } else if (joy_1_is_down()) {
        l_report_buffer1[1] = 127;
    } else {
        l_report_buffer1[1] = 0;
    }

    // buttons
    if (joy_1_is_fireing()) {
        l_report_buffer1[2] = 1;
    } else {
        l_report_buffer1[2] = 0;
    }
}


static void encode_report_joystick2 (void)
{
    // x-axis
    if (joy_2_is_left()) {
        l_report_buffer2[0] = -127;
    } else if (joy_2_is_right()) {
        l_report_buffer2[0] = 127;
    } else {
        l_report_buffer2[0] = 0;
    }

    // y-axis
    if (joy_2_is_up()) {
        l_report_buffer2[1] = -127;
    } else if (joy_2_is_down()) {
        l_report_buffer2[1] = 127;
    } else {
        l_report_buffer2[1] = 0;
    }

    // buttons
    if (joy_2_is_fireing()) {
        l_report_buffer2[2] = 1;
    } else {
        l_report_buffer2[2] = 0;
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

/**
 * Used to provide USB descriptors at runtime.
 */
usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq)
{
    static char buf[(sizeof(l_joystick_str) > sizeof(l_vendor_str) ? sizeof(l_joystick_str) : sizeof(l_vendor_str))*2];

    uchar ix = rq->wValue.bytes[0];
    const char* s;
    uchar l;
    uchar ifchar = 0;

    if (ix == 0) {                  // String0
        usbMsgPtr = (usbMsgPtr_t)l_desc0_str;
        return 4;
    }
    else if (ix == 1) {             // Vendor
        s = l_vendor_str;
        l = sizeof(l_vendor_str) * 2;
    }
    else if (ix == 2) {             // Product
        s = l_joystick_str;
        l = (sizeof(l_joystick_str) - 3) * 2;
    }
    else if (ix == 3 || ix == 4) {  // Interface 1 or 2
        s = l_joystick_str;
        l = sizeof(l_joystick_str) * 2;
        ifchar = '1' + ix - 3;
    }
    else {
        return 0;
    }
    
    // copy from ROM to RAM with char to wchar conversion
    char* d = buf + 2;
    char c;
    while ((c = pgm_read_byte(s++)) != 0) {
        *d = c;
        d += 2;
    }
    
    buf[0] = l; // length
    buf[1] = 3; // string descriptor mark
    if (ifchar) {
        buf[(sizeof(l_joystick_str)-1) * 2] = ifchar; // patch interface number
    }        
    usbMsgPtr = (usbMsgPtr_t)buf;
    return l;
}

/**
 * Called when driver receives a SETUP transaction from the host.
 */
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t *rq = (void *)data;
    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
        // class request type
        switch (rq->bRequest) {
        case USBRQ_HID_GET_REPORT:
            // wValue: ReportType, ReportID
            // wIndex: interface
            switch (rq->wIndex.bytes[0]) {
            case 0:
                usbMsgPtr = l_report_buffer2;
                return sizeof(l_report_buffer2);
            case 1:
                usbMsgPtr = l_report_buffer1;
                return sizeof(l_report_buffer1);
            default:
                return 0;
            }
            break;

        case USBRQ_HID_GET_IDLE:
            usbMsgPtr = &l_idle_rate;
            return 1;

        case USBRQ_HID_SET_IDLE:
            // ignore
            return 0;
        }
    }
    return 0;
}

/**
 * Main entry point.
 */
int main (void)
{
    wdt_enable(WDTO_1S);

    joy_init(); 
    usbInit();

    // force reconnect
    usbDeviceDisconnect();
    _delay_ms(255);
    usbDeviceConnect();
    sei();
    
    while(1) {
        wdt_reset();
        usbPoll();
        
        // event occurred on joystick 1
        encode_report_joystick1();

        // event occurred on joystick 2
        encode_report_joystick2();

        // event occurred on joystick 1?
        if (usbInterruptIsReady()) {
            usbSetInterrupt(l_report_buffer1, sizeof(l_report_buffer1));
        }
        // event occurred on joystick 2?
        if (usbInterruptIsReady3()) {
            usbSetInterrupt3(l_report_buffer2, sizeof(l_report_buffer2));
        }
    }
}

