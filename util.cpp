
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
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include "util.h"


bool getIDsByNameUNIX(const char *name,
                      std::vector<unsigned int> &id) {
  bool res = false;
  id.clear();

  const unsigned int stat_path_size = 32;
  const unsigned int proc_name_size = 64;
  const char *root_path = "/proc/";
  const char *stat_path_post = "/stat";
  const char name_sub_start = '(';
  const char name_sub_end = ')';

  bool name_t = false;
  int dir_n = 0;
  int pid = 0;
  int sub = 0;
  unsigned int sub_n = 0;
  char stat_path[stat_path_size];
  char proc_name[proc_name_size];
  FILE *fd;
  struct dirent **namelist;

  memset(stat_path, 0x00, sizeof(char) * stat_path_size);
  memset(proc_name, 0x00, sizeof(char) * proc_name_size);

  dir_n = scandir(root_path, &namelist, NULL, alphasort);
  if (dir_n != -1) {
    while (dir_n--) {
      if (namelist[dir_n]->d_type == DT_DIR) {
        name_t = false;
        pid = 0;
        sub = 0;
        sub_n = 0;
        if (sscanf(namelist[dir_n]->d_name, "%d", &pid) == 1) {
          strcpy(stat_path, root_path);
          strcat(stat_path, namelist[dir_n]->d_name);
          strcat(stat_path, stat_path_post);
          fd = fopen (stat_path, "r");
          if (fd != NULL) {
            while (sub != EOF) {
              sub = fgetc(fd);
              if (name_sub_end == (char) sub) {
                name_t = false;
                break;
              }
              if (name_t && sub_n < proc_name_size - 1) {
                proc_name[sub_n] = (char) sub;
                sub_n++;
              }
              if (name_sub_start == (char) sub) name_t = true;
            }
            fclose(fd);
          }

          if (strcmp(name, proc_name) == 0) id.push_back((unsigned int) pid);

          memset(stat_path, 0x00, sizeof(char) * stat_path_size);
          memset(proc_name, 0x00, sizeof(char) * proc_name_size);
        }
      }
      free((void *) namelist[dir_n]);
    }
    free((void *) namelist);
  }

  if (id.size() > 0) res = true;

  return res;
}

bool getIDsByName(const char *name,
                  std::vector<unsigned int> &id) {
  bool res = false;

  res = getIDsByNameUNIX(name, id);

  return res;
}
