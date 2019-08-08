
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


namespace Selfinfo {

bool mem_usage(const unsigned int pid,
               unsigned long int &mem_rss,
               long int &mem_vsize) {
  bool res = false;
  mem_rss = 0;
  mem_vsize = 0;

  const char *stat_pre = "/proc/";
  const char *stat_post = "/stat";
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
        if (sub_n > 0){
          buff[sub_n + 1] = 0x00;
          sub_n = 0;
          sub_pos_n++;
          switch(sub_pos_n) {
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
  if (mem_rss !=0 && mem_vsize != 0) res = true;

  return res;
}

}
