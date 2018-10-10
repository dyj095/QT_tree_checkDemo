#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();
    void updateParentItem(QTreeWidgetItem* item);

    void setParentPartiallyChecked(QTreeWidgetItem *itm);


    QTreeWidgetItem* addChildNode(QTreeWidgetItem *parent, int index, QString namePre);
    QTreeWidgetItem* addChildEmpNode(QTreeWidgetItem *parent, int index);

public slots:
    //申明信号与槽,当树形控件的子选项被改变时执行
    void treeItemChanged(QTreeWidgetItem* item , int column);

    void onItemExpanded(QTreeWidgetItem * item);
    void onItemCollapsed(QTreeWidgetItem * item);
    void onItemClicked(QTreeWidgetItem * item, int column);

private:
    void loadStyleSheet();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
