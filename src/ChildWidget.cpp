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

ChildWidget::ChildWidget(QWidget * parent) : QSplitter(Qt::Vertical, parent)
{
  textEdit = new QPlainTextEdit;
  scene = new QGraphicsScene;
  view = new QGraphicsView(scene);

  addWidget(view);
  addWidget(textEdit);

  modified = false;
  bold = false;
  italic = false;
  underline = false;
}

bool ChildWidget::loadImage(const QString &fileName)
{
  QImage image(fileName);
  if (image.isNull()) {
    QMessageBox::information(this, tr("Wrong file"), tr("Cannot load %1.").arg(fileName));
    return false;
  }
  QString boxFileName = QFileInfo(fileName).path() + QDir::separator () + QFileInfo(fileName).baseName() + ".box";

  if (!QFile::exists(boxFileName)) {
    QMessageBox::warning(this, tr("Missing file"), tr("Cannot load image, because there is no corresponding box file"));
    return false;
  }

  if (loadBoxes(boxFileName)) {
    setCurrentImageFile(fileName);
    scene->addPixmap(QPixmap::fromImage(image));
  } else
    return false;

  return true;
}

bool ChildWidget::loadBoxes(const QString &fileName)
{
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
      QMessageBox::warning(this, tr("MDI"),
                           tr("Cannot read file %1:\n%2.")
                           .arg(fileName)
                           .arg(file.errorString()));
      return false;
  }

  QTextStream in(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  textEdit->setPlainText(in.readAll());
  QApplication::restoreOverrideCursor();

  setCurrentBoxFile(fileName);

  connect(textEdit, SIGNAL(textChanged()), this, SLOT(documentWasModified()));

  return true;
}

bool ChildWidget::save()
{/*
  QFile file(fileName);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
      QMessageBox::warning(this, tr("Application"),
                           tr("Cannot write file %1:\n%2.")
                           .arg(fileName)
                           .arg(file.errorString()));
      return false;
  }

  QTextStream out(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  out << textEdit->toPlainText();
  QApplication::restoreOverrideCursor();

  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  return true;*/
  return true;
}

void ChildWidget::zoomIn()
{
  view->scale(1.2, 1.2);
}

void ChildWidget::zoomOut()
{
  view->scale(1 / 1.2, 1 / 1.2);
}

void ChildWidget::splitSymbol()
{
  //TODO
  QMessageBox::information(this, "", tr("Split symbol in %1").arg(userFriendlyCurrentFile()));
}

void ChildWidget::joinSymbol()
{
  //TODO
  QMessageBox::information(this, "", tr("Join symbol in %1").arg(userFriendlyCurrentFile()));
}

void ChildWidget::deleteSymbol()
{
  //TODO
  QMessageBox::information(this, "", tr("Delete symbol in %1").arg(userFriendlyCurrentFile()));
}

QString ChildWidget::userFriendlyCurrentFile()
{
  return strippedName(boxFile);
}
void ChildWidget::documentWasModified()
{
  modified = true;
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
