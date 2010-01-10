/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mk219533 (a) students.mimuw.edu.pl> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 *
 * ChildWidget.cpp
 *
 *  Created on: 2010-01-06
 *      Author: Marcel Kolodziejczyk
 */

#include <QtGui>
#include "ChildWidget.h"

ChildWidget::ChildWidget(QWidget * parent) :
  QSplitter(Qt::Horizontal, parent)
{
  model = new QStandardItemModel(0, 8, this);
  model->setHeaderData(0, Qt::Horizontal, tr("Letter"));
  model->setHeaderData(1, Qt::Horizontal, tr("Left"));
  model->setHeaderData(2, Qt::Horizontal, tr("Bottom"));
  model->setHeaderData(3, Qt::Horizontal, tr("Right"));
  model->setHeaderData(4, Qt::Horizontal, tr("Top"));
  model->setHeaderData(5, Qt::Horizontal, tr("Italic"));
  model->setHeaderData(6, Qt::Horizontal, tr("Bold"));
  model->setHeaderData(7, Qt::Horizontal, tr("Underline"));

  table = new QTableView;
  table->setModel(model);
  selectionModel = new QItemSelectionModel(model);
  connect(
      selectionModel,
      SIGNAL(selectionChanged ( const QItemSelection & , const QItemSelection & ) ),
      this,
      SLOT(emitBoxChanged()));
  connect(
      selectionModel,
      SIGNAL(selectionChanged ( const QItemSelection & , const QItemSelection & ) ),
      this,
      SLOT(drawSelectionRects()));
  table->setSelectionModel(selectionModel);
  table->verticalHeader()->hide();
  table->setSelectionBehavior(QAbstractItemView::SelectItems);
  table->setSelectionMode(QAbstractItemView::SingleSelection);
  //table->hideColumn(5);
  //table->hideColumn(6);
  //table->hideColumn(7);

  addWidget(table);

  imageScene = new QGraphicsScene;
  QColor selectionColor(Qt::red);
  selectionColor.setAlpha(127);
  imageSelectionRect = imageScene->addRect(0, 0, 0, 0, QPen(Qt::red), selectionColor);
  imageSelectionRect->setZValue(1);
  imageView = new QGraphicsView(imageScene);
  addWidget(imageView);

  modified = false;
}

bool ChildWidget::loadImage(const QString &fileName)
{
  QImage image(fileName);
  if (image.isNull()) {
    QMessageBox::information(this, tr("Wrong file"), tr("Cannot load %1.").arg(fileName));
    return false;
  }
  QString boxFileName = QFileInfo(fileName).path() + QDir::separator()
      + QFileInfo(fileName).baseName() + ".box";

  if (!QFile::exists(boxFileName)) {
    QMessageBox::warning(this, tr("Missing file"), tr(
        "Cannot load image, because there is no corresponding box file"));
    return false;
  }
  imageHeight = image.height();

  if (loadBoxes(boxFileName)) {
    setCurrentImageFile(fileName);
    imageItem = imageScene->addPixmap(QPixmap::fromImage(image));
    imageSelectionRect->setParentItem(imageItem);
    modified = false;
    emit
    modifiedChanged();
    connect(model, SIGNAL(itemChanged ( QStandardItem * ) ), this, SLOT(emitBoxChanged()));
    connect(model, SIGNAL(itemChanged ( QStandardItem * ) ), this, SLOT(documentWasModified()));
  } else
    return false;

  return true;
}

bool ChildWidget::loadBoxes(const QString &fileName)
{
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("MDI"), tr("Cannot read file %1:\n%2.") .arg(fileName) .arg(
        file.errorString()));
    return false;
  }

  QTextStream in(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  QString line;
  int row = 0;
  do {
    line = in.readLine();
    if (!line.isEmpty()) {

      model->insertRow(row);
      QStringList pieces = line.split(" ", QString::SkipEmptyParts);
      QString letter = pieces.value(0);
      bool bold = false, italic = false, underline = false;
      if (letter.at(0) == '@') {
        bold = true;
        letter.remove(0, 1);
      }
      if (letter.at(0) == '$') {
        italic = true;
        letter.remove(0, 1);
      }
      if (letter.at(0) == '\'') {
        underline = true;
        letter.remove(0, 1);
      }
      int left = pieces.value(1).toInt();
      int bottom = imageHeight - pieces.value(2).toInt();
      int right = pieces.value(3).toInt();
      int top = imageHeight - pieces.value(4).toInt();
      model->setData(model->index(row, 0, QModelIndex()), letter);
      model->setData(model->index(row, 1, QModelIndex()), left);
      model->setData(model->index(row, 2, QModelIndex()), bottom);
      model->setData(model->index(row, 3, QModelIndex()), right);
      model->setData(model->index(row, 4, QModelIndex()), top);
      model->setData(model->index(row, 5, QModelIndex()), italic);
      model->setData(model->index(row, 6, QModelIndex()), bold);
      model->setData(model->index(row, 7, QModelIndex()), underline);
      row++;
    }
  } while (!line.isEmpty());
  table->resizeColumnsToContents();
  file.close();
  QApplication::restoreOverrideCursor();

  setCurrentBoxFile(fileName);

  return true;
}

bool ChildWidget::save()
{
  QFile file(boxFile);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(
        this,
        tr("tesseract-gui"),
        tr("Cannot write file %1:\n%2.") .arg(boxFile) .arg(file.errorString()));
    return false;
  }

  QTextStream out(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);

  for (int row = 0; row < model->rowCount(); ++row) {
    QString letter = model->index(row, 0).data().toString();
    int left = model->index(row, 1).data().toInt();
    int bottom = model->index(row, 2).data().toInt();
    int right = model->index(row, 3).data().toInt();
    int top = model->index(row, 4).data().toInt();
    bool italic = model->index(row, 5).data().toBool();
    bool bold = model->index(row, 6).data().toBool();
    bool underline = model->index(row, 7).data().toBool();
    if (bold)
      out << "@";
    if (italic)
      out << "$";
    if (underline)
      out << "\'";
    out << letter << " " << left << " " << imageHeight - bottom << " " << right << " "
        << imageHeight - top << "\n";
  }

  QApplication::restoreOverrideCursor();

  modified = false;
  emit modifiedChanged();

  return true;
}

bool ChildWidget::isBoxSelected()
{
  return selectionModel->hasSelection();
}

bool ChildWidget::isBold()
{
  QModelIndex index = selectionModel->currentIndex();
  if (index.isValid()) {
    return model->index(index.row(), 6).data().toBool();
  }
  return false;
}

bool ChildWidget::isItalic()
{
  QModelIndex index = selectionModel->currentIndex();
  if (index.isValid()) {
    return model->index(index.row(), 5).data().toBool();
  }
  return false;
}

bool ChildWidget::isUnderLine()
{
  QModelIndex index = selectionModel->currentIndex();
  if (index.isValid()) {
    return model->index(index.row(), 7).data().toBool();
  }
  return false;
}

void ChildWidget::setBolded(bool v)
{
  QModelIndex index = selectionModel->currentIndex();
  if (index.isValid()) {
    model->setData(model->index(index.row(), 6, QModelIndex()), v);
  }
}

void ChildWidget::setItalic(bool v)
{
  QModelIndex index = selectionModel->currentIndex();
  if (index.isValid()) {
    model->setData(model->index(index.row(), 5, QModelIndex()), v);
  }
}

void ChildWidget::setUnderline(bool v)
{
  QModelIndex index = selectionModel->currentIndex();
  if (index.isValid()) {
    model->setData(model->index(index.row(), 7, QModelIndex()), v);
  }
}

void ChildWidget::zoomIn()
{
  imageView->scale(1.2, 1.2);
}

void ChildWidget::zoomOut()
{
  imageView->scale(1 / 1.2, 1 / 1.2);
}

void ChildWidget::splitSymbol()
{
  QModelIndex index = selectionModel->currentIndex();
  if (index.isValid()) {
    QModelIndex left = model->index(index.row(), 1);
    QModelIndex right = model->index(index.row(), 3);
    int width = right.data().toInt() - left.data().toInt();
    model->insertRow(index.row() + 1);
    model->setData(model->index(index.row() + 1, 0), "*");
    model->setData(model->index(index.row() + 1, 1), right.data().toInt() - width / 2);
    model->setData(model->index(index.row() + 1, 2), model->index(index.row(), 2).data().toInt());
    model->setData(model->index(index.row() + 1, 3), right.data().toInt());
    model->setData(model->index(index.row() + 1, 4), model->index(index.row(), 4).data().toInt());
    model->setData(model->index(index.row() + 1, 5), model->index(index.row(), 5).data().toBool());
    model->setData(model->index(index.row() + 1, 6), model->index(index.row(), 6).data().toBool());
    model->setData(model->index(index.row() + 1, 7), model->index(index.row(), 7).data().toBool());
    model->setData(right, right.data().toInt() - width / 2);
    drawSelectionRects();
  }
}

template<class T>
inline T min(T arg1, T arg2)
{
  return ((arg1 < arg2) ? arg1 : arg2);
}

template<class T>
inline T max(T arg1, T arg2)
{
  return ((arg1 > arg2) ? arg1 : arg2);
}

void ChildWidget::joinSymbol()
{
  QModelIndex index = selectionModel->currentIndex(), next = model->index(
      index.row() + 1,
      index.column());

  if (index.isValid() && next.isValid()) {
    int row = index.row();
    model->setData(model->index(row, 0), model->index(row, 0).data().toString() + model->index(row
        + 1, 0).data().toString());
    min(2, 3);
    model->setData(model->index(row, 1), min(model->index(row, 1).data().toInt(), model->index(row
        + 1, 1).data().toInt()));
    model->setData(model->index(row, 2), max(model->index(row, 2).data().toInt(), model->index(row
        + 1, 2).data().toInt()));
    model->setData(model->index(row, 3), max(model->index(row, 3).data().toInt(), model->index(row
        + 1, 3).data().toInt()));
    model->setData(model->index(row, 4), min(model->index(row, 4).data().toInt(), model->index(row
        + 1, 4).data().toInt()));
    model->setData(model->index(row, 5), model->index(row, 5).data().toBool() || model->index(row
        + 1, 5).data().toBool());
    model->setData(model->index(row, 6), model->index(row, 6).data().toBool() || model->index(row
        + 1, 6).data().toBool());
    model->setData(model->index(row, 7), model->index(row, 7).data().toBool() || model->index(row
        + 1, 7).data().toBool());
    model->removeRow(row + 1);
    drawSelectionRects();
  }
}

void ChildWidget::deleteSymbol()
{
  QModelIndex index = selectionModel->currentIndex();
  if (index.isValid()) {
    model->removeRow(index.row());
  }
}

QString ChildWidget::userFriendlyCurrentFile()
{
  return strippedName(boxFile);
}
void ChildWidget::documentWasModified()
{
  modified = true;
  emit modifiedChanged();
}

void ChildWidget::emitBoxChanged()
{
  emit boxChanged();
}

void ChildWidget::drawSelectionRects()
{
  QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
  if (!selectedIndexes.empty()) {
    QModelIndex index = selectedIndexes.first();
    int left = model->index(index.row(), 1).data().toInt();
    int bottom = model->index(index.row(), 2).data().toInt();
    int right = model->index(index.row(), 3).data().toInt();
    int top = model->index(index.row(), 4).data().toInt();
    imageSelectionRect->setRect(QRectF(QPoint(left, top), QPointF(right, bottom)));
    imageSelectionRect->setVisible(true);
    imageView->ensureVisible(imageSelectionRect);
  } else {
    imageSelectionRect->setVisible(false);
  }
}

void ChildWidget::closeEvent(QCloseEvent *event)
{
  if (!maybeSave()) {
    event->ignore();
  }
}

bool ChildWidget::maybeSave()
{
  if (isModified()) {
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("MDI"), tr("'%1' has been modified.\n"
      "Do you want to save your changes?") .arg(userFriendlyCurrentFile()), QMessageBox::Save
        | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
      return save();
    else if (ret == QMessageBox::Cancel)
      return false;
  }
  return true;
}

void ChildWidget::setCurrentImageFile(const QString &fileName)
{
  imageFile = QFileInfo(fileName).canonicalFilePath();
}

void ChildWidget::setCurrentBoxFile(const QString &fileName)
{
  boxFile = QFileInfo(fileName).canonicalFilePath();
}

QString ChildWidget::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}
