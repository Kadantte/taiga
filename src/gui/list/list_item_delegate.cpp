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

#include "list_item_delegate.hpp"

#include <QComboBox>
#include <QPainter>
#include <QProxyStyle>

#include "gui/list/list_model.hpp"
#include "gui/utils/painter_state_saver.hpp"

namespace gui {

ListItemDelegate::ListItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent), m_proxyStyle(new QProxyStyle("fusion")) {
  m_proxyStyle->setParent(parent);
}

void ListItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
  auto* combobox = static_cast<QComboBox*>(editor);
  combobox->setCurrentIndex(0);
}

QWidget* ListItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&,
                                        const QModelIndex& index) const {
  if (index.column() != ListModel::COLUMN_SCORE) return nullptr;

  auto* editor = new QComboBox(parent);
  editor->addItem("-");
  editor->setItemData(0, Qt::AlignCenter, Qt::TextAlignmentRole);
  for (int i = 1; i <= 10; ++i) {
    editor->addItem(tr("%1").arg(i));
    editor->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
  }

  return editor;
}

void ListItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                    const QModelIndex& index) const {
  const auto* combobox = static_cast<QComboBox*>(editor);
  model->setData(index, combobox->currentText(), Qt::EditRole);
}

void ListItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                            const QModelIndex&) const {
  editor->setGeometry(option.rect);
}

void ListItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                             const QModelIndex& index) const {
  // Grid lines
  if (index.column() > 0) {
    const PainterStateSaver painterStateSaver(painter);
    painter->setPen(QColor{0, 0, 0, 8});
    painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
  }

  switch (index.column()) {
    case ListModel::COLUMN_PROGRESS: {
      const PainterStateSaver painterStateSaver(painter);

      const auto anime = index.data(Qt::UserRole).value<const Anime*>();
      const int episodes = anime->episode_count;
      const int progress = episodes > 0 ? std::clamp(3, 0, episodes) : 3;
      const int percent = episodes > 0 ? static_cast<float>(progress) / episodes * 100.0f : 50;
      const auto text =
          tr("%1/%2").arg(progress).arg(episodes > 0 ? QString::number(episodes) : "?");

      QStyleOptionProgressBar styleOption{};
      styleOption.state = option.state | QStyle::State_Horizontal;
      styleOption.direction = option.direction;
      styleOption.rect = option.rect.adjusted(2, 2, -2, -2);
      styleOption.palette = option.palette;
      styleOption.palette.setCurrentColorGroup(QPalette::Active);
      styleOption.fontMetrics = option.fontMetrics;
      styleOption.maximum = 100;
      styleOption.minimum = 0;
      styleOption.progress = percent;
      styleOption.text = text;
      styleOption.textAlignment = Qt::AlignCenter;
      styleOption.textVisible = true;

      m_proxyStyle->drawControl(QStyle::CE_ProgressBar, &styleOption, painter);
      break;
    }
  }

  QStyledItemDelegate::paint(painter, option, index);
}

QSize ListItemDelegate::sizeHint(const QStyleOptionViewItem& option,
                                 const QModelIndex& index) const {
  if (index.isValid()) {
    return QSize(0, 24);
  }

  return QStyledItemDelegate::sizeHint(option, index);
}

}  // namespace gui
