/*
Copyright (c) 2020 Mauro Riva
Adapted by Mauro Riva (lemariva.com) for MicroPython support
*/

#include "ft6336u.h"
#include "i2cdev.h"

static uint8_t devAddr;
static I2C_Dev *I2Cx;
static uint8_t buffer[14];
static bool isInit;
FT6336U_TouchPointType touchPoint;

int ft6336u_init() {

    bool status = false;

    status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_FIRMARE_ID, buffer);
    if (buffer[0] != 0x10)
        return FT6336U_ERROR_NOTTY;

    isInit = true;
    return FT6336U_OK;
}

int ft6336uInit(I2C_Dev *i2cPort) {
    if (isInit) {
        return FT6336U_ERROR_NOTTY;
    }

    I2Cx = i2cPort;
    devAddr = I2C_ADDR_FT6336U;

    return ft6336u_init();
}


uint8_t read_g_mode(void) {
    bool status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_G_MODE, buffer);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
    return buffer[0];
}

uint8_t write_g_mode(G_MODE_Enum mode){
    bool status = i2cdevWriteByte(I2Cx, devAddr, FT6336U_ADDR_G_MODE, mode);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
    return FT6336U_OK;
}

uint8_t read_gesture_id(void) {
    bool status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_GESTURE_ID, buffer);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
    return buffer[0];
}

uint8_t read_td_status(void) {
    bool status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_TD_STATUS, buffer);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
    return buffer[0];
}

// Touch 1 functions
uint16_t read_touch1_x(void) {
    uint8_t read_buf[2];

    bool status = i2cdevReadReg8(I2Cx, devAddr, FT6336U_ADDR_TOUCH1_X, 2, read_buf);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }

	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint16_t read_touch1_y(void) {
    uint8_t read_buf[2];

    bool status = i2cdevReadReg8(I2Cx, devAddr, FT6336U_ADDR_TOUCH1_Y, 2, read_buf);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }

	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint8_t read_touch1_event(void) {
    bool status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_TOUCH1_EVENT, buffer);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }

    return buffer[0] >> 6;
}
uint8_t read_touch1_id(void) {
    bool status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_TOUCH1_ID, buffer);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
    return buffer[0] >> 4;
}
uint8_t read_touch1_weight(void) {
    bool status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_TOUCH1_WEIGHT, buffer);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
    return buffer[0];
}
uint8_t read_touch1_misc(void) {
    bool status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_TOUCH1_MISC, buffer);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
    return buffer[0] >> 4;
}
// Touch 2 functions
uint16_t read_touch2_x(void) {
    uint8_t read_buf[2];
    bool status = i2cdevReadReg8(I2Cx, devAddr, FT6336U_ADDR_TOUCH2_X, 2, read_buf);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint16_t read_touch2_y(void) {
    uint8_t read_buf[2];
    bool status = i2cdevReadReg8(I2Cx, devAddr, FT6336U_ADDR_TOUCH2_Y, 2, read_buf);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint8_t read_touch2_event(void) {
    bool status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_TOUCH2_EVENT, buffer);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
    return buffer[0] >> 6;
}
uint8_t read_touch2_id(void) {
    bool status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_TOUCH2_ID, buffer);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
    return buffer[0] >> 4;
}
uint8_t read_touch2_weight(void) {
    bool status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_TOUCH2_WEIGHT, buffer);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
    return buffer[0];
}
uint8_t read_touch2_misc(void) {
    bool status = i2cdevReadByte(I2Cx, devAddr, FT6336U_ADDR_TOUCH2_MISC, buffer);
    if (!status) {
        return FT6336U_ERROR_NOTTY;
    }
    return buffer[0] >> 4;
}

FT6336U_TouchPointType touchPointScan(void){
    touchPoint.touch_count = read_td_status();

    if(touchPoint.touch_count == 0) {
        touchPoint.tp[0].status = release;
        touchPoint.tp[1].status = release;
    }
    else if(touchPoint.touch_count == 1) {
        uint8_t id1 = read_touch1_id(); // id1 = 0 or 1
        touchPoint.tp[id1].status = (touchPoint.tp[id1].status == release) ? touch : stream;
        touchPoint.tp[id1].x = read_touch1_x();
        touchPoint.tp[id1].y = read_touch1_y();
        touchPoint.tp[~id1 & 0x01].status = release;
    }
    else {
        uint8_t id1 = read_touch1_id(); // id1 = 0 or 1
        touchPoint.tp[id1].status = (touchPoint.tp[id1].status == release) ? touch : stream;
        touchPoint.tp[id1].x = read_touch1_x();
        touchPoint.tp[id1].y = read_touch1_y();
        uint8_t id2 = read_touch2_id(); // id2 = 0 or 1(~id1 & 0x01)
        touchPoint.tp[id2].status = (touchPoint.tp[id2].status == release) ? touch : stream;
        touchPoint.tp[id2].x = read_touch2_x();
        touchPoint.tp[id2].y = read_touch2_y();
    }

    return touchPoint;
}

// uint8_t read_device_mode(void) {
//     return (readByte(FT6336U_ADDR_DEVICE_MODE) & 0x70) >> 4;
// }
// void write_device_mode(DEVICE_MODE_Enum mode) {
//     writeByte(FT6336U_ADDR_DEVICE_MODE, (mode & 0x07) << 4);
// }
// uint8_t read_gesture_id(void) {
//     return readByte(FT6336U_ADDR_GESTURE_ID);
// }
// uint8_t read_td_status(void) {
//     return readByte(FT6336U_ADDR_TD_STATUS);
// }
// uint8_t read_touch_number(void) {
//     return readByte(FT6336U_ADDR_TD_STATUS) & 0x0F;
// }
// // Touch 1 functions
// uint16_t read_touch1_x(void) {
//     uint8_t read_buf[2];
//     read_buf[0] = readByte(FT6336U_ADDR_TOUCH1_X);
//     read_buf[1] = readByte(FT6336U_ADDR_TOUCH1_X + 1);
// 	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
// }
// uint16_t read_touch1_y(void) {
//     uint8_t read_buf[2];
//     read_buf[0] = readByte(FT6336U_ADDR_TOUCH1_Y);
//     read_buf[1] = readByte(FT6336U_ADDR_TOUCH1_Y + 1);
// 	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
// }
// uint8_t read_touch1_event(void) {
//     return readByte(FT6336U_ADDR_TOUCH1_EVENT) >> 6;
// }
// uint8_t read_touch1_id(void) {
//     return readByte(FT6336U_ADDR_TOUCH1_ID) >> 4;
// }
// uint8_t read_touch1_weight(void) {
//     return readByte(FT6336U_ADDR_TOUCH1_WEIGHT);
// }
// uint8_t read_touch1_misc(void) {
//     return readByte(FT6336U_ADDR_TOUCH1_MISC) >> 4;
// }
// // Touch 2 functions
// uint16_t read_touch2_x(void) {
//     uint8_t read_buf[2];
//     read_buf[0] = readByte(FT6336U_ADDR_TOUCH2_X);
//     read_buf[1] = readByte(FT6336U_ADDR_TOUCH2_X + 1);
// 	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
// }
// uint16_t read_touch2_y(void) {
//     uint8_t read_buf[2];
//     read_buf[0] = readByte(FT6336U_ADDR_TOUCH2_Y);
//     read_buf[1] = readByte(FT6336U_ADDR_TOUCH2_Y + 1);
// 	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
// }
// uint8_t read_touch2_event(void) {
//     return readByte(FT6336U_ADDR_TOUCH2_EVENT) >> 6;
// }
// uint8_t read_touch2_id(void) {
//     return readByte(FT6336U_ADDR_TOUCH2_ID) >> 4;
// }
// uint8_t read_touch2_weight(void) {
//     return readByte(FT6336U_ADDR_TOUCH2_WEIGHT);
// }
// uint8_t read_touch2_misc(void) {
//     return readByte(FT6336U_ADDR_TOUCH2_MISC) >> 4;
// }

// // Mode Parameter Register
// uint8_t read_touch_threshold(void) {
//     return readByte(FT6336U_ADDR_THRESHOLD);
// }
// uint8_t read_filter_coefficient(void) {
//     return readByte(FT6336U_ADDR_FILTER_COE);
// }
// uint8_t read_ctrl_mode(void) {
//     return readByte(FT6336U_ADDR_CTRL);
// }
// void write_ctrl_mode(CTRL_MODE_Enum mode) {
//     writeByte(FT6336U_ADDR_CTRL, mode);
// }
// uint8_t read_time_period_enter_monitor(void) {
//     return readByte(FT6336U_ADDR_TIME_ENTER_MONITOR);
// }
// uint8_t read_active_rate(void) {
//     return readByte(FT6336U_ADDR_ACTIVE_MODE_RATE);
// }
// uint8_t read_monitor_rate(void) {
//     return readByte(FT6336U_ADDR_MONITOR_MODE_RATE);
// }

// // Gesture Parameters
// uint8_t read_radian_value(void) {
// 	return readByte(FT6336U_ADDR_RADIAN_VALUE);
// }
// void write_radian_value(uint8_t val) {
// 	writeByte(FT6336U_ADDR_RADIAN_VALUE, val);
// }
// uint8_t read_offset_left_right(void) {
// 	return readByte(FT6336U_ADDR_OFFSET_LEFT_RIGHT);
// }
// void write_offset_left_right(uint8_t val) {
// 	writeByte(FT6336U_ADDR_OFFSET_LEFT_RIGHT, val);
// }
// uint8_t read_offset_up_down(void) {
// 	return readByte(FT6336U_ADDR_OFFSET_UP_DOWN);
// }
// void write_offset_up_down(uint8_t val) {
// 	writeByte(FT6336U_ADDR_OFFSET_UP_DOWN, val);
// }
// uint8_t read_distance_left_right(void) {
// 	return readByte(FT6336U_ADDR_DISTANCE_LEFT_RIGHT);
// }
// void write_distance_left_right(uint8_t val) {
// 	writeByte(FT6336U_ADDR_DISTANCE_LEFT_RIGHT, val);
// }
// uint8_t read_distance_up_down(void) {
// 	return readByte(FT6336U_ADDR_DISTANCE_UP_DOWN);
// }
// void write_distance_up_down(uint8_t val) {
// 	writeByte(FT6336U_ADDR_DISTANCE_UP_DOWN, val);
// }
// uint8_t read_distance_zoom(void) {
// 	return readByte(FT6336U_ADDR_DISTANCE_ZOOM);
// }
// void write_distance_zoom(uint8_t val) {
// 	writeByte(FT6336U_ADDR_DISTANCE_ZOOM, val);
// }


// // System Information
// uint16_t read_library_version(void) {
//     uint8_t read_buf[2];
//     read_buf[0] = readByte(FT6336U_ADDR_LIBRARY_VERSION_H);
//     read_buf[1] = readByte(FT6336U_ADDR_LIBRARY_VERSION_L);
// 	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
// }
// uint8_t read_chip_id(void) {
//     return readByte(FT6336U_ADDR_CHIP_ID);
// }
// uint8_t read_pwrmode(void) {
//     return readByte(FT6336U_ADDR_POWER_MODE);
// }
// uint8_t read_firmware_id(void) {
//     return readByte(FT6336U_ADDR_FIRMARE_ID);
// }
// uint8_t read_focaltech_id(void) {
//     return readByte(FT6336U_ADDR_FOCALTECH_ID);
// }
// uint8_t read_release_code_id(void) {
//     return readByte(FT6336U_ADDR_RELEASE_CODE_ID);
// }
// uint8_t read_state(void) {
//     return readByte(FT6336U_ADDR_STATE);
// }
