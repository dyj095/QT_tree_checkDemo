#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DepartNodeItem.h"
#include "EmployeeNodeItem.h"

#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadStyleSheet();
    init();
    connect(ui->tree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(treeItemChanged(QTreeWidgetItem*,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    ui->tree->setHeaderHidden(true);
    // 1.创建表格
    ui->tree->setColumnCount(2);
    // 2.拿到表头
    QHeaderView *head = ui->tree->header();
    // 3.设置不能拉伸的列的宽度，设置哪一列能拉伸
    head->setSectionResizeMode(0,QHeaderView::Stretch);
    head->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tree->setColumnWidth(1, 30);
    // 4.（最重要的一步）去掉默认的拉伸最后列属性
    head->setStretchLastSection(false);

    //展开和收缩时信号，以达到变更我三角图片；
    connect(ui->tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(onItemClicked(QTreeWidgetItem *, int)));
    connect(ui->tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(onItemExpanded(QTreeWidgetItem *)));
    connect(ui->tree, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(onItemCollapsed(QTreeWidgetItem *)));
    for(int i = 0; i < 10; i++){
        // 一级部门节点
        QTreeWidgetItem *pRootDeptItem = new QTreeWidgetItem();
        pRootDeptItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        //设置Data用于区分，Item是分组节点还是子节点，0代表分组节点，1代表子节点
        pRootDeptItem->setData(0, Qt::UserRole, 0);
        pRootDeptItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        pRootDeptItem->setCheckState(1, Qt::Unchecked);
        DepartNodeItem *pItemName = new DepartNodeItem(ui->tree);
        pItemName->setLevel(0);

        int nMyFriendNum = 6;
        QString qsGroupName = QString::fromLocal8Bit("一级部门%3 [%1/%2]").arg(0).arg(nMyFriendNum).arg(i);
        pItemName->setText(qsGroupName);
        //插入分组节点
        ui->tree->addTopLevelItem(pRootDeptItem);
        ui->tree->setItemWidget(pRootDeptItem, 0, pItemName);
        for(int j = 0; j < 5; j++){
            //addChildEmpNode(pRootDeptItem, j);
        }
        for(int j = 0; j < 5; j++){
            QString name = QString::fromLocal8Bit("二级部门%1").arg(j);
            QTreeWidgetItem *childItem = addChildNode(pRootDeptItem, i * 10 + j,name);
            for(int g = 0; g < 5; g++){
                addChildEmpNode(childItem, g);
            }
        }
    }
}

QTreeWidgetItem* MainWindow::addChildNode(QTreeWidgetItem *parent, int index, QString namePre)
{
    QTreeWidgetItem *pDeptItem = new QTreeWidgetItem();
    pDeptItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    //设置Data用于区分，Item是分组节点还是子节点，0代表分组节点，1代表子节点
    pDeptItem->setData(0, Qt::UserRole, 0);
    DepartNodeItem *pItemName = new DepartNodeItem(ui->tree);
    pDeptItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    pDeptItem->setCheckState(1, Qt::Unchecked);
    int level = 0;
    DepartNodeItem *departNode = dynamic_cast<DepartNodeItem*>(ui->tree->itemWidget(parent, 0));
    if (departNode) {
        level = departNode->getLevel();
        level ++;
    }
    pItemName->setLevel(level);

    int nMyFriendNum = 6;
    QString qsGroupName = QString::fromLocal8Bit("%4%3 [%1/%2]").arg(0).arg(nMyFriendNum).arg(index).arg(namePre);
    pItemName->setText(qsGroupName);
    //擦入分组节点
    parent->addChild(pDeptItem);
    ui->tree->setItemWidget(pDeptItem, 0, pItemName);

    return pDeptItem;
}

void MainWindow::updateParentItem(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    if(parent == NULL)
    {
        return ;
    }
    int selectedCount = 0;
    int childCount = parent->childCount();
    //判断有多少个子项被选中
    for(int i=0; i<childCount; i++) {
        QTreeWidgetItem* childItem = parent->child(i);
        if(childItem->checkState(1) == Qt::Checked) {
            selectedCount++;
        }
    }
    if(selectedCount <= 0)  //如果没有子项被选中，父项设置为未选中状态
    {
        parent->setCheckState(1,Qt::Unchecked);
    }
    else if(selectedCount>0 && selectedCount<childCount)    //如果有部分子项被选中，父项设置为部分选中状态，即用灰色显示
    {
        parent->setCheckState(1,Qt::PartiallyChecked);
        // 重点：针对半选中状态的设置需要单独调用setParentPartiallyChecked后返回，否则上级节点的状态及当前节点的下级第一个节点的状态会不正确
        setParentPartiallyChecked(parent);
        return;
    }
    else if(selectedCount == childCount)    //如果子项全部被选中，父项则设置为选中状态
    {
        parent->setCheckState(1,Qt::Checked);
    }
    updateParentItem(parent);
}

void MainWindow::setParentPartiallyChecked(QTreeWidgetItem *itm)
{
    QTreeWidgetItem *parent = itm->parent();
    if(parent) {
        parent->setCheckState(1,Qt::PartiallyChecked);
        setParentPartiallyChecked(parent);
    }
}

void MainWindow::treeItemChanged(QTreeWidgetItem *item, int column)
{
    if(Qt::Checked == item->checkState(1)){
        int count = item->childCount(); //返回子项的个数
        if(count >0) {
            for(int i=0; i<count; i++) {
                item->child(i)->setCheckState(1,Qt::Checked);
            }
        } else {
            updateParentItem(item);
        }
    } else if(Qt::Unchecked == item->checkState(1)) {
        int count = item->childCount();
        if(count > 0) {
            for(int i=0; i<count; i++) {
                item->child(i)->setCheckState(1,Qt::Unchecked);
            }
        } else {
            updateParentItem(item);
        }
    }
}
void MainWindow::onItemExpanded(QTreeWidgetItem * item)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if (!bIsChild) {
        DepartNodeItem *departNode = dynamic_cast<DepartNodeItem*>(ui->tree->itemWidget(item, 0));
        if (departNode) {
            departNode->setExpanded(true);
        }
    }

}

void MainWindow::onItemCollapsed(QTreeWidgetItem * item)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if (!bIsChild) {
        DepartNodeItem *departNode = dynamic_cast<DepartNodeItem*>(ui->tree->itemWidget(item, 0));
        if (departNode) {
            departNode->setExpanded(false);
        }
    }
}

void MainWindow::onItemClicked(QTreeWidgetItem * item, int column)
{
    if (column == 0) {
        bool bIsChild = item->data(0, Qt::UserRole).toBool();
        if (!bIsChild)
        {
            item->setExpanded(!item->isExpanded());
        }
    }
}

void MainWindow::loadStyleSheet()
{
    QFile file(":/tree/Resources/QSS/MainWindow.css");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        this->setStyleSheet("");
        QString qsstyleSheet = QString(file.readAll());
        this->setStyleSheet(qsstyleSheet);
    }
    file.close();
}

QTreeWidgetItem *MainWindow::addChildEmpNode(QTreeWidgetItem *parent, int index)
{
    QTreeWidgetItem *pDeptItem = new QTreeWidgetItem();
    //设置Data用于区分，Item是分组节点还是子节点，0代表分组节点，1代表子节点
    pDeptItem->setData(0, Qt::UserRole, 1);
    pDeptItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    pDeptItem->setCheckState(1, Qt::Unchecked);
    int level = 0;
    DepartNodeItem *departNode = dynamic_cast<DepartNodeItem*>(ui->tree->itemWidget(parent, 0));
    if (departNode) {
        level = departNode->getLevel();
        level ++;
    }

    EmployeeNodeItem *pItemName = new EmployeeNodeItem(ui->tree);
    pItemName->setLevel(level);
    // 加载本地文件，需要修改成本地的路径
    pItemName->setHeadPath(QString::fromLocal8Bit("D:/Qt/workspace/project/QT_tree_checkDemo/images/pic/%1.jpg").arg(index));

    QString qfullName = QString::fromLocal8Bit("人员%1").arg(index);
    pItemName->setFullName(qfullName);
    //擦入分组节点
    parent->addChild(pDeptItem);
    ui->tree->setItemWidget(pDeptItem, 0, pItemName);
    return pDeptItem;
}
