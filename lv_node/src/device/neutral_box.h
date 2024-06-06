/*
 * @Description: 中立智能电箱
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-04-17 13:58:49
 * @LastEditTime: 2024-04-24 19:33:15
 * @FilePath: lv_node/src/device/neutral_box.h
 */
#ifndef __NEUTRAL_BOX_H__
#define __NEUTRAL_BOX_H__

extern int g_neutral_fd;

void Setcombo_zhongli(uint8_t motor_four, uint8_t motor_eight, uint8_t motor_twelve, uint8_t motor_fifteen);
void Setopen_zhongli(void);
void Setdeparture_zhongli(void);
void Jerk_zhongli(void);
void Setoutput_zhongli(uint8_t motor_four, uint8_t motor_eight, uint8_t motor_twelve, uint8_t motor_fifteen);

void Setcombo_neutral(uint8_t motor_four, uint8_t motor_eight, uint8_t motor_twelve, uint8_t motor_fifteen, uint8_t load_eight, uint8_t load_ten);
void Setoutput_neutral(uint8_t motor_four, uint8_t motor_eight, uint8_t motor_twelve, uint8_t motor_fifteen, uint8_t load_eight, uint8_t load_ten);

void hex_to_bin(char *hex, char *bin);
void print_switch_states(char *bin_data, int start, char *description);

#endif