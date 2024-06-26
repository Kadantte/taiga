/**
 * Taiga
 * Copyright (C) 2010-2024, Eren Okka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "list_view.hpp"

#include <QHeaderView>
#include <QStatusBar>

#include "gui/list/list_item_delegate.hpp"
#include "gui/list/list_model.hpp"
#include "gui/main/main_window.hpp"
#include "gui/main/now_playing_widget.hpp"
#include "gui/media/media_dialog.hpp"
#include "gui/media/media_menu.hpp"
#include "media/anime.hpp"

namespace gui {

ListView::ListView(QWidget* parent, MainWindow* mainWindow)
    : QTreeView(parent), m_model(new ListModel(this)), m_mainWindow(mainWindow) {
  setObjectName("animeList");

  setItemDelegate(new ListItemDelegate(this));
  setModel(m_model);

  setAllColumnsShowFocus(true);
  setAlternatingRowColors(true);
  setContextMenuPolicy(Qt::CustomContextMenu);
  setExpandsOnDoubleClick(false);
  setFrameShape(QFrame::Shape::NoFrame);
  setItemsExpandable(false);
  setRootIsDecorated(false);
  setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
  setSortingEnabled(true);
  setUniformRowHeights(true);

  header()->setFirstSectionMovable(true);
  header()->setStretchLastSection(false);
  header()->setTextElideMode(Qt::ElideRight);
  header()->resizeSection(ListModel::COLUMN_TITLE, 300);
  header()->resizeSection(ListModel::COLUMN_PROGRESS, 150);

  connect(this, &QWidget::customContextMenuRequested, this, [this]() {
    const auto selectedRows = selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) return;

    QList<Anime> items;
    for (auto selectedIndex : selectedRows) {
      if (const auto item = m_model->getAnime(selectedIndex)) {
        items.push_back(*item);
      }
    }

    auto* menu = new MediaMenu(this, items);
    menu->popup();
  });

  connect(this, &QAbstractItemView::clicked, this,
          qOverload<const QModelIndex&>(&QAbstractItemView::edit));

  connect(this, &QAbstractItemView::doubleClicked, this, [this](const QModelIndex& index) {
    const auto selectedItem = m_model->getAnime(index);
    if (!selectedItem) return;
    MediaDialog::show(this, *selectedItem);
  });

  connect(selectionModel(), &QItemSelectionModel::selectionChanged, this,
          &ListView::selectionChanged);
}

void ListView::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
  QTreeView::selectionChanged(selected, deselected);

  if (!selected.empty()) {
    const auto selectedItem = m_model->getAnime(selected.indexes().first());
    m_mainWindow->nowPlaying()->setPlaying(*selectedItem);
    m_mainWindow->nowPlaying()->show();
  } else if (m_mainWindow && m_mainWindow->nowPlaying()) {
    m_mainWindow->nowPlaying()->hide();
  }

  if (const auto n = selectionModel()->selectedRows().size()) {
    m_mainWindow->statusBar()->showMessage(tr("%n item(s) selected", nullptr, n));
  } else {
    m_mainWindow->statusBar()->clearMessage();
  }
}

}  // namespace gui
