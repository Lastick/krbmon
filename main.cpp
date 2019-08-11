
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
#include <iostream>
#include <string.h>
#include <vector>
#include "selfinfo.h"
#include "sleep.h"
#include "util.h"


void showInfo(unsigned int pid) {
  const unsigned int num_str_len = 64;
  unsigned long int mem_rss = 0;
  long int mem_vsize = 0;
  float cpu_proc_load = 0.0f;
  char num_str[num_str_len];

  Selfinfo::Selfinfo si;
  si.start(pid);

  if (si.getStatus()) {
    si.getRssMem(mem_rss);
    si.getVirtMem(mem_vsize);
    si.getProcLoad(cpu_proc_load);
    Selfinfo::mem_size_format((long) mem_rss, num_str, num_str_len);
    std::cout << "Resident Set Size: " << num_str << std::endl;
    Selfinfo::mem_size_format((long) mem_vsize, num_str, num_str_len);
    std::cout << "Virtual memory size: " << num_str << std::endl;
    std::cout << "CPU usage: " << cpu_proc_load << "%"<< std::endl;
    si.stop();
  } else {
    std::cout << "Can't find process for pid " << pid << std::endl;
  }
}

void showMemInfoByName(const char *name) {
  std::vector<unsigned int> id;
  if (getIDsByName(name, id)) {
    if (id.size() == 1) {
      showInfo(id[0]);
    } else {
      std::cout << "Several processes were found with this name:" << std::endl;
      for (unsigned int target_id : id) {
        std::cout << " -> " << target_id << std::endl;
      }
    }
  } else {
    std::cout << "Can't find process for name " << name << std::endl;
  }
}

void showHelp() {
  std::cout << "Please, indicate the process by template" << std::endl;
  std::cout << "  --pid ID" << std::endl;
  std::cout << "  --proc_name NAME" << std::endl;
}

int main(int argc, char **argv) {

  bool help_t = true;
  unsigned int pid = 0;

  if (argc == 3) {
    if (strcmp(argv[1], "--pid") == 0) {
      sscanf(argv[2], "%ud", &pid);
      showInfo(pid);
      help_t = false;
    }
    if (strcmp(argv[1], "--proc_name") == 0) {
      showMemInfoByName(argv[2]);
      help_t = false;
    }
  }

  if (help_t) showHelp();

  return 0;
}
