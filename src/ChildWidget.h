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
class QPlainTextEdit;
class QGraphicsScene;
class QGraphicsView;
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
  bool isBold() { return bold; }
  bool isItalic() { return italic; }
  bool isUnderLine() { return underline; }
  void setBolded(bool v) { bold = v; }
  void setItalic(bool v) { italic = v; }
  void setUnderline(bool v) { underline = v; }
  void zoomIn();
  void zoomOut();
  void splitSymbol();
  void joinSymbol();
  void deleteSymbol();
  QString userFriendlyCurrentFile();

private slots:
    void documentWasModified();

protected:
    void closeEvent(QCloseEvent *event);

    bool maybeSave();

    void setCurrentImageFile(const QString &fileName);
    void setCurrentBoxFile(const QString &fileName);

    QString strippedName(const QString &fullFileName);

protected:
  QGraphicsScene *scene;
  QGraphicsView *view;
  QPlainTextEdit *textEdit;

  QString imageFile;
  QString boxFile;

  bool modified;
  bool bold;
  bool italic;
  bool underline;

};

#endif /* CHILDWIDGET_H_ */
