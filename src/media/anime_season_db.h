/*
** Taiga
** Copyright (C) 2010-2019, Eren Okka
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <vector>

#include "media/anime_season.h"

namespace anime {

class SeasonDatabase {
public:
  bool IsRefreshRequired() const;

  void Set(const Season& season);
  void Reset();

  void Review(bool hide_nsfw = true);

  std::vector<int> items;
  Season current_season;
};

inline SeasonDatabase season_db;

}  // namespace anime