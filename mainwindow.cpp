#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::ElemHasAttribute(const QDomElement &domElement)
{
    QString str = "";
    int i = 0;
    while(!domElement.hasAttribute(str))
    {
        i++;
        switch(i)
        {
        case 1: str = "index";
            break;
        case 2: str = "format";
            break;
        case 3: str = "lang";
            break;
        default:
            return QString("");
        }
    }
    return domElement.hasAttribute(str) ?
                "(" + str.append(": ").append(domElement.attribute(str, "")) + ")" : QString("");
}

void MainWindow::ParserXML(const QDomNode &node, QStack<QTreeWidgetItem*> widgetStack)
{
    QDomNode domNode = node.firstChild();

    while(!domNode.isNull())
    {
        if(domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull())
            {
                QString tagStr = domElement.tagName(), attrStr = ElemHasAttribute(domElement);
                QStringList strList;

                if(attrStr!="")
                {
                    tagStr.append(" ").append(attrStr);
                }

                if(!domElement.firstChildElement().isNull())
                {
                    //qDebug() << qPrintable(tagStr);
                    strList << tagStr;
                    QTreeWidgetItem* item = new QTreeWidgetItem(strList);

                    ui->treeWidget1->addTopLevelItem(item);

                    if(widgetStack.count() != 0)
                    {
                        widgetStack.top()->addChild(item);
                    }
                    widgetStack.push(item);
                }
                else
                {
                    //qDebug() << "\t" << qPrintable(tagStr) << " - " << qPrintable(domElement.text());
                    strList << tagStr + " - " + domElement.text();
                    QTreeWidgetItem* item = new QTreeWidgetItem(strList);

                    if(widgetStack.count() != 0)
                    {
                        widgetStack.top()->addChild(item);
                    }
                    widgetStack.push(item);
                    widgetStack.pop();
                }
            }
        }
        ParserXML(domNode, widgetStack);
        domNode = domNode.nextSibling();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QFile file(ui->lineEdit->text());
    QDomDocument domDoc;
    QStack<QTreeWidgetItem*> widgetStack;
    ui->treeWidget1->clear();
    if(file.open(QIODevice::ReadOnly))
    {
        ui->treeWidget1->setHeaderLabel(ui->lineEdit->text());

        if(domDoc.setContent(&file))
        {
            QDomElement domElement = domDoc.documentElement();
            ParserXML(domElement, widgetStack);
        }
        file.close();
    }
    else
    {
        ui->treeWidget1->setHeaderLabel("File error!");
    }
}
