#include "mainwindow.h"
//#include "./ui_mainwindow.h"

#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QVector>
#include <QQueue>
#include <QDebug>

#include <cmath> // for pow()
#include <queue>
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    resize(1000, 600);

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

    style();

    // checking the coordinates
//    QGraphicsScene *scene = new QGraphicsScene(this);
//    QPen pen(Qt::yellow);
//    QPen pen2(Qt:: green);
//    scene->addEllipse(0, 0, 60, 60, pen);
//    scene->addEllipse(-100, -100, 60, 60);
//    scene->addEllipse(100, 100, 60, 60, pen2);
//    graphicsView->setScene(scene);



    connect(displayButton, &QPushButton::clicked, this, &MainWindow::displayTree);

}


MainWindow::~MainWindow()
{

    delete graphicsView;
    delete traversalInput;
    delete displayButton;
    //delete ui;
}

void MainWindow::style()
{
    traversalInput->setPlaceholderText("Enter text here");

    // Apply styles for QGraphicsView
    graphicsView->setStyleSheet("QGraphicsView {"
                                "background-color: #254041;"
                                "border: 2px solid #3daee9;"
                                "border-radius: 10px;"
                                "}");

    // Apply styles for QLineEdit
    traversalInput->setStyleSheet("QLineEdit {"
                                  "border: 2px solid #3daee9;"
                                  "border-radius: 5px;"
                                  "margin: 0 30px 0;"
                                  "padding: 15px;"
                                  "}");

    // Apply styles for QPushButton
    displayButton->setStyleSheet("QPushButton {"
                                 "background-color: #3daee9;"
                                 "border: 2px solid #3daee9;"
                                 "font-size: 15px;"
                                 "font-weight: bold;"
                                 "border-radius: 5px;"
                                 "color: #212f3d;"
                                 "padding: 15px;"
                                 "margin: 0px 400px 0px;"
                                 "}");
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


int findTreeHeight(Node* root)
{
    if (root == NULL) return 0;
    return std::max(findTreeHeight(root->left), findTreeHeight(root->right)) + 1;
}

void dfs(Node* root, int left, int right, int height, vector<vector<int>> &ans)
{
    if (root == NULL) return;
    if (left > right) return;
    int mid = (left + right )/2;
    ans[height][mid] = root->data;
    dfs(root->left, left, mid-1, height+1, ans);
    dfs(root->right, mid+1, right, height+1, ans);
}

void MainWindow::displayTree()
{
    QString input = traversalInput->text();
    QStringList values = input.split(' ');


    Node* root = createNode(values[0].toInt());
    for (int i = 1; i < values.size(); ++i)
        insertLevelOrder(root, values[i].toInt());

    // 1 : find the height
    int h = findTreeHeight(root);

    // 2: Find the width if the matrix
    int w = std::pow(2, h)-1;

    qDebug() << "height is " << h << " width is : " << w;

    // Find the 2-D matrix
    vector<vector<int>> ans(h, vector<int> (w, INT_MIN));
    dfs(root, 0, w-1, 0, ans);


    // print the 2-d matrix
    for (int i=0; i<ans.size(); i++) {
        for (int j=0; j<ans[i].size(); j++) {
            cout << ans[i][j] << " ";
        }
        cout << endl;
    }

    // display the nodes using this 2d matrix

    QBrush brush(Qt::yellow);
    QBrush brushNull(Qt::lightGray);



    // size of node : 60*60 leave a gap of 60 when iterating
    QGraphicsScene* scene = new QGraphicsScene(this);

    int y = 0;
    for (int i=0; i<ans.size(); i++) {
        int x = 0;
        for (int j=0; j<ans[i].size(); j++) {
            if (ans[i][j] == -1) {
                QGraphicsEllipseItem* ellipse = scene->addEllipse(x, y, 60, 60, Qt::NoPen, brushNull);
                QGraphicsSimpleTextItem* text = scene->addSimpleText("N");
                text->setPos(x + 25, y + 20);
            } else if (ans[i][j] != INT_MIN) {
                QGraphicsEllipseItem* ellipse = scene->addEllipse(x, y, 60, 60, Qt::NoPen, brush);
                QGraphicsSimpleTextItem* text = scene->addSimpleText(QString::number(ans[i][j]));
                text->setPos(x + 25, y + 20);
            }
            x += 60;
        }
        y += 100;
    }
    graphicsView->setScene(scene);


    // Clear all the memory given to the tree
    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int sz = q.size();

        for (int i=0; i<sz; i++) {
            Node *front = q.front();
            q.pop();

            if (front->left) q.push(front->left);
            if (front->right) q.push(front->right);

            delete front; // delete the memory assigned to the node
        }
    }

}
