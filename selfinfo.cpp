
// Copyright (c) 2019 The Karbowanec developers
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Code formatting based on CS106B Style

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "selfinfo.h"


void num_to_str(const long int num,
                char *num_str,
                unsigned int &pow_index,
                const unsigned int num_str_len) {
  memset(num_str, 0x00, (size_t) num_str_len);
  pow_index = 0;

  const unsigned int kb = 1024;

  long int num_abs = 0;
  long unsigned int k = 1;
  unsigned int pseudo_pow_i = 0;
  float num_buff = 0;

  if (num >= 0) num_abs = num;
  else num_abs = -num;

  while (true) {
    if (num_abs / k < kb) {
      break;
    } else {
      pseudo_pow_i++;
      k *= kb;
    }
    if (pseudo_pow_i == 4) break;
  }
  pow_index = pseudo_pow_i;

  num_buff = (float) num / k;
  sprintf(num_str, "%.2f", num_buff);
}

bool cpu_usage_unix(unsigned long int &cpu_total,
                    unsigned long int &cpu_idle) {
  bool res = false;
  cpu_total = 0;
  cpu_idle = 0;

  const char *stat_path = "/proc/stat";
  const unsigned int idle_pos = 4;
  const unsigned int buff_size = 64;

  int sub = 0;
  unsigned int sub_n = 0;
  unsigned int sub_pos_n = 0;
  unsigned long int cpu_buff = 0;
  char buff[buff_size];
  FILE *fd;

  memset(buff, 0x00, buff_size);

  fd = fopen (stat_path, "r");
  if (fd != NULL) {
    sub_n = 0;
    sub_pos_n = 0;
    while (true) {
      sub = fgetc(fd);
      if (sub != 0x20 && sub != 0x0A && sub != EOF) {
        buff[sub_n] = (char) sub;
        if (sub_n < buff_size - 2) sub_n++;
      } else {
        if (sub_n > 0) {
          buff[sub_n + 1] = 0x00;
          sub_n = 0;
          sub_pos_n++;
          if (sub_pos_n != 0) {
            cpu_buff = 0;
            sscanf(buff, "%lu", &cpu_buff);
            cpu_total += cpu_buff;
            if (sub_pos_n == idle_pos + 1) {
              cpu_idle = cpu_buff;
			}
          }
          memset(buff, 0x00, buff_size);
        }
      }
      if (sub == EOF) break;
    }
    fclose(fd);
  }
  if (cpu_total !=0 && cpu_idle != 0) res = true;

  return res;
}

bool proc_info_unix(const unsigned int pid,
                    long int &proc_utime,
                    long int &proc_stime,
                    long int &mem_vsize,
                    unsigned long int &mem_rss) {
  bool res = false;
  proc_utime = 0;
  proc_stime = 0;
  mem_rss = 0;
  mem_vsize = 0;

  const char *stat_pre = "/proc/";
  const char *stat_post = "/stat";
  const unsigned int utime_pos = 14;
  const unsigned int stime_pos = 15;
  const unsigned int vsize_pos = 23;
  const unsigned int rss_pos = 24;
  const unsigned int buff_size = 64;

  int sub = 0;
  unsigned int sub_n = 0;
  unsigned int sub_pos_n = 0;
  char buff[buff_size];
  char stat_path[buff_size];
  FILE *fd;

  memset(buff, 0x00, buff_size);
  memset(stat_path, 0x00, buff_size);

  sprintf(buff, "%d", pid);
  strcpy(stat_path, stat_pre);
  strcat(stat_path, buff);
  strcat(stat_path, stat_post);
  memset(buff, 0x00, buff_size);

  fd = fopen (stat_path, "r");
  if (fd != NULL) {
    sub_n = 0;
    sub_pos_n = 0;
    while (true) {
      sub = fgetc(fd);
      if (sub != 0x20 && sub != 0x0A && sub != EOF) {
        buff[sub_n] = (char) sub;
        if (sub_n < buff_size - 2) sub_n++;
      } else {
        if (sub_n > 0) {
          buff[sub_n + 1] = 0x00;
          sub_n = 0;
          sub_pos_n++;
          switch(sub_pos_n) {
            case utime_pos:
              sscanf(buff, "%lu", &proc_utime);
              break;
            case stime_pos:
              sscanf(buff, "%lu", &proc_stime);
              break;
            case vsize_pos:
              sscanf(buff, "%lu", &mem_vsize);
              break;
            case rss_pos:
              sscanf(buff, "%ld", &mem_rss);
              mem_rss = mem_rss * sysconf(_SC_PAGE_SIZE);
              break;
          }
          memset(buff, 0x00, buff_size);
        }
      }
      if (sub == EOF) break;
    }
    fclose(fd);
  }
  if (proc_utime !=0 && proc_stime != 0 &&
      mem_vsize !=0 && mem_rss !=0) res = true;

  return res;
}

namespace Selfinfo {

bool mem_usage(const unsigned int pid,
               unsigned long int &mem_rss,
               long int &mem_vsize) {
  bool res = false;
  mem_rss = 0;
  mem_vsize = 0;
  long int proc_utime = 0;
  long int proc_stime = 0;

  res = proc_info_unix(pid, proc_utime, proc_stime, mem_vsize, mem_rss);

  return res;
}

void mem_size_format(const long mem_size,
                     char *str,
                     const unsigned int str_len) {
  memset(str, 0x00, (size_t) str_len);

  const unsigned int num_str_len = 64;
  const char *b = " B";
  const char *kb = " KiB";
  const char *mb = " MiB";
  const char *gb = " GiB";
  const char *tb = " TiB";

  char num_str[num_str_len];
  unsigned int pow_index = 0;

  num_to_str(mem_size, num_str, pow_index, num_str_len);

  strcpy(str, num_str);
  switch(pow_index) {
    case 0:
      strcat(str, b);
      break;
    case 1:
      strcat(str, kb);
      break;
    case 2:
      strcat(str, mb);
      break;
    case 3:
      strcat(str, gb);
      break;
    case 4:
      strcat(str, tb);
      break;
  }
}

}
