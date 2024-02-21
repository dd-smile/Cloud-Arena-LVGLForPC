
#include "ui_app.h"

#define MAX_BUFFER_SIZE 512

char* get_connected_wifi_name() {
    char buffer[MAX_BUFFER_SIZE];
    FILE* pipe = popen("iwconfig wlan0", "r");
    if (!pipe) {
        return NULL;
    }

    char* connected_wifi_name = NULL;
    regex_t regex;
    int reti;

    // Regular expression pattern to match ESSID:"<Wi-Fi Name>"
    const char* pattern = "ESSID:\"([^\"]*)\"";

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        pclose(pipe);
        return NULL;
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        // Try to match the pattern in the buffer
        reti = regexec(&regex, buffer, 2, NULL, 0);
        if (reti == 0) {
            // Match found, extract the Wi-Fi name from the buffer
            char* token = strtok(buffer, "\"");
            if (token != NULL) {
                token = strtok(NULL, "\"");
                if (token != NULL) {
                    connected_wifi_name = strdup(token);
                    break;
                }
            }
        }
    }

    regfree(&regex);
    pclose(pipe);
    return connected_wifi_name;
}



int wifi_scan(struct WifiInfo *wifi_results, int max_results)
{
    FILE *fp = popen("iwlist wlan0 scanning", "r"); // Replace wlan0 with your Wi-Fi interface name

    if (fp == NULL)
    {
        perror("popen failed");
        return -1;
    }

    char buffer[1024];
    int wifi_count = 0;

    // Regular expressions for matching relevant information in the iwlist output
    regex_t ssid_pattern, bssid_pattern, channel_pattern, signal_pattern;
    regcomp(&ssid_pattern, "ESSID:\"([^\"]*)\"", REG_EXTENDED); // Changed to allow empty SSID
    regcomp(&bssid_pattern, "Address: ([0-9A-Fa-f:]+)", REG_EXTENDED);
    regcomp(&channel_pattern, "Frequency:([0-9\\.]+) GHz \\(Channel ([0-9]+)\\)", REG_EXTENDED);
    regcomp(&signal_pattern, "Signal level=([-0-9]+)", REG_EXTENDED);

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        regmatch_t matches[3];

        // Match SSID
        if (regexec(&ssid_pattern, buffer, 2, matches, 0) == 0)
        {
            int ssid_length = matches[1].rm_eo - matches[1].rm_so;
            if (ssid_length == 0)
                continue; // Skip Wi-Fi networks with empty SSID

            strncpy(wifi_results[wifi_count].SSID, buffer + matches[1].rm_so, ssid_length);
            wifi_results[wifi_count].SSID[ssid_length] = '\0';
        }
        // Match BSSID
        else if (regexec(&bssid_pattern, buffer, 2, matches, 0) == 0)
        {
            int bssid_length = matches[1].rm_eo - matches[1].rm_so;
            strncpy(wifi_results[wifi_count].BSSID, buffer + matches[1].rm_so, bssid_length);
            wifi_results[wifi_count].BSSID[bssid_length] = '\0';
        }
        // Match Channel and Frequency
        else if (regexec(&channel_pattern, buffer, 3, matches, 0) == 0)
        {
            int channel_length = matches[2].rm_eo - matches[2].rm_so;
            strncpy(wifi_results[wifi_count].channel, buffer + matches[2].rm_so, channel_length);
            wifi_results[wifi_count].channel[channel_length] = '\0';
        }
        // Match Signal Strength
        else if (regexec(&signal_pattern, buffer, 2, matches, 0) == 0)
        {
            int signal_length = matches[1].rm_eo - matches[1].rm_so;
            strncpy(wifi_results[wifi_count].signal_strength, buffer + matches[1].rm_so, signal_length);
            wifi_results[wifi_count].signal_strength[signal_length] = '\0';
            wifi_count++;
        }

        if (wifi_count >= max_results)
        {
            break;
        }
    }

    // Free the regex patterns
    regfree(&ssid_pattern);
    regfree(&bssid_pattern);
    regfree(&channel_pattern);
    regfree(&signal_pattern);

    pclose(fp);

    return wifi_count;
}
char *convert_unicode_str(const char *str)
{
    int length = strlen(str);
    char *output = (char *)malloc(length + 1); // 分配足够的内存来存储转换后的字符串
    int i = 0;                                 // 初始化循环的计数器
    int out_index = 0;                         // 输出字符串的索引

    while (i < length)
    {
        if (str[i] == '\\' && str[i + 1] == 'x')
        {
            // 如果当前字符是'\'并且下一个字符是'x'，则它是一个Unicode转义序列
            unsigned int codepoint;
            if (sscanf(str + i + 2, "%X", &codepoint) == 1)
            {
                // 将'\x'后面的十六进制数字转换为表示Unicode码点的无符号整数
                output[out_index++] = codepoint; // 将Unicode字符添加到输出字符串
                i += 4;                          // 将索引移动到跳过整个Unicode转义序列（例如，\xE5\x9C\x86\xE9\x80\x9A，每个Unicode字符占4个字节）
            }
            else
            {
                // 如果sscanf无法将十六进制数字转换为整数，则只将'\'添加到输出字符串并移动到下一个字符
                output[out_index++] = str[i];
                i++; // 将索引移动到跳过当前字符（'\'）和下一个字符（'x'）
            }
        }
        else
        {
            // 如果当前字符不是Unicode转义序列的一部分，则直接添加它到输出字符串
            output[out_index++] = str[i];
            i++; // 将索引移动到下一个字符
        }
    }

    output[out_index] = '\0'; // 在输出字符串的结尾添加NULL终止符

    return output;
}
