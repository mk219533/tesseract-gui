/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mk219533 (a) students.mimuw.edu.pl> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 *
 * ChildWidget.h
 *
 *  Created on: 2010-01-06
 *      Author: Marcel Kolodziejczyk
 */

#ifndef CHILDWIDGET_H_
#define CHILDWIDGET_H_

#include <QSplitter>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QGraphicsView;
class QAbstractItemModel;
class QItemSelectionModel;
class QTableView;
class QGraphicsItem;
class QGraphicsRectItem;
//class QStandardItem;
QT_END_NAMESPACE

class ChildWidget : public QSplitter
{
  Q_OBJECT

public:
  ChildWidget(QWidget * parent = 0);

  bool loadImage(const QString &fileName);
  bool loadBoxes(const QString &fileName);
  bool save();
  bool isModified() { return modified; }
  bool isBoxSelected();
  bool isBold();
  bool isItalic();
  bool isUnderLine();
  void setBolded(bool v);
  void setItalic(bool v);
  void setUnderline(bool v);
  void zoomIn();
  void zoomOut();
  void splitSymbol();
  void joinSymbol();
  void deleteSymbol();
  QString userFriendlyCurrentFile();

private slots:
  void documentWasModified();
  void emitBoxChanged();
  void drawSelectionRects();
//  void updateModel(QStandardItem *);

protected:
  void closeEvent(QCloseEvent *event);

  bool maybeSave();

  void setCurrentImageFile(const QString &fileName);
  void setCurrentBoxFile(const QString &fileName);

  QString strippedName(const QString &fullFileName);

signals:
  void boxChanged();
  void modifiedChanged();

protected:
  QGraphicsScene *imageScene;
  QGraphicsView *imageView;
  QGraphicsItem * imageItem;
  QGraphicsRectItem *imageSelectionRect;
/*
  QGraphicsScene *textScene;
  QGraphicsView *textView;
  QGraphicsRectItem *textSelectionRect;
//*/
  QTableView *table;

  QAbstractItemModel *model;
  QItemSelectionModel *selectionModel;

  QString imageFile;
  QString boxFile;

  bool modified;
  int imageHeight;
};

#endif /* CHILDWIDGET_H_ */
