#pragma once
#include <iostream>  
#include <string>
#include <vector>
#include <queue>
#include <random>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>  
using namespace std;
using namespace cv;
constexpr auto WIDTH = 600;
constexpr auto HEIGHT = 600; // TODO: 设置字体大小的自动适应得到比较好的效果
//constexpr auto MIME = 10; // 10 这个数字代表雷;
constexpr auto ROW = 10;
constexpr auto COL = 10; // 定义行和列;
constexpr auto NUM = 20; // 需要生成几个雷
constexpr auto ROW_STEP = WIDTH / ROW;
constexpr auto COL_STEP = HEIGHT / COL;

class Mime
{
public:
	Mime(string wname);
	~Mime();

private:
	int count; // 统计到目前为止 一共有多少个格子被点击
	vector<vector<int>> flag;
	vector<vector<int>> count_flag;
	vector<vector<int>> is_pushed; // 统计这个格点是否被加进队列
	
	queue<Point> next_points; // 记录中键点击的下一些节点
	//vector<Point> mime; // 记录所有地雷的位置
	vector<vector<int>> grid; // 创建棋盘
	vector<vector<int>> marked; // 标志是否是一个雷
	string window_name; // 窗口名字
	Mat img;  // 图形矩阵

	// 画棋盘
	void drawLine(string windowname, Mat name); 

	// 初始化棋盘和地雷
	void init_mime();

	// 鼠标事件回调函数
	void _on_Mouse(int events, int x, int y);
	static void onMouse(int events, int x, int y, int, void* userdata);
	void lbutton_click(int row, int col);
	void rbutton_click(int row, int col);
	void mbutton_click(int row, int col);
	void get_next_points(int row, int col);
	void message(int succ);
};


