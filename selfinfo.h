
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

#ifndef SELFINFO_H_INCLUDED
#define SELFINFO_H_INCLUDED

namespace Selfinfo {

bool mem_usage(const unsigned int pid,
               unsigned long int &mem_rss,
               long int &mem_vsize);

void mem_size_format(const long mem_size,
                     char *str,
                     const unsigned int str_len);

}

#endif // SELFINFO_H_INCLUDED
