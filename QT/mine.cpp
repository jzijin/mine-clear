#include "mine.h"
#include "ui_widget.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    is_start = false;
    ui->setupUi(this);
}

void Widget::message(int succ)
{
    if(!succ)
    {
        QMessageBox::information(NULL, "lose", "你失败了！你可以点击start按钮重新开始");
    }
    else
    {
        QMessageBox::information(NULL, "succ", "你成功清除了所有的地雷 你可以点击start按钮重新开始");
    }
    // 需要用户点击start按钮重新开始
    is_start = false;
}

void Widget::lbutton_click(int row, int col)
{
     if (!marked[row][col] && !flag[row][col]) // 如果不是地雷 且还未曾被点击
     {
         ++count; // 点中格子的加上1
         // 如果已经清除的格子数目 等于总的格子数目 那么就说明扫除了所有的地雷
         if (count == ROW * COL)
             message(1);

         // 2 代表已经点击过
         flag[row][col] = LCLICKED;
         is_pushed[row][col] = 1;

         if(grid[row][col] == 0)
             mbutton_click(row, col);
         this->update();
     }
     else if (!flag[row][col]) // 如果还没被点击
     {
         // 点中地雷
         for (auto i = 0; i < ROW; ++i)
         {
             for (auto j = 0; j < COL; ++j)
             {
                 if (marked[i][j])
                 {
                     flag[i][j] = ISMIME;
                 }

             }
         }
         this->update();
         //点中地雷游戏失败 显示失败信息
         message(0);
     }
}

void Widget::rbutton_click(int row, int col)
{
    if (!flag[row][col])
    {

        // 设置flag  1 代表用户标记为地雷
        flag[row][col] = RCLICKED;

        // 主要是方便计算 格点周围共有几个已经被用户标记为地雷的格点。
        count_mime_flag[row][col] = 1;

        // is_pushed 1 代表不需要加到 mbutton 中的队列中
        is_pushed[row][col] = 1;

        // 已经清楚不是地雷和已经标记了地雷的格点加1；
        ++count;

        // 更新界面
        this->update();

        // 如果已经知道不是地雷和已经标记为地雷的格点 等于总格点 那么用户赢得了胜利。
        if (count == ROW * COL)
            message(1);
    }
    else if (flag[row][col] == 1)
    {

        // flag重新设置为未点击状态
        flag[row][col] = 0;

        // is_pushed 0 可以进入mbutton中的队列
        is_pushed[row][col] = 0;

        // 主要是方便计算 格点周围共有几个已经被用户标记为地雷的格点。
        count_mime_flag[row][col] = 0;

        // 取消标记 已经清楚的格子数目减1
        --count;

        this->update();

    }
}

void Widget::mbutton_click(int row, int col)
{
    if (flag[row][col])
    {
        get_next_points(row, col);
        // 广度优先搜索
        while (!next_points.empty())
        {
            QPoint tmp = next_points.front();
            next_points.pop();
            lbutton_click(tmp.x(), tmp.y());
            // 得到这个格点的周围格点
            get_next_points(tmp.x(), tmp.y());
        }
    }
}

void Widget::get_next_points(int row, int col)
{
    // 得到以此点为中心的地雷数目
    int mime_num = grid[row][col];

    // 统计以此点为中心的周围格子的标志总数
    int flag_count = 0;

    // 左上点
    if (row == 0 && col == 0)
    {
        flag_count = count_mime_flag[row + 1][col] + count_mime_flag[row + 1][col + 1] + count_mime_flag[row][col + 1];

        if (flag_count == mime_num)
        {
            if (!is_pushed[row + 1][col])
            {
                next_points.push(QPoint(row + 1, col));
                is_pushed[row + 1][col] = 1;
            }
            if (!is_pushed[row + 1][col+1])
            {
                next_points.push(QPoint(row + 1, col + 1));
                is_pushed[row + 1][col+1] = 1;
            }
            if (!is_pushed[row][col+1])
            {
                next_points.push(QPoint(row, col + 1));
                is_pushed[row][col+1] = 1;
            }
        }
    }
    // 右上点
    else if (row == 0 && col == COL - 1)
    {
        flag_count = count_mime_flag[row][col - 1] + count_mime_flag[row + 1][col - 1] + count_mime_flag[row + 1][col];
        if (flag_count == mime_num) {

            if (!is_pushed[row][col - 1])
            {
                next_points.push(QPoint(row, col - 1));
                is_pushed[row][col - 1] = 1;
            }

            if (!is_pushed[row+1][col - 1])
            {
                next_points.push(QPoint(row + 1, col - 1));
                is_pushed[row+1][col - 1] = 1;
            }

            if (!is_pushed[row+1][col])
            {
                next_points.push(QPoint(row + 1, col));
                is_pushed[row + 1][col] = 1;
            }
        }
    }

    // 左下点
    else if (row == ROW - 1 && col == 0)
    {
        flag_count = count_mime_flag[row][col + 1] + count_mime_flag[row - 1][col] + count_mime_flag[row - 1][col + 1];
        if (flag_count == mime_num) {

            if (!is_pushed[row][col + 1])
            {
                next_points.push(QPoint(row, col + 1));
                is_pushed[row][col+1] = 1;
            }

            if (!is_pushed[row-1][col])
            {
                next_points.push(QPoint(row - 1, col));
                is_pushed[row-1][col] = 1;
            }

            if (!is_pushed[row - 1][col+1])
            {
                next_points.push(QPoint(row - 1, col + 1));
                is_pushed[row - 1][col+1] = 1;
            }
        }
    }
    // 右下点
    else if (row == ROW - 1 && col == COL - 1)
    {
        flag_count = count_mime_flag[row][col - 1] + count_mime_flag[row - 1][col] + count_mime_flag[row - 1][col - 1];
        if (flag_count == mime_num) {

            if (!is_pushed[row][col - 1])
            {
                next_points.push(QPoint(row, col - 1));
                is_pushed[row][col - 1] = 1;
            }

            if (!is_pushed[row-1][col])
            {
                next_points.push(QPoint(row - 1, col));
                is_pushed[row-1][col] = 1;
            }

            if (!is_pushed[row - 1][col-1])
            {
                next_points.push(QPoint(row - 1, col - 1));
                is_pushed[row - 1][col-1] = 1;
            }
        }
    }
    // 上边
    else if (row == 0)
    {
        flag_count = count_mime_flag[row][col - 1] + count_mime_flag[row][col + 1] + count_mime_flag[row + 1][col] + count_mime_flag[row + 1][col - 1] + count_mime_flag[row + 1][col + 1];
        if (flag_count == mime_num) {

            if (!is_pushed[row][col - 1])
            {
                next_points.push(QPoint(row, col - 1));
                is_pushed[row][col - 1] = 1;
            }

            if (!is_pushed[row][col + 1])
            {
                next_points.push(QPoint(row, col + 1));
                is_pushed[row][col + 1] = 1;
            }
            if (!is_pushed[row+1][col])
            {
                next_points.push(QPoint(row + 1, col));
                is_pushed[row+1][col] = 1;
            }

            if (!is_pushed[row + 1][col-1])
            {
                next_points.push(QPoint(row + 1, col - 1));
                is_pushed[row + 1][col-1] = 1;
            }

            if (!is_pushed[row + 1][col + 1])
            {
                next_points.push(QPoint(row + 1, col + 1));
                is_pushed[row + 1][col + 1] = 1;
            }
        }
    }
    // 下边
    else if (row == ROW - 1)
    {
        flag_count = count_mime_flag[row][col - 1] + count_mime_flag[row][col + 1] + count_mime_flag[row - 1][col] + count_mime_flag[row - 1][col - 1] + count_mime_flag[row - 1][col + 1];
        if (flag_count == mime_num) {
            if (!is_pushed[row][col - 1])
            {
                next_points.push(QPoint(row, col - 1));
                is_pushed[row][col - 1] = 1;
            }

            if (!is_pushed[row][col + 1])
            {
                next_points.push(QPoint(row, col + 1));
                is_pushed[row][col + 1] = 1;
            }

            if (!is_pushed[row-1][col])
            {
                next_points.push(QPoint(row - 1, col));
                is_pushed[row-1][col] = 1;
            }

            if (!is_pushed[row - 1][col-1])
            {
                next_points.push(QPoint(row - 1, col - 1));
                is_pushed[row - 1][col-1] = 1;
            }

            if (!is_pushed[row - 1][col + 1])
            {
                next_points.push(QPoint(row - 1, col + 1));
                is_pushed[row - 1][col + 1] = 1;
            }
        }
    }
    // 左边
    else if (col == 0)
    {
        flag_count = count_mime_flag[row][col + 1] + count_mime_flag[row - 1][col] + count_mime_flag[row + 1][col] + count_mime_flag[row + 1][col + 1] + count_mime_flag[row - 1][col + 1];
        if (flag_count == mime_num) {
            if (!is_pushed[row][col + 1])
            {
                next_points.push(QPoint(row, col + 1));
                is_pushed[row][col + 1] = 1;
            }

            if (!is_pushed[row-1][col])
            {
                next_points.push(QPoint(row - 1, col));
                is_pushed[row-1][col] = 1;
            }

            if (!is_pushed[row + 1][col])
            {
                next_points.push(QPoint(row + 1, col));
                is_pushed[row + 1][col] = 1;
            }

            if (!is_pushed[row + 1][col + 1])
            {
                next_points.push(QPoint(row + 1, col + 1));
                is_pushed[row + 1][col + 1] = 1;
            }

            if (!is_pushed[row - 1][col + 1])
            {
                next_points.push(QPoint(row - 1, col + 1));
                is_pushed[row - 1][col + 1] = 1;
            }
        }
    }
    // 右边
    else if (col == COL - 1)
    {
        flag_count = count_mime_flag[row][col - 1] + count_mime_flag[row - 1][col] + count_mime_flag[row + 1][col] + count_mime_flag[row + 1][col - 1] + count_mime_flag[row - 1][col - 1];
        if (flag_count == mime_num) {

            if (!is_pushed[row ][col - 1])
            {
                next_points.push(QPoint(row, col - 1));
                is_pushed[row ][col - 1] = 1;
            }

            if (!is_pushed[row-1][col])
            {
                next_points.push(QPoint(row - 1, col));
                is_pushed[row-1][col] = 1;
            }

            if (!is_pushed[row + 1][col])
            {
                next_points.push(QPoint(row + 1, col));
                is_pushed[row + 1][col] = 1;
            }

            if (!is_pushed[row + 1][col-1])
            {
                next_points.push(QPoint(row + 1, col - 1));
                is_pushed[row + 1][col-1] = 1;
            }

            if (!is_pushed[row - 1][col - 1])
            {
                next_points.push(QPoint(row - 1, col - 1));
                is_pushed[row - 1][col - 1] = 1;
            }		}
    }
    // 正常情况
    else
    {
        flag_count = count_mime_flag[row - 1][col] + count_mime_flag[row - 1][col - 1] + count_mime_flag[row - 1][col + 1] + count_mime_flag[row + 1][col]
            + count_mime_flag[row + 1][col + 1] + count_mime_flag[row + 1][col - 1] + count_mime_flag[row][col + 1] + count_mime_flag[row][col - 1];
        if (flag_count == mime_num) {

            if (!is_pushed[row - 1][col])
            {
                next_points.push(QPoint(row - 1, col));
                is_pushed[row - 1][col] = 1;
            }

            if (!is_pushed[row - 1][col-1])
            {
                next_points.push(QPoint(row - 1, col - 1));
                is_pushed[row - 1][col-1] = 1;
            }

            if (!is_pushed[row - 1][col + 1])
            {
                next_points.push(QPoint(row - 1, col + 1));
                is_pushed[row - 1][col + 1] = 1;
            }

            if (!is_pushed[row + 1][col])
            {
                next_points.push(QPoint(row + 1, col));
                is_pushed[row + 1][col ] = 1;
            }

            if (!is_pushed[row + 1][col+1])
            {
                next_points.push(QPoint(row + 1, col + 1));
                is_pushed[row + 1][col+1] = 1;
            }

            if (!is_pushed[row + 1][col - 1])
            {
                next_points.push(QPoint(row + 1, col - 1));
                is_pushed[row + 1][col - 1] = 1;
            }

            if (!is_pushed[row][col + 1])
            {
                next_points.push(QPoint(row, col + 1));
                is_pushed[row][col + 1] = 1;
            }

            if (!is_pushed[row][col - 1])
            {
                next_points.push(QPoint(row, col - 1));
                is_pushed[row][col - 1] = 1;
            }
        }
    }
}


void Widget::mousePressEvent(QMouseEvent* event)
{
    // 得到坐标
    QPoint tmp = event->pos();
    int row = tmp.x() / ROW_STEP;
    int col = tmp.y() / COL_STEP;

    // 鼠标左键点击
    if(is_start && event->button() == Qt::LeftButton && row < ROW && col < COL)
    {
        lbutton_click(row, col);
    }
    // 鼠标右键点击
    else if(is_start && event->button() == Qt::RightButton && row < ROW && col < COL)
    {
        rbutton_click(row, col);
    }
    // 鼠标中键点击
    else if(is_start && event->button() == Qt::MidButton && row < ROW && col < COL)
    {
        mbutton_click(row, col);
    }
}

void Widget::paintEvent(QPaintEvent *)
{
    if(is_start == true)
    {
        // 开始绘图事件
        QPainter p(this);
        // 绘制棋盘
        paintLine(p);
        // 绘制其他信息
        paintText(p);
    }
}
void Widget::paintText(QPainter &p)
{
    for(int i=0; i<ROW; ++i)
    {
        for(int j=0; j<COL; ++j)
        {
            // 根据flag矩阵的内容绘制
            if(flag[i][j] == RCLICKED)
            {
                // 绘制地雷标记
                p.save();
                QPen pen;
                pen.setColor(QColor(0, 0, 255)); // 蓝色
                p.setPen(pen);
                p.drawLine(QPoint(i * ROW_STEP + ROW_STEP/2, j * COL_STEP), QPoint(i * ROW_STEP + ROW_STEP/2, j * COL_STEP + COL_STEP));
                p.drawLine(QPoint(i * ROW_STEP, j * COL_STEP + COL_STEP/2), QPoint(i * ROW_STEP + ROW_STEP, j * COL_STEP + COL_STEP/2));
                p.restore();

            }
            else if(flag[i][j] == LCLICKED)
            {
                // 绘制数字
                QRect rect(QPoint(i * ROW_STEP, j * COL_STEP), QPoint(i * ROW_STEP + ROW_STEP, j * COL_STEP + COL_STEP));
                QString s;
                s.setNum(grid[i][j]);
                p.save();

                QPen pen;
                pen.setColor(QColor(0, 255, 0));
                // 设置字体
                QFont font;
                font.setPixelSize(ROW_STEP);
                p.setPen(pen);
                p.setFont(font);
                p.drawText(rect, Qt::AlignCenter, s);
                p.restore();
            }
            else if(flag[i][j] == ISMIME)
            {
               // 画地雷
                p.save();
                QPen pen;
                pen.setColor(QColor(255, 0, 0)); // 红色
                p.setPen(pen);
                p.drawLine(QPoint(i * ROW_STEP, j * COL_STEP), QPoint(i * ROW_STEP + ROW_STEP, j * COL_STEP + COL_STEP));
                p.drawLine(QPoint(i * ROW_STEP + ROW_STEP, j * COL_STEP), QPoint(i * ROW_STEP, j * COL_STEP + COL_STEP));
                p.restore();
            }
        }
    }
}

void Widget::paintLine(QPainter &p)
{
    // 画竖线
    for (int i = 0; i <= ROW; ++i)
    {
        p.drawLine(QPoint(i*ROW_STEP, 0), QPoint(i*ROW_STEP, HEIGHT));

    }
    // 画横线
    for (int j = 0; j <= COL; ++j)
    {
        p.drawLine(QPoint(0, j*COL_STEP), QPoint(WIDTH, j*COL_STEP));
    }
}
Widget::~Widget()
{
    delete ui;
}

void Widget::init_mime()
{
    // 设置随机种子
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    int total = 0; // 记录地雷的个数
    while (1)   // 随机生成指定个数的地雷
    {
        auto row = qrand() % ROW;
        auto col = qrand() % COL;
        if (!marked[row][col])
        {
            marked[row][col] = 1;
            ++total;
        }
        if (total == MIME_NUM) // 生成的地雷数目等于指定的地雷数目 退出
            break;
    }

    // 初始化grid
    for (int i = 0; i < ROW; ++i)
    {
        for (int j = 0; j < COL; ++j)
        {
            if (marked[i][j] == 1)
            {
                grid[i][j] = marked[i][j];
            }
            // 计算以此格点周围的地雷数
            else if (i == 0 && j == 0)
            {
                grid[i][j] = marked[i + 1][j] + marked[i + 1][j + 1] + marked[i][j + 1];
            }
            else if (i == 0 && j == COL - 1)
            {
                grid[i][j] = marked[i][j - 1] + marked[i + 1][j - 1] + marked[i + 1][j];
            }
            else if (i == ROW - 1 && j == 0)
            {
                grid[i][j] = marked[i][j + 1] + marked[i - 1][j] + marked[i - 1][j + 1];
            }
            else if (i == ROW - 1 && j == COL - 1)
            {
                grid[i][j] = marked[i][j - 1] + marked[i - 1][j] + marked[i - 1][j - 1];
            }
            else if (i == 0)
            {
                grid[i][j] = marked[i][j - 1] + marked[i][j + 1] + marked[i + 1][j] + marked[i + 1][j - 1] + marked[i + 1][j + 1];
            }
            else if (i == ROW - 1)
            {
                grid[i][j] = marked[i][j - 1] + marked[i][j + 1] + marked[i - 1][j] + marked[i - 1][j - 1] + marked[i - 1][j + 1];
            }
            else if (j == 0)
            {
                grid[i][j] = marked[i][j + 1] + marked[i - 1][j] + marked[i + 1][j] + marked[i + 1][j + 1] + marked[i - 1][j + 1];
            }
            else if (j == COL - 1)
            {
                grid[i][j] = marked[i][j - 1] + marked[i - 1][j] + marked[i + 1][j] + marked[i + 1][j - 1] + marked[i - 1][j - 1];
            }
            else
            {
                grid[i][j] = marked[i - 1][j] + marked[i - 1][j - 1] + marked[i - 1][j + 1] + marked[i + 1][j]
                    + marked[i + 1][j + 1] + marked[i + 1][j - 1] + marked[i][j + 1] + marked[i][j - 1];
            }


        }
    }

}

void Widget::on_pushButton_start_clicked()
{
    int hard_leval = ui->comboBox_leval->currentIndex();
    // 简单难度
    if(hard_leval == 0)
    {
        ROW = 15;
        COL = 15;
        MIME_NUM = 20;
        WIDTH = ROW * ROW_STEP;
        HEIGHT = COL * COL_STEP;
    }
    else if(hard_leval == 1)
    {
        ROW = 20;
        COL = 20;
        MIME_NUM = 50;
        WIDTH = ROW * ROW_STEP;
        HEIGHT = COL * COL_STEP;
    }
    else if(hard_leval == 2)
    {
        ROW = 20;
        COL = 20;
        MIME_NUM = 80;
        WIDTH = ROW * ROW_STEP;
        HEIGHT = COL * COL_STEP;
    }

    // 设置开始标志
    is_start = true;
    count = 0;

    // 初始化
    grid = vector<vector<int>>(ROW, vector<int>(COL));
    marked = vector<vector<int>>(ROW, vector<int>(COL));
    flag = vector<vector<int>>(ROW, vector<int>(COL));
    count_mime_flag = vector<vector<int>>(ROW, vector<int>(COL));
    is_pushed = vector<vector<int>>(ROW, vector<int>(COL));

    // 初始化棋盘和地雷
    init_mime();

    // 绘图
    this->update();
}
