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

#include "list_widget.hpp"

#include <QListView>
#include <QToolBar>
#include <QToolButton>

#include "gui/list/list_view.hpp"
#include "gui/utils/theme.hpp"
#include "ui_list_widget.h"

namespace gui {

ListWidget::ListWidget(QWidget* parent, MainWindow* mainWindow)
    : QWidget(parent),
      m_listView(new ListView(this, mainWindow)),
      m_mainWindow(mainWindow),
      ui_(new Ui::ListWidget) {
  ui_->setupUi(this);

  ui_->filterButton->hide();

  // Toolbar
  {
    auto toolbar = new QToolBar(this);
    toolbar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
    toolbar->setIconSize({18, 18});

    toolbar->addAction("TV");
    toolbar->addAction("Finished airing");
    toolbar->addAction("Average > 80%");

    {
      auto button = new QToolButton(this);
      button->setToolButtonStyle(Qt::ToolButtonIconOnly);
      button->setIcon(theme.getIcon("add_box"));
      button->setText("Add filter");
      toolbar->addWidget(button);
    }

    ui_->toolbarLayout->insertWidget(1, toolbar);
  }

  ui_->editFilter->hide();

  // Toolbar
  {
    auto toolbar = new QToolBar(this);
    toolbar->setIconSize({18, 18});
    ui_->actionSort->setIcon(theme.getIcon("sort"));
    ui_->actionView->setIcon(theme.getIcon("grid_view"));
    ui_->actionMore->setIcon(theme.getIcon("more_horiz"));
    toolbar->addAction(ui_->actionSort);
    toolbar->addAction(ui_->actionView);
    toolbar->addAction(ui_->actionMore);
    ui_->toolbarLayout->addWidget(toolbar);
  }

  // List
  ui_->verticalLayout->addWidget(m_listView);
}

}  // namespace gui
