
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"QTreeWidgetItem"
#include"QFileInfoList"
#include"QDir"
#include"QDebug"
#include <QFileDialog>
#include <QString>
#include <qdesktopservices.h>
//this->addTopLevelItem(item);
#include <dialog.h>
QString lujing;
QString treewway;
QString baocunlujing;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     ui->treeWidget->setHeaderLabels(QStringList()<<"目录");//设置目录
//    QString rootpath=lujing;
//        QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootpath);
//        root->setIcon(0, QIcon("D:/1.jpg"));
//        root->setCheckState(1, Qt::Checked);
//        allfile(root,rootpath);        //遍地添加/home/XXX目录下所有文件，此函数具体内容如下

//
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget,SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(SlotCustomContextMenuRequested(const QPoint&)));//实现右击添加菜单的信号与槽的连接
//    ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);   //使得该控件可以支持右键菜单

//    connect(ui.treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
//            this, SLOT(SlotCustomContextMenuRequested(const QPoint&)));//实现右击添加菜单的信号与槽的连接
     connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem* ,int )), this,SLOT(checkself(QTreeWidgetItem* ,int )));
     ui->treeWidget->setDefaultDropAction(Qt::MoveAction);

     ui->treeWidget->setDragDropMode(QAbstractItemView::DragDrop);

//     ui->treeWidget->setSelectionBehavior(QTreeWidget::SelectRows);     // 选择行
//         ui->treeWidget->setSelectionMode(QTreeWidget::ContiguousSelection);// 连续选择

}

MainWindow::~MainWindow()
{
    delete ui;
}

QFileInfoList MainWindow::allfile(QTreeWidgetItem *root,QString path)         //参数为主函数中添加的item和路径名
{

    /*添加path路径文件*/
    QDir dir(path);          //遍历各级子目录
    QDir dir_file(path);    //遍历子目录中所有文件
    dir_file.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);        //获取当前所有文件
    dir_file.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list_file = dir_file.entryInfoList();
    for (int i = 0; i < list_file.size(); ++i) {       //将当前目录中所有文件添加到treewidget中
        QFileInfo fileInfo = list_file.at(i);
        QString name2=fileInfo.fileName();
        QTreeWidgetItem* child = new QTreeWidgetItem(QStringList()<<name2);
        child->setIcon(0, QIcon("D:/1.jpg"));
        child->setCheckState(1, Qt::Checked);
        root->addChild(child);
    }


    QFileInfoList file_list=dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);   //获取当前所有目录

    for(int i = 0; i != folder_list.size(); i++)         //自动递归添加各目录到上一级目录
    {

             QString namepath = folder_list.at(i).absoluteFilePath();    //获取路径
             QFileInfo folderinfo= folder_list.at(i);
             QString name=folderinfo.fileName();      //获取目录名
             QTreeWidgetItem* childroot = new QTreeWidgetItem(QStringList()<<name);
             childroot->setIcon(0, QIcon("D:/1.jpg"));
             childroot->setCheckState(1, Qt::Checked);
             root->addChild(childroot);              //将当前目录添加成path的子项
             QFileInfoList child_file_list = allfile(childroot,namepath);          //进行递归
             file_list.append(child_file_list);
             file_list.append(name);
     }
    return file_list;
}

void MainWindow::on_pushButton_clicked()//遍历文件夹的路径选择
{
    QString file_path = QFileDialog::getExistingDirectory(this, "请选择文件夹路径...", "./");
    ui->textBrowser->setText(file_path);
    lujing=file_path;
    QString rootpath=lujing;
       QTreeWidgetItem* root = new QTreeWidgetItem(ui->treeWidget);
      root->setIcon(0, QIcon("D:/1.jpg"));
      root->setCheckState(1, Qt::Checked);
      root->setText(0,lujing);
      QFileInfoList lists= allfile(root,rootpath);        //遍历文件夹
}
void MainWindow::all()
{
    QTreeWidgetItem* root = new QTreeWidgetItem(ui->treeWidget);
   root->setIcon(0, QIcon("D:/1.jpg"));
   root->setCheckState(1, Qt::Checked);
   root->setText(0,lujing);
     QFileInfoList lists= allfile(root,lujing);        //遍历文件夹
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
        QAction *pDelAction_4 = new QAction(QStringLiteral("打开"), this);
        QAction *m_pact= new QAction(QStringLiteral("新增文件夹"), this);
        QAction *pDelAction_1 = new QAction(QStringLiteral("新增文件"), this);

        QAction *pDelAction_2 = new QAction(QStringLiteral("删除"), this);

        QAction *pDelAction_3 = new QAction(QStringLiteral("重命名"), this);
        QMenu *popMenu = new QMenu(this);//定义右键弹出菜单
        popMenu->addAction(m_pact);//往菜单内添加QAction
        popMenu->addAction(pDelAction_1);//往菜单内添加QAction
        popMenu->addAction(pDelAction_2);//往菜单内添加QAction
        popMenu->addAction(pDelAction_3);//往菜单内添加QAction

         popMenu->addAction(pDelAction_4);//往菜单内添加QAction
        // QAction *deleAction = cmenu->addAction("Delete");//另一种创建方式
         connect(pDelAction_4 , SIGNAL(triggered()), this, SLOT(openfile()));
        connect(m_pact, SIGNAL(triggered()), this, SLOT(newfile()));
        connect(pDelAction_1 , SIGNAL(triggered()), this, SLOT(newwenjia()));
        connect(pDelAction_2 , SIGNAL(triggered()), this, SLOT(deletes()));
        connect(pDelAction_3, SIGNAL(triggered()), this, SLOT(rename()));
        popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置



}
void MainWindow::openfile()
{
    QString filename=treewway;
    QFile file(filename);

    if (file.exists())

    {

    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
    }

}
//新建文件夹
void MainWindow::newfile()
{

      QTreeWidgetItem * currentItem = ui->treeWidget->currentItem();//获取当前节点
      QString name=ui->textEdit->toPlainText();
      QDir *temp = new QDir;
      QString lu=treewway+"/"+name;
      temp->mkpath(lu);
      newpulic(currentItem);



}
//删除文件
void MainWindow::deletes()
{

    QTreeWidgetItem * currentItem = ui->treeWidget->currentItem();//获取当前节点
    QString lu=treewway;
    QFileInfo file(lu);
    if (file.isFile())//如果是文件
            QFile::remove(lu);
        else if (file.isDir())//如果是文件夹
        {
            QDir qDir(lu);
            qDir.removeRecursively();
        }
    delete currentItem;



}
//新建文件
void MainWindow::newwenjia()
{

    QTreeWidgetItem * currentItem = ui->treeWidget->currentItem();//获取当前节点
     QString name=ui->textEdit->toPlainText();
    QString lu=treewway;
    QFile file(lu + "/"+name);
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    newpulic(currentItem);

//    QStringList cfile;
//    cfile<<name;
//    QTreeWidgetItem* cfile1=new QTreeWidgetItem(QStringList(cfile));
//    currentItem->addChild(cfile1);
   // all();
    ui->treeWidget->update();

}
//重命名
void MainWindow::rename()
{

    QTreeWidgetItem * currentItem = ui->treeWidget->currentItem();//获取当前节点
    QString lu=treewway;
    QString newfile=lu;
    int addr;
    QString name= ui->textEdit->toPlainText();
    for(int i=newfile.length()-1;i>=0;i--)
    {
        if(newfile[i]=="/")
        {
            addr=i;
            break;
        }
    }
    addr++;
    QString filename=ui->textEdit->toPlainText();
    for(int j=0;j<filename.length();j++)
    {
        newfile[addr]=filename[j];
        addr++;
    }
    QFileInfo file(lu);
            QDir dirOld(lu);
            dirOld.rename(lu, newfile);
            currentItem->setText(0,name);

}
//公共新建端子
void MainWindow::newpulic(QTreeWidgetItem * currentItem)
{
    QString name=ui->textEdit->toPlainText();
 //     bool ok = folder->mkpath(treewway+"/"+name);
 //     all();
    QStringList cfile;
    cfile<<name;
    QTreeWidgetItem* cfile1=new QTreeWidgetItem(QStringList(cfile));
    currentItem->addChild(cfile1);
}
//获取点击的路径
void MainWindow::checkself(QTreeWidgetItem* item ,int c)
{
    QStringList filepath;
         QTreeWidgetItem *itemfile=item; //获取被点击的item
         while(itemfile!=NULL)
         {
             filepath<<itemfile->text(0); //获取itemfile名称
             itemfile=itemfile->parent(); //将itemfile指向父item
         }
         QString strpath;
         for(int i=(filepath.size()-1);i>=0;i--) //QStringlist类filepath反向存着初始item的路径
         { //将filepath反向输出，相应的加入’/‘
             strpath+=filepath.at(i);
             if(i!=0)
                 strpath+="/";
         }
         //ui->textBrowser_2->setText(strpath);
         treewway=strpath;
       //  ui->plainTextEdit->insertPlainText(strpath); 将路径显示到plainTextEdit中
}



//实现拖拽
//鼠标拖动操作
void MainWindow::setDragAndDrop(QTreeWidget &w) {
    w.setDragEnabled(true);              // 启用拖动
    w.viewport()->setAcceptDrops(true); // viewport 接受放下动作，默认是复制操作
    w.showDropIndicator();               // 设置放下指示
    w.setDragDropMode(QTreeWidget::InternalMove);// 内部 移动


}

//鼠标选择节点
void MainWindow::setSelectionMode(QTreeWidget &w) {
//    w.setSelectionBehavior(QTreeWidget::SelectRows);     // 选择行
//    w.setSelectionMode(QTreeWidget::ContiguousSelection);// 连续选择
}
//movefile
void MainWindow::movefile()
{

    QString old_name=treewway;
    QStringList filepath;
         QTreeWidgetItem *itemfile=ui->treeWidget->currentItem(); //获取被点击的item
         while(itemfile!=NULL)
         {
             filepath<<itemfile->text(0); //获取itemfile名称
             itemfile=itemfile->parent(); //将itemfile指向父item
         }
         QString strpath;
         for(int i=(filepath.size()-1);i>=0;i--) //QStringlist类filepath反向存着初始item的路径
         { //将filepath反向输出，相应的加入’/‘
             strpath+=filepath.at(i);
             if(i!=0)
                 strpath+="/";
         }
    QString new_name=strpath;



}
