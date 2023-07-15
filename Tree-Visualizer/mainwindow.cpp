#include "mainwindow.h"
//#include "./ui_mainwindow.h"

#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QVector>
#include <QQueue>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    resize(800, 600);

    setWindowTitle("Tree Visualizer");


    // Creating the UI
    graphicsView = new QGraphicsView(this);
    traversalInput = new QLineEdit(this);
    displayButton = new QPushButton("Display Tree", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(graphicsView);
    layout->addWidget(traversalInput);
    layout->addWidget(displayButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(displayButton, &QPushButton::clicked, this, &MainWindow::displayTree);

}

MainWindow::~MainWindow()
{
    //
}


struct Node {
    int data;
    Node* left;
    Node* right;
};

Node* createNode(int data) {
    Node* node = new Node;
    node->data = data;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

void insertLevelOrder(Node* root, int data) {
    if (!root)
        return;

    QQueue<Node*> queue;
    queue.enqueue(root);

    while (!queue.isEmpty()) {
        Node* temp = queue.front();
        queue.dequeue();

        if (!temp->left) {
            temp->left = createNode(data);
            break;
        } else
            queue.enqueue(temp->left);

        if (!temp->right) {
            temp->right = createNode(data);
            break;
        } else
            queue.enqueue(temp->right);
    }
}

void levelOrderTraversal(Node* root, QVector<int>& nodes) {
    if (!root)
        return;

    QQueue<Node*> queue;
    queue.enqueue(root);

    while (!queue.isEmpty()) {
        Node* temp = queue.front();
        queue.dequeue();
        nodes.append(temp->data);

        if (temp->left)
            queue.enqueue(temp->left);

        if (temp->right)
            queue.enqueue(temp->right);
    }
}


void MainWindow::displayTree()
{
    QString input = traversalInput->text();
    QStringList values = input.split(' ');


    Node* root = createNode(values[0].toInt());
    for (int i = 1; i < values.size(); ++i)
        insertLevelOrder(root, values[i].toInt());

    QGraphicsScene* scene = new QGraphicsScene(this);

    QVector<int> nodes;
    levelOrderTraversal(root, nodes);

    int level = 0;
    int nodesInLevel = 1;
    int nodeIndex = 0;

//    QPen pen(Qt::white);
//    pen.setWidth(4);
//    ellipse->setPen(pen);  // We are setting in the addEllipse function directly

    QBrush brush(Qt::yellow);
//    ellipse->setBrush(brush);

    while (nodeIndex < nodes.size()) {
        int startX = -(nodesInLevel / 2) * 80;
        for (int i = 0; i < nodesInLevel && nodeIndex < nodes.size(); ++i) {
            int x = startX + 2*i * 80;
            int y = level * 100;

            QGraphicsEllipseItem* ellipse = scene->addEllipse(x, y, 60, 60, Qt::NoPen, brush);

            QGraphicsSimpleTextItem* text = scene->addSimpleText(QString::number(nodes[nodeIndex]));
            text->setPos(x + 25, y + 20);

            ++nodeIndex;
        }

        ++level;
        nodesInLevel *= 2;
    }

    graphicsView->setScene(scene);
}
