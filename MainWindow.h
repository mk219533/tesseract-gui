/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mk219533 (a) students.mimuw.edu.pl> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 *
 * MainWindow.h
 *
 *  Created on: 2010-01-04
 *      Author: Marcel Kolodziejczyk
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>

class ChildWidget;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTabWidget;
class QSignalMapper;
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
Q_OBJECT

public:
  MainWindow();

protected:

  void closeEvent(QCloseEvent *event);

private slots:
    void open();
    void save();
    bool closeActiveTab();
    bool closeAllTabs();
    void nextTab();
    void previousTab();
    void bold(bool checked);
    void italic(bool checked);
    void underline(bool checked);
    void zoomIn();
    void zoomOut();
    void splitSymbol();
    void joinSymbol();
    void deleteSymbol();
    void about();
    void shortcuts();
    void handleClose(int i);
    void updateMenus();
    void updateViewMenu();
    void updateCommandActions();

private:
    ChildWidget *activeChild();
    void createActions();
    void createMenus();
    void createToolBars();
    void readSettings();
    void writeSettings();

    QTabWidget *tabWidget;

    QSignalMapper *windowMapper;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *commandMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *viewToolBar;

    QAction *openAct;
    QAction *saveAct;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *exitAct;

    QAction *boldAct;
    QAction *italicAct;
    QAction *underlineAct;

    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;

    QAction *splitAct;
    QAction *joinAct;
    QAction *deleteAct;
    QAction *aboutAct;
    QAction *shortcutsAct;
};

#endif /* MAINWINDOW_H_ */
