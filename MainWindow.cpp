/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mk219533 (a) students.mimuw.edu.pl> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 *
 * MainWindow.cpp
 *
 *  Created on: 2010-01-04
 *      Author: Marcel Kolodziejczyk
 */

#include <QtGui>

#include "MainWindow.h"
#include "ChildWidget.h"

MainWindow::MainWindow()
{
  tabWidget = new QTabWidget;
  tabWidget->setTabsClosable(true);
  tabWidget->setMovable(true);
  connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(handleClose(int)));
  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateMenus()));

  setCentralWidget(tabWidget);

  windowMapper = new QSignalMapper(this);
  connect(windowMapper, SIGNAL(mapped(int)), tabWidget, SLOT(setCurrentIndex(int)));

  createActions();
  createMenus();
  createToolBars();
  updateMenus();
  readSettings();
  setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (closeAllTabs()) {
    writeSettings();
    event->accept();
  } else {
    event->ignore();
  }
}

ChildWidget *MainWindow::activeChild()
{
  if (QWidget *currentWidget = tabWidget->currentWidget())
    return qobject_cast<ChildWidget *> (currentWidget);
  return 0;
}

void MainWindow::open()
{
  QString imageFile = QFileDialog::getOpenFileName(
      this,
      QString(),
      QString(),
      "TIFF Images (*.tif *.tiff)");
  if (!imageFile.isEmpty()) {
    //TODO sprawdzic czy nie mamy otwartego takiego pliku

    ChildWidget *child = new ChildWidget;
    if (child->loadImage(imageFile)) {
      statusBar()->showMessage(tr("File loaded"), 2000);
      tabWidget->setCurrentIndex(tabWidget->addTab(child, child->userFriendlyCurrentFile()));
      connect(child, SIGNAL(boxChanged()), this, SLOT(updateCommandActions()));
    } else {
      child->close();
    }
  }
}

void MainWindow::save()
{
  if (activeChild() && activeChild()->save())
      statusBar()->showMessage(tr("File saved"), 2000);
}

bool MainWindow::closeActiveTab()
{
  if (tabWidget->currentWidget() && tabWidget->currentWidget()->close()) {
    tabWidget->removeTab(tabWidget->currentIndex());
    return true;
  }
  return false;
}

bool MainWindow::closeAllTabs()
{
  while (tabWidget->currentWidget()) {
    if (!closeActiveTab())
      return false;
  }
  return true;
}

void MainWindow::nextTab()
{
  if (tabWidget->count() > 0)
    tabWidget->setCurrentIndex((tabWidget->currentIndex() + 1) % tabWidget->count());
}

void MainWindow::previousTab()
{
  if (tabWidget->count() > 0) {
    tabWidget->setCurrentIndex((tabWidget->currentIndex() + tabWidget->count() - 1) % tabWidget->count());
  }
}

void MainWindow::bold(bool checked)
{
  if (activeChild()) {
    activeChild()->setBolded(checked);
  }
}

void MainWindow::italic(bool checked)
{
  if (activeChild()) {
    activeChild()->setItalic(checked);
  }
}

void MainWindow::underline(bool checked)
{
  if (activeChild()) {
    activeChild()->setUnderline(checked);
  }
}

void MainWindow::zoomIn()
{
  if (activeChild()) {
    activeChild()->zoomIn();
    statusBar()->showMessage(tr("Zoomed in"), 2000);
  }
}

void MainWindow::zoomOut()
{
  if (activeChild()) {
    activeChild()->zoomOut();
    statusBar()->showMessage(tr("Zoomed out"), 2000);
  }
}

void MainWindow::splitSymbol()
{
  if (activeChild()) {
    activeChild()->splitSymbol();
    statusBar()->showMessage(tr("Split symbol"), 2000);
  }
}

void MainWindow::joinSymbol()
{
  if (activeChild()) {
    activeChild()->joinSymbol();
    statusBar()->showMessage(tr("Join symbol"), 2000);
  }
}

void MainWindow::deleteSymbol()
{
  if (activeChild()) {
    activeChild()->deleteSymbol();
    statusBar()->showMessage(tr("Delete symbol"), 2000);
  }
}


//TODO
void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}

//TODO
void MainWindow::shortcuts()
{
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}

void MainWindow::handleClose(int i)
{
  if (tabWidget->widget(i) && tabWidget->widget(i)->close())
    tabWidget->removeTab(i);
}

void MainWindow::updateMenus()
{
  saveAct->setEnabled((activeChild()) ? activeChild()->isModified() : false);
  closeAct->setEnabled(activeChild() != 0);
  closeAllAct->setEnabled(activeChild() != 0);
  nextAct->setEnabled(activeChild() != 0);
  previousAct->setEnabled(activeChild() != 0);
  separatorAct->setVisible(activeChild() != 0);
  zoomInAct->setEnabled(activeChild() != 0);
  zoomOutAct->setEnabled(activeChild() != 0);
  updateCommandActions();
}

void MainWindow::updateCommandActions()
{
  bool enable = (activeChild()) ? activeChild()->isBoxSelected() : false;
  boldAct->setEnabled(enable);
  boldAct->setChecked((activeChild()) ? activeChild()->isBold() : false);
  italicAct->setEnabled(enable);
  italicAct->setChecked((activeChild()) ? activeChild()->isItalic() : false);
  underlineAct->setEnabled(enable);
  underlineAct->setChecked((activeChild()) ? activeChild()->isUnderLine() : false);
  splitAct->setEnabled(enable);
  joinAct->setEnabled(enable);
  deleteAct->setEnabled(enable);
}

void MainWindow::updateViewMenu()
{
  viewMenu->clear();
  viewMenu->addAction(nextAct);
  viewMenu->addAction(previousAct);
  viewMenu->addSeparator();
  viewMenu->addAction(zoomInAct);
  viewMenu->addAction(zoomOutAct);
  viewMenu->addAction(separatorAct);

  separatorAct->setVisible(tabWidget->count() > 0);

  for (int i = 0; i < tabWidget->count(); ++i) {
    ChildWidget *child = qobject_cast<ChildWidget *> (tabWidget->widget(i));

    QString text;
    if (i < 9) {
      text = tr("&%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
    } else {
      text = tr("%1 %2").arg(i + 1) .arg(child->userFriendlyCurrentFile());
    }
    QAction *action = viewMenu->addAction(text);
    action->setCheckable(true);
    action ->setChecked(child == activeChild());
    connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
    windowMapper->setMapping(action, i);
  }
}

void MainWindow::createActions()
{
  openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save the document to disk"));
  saveAct->setEnabled(false);
  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  closeAct = new QAction(tr("Cl&ose"), this);
  closeAct->setShortcut(QKeySequence::Close);
  closeAct->setStatusTip(tr("Close the active tab"));
  connect(closeAct, SIGNAL(triggered()), this, SLOT(closeActiveTab()));

  closeAllAct = new QAction(tr("Close &All"), this);
  closeAllAct->setShortcut(tr("Ctrl+Shift+W"));
  closeAllAct->setStatusTip(tr("Close all the tabs"));
  connect(closeAllAct, SIGNAL(triggered()), this, SLOT(closeAllTabs()));

  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  boldAct = new QAction(QIcon(":/images/textbold.png"), tr("&Bold"), this);
  boldAct->setShortcut(QKeySequence::Bold);
  boldAct->setCheckable(true);
  connect(boldAct, SIGNAL(triggered(bool)), this, SLOT(bold(bool)));

  italicAct = new QAction(QIcon(":/images/textitalic.png"), tr("&Italic"), this);
  italicAct->setShortcut(QKeySequence::Italic);
  italicAct->setCheckable(true);
  connect(italicAct, SIGNAL(triggered(bool)), this, SLOT(italic(bool)));

  underlineAct = new QAction(QIcon(":/images/textunder.png"), tr("&Underine"), this);
  underlineAct->setShortcut(QKeySequence::Underline);
  underlineAct->setCheckable(true);
  connect(underlineAct, SIGNAL(triggered(bool)), this, SLOT(underline(bool)));

  zoomInAct = new QAction(QIcon(":/images/zoomin.png"), tr("Zoom &in"), this);
  zoomInAct->setShortcut(QKeySequence::ZoomIn);
  connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

  zoomOutAct = new QAction(QIcon(":/images/zoomout.png"), tr("Zoom &out"), this);
  zoomOutAct->setShortcut(QKeySequence::ZoomOut);
  connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

  nextAct = new QAction(tr("Ne&xt"), this);
  nextAct->setShortcuts(QKeySequence::NextChild);
  nextAct->setStatusTip(tr("Move the focus to the next window"));
  connect(nextAct, SIGNAL(triggered()), this, SLOT(nextTab()));

  previousAct = new QAction(tr("Pre&vious"), this);
  previousAct->setShortcuts(QKeySequence::PreviousChild);
  previousAct->setStatusTip(tr("Move the focus to the previous window"));
  connect(previousAct, SIGNAL(triggered()), this, SLOT(previousTab()));

  separatorAct = new QAction(this);
  separatorAct->setSeparator(true);


  splitAct = new QAction(tr("&Split symbol"), this);
  splitAct->setShortcut(tr("Ctrl+2"));
  connect(splitAct, SIGNAL(triggered()), this, SLOT(splitSymbol()));

  joinAct = new QAction(tr("&Join with Next Symbol"), this);
  joinAct->setShortcut(tr("Ctrl+1"));
  connect(joinAct, SIGNAL(triggered()), this, SLOT(joinSymbol()));

  deleteAct = new QAction(tr("&Delete symbol"), this);
  deleteAct->setShortcut(QKeySequence::Delete);
  connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteSymbol()));

  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  shortcutsAct = new QAction(tr("&Keyboard shortcuts"), this);
  shortcutsAct->setStatusTip(tr("Show the application's shortcuts"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);
    fileMenu->addAction(closeAllAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(boldAct);
    editMenu->addAction(italicAct);
    editMenu->addAction(underlineAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    updateViewMenu();
    connect(viewMenu, SIGNAL(aboutToShow()), this, SLOT(updateViewMenu()));

    commandMenu = menuBar()->addMenu(tr("&Command"));
    commandMenu->addAction(splitAct);
    commandMenu->addAction(joinAct);
    commandMenu->addAction(deleteAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(shortcutsAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    viewToolBar = addToolBar(tr("View"));
    viewToolBar->addAction(zoomInAct);
    viewToolBar->addAction(zoomOutAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(boldAct);
    editToolBar->addAction(italicAct);
    editToolBar->addAction(underlineAct);
}

void MainWindow::readSettings()
{
  QSettings settings("", "tesseract-gui");
  QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
  QSize size = settings.value("size", QSize(400, 400)).toSize();
  move(pos);
  resize(size);
}

void MainWindow::writeSettings()
{
  QSettings settings("", "tesseract-gui");
  settings.setValue("pos", pos());
  settings.setValue("size", size());
}
