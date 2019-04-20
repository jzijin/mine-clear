#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>
#include <queue>
#include <random>
#include <QTime>
using namespace std;

constexpr auto ROW_STEP = 40;
constexpr auto COL_STEP = 40;
constexpr auto LCLICKED = 2;
constexpr auto RCLICKED = 1;
constexpr auto ISMIME = 10;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
private:
    int ROW; // 行数
    int COL; // 列数
    int WIDTH; // 宽度
    int HEIGHT; // 高度
    int MIME_NUM;   // 需要生成几个雷
    bool is_start;
    int count; // 统计到目前为止 一共有多少个格子被点击

    vector<vector<int>> flag;
    vector<vector<int>> count_mime_flag;

    // 统计这个格点是否被加进队列 用于广度优先搜索
    vector<vector<int>> is_pushed;

    // 记录中键点击的下一些节点
    queue<QPoint> next_points;
    vector<vector<int>> grid; // 棋盘
    vector<vector<int>> marked; // 棋盘上的地雷
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    //绘图事件
    void paintEvent(QPaintEvent *event);

    // 鼠标响应事件
    void mousePressEvent(QMouseEvent* event);

    // 画线
    void paintLine(QPainter &p);

    // 画其他信息地雷标志 地雷 数字
    void paintText(QPainter &p);

    // 初始化棋盘和地雷
    void init_mime();

    // 鼠标左键点击事件
    void lbutton_click(int row, int col);

    // 鼠标右键点击事件
    void rbutton_click(int row, int col);

    // 鼠标中键点击事件
    void mbutton_click(int row, int col);

    void get_next_points(int row, int col);

    // 显示成功失败消息
    void message(int succ);

private slots:
    // start 按钮槽函数
    void on_pushButton_start_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
